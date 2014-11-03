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
// based on G4 examples/extended/parametrisations/Par01/include/Par01EMShowerModel.hh
//
//----------------------------------------------
// Parameterisation of e+/e-/gamma producing hits
// The hits are the same as defined in the detailed
// simulation.
//----------------------------------------------

#ifndef FCC_EM_SHOWER_MODEL_H
#define FCC_EM_SHOWER_MODEL_H

#include "FCCEnergySpot.hh"
#include "G4VFastSimulationModel.hh"
#include "G4Step.hh"
#include "G4TouchableHandle.hh"
#include <vector>

class FCCEMShowerModel : public G4VFastSimulationModel
{
public:
  //-------------------------
  // Constructor, destructor
  //-------------------------
  FCCEMShowerModel (G4String, G4Region*);
  FCCEMShowerModel (G4String);
  ~FCCEMShowerModel ();

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
  void AssignSpotAndCallHit(const FCCEnergySpot &eSpot);
  void FillFakeStep(const FCCEnergySpot &eSpot);
  void Explode(const G4FastTrack&);
  void BuildDetectorResponse();
   void SaveParticle(const G4Track*);

private:
  G4Step                         *fFakeStep;
  G4StepPoint                    *fFakePreStepPoint, *fFakePostStepPoint;
  G4TouchableHandle              fTouchableHandle;
  G4Navigator                    *fpNavigator;
  G4bool                         fNaviSetup;
  G4Material*                    fCsI;

  std::vector<FCCEnergySpot> feSpotList;

};
#endif




