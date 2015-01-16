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
// based on G4 examples/extended/eventgenerator/HepMC/HepMCEx01/src/HepMCG4PythiaMessenger.cc
//

#ifdef G4LIB_USE_PYTHIA8

#include <sstream>
#include <fstream>
#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "FCCPythiaMessenger.hh"
#include "FCCPythiaInterface.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCPythiaMessenger::FCCPythiaMessenger(FCCPythiaInterface* aGenerator)
  : fGenerator(aGenerator)
{
  fDirectory= new G4UIdirectory("/generator/pythia8/");
  fDirectory-> SetGuidance("Commands for Pythia 8 event generation");

  fVerboseCommand= new G4UIcmdWithAnInteger("/generator/pythia8/verbose",this);
  fVerboseCommand-> SetGuidance("set verbose level");
  fVerboseCommand-> SetParameterName("verboseLevel", false, false);
  fVerboseCommand-> SetRange("verboseLevel>=0 && verboseLevel<=2");

  fPrintCommand= new G4UIcmdWithoutParameter("/generator/pythia8/print", this);
  fPrintCommand-> SetGuidance("print user information.");

  fInitCommand= new G4UIcommand("/generator/pythia8/init", this);
  fInitCommand-> SetGuidance("call INIT");
  G4UIparameter* beam= new G4UIparameter("beam particle (pdg code)", 's', false);
  fInitCommand-> SetParameter(beam);
  G4UIparameter* target= new G4UIparameter("target particle (pdg code)", 's', false);
  fInitCommand-> SetParameter(target);
  G4UIparameter* eCM= new G4UIparameter("energy of system in CM frame (GeV)", 'd', false);
  fInitCommand-> SetParameter(eCM);

  fStatCommand= new G4UIcmdWithoutParameter("/generator/pythia8/stat", this);
  fStatCommand-> SetGuidance("print statistics");

  fReadCommand= new G4UIcommand("/generator/pythia8/read",this);
  fReadCommand-> SetGuidance("call PYTHIAREAD");
  G4UIparameter* parameter= new G4UIparameter ("Parameter", 's', false);
  fReadCommand-> SetParameter(parameter);

  fSeedCommand= new G4UIcmdWithAnInteger("/generator/pythia8/setSeed", this);
  fSeedCommand-> SetGuidance("set initial seed.");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCPythiaMessenger::~FCCPythiaMessenger()
{
  delete fVerboseCommand;
  delete fPrintCommand;
  delete fInitCommand;
  delete fStatCommand;
  delete fReadCommand;
  delete fSeedCommand;
  delete fDirectory;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FCCPythiaMessenger::SetNewValue(G4UIcommand* aCommand, G4String aNewValues)
{

   if(aCommand == fVerboseCommand)
   {
      G4int level= fVerboseCommand-> GetNewIntValue(aNewValues);
      fGenerator-> SetVerboseLevel(level);
   } else if (aCommand == fPrintCommand)
   {
      fGenerator-> Print();
   } else if (aCommand == fInitCommand)
   {
      const char* strvaluelist= aNewValues.c_str();
      std::istringstream is(strvaluelist);
      G4int sbeam, starget; G4double dwin;
      is >> sbeam >> starget >> dwin;
      fGenerator-> CallPythiaInit(sbeam, starget, dwin);

   } else if (aCommand == fStatCommand)
   {
      fGenerator-> CallPythiaStat();

   } else if (aCommand == fReadCommand)
   {
      G4String s= aNewValues;
      fGenerator-> CallPythiaReadString(s);

   } else if (aCommand == fSeedCommand)
   {
      G4int iseed= fSeedCommand-> GetNewIntValue(aNewValues);
      fGenerator-> SetRandomSeed(iseed);

   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4String FCCPythiaMessenger::GetCurrentValue(G4UIcommand* aCommand)
{
  G4String cv;
  if (aCommand == fVerboseCommand)
  {
    cv= fVerboseCommand-> ConvertToString(fGenerator->GetVerboseLevel());
  }
  return cv;
}

#endif
