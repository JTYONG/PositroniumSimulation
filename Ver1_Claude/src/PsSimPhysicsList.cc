#include "PsSimPhysicsList.hh"
#include "PsSimFormationPhysics.hh"

#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4SystemOfUnits.hh"

PsSimPhysicsList::PsSimPhysicsList() : G4VModularPhysicsList() {
    SetVerboseLevel(1);
    RegisterPhysics(new G4DecayPhysics());
    RegisterPhysics(new G4RadioactiveDecayPhysics());
    RegisterPhysics(new G4EmStandardPhysics_option4());
    // Must be registered after G4EmStandardPhysics_option4 so it can remove G4eplusAnnihilation
    fFormationPhysics = new PsSimFormationPhysics();
    RegisterPhysics(fFormationPhysics);
}

void PsSimPhysicsList::SetCuts() {
    SetCutsWithDefault();
}
