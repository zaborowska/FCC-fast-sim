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

#ifndef FCC_PRIMARY_PARTICLE_INFORMATION_H
#define FCC_PRIMARY_PARTICLE_INFORMATION_H

#include "G4VUserPrimaryParticleInformation.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

/**
	@brief     Primary particle information
   @details   Describes the information that can be assosiated with a G4PrimaryParticle class object.
 	@author    Anna Zaborowska
 */

class FCCPrimaryParticleInformation: public G4VUserPrimaryParticleInformation
{
public:
   /**
      A constructor.
      @param aID A unique particle ID within event.
      @param aPDG A PDG code of the particle.
      @param aMomentum An initial particle momentum (at the primary vertex).
    */
   FCCPrimaryParticleInformation(G4int aID, G4int aPDG, G4ThreeVector aMomentum);
   virtual ~FCCPrimaryParticleInformation();
   /**
      A printing method.
   */
   virtual void Print() const;
   /**
      A setter of the initial particle momentum (from particle generator).
      @param aMomentum The particle momentum.
   */
   void SetMCMomentum(G4ThreeVector aMomentum){momentumMC = aMomentum;};
   /**
      A getter of the initial particle momentum (from particle generator).
   */
   G4ThreeVector GetMCMomentum() {return momentumMC;};
   /**
      A setter of the particle momentum at the entrance to the tracker detector.
      @param aMomentum The particle momentum.
   */
   void SetTrackerMomentum(G4ThreeVector aMomentum){momentumTracker = aMomentum;};
   /**
      A getter of the particle momentum at the entrance to the tracker detector.
   */
   G4ThreeVector GetTrackerMomentum() {return momentumTracker;}
   /**
      A setter of the tracker detector resolution. Currently equal to -1 if AtlFast type of smearing is used.
      @param aResolution The detector resolution (particle type and momentum dependent).
   */
   void SetTrackerResolution(G4double aResolution) {resTracker = aResolution;};
   /**
      A getter of the tracking detector resolution. Currently equal to -1 if AtlFast type of smearing is used.
   */
   G4double GetTrackerResolution() {return resTracker;};
   /**
      A setter of the tracking detector efficiency. Currently not used (efficiency is 1).
      @param aEfficiency The detector efficiency.
   */
   void SetTrackerEfficiency(G4double aEfficiency) {effTracker = aEfficiency;};
   /**
      A getter of the tracker detector efficiency. Currently not used (efficiency is 1).
   */
   G4double GetTrackerEfficiency() {return effTracker;};
   /**
      A setter of the position of the energy deposit in the electromagnetic calorimeter.
      @param aPosition The position of the energy deposit.
   */
   void SetEMCalPosition(G4ThreeVector aPosition){positionEMCal = aPosition;};
   /**
      A getter of the position of the energy deposit in the electromagnetic calorimeter.
   */
   G4ThreeVector GetEMCalPosition() {return positionEMCal;};
   /**
      A setter of the energy deposit in the electromagnetic calorimeter.
      @param aEnergy The energy deposited in the detector.
   */
   void SetEMCalEnergy(G4double aEnergy) {energyEMCal = aEnergy;};
   /**
      A setter of the energy deposit in the electromagnetic calorimeter.
   */
   G4double GetEMCalEnergy() {return energyEMCal;};
   /**
      A setter of the electromagnetic calorimeter resolution. Currently equal to -1 if AtlFast type of smearing is used.
      @param aResolution The calorimeter resolution (particle type and momentum dependent).
   */
   void SetEMCalResolution(G4double aResolution) {resEMCal = aResolution;};
   /**
      A getter of the electromagnetic calorimeter resolution. Currently equal to -1 if AtlFast type of smearing is used.
   */
   G4double GetEMCalResolution() {return resEMCal;};
   /**
      A setter of the electromagnetic calorimeter efficiency. Currently not used (efficiency is 1).
      @param aResolution The detector efficiency.
   */
   void SetEMCalEfficiency(G4double aEfficiency) {effEMCal = aEfficiency;};
   /**
      A getter of the electromagnetic calorimeter efficiency. Currently not used (efficiency is 1).
   */
   G4double GetEMCalEfficiency() {return effEMCal;};
   /**
      A setter of the position of the energy deposit in the hadronic calorimeter.
      @param aPosition The position of the energy deposit.
   */
   void SetHCalPosition(G4ThreeVector aPosition){positionHCal = aPosition;};
   /**
      A getter of the position of the energy deposit in the hadronic calorimeter.
   */
   G4ThreeVector GetHCalPosition() {return positionHCal;};
   /**
      A setter of the energy deposit in the hadronic calorimeter.
      @param aEnergy The energy deposited in the detector.
   */
   void SetHCalEnergy(G4double aEnergy) {energyHCal = aEnergy;};
   /**
      A setter of the energy deposit in the hadronic calorimeter.
   */
   G4double GetHCalEnergy() {return energyHCal;};
   /**
      A setter of the hadronic calorimeter resolution. Currently equal to -1 if AtlFast type of smearing is used.
      @param aResolution The calorimeter resolution (particle type and momentum dependent).
   */
   void SetHCalResolution(G4double aResolution) {resHCal = aResolution;};
   /**
      A getter of the hadronic calorimeter resolution. Currently equal to -1 if AtlFast type of smearing is used.
   */
   G4double GetHCalResolution() {return resHCal;};
   /**
      A setter of the hadronic calorimeter efficiency. Currently not used (efficiency is 1).
      @param aResolution The detector efficiency.
   */
   void SetHCalEfficiency(G4double aEfficiency) {effHCal = aEfficiency;};
   /**
      A getter of the hadronic calorimeter efficiency. Currently not used (efficiency is 1).
   */
   G4double GetHCalEfficiency() {return effHCal;};
   /**
      A getter of the particle unique ID (within event). Can be set only in the constructor.
   */
   G4int GetID() const;
   /**
      A getter of the standard PDG code. Can be set only in the constructor.
   */
   G4int GetPID() const {return pid;};

  private:
   /**
      A particle unique ID.
   */
   G4int id;
   /**
      A particle type (PDG code).
   */
   G4int pid;
   /**
      A particle initial momentum (from particle generator).
   */
   G4ThreeVector momentumMC;
   /**
      A particle momentum at the entrance to the tracking detector.
   */
   G4ThreeVector momentumTracker;
   /**
      A resolution of the tracking detector.
   */
   G4double resTracker;
   /**
      An efficiency of the tracking detector. Currently not used (equal to 1).
   */
   G4double effTracker;
   /**
      A position of the energy deposited in the electromagnetic calorimeter.
   */
   G4ThreeVector positionEMCal;
   /**
      An energy deposited in the electromagnetic calorimeter.
   */
   G4double energyEMCal;
   /**
      A resolution of the electormagnetic calorimeter.
   */
   G4double resEMCal;
   /**
      An efficiency of the electormagnetic calorimeter. Currently not used (equal to 1).
   */
   G4double effEMCal;
   /**
      A position of the energy deposited in the hadronic calorimeter.
   */
   G4ThreeVector positionHCal;
   /**
      An energy deposited in the hadronic calorimeter.
   */
   G4double energyHCal;
   /**
      A resolution of the hadronic calorimeter.
   */
   G4double resHCal;
   /**
      An efficiency of the hadronic calorimeter. Currently not used (equal to 1).
   */
   G4double effHCal;

};

#endif

