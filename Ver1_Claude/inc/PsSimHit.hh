#pragma once

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class PsSimHit : public G4VHit {
public:
    PsSimHit() = default;
    ~PsSimHit() override = default;

    inline void* operator new(size_t);
    inline void  operator delete(void* hit);

    void SetEdep(G4double e)          { fEdep = e; }
    void SetTime(G4double t)          { fTime = t; }
    void SetPosition(G4ThreeVector p) { fPos  = p; }
    void SetPDGCode(G4int pdg)        { fPDG  = pdg; }

    G4double      GetEdep()     const { return fEdep; }
    G4double      GetTime()     const { return fTime; }
    G4ThreeVector GetPosition() const { return fPos;  }
    G4int         GetPDGCode()  const { return fPDG;  }

private:
    G4double      fEdep = 0.;
    G4double      fTime = 0.;
    G4ThreeVector fPos;
    G4int         fPDG  = 0;
};

using PsSimHitsCollection = G4THitsCollection<PsSimHit>;

extern G4ThreadLocal G4Allocator<PsSimHit>* PsSimHitAllocator;

inline void* PsSimHit::operator new(size_t) {
    if (!PsSimHitAllocator)
        PsSimHitAllocator = new G4Allocator<PsSimHit>;
    return PsSimHitAllocator->MallocSingle();
}

inline void PsSimHit::operator delete(void* hit) {
    PsSimHitAllocator->FreeSingle(static_cast<PsSimHit*>(hit));
}
