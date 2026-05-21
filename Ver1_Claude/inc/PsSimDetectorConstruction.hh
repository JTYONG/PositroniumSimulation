#pragma once

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4LogicalVolume;
class PsSimDetectorMessenger;

class PsSimDetectorConstruction : public G4VUserDetectorConstruction {
public:
    PsSimDetectorConstruction();
    ~PsSimDetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;

    // Messenger setters
    void SetWorldHalfSize(G4double v)   { fWorldHalfSize = v; }
    void SetScintRmin(G4double v)       { fScintRmin = v; }
    void SetScintRmax(G4double v)       { fScintRmax = v; }
    void SetScintHalfY(G4double v)      { fScintHalfY = v; }
    void SetSilicaRmin(G4double v)      { fSilicaRmin = v; }
    void SetSilicaRmax(G4double v)      { fSilicaRmax = v; }
    void SetSilicaDensity(G4double v)   { fSilicaDensity = v; }

private:
    void DefineMaterials();

    G4double fWorldHalfSize = 0.;
    G4double fScintRmin     = 0.;
    G4double fScintRmax     = 0.;
    G4double fScintHalfY    = 0.;
    G4double fSilicaRmin    = 0.;
    G4double fSilicaRmax    = 0.;
    G4double fSilicaDensity = 0.;

    G4LogicalVolume* fScintLV  = nullptr;
    G4LogicalVolume* fSilicaLV = nullptr;

    PsSimDetectorMessenger* fMessenger = nullptr;
};
