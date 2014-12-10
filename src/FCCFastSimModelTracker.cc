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

#include "FCCFastSimModelTracker.hh"
#include "FCCEventInformation.hh"
#include "FCCPrimaryParticleInformation.hh"
#include "FCCSmearer.hh"
#include "FCCOutput.hh"

#include "G4Track.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "g4root.hh"

#include "Randomize.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Gamma.hh"

FCCFastSimModelTracker::FCCFastSimModelTracker(G4String modelName, G4Region* envelope, FCCDetectorParametrisation::Parametrisation type)
   : G4VFastSimulationModel(modelName, envelope), fCalcParam(), fParam(type)
{}

FCCFastSimModelTracker::FCCFastSimModelTracker(G4String modelName, G4Region* envelope)
   : G4VFastSimulationModel(modelName, envelope), fCalcParam(), fParam(FCCDetectorParametrisation::eCMS)
{}

FCCFastSimModelTracker::FCCFastSimModelTracker(G4String modelName)
   : G4VFastSimulationModel(modelName), fCalcParam(), fParam(FCCDetectorParametrisation::eCMS)
{}

FCCFastSimModelTracker::~FCCFastSimModelTracker()
{}

G4bool FCCFastSimModelTracker::IsApplicable(const G4ParticleDefinition& particleType)
{
   return  particleType.GetPDGCharge() != 0;
}

G4bool FCCFastSimModelTracker::ModelTrigger(const G4FastTrack& fastTrack)
{
   return ! (fastTrack.GetEnvelopeSolid()->DistanceToIn( fastTrack.GetPrimaryTrackLocalPosition () ));
}

void FCCFastSimModelTracker::DoIt(const G4FastTrack& fastTrack,
                            G4FastStep& fastStep)
{
   // move track to the edgeol/exec of tracker
// ================
   G4double distance = fastTrack.GetEnvelopeSolid()->DistanceToOut( fastTrack.GetPrimaryTrackLocalPosition(),
                                                                    fastTrack.GetPrimaryTrackLocalDirection() );
   G4ThreeVector position = fastTrack.GetPrimaryTrackLocalPosition() +  distance*fastTrack.GetPrimaryTrackLocalDirection();
   fastStep.ProposePrimaryTrackFinalPosition( position );

// ================

   // smear according to tracker resolution
   G4int PID = fastTrack.GetPrimaryTrack()->GetDynamicParticle()->GetPDGcode();
   G4ThreeVector Porg = fastTrack.GetPrimaryTrack()->GetMomentum();
   G4double res = fCalcParam->GetResolution(FCCDetectorParametrisation::eTracker, fParam, Porg.mag());
   G4double eff = fCalcParam->GetEfficiency(FCCDetectorParametrisation::eTracker, fParam, Porg.mag());

   if ( !fastTrack.GetPrimaryTrack()->GetParentID() )
   {
      if (((FCCEventInformation*) G4EventManager::GetEventManager()->GetUserInformation())->GetDoSmearing())
      {
         G4ThreeVector Psm = FCCSmearer::Instance()->Smear(fastTrack.GetPrimaryTrack(), res);
         FCCOutput::Instance()->FillHistogram(0,Porg.mag() - Psm.mag() );
         FCCOutput::Instance()->SaveTrack(FCCOutput::eTracker, fastTrack.GetPrimaryTrack()->GetTrackID(), PID, Psm, res, eff);
      }
      else
      {
         FCCOutput::Instance()->SaveTrack(FCCOutput::eTracker, fastTrack.GetPrimaryTrack()->GetTrackID(), PID, Porg);
      }
   }
}
