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
/// \file eventgenerator/HepMC/HepMCEx03/src/HepMCG4Pythia8Messenger.cc
/// \brief Implementation of the HepMCG4Pythia8Messenger class for Pythia 8
//
//

#ifdef G4LIB_USE_PYTHIA8

#include <sstream>
#include <fstream>
#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "HepMCG4Pythia8Messenger.hh"
#include "HepMCG4Pythia8Interface.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
HepMCG4Pythia8Messenger::HepMCG4Pythia8Messenger(HepMCG4Pythia8Interface* agen)
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

  setUserParameters=
    new G4UIcmdWithoutParameter("/generator/pythia8/setUserParameters",this);
  setUserParameters->
    SetGuidance("Set user parameters in the Pythia common blocks");

  setSeed= new G4UIcmdWithAnInteger("/generator/pythia8/setSeed", this);
  setSeed-> SetGuidance("set initial seed.");

  printRandomStatus=
    new G4UIcmdWithAString("/generator/pythia8/printRandomStatus", this);
  printRandomStatus-> SetGuidance("print random number status.");
  printRandomStatus-> SetParameterName("filename", true, false);
  printRandomStatus-> SetDefaultValue("std::cout");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
HepMCG4Pythia8Messenger::~HepMCG4Pythia8Messenger()
{
  delete verbose;
  delete print;
  delete cpythiainit;
  delete cpythiastat;
  delete cpythiaread;
  delete setUserParameters;
  delete setSeed;
  delete printRandomStatus;

  delete dir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HepMCG4Pythia8Messenger::SetNewValue(G4UIcommand* command,
                                         G4String newValues)
{

  if(command == verbose) {  // /verbose ...
    G4int level= verbose-> GetNewIntValue(newValues);
    gen-> SetVerboseLevel(level);

  } else if (command == print) { // /print ...
    gen-> Print();

  } else if (command == cpythiainit) { // /pythiainit ...
    const char* strvaluelist= newValues.c_str();
    std::istringstream is(strvaluelist);
    G4int sbeam, starget; G4double dwin;
    is >> sbeam >> starget >> dwin;
    gen-> CallPythiaInit(sbeam, starget, dwin);

  } else if (command == cpythiastat) { // /pythiastat ...
    gen-> CallPythiaStat();

  } else if (command == cpythiaread) { // /pythiaread ...
    G4String s= newValues;
    gen-> CallPythiaReadString(s);

  } else if (command == setUserParameters) { // /setUserParameters ...
    gen-> SetUserParameters();

  } else if (command == setSeed) { // /setSeed ...
    G4int iseed= setSeed-> GetNewIntValue(newValues);
    gen-> SetRandomSeed(iseed);

  } else if (command == printRandomStatus) { // /printRandomStatus ...
    G4String s= newValues;
    if (newValues == "std::cout") {
      gen-> PrintRandomStatus();
    } else {
      // to a file (overwrite mode)
      std::ofstream ofs;
      ofs.open(s.c_str(), std::ios::out);
      //ofs.open(randomStatusFileName.c_str(), std::ios::out|std::ios::app);
      ofs.setf(std::ios::fixed | std::ios::showpoint);
      gen-> PrintRandomStatus(ofs);
      ofs.close();
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4String HepMCG4Pythia8Messenger::GetCurrentValue(G4UIcommand* command)
{
  G4String cv;
  if (command == verbose) {
    cv= verbose-> ConvertToString(gen->GetVerboseLevel());
  }
  return cv;
}

#endif
