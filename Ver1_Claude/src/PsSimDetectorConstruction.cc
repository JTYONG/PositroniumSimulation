#include "PsSimDetectorConstruction.hh"
#include "PsSimDetectorMessenger.hh"
#include "PsSimSensitiveDetector.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"

PsSimDetectorConstruction::PsSimDetectorConstruction() {
    fWorldHalfSize = 10. * cm;  // must exceed scintRmax (7 cm)
    fScintRmin     = 5.  * cm;
    fScintRmax     = 7.  * cm;
    fScintHalfY    = 5.  * cm;
    fSilicaRmin    = 2.  * cm;
    fSilicaRmax    = 2.5 * cm;
    fSilicaDensity = 1.0 * g / cm3;
    fMessenger = new PsSimDetectorMessenger(this);
}

PsSimDetectorConstruction::~PsSimDetectorConstruction() {
    delete fMessenger;
}

G4VPhysicalVolume* PsSimDetectorConstruction::Construct() {
    auto* nist = G4NistManager::Instance();

    // ── Porous SiO2 material ────────────────────────────────────────────────
    auto* porousSiO2 = new G4Material("PorousSiO2", fSilicaDensity, 2);
    porousSiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
    porousSiO2->AddElement(nist->FindOrBuildElement("O"),  2);

    // ── World ────────────────────────────────────────────────────────────────
    auto* worldSolid = new G4Box("World", fWorldHalfSize, fWorldHalfSize, fWorldHalfSize);
    auto* worldLV    = new G4LogicalVolume(worldSolid, nist->FindOrBuildMaterial("G4_AIR"), "World");
    auto* worldPV    = new G4PVPlacement(nullptr, G4ThreeVector(), worldLV, "World", nullptr, false, 0);

    // ── Plastic scintillator (G4Tubs axis = Z by default; rotate 90° about X for Y-axis) ──
    auto* scintSolid = new G4Tubs("Scintillator", fScintRmin, fScintRmax, fScintHalfY, 0., CLHEP::twopi);
    auto* scintMat   = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    fScintLV         = new G4LogicalVolume(scintSolid, scintMat, "Scintillator");

    auto* rotX = new G4RotationMatrix();
    rotX->rotateX(90. * deg);
    new G4PVPlacement(rotX, G4ThreeVector(), fScintLV, "Scintillator", worldLV, false, 0);

    // ── Porous silica spherical shell ────────────────────────────────────────
    auto* silicaSolid = new G4Sphere("PorousSilica",
                                     fSilicaRmin, fSilicaRmax,
                                     0., CLHEP::twopi,
                                     0., CLHEP::pi);
    fSilicaLV = new G4LogicalVolume(silicaSolid, porousSiO2, "PorousSilica");
    new G4PVPlacement(nullptr, G4ThreeVector(), fSilicaLV, "PorousSilica", worldLV, false, 0);

    // ── Fine production cuts for positron thermalization in silica ───────────
    auto* silicaRegion = new G4Region("SilicaRegion");
    silicaRegion->AddRootLogicalVolume(fSilicaLV);
    auto* cuts = new G4ProductionCuts();
    cuts->SetProductionCut(0.05 * mm);
    silicaRegion->SetProductionCuts(cuts);

    return worldPV;
}

void PsSimDetectorConstruction::ConstructSDandField() {
    auto* sd = new PsSimSensitiveDetector("ScintSD");
    G4SDManager::GetSDMpointer()->AddNewDetector(sd);
    SetSensitiveDetector(fScintLV, sd);
}
