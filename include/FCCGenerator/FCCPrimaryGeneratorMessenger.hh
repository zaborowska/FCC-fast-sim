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

#ifndef FCC_PRIMARY_GENERATOR_MESSENGER_H
#define FCC_PRIMARY_GENERATOR_MESSENGER_H

#include "globals.hh"
#include "G4UImessenger.hh"

class FCCPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAString;

/**
	@brief     Communicator between UI and FCCPrimaryGeneratorAction.
 	@details   A messenger class delivering the commands from the UI to FCCPrimaryGeneratorAction. Based on G4 examples/eventgenerator/HepMC/HepMCEx02/include/H02PrimaryGeneratorMessenger.hh.
 	@author    Anna Zaborowska
*/

class FCCPrimaryGeneratorMessenger : public G4UImessenger {
public:
   /**
      A constructor. The directory and the command names are set here.
      @param aGeneratorAction A pointer to the FCCPrimaryGeneratorAction class object.
   */
   FCCPrimaryGeneratorMessenger(FCCPrimaryGeneratorAction* aGeneratorAction);
   ~FCCPrimaryGeneratorMessenger();
   /**
      Calls the methods from the FCCPrimaryGeneratorAction class, depending on the command.
      @param aCommand A pointer to the command typed by the user in the UI.
      @param aNewValues A new value of the command set by the user.
   */
   void SetNewValue(G4UIcommand* aCommand, G4String aNewValues);
   /**
      Calls the methods from the FCCPrimaryGeneratorAction class, depending on the command.
      @param aCommand A pointer to the command typed by the user in the UI.
   */
   G4String GetCurrentValue(G4UIcommand* aCommand);

private:
   /**
      A pointer to the FCCPrimaryGeneratorAction class that is connected to the UI.
   */
   FCCPrimaryGeneratorAction* fPrimaryAction;
   /**
      A directory for the commands associated with this messenger.
   */
   G4UIdirectory* fDirectory;
   /**
      A command that can take a string. Used for selection of the particle generator.
   */
   G4UIcmdWithAString* fSelectCommand;

};

#endif
