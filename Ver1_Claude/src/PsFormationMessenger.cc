#include "PsFormationMessenger.hh"
#include "PsSimFormationPhysics.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4SystemOfUnits.hh"

PsFormationMessenger::PsFormationMessenger(PsSimFormationPhysics* physics)
  : fPhysics(physics) {
    fOPsDir = new G4UIdirectory("/sim/oPs/");
    fOPsDir->SetGuidance("Ortho-positronium formation parameters.");

    fProbCmd = new G4UIcmdWithADouble("/sim/oPs/formationProb", this);
    fProbCmd->SetGuidance("Probability of o-Ps formation when positron stops in porous silica.");
    fProbCmd->SetParameterName("prob", false);
    fProbCmd->SetDefaultValue(0.75);
    fProbCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fLifetimeCmd = new G4UIcmdWithADoubleAndUnit("/sim/oPs/meanLifetime", this);
    fLifetimeCmd->SetGuidance("Mean o-Ps lifetime in porous silica (exponential distribution).");
    fLifetimeCmd->SetParameterName("tau", false);
    fLifetimeCmd->SetDefaultUnit("ns");
    fLifetimeCmd->SetDefaultValue(2.0);
    fLifetimeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

PsFormationMessenger::~PsFormationMessenger() {
    delete fProbCmd;
    delete fLifetimeCmd;
    delete fOPsDir;
}

void PsFormationMessenger::SetNewValue(G4UIcommand* cmd, G4String val) {
    if (cmd == fProbCmd)
        fPhysics->SetOPsProb(fProbCmd->GetNewDoubleValue(val));
    else if (cmd == fLifetimeCmd)
        fPhysics->SetOPsLifetime(fLifetimeCmd->GetNewDoubleValue(val));
}
