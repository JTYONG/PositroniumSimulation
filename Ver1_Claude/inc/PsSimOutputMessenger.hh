#pragma once

#include "G4UImessenger.hh"
#include "globals.hh"

class G4UIdirectory;
class G4UIcmdWithAString;

class PsSimOutputMessenger : public G4UImessenger {
public:
    PsSimOutputMessenger();
    ~PsSimOutputMessenger() override;

    void SetNewValue(G4UIcommand* cmd, G4String val) override;

    const G4String& GetRootFile() const { return fRootFile; }
    const G4String& GetTxtBase()  const { return fTxtBase;  }

private:
    G4UIdirectory*    fOutDir      = nullptr;
    G4UIcmdWithAString* fRootCmd   = nullptr;
    G4UIcmdWithAString* fTxtCmd    = nullptr;

    G4String fRootFile = "result_cache/root_file/pals_output";
    G4String fTxtBase  = "result_cache/pals_output";
};
