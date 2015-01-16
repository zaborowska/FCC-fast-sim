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
// based on G4 examples/extended/eventgenerator/HepMC/HepMCEx01/src/HepMCG4AsciiReaderMessenger.cc
//
#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "FCCRootMessenger.hh"
#include "FCCRootReader.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCRootMessenger::FCCRootMessenger (FCCRootReader* aGenerator) : fGenerator(aGenerator)
{
  fDirectory= new G4UIdirectory("/generator/hepmcRoot/");
  fDirectory-> SetGuidance("Reading HepMC event from an Root file");

  fVerboseCommand=
    new G4UIcmdWithAnInteger("/generator/hepmcRoot/verbose", this);
  fVerboseCommand-> SetGuidance("Set verbose level");
  fVerboseCommand-> SetParameterName("verboseLevel", false, false);
  fVerboseCommand-> SetRange("verboseLevel>=0 && verboseLevel<=1");

  fOpenCommand= new G4UIcmdWithAString("/generator/hepmcRoot/open", this);
  fOpenCommand-> SetGuidance("(re)open data file (HepMC Root format)");
  fOpenCommand-> SetParameterName("input root file", true, true);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCRootMessenger::~FCCRootMessenger()
{
  delete fVerboseCommand;
  delete fOpenCommand;

  delete fDirectory;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FCCRootMessenger::SetNewValue(G4UIcommand* aCommand,
                                              G4String aNewValues)
{
  if (aCommand==fVerboseCommand)
  {
    int level= fVerboseCommand-> GetNewIntValue(aNewValues);
    fGenerator-> SetVerboseLevel(level);
  }
  else if (aCommand==fOpenCommand)
  {
    fGenerator-> SetFileName(aNewValues);
    G4cout << "HepMC Root inputfile: "
           << fGenerator-> GetFileName() << G4endl;
    fGenerator-> Initialize();
  }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4String FCCRootMessenger::GetCurrentValue(G4UIcommand* aCommand)
{
  G4String cv;

  if (aCommand == fVerboseCommand)
  {
    cv= fVerboseCommand-> ConvertToString(fGenerator-> GetVerboseLevel());
  }
  else  if (aCommand == fOpenCommand)
  {
    cv= fGenerator-> GetFileName();
  }
  return cv;
}
