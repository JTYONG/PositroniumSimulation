#include "PsSimDetectorMessenger.hh"
#include "PsSimDetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4SystemOfUnits.hh"

PsSimDetectorMessenger::PsSimDetectorMessenger(PsSimDetectorConstruction* det)
  : fDetector(det) {
    fSimDir = new G4UIdirectory("/sim/");
    fSimDir->SetGuidance("Positronium simulation commands.");

    fGeoDir = new G4UIdirectory("/sim/geometry/");
    fGeoDir->SetGuidance("Geometry parameter commands.");

    fWorldHalfCmd = new G4UIcmdWithADoubleAndUnit("/sim/geometry/worldHalfSize", this);
    fWorldHalfCmd->SetGuidance("Half-side of the cubic world volume.");
    fWorldHalfCmd->SetParameterName("size", false);
    fWorldHalfCmd->SetDefaultUnit("cm");
    fWorldHalfCmd->SetDefaultValue(5.);
    fWorldHalfCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fScintRminCmd = new G4UIcmdWithADoubleAndUnit("/sim/geometry/scintRmin", this);
    fScintRminCmd->SetGuidance("Scintillator inner radius.");
    fScintRminCmd->SetParameterName("r", false);
    fScintRminCmd->SetDefaultUnit("cm");
    fScintRminCmd->SetDefaultValue(5.);
    fScintRminCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fScintRmaxCmd = new G4UIcmdWithADoubleAndUnit("/sim/geometry/scintRmax", this);
    fScintRmaxCmd->SetGuidance("Scintillator outer radius.");
    fScintRmaxCmd->SetParameterName("r", false);
    fScintRmaxCmd->SetDefaultUnit("cm");
    fScintRmaxCmd->SetDefaultValue(7.);
    fScintRmaxCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fScintHalfYCmd = new G4UIcmdWithADoubleAndUnit("/sim/geometry/scintHalfY", this);
    fScintHalfYCmd->SetGuidance("Scintillator half-height along Y.");
    fScintHalfYCmd->SetParameterName("h", false);
    fScintHalfYCmd->SetDefaultUnit("cm");
    fScintHalfYCmd->SetDefaultValue(5.);
    fScintHalfYCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSilicaRminCmd = new G4UIcmdWithADoubleAndUnit("/sim/geometry/silicaRmin", this);
    fSilicaRminCmd->SetGuidance("Porous silica shell inner radius.");
    fSilicaRminCmd->SetParameterName("r", false);
    fSilicaRminCmd->SetDefaultUnit("cm");
    fSilicaRminCmd->SetDefaultValue(2.);
    fSilicaRminCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSilicaRmaxCmd = new G4UIcmdWithADoubleAndUnit("/sim/geometry/silicaRmax", this);
    fSilicaRmaxCmd->SetGuidance("Porous silica shell outer radius.");
    fSilicaRmaxCmd->SetParameterName("r", false);
    fSilicaRmaxCmd->SetDefaultUnit("cm");
    fSilicaRmaxCmd->SetDefaultValue(2.5);
    fSilicaRmaxCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSilicaDensCmd = new G4UIcmdWithADoubleAndUnit("/sim/geometry/silicaDensity", this);
    fSilicaDensCmd->SetGuidance("Porous silica density (models porosity).");
    fSilicaDensCmd->SetParameterName("rho", false);
    fSilicaDensCmd->SetUnitCategory("Volumic Mass");
    fSilicaDensCmd->SetDefaultUnit("g/cm3");
    fSilicaDensCmd->SetDefaultValue(1.0);
    fSilicaDensCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

PsSimDetectorMessenger::~PsSimDetectorMessenger() {
    delete fWorldHalfCmd;
    delete fScintRminCmd;
    delete fScintRmaxCmd;
    delete fScintHalfYCmd;
    delete fSilicaRminCmd;
    delete fSilicaRmaxCmd;
    delete fSilicaDensCmd;
    delete fGeoDir;
    delete fSimDir;
}

void PsSimDetectorMessenger::SetNewValue(G4UIcommand* cmd, G4String val) {
    if (cmd == fWorldHalfCmd) fDetector->SetWorldHalfSize(fWorldHalfCmd->GetNewDoubleValue(val));
    else if (cmd == fScintRminCmd) fDetector->SetScintRmin(fScintRminCmd->GetNewDoubleValue(val));
    else if (cmd == fScintRmaxCmd) fDetector->SetScintRmax(fScintRmaxCmd->GetNewDoubleValue(val));
    else if (cmd == fScintHalfYCmd) fDetector->SetScintHalfY(fScintHalfYCmd->GetNewDoubleValue(val));
    else if (cmd == fSilicaRminCmd) fDetector->SetSilicaRmin(fSilicaRminCmd->GetNewDoubleValue(val));
    else if (cmd == fSilicaRmaxCmd) fDetector->SetSilicaRmax(fSilicaRmaxCmd->GetNewDoubleValue(val));
    else if (cmd == fSilicaDensCmd) fDetector->SetSilicaDensity(fSilicaDensCmd->GetNewDoubleValue(val));
}
