#pragma once

#include "G4VSensitiveDetector.hh"
#include "PsSimHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class PsSimSensitiveDetector : public G4VSensitiveDetector {
public:
    explicit PsSimSensitiveDetector(const G4String& name);
    ~PsSimSensitiveDetector() override = default;

    void Initialize(G4HCofThisEvent* hce) override;
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;

private:
    PsSimHitsCollection* fHitsCollection = nullptr;
    G4int                fHCID           = -1;
};
