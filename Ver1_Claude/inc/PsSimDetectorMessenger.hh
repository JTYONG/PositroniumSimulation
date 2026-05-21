#pragma once

#include "G4UImessenger.hh"
#include "globals.hh"

class PsSimDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;

class PsSimDetectorMessenger : public G4UImessenger {
public:
    explicit PsSimDetectorMessenger(PsSimDetectorConstruction* det);
    ~PsSimDetectorMessenger() override;

    void SetNewValue(G4UIcommand* cmd, G4String val) override;

private:
    PsSimDetectorConstruction* fDetector;

    G4UIdirectory*               fSimDir       = nullptr;
    G4UIdirectory*               fGeoDir       = nullptr;
    G4UIcmdWithADoubleAndUnit*   fWorldHalfCmd = nullptr;
    G4UIcmdWithADoubleAndUnit*   fScintRminCmd = nullptr;
    G4UIcmdWithADoubleAndUnit*   fScintRmaxCmd = nullptr;
    G4UIcmdWithADoubleAndUnit*   fScintHalfYCmd= nullptr;
    G4UIcmdWithADoubleAndUnit*   fSilicaRminCmd= nullptr;
    G4UIcmdWithADoubleAndUnit*   fSilicaRmaxCmd= nullptr;
    G4UIcmdWithADoubleAndUnit*   fSilicaDensCmd= nullptr;
};
