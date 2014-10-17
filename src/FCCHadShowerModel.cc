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
// $Id: FCCHadShowerModel.cc 77940 2013-11-29 15:15:17Z gcosmo $
//
#include "FCCHadShowerModel.hh"
#include "FCCEnergySpot.hh"

#include "Randomize.hh"

#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4Eta.hh"
#include "G4TransportationManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4TouchableHandle.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"

FCCHadShowerModel::FCCHadShowerModel(G4String modelName, G4Region* envelope)
: G4VFastSimulationModel(modelName, envelope)
{
  fFakeStep          = new G4Step();
  fFakePreStepPoint  = fFakeStep->GetPreStepPoint();
  fFakePostStepPoint = fFakeStep->GetPostStepPoint();
  fTouchableHandle   = new G4TouchableHistory();
  fpNavigator        = new G4Navigator();
  fNaviSetup         = false;
  fCsI               = 0;
}

FCCHadShowerModel::FCCHadShowerModel(G4String modelName)
: G4VFastSimulationModel(modelName)
{
  fFakeStep          = new G4Step();
  fFakePreStepPoint  = fFakeStep->GetPreStepPoint();
  fFakePostStepPoint = fFakeStep->GetPostStepPoint();
  fTouchableHandle   = new G4TouchableHistory();
  fpNavigator        = new G4Navigator();
  fNaviSetup         = false;
  fCsI               = 0;
}

FCCHadShowerModel::~FCCHadShowerModel()
{
  delete fFakeStep;
  delete fpNavigator;
}

G4bool FCCHadShowerModel::IsApplicable(const G4ParticleDefinition& particleType)
{
  return 
    &particleType == G4PionPlus::PionPlusDefinition() ||
    &particleType == G4PionMinus::PionMinusDefinition() ||
    &particleType == G4Eta::EtaDefinition();
}

G4bool FCCHadShowerModel::ModelTrigger(const G4FastTrack& fastTrack)
{
  // Applies the parameterisation above 100 MeV:
  return fastTrack.GetPrimaryTrack()->GetKineticEnergy() > 100*MeV;
}

void FCCHadShowerModel::DoIt(const G4FastTrack& fastTrack, 
                     G4FastStep& fastStep)
{
  // Kill the parameterised particle:
  fastStep.KillPrimaryTrack();
  fastStep.ProposePrimaryTrackPathLength(0.0);
  fastStep.ProposeTotalEnergyDeposited(fastTrack.GetPrimaryTrack()->GetKineticEnergy());

  // split into "energy spots" energy according to the shower shape:
  Explode(fastTrack);
  
  // and put those energy spots into the crystals:
  BuildDetectorResponse();
  
}


void FCCHadShowerModel::Explode(const G4FastTrack& fastTrack)
{
  //-----------------------------------------------------
  //
  //-----------------------------------------------------

  FCCEnergySpot eSpot;

  //Put all kinetic energy of particle into one spot 
  G4double Energy = fastTrack.GetPrimaryTrack()->GetKineticEnergy();
  eSpot.SetEnergy(Energy);

  // point of entry of the particle is also position of the spot
  G4ThreeVector StartPosition = fastTrack.GetPrimaryTrack()->GetPosition();

  eSpot.SetPosition(StartPosition);

  feSpotList.clear();
  feSpotList.push_back(eSpot);

}


void FCCHadShowerModel::BuildDetectorResponse()
{
  // Does the assignation of the energy spots to the sensitive volumes:
  for (size_t i = 0; i < feSpotList.size(); i++)
    {
      // Draw the energy spot:
      feSpotList[i].Draw();
      //      feSpotList[i].Print();
      
      // "converts" the energy spot into the fake
      // G4Step to pass to sensitive detector:
      AssignSpotAndCallHit(feSpotList[i]);
    }
}


void FCCHadShowerModel::AssignSpotAndCallHit(const FCCEnergySpot &eSpot)
{
  //
  // "converts" the energy spot into the fake
  // G4Step to pass to sensitive detector:
  //
  FillFakeStep(eSpot);

  //
  // call sensitive part: taken/adapted from the stepping:
  // Send G4Step information to Hit/Dig if the volume is sensitive
  //
  G4VPhysicalVolume* pCurrentVolume = 
    fFakeStep->GetPreStepPoint()->GetPhysicalVolume();
  G4VSensitiveDetector* pSensitive;
  
  if( pCurrentVolume != 0 )
    {
      pSensitive = pCurrentVolume->GetLogicalVolume()->
        GetSensitiveDetector();
      if( pSensitive != 0 )
        {
          pSensitive->Hit(fFakeStep);
        }
    }
}


void FCCHadShowerModel::FillFakeStep(const FCCEnergySpot &eSpot)
{
  //-----------------------------------------------------------
  // find in which volume the spot is.
  //-----------------------------------------------------------
  if (!fNaviSetup)
    {
      fpNavigator->
        SetWorldVolume(G4TransportationManager::GetTransportationManager()->
                       GetNavigatorForTracking()->GetWorldVolume());
      fpNavigator->
        LocateGlobalPointAndUpdateTouchableHandle(eSpot.GetPosition(),
                                            G4ThreeVector(0.,0.,0.),
                                            fTouchableHandle,
                                            false);
      fNaviSetup = true;
    }
  else
    {
      fpNavigator->
        LocateGlobalPointAndUpdateTouchableHandle(eSpot.GetPosition(),
                                            G4ThreeVector(0.,0.,0.),
                                            fTouchableHandle);
     }
  //--------------------------------------
  // Fills attribute of the G4Step needed
  // by our sensitive detector:
  //-------------------------------------
  // set touchable volume at PreStepPoint:
  fFakePreStepPoint->SetTouchableHandle(fTouchableHandle);
  // set total energy deposit:
  fFakeStep->SetTotalEnergyDeposit(eSpot.GetEnergy());
}
