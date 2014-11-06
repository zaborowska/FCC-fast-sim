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
// $Id: FCCHadSmearModel.cc 77940 2013-11-29 15:15:17Z gcosmo $
//
#include "FCCHadSmearModel.hh"
#include "FCCPrimaryParticleInformation.hh"
#include "FCCEventInformation.hh"

#include "Randomize.hh"

#include "AtlfastPionMatrixManager.hh"

#include "G4Track.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "g4root.hh"

#include "G4PionPlus.hh"
#include "G4PionMinus.hh"

FCCHadSmearModel::FCCHadSmearModel(G4String modelName, G4Region* envelope)
: G4VFastSimulationModel(modelName, envelope)
{}

FCCHadSmearModel::FCCHadSmearModel(G4String modelName)
: G4VFastSimulationModel(modelName)
{}

FCCHadSmearModel::~FCCHadSmearModel()
{}

G4bool FCCHadSmearModel::IsApplicable(const G4ParticleDefinition& particleType)
{
  return
    &particleType == G4PionPlus::PionPlusDefinition() ||
    &particleType == G4PionMinus::PionMinusDefinition();
}

G4bool FCCHadSmearModel::ModelTrigger(const G4FastTrack& fastTrack)
{
   // Applies the parameterisation above some energy:
   return true;//astTrack.GetPrimaryTrack()->GetKineticEnergy() > 100*MeV;
}

void FCCHadSmearModel::DoIt(const G4FastTrack& fastTrack,
                     G4FastStep& fastStep)
{
  // Kill the parameterised particle:
  fastStep.KillPrimaryTrack();
  fastStep.ProposePrimaryTrackPathLength(0.0);
  fastStep.ProposeTotalEnergyDeposited(fastTrack.GetPrimaryTrack()->GetKineticEnergy());

   if ( !fastTrack.GetPrimaryTrack()->GetParentID() ) SaveParticle(fastTrack.GetPrimaryTrack());
}


void FCCHadSmearModel::SaveParticle(const G4Track* aTrackOriginal)
{
   const G4Event* event = G4RunManager::GetRunManager()->GetCurrentEvent();
   G4int evNo = event->GetEventID();
   G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
   CLHEP::HepSymMatrix sigma; // smear matrix for track
   std::vector<double> smearVariables;   // Vector of correlated gaussian variables

   Atlfast::PionMatrixManager* pionMatrixManager = Atlfast::PionMatrixManager::Instance();
   smearVariables = pionMatrixManager->getVariables( *aTrackOriginal, sigma );

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
   analysisManager->FillNtupleIColumn(1+2*evNo, 0, ((FCCPrimaryParticleInformation*) aTrackOriginal->GetDynamicParticle()->GetPrimaryParticle()->GetUserInformation())->GetID()   );
   analysisManager->FillNtupleIColumn(1+2*evNo, 1, aTrackOriginal->GetDynamicParticle()->GetPDGcode());
   analysisManager->FillNtupleDColumn(1+2*evNo, 2, Px);
   analysisManager->FillNtupleDColumn(1+2*evNo, 3, Py);
   analysisManager->FillNtupleDColumn(1+2*evNo, 4, Pz);
   analysisManager->AddNtupleRow(1+2*evNo);

   // G4cout<<"____Current particle : "<<aTrackOriginal->GetDefinition()->GetParticleName()<<" with ID  "<<aTrackOriginal->GetTrackID()<<G4endl;
   // G4cout<<" PHI : "<<originPhi<<" ->  "<<Phi<<G4endl;
   // G4cout<<" THETA : "<<originTheta<<" ->  "<<atan(1./cotTheta)<<G4endl;
   // G4cout<<" invPtCharge : "<<originPt<<" ->  "<<((double)(1./(double)invPtCharge))<<G4endl<<G4endl;

}
