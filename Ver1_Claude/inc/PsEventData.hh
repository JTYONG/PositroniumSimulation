#pragma once

#include "globals.hh"
#include "G4Threading.hh"

// Thread-local store for positronium formation data produced in PsFormationProcess
// and consumed in PsSimEventAction. Reset at the start of each event.
struct PsEventData {
    bool   oPsFormed   = false;
    double oPsLifetime = -1.;  // ns; -1 if no o-Ps formed this event

    void Reset() { oPsFormed = false; oPsLifetime = -1.; }
    void SetOPs(bool formed, double tau_ns) { oPsFormed = formed; oPsLifetime = tau_ns; }

    static PsEventData& Instance() {
        static G4ThreadLocal PsEventData* fgInst = nullptr;
        if (!fgInst) fgInst = new PsEventData();
        return *fgInst;
    }
private:
    PsEventData() = default;
};
