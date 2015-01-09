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
// based on G4 examples/extended/parametrisations/Par01/include/Par01PhysicsList.hh
//

#ifndef FCC_PHYSICS_LIST_H
#define FCC_PHYSICS_LIST_H

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

/**
	@brief     Construction of a physics list.
 	@details   A mandatory initialization class of the physics list. For the purposes of fast simulation, only transportation, decays and parametrisation is used. Based on G4 examples/extended/parametrisations/Par01/include/Par01PhysicsList.hh.
 	@author    Anna Zaborowska
 */

class FCCPhysicsList: public G4VUserPhysicsList
{
public:
  FCCPhysicsList();
  virtual ~FCCPhysicsList();

protected:
  // Construct particle and physics
  virtual void ConstructParticle();
  virtual void ConstructProcess();

  //
  virtual void SetCuts();

protected:
  // these methods Construct particles
  virtual void ConstructBosons();
  virtual void ConstructLeptons();
  virtual void ConstructMesons();
  virtual void ConstructBaryons();
  virtual void ConstructIons();

protected:
  // these methods Construct physics processes and register them
  void AddParameterisation();

  virtual void ConstructGeneral();
  virtual void ConstructEM();

  virtual void AddTransportation();
};

#endif



