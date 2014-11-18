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
#include "FCCPrimaryParticleInformation.hh"
#include "FCCOutput.hh"
#include "FCCSmearer.hh"

#include "G4ThreeVector.hh"

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
      FCCOutput::Instance()->SaveTrack(false, PID, PID, P);

   // SMEAR HERE
   FCCSmearer::Instance()->Smear(new G4Track(*aTrack));

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FCCTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
// This method is called not only once at the end of the life of
// a track, but also each time it is suspended, as it happens
// in the case of neutrons with _HP Physics Lists.
// To be sure that we collect information about a track only once
// when its   if ( aTrack->GetTrackStatus() == fStopAndKill )
{}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

