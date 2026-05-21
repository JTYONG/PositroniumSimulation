#pragma once

#include "G4VUserActionInitialization.hh"

class PsSimOutputMessenger;

class PsSimActionInitialization : public G4VUserActionInitialization {
public:
    explicit PsSimActionInitialization(PsSimOutputMessenger* outMsg);
    ~PsSimActionInitialization() override = default;

    void BuildForMaster() const override;
    void Build() const override;

private:
    PsSimOutputMessenger* fOutMsg;
};
