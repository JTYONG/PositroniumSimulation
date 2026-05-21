#include "PsSimOutputMessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

PsSimOutputMessenger::PsSimOutputMessenger() {
    fOutDir = new G4UIdirectory("/sim/output/");
    fOutDir->SetGuidance("Output file path commands.");

    fRootCmd = new G4UIcmdWithAString("/sim/output/rootFile", this);
    fRootCmd->SetGuidance("Base path for ROOT output file (no extension).");
    fRootCmd->SetParameterName("path", false);
    fRootCmd->SetDefaultValue("result_cache/root_file/pals_output");
    fRootCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fTxtCmd = new G4UIcmdWithAString("/sim/output/txtBase", this);
    fTxtCmd->SetGuidance("Base path for TXT backup files (thread ID and .txt appended).");
    fTxtCmd->SetParameterName("path", false);
    fTxtCmd->SetDefaultValue("result_cache/pals_output");
    fTxtCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

PsSimOutputMessenger::~PsSimOutputMessenger() {
    delete fRootCmd;
    delete fTxtCmd;
    delete fOutDir;
}

void PsSimOutputMessenger::SetNewValue(G4UIcommand* cmd, G4String val) {
    if (cmd == fRootCmd) fRootFile = val;
    else if (cmd == fTxtCmd) fTxtBase = val;
}
