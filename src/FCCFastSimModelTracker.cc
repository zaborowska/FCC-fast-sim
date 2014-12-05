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
#include "FCCOutput.hh"

#include "G4Track.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "g4root.hh"

#include "Randomize.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Gamma.hh"

FCCFastSimModelTracker::FCCFastSimModelTracker(G4String modelName, G4Region* envelope)
  : G4VFastSimulationModel(modelName, envelope)
{}

FCCFastSimModelTracker::FCCFastSimModelTracker(G4String modelName)
  : G4VFastSimulationModel(modelName)
{}

FCCFastSimModelTracker::~FCCFastSimModelTracker()
{}

G4bool FCCFastSimModelTracker::IsApplicable(const G4ParticleDefinition& particleType)
{
   return  particleType.GetPDGCharge() != 0;
      // &particleType == G4Electron::Definition() ||
      // &particleType == G4Positron::Definition() ||
      // &particleType == G4Gamma::Definition();
}

G4bool FCCFastSimModelTracker::ModelTrigger(const G4FastTrack& fastTrack)
{
   return true;//fastTrack.GetPrimaryTrack()->GetKineticEnergy() > 100*MeV;
}

void FCCFastSimModelTracker::DoIt(const G4FastTrack& fastTrack,
                            G4FastStep& fastStep)
{
   // Kill the parameterised particle:
   fastStep.KillPrimaryTrack();
   fastStep.ProposePrimaryTrackPathLength(0.0);
   fastStep.ProposeTotalEnergyDeposited(fastTrack.GetPrimaryTrack()->GetKineticEnergy());

   // here Smear according to tracker resolution

   G4int PID = fastTrack.GetPrimaryTrack()->GetDynamicParticle()->GetPDGcode();
   G4double res =  Resolution(PID, fastTrack.GetPrimaryTrack()->GetMomentum());
   G4double eff = Efficiency(PID, fastTrack.GetPrimaryTrack()->GetMomentum());

   if ( !fastTrack.GetPrimaryTrack()->GetParentID() )
   {
      // G4double mass = fastTrack.GetPrimaryTrack()->GetDynamicParticle()->GetDefinition()->GetPDGMass();
      // G4double q = fastTrack.GetPrimaryTrack()->GetDynamicParticle()->GetCharge();
      if (((FCCEventInformation*) G4EventManager::GetEventManager()->GetUserInformation())->GetDoSmearing())
      {
         //FCCSmearer::Instance()->Smear(fastTrack.GetPrimaryTrack());
         G4ThreeVector P = fastTrack.GetPrimaryTrack()->GetMomentum() * res;
         FCCOutput::Instance()->SaveTrack(true, PID, PID, P, res, eff);
         FCCOutput::Instance()->FillHistogram(0,P.mag() - fastTrack.GetPrimaryTrack()->GetMomentum().mag() );
         G4cout<<" filling hist with "<<P.mag() - fastTrack.GetPrimaryTrack()->GetMomentum().mag()<<G4endl;
      }
      else
      {
         G4ThreeVector P = fastTrack.GetPrimaryTrack()->GetMomentum();
         FCCOutput::Instance()->SaveTrack(true, PID, PID, P);
      }
   }
}

G4double FCCFastSimModelTracker::Resolution(const G4int PID, const G4ThreeVector mom) const
{
   if(PID)
      return 0.013;
   else
      return 0;
}

G4double FCCFastSimModelTracker::Efficiency(const G4int PID, const G4ThreeVector mom) const
{
   if(PID)
   {
      float pt = mom.perp();
      float eta = mom.eta();
      float effvalue = 0.;
      if (pt>1.0 && fabs(eta)<2.5)
         effvalue = 0.90;
      return effvalue;
   }
   else
      return 0;
}
