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

#include "AtlfastMuonMatrixManager.hh"
#include "AtlfastPionMatrixManager.hh"
#include "AtlfastElectronMatrixManager.hh"

#include "G4Track.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "g4root.hh"

#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "Randomize.hh"
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
      const G4Event* event = G4RunManager::GetRunManager()->GetCurrentEvent();
      G4int evNo = event->GetEventID();
      G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

      // Fill ntuple with G4 original data
      G4int PID = aTrack->GetDynamicParticle()->GetPDGcode();
      G4ThreeVector P = aTrack->GetMomentum();
      if(P.x()!=0 && P.y()!=0 && P.z()!=0  && aTrack->GetDynamicParticle()->GetCharge())
      {
         analysisManager->FillNtupleIColumn(evNo, 0, PID);
         analysisManager->FillNtupleDColumn(evNo, 1, P.x());
         analysisManager->FillNtupleDColumn(evNo, 2, P.y());
         analysisManager->FillNtupleDColumn(evNo, 3, P.z());
         if(((FCCEventInformation*)event->GetUserInformation())->GetDoSmearing() && (abs(PID) == 13 || abs(PID) == 11 || abs(PID) == 211) )
         {
            G4Track* trackSmeared;
            trackSmeared = Smear(aTrack);
            delete trackSmeared;
         }
         analysisManager->AddNtupleRow(evNo);
      }
      // else if(PID!=11)G4cout<<" particle "<<PID<<" with 0 momentum..."<<G4endl;
   }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4Track* FCCTrackingAction::Smear(const G4Track* aTrackOriginal)
{
   G4Track* trackSmeared = new G4Track();//(*aTrackOriginal);
   CLHEP::HepSymMatrix sigma; // smear matrix for track
   std::vector<double> smearVariables;   // Vector of correlated gaussian variables
   if (aTrackOriginal->GetDefinition() == G4MuonPlus::MuonPlusDefinition() ||
       aTrackOriginal->GetDefinition() == G4MuonMinus::MuonMinusDefinition())
   {
      Atlfast::MuonMatrixManager* muonMatrixManager = Atlfast::MuonMatrixManager::Instance();
      smearVariables = muonMatrixManager->getVariables( *aTrackOriginal, sigma );
      //G4cout<<" Smearing done according to MUON data file" <<G4endl;
   }
   if (aTrackOriginal->GetDefinition() == G4Electron::ElectronDefinition() ||
       aTrackOriginal->GetDefinition() == G4Positron::PositronDefinition())
   {
      Atlfast::ElectronMatrixManager* electronMatrixManager = Atlfast::ElectronMatrixManager::Instance();
      smearVariables = electronMatrixManager->getVariables( *aTrackOriginal, sigma );
      //G4cout<<" Smearing done according to ELECTRON data file" <<G4endl;
   }
   if (aTrackOriginal->GetDefinition() == G4PionPlus::PionPlusDefinition() ||
       aTrackOriginal->GetDefinition() == G4PionMinus::PionMinusDefinition())
   {
      Atlfast::PionMatrixManager* pionMatrixManager = Atlfast::PionMatrixManager::Instance();
      smearVariables = pionMatrixManager->getVariables( *aTrackOriginal, sigma );
      //G4cout<<" Smearing done according to PION data file" <<G4endl;
   }
   // helpful variables
   double originPhi = aTrackOriginal->GetMomentum().phi();
   double originTheta = aTrackOriginal->GetMomentum().theta();
   double originCharge = aTrackOriginal->GetDynamicParticle()->GetCharge();
   double originPt = aTrackOriginal->GetMomentum().perp();

   // Atlfast smeared variables
   double impactParameter; // [0]
   double zPerigee; //[1]
   double Phi = originPhi +  smearVariables[2]; //[2]
   if(Phi<-M_PI)
      while(Phi<M_PI)
         Phi+=2*M_PI;
   else if(Phi>M_PI)
      while(Phi>M_PI)
         Phi-=2*M_PI;
   double cotTheta = 1/tan(originTheta) + smearVariables[3] ; //[3]
   double invPtCharge = (double)((double)originCharge/(abs((double)originCharge)*originPt)) +  smearVariables[4]; // q/pT where q = q/|q| (just sign) //[4]

   // back to P
   double Px = abs(1./invPtCharge)*sin(Phi);
   double Py = abs(1./invPtCharge)*cos(Phi);
   double Pz = abs(1./invPtCharge)/sin( atan(1./cotTheta) );

   //saving to ntuple
   const G4Event* event = G4RunManager::GetRunManager()->GetCurrentEvent();
   G4int evNo = event->GetEventID();
   G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
   analysisManager->FillNtupleIColumn(evNo, 4, aTrackOriginal->GetDynamicParticle()->GetPDGcode());
   analysisManager->FillNtupleDColumn(evNo, 5, Px);
   analysisManager->FillNtupleDColumn(evNo, 6, Py);
   analysisManager->FillNtupleDColumn(evNo, 7, Pz);

//     G4cout<<"____Current particle : "<<aTrackOriginal->GetDefinition()->GetParticleName()<<" with ID  "<<aTrackOriginal->GetTrackID()<<G4endl;
// //   if (Phi==Phi)
//  G4cout<<" PHI : "<<originPhi<<" ->  "<<Phi<<G4endl;
// //   if (cotTheta==cotTheta) 
// G4cout<<" THETA : "<<originTheta<<" ->  "<<atan(1./cotTheta)<<G4endl;
// //   if (invPtCharge==invPtCharge) 
// G4cout<<" invPtCharge : "<<originPt<<" ->  "<<((double)(1./(double)invPtCharge))<<G4endl<<G4endl;

   return trackSmeared;
}
