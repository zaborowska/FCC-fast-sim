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
// based on G4 examples/extended/eventgenerator/HepMC/HepMCEx01/src/HepMCG4PythiaInterface.cc
//

#ifdef G4LIB_USE_PYTHIA8

#include "FCCPythiaInterface.hh"
#include "FCCPythiaMessenger.hh"

#include "HepMC/GenEvent.h"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCPythiaInterface::FCCPythiaInterface() : fVerbose(0)
{
  fMessenger= new FCCPythiaMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCPythiaInterface::~FCCPythiaInterface()
{
  delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FCCPythiaInterface::CallPythiaReadString(G4String aInstruction)
{
   fPythia.readString(aInstruction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FCCPythiaInterface::CallPythiaInit(G4int aBeam, G4int aTarget, G4double aEnergyCM)
{
   fPythia.init(aBeam, aTarget, aEnergyCM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FCCPythiaInterface::CallPythiaStat()
{
   fPythia.stat();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FCCPythiaInterface::SetRandomSeed(G4int aSeed)
{
   fPythia.readString("Random:setSeed = on");
   ostringstream Seed;
   Seed<<"Random:seed = "<<aSeed;
   fPythia.readString(Seed.str());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
HepMC::GenEvent* FCCPythiaInterface::GenerateHepMCEvent()
{
   fPythia.next();
   HepMC::GenEvent* hepmcevt = new HepMC::GenEvent();
   fToHepMC.fill_next_event( fPythia, hepmcevt );
   if(fVerbose>0) hepmcevt-> print();

  return hepmcevt;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FCCPythiaInterface::Print() const
{
  G4cout << "PythiaInterface::Print()..." << G4endl;
}

#endif
