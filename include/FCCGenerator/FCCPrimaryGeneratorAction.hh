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
      @param aEvent A generated event.
   */
   virtual void GeneratePrimaries(G4Event* aEvent);

   /**
      A setter of the current particle generator.
      @param aGenerator A pointer to the particle generator.
   */
   void SetGenerator(G4VPrimaryGenerator* aGenerator);
   /**
      A setter of the current particle generator.
      @param aGeneratorName A name of the particle generator (set in the constructor).
   */
   void SetGenerator(G4String aGeneratorName);
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
   G4VPrimaryGenerator* fParticleGun;
   /**
      A pointer to the Pythia generator (FCCPythiaInterface) .
   */
   G4VPrimaryGenerator* fPythia8;
   /**
      A pointer to the reader of the ROOT files with events stored in HepMC format (FCCRootReader) .
   */
   G4VPrimaryGenerator* fHepMCRoot;
   /**
      A pointer to the current particle generator.
   */
   G4VPrimaryGenerator* fCurrentGenerator;
   /**
      A name of the current particle generator.
   */
   G4String fCurrentGeneratorName;
   /**
      A map storing the pointers to the particle generators and their names.
   */
   std::map<G4String, G4VPrimaryGenerator*> fGeneratorMap;
   /**
      A pointer to the messenger that connects the UI with this class.
   */
   FCCPrimaryGeneratorMessenger* fMessenger;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void FCCPrimaryGeneratorAction::SetGenerator(G4VPrimaryGenerator* aGenerator)
{
   fCurrentGenerator= aGenerator;
}

inline void FCCPrimaryGeneratorAction::SetGenerator(G4String aGeneratorName)
{
  std::map<G4String, G4VPrimaryGenerator*>::iterator
       pos = fGeneratorMap.find(aGeneratorName);
  if(pos != fGeneratorMap.end())
  {
    fCurrentGenerator= pos->second;
    fCurrentGeneratorName= aGeneratorName;
  }
}

inline G4VPrimaryGenerator* FCCPrimaryGeneratorAction::GetGenerator() const
{
  return fCurrentGenerator;
}

inline G4String FCCPrimaryGeneratorAction::GetGeneratorName() const
{
  return fCurrentGeneratorName;
}

#endif
