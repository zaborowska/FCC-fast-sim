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
// based on G4 examples/extended/parametrisations/Par01/include/Par01EMShowerModel.hh
//

#ifndef FCC_EMCAL_FAST_SIM_MODEL_H
#define FCC_EMCAL_FAST_SIM_MODEL_H

#include "G4VFastSimulationModel.hh"
#include "FCCDetectorParametrisation.hh"
#include "G4Step.hh"

/**
	@brief     A shortcut to the ordinary tracking for electromagnetic calorimeters.
   @details   Fast simulation model describes what should be done instead of a normal tracking. Instead of the ordinary tracking, a particle deposits its energy at the entrance to the detector and its value is smeared (by FCCSmearer::SmearMomentum()). Based on G4 examples/extended/parametrisations/Par01/include/Par01EMShowerModel.hh.
   @author    Anna Zaborowska
*/

class FCCFastSimModelEMCal : public G4VFastSimulationModel
{
public:
   /**
      A constructor.
      @param aModelName A name of the fast simulation model.
      @param aEnvelope A region where the model can take over the ordinary tracking.
      @param aParamType A parametrisation type.
    */
   FCCFastSimModelEMCal (G4String aModelName, G4Region* aEnvelope, FCCDetectorParametrisation::Parametrisation aParamType);
   /**
      A constructor.
      @param aModelName A name of the fast simulation model.
      @param aEnvelope A region where the model can take over the ordinary tracking.
    */
   FCCFastSimModelEMCal (G4String aModelName, G4Region* aEnvelope);
   /**
      A constructor.
      @param aModelName A name of the fast simulation model.
    */
   FCCFastSimModelEMCal (G4String aModelName);
   ~FCCFastSimModelEMCal ();
   /**
      Checks if this model should be applied to this particle type.
      @param aParticle A particle definition (type).
    */
   virtual G4bool IsApplicable(const G4ParticleDefinition&);
   /**
      Checks if the model should be applied taking into account the kinematics of a track.
      @param aFastTrack A track.
    */
   virtual G4bool ModelTrigger(const G4FastTrack &);
   /**
      Smears the energy deposit and saves it, together with the position of the deposit, the detector resolution and efficiency to the FCCPrimaryParticleInformation.
    */
   virtual void DoIt(const G4FastTrack&, G4FastStep&);


private:
   /**
      A pointer to FCCDetectorParametrisation used to get the efficiency and resolution of the detector for a given particle and parametrisation type (fParam).
    */
   FCCDetectorParametrisation* fCalcParam;
   /**
      A parametrisation type.
    */
   FCCDetectorParametrisation::Parametrisation fParam;
};
#endif




