#pragma once

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

class PsFormationMessenger;

class PsSimFormationPhysics : public G4VPhysicsConstructor {
public:
    PsSimFormationPhysics();
    ~PsSimFormationPhysics() override;

    void ConstructParticle() override;
    void ConstructProcess() override;

    void SetOPsProb(G4double p)     { fOPsProb = p; }
    void SetOPsLifetime(G4double t) { fOPsLifetime = t; }
    G4double GetOPsProb()     const { return fOPsProb; }
    G4double GetOPsLifetime() const { return fOPsLifetime; }

private:
    G4double fOPsProb     = 0.75;
    G4double fOPsLifetime = 0.;  // set to 2*ns in constructor

    PsFormationMessenger* fMessenger = nullptr;
};
