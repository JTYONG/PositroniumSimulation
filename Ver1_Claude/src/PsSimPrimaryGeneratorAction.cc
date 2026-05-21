#include "PsSimPrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4IonTable.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"

PsSimPrimaryGeneratorAction::PsSimPrimaryGeneratorAction() {
    fParticleGun = new G4ParticleGun(1);
    fParticleGun->SetParticleEnergy(0.);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
}

PsSimPrimaryGeneratorAction::~PsSimPrimaryGeneratorAction() {
    delete fParticleGun;
}

void PsSimPrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
    // Lazy init: G4IonTable is populated only after /run/initialize
    if (!fNa22)
        fNa22 = G4IonTable::GetIonTable()->GetIon(11, 22, 0.);

    fParticleGun->SetParticleDefinition(fNa22);
    fParticleGun->GeneratePrimaryVertex(event);
}
