#pragma once

#include "G4UserRunAction.hh"
#include "globals.hh"
#include <fstream>

class PsSimOutputMessenger;
class G4Run;

class PsSimRunAction : public G4UserRunAction {
public:
    PsSimRunAction(G4bool isMaster, PsSimOutputMessenger* outMsg);
    ~PsSimRunAction() override;

    void BeginOfRunAction(const G4Run* run) override;
    void EndOfRunAction(const G4Run* run) override;

    std::ofstream& GetTxtStream() { return fTxtStream; }

private:
    G4bool                fIsMaster;
    PsSimOutputMessenger* fOutMsg;
    std::ofstream         fTxtStream;
};
