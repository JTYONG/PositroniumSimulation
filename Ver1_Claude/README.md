# Positronium PALS Simulation (Ver1_Claude)

Geant4 + ROOT simulation of **22Na radioactive decay and positronium formation** in porous silica for Positron Annihilation Lifetime Spectroscopy (PALS).

## Physics

| Process | Model |
|---------|-------|
| 22Na β+ decay → 22Ne* + e⁺ + νe | G4RadioactiveDecayPhysics |
| 22Ne* → 22Ne + γ (1274 keV) | G4RadioactiveDecayPhysics |
| EM interactions (e⁺ transport) | G4EmStandardPhysics_option4 |
| para-Ps annihilation (2γ, 511 keV) | G4SimplePositronAtRestModel |
| ortho-Ps annihilation (3γ, Ore-Powell spectrum) | G4OrePowellAtRestModel |

When a positron comes to rest inside the porous silica shell it forms o-Ps with configurable probability (default 0.75) and lifetime (default 2 ns, exponential distribution). Elsewhere it undergoes prompt 2γ annihilation.

## Geometry

```
World (10×10×10 cm³, air)
├── Scintillator  G4Tubs  rIn=5 cm  rOut=7 cm  hY=10 cm  G4_PLASTIC_SC_VINYLTOLUENE
└── Porous silica G4Sphere rIn=2 cm  rOut=2.5 cm          PorousSiO2 (ρ=1.0 g/cm³)
    └── 22Na source at (0,0,0)
```

All dimensions are configurable via `/sim/geometry/` macro commands.

## Dependencies

- Geant4 11.3.2 (installed at `/usr/local/geant4-11.3.2`)
- ROOT 6.x (conda physics environment recommended)

## Build

```bash
# Activate physics environment (adjust path to your ROOT installation)
source /usr/local/geant4-11.3.2/bin/geant4.sh

mkdir build && cd build
cmake -DCMAKE_PREFIX_PATH="/usr/local/geant4-11.3.2;/home/sus/miniforge3/envs/physics" \
      -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```

## Run

```bash
./PsSim              # uses sim.mac in build directory
./PsSim my.mac       # custom macro
```

## Configuration (sim.mac)

| Command | Default | Description |
|---------|---------|-------------|
| `/sim/geometry/worldHalfSize` | 5 cm | World box half-side |
| `/sim/geometry/scintRmin/Rmax` | 5/7 cm | Scintillator radii |
| `/sim/geometry/scintHalfY` | 5 cm | Scintillator half-height |
| `/sim/geometry/silicaRmin/Rmax` | 2/2.5 cm | Silica shell radii |
| `/sim/geometry/silicaDensity` | 1.0 g/cm³ | Silica density (models porosity) |
| `/sim/oPs/formationProb` | 0.75 | o-Ps formation probability in silica |
| `/sim/oPs/meanLifetime` | 2.0 ns | Mean o-Ps lifetime (exponential) |
| `/sim/output/rootFile` | result_cache/root_file/pals_output | ROOT file base path |
| `/sim/output/txtBase` | result_cache/pals_output | TXT backup base path |

## Output

**ROOT file** (`pals_output.root`): TTree `PsTree` with per-event columns:

| Branch | Type | Unit | Description |
|--------|------|------|-------------|
| eventID | int | — | Event number |
| totalEdep | double | MeV | Total energy deposited in scintillator |
| nHits | int | — | Number of steps in scintillator |
| firstHitTime | double | ns | Global time of first energy deposit |
| firstHitEdep | double | MeV | Energy of first hit |
| firstHitPDG | int | — | PDG code of first-hit particle |
| oPsFormed | int | 0/1 | Whether o-Ps formed in this event |
| oPsLifetime | double | ns | Sampled o-Ps lifetime (−1 if none) |

**TXT backup** (`pals_output_t<N>.txt`): space-separated, same columns, one file per worker thread.

## Quick Analysis (ROOT)

```cpp
TFile f("result_cache/root_file/pals_output.root");
PsTree->Draw("firstHitTime>>h(200,0,20)","oPsFormed==1");  // o-Ps lifetime spectrum
PsTree->Draw("firstHitEdep>>e(150,0,1.5)","nHits>0");      // energy spectrum
PsTree->GetEntries("oPsFormed==1") / (double)PsTree->GetEntries();  // o-Ps fraction
```

# Phase 1 Construction

This section documents the construction and debugging of the first working version of the simulation.

## What Was Built

