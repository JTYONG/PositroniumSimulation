#include "PsSimRunAction.hh"
#include "PsSimOutputMessenger.hh"

#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4Threading.hh"
#include "G4SystemOfUnits.hh"

PsSimRunAction::PsSimRunAction(G4bool isMaster, PsSimOutputMessenger* outMsg)
  : fIsMaster(isMaster), fOutMsg(outMsg) {}

PsSimRunAction::~PsSimRunAction() {
    if (fTxtStream.is_open()) fTxtStream.close();
}

void PsSimRunAction::BeginOfRunAction(const G4Run*) {
    auto* am = G4AnalysisManager::Instance();
    am->SetVerboseLevel(1);
    am->SetDefaultFileType("root");
    am->SetNtupleMerging(true);  // must be called on all threads before CreateNtuple

    am->OpenFile(fOutMsg->GetRootFile());

    // Book ntuple (global sequential column IDs 0–7)
    am->CreateNtuple("PsTree", "PALS simulation per-event summary");
    am->CreateNtupleIColumn("eventID");      // col 0
    am->CreateNtupleDColumn("totalEdep");    // col 1  [MeV]
    am->CreateNtupleIColumn("nHits");        // col 2
    am->CreateNtupleDColumn("firstHitTime"); // col 3  [ns]
    am->CreateNtupleDColumn("firstHitEdep"); // col 4  [MeV]
    am->CreateNtupleIColumn("firstHitPDG");  // col 5
    am->CreateNtupleIColumn("oPsFormed");    // col 6  (0/1)
    am->CreateNtupleDColumn("oPsLifetime");  // col 7  [ns] (-1 if no o-Ps)
    am->FinishNtuple();

    // Open per-thread TXT backup (master thread skips)
    if (!fIsMaster) {
        G4int tid = G4Threading::G4GetThreadId();
        G4String path = fOutMsg->GetTxtBase() + "_t" + std::to_string(tid) + ".txt";
        fTxtStream.open(path);
        fTxtStream << "eventID totalEdep_MeV nHits firstHitTime_ns "
                      "firstHitEdep_MeV firstHitPDG oPsFormed oPsLifetime_ns\n";
    }
}

void PsSimRunAction::EndOfRunAction(const G4Run*) {
    auto* am = G4AnalysisManager::Instance();
    am->Write();
    am->CloseFile();
    if (fTxtStream.is_open()) fTxtStream.close();
}
