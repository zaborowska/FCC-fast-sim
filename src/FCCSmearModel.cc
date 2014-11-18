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
// $Id: FCCSmearModel.cc 77940 2013-11-29 15:15:17Z gcosmo $
//
#include "FCCSmearModel.hh"
#include "FCCTrackInformation.hh"
#include "FCCSmearer.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4MuonMinus.hh"
#include "G4MuonPlus.hh"

FCCSmearModel::FCCSmearModel(G4String modelName, G4Region* envelope)
  : G4VFastSimulationModel(modelName, envelope)
{}

FCCSmearModel::FCCSmearModel(G4String modelName)
  : G4VFastSimulationModel(modelName)
{}

FCCSmearModel::~FCCSmearModel()
{}

G4bool FCCSmearModel::IsApplicable(const G4ParticleDefinition& particleType)
{
   G4cout<<"    Particle entered the world..."<<G4endl;
   return
      &particleType == G4Electron::Definition() ||
      &particleType == G4Positron::Definition() ||
      &particleType == G4PionPlus::Definition() ||
      &particleType == G4PionMinus::Definition() ||
      &particleType == G4MuonPlus::Definition() ||
      &particleType == G4MuonMinus::Definition() ;
}

G4bool FCCSmearModel::ModelTrigger(const G4FastTrack& fastTrack)
{
   return (bool) ((fastTrack.GetPrimaryTrack()->GetVertexMomentumDirection()) == (fastTrack.GetPrimaryTrack()->GetMomentumDirection()));
}

void FCCSmearModel::DoIt(const G4FastTrack& fastTrack,
                            G4FastStep& fastStep)
{

   // fastStep.KillPrimaryTrack();
   // fastStep.ProposePrimaryTrackPathLength(0.0);
   // fastStep.ProposeTotalEnergyDeposited(fastTrack.GetPrimaryTrack()->GetKineticEnergy());

   G4Track* newTrack = FCCSmearer::Instance()->Smear(fastTrack.GetPrimaryTrack());

   fastStep.ProposePrimaryTrackFinalPosition(newTrack->GetPosition());
   fastStep.ProposePrimaryTrackFinalKineticEnergyAndDirection(
      newTrack->GetKineticEnergy(),
      newTrack->GetMomentumDirection()
      );
   G4cout<<"Flag: "<<(bool) ( ! ((FCCTrackInformation*)fastTrack.GetPrimaryTrack()->GetUserInformation())->GetSmeared())<<G4endl;
   ((FCCTrackInformation*)fastTrack.GetPrimaryTrack()->GetUserInformation())->SetSmeared(true);
   G4cout<<"Flag: "<<(bool) ( ! ((FCCTrackInformation*)fastTrack.GetPrimaryTrack()->GetUserInformation())->GetSmeared())<<G4endl;
}
