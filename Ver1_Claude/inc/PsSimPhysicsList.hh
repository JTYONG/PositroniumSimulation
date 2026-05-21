#pragma once

#include "G4VModularPhysicsList.hh"

class PsSimFormationPhysics;

class PsSimPhysicsList : public G4VModularPhysicsList {
public:
    PsSimPhysicsList();
    ~PsSimPhysicsList() override = default;

    void SetCuts() override;

private:
    PsSimFormationPhysics* fFormationPhysics = nullptr;
};
