#include "FCCTrackingAction.hh"

#include "G4Track.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "B5Analysis.hh"

#include "Randomize.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FCCTrackingAction::FCCTrackingAction()
 : G4UserTrackingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FCCTrackingAction::~FCCTrackingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FCCTrackingAction::PostUserTrackingAction(const G4Track* track)
{
   const G4Event* event = G4RunManager::GetRunManager()->GetCurrentEvent();
   //
    // Fill histograms & ntuple
    //

    // Get analysis manager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    // Fill ntuple
    G4int PID = track->GetDynamicParticle()->GetPDGcode();
    G4ThreeVector P = track->GetMomentum();
    if(P.x()!=0 && P.y()!=0 && P.z()!=0)
    {
    analysisManager->FillNtupleIColumn(0, PID);
    analysisManager->FillNtupleDColumn(1, P.x());
    analysisManager->FillNtupleDColumn(2, P.y());
    analysisManager->FillNtupleDColumn(3, P.z());
    analysisManager->AddNtupleRow(event->GetEventID());
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
