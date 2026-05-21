#pragma once

#include "G4UserEventAction.hh"
#include "globals.hh"

class PsSimRunAction;
class G4Event;

class PsSimEventAction : public G4UserEventAction {
public:
    explicit PsSimEventAction(PsSimRunAction* runAction);
    ~PsSimEventAction() override = default;

    void BeginOfEventAction(const G4Event* event) override;
    void EndOfEventAction(const G4Event* event) override;

private:
    PsSimRunAction* fRunAction = nullptr;
    G4int           fHCID      = -1;

    // per-event accumulators
    G4double fTotalEdep    = 0.;
    G4int    fNHits        = 0;
    G4double fFirstHitTime = -1.;
    G4double fFirstHitEdep = 0.;
    G4int    fFirstHitPDG  = 0;
    G4double fFirstHitX    = 0.;
    G4double fFirstHitY    = 0.;
    G4double fFirstHitZ    = 0.;
};
