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
#include "FCCSmearModel.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FCCSmearModel::FCCSmearModel(const G4GDMLAuxMapType* auxmap)

{

   ///////////////////////////////////////////////////////////////////////
   //
   // Retrieving Auxiliary Information
   //


   for(G4GDMLAuxMapType::const_iterator iter=auxmap->begin();
       iter!=auxmap->end(); iter++)
   {
      for (G4GDMLAuxListType::const_iterator vit=(*iter).second.begin();
           vit!=(*iter).second.end();vit++)
      {
         if ((*vit).type=="SensDet")
         {
            G4LogicalVolume* myvol = (*iter).first;
            if ((myvol->GetName()).find("HCal") != std::string::npos){
               fHCalList.push_back(new G4Region(myvol->GetName()));
               fHCalList.back()->AddRootLogicalVolume(myvol);
            }
            else if ((myvol->GetName()).find("ECal") != std::string::npos){
               fECalList.push_back(new G4Region(myvol->GetName()));
               fECalList.back()->AddRootLogicalVolume(myvol);
            }
            else if ((myvol->GetName()).find("Muon") != std::string::npos){
               fMuonList.push_back(new G4Region(myvol->GetName()));
               fMuonList.back()->AddRootLogicalVolume(myvol);
            }
            else {
               G4cout << G4endl << "NOT A KNOWN DETECTOR !!!" << G4endl;
            }
         }
      }
   }

//
   // End of Auxiliary Information block
   //
   ////////////////////////////////////////////////////////////////////////

   //  // Example of User Limits
   //  //
   //  G4double maxStep = 0.1*m, maxLength = 0.1*m, maxTime = 5*ns, minEkin = 5*MeV;
   // fStepLimit = new G4UserLimits(maxStep,  maxLength,  maxTime,  minEkin);

   for (G4int iterECal=0; iterECal<G4int(fECalList.size()); iterECal++)
   {
      fECalList[iterECal]->SetProductionCuts(new G4ProductionCuts());
      fECalList[iterECal]->GetProductionCuts()->SetProductionCut
         (0.5* ((*fECalList[iterECal]->GetRootLogicalVolumeIterator())->GetMaterial()->GetRadlen()) );
      fECalList[iterECal]->GetProductionCuts()->SetProductionCut(
         0.1*m, idxG4GammaCut );
      //fECalList[iterECal]->SetUserLimits(fStepLimit);
      fECalSmearModel.push_back(
         new FCCEmSmearModel("emKillModel",fECalList[iterECal]));
   }
   for (G4int iterHCal=0; iterHCal<G4int(fHCalList.size()); iterHCal++)
   {
      fHCalList[iterHCal]->SetProductionCuts(new G4ProductionCuts());
      fHCalList[iterHCal]->GetProductionCuts()->SetProductionCut(
         0.5* ((*fHCalList[iterHCal]->GetRootLogicalVolumeIterator())->GetMaterial()->GetRadlen()) );
      fHCalList[iterHCal]->GetProductionCuts()->SetProductionCut(
         1.*m, idxG4GammaCut );

      fHCalSmearModel.push_back(
         new FCCHadSmearModel("hadKillModel",fHCalList[iterHCal]));
   }

   for (G4int iterMuon=0; iterMuon<G4int(fMuonList.size()); iterMuon++)
   {
      fMuonList[iterMuon]->SetProductionCuts(new G4ProductionCuts());
      fMuonList[iterMuon]->GetProductionCuts()->SetProductionCut(
         0.5* ((*fMuonList[iterMuon]->GetRootLogicalVolumeIterator())->GetMaterial()->GetRadlen()) );
      fMuonList[iterMuon]->GetProductionCuts()->SetProductionCut(
         1.*m, idxG4GammaCut );

      fMuonSmearModel.push_back(
         new FCCMuonSmearModel("MuonKillModel",fMuonList[iterMuon]));
   }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


FCCSmearModel::~FCCSmearModel()
{
   for (G4int iterECal=0; iterECal<G4int(fECalList.size()); iterECal++)
   {
      delete fECalSmearModel[iterECal];
      delete fECalList[iterECal];
   }
for (G4int iterHCal=0; iterHCal<G4int(fHCalList.size()); iterHCal++)
   {

      delete fHCalSmearModel[iterHCal];
      delete fHCalList[iterHCal];
   }
   for (G4int iterMuon=0; iterMuon<G4int(fMuonList.size()); iterMuon++)
   {

      delete fMuonSmearModel[iterMuon];
      delete fMuonList[iterMuon];
   }
}

