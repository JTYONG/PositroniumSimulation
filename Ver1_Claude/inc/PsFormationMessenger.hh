#pragma once

#include "G4UImessenger.hh"
#include "globals.hh"

class PsSimFormationPhysics;
class G4UIdirectory;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;

class PsFormationMessenger : public G4UImessenger {
public:
    explicit PsFormationMessenger(PsSimFormationPhysics* physics);
    ~PsFormationMessenger() override;

    void SetNewValue(G4UIcommand* cmd, G4String val) override;

private:
    PsSimFormationPhysics*     fPhysics    = nullptr;
    G4UIdirectory*             fOPsDir     = nullptr;
    G4UIcmdWithADouble*        fProbCmd    = nullptr;
    G4UIcmdWithADoubleAndUnit* fLifetimeCmd= nullptr;
};
