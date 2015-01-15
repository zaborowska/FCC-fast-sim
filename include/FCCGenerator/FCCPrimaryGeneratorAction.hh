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

#ifndef FCC_PRIMARY_GENERATOR_ACTION_H
#define FCC_PRIMARY_GENERATOR_ACTION_H

#include <map>
#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class G4Event;
class G4VPrimaryGenerator;
class FCCPrimaryGeneratorMessenger;

/**
	@brief     Primary generactor action.
 	@details   Defines the mandatory particle generator class. It allows to choose between different particle generators: user's particle gun implementation, Pythia8 generator or the events could be read from the ROOT file. Based on G4 examples/eventgenerator/HepMC/HepMCEx02/include/H02PrimaryGeneratorAction.hh.
 	@author    Anna Zaborowska
*/

class FCCPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
   /**
      A default constructor.
   */
   FCCPrimaryGeneratorAction();
   ~FCCPrimaryGeneratorAction();
   /**
      A method invoked from G4RunManager during the event loop. It calls the selected particle generator in order to generate an event.
   */
   virtual void GeneratePrimaries(G4Event* anEvent);

   /**
      A setter of the current particle generator.
      @param gen A pointer to the particle generator.
   */
   void SetGenerator(G4VPrimaryGenerator* gen);
   /**
      A setter of the current particle generator.
      @param genname A name of the particle generator (set in the constructor).
   */
   void SetGenerator(G4String genname);
   /**
      A getter of the current particle generator.
   */
   G4VPrimaryGenerator* GetGenerator() const;
   /**
      A getter of the name of the current particle generator.
   */
   G4String GetGeneratorName() const;

private:
   /**
      A pointer to the particle gun (FCCParticleGun) .
   */
   G4VPrimaryGenerator* particleGun;
   /**
      A pointer to the Pythia generator (FCCPythiaInterface) .
   */
   G4VPrimaryGenerator* pythia8Gen;
   /**
      A pointer to the reader of the ROOT files with events stored in HepMC format (FCCRootReader) .
   */
   G4VPrimaryGenerator* hepmcRoot;
   /**
      A pointer to the current particle generator.
   */
   G4VPrimaryGenerator* currentGenerator;
   /**
      A name of the current particle generator.
   */
   G4String currentGeneratorName;
   /**
      A map storing the pointers to the particle generators and their names.
   */
   std::map<G4String, G4VPrimaryGenerator*> gentypeMap;
   /**
      A pointer to the messenger that connects the UI with this class.
   */
   FCCPrimaryGeneratorMessenger* messenger;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void FCCPrimaryGeneratorAction::SetGenerator(G4VPrimaryGenerator* gen)
{
   currentGenerator= gen;
}

inline void FCCPrimaryGeneratorAction::SetGenerator(G4String genname)
{
  std::map<G4String, G4VPrimaryGenerator*>::iterator
       pos = gentypeMap.find(genname);
  if(pos != gentypeMap.end()) {
    currentGenerator= pos->second;
    currentGeneratorName= genname;
  }
}

inline G4VPrimaryGenerator* FCCPrimaryGeneratorAction::GetGenerator() const
{
  return currentGenerator;
}

inline G4String FCCPrimaryGeneratorAction::GetGeneratorName() const
{
  return currentGeneratorName;
}

#endif
