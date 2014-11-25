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

#include "FCCMuonSmearModel.hh"
#include "FCCEventInformation.hh"
#include "FCCOutput.hh"
#include "FCCSmearer.hh"

#include "G4Track.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "g4root.hh"

#include "Randomize.hh"

#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"

FCCMuonSmearModel::FCCMuonSmearModel(G4String modelName, G4Region* envelope)
: G4VFastSimulationModel(modelName, envelope)
{}

FCCMuonSmearModel::FCCMuonSmearModel(G4String modelName)
: G4VFastSimulationModel(modelName)
{}

FCCMuonSmearModel::~FCCMuonSmearModel()
{}

G4bool FCCMuonSmearModel::IsApplicable(const G4ParticleDefinition& particleType)
{
  return
    &particleType == G4MuonPlus::MuonPlusDefinition() ||
    &particleType == G4MuonMinus::MuonMinusDefinition();
}

G4bool FCCMuonSmearModel::ModelTrigger(const G4FastTrack& fastTrack)
{
  // Applies the parameterisation above some energy:
   return true;// fastTrack.GetPrimaryTrack()->GetKineticEnergy() > 100*MeV;
}

void FCCMuonSmearModel::DoIt(const G4FastTrack& fastTrack,
                             G4FastStep& fastStep)
{
   // Kill the parameterised particle:
   fastStep.KillPrimaryTrack();
   fastStep.ProposePrimaryTrackPathLength(0.0);
   fastStep.ProposeTotalEnergyDeposited(fastTrack.GetPrimaryTrack()->GetKineticEnergy());

   if ( !fastTrack.GetPrimaryTrack()->GetParentID() )
   {
      G4int PID = fastTrack.GetPrimaryTrack()->GetDynamicParticle()->GetPDGcode();
      G4double mass = fastTrack.GetPrimaryTrack()->GetDynamicParticle()->GetDefinition()->GetPDGMass();
      G4double q = fastTrack.GetPrimaryTrack()->GetDynamicParticle()->GetCharge();
      if (((FCCEventInformation*) G4EventManager::GetEventManager()->GetUserInformation())->GetDoSmearing())
      {
         G4double* params = FCCSmearer::Instance()->Smear(fastTrack.GetPrimaryTrack());
         FCCOutput::Instance()->SaveTrack(true, PID, PID, q,  params);
      }
      else
      {
         G4ThreeVector eP = fastTrack.GetPrimaryTrack()->GetVertexMomentumDirection();
         G4double Ekin = fastTrack.GetPrimaryTrack()->GetVertexKineticEnergy();
         G4ThreeVector P = eP * sqrt (Ekin*Ekin+2*mass*Ekin);
         G4ThreeVector pos = fastTrack.GetPrimaryTrack()->GetVertexPosition();
         FCCOutput::Instance()->SaveTrack(true, PID, PID, q,  P, pos);
      }
   }
}
