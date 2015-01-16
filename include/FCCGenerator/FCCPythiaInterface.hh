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

#ifndef FCC_PYTHIA_INTERFACE_H
#define FCC_PYTHIA_INTERFACE_H

#include "FCCHepMCInterface.hh"
#include "Pythia8/Pythia8ToHepMC.h"
#include "Pythia8/Pythia.h"

class FCCPythiaMessenger;

/**
	@brief     Interface to Pythia8 event generator.
 	@details   An interface to Pythia8 event generator via HepMC. Based on G4 examples/eventgenerator/HepMC/HepMCEx03/include/HepMCG4Pythia8Interface.hh.
 	@author    Anna Zaborowska
*/

class FCCPythiaInterface : public FCCHepMCInterface {
protected:
   /**
      A verbosity level.
   */
   G4int fVerbose;
   /**
      An interface to the HepMC standard format.
   */
   HepMC::Pythia8ToHepMC fToHepMC;
   /**
      Pythia8.
   */
   Pythia8::Pythia fPythia;
   /**
      A pointer to the messenger that connects the UI with this class.
   */
  FCCPythiaMessenger* fMessenger;
   /**
      Generates a Pythia event according to the parameters set with other methods. Converts it to HepMC standard format.
   */
  virtual HepMC::GenEvent* GenerateHepMCEvent();

public:
   /**
      A default constructor.
   */
  FCCPythiaInterface();
  ~FCCPythiaInterface();
   /**
      A setter of the verbosity level.
      @param aLevel A verbosity level.
   */
  void SetVerboseLevel(G4int aLevel);
   /**
      A getter of the verbosity level.
   */
  G4int GetVerboseLevel() const;
   /**
      Initializes Pythia8.
      @param aBeam The beam particle PDG code.
      @param aTarget The target particle PDG code.
      @param aEnergyCM The energy of the system in the centre of mass frame.
   */
  void CallPythiaInit(G4int aBeam, G4int aTarget, G4double aEnergyCM);
   /**
      Prints Pytia statistics.
   */
  void CallPythiaStat();
   /**
      Passes a string instruction to Pythia.
      @param aInstruction A string instruction.
   */
  void CallPythiaReadString(G4String aInstruction);
   /**
      Sets the random number seed.
      @param aSeed A seed.
   */
  void SetRandomSeed(G4int aSeed);
   /**
      Printing method.
   */
  virtual void Print() const;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void FCCPythiaInterface::SetVerboseLevel(G4int aLevel)
{
  fVerbose= aLevel;
}

inline G4int FCCPythiaInterface::GetVerboseLevel() const
{
  return fVerbose;
}

#endif
