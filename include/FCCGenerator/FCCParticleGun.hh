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

#ifndef FCC_PARTICLE_GUN_H
#define FCC_PARTICLE_GUN_H

#include "FCCParticleGunMessenger.hh"
#include "G4VPrimaryGenerator.hh"
#include "G4Event.hh"
#include "globals.hh"
#include "G4VPrimaryGenerator.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4PrimaryVertex.hh"
#include "G4ParticleMomentum.hh"

class FCCParticleGun: public G4VPrimaryGenerator
{
public:
   FCCParticleGun();
   virtual ~FCCParticleGun();

   void GeneratePrimaryVertex(G4Event* evt);

   void SetParticleDefinition
   (G4ParticleDefinition * aParticleDefinition);
   void SetParticleEnergy(G4double aKineticEnergy);
   void SetParticleMomentum(G4double aMomentum);
   void SetParticleMomentum(G4ParticleMomentum aMomentum);
   void SetParticleMomentumDirection
   (G4ParticleMomentum aMomentumDirection)
      { particle_momentum_direction =  aMomentumDirection.unit(); }
   void SetParticleCharge(G4double aCharge)
      { particle_charge = aCharge; }
   void SetParticlePolarization(G4ThreeVector aVal)
      { particle_polarization = aVal; }
   void SetNumberOfParticles(G4int i)
      { NumberOfParticlesToBeGenerated = i; }

   G4ParticleDefinition* GetParticleDefinition() const
      { return particle_definition; }
   G4ParticleMomentum GetParticleMomentumDirection() const
      { return particle_momentum_direction; }
   G4double GetParticleEnergy() const
      { return particle_energy; }
   G4double GetParticleMomentum() const
      { return particle_momentum; }
   G4double GetParticleCharge() const
      { return particle_charge; }
   G4ThreeVector GetParticlePolarization() const
      { return particle_polarization; }
   G4int GetNumberOfParticles() const
      { return NumberOfParticlesToBeGenerated; }

   G4int                 NumberOfParticlesToBeGenerated;
   G4ParticleDefinition* particle_definition;
   G4ParticleMomentum    particle_momentum_direction;
   G4double	           particle_energy;
   G4double              particle_momentum;
   G4double	           particle_charge;
   G4ThreeVector         particle_polarization;

private:
   FCCParticleGunMessenger* theMessenger;

};

#endif

