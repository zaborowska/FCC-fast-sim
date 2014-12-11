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

#include "FCCFastSimModelHCal.hh"
#include "FCCEventInformation.hh"
#include "FCCPrimaryParticleInformation.hh"
#include "FCCSmearer.hh"
#include "FCCOutput.hh"

#include "G4Track.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "g4root.hh"

#include "Randomize.hh"

FCCFastSimModelHCal::FCCFastSimModelHCal(G4String modelName, G4Region* envelope, FCCDetectorParametrisation::Parametrisation type)
   : G4VFastSimulationModel(modelName, envelope), fCalcParam(), fParam(type)
{}

FCCFastSimModelHCal::FCCFastSimModelHCal(G4String modelName, G4Region* envelope)
   : G4VFastSimulationModel(modelName, envelope), fCalcParam(), fParam(FCCDetectorParametrisation::eCMS)
{}

FCCFastSimModelHCal::FCCFastSimModelHCal(G4String modelName)
   : G4VFastSimulationModel(modelName), fCalcParam(), fParam(FCCDetectorParametrisation::eCMS)
{}

FCCFastSimModelHCal::~FCCFastSimModelHCal()
{}

G4bool FCCFastSimModelHCal::IsApplicable(const G4ParticleDefinition& particleType)
{
   return (particleType.GetQuarkContent(1) +
           particleType.GetQuarkContent(2) +
           particleType.GetQuarkContent(3) +
           particleType.GetQuarkContent(4) +
           particleType.GetQuarkContent(5) +
           particleType.GetQuarkContent(6) );
}

G4bool FCCFastSimModelHCal::ModelTrigger(const G4FastTrack& fastTrack)
{
   return true;
}

void FCCFastSimModelHCal::DoIt(const G4FastTrack& fastTrack,
                            G4FastStep& fastStep)
{
   // Kill the parameterised particle:
   fastStep.KillPrimaryTrack();
   fastStep.ProposePrimaryTrackPathLength(0.0);
   G4double Edep = fastTrack.GetPrimaryTrack()->GetKineticEnergy();

   // here Smear according to tracker resolution

   G4int PID = fastTrack.GetPrimaryTrack()->GetDynamicParticle()->GetPDGcode();
   G4ThreeVector Pos = fastTrack.GetPrimaryTrack()->GetPosition();

   if ( !fastTrack.GetPrimaryTrack()->GetParentID() )
   {
      if (((FCCEventInformation*) G4EventManager::GetEventManager()->GetUserInformation())->GetDoSmearing())
      {
         G4ThreeVector Porg = fastTrack.GetPrimaryTrack()->GetMomentum();
         G4double res = fCalcParam->GetResolution(FCCDetectorParametrisation::eHCal, fParam, Porg.mag());
         G4double eff = fCalcParam->GetEfficiency(FCCDetectorParametrisation::eHCal, fParam, Porg.mag());
         G4double Esm = FCCSmearer::Instance()->Smear(Edep, res);
         FCCOutput::Instance()->FillHistogram(2,Edep-Esm );
         ((FCCPrimaryParticleInformation*)(const_cast<G4PrimaryParticle*>
                                           (fastTrack.GetPrimaryTrack()->GetDynamicParticle()->GetPrimaryParticle())->GetUserInformation()))->SetHCalPosition(Pos);
         ((FCCPrimaryParticleInformation*)(const_cast<G4PrimaryParticle*>
                                           (fastTrack.GetPrimaryTrack()->GetDynamicParticle()->GetPrimaryParticle())->GetUserInformation()))->SetHCalEnergy(Esm);
         ((FCCPrimaryParticleInformation*)(const_cast<G4PrimaryParticle*>
                                           (fastTrack.GetPrimaryTrack()->GetDynamicParticle()->GetPrimaryParticle())->GetUserInformation()))->SetHCalResolution(res);
         ((FCCPrimaryParticleInformation*)(const_cast<G4PrimaryParticle*>
                                           (fastTrack.GetPrimaryTrack()->GetDynamicParticle()->GetPrimaryParticle())->GetUserInformation()))->SetHCalEfficiency(eff);
         fastStep.ProposeTotalEnergyDeposited(Esm);
      }
      else
      {
         ((FCCPrimaryParticleInformation*)(const_cast<G4PrimaryParticle*>
                                           (fastTrack.GetPrimaryTrack()->GetDynamicParticle()->GetPrimaryParticle())->GetUserInformation()))->SetHCalEnergy(Edep);
         fastStep.ProposeTotalEnergyDeposited(Edep);
      }
   }
}
