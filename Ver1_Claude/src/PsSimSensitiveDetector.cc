#include "PsSimSensitiveDetector.hh"

#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"

PsSimSensitiveDetector::PsSimSensitiveDetector(const G4String& name)
  : G4VSensitiveDetector(name) {
    collectionName.insert("ScintHitsCollection");
}

void PsSimSensitiveDetector::Initialize(G4HCofThisEvent* hce) {
    fHitsCollection = new PsSimHitsCollection(SensitiveDetectorName, collectionName[0]);
    if (fHCID < 0)
        fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
    hce->AddHitsCollection(fHCID, fHitsCollection);
}

G4bool PsSimSensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*) {
    G4double edep = step->GetTotalEnergyDeposit();
    if (edep <= 0.) return false;

    auto* hit = new PsSimHit();
    hit->SetEdep(edep / MeV);
    hit->SetTime(step->GetPreStepPoint()->GetGlobalTime() / ns);
    hit->SetPosition(step->GetPreStepPoint()->GetPosition() / mm);
    hit->SetPDGCode(step->GetTrack()->GetDefinition()->GetPDGEncoding());
    fHitsCollection->insert(hit);
    return true;
}
