#include "PsFormationProcess.hh"
#include "PsEventData.hh"

#include "G4SimplePositronAtRestModel.hh"
#include "G4OrePowellAtRestModel.hh"
#include "G4Positron.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4DynamicParticle.hh"
#include "G4ForceCondition.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"
#include "CLHEP/Random/RandExponential.h"

PsFormationProcess::PsFormationProcess(G4double oPsProb, G4double oPsLifetime)
  : G4VRestProcess("PsFormation", fElectromagnetic),
    fOPsProb(oPsProb),
    fOPsLifetime(oPsLifetime) {
    f2GammaModel = new G4SimplePositronAtRestModel();
    f3GammaModel = new G4OrePowellAtRestModel();
}

PsFormationProcess::~PsFormationProcess() {
    delete f2GammaModel;
    delete f3GammaModel;
}

G4bool PsFormationProcess::IsApplicable(const G4ParticleDefinition& p) {
    return &p == G4Positron::Positron();
}

G4double PsFormationProcess::GetMeanLifeTime(const G4Track&, G4ForceCondition* cond) {
    *cond = NotForced;
    return 0.;  // fires immediately when positron comes to rest
}

G4VParticleChange* PsFormationProcess::AtRestDoIt(const G4Track& track, const G4Step&) {
    aParticleChange.Initialize(track);
    aParticleChange.ProposeTrackStatus(fStopAndKill);
    aParticleChange.ProposeLocalEnergyDeposit(0.);

    const G4Material* material = track.GetMaterial();
    G4ThreeVector     pos      = track.GetPosition();
    G4double          t0       = track.GetGlobalTime();

    G4String volName = track.GetVolume()->GetLogicalVolume()->GetName();
    bool inSilica    = (volName == "PorousSilica");

    std::vector<G4DynamicParticle*> secParticles;
    G4double localEdep  = 0.;
    G4double timeShift  = 0.;
    bool     oPsFormed  = false;
    double   oPsLife_ns = -1.;

    if (inSilica && G4UniformRand() < fOPsProb) {
        // ortho-Ps: sample exponential lifetime and generate 3 gammas
        timeShift  = CLHEP::RandExponential::shoot(fOPsLifetime);
        oPsFormed  = true;
        oPsLife_ns = timeShift / ns;
        f3GammaModel->SampleSecondaries(secParticles, localEdep, material);
    } else {
        // para-Ps or positron stopped outside silica: prompt 2-gamma annihilation
        f2GammaModel->SampleSecondaries(secParticles, localEdep, material);
    }

    aParticleChange.SetNumberOfSecondaries(static_cast<G4int>(secParticles.size()));
    G4double secTime = t0 + timeShift;
    for (auto* dp : secParticles) {
        auto* secTrack = new G4Track(dp, secTime, pos);
        aParticleChange.AddSecondary(secTrack);
    }

    PsEventData::Instance().SetOPs(oPsFormed, oPsLife_ns);

    return &aParticleChange;
}
