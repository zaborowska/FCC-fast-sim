//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//

#include "FCCTrackingAction.hh"
#include "FCCEventInformation.hh"
#include "FCCTrackInformation.hh"
#include "FCCPrimaryParticleInformation.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "g4root.hh"

#include "Randomize.hh"
#include "G4TrackingManager.hh"
#include <iomanip>
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FCCTrackingAction::FCCTrackingAction()
   : G4UserTrackingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FCCTrackingAction::~FCCTrackingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FCCTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
   G4int PID = aTrack->GetDynamicParticle()->GetPDGcode();
   if(
      !( // (abs(PID)==11 || abs(PID)==211 || abs(PID)==13) && // to reject other particles that are not being registered
         abs(aTrack->GetMomentum().pseudoRapidity())<5.5
         ))
   {
    ((G4Track*)aTrack)->SetTrackStatus(fStopAndKill);
   }


// filling data only for primary particles
   if(aTrack->GetParentID()) return;

   // Fill ntuple with G4 original data
   G4ThreeVector P = aTrack->GetMomentum();
   if(P.x()!=0 && P.y()!=0 && P.z()!=0 )
      SaveTrack(false, PID, PID, P);

   // SMEAR HERE

   fpTrackingManager->SetUserTrackInformation(new FCCTrackInformation());


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FCCTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
// This method is called not only once at the end of the life of
// a track, but also each time it is suspended, as it happens
// in the case of neutrons with _HP Physics Lists.
// To be sure that we collect information about a track only once
// when its life comes to the end, we have to require that its
// status is "fStopAndKill".
{
   if ( aTrack->GetTrackStatus() == fStopAndKill )
   {
      // filling data only for primary particles
      if(aTrack->GetParentID()) return;
      ((FCCTrackInformation*) aTrack->GetUserInformation())->Print();
      if ( ((FCCTrackInformation*) aTrack->GetUserInformation())->GetHitDetector() )
      {
         G4int PID = aTrack->GetDynamicParticle()->GetPDGcode();
         G4ThreeVector P = aTrack->GetMomentum();
         SaveTrack(true, PID, PID, P);
      }
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FCCTrackingAction::SaveTrack(G4bool HitDetector, G4int partID,  G4int PID, G4ThreeVector P) const
{
   const G4Event* event = G4RunManager::GetRunManager()->GetCurrentEvent();
   G4int evNo = event->GetEventID();
   G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
   G4int ntupleID = evNo+HitDetector;
   if (((FCCEventInformation*)event->GetUserInformation())->GetDoSmearing())
      ntupleID = 2* evNo+HitDetector;
   analysisManager->FillNtupleIColumn(ntupleID, 0, partID);
   analysisManager->FillNtupleIColumn(ntupleID, 1, PID);
   analysisManager->FillNtupleDColumn(ntupleID, 2, P.x());
   analysisManager->FillNtupleDColumn(ntupleID, 3, P.y());
   analysisManager->FillNtupleDColumn(ntupleID, 4, P.z());
   analysisManager->AddNtupleRow(ntupleID);

   return;
}
