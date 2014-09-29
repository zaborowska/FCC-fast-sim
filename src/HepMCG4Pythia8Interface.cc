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
/// \file eventgenerator/HepMC/HepMCEx03/src/HepMCG4Pythia8Interface.cc
/// \brief Implementation of the HepMCG4PythiaInterface class for Pythia8
//

#ifdef G4LIB_USE_PYTHIA8

#include "HepMCG4Pythia8Interface.hh"
#include "HepMCG4Pythia8Messenger.hh"

#include "HepMC/GenEvent.h"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
HepMCG4Pythia8Interface::HepMCG4Pythia8Interface()
   : verbose(0)
{
  pythia.readString("HardQCD:all = on");
  pythia.readString("PhaseSpace:pTHatMin = 20.");
  messenger= new HepMCG4Pythia8Messenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
HepMCG4Pythia8Interface::~HepMCG4Pythia8Interface()
{
  delete messenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HepMCG4Pythia8Interface::CallPythiaReadString(G4String par)
{
   pythia.readString(par);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HepMCG4Pythia8Interface::CallPythiaInit(G4int beam,
                                        G4int target, G4double eCM)
{
   pythia.init(beam,target, eCM);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HepMCG4Pythia8Interface::CallPythiaStat()
{
   pythia.stat();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HepMCG4Pythia8Interface::SetRandomSeed(G4int iseed)
{
   pythia.readString("Random:setSeed = on");
   ostringstream Seed;
   Seed<<"Random:seed = "<<iseed;
   pythia.readString(Seed.str());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HepMCG4Pythia8Interface::PrintRandomStatus(std::ostream& ostr) const
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HepMCG4Pythia8Interface::SetUserParameters()
{
  G4cout << "set user parameters of PYTHIA common." << G4endl
         << "nothing to be done in default."
         << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
HepMC::GenEvent* HepMCG4Pythia8Interface::GenerateHepMCEvent()
{
   pythia.next();

   HepMC::GenEvent* hepmcevt = new HepMC::GenEvent();
   ToHepMC.fill_next_event( pythia, hepmcevt );
   if(verbose>0) hepmcevt-> print();

  return hepmcevt;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HepMCG4Pythia8Interface::Print() const
{
  G4cout << "Pythia8Interface::Print()..." << G4endl;
}

#endif
