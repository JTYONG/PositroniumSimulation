#include "PsSimDetectorConstruction.hh"
#include "PsSimPhysicsList.hh"
#include "PsSimActionInitialization.hh"
#include "PsSimOutputMessenger.hh"

#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4String.hh"

int main(int argc, char** argv) {
    auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::MT);

    // Output messenger created before action initialization so both share it
    auto* outMsg = new PsSimOutputMessenger();

    runManager->SetUserInitialization(new PsSimDetectorConstruction());
    runManager->SetUserInitialization(new PsSimPhysicsList());
    runManager->SetUserInitialization(new PsSimActionInitialization(outMsg));

    G4UImanager* UI = G4UImanager::GetUIpointer();
    G4String macro = (argc == 2) ? argv[1] : "sim.mac";
    UI->ApplyCommand("/control/execute " + macro);

    delete runManager;
    delete outMsg;
    return 0;
}
