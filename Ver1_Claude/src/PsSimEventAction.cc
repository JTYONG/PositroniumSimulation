#include "PsSimEventAction.hh"
#include "PsSimRunAction.hh"
#include "PsSimHit.hh"
#include "PsEventData.hh"

#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

PsSimEventAction::PsSimEventAction(PsSimRunAction* runAction)
  : fRunAction(runAction) {}

void PsSimEventAction::BeginOfEventAction(const G4Event*) {
    PsEventData::Instance().Reset();
    fTotalEdep    = 0.;
    fNHits        = 0;
    fFirstHitTime = -1.;
    fFirstHitEdep = 0.;
    fFirstHitPDG  = 0;
}

void PsSimEventAction::EndOfEventAction(const G4Event* event) {
    // Resolve hits collection ID once
    if (fHCID < 0)
        fHCID = G4SDManager::GetSDMpointer()->GetCollectionID("ScintSD/ScintHitsCollection");

    auto* hce = event->GetHCofThisEvent();
    if (hce) {
        auto* hc = static_cast<PsSimHitsCollection*>(hce->GetHC(fHCID));
        if (hc) {
            G4double tBest = 1.e30;
            for (std::size_t i = 0; i < hc->entries(); ++i) {
                auto* h = (*hc)[i];
                fTotalEdep += h->GetEdep();
                fNHits++;
                if (h->GetTime() < tBest) {
                    tBest         = h->GetTime();
                    fFirstHitTime = tBest;
                    fFirstHitEdep = h->GetEdep();
                    fFirstHitPDG  = h->GetPDGCode();
                }
            }
        }
    }

    // Read o-Ps data written by PsFormationProcess
    auto& ps = PsEventData::Instance();

    // Fill ROOT ntuple — column IDs are global sequential (0-7)
    // col0:eventID(I) col1:totalEdep(D) col2:nHits(I) col3:firstHitTime(D)
    // col4:firstHitEdep(D) col5:firstHitPDG(I) col6:oPsFormed(I) col7:oPsLifetime(D)
    auto* am = G4AnalysisManager::Instance();
    am->FillNtupleIColumn(0, 0, event->GetEventID());
    am->FillNtupleDColumn(0, 1, fTotalEdep);
    am->FillNtupleIColumn(0, 2, fNHits);
    am->FillNtupleDColumn(0, 3, fFirstHitTime);
    am->FillNtupleDColumn(0, 4, fFirstHitEdep);
    am->FillNtupleIColumn(0, 5, fFirstHitPDG);
    am->FillNtupleIColumn(0, 6, static_cast<G4int>(ps.oPsFormed));
    am->FillNtupleDColumn(0, 7, ps.oPsLifetime);
    am->AddNtupleRow(0);

    // Write TXT backup line
    auto& txt = fRunAction->GetTxtStream();
    if (txt.is_open()) {
        txt << event->GetEventID() << " "
            << fTotalEdep    << " "
            << fNHits        << " "
            << fFirstHitTime << " "
            << fFirstHitEdep << " "
            << fFirstHitPDG  << " "
            << static_cast<int>(ps.oPsFormed) << " "
            << ps.oPsLifetime << "\n";
    }
}
