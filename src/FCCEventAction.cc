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

#include "FCCEventAction.hh"
#include "FCCEventInformation.hh"
#include "FCCRunAction.hh"
#include "g4root.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include "Randomize.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FCCEventAction::FCCEventAction()
 : G4UserEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FCCEventAction::~FCCEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FCCEventAction::BeginOfEventAction(const G4Event* event)
{
//New event, add the user information object
//if set to false -> no track smearing. if true -> smear tracks
  G4EventManager::GetEventManager()->SetUserInformation(new FCCEventInformation(true));

   G4String evName = "Event_";
   evName += G4UIcommand::ConvertToString(event->GetEventID());

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  // Creating ntuple
  //
  analysisManager->CreateNtuple(evName, evName);
  analysisManager->CreateNtupleIColumn("PID");  // column Id = 0
  analysisManager->CreateNtupleDColumn("pX");  // column Id = 1
  analysisManager->CreateNtupleDColumn("pY"); // column Id = 2
  analysisManager->CreateNtupleDColumn("pZ"); // column Id = 3

  if(((FCCEventInformation*)event->GetUserInformation())->GetDoSmearing())
  {
     analysisManager->CreateNtupleIColumn("PIDsmeared");  // column Id = 0
     analysisManager->CreateNtupleDColumn("pXsmeared");  // column Id = 1
     analysisManager->CreateNtupleDColumn("pYsmeared"); // column Id = 2
     analysisManager->CreateNtupleDColumn("pZsmeared"); // column Id = 3
  }
  analysisManager->FinishNtuple(event->GetEventID());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FCCEventAction::EndOfEventAction(const G4Event* event){

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
