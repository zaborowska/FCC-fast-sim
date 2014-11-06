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
// based on G4 examples/extended/parametrisations/Par01/src/Par01PhysicsConstructor.cc
//

#include "FCCPhysicsConstructor.hh"
#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4ios.hh"
#include <iomanip>
#include "G4FastSimulationManagerProcess.hh"

FCCPhysicsConstructor::FCCPhysicsConstructor():  G4VPhysicsConstructor()
{}

FCCPhysicsConstructor::~FCCPhysicsConstructor()
{
}

void FCCPhysicsConstructor::ConstructProcess()
{
  AddParameterisation();
}
void FCCPhysicsConstructor::ConstructParticle(){}

void FCCPhysicsConstructor::AddParameterisation()
{
  // -- Fast simulation manager process for "mass geometry":
  G4FastSimulationManagerProcess*
    fastSimProcess_massGeom     = new G4FastSimulationManagerProcess();
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = G4Electron::Definition()->GetParticleTable()->GetIterator();
  theParticleIterator->reset();
  while( (*theParticleIterator)() )
    {
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();
      // -- For the mass geometry, G4FSMP is a PostStep process, ordering does not matter:
      if (particle->GetParticleName() == "pi+"  ||
          particle->GetParticleName() == "pi-"  ||
          particle->GetParticleName() == "mu+"  ||
          particle->GetParticleName() == "mu-"  ||
          particle->GetParticleName() == "e+"  ||
          particle->GetParticleName() == "e-"  ) pmanager->
                                                    AddDiscreteProcess(fastSimProcess_massGeom);

    }
}
