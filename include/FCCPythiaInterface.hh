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
// based on G4 examples/eventgenerator/HepMC/HepMCEx03/include/HepMCG4Pythia8Interface.hh
//

#ifndef FCC_PYTHIA_INTERFACE_H
#define FCC_PYTHIA_INTERFACE_H

#include "FCCHepMCInterface.hh"
#include "Pythia8/Pythia8ToHepMC.h"
#include "Pythia8/Pythia.h"

class FCCPythiaMessenger;

/// A generic interface class with Pythia8 event generator via HepMC.

class FCCPythiaInterface : public FCCHepMCInterface {
protected:
   G4int verbose;
   HepMC::Pythia8ToHepMC ToHepMC;
   Pythia8::Pythia pythia;

  FCCPythiaMessenger* messenger;

  virtual HepMC::GenEvent* GenerateHepMCEvent();

public:
  FCCPythiaInterface();
  ~FCCPythiaInterface();

  // set/get methods
  void SetVerboseLevel(G4int i);
  G4int GetVerboseLevel() const;

  // call pyxxx
  void CallPythiaInit(G4int beam, G4int target, G4double eCM);
  void CallPythiaStat();
  void CallPythiaReadString(G4String par);

  // random numbers operations
  void SetRandomSeed(G4int iseed);
  void PrintRandomStatus(std::ostream& ostr=G4cout) const;

  // setup user parameters (empty in default).
  // Implement your parameters in a delived class if you want.
  virtual void SetUserParameters();

  virtual void Print() const;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
inline void FCCPythiaInterface::SetVerboseLevel(G4int i)
{
  verbose= i;
}

inline G4int FCCPythiaInterface::GetVerboseLevel() const
{
  return verbose;
}

#endif
