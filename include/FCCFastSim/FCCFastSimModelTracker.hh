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

#ifndef FCC_TRACKER_FAST_SIM_MODEL_H
#define FCC_TRACKER_FAST_SIM_MODEL_H

#include "G4VFastSimulationModel.hh"
#include "FCCDetectorParametrisation.hh"
#include "G4Step.hh"
#include "G4Navigator.hh"

/**
	@brief     Shortcut to the ordinary tracking for tracking detectors.
   @details   Fast simulation model describes what should be done instead of a normal tracking. Instead of the ordinary tracking, a particle momentum at the entrance to the detector is smeared (by FCCSmearer::SmearMomentum()) and the particle is place at the detector exit, at the place it would reach without the change of momentum. Based on G4 examples/extended/parametrisations/Par01/include/Par01EMShowerModel.hh.
   @author    Anna Zaborowska
*/

class FCCFastSimModelTracker : public G4VFastSimulationModel
{
public:
   /**
      A constructor.
      @param aModelName A name of the fast simulation model.
      @param aEnvelope A region where the model can take over the ordinary tracking.
      @param aParamType A parametrisation type.
    */
   FCCFastSimModelTracker (G4String aModelName, G4Region* aEnvelope, FCCDetectorParametrisation::Parametrisation aParamType);
   /**
      A constructor.
      @param aModelName A name of the fast simulation model.
      @param aEnvelope A region where the model can take over the ordinary tracking.
    */
   FCCFastSimModelTracker (G4String aModelName, G4Region* aEnvelope);
   /**
      A constructor.
      @param aModelName A name of the fast simulation model.
    */
   FCCFastSimModelTracker (G4String aModelName);
   ~FCCFastSimModelTracker ();
   /**
      Checks if this model should be applied to this particle type.
      @param aParticle A particle definition (type).
    */
   virtual G4bool IsApplicable(const G4ParticleDefinition& aParticle);
   /**
      Checks if the model should be applied taking into account the kinematics of a track.
      @param aFastTrack A track.
    */
   virtual G4bool ModelTrigger(const G4FastTrack & aFastTrack);
   /**
      Calculates the final position (at the outer boundary of a detector) of a particle with the momentum at the entrance of the detector. Smears the particle momentum and saves it, together with the detector resolution and efficiency to the FCCPrimaryParticleInformation.
      @param aFastTrack A track.
      @param aFastStep A step.
    */
   virtual void DoIt(const G4FastTrack& aFastTrack, G4FastStep& aFastStep);


private:
   /**
      A pointer to FCCDetectorParametrisation used to get the efficiency and resolution of the detector for a given particle and parametrisation type (fParam).
    */
   FCCDetectorParametrisation* fCalculateParametrisation;
   /**
      A parametrisation type.
    */
   FCCDetectorParametrisation::Parametrisation fParametrisation;
};
#endif




