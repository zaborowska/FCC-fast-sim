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

class FCCPrimaryParticleInformation: public G4VUserPrimaryParticleInformation
{
public:
   FCCPrimaryParticleInformation(G4int, G4int, G4ThreeVector);
   virtual ~FCCPrimaryParticleInformation();

   virtual void Print() const;
   void SetMCMomentum(G4ThreeVector mom){momentumMC = mom;};
   G4ThreeVector GetMCMomentum() {return momentumMC;};
   void SetTrackerMomentum(G4ThreeVector mom){momentumTracker = mom;};
   G4ThreeVector GetTrackerMomentum() {return momentumTracker;};
   void SetTrackerResolution(G4double res) {resTracker = res;};
   G4double GetTrackerResolution() {return resTracker;};
   void SetTrackerEfficiency(G4double eff) {effTracker = eff;};
   G4double GetTrackerEfficiency() {return effTracker;};
   void SetEMCalPosition(G4ThreeVector pos){positionEMCal = pos;};
   G4ThreeVector GetEMCalPosition() {return positionEMCal;};
   void SetEMCalEnergy(G4double en) {energyEMCal = en;};
   G4double GetEMCalEnergy() {return energyEMCal;};
   void SetEMCalResolution(G4double res) {resEMCal = res;};
   G4double GetEMCalResolution() {return resEMCal;};
   void SetEMCalEfficiency(G4double eff) {effEMCal = eff;};
   G4double GetEMCalEfficiency() {return effEMCal;};
   void SetHCalPosition(G4ThreeVector pos){positionHCal = pos;};
   G4ThreeVector GetHCalPosition() {return positionHCal;};
   void SetHCalEnergy(G4double en) {energyHCal = en;};
   G4double GetHCalEnergy() {return energyHCal;};
   void SetHCalResolution(G4double res) {resHCal = res;};
   G4double GetHCalResolution() {return resHCal;};
   void SetHCalEfficiency(G4double eff) {effHCal = eff;};
   G4double GetHCalEfficiency() {return effHCal;};
   G4int GetID() const;
   G4int GetPID() const {return pid;};

  private:
   G4int id; // unique id within the event
   G4int pid;
   G4ThreeVector momentumMC;
   G4ThreeVector momentumTracker;
   G4double resTracker;
   G4double effTracker;
   G4ThreeVector positionEMCal;
   G4double energyEMCal;
   G4double resEMCal;
   G4double effEMCal;
   G4ThreeVector positionHCal;
   G4double energyHCal;
   G4double resHCal;
   G4double effHCal;

};

#endif

