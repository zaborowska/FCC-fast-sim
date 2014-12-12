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
// based on G4 examples/eventgenerator/HepMC/HepMCEx01/include/HepMCG4PythiaMessenger.hh
//

#ifndef FCC_PYTHIA_MESSENGER_H
#define FCC_PYTHIA_MESSENGER_H

#include "G4UImessenger.hh"

class FCCPythiaInterface;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;

class FCCPythiaMessenger : public G4UImessenger {
private:
  FCCPythiaInterface* gen;

  G4UIdirectory*           dir;
  G4UIcmdWithAnInteger*    verbose;
  G4UIcmdWithoutParameter* print;
  G4UIcommand*             cpythiainit;
  G4UIcmdWithoutParameter* cpythiastat;
  G4UIcommand*             cpythiaread;
  G4UIcommand*             setUserParameters;
  G4UIcmdWithAnInteger*    setSeed;
  G4UIcmdWithAString*      printRandomStatus;

public:
  FCCPythiaMessenger(FCCPythiaInterface* agen);
  ~FCCPythiaMessenger();

  void SetNewValue(G4UIcommand* command, G4String newValues);
  G4String GetCurrentValue(G4UIcommand* command);
};

#endif
