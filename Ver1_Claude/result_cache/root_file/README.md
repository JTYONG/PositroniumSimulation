# result_cache/root_file

This directory holds ROOT output files produced by the PALS simulation and the analysis macros used to inspect them.

---

## Simulation output

| File | Description |
|------|-------------|
| `pals_output.root` | Merged ROOT file — TTree `PsTree`, one row per simulated event |
| `pals_output_tN.root` | Per-worker-thread ROOT files (intermediate; kept for reference) |

### PsTree branches

| Branch | Type | Unit | Description |
|--------|------|------|-------------|
| `eventID` | int | — | Event number |
| `totalEdep` | double | MeV | Total energy deposited in scintillator |
| `nHits` | int | — | Number of hit steps in scintillator |
| `firstHitTime` | double | ns | Absolute global time of first energy deposit |
| `firstHitEdep` | double | MeV | Energy deposited in first hit |
| `firstHitPDG` | int | — | PDG code of particle making first hit |
| `oPsFormed` | int | 0/1 | Whether ortho-Ps formed in this event |
| `oPsLifetime` | double | ns | Sampled o-Ps lifetime (−1 if none formed) |
| `firstHitX` | double | mm | X position of first scintillator hit |
| `firstHitY` | double | mm | Y position of first scintillator hit |
| `firstHitZ` | double | mm | Z position of first scintillator hit |

---

## Observable1.C

### Purpose

Reads `pals_output.root` and produces two diagnostic plots that visualise the two primary observables of a PALS measurement:

1. **Hit-time spectrum** — the distribution of the first scintillator hit time for events where ortho-Ps formed and was detected (`oPsFormed==1 && nHits>0`).
2. **XZ hit-position map** — a 2D histogram showing where in the scintillator the first energy deposit occurred, projected onto the XZ plane (perpendicular to the scintillator axis Y).

### Output files

| File | Contents |
|------|----------|
| `plot_oPsHitTime.png` | 1D histogram of first hit time [ns], o-Ps events only |
| `plot_firstHitXZ.png` | 2D COLZ histogram of first hit X vs Z position [mm] |

### Usage

Run from inside the `root_file` directory so the output PNGs land here:

```bash
# default — reads pals_output.root in the current directory
root -l Observable1.C

# explicit file path
root -l 'Observable1.C("pals_output.root")'

# batch mode (no GUI window, just saves PNGs and exits)
root -l -q -b Observable1.C
```

### Physics notes

**Plot 1 — Hit-time spectrum**

`firstHitTime` is the absolute global time of the hit, which includes the Na22 radioactive decay time (~3.75-year mean lifetime sampled by Geant4) plus the positron thermalisation and o-Ps lifetime, plus the gamma travel time to the scintillator. Because of the dominant Na22 decay-time component the distribution is very broad. In a real PALS experiment this is removed by measuring the time *difference* between the 1274 keV start-signal and the annihilation stop-signal; the simulation records only the absolute time, so the histogram reflects the full folded distribution.

**Plot 2 — XZ hit-position map**

The scintillator is a cylindrical annulus with its axis along Y, inner radius 50 mm, outer radius 70 mm. Projecting onto XZ should reveal a filled annular ring between r = 50 mm and r = 70 mm. Gaps or asymmetries would indicate geometry or acceptance issues. The selection `nHits>0` includes all particle species (511 keV annihilation gammas, 1274 keV gammas from Ne22\* de-excitation) that reach the scintillator.
