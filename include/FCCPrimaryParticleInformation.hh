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

#ifndef FCC_PRIMARY_PARTICLE_INFORMATION_H
#define FCC_PRIMARY_PARTICLE_INFORMATION_H

#include "G4VUserPrimaryParticleInformation.hh"
#include "G4LorentzVector.hh"
#include "globals.hh"

class FCCPrimaryParticleInformation: public G4VUserPrimaryParticleInformation
{
  public:
    FCCPrimaryParticleInformation();
   FCCPrimaryParticleInformation(G4int, G4LorentzVector*, G4LorentzVector*);
   FCCPrimaryParticleInformation(G4int, G4double, G4double, G4double, G4double, G4double, G4double, G4double, G4double);
    virtual ~FCCPrimaryParticleInformation();

    virtual void Print() const;
   G4LorentzVector* GetMomentumProduction() const;
   G4LorentzVector* GetVertexProduction() const;
   G4int GetID() const;

  private:
   G4LorentzVector* vertexProduction;
   G4LorentzVector* momentumProduction; // initial momentum
   G4int id; // unique id within the event
};

#endif

