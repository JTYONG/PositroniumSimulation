#pragma once

#include "G4VRestProcess.hh"
#include "globals.hh"

class G4SimplePositronAtRestModel;
class G4OrePowellAtRestModel;

class PsFormationProcess : public G4VRestProcess {
public:
    PsFormationProcess(G4double oPsProb, G4double oPsLifetime);
    ~PsFormationProcess() override;

    G4bool IsApplicable(const G4ParticleDefinition& p) override;

    G4VParticleChange* AtRestDoIt(const G4Track& track,
                                   const G4Step&  step) override;

protected:
    G4double GetMeanLifeTime(const G4Track&, G4ForceCondition*) override;

private:
    G4double fOPsProb;
    G4double fOPsLifetime;  // mean o-Ps lifetime in Geant4 time units

    G4SimplePositronAtRestModel* f2GammaModel = nullptr;
    G4OrePowellAtRestModel*      f3GammaModel = nullptr;
};
