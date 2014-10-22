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

#include "FCCRunAction.hh"
#include "AtlfastMuonMatrixManager.hh"
#include "AtlfastPionMatrixManager.hh"
#include "AtlfastElectronMatrixManager.hh"
#include "g4root.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FCCRunAction::FCCRunAction()
 : G4UserRunAction()
{
  // Create analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  //Create mangers for smeaing muons, pions and electrons
  Atlfast::MuonMatrixManager* muonMatrixManager = Atlfast::MuonMatrixManager::Instance();
  muonMatrixManager->initialise("data/Atlfast_MuonResParam_CSC.dat",time(NULL));

  Atlfast::PionMatrixManager* pionMatrixManager = Atlfast::PionMatrixManager::Instance();
  pionMatrixManager->initialise("data/Atlfast_PionResParam_DC1_NewUnits.dat",time(NULL));

  Atlfast::ElectronMatrixManager* electronMatrixManager = Atlfast::ElectronMatrixManager::Instance();
  electronMatrixManager->initialise("data/Atlfast_ElectronResParam_CSC.dat","data/Atlfast_ElectronBremParam_CSC.dat",time(NULL));

  // Default settings
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFileName("SimpleOutput");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FCCRunAction::~FCCRunAction()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FCCRunAction::BeginOfRunAction(const G4Run* /*run*/)
{
  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);

  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  // The default file name is set in FCCRunAction::FCCRunAction(),
  // it can be overwritten in a macro
  analysisManager->OpenFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FCCRunAction::EndOfRunAction(const G4Run* /*run*/)
{
  // save histograms & ntuple
  //
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
