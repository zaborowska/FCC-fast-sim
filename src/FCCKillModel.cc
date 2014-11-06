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
//
// $Id: FCCKillModel.cc 77940 2013-11-29 15:15:17Z gcosmo $
//
#include "FCCKillModel.hh"

#include "G4Track.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "g4root.hh"

#include "Randomize.hh"

FCCKillModel::FCCKillModel(G4String modelName, G4Region* envelope)
  : G4VFastSimulationModel(modelName, envelope)
{
   G4cout<<"+++++++++++++ creating model attached to "<<envelope->GetName()<<G4endl;
}

FCCKillModel::FCCKillModel(G4String modelName)
  : G4VFastSimulationModel(modelName)
{
}

FCCKillModel::~FCCKillModel()
{
}

G4bool FCCKillModel::IsApplicable(const G4ParticleDefinition& particleType)
{
   G4cout<<"+++++++++++++ Killing model !!! IsApplicable "<<G4endl;
   return true;
}

G4bool FCCKillModel::ModelTrigger(const G4FastTrack& fastTrack)
{
   G4cout<<"+++++++++++++ Killing model !!! ModelTrigger "<<G4endl;
   return true;
}

void FCCKillModel::DoIt(const G4FastTrack& fastTrack,
                            G4FastStep& fastStep)
{
   // Kill the parameterised particle:
   G4cout<<"+++++++++++++ Killing particle !!! "<<fastTrack.GetPrimaryTrack()->GetDefinition()->GetParticleName()<< " in "<<
      fastTrack.GetEnvelope()->GetName()<< G4endl;
   fastStep.KillPrimaryTrack();
   fastStep.ProposePrimaryTrackPathLength(0.0);
   fastStep.ProposeTotalEnergyDeposited(fastTrack.GetPrimaryTrack()->GetKineticEnergy());
}
