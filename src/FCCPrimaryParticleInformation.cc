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

#include "FCCPrimaryParticleInformation.hh"

FCCPrimaryParticleInformation::FCCPrimaryParticleInformation()
{}

FCCPrimaryParticleInformation::FCCPrimaryParticleInformation(G4int ID): id(ID)
{}

FCCPrimaryParticleInformation::~FCCPrimaryParticleInformation()
{}

// G4LorentzVector* FCCPrimaryParticleInformation::GetVertexProduction() const
// {
//    return vertexProduction;
// }

// G4LorentzVector* FCCPrimaryParticleInformation::GetMomentumProduction() const
// {
//    return momentumProduction;
// }
G4int FCCPrimaryParticleInformation::GetID() const
{
   return id;
}

void FCCPrimaryParticleInformation::Print() const
{
   // G4cout<<"FCCPrimaryParticleInformation: "<<G4endl
   //       <<"Particle unique ID:  "<<id<<G4endl
   //       <<"Production vertex: ( "<<vertexProduction->x()<<" , "
   //       <<vertexProduction->y()<<" , "
   //       <<vertexProduction->z()<<" , "
   //       <<vertexProduction->t()<<" ) "<<G4endl
   //       <<"Initial momentum: ( "<<momentumProduction->px()<<" , "
   //       <<momentumProduction->py()<<" , "
   //       <<momentumProduction->pz()<<" , "
   //       <<momentumProduction->e()<<" ) "<<G4endl;
}
