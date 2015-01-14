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

#include "FCCParticleGun.hh"
#include "FCCPrimaryParticleInformation.hh"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"
#include "G4MuonPlus.hh"

FCCParticleGun::FCCParticleGun()
{
   NumberOfParticlesToBeGenerated = 1;
   particle_definition = 0;
   G4ThreeVector zero;
   particle_momentum_direction = (G4ParticleMomentum)zero;
   particle_energy = 0.0;
   particle_momentum = 0.0;
   particle_position = zero;
   particle_time = 0.0;
   particle_polarization = zero;
   particle_charge = 0.0;
   theMessenger = new G4ParticleGunMessenger((G4ParticleGunMessenger*)this);
}

FCCParticleGun::~FCCParticleGun()
{
  delete theMessenger;
}

void FCCParticleGun::SetParticleDefinition
                 (G4ParticleDefinition * aParticleDefinition)
{
  if(!aParticleDefinition)
  {
    G4Exception("FCCParticleGun::SetParticleDefinition()","Event0101",
     FatalException,"Null pointer is given.");
  }
  if(aParticleDefinition->IsShortLived())
  {
    if(!(aParticleDefinition->GetDecayTable()))
    {
      G4ExceptionDescription ED;
      ED << "FCCParticleGun does not support shooting a short-lived particle without a valid decay table." << G4endl;
      ED << "FCCParticleGun::SetParticleDefinition for "
         << aParticleDefinition->GetParticleName() << " is ignored." << G4endl;
      G4Exception("FCCParticleGun::SetParticleDefinition()","Event0102",
      JustWarning,ED);
      return;
    }
  }
  particle_definition = aParticleDefinition;
  particle_charge = particle_definition->GetPDGCharge();
  if(particle_momentum>0.0)
  {
    G4double mass =  particle_definition->GetPDGMass();
    particle_energy =
                 std::sqrt(particle_momentum*particle_momentum+mass*mass)-mass;
  }
}

void FCCParticleGun::SetParticleEnergy(G4double aKineticEnergy)
{
  particle_energy = aKineticEnergy;
  if(particle_momentum>0.0){
    if(particle_definition){
      G4cout << "FCCParticleGun::" << particle_definition->GetParticleName()
             << G4endl;
    }else{
      G4cout << "FCCParticleGun::" << " " << G4endl;
    }
    G4cout << " was defined in terms of Momentum: "
           << particle_momentum/GeV << "GeV/c" << G4endl;
    G4cout << " is now defined in terms of KineticEnergy: "
           << particle_energy/GeV   << "GeV"   << G4endl;
    particle_momentum = 0.0;
  }
}

void FCCParticleGun::SetParticleMomentum(G4double aMomentum)
{
  if(particle_energy>0.0){
    if(particle_definition){
      G4cout << "FCCParticleGun::" << particle_definition->GetParticleName()
             << G4endl;
    }else{
      G4cout << "FCCParticleGun::" << " " << G4endl;
    }
    G4cout << " was defined in terms of KineticEnergy: "
           << particle_energy/GeV << "GeV"   << G4endl;
    G4cout << " is now defined in terms Momentum: "
           << aMomentum/GeV       << "GeV/c" << G4endl;
  }
  if(particle_definition==0)
  {
    G4cout <<"Particle Definition not defined yet for FCCParticleGun"<< G4endl;
    G4cout <<"Zero Mass is assumed"<<G4endl;
    particle_momentum = aMomentum;
    particle_energy = aMomentum;
  }
  else
  {
    G4double mass =  particle_definition->GetPDGMass();
    particle_momentum = aMomentum;
    particle_energy =
                 std::sqrt(particle_momentum*particle_momentum+mass*mass)-mass;
  }
}

void FCCParticleGun::SetParticleMomentum(G4ParticleMomentum aMomentum)
{
  if(particle_energy>0.0){
    if(particle_definition){
      G4cout << "G4ParticleGun::" << particle_definition->GetParticleName()
             << G4endl;
    }else{
      G4cout << "G4ParticleGun::" << " " << G4endl;
    }
    G4cout << " was defined in terms of KineticEnergy: "
           << particle_energy/GeV << "GeV"   << G4endl;
    G4cout << " is now defined in terms Momentum: "
           << aMomentum.mag()/GeV << "GeV/c" << G4endl;
  }
  if(particle_definition==0)
  {
    G4cout <<"Particle Definition not defined yet for G4ParticleGun"<< G4endl;
    G4cout <<"Zero Mass is assumed"<<G4endl;
    particle_momentum_direction =  aMomentum.unit();
    particle_momentum = aMomentum.mag();
    particle_energy = aMomentum.mag();
  }
  else
  {
    G4double mass =  particle_definition->GetPDGMass();
    particle_momentum = aMomentum.mag();
    particle_momentum_direction =  aMomentum.unit();
    particle_energy =
                 std::sqrt(particle_momentum*particle_momentum+mass*mass)-mass;
  }
}


void FCCParticleGun::GeneratePrimaryVertex(G4Event* evt)
{
   // randomize particle momentum direction
   G4double x0  = 0, y0  = 0, z0  = 0;
   G4double dx0 = 1, dy0 = 1, dz0 = 1;
   x0 = x0+ dx0*(G4UniformRand()-0.5);
   y0 = y0+ dy0*(G4UniformRand()-0.5);
   z0 = z0+ dz0*(G4UniformRand()-0.5);
   G4ThreeVector pos(x0,y0,z0);
   SetParticlePosition(pos);
   G4double px0  = 0, py0  = 0, pz0  = 0;
   G4double dpx0 = 1, dpy0 = 1, dpz0 = 1;
   px0 = px0+ dpx0*(G4UniformRand()-0.5);
   py0 = py0+ dpy0*(G4UniformRand()-0.5);
   pz0 = pz0+ dpz0*(G4UniformRand()-0.5);
   G4ThreeVector eP(px0,py0,pz0);
   SetParticleMomentumDirection(eP.unit());
   // ensure that particle pT = given energy in G4 macro
   // G4double pTval[] = {1,3,6,10,16,20,25,30,35,40,50,60,70,80,100}; // in GeV
   // G4double pTval[] = {10}; // in GeV
   // G4double Egiven = pTval[(int)(G4UniformRand()*sizeof(pTval)/sizeof(G4double))]*GeV;
   // SetParticleEnergy( Egiven/eP.unit().perp() );


   if(particle_definition==0) return;

   // create a new vertex
   G4PrimaryVertex* vertex = new G4PrimaryVertex(particle_position,particle_time);

   // create new primaries and set them to the vertex
   G4double mass =  particle_definition->GetPDGMass();
   for( G4int i=0; i<NumberOfParticlesToBeGenerated; i++ )
   {
      G4PrimaryParticle* particle =
         new G4PrimaryParticle(particle_definition);
      particle->SetKineticEnergy( particle_energy );
      particle->SetMass( mass );
      particle->SetMomentumDirection( particle_momentum_direction );
      particle->SetCharge( particle_charge );
      particle->SetPolarization(particle_polarization.x(),
                                particle_polarization.y(),
                                particle_polarization.z());
      particle->SetUserInformation( new FCCPrimaryParticleInformation(i, particle_definition->GetPDGEncoding() , (eP.unit())*particle_energy) );
      vertex->SetPrimary( particle );
   }

   evt->AddPrimaryVertex( vertex );
}
