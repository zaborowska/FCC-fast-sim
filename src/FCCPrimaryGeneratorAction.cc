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
#include "G4ParticleGun.hh"
#include "FCCRootReader.hh"
#include "FCCPythiaInterface.hh"
#include "FCCPrimaryGeneratorAction.hh"
#include "FCCPrimaryGeneratorMessenger.hh"

#include "G4MuonPlus.hh"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCPrimaryGeneratorAction::FCCPrimaryGeneratorAction()
{
  // default generator is particle gun.
  currentGenerator= particleGun= new G4ParticleGun();
  currentGeneratorName= "particleGun";
  // default particle kinematic
  ((G4ParticleGun*)particleGun)->SetParticleDefinition(G4MuonPlus::Definition());
  ((G4ParticleGun*)particleGun)->SetParticlePosition(G4ThreeVector(0.,0.,0.));
  ((G4ParticleGun*)particleGun)->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));

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
     if(currentGenerator == particleGun)
    {
       // randomized position
       G4double x0  = 0*cm, y0  = 0*cm, z0  = 0*cm;
       G4double px0  = 0, py0  = 0, pz0  = 0;
       G4double dx0 = 0.01*cm, dy0 = 0.01*cm, dz0 = 0.1*cm;
       G4double dpx0 = 0.01, dpy0 = 0.01, dpz0 = 0.1;
       x0 += dx0*(G4UniformRand()-0.5);
       y0 += dy0*(G4UniformRand()-0.5);
       z0 += dz0*(G4UniformRand()-0.5);
       px0 = px0+ dpx0*(G4UniformRand()-0.5);
       py0 = py0+ dpy0*(G4UniformRand()-0.5);
       pz0 = pz0+ dpz0*(G4UniformRand()-0.5);
       ((G4ParticleGun*)particleGun)->SetParticlePosition(G4ThreeVector(x0,y0,z0));
       ((G4ParticleGun*)particleGun)->SetParticleMomentumDirection(G4ThreeVector(px0,py0,pz0));
    }
    currentGenerator-> GeneratePrimaryVertex(anEvent);
  }
  else
    G4Exception("FCCPrimaryGeneratorAction::GeneratePrimaries",
                "InvalidSetup", FatalException,
                "Generator is not instanciated.");
}
