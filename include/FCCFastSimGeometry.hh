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
#ifndef FCC_FAST_SIM_GEOMETRY_H
#define FCC_FAST_SIM_GEOMETRY_H

#include <vector>
//------------
// Geometry:
//------------
#include "FCCDetectorConstruction.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4TransportationManager.hh"
#include "G4RegionStore.hh"
#include "G4GDMLParser.hh"

//-----------------------------------
//Sensitive Detectors
//-----------------------------------
#include "G4SDManager.hh"

//-----------------------------------
// Fast Simulation Models
//-----------------------------------
#include "FCCFastSimModelTracker.hh"
#include "FCCFastSimModelEMCal.hh"
#include "FCCFastSimModelHCal.hh"

//---------------------------
// Parameterisation manager:
//---------------------------
#include "G4GlobalFastSimulationManager.hh"

#include "G4UniformMagField.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class FCCFastSimGeometry
{
  public:
    FCCFastSimGeometry(const G4GDMLAuxMapType* auxmap);
    ~FCCFastSimGeometry();

private:
    G4UniformMagField*        fEMfield;
   std::vector<FCCFastSimModelTracker*> fTrackerSmearModel;
   std::vector<FCCFastSimModelEMCal*> fEMCalSmearModel;
   std::vector<FCCFastSimModelHCal*> fHCalSmearModel;
   // std::vector<FCCMuonSmearModel*> fMuonSmearModel;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