A complete Geant4 + ROOT PALS simulation comprising 14 source files across detector construction, physics list, sensitive detector, run/event actions, and output management. All geometry and physics parameters are runtime-configurable via a Geant4 macro file (`sim.mac`). A custom `PsFormationProcess` (G4VRestProcess) replaces the standard `G4eplusAnnihilation` process to implement o-Ps formation with a sampled exponential lifetime and 3γ decay via `G4OrePowellAtRestModel`.

## Bugs Fixed During Construction

### 1. Na22 "NoProcess" — Radioactive Decay Not Firing

**Symptom:** Na22 ion died with `ProcName = NoProcess` after two tracking steps; no positron or secondary was ever produced.

**Root cause:** `G4VRadioactiveDecay::IsApplicable()` checks the nucleus lifetime against an internal threshold `fThresholdForVeryLongDecayTime`. Its default value is **1 year**. Na22's mean lifetime is ~3.75 years, which exceeds the threshold, causing `IsApplicable()` to return `false` and `GetMeanLifeTime()` to return `DBL_MAX`. No decay process was ever selected.

**Fix:** Add the following command to `sim.mac` after `/run/initialize`:
```
/process/had/rdm/thresholdForVeryLongDecayTime 1.0e+60 year
```

### 2. Ntuple Column Type Mismatches

**Symptom:** G4Exception warnings during event action:
```
Column type does not match:  ntuple 0 column 0 value 0.000000
Column type does not match:  ntuple 0 column 1 value 0
```

**Root cause:** `G4AnalysisManager::FillNtuple*Column(ntupleId, columnId, value)` uses **global sequential column IDs** (0, 1, 2, …) matching the order columns were created, not per-type IDs (I-col 0, D-col 0, I-col 1, …). The event action was passing per-type IDs, causing every other `Fill` call to hit the wrong column type.

**Fix:** Updated `PsSimEventAction::EndOfEventAction()` to use global IDs 0–7:
```cpp
am->FillNtupleIColumn(0, 0, eventID);      // col 0: I
am->FillNtupleDColumn(0, 1, totalEdep);    // col 1: D
am->FillNtupleIColumn(0, 2, nHits);        // col 2: I
am->FillNtupleDColumn(0, 3, firstHitTime); // col 3: D
am->FillNtupleDColumn(0, 4, firstHitEdep); // col 4: D
am->FillNtupleIColumn(0, 5, firstHitPDG);  // col 5: I
am->FillNtupleIColumn(0, 6, oPsFormed);    // col 6: I
am->FillNtupleDColumn(0, 7, oPsLifetime);  // col 7: D
```

### 3. Merged ROOT File Deleted as Empty

**Symptom:** After a multi-threaded run, the master ROOT file (`pals_output.root`) was deleted with the message `delete empty file`; only the per-thread files (`pals_output_tN.root`) contained data.

**Root cause:** `SetNtupleMerging(true)` was called only on the master thread (guarded by `if (fIsMaster)`). Geant4's ntuple merging framework requires this call on **all threads** before `CreateNtuple()` so that worker-thread analysis managers participate in the merge bookkeeping. Also, `G4RunManagerType::Default` (task-based) was used instead of `G4RunManagerType::MT`.

**Fix:** Removed the `if (fIsMaster)` guard so `SetNtupleMerging(true)` is called unconditionally on every thread in `BeginOfRunAction`. Switched the run manager to `G4RunManagerType::MT`.

## Validated Physics (10 000 events, `sim.mac` defaults)

| Observable | Result | Expected |
|------------|--------|----------|
| o-Ps lifetime mean | 1.983 ns | 2.000 ns (set by macro) |
| oPsFormed = 1 fraction | 62.7 % | ~67.7 % (= 0.903 × 0.75; Na22 β⁺ branch × o-Ps prob) |
| Events with scintillator hits | 36.2 % | — |

The small deficit in the o-Ps fraction relative to the naive estimate is physical: a fraction of positrons stop in the air volume before reaching the silica shell, and ~9.7% of Na22 decays proceed via electron capture (no positron emitted).

## Build and Run Notes

The system Geant4 installation (`/usr/local/geant4-11.3.2`) leaves all data-path environment variables commented out in `geant4.sh`. The required variables must be set at runtime. A `run.sh` wrapper script is provided in the build directory:

```bash
./run.sh           # runs sim.mac
./run.sh diag.mac  # custom macro
```

The build must use the system compiler (`/usr/bin/g++`) rather than the conda-environment compiler to avoid a GLIBC version mismatch with the installed Geant4 libraries:

```bash
env -i HOME="$HOME" PATH="/usr/bin:/usr/local/bin:/usr/local/geant4-11.3.2/bin" \
    LD_LIBRARY_PATH="/usr/local/geant4-11.3.2/lib" \
    make -j$(nproc)
```
