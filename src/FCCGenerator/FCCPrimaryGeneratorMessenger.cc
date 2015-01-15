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
// based on G4 examples/extended/eventgenerator/HepMC/HepMCEx02/src/H02PrimaryGeneratorMessenger.cc
//

#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIparameter.hh"
#include "FCCPrimaryGeneratorMessenger.hh"
#include "FCCPrimaryGeneratorAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCPrimaryGeneratorMessenger::FCCPrimaryGeneratorMessenger (FCCPrimaryGeneratorAction* aGeneratorAction) : fPrimaryAction(aGeneratorAction)
{
  aDirectory= new G4UIdirectory("/generator/");
  aDirectory-> SetGuidance("Control commands for primary generator");

  fSelectCommand= new G4UIcmdWithAString("/generator/select", this);
  fSelectCommand-> SetGuidance("select generator type");
  fSelectCommand-> SetParameterName("generator_type", false, false);
#ifdef G4LIB_USE_PYTHIA8
  fSelectCommand-> SetCandidates("particleGun pythia8 hepmcRoot");
#else
  fSelectCommand-> SetCandidates("particleGun hepmcRoot");
#endif
  fSelectCommand-> SetDefaultValue("particleGun");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCPrimaryGeneratorMessenger::~FCCPrimaryGeneratorMessenger()
{
  delete fSelectCommand;
  delete aDirectory;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FCCPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* aCommand, G4String aNewValues)
{
  if ( aCommand==fSelectCommand) {
    fPrimaryAction-> SetGenerator(aNewValues);
    G4cout << "current generator type: "
            << fPrimaryAction-> GetGeneratorName() << G4endl;
  } else {
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4String FCCPrimaryGeneratorMessenger::GetCurrentValue(G4UIcommand* aCommand)
{
  G4String cv, st;
  if (aCommand == fSelectCommand) {
    cv= fPrimaryAction-> GetGeneratorName();
  }

 return cv;
}
