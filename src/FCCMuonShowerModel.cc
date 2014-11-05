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
// $Id: FCCMuonShowerModel.cc 77940 2013-11-29 15:15:17Z gcosmo $
//
#include "FCCMuonShowerModel.hh"
#include "FCCEnergySpot.hh"
#include "FCCPrimaryParticleInformation.hh"
#include "FCCEventInformation.hh"

#include "AtlfastMuonMatrixManager.hh"

#include "G4Track.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "g4root.hh"

#include "Randomize.hh"

#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4TransportationManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4TouchableHandle.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"

FCCMuonShowerModel::FCCMuonShowerModel(G4String modelName, G4Region* envelope)
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

FCCMuonShowerModel::FCCMuonShowerModel(G4String modelName)
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

FCCMuonShowerModel::~FCCMuonShowerModel()
{
  delete fFakeStep;
  delete fpNavigator;
}

G4bool FCCMuonShowerModel::IsApplicable(const G4ParticleDefinition& particleType)
{
  return
    &particleType == G4MuonPlus::MuonPlusDefinition() ||
    &particleType == G4MuonMinus::MuonMinusDefinition();
}

G4bool FCCMuonShowerModel::ModelTrigger(const G4FastTrack& fastTrack)
{
  // Applies the parameterisation above 100 MeV:
   return fastTrack.GetPrimaryTrack()->GetKineticEnergy() > 100*MeV;
}

void FCCMuonShowerModel::DoIt(const G4FastTrack& fastTrack,
                     G4FastStep& fastStep)
{
   G4cout<<" Killing the muon !!"<<G4endl;

  // Kill the parameterised particle:
  fastStep.KillPrimaryTrack();
  fastStep.ProposePrimaryTrackPathLength(0.0);
  fastStep.ProposeTotalEnergyDeposited(fastTrack.GetPrimaryTrack()->GetKineticEnergy());

  // split into "energy spots" energy according to the shower shape:
  Explode(fastTrack);

  // and put those energy spots into the crystals:
  BuildDetectorResponse();

  //SaveParticle(fastTrack.GetPrimaryTrack());
}


void FCCMuonShowerModel::Explode(const G4FastTrack& fastTrack)
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


void FCCMuonShowerModel::BuildDetectorResponse()
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


void FCCMuonShowerModel::AssignSpotAndCallHit(const FCCEnergySpot &eSpot)
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


void FCCMuonShowerModel::FillFakeStep(const FCCEnergySpot &eSpot)
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


void FCCMuonShowerModel::SaveParticle(const G4Track* aTrackOriginal)
{
   const G4Event* event = G4RunManager::GetRunManager()->GetCurrentEvent();
   G4int evNo = event->GetEventID();
   G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

   CLHEP::HepSymMatrix sigma; // smear matrix for track
   std::vector<double> smearVariables;   // Vector of correlated gaussian variables

   Atlfast::MuonMatrixManager* muonMatrixManager = Atlfast::MuonMatrixManager::Instance();
   smearVariables = muonMatrixManager->getVariables( *aTrackOriginal, sigma );

   // helpful variables
   double originPhi = aTrackOriginal->GetMomentum().phi();
   double originTheta = aTrackOriginal->GetMomentum().theta();
   double originCharge = aTrackOriginal->GetDynamicParticle()->GetCharge();
   double originPt = aTrackOriginal->GetMomentum().perp();

   // Atlfast smeared variables
   double impactParameter; // [0]
   double zPerigee; //[1]
   double Phi = originPhi +  smearVariables[2]; //[2]
   if(Phi<-M_PI)
      while(Phi<M_PI)
         Phi+=2*M_PI;
   else if(Phi>M_PI)
      while(Phi>M_PI)
         Phi-=2*M_PI;
   double cotTheta = 1/tan(originTheta) + smearVariables[3] ; //[3]
   double invPtCharge = (double)((double)originCharge/(abs((double)originCharge)*originPt)) +  smearVariables[4]; // q/pT where q = q/|q| (just sign) //[4]

   // back to P
   double Px = abs(1./invPtCharge)*sin(Phi);
   double Py = abs(1./invPtCharge)*cos(Phi);
   double Pz = abs(1./invPtCharge)/sin( atan(1./cotTheta) );

   //saving to ntuple
   //analysisManager->FillNtupleIColumn((-1)*evNo, 0, ((FCCPrimaryParticleInformation*) aTrackOriginal->GetDynamicParticle()->GetPrimaryParticle()->GetUserInformation())->GetID()   );
   analysisManager->FillNtupleIColumn((-1)*evNo, 0, aTrackOriginal->GetTrackID()   );
   analysisManager->FillNtupleIColumn((-1)*evNo, 1, aTrackOriginal->GetDynamicParticle()->GetPDGcode());
   analysisManager->FillNtupleDColumn((-1)*evNo, 2, Px);
   analysisManager->FillNtupleDColumn((-1)*evNo, 3, Py);
   analysisManager->FillNtupleDColumn((-1)*evNo, 4, Pz);
   analysisManager->AddNtupleRow((-1)*evNo);

   G4cout<<"____Current particle : "<<aTrackOriginal->GetDefinition()->GetParticleName()<<" with ID  "<<aTrackOriginal->GetTrackID()<<G4endl;
   G4cout<<" PHI : "<<originPhi<<" ->  "<<Phi<<G4endl;
   G4cout<<" THETA : "<<originTheta<<" ->  "<<atan(1./cotTheta)<<G4endl;
   G4cout<<" invPtCharge : "<<originPt<<" ->  "<<((double)(1./(double)invPtCharge))<<G4endl<<G4endl;

}

