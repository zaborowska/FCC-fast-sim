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

#ifndef FCC_TRACKER_FAST_SIM_MODEL_H
#define FCC_TRACKER_FAST_SIM_MODEL_H

#include "G4VFastSimulationModel.hh"
#include "FCCDetectorParametrisation.hh"
#include "G4Step.hh"

class FCCFastSimModelTracker : public G4VFastSimulationModel
{
public:
   //-------------------------
   // Constructor, destructor
   //-------------------------
   FCCFastSimModelTracker (G4String, G4Region*, FCCDetectorParametrisation::Parametrisation);
   FCCFastSimModelTracker (G4String, G4Region*);
   FCCFastSimModelTracker (G4String);
   ~FCCFastSimModelTracker ();

   //------------------------------
   // Virtual methods of the base
   // class to be coded by the user
   //------------------------------

   // -- IsApplicable
   virtual G4bool IsApplicable(const G4ParticleDefinition&);
   // -- ModelTrigger
   virtual G4bool ModelTrigger(const G4FastTrack &);
   // -- User method DoIt
   virtual void DoIt(const G4FastTrack&, G4FastStep&);


private:
   FCCDetectorParametrisation* fCalcParam;
   FCCDetectorParametrisation::Parametrisation fParam;
};
#endif



