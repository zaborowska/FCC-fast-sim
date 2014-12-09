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
#include "FCCFastSimGeometry.hh"
#include "FCCDetectorParametrisation.hh"
#include "G4ProductionCuts.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FCCFastSimGeometry::FCCFastSimGeometry(const G4GDMLAuxMapType* auxmap)
{
   std::vector<G4Region*> TrackerList;
   std::vector<G4Region*> ECalList;
   std::vector<G4Region*> HCalList;
   std::vector<G4Region*> MuonList;

   // Retrieving Auxiliary Information

   for(G4GDMLAuxMapType::const_iterator iter=auxmap->begin();
       iter!=auxmap->end(); iter++)
   {
      for (G4GDMLAuxListType::const_iterator vit=(*iter).second.begin();
           vit!=(*iter).second.end();vit++)
      {
         if ((*vit).type=="SensDet")
         {
            G4LogicalVolume* myvol = (*iter).first;
            if ((myvol->GetName()).find("Tracker") != std::string::npos){
               TrackerList.push_back(new G4Region(myvol->GetName()));
               TrackerList.back()->AddRootLogicalVolume(myvol);
            }
            else if ((myvol->GetName()).find("HCal") != std::string::npos){
               HCalList.push_back(new G4Region(myvol->GetName()));
               HCalList.back()->AddRootLogicalVolume(myvol);
            }
            else if ((myvol->GetName()).find("ECal") != std::string::npos){
               ECalList.push_back(new G4Region(myvol->GetName()));
               ECalList.back()->AddRootLogicalVolume(myvol);
            }
            else if ((myvol->GetName()).find("Muon") != std::string::npos){
               MuonList.push_back(new G4Region(myvol->GetName()));
               MuonList.back()->AddRootLogicalVolume(myvol);
            }
            else {
               G4cout << G4endl << "NOT A KNOWN DETECTOR !!!" << G4endl;
            }
         }
      }
   }

   for (G4int iterTracker=0; iterTracker<G4int(TrackerList.size()); iterTracker++)
   {
      TrackerList[iterTracker]->SetProductionCuts(new G4ProductionCuts());
      TrackerList[iterTracker]->GetProductionCuts()->SetProductionCut
         (0.5* ((*TrackerList[iterTracker]->GetRootLogicalVolumeIterator())->GetMaterial()->GetRadlen()) );
      TrackerList[iterTracker]->GetProductionCuts()->SetProductionCut(
         0.1*m, idxG4GammaCut );
      new FCCFastSimModelTracker("fastSimModelTracker",TrackerList[iterTracker], FCCDetectorParametrisation::eCMS);
   }
   for (G4int iterECal=0; iterECal<G4int(ECalList.size()); iterECal++)
   {
      ECalList[iterECal]->SetProductionCuts(new G4ProductionCuts());
      ECalList[iterECal]->GetProductionCuts()->SetProductionCut
         (0.5* ((*ECalList[iterECal]->GetRootLogicalVolumeIterator())->GetMaterial()->GetRadlen()) );
      ECalList[iterECal]->GetProductionCuts()->SetProductionCut(
         0.1*m, idxG4GammaCut );
      new FCCFastSimModelEMCal("fastSimModelEMCal",ECalList[iterECal], FCCDetectorParametrisation::eCMS);
   }
   // for (G4int iterHCal=0; iterHCal<G4int(fHCalList.size()); iterHCal++)
   // {
   //    fHCalList[iterHCal]->SetProductionCuts(new G4ProductionCuts());
   //    fHCalList[iterHCal]->GetProductionCuts()->SetProductionCut(
   //       0.5* ((*fHCalList[iterHCal]->GetRootLogicalVolumeIterator())->GetMaterial()->GetRadlen()) );
   //    fHCalList[iterHCal]->GetProductionCuts()->SetProductionCut(
   //       1.*m, idxG4GammaCut );

   //    fHCalSmearModel.push_back(
   //       new FCCHadSmearModel("hadKillModel",fHCalList[iterHCal]));
   // }

   // for (G4int iterMuon=0; iterMuon<G4int(fMuonList.size()); iterMuon++)
   // {
   //    fMuonList[iterMuon]->SetProductionCuts(new G4ProductionCuts());
   //    fMuonList[iterMuon]->GetProductionCuts()->SetProductionCut(
   //       0.5* ((*fMuonList[iterMuon]->GetRootLogicalVolumeIterator())->GetMaterial()->GetRadlen()) );
   //    fMuonList[iterMuon]->GetProductionCuts()->SetProductionCut(
   //       1.*m, idxG4GammaCut );

   //    fMuonSmearModel.push_back(
   //       new FCCMuonSmearModel("MuonKillModel",fMuonList[iterMuon]));
   // }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


FCCFastSimGeometry::~FCCFastSimGeometry()
{
   // for (G4int iterECal=0; iterECal<G4int(fECalList.size()); iterECal++)
   // {
   //    delete fECalSmearModel[iterECal];
   //    delete fECalList[iterECal];
   // }
// for (G4int iterHCal=0; iterHCal<G4int(fHCalList.size()); iterHCal++)
//    {

//       delete fHCalSmearModel[iterHCal];
//       delete fHCalList[iterHCal];
//    }
//    for (G4int iterMuon=0; iterMuon<G4int(fMuonList.size()); iterMuon++)
//    {

//       delete fMuonSmearModel[iterMuon];
//       delete fMuonList[iterMuon];
//    }
}

