#include "FCCOutput.hh"
#include "FCCEventInformation.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "g4root.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCOutput* FCCOutput::fFCCOutput = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCOutput::FCCOutput(): fFileNameWithRunNo(false), fCurrentID()
{
   fFileName = "DefaultOutput.root";
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCOutput::~FCCOutput()
{
   delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCOutput* FCCOutput::Instance()
{
   if(!fFCCOutput)
   {
      fFCCOutput = new FCCOutput();
   }
   return fFCCOutput;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FCCOutput::SetFileName(G4String name)
{
   fFileName = name;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FCCOutput::AppendName(G4bool app)
{
   fFileNameWithRunNo = app;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4String FCCOutput::GetFileName()
{
   return fFileName;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FCCOutput::StartAnalysis(G4int runID)
{
   G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
   if ( fFileNameWithRunNo)
   {
      fFileName +=  "_run";
      fFileName += G4UIcommand::ConvertToString(runID);
   }
   analysisManager->SetVerboseLevel(1);
   analysisManager->SetFileName(fFileName);
   analysisManager->OpenFile(fFileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FCCOutput::EndAnalysis()
{
   G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
   // save histograms & ntuple
   analysisManager->Write();
   analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FCCOutput::CreateNtuples()
{
   const G4Event* event = G4RunManager::GetRunManager()->GetCurrentEvent();
   G4String evName = "Event_";
   evName += G4UIcommand::ConvertToString(event->GetEventID());

   G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
   // Creating ntuple
   //
   G4int ntupID = 2*event->GetEventID();

   analysisManager->CreateNtuple(evName, evName);
   analysisManager->CreateNtupleIColumn("particleID");  // column Id = 0
   analysisManager->CreateNtupleIColumn("PID");  // column Id = 1
   analysisManager->CreateNtupleDColumn("MC_pX");  // column Id = 2
   analysisManager->CreateNtupleDColumn("MC_pY"); // column Id = 3
   analysisManager->CreateNtupleDColumn("MC_pZ"); // column Id = 4

   analysisManager->CreateNtupleDColumn("tracker_res"); // column Id = 5
   analysisManager->CreateNtupleDColumn("tracker_eff"); // column Id = 6
   analysisManager->CreateNtupleDColumn("tracker_pX");  // column Id = 7
   analysisManager->CreateNtupleDColumn("tracker_pY"); // column Id = 8
   analysisManager->CreateNtupleDColumn("tracker_pZ"); // column Id = 9

   analysisManager->CreateNtupleDColumn("emcal_res"); // column Id = 10
   analysisManager->CreateNtupleDColumn("emcal_eff");  // column Id = 11
   analysisManager->CreateNtupleDColumn("emcal_X");  // column Id = 12
   analysisManager->CreateNtupleDColumn("emcal_Y"); // column Id = 13
   analysisManager->CreateNtupleDColumn("emcal_Z"); // column Id = 14
   analysisManager->CreateNtupleDColumn("emcal_E"); // column Id = 15

   analysisManager->CreateNtupleDColumn("hcal_res"); // column Id = 16
   analysisManager->CreateNtupleDColumn("hcal_eff"); // column Id = 17
   analysisManager->CreateNtupleDColumn("hcal_X");  // column Id = 18
   analysisManager->CreateNtupleDColumn("hcal_Y"); // column Id = 19
   analysisManager->CreateNtupleDColumn("hcal_Z"); // column Id = 20
   analysisManager->CreateNtupleDColumn("hcal_E");  // column Id = 21
   analysisManager->FinishNtuple(ntupID);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FCCOutput::CreateHistograms()
{
   G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
   analysisManager->CreateH1("Pdiff","P smeared in tracker", 100, -1, 1);
   analysisManager->SetH1XAxisTitle(0, "#Delta p (GeV)");
   analysisManager->SetH1YAxisTitle(0, "Entries");
   analysisManager->CreateH1("EMCalEdiff","E smeared in EMCal", 100, -1., 1);
   analysisManager->SetH1XAxisTitle(1, "#Delta E (GeV)");
   analysisManager->SetH1YAxisTitle(1, "Entries");
   analysisManager->CreateH1("HCalEdiff","E smeared in HCal", 100, -10., 10);
   analysisManager->SetH1XAxisTitle(2, "#Delta E (GeV)");
   analysisManager->SetH1YAxisTitle(2, "Entries");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FCCOutput::SaveTrack(SaveType what, G4int partID,  G4int PID,
                          G4ThreeVector vec, G4double resolution, G4double efficiency, G4double energy)
{
   G4int evNo = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
   G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
   switch(what)
   {
   case eMC:
   {
      analysisManager->FillNtupleIColumn(evNo, 0, partID);
      analysisManager->FillNtupleIColumn(evNo, 1, PID);
      analysisManager->FillNtupleDColumn(evNo, 2, vec.x());
      analysisManager->FillNtupleDColumn(evNo, 3, vec.y());
      analysisManager->FillNtupleDColumn(evNo, 4, vec.z());
      fCurrentID = partID;
      break;
   }
   case eTracker:
   {
      if (partID != fCurrentID)
         G4cout<<" Wrong particle - trying to save Tracker information of different particle"<<G4endl;
      analysisManager->FillNtupleDColumn(evNo, 5, resolution);
      analysisManager->FillNtupleDColumn(evNo, 6, efficiency);
      analysisManager->FillNtupleDColumn(evNo, 7, vec.x());
      analysisManager->FillNtupleDColumn(evNo, 8, vec.y());
      analysisManager->FillNtupleDColumn(evNo, 9, vec.z());
      break;
   }
   case eEMCal:
   {
      if (partID != fCurrentID)
         G4cout<<" Wrong particle - trying to save EMCal information of different particle"<<G4endl;
      analysisManager->FillNtupleDColumn(evNo, 10, resolution);
      analysisManager->FillNtupleDColumn(evNo, 11, efficiency);
      analysisManager->FillNtupleDColumn(evNo, 12, vec.x());
      analysisManager->FillNtupleDColumn(evNo, 13, vec.y());
      analysisManager->FillNtupleDColumn(evNo, 14, vec.z());
      analysisManager->FillNtupleDColumn(evNo, 15, energy);
      break;
   }
   case eHCal:
   {
      if (partID != fCurrentID)
         G4cout<<" Wrong particle - trying to save HCal information of different particle"<<G4endl;
      analysisManager->FillNtupleDColumn(evNo, 16, resolution);
      analysisManager->FillNtupleDColumn(evNo, 17, efficiency);
      analysisManager->FillNtupleDColumn(evNo, 18, vec.x());
      analysisManager->FillNtupleDColumn(evNo, 19, vec.y());
      analysisManager->FillNtupleDColumn(evNo, 20, vec.z());
      analysisManager->FillNtupleDColumn(evNo, 21, energy);
      analysisManager->AddNtupleRow(evNo);
      break;
   }
   }
   return;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FCCOutput::FillHistogram(G4int HNo, G4double value) const
{
   G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
   analysisManager->FillH1(HNo, value/GeV); // value is put in GeV
   return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
