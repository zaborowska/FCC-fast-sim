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

#ifndef FCC_PYTHIA_MESSENGER_H
#define FCC_PYTHIA_MESSENGER_H

#include "G4UImessenger.hh"

class FCCPythiaInterface;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;

/**
	@brief     A class delivering the commands from the UI to FCCPythiaInterface.
 	@details   A messenger class delivering the commands from the UI to FCCPythiaInterface. Based on G4 examples/eventgenerator/HepMC/HepMCEx01/include/HepMCG4PythiaMessenger.hh.
 	@author    Anna Zaborowska
*/
class FCCPythiaMessenger : public G4UImessenger {
private:
   /**
      A pointer to the FCCPythiaInterface class that is connected to the UI.
   */
   FCCPythiaInterface* gen;
   /**
      A directory for the commands associated with this messenger.
   */
   G4UIdirectory*           dir;
   /**
      A command that takes an integer parameter. Used to set the verbose level.
   */
   G4UIcmdWithAnInteger*    verbose;
   /**
      A command that takes no parameters. Used to print out information.
   */
   G4UIcmdWithoutParameter* print;
   /**
      A command that takes 3 parameters. Initializes Pythia. Parameters: beam particle (PDG code), target particle (PDG code) and energy of the system in the centre of mass frame.
   */
   G4UIcommand*             cpythiainit;
   /**
      A command that takes no parameters. Prints Pythia statistics.
   */
   G4UIcmdWithoutParameter* cpythiastat;
   /**
      A command that takes parameters. Allows Pythia to read the given string.
   */
   G4UIcommand*             cpythiaread;
   /**
      A command that takes an integer parameter. Sets Pythia seed.
   */
   G4UIcmdWithAnInteger*    setSeed;

public:
   /**
      A constructor. The directory and the commands names are set here.
      @param agen A pointer to the FCCPythiaInterface class object.
   */
   FCCPythiaMessenger(FCCPythiaInterface* agen);
   ~FCCPythiaMessenger();
   /**
      A method calling the methods from the FCCPythiaInterface class, depending on the command.
      @param command A pointer to the command typed by the user in the UI.
      @param newValues A new value of the command set by the user.
   */
   void SetNewValue(G4UIcommand* command, G4String newValues);
   /**
      A method calling the methods from the FCCPythiaInterface class, depending on the command.
      @param command A pointer to the command typed by the user in the UI.
   */
   G4String GetCurrentValue(G4UIcommand* command);
};

#endif
