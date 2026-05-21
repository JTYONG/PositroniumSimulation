#include "PsSimActionInitialization.hh"
#include "PsSimOutputMessenger.hh"
#include "PsSimRunAction.hh"
#include "PsSimEventAction.hh"
#include "PsSimPrimaryGeneratorAction.hh"

PsSimActionInitialization::PsSimActionInitialization(PsSimOutputMessenger* outMsg)
  : fOutMsg(outMsg) {}

void PsSimActionInitialization::BuildForMaster() const {
    SetUserAction(new PsSimRunAction(true, fOutMsg));
}

void PsSimActionInitialization::Build() const {
    SetUserAction(new PsSimPrimaryGeneratorAction());
    auto* runAction = new PsSimRunAction(false, fOutMsg);
    SetUserAction(runAction);
    SetUserAction(new PsSimEventAction(runAction));
}
