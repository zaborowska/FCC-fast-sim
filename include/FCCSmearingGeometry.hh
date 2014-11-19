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
#ifndef FCC_SMEARING_GEOMETRY_H
#define FCC_SMEARING_GEOMETRY_H

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
// PhysicsList
//-----------------------------------
#include "FCCPhysicsList.hh"
#include "FCCEmSmearModel.hh"
#include "FCCHadSmearModel.hh"
#include "FCCMuonSmearModel.hh"
// #include "FCCSmearModel.hh"
#include "FTFP_BERT.hh"
//---------------------------
// Parameterisation manager:
//---------------------------
#include "G4GlobalFastSimulationManager.hh"

#include "G4UserLimits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class FCCSmearingGeometry
{
  public:
    FCCSmearingGeometry(const G4GDMLAuxMapType* auxmap);
    ~FCCSmearingGeometry();

private:
   // G4Region* fWorldRegion;
   // FCCSmearModel* fSmearModel;

   std::vector<G4Region*> fECalList;
   std::vector<G4Region*> fHCalList;
   std::vector<G4Region*> fMuonList;
   //G4UserLimits* fStepLimit;
   std::vector<FCCEmSmearModel*> fECalSmearModel;
   std::vector<FCCHadSmearModel*> fHCalSmearModel;
   std::vector<FCCMuonSmearModel*> fMuonSmearModel;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
