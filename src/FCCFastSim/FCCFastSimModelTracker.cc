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

#include "G4PathFinder.hh"
#include "G4FieldTrack.hh"
#include "G4FieldTrackUpdator.hh"

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
// ================
   G4double distance = fastTrack.GetEnvelopeSolid()->DistanceToOut( fastTrack.GetPrimaryTrackLocalPosition(),
                                                                    fastTrack.GetPrimaryTrackLocalDirection() );
   G4ThreeVector position = fastTrack.GetPrimaryTrackLocalPosition() +  distance*fastTrack.GetPrimaryTrackLocalDirection();
   fastStep.ProposePrimaryTrackFinalPosition( position );
   // fastStep.KillPrimaryTrack();
   // fastStep.ProposePrimaryTrackPathLength(0.0);
   // G4double Edep = fastTrack.GetPrimaryTrack()->GetKineticEnergy();
   //       fastStep.ProposeTotalEnergyDeposited(Edep);

    // G4double retSafety= -1.0;
    // ELimited retStepLimited;
    // G4FieldTrack* endTrack ;
    // G4FieldTrack* startTrack = G4FieldTrackUpdator::CreateFieldTrack(fastTrack.GetPrimaryTrack());
    // G4double returnedStep
    //    = G4PathFinder::GetInstance()->ComputeStep(*startTrack,
    //                               1000.,
    //                               0,
    //                               fastTrack.GetPrimaryTrack()->GetCurrentStepNumber(),
    //                               retSafety,
    //                               retStepLimited,
    //                               *endTrack,
    //                               fastTrack.GetPrimaryTrack()->GetVolume());
    // G4cout<<"step : "<<returnedStep<<G4endl;

// ================

   // smear according to tracker resolution
   G4int PID = fastTrack.GetPrimaryTrack()->GetDynamicParticle()->GetPDGcode();
   G4ThreeVector Porg = fastTrack.GetPrimaryTrack()->GetMomentum();

   if ( !fastTrack.GetPrimaryTrack()->GetParentID() )
   {
      if (((FCCEventInformation*) G4EventManager::GetEventManager()->GetUserInformation())->GetDoSmearing())
      {
         G4double res = fCalcParam->GetResolution(FCCDetectorParametrisation::eTracker, fParam, Porg.mag());
         G4double eff = fCalcParam->GetEfficiency(FCCDetectorParametrisation::eTracker, fParam, Porg.mag());
         G4ThreeVector Psm = FCCSmearer::Instance()->Smear(fastTrack.GetPrimaryTrack(), res);
         FCCOutput::Instance()->FillHistogram(0,Porg.mag() - Psm.mag() );
         // setting values of Psm, res and eff
         ((FCCPrimaryParticleInformation*)(const_cast<G4PrimaryParticle*>
                                           (fastTrack.GetPrimaryTrack()->GetDynamicParticle()->GetPrimaryParticle())->GetUserInformation()))->SetTrackerMomentum(Psm);
         ((FCCPrimaryParticleInformation*)(const_cast<G4PrimaryParticle*>
                                           (fastTrack.GetPrimaryTrack()->GetDynamicParticle()->GetPrimaryParticle())->GetUserInformation()))->SetTrackerResolution(res);
         ((FCCPrimaryParticleInformation*)(const_cast<G4PrimaryParticle*>
                                           (fastTrack.GetPrimaryTrack()->GetDynamicParticle()->GetPrimaryParticle())->GetUserInformation()))->SetTrackerEfficiency(eff);
      }
      else
      {
         // setting a value of Porg
         ((FCCPrimaryParticleInformation*)(const_cast<G4PrimaryParticle*>
                                           (fastTrack.GetPrimaryTrack()->GetDynamicParticle()->GetPrimaryParticle())->GetUserInformation()))->SetTrackerMomentum(Porg);
      }
   }
}
