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
/// \file eventgenerator/HepMC/HepMCEx02/src/H02PrimaryGeneratorAction.cc
//
#include "G4Event.hh"
#include "FCCParticleGun.hh"
#include "FCCRootReader.hh"
#include "FCCPythiaInterface.hh"
#include "FCCPrimaryGeneratorAction.hh"
#include "FCCPrimaryGeneratorMessenger.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCPrimaryGeneratorAction::FCCPrimaryGeneratorAction()
{
  // default generator is particle gun.
  currentGenerator= particleGun= new FCCParticleGun();
  currentGeneratorName= "particleGun";

  hepmcRoot = new FCCRootReader();
#ifdef G4LIB_USE_PYTHIA8
  pythia8Gen= new FCCPythiaInterface();
#else
  pythia8Gen= 0;
#endif

  gentypeMap["particleGun"]= particleGun;
#ifdef G4LIB_USE_PYTHIA8
  gentypeMap["pythia8"]= pythia8Gen;
#endif
  gentypeMap["hepmcRoot"]= hepmcRoot;

  messenger= new FCCPrimaryGeneratorMessenger(this);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCPrimaryGeneratorAction::~FCCPrimaryGeneratorAction()
{
  delete messenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FCCPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if(currentGenerator)
  {
    currentGenerator-> GeneratePrimaryVertex(anEvent);
  }
  else
    G4Exception("FCCPrimaryGeneratorAction::GeneratePrimaries",
                "InvalidSetup", FatalException,
                "Generator is not instanciated.");
}