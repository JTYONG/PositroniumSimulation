#pragma once

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class G4ParticleDefinition;

class PsSimPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
    PsSimPrimaryGeneratorAction();
    ~PsSimPrimaryGeneratorAction() override;

    void GeneratePrimaries(G4Event* event) override;

private:
    G4ParticleGun*       fParticleGun = nullptr;
    G4ParticleDefinition* fNa22       = nullptr;  // lazy-initialised
};
