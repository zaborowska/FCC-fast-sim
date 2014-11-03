

#include "FCCSDInfo.hh"

//---------------------------
// Parameterisation manager:
//---------------------------
#include "G4GlobalFastSimulationManager.hh"

//------------
// Geometry:
//------------
#include "FCCDetectorConstruction.hh"
#include "G4GDMLParser.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4TransportationManager.hh"
#include "G4RegionStore.hh"

//-----------------------------------
//Sensitive Detectors
//-----------------------------------
#include "FCCTrackerSD.hh"
#include "FCCEmCalorimeterSD.hh"
#include "FCCHadCalorimeterSD.hh"
#include "G4SDManager.hh"

//-----------------------------------
// PhysicsList
//-----------------------------------
#include "FCCPhysicsList.hh"
#include "FCCEMShowerModel.hh"
#include "FCCHadShowerModel.hh"
#include "FCCMuonShowerModel.hh"
#include "FTFP_BERT.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FCCSDInfo::FCCSDInfo(const G4GDMLAuxMapType* auxmap)

{


   //------------------------------------------------
   // Sensitive detectors
   //------------------------------------------------

   //SDman = G4SDManager::GetSDMpointer();
   G4SDManager* SDman = G4SDManager::GetSDMpointer();



//EM Calorimeters
   FCCEmCalorimeterSD* aTrackerSD = new FCCEmCalorimeterSD("ECalBarrel");
   SDman->AddNewDetector( aTrackerSD );

   FCCEmCalorimeterSD* aTrackerSD2 = new FCCEmCalorimeterSD("ECalEndCap1");
   SDman->AddNewDetector( aTrackerSD2 );

   FCCEmCalorimeterSD* aTrackerSD3 = new FCCEmCalorimeterSD("ECalEndCap2");
   SDman->AddNewDetector( aTrackerSD3 );

   FCCEmCalorimeterSD* aTrackerSD4 = new FCCEmCalorimeterSD("ECalForward1");
   SDman->AddNewDetector( aTrackerSD4 );

   FCCEmCalorimeterSD* aTrackerSD5 = new FCCEmCalorimeterSD("ECalForward2");
   SDman->AddNewDetector( aTrackerSD5 );


//Hadron Calorimeters
   FCCEmCalorimeterSD* aTrackerSD6 = new FCCEmCalorimeterSD("HCalBarrel");
   SDman->AddNewDetector( aTrackerSD6 );

   FCCEmCalorimeterSD* aTrackerSD7 = new FCCEmCalorimeterSD("HCalEndCap1");
   SDman->AddNewDetector( aTrackerSD7 );

   FCCEmCalorimeterSD* aTrackerSD8 = new FCCEmCalorimeterSD("HCalEndCap2");
   SDman->AddNewDetector( aTrackerSD8 );

   FCCEmCalorimeterSD* aTrackerSD9 = new FCCEmCalorimeterSD("HCalForward1");
   SDman->AddNewDetector( aTrackerSD9 );

   FCCEmCalorimeterSD* aTrackerSD10 = new FCCEmCalorimeterSD("HCalForward2");
   SDman->AddNewDetector( aTrackerSD10 );


//Muon

   FCCTrackerSD* aTrackerSD11 = new FCCTrackerSD("MuonEndCap1", "MuonEndCap1");
   SDman->AddNewDetector( aTrackerSD11 );

   FCCTrackerSD* aTrackerSD12 = new FCCTrackerSD("MuonEndCap2", "MuonEndCap2");
   SDman->AddNewDetector( aTrackerSD12 );

   FCCTrackerSD* aTrackerSD13 = new FCCTrackerSD("MuonEndCap3", "MuonEndCap3");
   SDman->AddNewDetector( aTrackerSD13 );

   FCCTrackerSD* aTrackerSD14 = new FCCTrackerSD("MuonEndCap4", "MuonEndCap4");
   SDman->AddNewDetector( aTrackerSD14 );

   FCCTrackerSD* aTrackerSD15 = new FCCTrackerSD("MuonEndCap5", "MuonEndCap5");
   SDman->AddNewDetector( aTrackerSD15 );

   FCCTrackerSD* aTrackerSD16 = new FCCTrackerSD("MuonEndCap6", "MuonEndCap6");
   SDman->AddNewDetector( aTrackerSD16 );

   FCCTrackerSD* aTrackerSD17 = new FCCTrackerSD("MuonForward1", "MuonForward1");
   SDman->AddNewDetector( aTrackerSD17 );

   FCCTrackerSD* aTrackerSD18 = new FCCTrackerSD("MuonForward2", "MuonForward2");
   SDman->AddNewDetector( aTrackerSD18 );

   FCCTrackerSD* aTrackerSD19 = new FCCTrackerSD("MuonForward3", "MuonForward3");
   SDman->AddNewDetector( aTrackerSD19 );

   FCCTrackerSD* aTrackerSD20 = new FCCTrackerSD("MuonForward4", "MuonForward4");
   SDman->AddNewDetector( aTrackerSD20 );

   FCCTrackerSD* aTrackerSD21 = new FCCTrackerSD("MuonForward5", "MuonForward5");
   SDman->AddNewDetector( aTrackerSD21 );

   FCCTrackerSD* aTrackerSD22 = new FCCTrackerSD("MuonForward6", "MuonForward6");
   SDman->AddNewDetector( aTrackerSD22 );



   ///////////////////////////////////////////////////////////////////////
   //
   // Retrieving Auxiliary Information
   //


   std::vector<G4LogicalVolume*> ECalList;
   std::vector<G4LogicalVolume*> HCalList;
   std::vector<G4LogicalVolume*> MuonList;


   std::cout << "Found " << auxmap->size()
             << " volume(s) with auxiliary information."
             << G4endl << G4endl;


   // The same as above, but now we are looking for
   // sensitive detectors setting them for the volumes


   for(G4GDMLAuxMapType::const_iterator iter=auxmap->begin();
       iter!=auxmap->end(); iter++)
   {
      G4cout << "Volume " << ((*iter).first)->GetName()
             << " has the following list of auxiliary information: "
             << G4endl << G4endl;
      for (G4GDMLAuxListType::const_iterator vit=(*iter).second.begin();
           vit!=(*iter).second.end();vit++)
      {
         if ((*vit).type=="SensDet")
         {
            G4cout << "Attaching sensitive detector " << (*vit).value
                   << " to volume " << ((*iter).first)->GetName()
                   <<  G4endl << G4endl;

            G4VSensitiveDetector* mydet = SDman->FindSensitiveDetector((*vit).value);
         if(mydet)
         {
           G4LogicalVolume* myvol = (*iter).first;
           myvol->SetSensitiveDetector(mydet);
           if ((myvol->GetName()).find("HCal") != std::string::npos){
               HCalList.push_back(myvol);
           }
           else if ((myvol->GetName()).find("ECal") != std::string::npos){
               ECalList.push_back(myvol);
           }
           else if ((myvol->GetName()).find("Muon") != std::string::npos){
               MuonList.push_back(myvol);
           }
           else {
               G4cout << G4endl << "NOT A KNOWN DETECTOR !!!" << G4endl;
           }
         }
         else
         {
           G4cout << (*vit).value << " detector not found" << G4endl;
         }
         }
      }
   }
   //
   // End of Auxiliary Information block
   //
   ////////////////////////////////////////////////////////////////////////


   G4double cutsECal = 1*m;
   G4double cutsHCal = 1*m;
   G4double cutsMuon = 1*m;
   // if(!ECalList.empty())
   //    cutsECal = ECalList[0]->GetMaterial()->GetRadlen();
   // if(!HCalList.empty())
   //    cutsHCal = HCalList[0]->GetMaterial()->GetRadlen();
   // if(!MuonList.empty())
   //    cutsECal = MuonList[0]->GetMaterial()->GetRadlen();
   G4cout<<" Setting production cuts to: "<<cutsECal<<G4endl;

   for (G4int ECalCounter=0; ECalCounter<G4int(ECalList.size()); ECalCounter++){

       char RegionName[50];
       sprintf(RegionName, "EM_calo_region%d", ECalCounter);
       G4Region* caloRegion = new G4Region(RegionName);
       caloRegion->AddRootLogicalVolume(ECalList[ECalCounter]);

       caloRegion->SetProductionCuts(new G4ProductionCuts());
       caloRegion->GetProductionCuts()->SetProductionCut(cutsECal);

       new FCCEMShowerModel("emShowerModel",caloRegion);

    }

   for (G4int HCalCounter=0; HCalCounter<G4int(HCalList.size()); HCalCounter++){

       char RegionName[50];
       sprintf(RegionName, "H_calo_region%d", HCalCounter);
       G4Region* caloRegion = new G4Region(RegionName);
       caloRegion->AddRootLogicalVolume(HCalList[HCalCounter]);

       caloRegion->SetProductionCuts(new G4ProductionCuts());
       caloRegion->GetProductionCuts()->SetProductionCut(cutsHCal);

       new FCCHadShowerModel("HadShowerModel",caloRegion);

    }

   for (G4int MuonCounter=0; MuonCounter<G4int(MuonList.size()); MuonCounter++){

       char RegionName[50];
       sprintf(RegionName, "Muon_calo_region%d", MuonCounter);
       G4Region* caloRegion = new G4Region(RegionName);
       caloRegion->AddRootLogicalVolume(MuonList[MuonCounter]);

       caloRegion->SetProductionCuts(new G4ProductionCuts());
       caloRegion->GetProductionCuts()->SetProductionCut(cutsMuon);

       new FCCMuonShowerModel("MuonShowerModel",caloRegion);

    }





}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


FCCSDInfo::~FCCSDInfo()
{}

