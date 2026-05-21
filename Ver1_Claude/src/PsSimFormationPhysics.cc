#include "PsSimFormationPhysics.hh"
#include "PsFormationProcess.hh"
#include "PsFormationMessenger.hh"

#include "G4Positron.hh"
#include "G4Gamma.hh"
#include "G4ProcessManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

PsSimFormationPhysics::PsSimFormationPhysics()
  : G4VPhysicsConstructor("PsFormationPhysics") {
    fOPsLifetime = 2. * ns;
    fMessenger   = new PsFormationMessenger(this);
}

PsSimFormationPhysics::~PsSimFormationPhysics() {
    delete fMessenger;
}

void PsSimFormationPhysics::ConstructParticle() {
    G4Positron::PositronDefinition();
    G4Gamma::GammaDefinition();
}

void PsSimFormationPhysics::ConstructProcess() {
    auto* particleIterator = GetParticleIterator();
    particleIterator->reset();

    while ((*particleIterator)()) {
        G4ParticleDefinition* particle = particleIterator->value();
        if (particle->GetParticleName() != "e+") continue;

        G4ProcessManager* pManager = particle->GetProcessManager();
        G4ProcessVector*  pList    = pManager->GetProcessList();

        // Remove the standard G4eplusAnnihilation (process name "annihil")
        G4VProcess* annihil = nullptr;
        for (std::size_t i = 0; i < pList->size(); ++i) {
            if ((*pList)[i]->GetProcessName() == "annihil") {
                annihil = (*pList)[i];
                break;
            }
        }
        if (annihil) pManager->RemoveProcess(annihil);

        // Insert our custom at-rest process with current physics parameters
        auto* psProc = new PsFormationProcess(fOPsProb, fOPsLifetime);
        pManager->AddRestProcess(psProc);
        break;
    }
}
