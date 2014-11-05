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
// $Id: FCCDetectorConstruction.cc 75117 2013-10-28 09:38:37Z gcosmo $
//
/// \file FCCDetectorConstruction.cc
/// \brief Implementation of the FCCDetectorConstruction class

#include "FCCDetectorConstruction.hh"
#include "FCCEmCalorimeterSD.hh"
#include "FCCEMShowerModel.hh"

/*
#include "G4SDManager.hh"
#include "G4GlobalFastSimulationManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4TransportationManager.hh"
*/
#include "G4ProductionCuts.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4RegionStore.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FCCDetectorConstruction::FCCDetectorConstruction()
: G4VUserDetectorConstruction()
{ }

/*
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FCCDetectorConstruction::~FCCDetectorConstruction()
{ }
*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* FCCDetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //

  G4Material* MaterialAir = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* MaterialAl = nist->FindOrBuildMaterial("G4_Al");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       10000*mm,10000*mm,15000*mm);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        MaterialAir,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                     

 
  //     
  // Shape 1 : ECalBarrel
  //  
  

  G4Tubs* ECalBarrelSol =    
    new G4Tubs("ECalBarrel", 1500.*mm, 2990.*mm, 6000.*mm, 0.*deg, 360.*deg);
                      

  G4LogicalVolume* ECalBarrelLog =                         
    new G4LogicalVolume(ECalBarrelSol,         //its solid
                        MaterialAl,          //its material
                        "ECalBarrel");           //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at position
                    ECalBarrelLog,           //its logical volume
                    "ECalBarrel",            //its name
                    logicWorld,               //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  FCCEmCalorimeterSD* ECalBarrelSD = new FCCEmCalorimeterSD("ECalBarrel");

   

   //std::vector<double> ECalCuts; 
   //ECalCuts.push_back(1.0*m);ECalCuts.push_back(1.0*m);ECalCuts.push_back(1.0*m);ECalCuts.push_back(1.0*m);
   G4ProductionCuts* ECalCuts;
   ECalCuts = new G4ProductionCuts;
   ECalCuts->SetProductionCut(1.*mm);


    G4Region* ECalBarrelRegion = new G4Region("ECalBarrelRegion");
    ECalBarrelRegion->AddRootLogicalVolume(ECalBarrelLog);
    ECalBarrelRegion->SetProductionCuts(ECalCuts);
    new FCCEMShowerModel("emShowerModel",ECalBarrelRegion);



  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
