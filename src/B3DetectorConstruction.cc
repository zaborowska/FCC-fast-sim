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
// $Id: B3DetectorConstruction.cc 71323 2013-06-13 16:54:23Z gcosmo $
//
/// \file B3DetectorConstruction.cc
/// \brief Implementation of the B3DetectorConstruction class

#include "B3DetectorConstruction.hh"

#include "FCCKillModel.hh"
#include "FCCEmSmearModel.hh"
#include "G4FastSimulationManager.hh"
#include "Par01EMShowerModel.hh"
#include "G4GlobalFastSimulationManager.hh"

#include "G4NistManager.hh"
#include "G4ProductionCuts.hh"
#include "G4RegionStore.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSDoseDeposit.hh"
#include "G4VisAttributes.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B3DetectorConstruction::B3DetectorConstruction()
  : G4VUserDetectorConstruction(),
    fCheckOverlaps(true)
{
   DefineMaterials();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B3DetectorConstruction::~B3DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B3DetectorConstruction::DefineMaterials()
{
      G4Material* myAl = new G4Material("Aluminum0x7ff5f9e42670", 13., 26.9814858034413*g/mole, 2.69999857937074*g/cm3);

   G4Element* myN14 = new G4Element("N140x7ff5f9e3bbf0", "N", 7, 14.0031*g/mole);
   G4Element* myN15 = new G4Element("N150x7ff5f9e3e570", "N", 7, 15.0001*g/mole);

   G4Material* myN = new G4Material("N0x7ff5f9e3e780",0.0011652*g/cm3,2);
   myN->AddElement(myN14, 0.99632);
   myN->AddElement(myN15, 0.00368);

   G4Element* myO16 = new G4Element("O160x7ff5f9e3f410", "O", 8, 15.9949*g/mole);
   G4Element* myO17 = new G4Element("O170x7ff5f9e3f450", "O", 8, 16.9991*g/mole);
   G4Element* myO18 = new G4Element("O180x7ff5f9e3f490", "O", 8, 17.9992*g/mole);

   G4Material* myO = new G4Material("O0x7ff5f9e3f300",0.00133151*g/cm3,3);
   myO->AddElement(myO16, 0.99757);
   myO->AddElement(myO17, 0.00038);
   myO->AddElement(myO18, 0.00205);

   G4Element* myAr36 = new G4Element("Ar360x7ff5f9e45fb0", "Ar", 18, 35.9675*g/mole);
   G4Element* myAr38 = new G4Element("Ar380x7ff5f9e45ff0", "Ar", 18, 37.9627*g/mole);
   G4Element* myAr40 = new G4Element("Ar400x7ff5f9e46030", "Ar", 18, 39.9624*g/mole);

   G4Material* myAr = new G4Material("Ar0x7ff5f9e45e80",0.00166201*g/cm3,3);
   myAr->AddElement(myAr36, 0.003365);
   myAr->AddElement(myAr38, 0.000632);
   myAr->AddElement(myAr40, 0.996003);

   G4Element* myH1 = new G4Element("H10x7ff5f9e38e80", "H", 1, 1.00782503081372*g/mole);
   G4Element* myH2 = new G4Element("H20x7ff5f9e38c00", "H", 1, 2.01410199966617*g/mole);

   G4Material* myH = new G4Material("H0x7ff5f9e38da0",0.000083748*g/cm3,2);
   myH->AddElement(myH1, 0.999885);
   myH->AddElement(myH2, 0.000115);


   G4Material* myAir = new G4Material("Air0x7ff5f9e72ea0",0.00120479*g/cm3,4);
   myAir->AddMaterial(myN, 0.7494);
   myAir->AddMaterial(myO, 0.2369);
   myAir->AddMaterial(myAr, 0.0129);
   myAir->AddMaterial(myH, 0.0008);

//
// What about vacuum ?  Vacuum is an ordinary gas with very low density
//

G4double density     = universe_mean_density;                //from PhysicalConstants.h
G4double pressure    = 1.e-19*pascal;
G4double temperature = 0.1*kelvin;
new G4Material("Galactic", 1., 1.01*g/mole, density,
                   kStateGas,temperature,pressure);


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B3DetectorConstruction::Construct()
{
   //
   G4NistManager* nist = G4NistManager::Instance();
   G4Material* Al_mat = nist->FindOrBuildMaterial("Aluminum0x7ff5f9e42670");
   G4Material* Air_mat   = nist->FindOrBuildMaterial("Air0x7ff5f9e72ea0");
   G4Material* Vacuum_mat   = nist->FindOrBuildMaterial("Galactic");

   //
   // World
   //
   G4Box* solidWorld =
      new G4Box("world_volume0x7ff5f9e788a0",         //its name
                10000*mm,10000*mm,20000*mm); //its size

   G4LogicalVolume* logicWorld =
      new G4LogicalVolume(solidWorld,          //its solid
                          Air_mat,         //its material
                          "World");            //its name

   G4VPhysicalVolume* physWorld =
      new G4PVPlacement(0,                     //no rotation
                        G4ThreeVector(),       //at (0,0,0)
                        logicWorld,            //its logical volume
                        "World",               //its name
                        0,                     //its mother  volume
                        false,                 //no boolean operation
                        0,                     //copy number
                        fCheckOverlaps);       // checking overlaps

   G4Tubs* solidRing =
      new G4Tubs("Ring",1500*mm, 2990*mm, 6000*mm, 0., twopi);

   G4LogicalVolume* logicRing =
      new G4LogicalVolume(solidRing,           //its solid
                          Al_mat,         //its material
                          "Ring");             //its name

   new G4PVPlacement(0,G4ThreeVector(),             //rotation,position
                     logicRing,            //its logical volume
                     "ring",             //its name
                     logicWorld,             //its mother  volume
                     false,                 //no boolean operation
                     0,                 //copy number
                     fCheckOverlaps);       // checking overlaps

   // Print materials
      G4cout << *(G4Material::GetMaterialTable()) << G4endl;

   G4Region* caloRegion = new G4Region("EM_calo_region");
   caloRegion->AddRootLogicalVolume(logicRing);
   // G4double cuts = 0.1 * Al_mat->GetRadlen();
   std::vector<double> cutsL;
   std::vector<double> cutsS;
   cutsS.push_back(1*m);cutsS.push_back(0.1*m);cutsS.push_back(0.1*m);cutsS.push_back(0.1*m);
   cutsL.push_back(10*km);cutsL.push_back(10*km);cutsL.push_back(10*km);cutsL.push_back(10*km);
   caloRegion->SetProductionCuts(new G4ProductionCuts());
   caloRegion->GetProductionCuts()->SetProductionCuts(cutsS);


   //always return the physical World
   //
   return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B3DetectorConstruction::ConstructSDandField()
{
// --------------- fast simulation ----------------------------
   G4RegionStore* regionStore = G4RegionStore::GetInstance();

   G4Region* caloRegion = regionStore->GetRegion("EM_calo_region");
   // new FCCKillModel("KILLModel",caloRegion);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
