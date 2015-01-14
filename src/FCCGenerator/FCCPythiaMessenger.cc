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
FCCPythiaMessenger::FCCPythiaMessenger(FCCPythiaInterface* agen)
  : gen(agen)
{
  dir= new G4UIdirectory("/generator/pythia8/");
  dir-> SetGuidance("Commands for Pythia 8 event generation");

  verbose= new G4UIcmdWithAnInteger("/generator/pythia8/verbose",this);
  verbose-> SetGuidance("set verbose level");
  verbose-> SetParameterName("verboseLevel", false, false);
  verbose-> SetRange("verboseLevel>=0 && verboseLevel<=2");

  print= new G4UIcmdWithoutParameter("/generator/pythia8/print", this);
  print-> SetGuidance("print user information.");

  cpythiainit= new G4UIcommand("/generator/pythia8/init", this);
  cpythiainit-> SetGuidance("call INIT");
  G4UIparameter* beam= new G4UIparameter("beam particle (pdg code)", 's', false);
  cpythiainit-> SetParameter(beam);
  G4UIparameter* target= new G4UIparameter("target particle (pdg code)", 's', false);
  cpythiainit-> SetParameter(target);
  G4UIparameter* eCM= new G4UIparameter("energy of system in CM frame (GeV)", 'd', false);
  cpythiainit-> SetParameter(eCM);

  cpythiastat= new G4UIcmdWithoutParameter("/generator/pythia8/stat", this);
  cpythiastat-> SetGuidance("print statistics");

  cpythiaread= new G4UIcommand("/generator/pythia8/read",this);
  cpythiaread-> SetGuidance("call PYTHIAREAD");
  G4UIparameter* parameter= new G4UIparameter ("Parameter", 's', false);
  cpythiaread-> SetParameter(parameter);

  setSeed= new G4UIcmdWithAnInteger("/generator/pythia8/setSeed", this);
  setSeed-> SetGuidance("set initial seed.");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCPythiaMessenger::~FCCPythiaMessenger()
{
  delete verbose;
  delete print;
  delete cpythiainit;
  delete cpythiastat;
  delete cpythiaread;
  delete setSeed;
  delete dir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FCCPythiaMessenger::SetNewValue(G4UIcommand* command,
                                     G4String newValues)
{

   if(command == verbose)
   {
      G4int level= verbose-> GetNewIntValue(newValues);
      gen-> SetVerboseLevel(level);
   } else if (command == print)
   {
      gen-> Print();
   } else if (command == cpythiainit)
   {
      const char* strvaluelist= newValues.c_str();
      std::istringstream is(strvaluelist);
      G4int sbeam, starget; G4double dwin;
      is >> sbeam >> starget >> dwin;
      gen-> CallPythiaInit(sbeam, starget, dwin);

   } else if (command == cpythiastat)
   {
      gen-> CallPythiaStat();

   } else if (command == cpythiaread)
   {
      G4String s= newValues;
      gen-> CallPythiaReadString(s);

   } else if (command == setSeed)
   {
      G4int iseed= setSeed-> GetNewIntValue(newValues);
      gen-> SetRandomSeed(iseed);

   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4String FCCPythiaMessenger::GetCurrentValue(G4UIcommand* command)
{
  G4String cv;
  if (command == verbose) {
    cv= verbose-> ConvertToString(gen->GetVerboseLevel());
  }
  return cv;
}

#endif
