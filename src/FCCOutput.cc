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
FCCOutput::FCCOutput(): fFileNameWithRunNo(false)
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

  analysisManager->CreateNtuple(evName+"_MC", evName+"_MC");
  analysisManager->CreateNtupleIColumn("particleID");  // column Id = 0
  analysisManager->CreateNtupleIColumn("PID");  // column Id = 1
  analysisManager->CreateNtupleDColumn("pX");  // column Id = 2
  analysisManager->CreateNtupleDColumn("pY"); // column Id = 3
  analysisManager->CreateNtupleDColumn("pZ"); // column Id = 4
  analysisManager->FinishNtuple(ntupID);

  analysisManager->CreateNtuple(evName+"_det", evName+"_det");
  analysisManager->CreateNtupleIColumn("particleID");  // column Id = 0
  analysisManager->CreateNtupleIColumn("PID");  // column Id = 1
  analysisManager->CreateNtupleDColumn("TRpX");  // column Id = 2
  analysisManager->CreateNtupleDColumn("TRpY"); // column Id = 3
  analysisManager->CreateNtupleDColumn("TRpZ"); // column Id = 4
  analysisManager->CreateNtupleDColumn("TRres"); // column Id = 5
  analysisManager->CreateNtupleDColumn("TReff"); // column Id = 6
  analysisManager->FinishNtuple(ntupID + 1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FCCOutput::CreateHistograms()
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
   analysisManager->CreateH1("Pdiff","P smeared in tracker", 101, -100.*GeV, 100*GeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FCCOutput::SaveTrack(G4bool HitDetector, G4int partID,  G4int PID,
                          G4ThreeVector momentum, G4double resolution, G4double efficiency) const
{
   const G4Event* event = G4RunManager::GetRunManager()->GetCurrentEvent();
   G4int evNo = event->GetEventID();
   G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
   G4int ntupID = 2* evNo+HitDetector; // 2*evNo is TTree number for storing MC at vertex, 2*evNo+1 is TTree number for storing detected particles (smeared)
   analysisManager->FillNtupleIColumn(ntupID, 0, partID);
   analysisManager->FillNtupleIColumn(ntupID, 1, PID);
   analysisManager->FillNtupleDColumn(ntupID, 2, momentum.x());
   analysisManager->FillNtupleDColumn(ntupID, 3, momentum.y());
   analysisManager->FillNtupleDColumn(ntupID, 4, momentum.z());
   if(HitDetector)
   {
      analysisManager->FillNtupleDColumn(ntupID, 5, resolution);
      analysisManager->FillNtupleDColumn(ntupID, 6, efficiency);
   }
   analysisManager->AddNtupleRow(ntupID);
   return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FCCOutput::FillHistogram(G4int HNo, G4double value) const
{
   G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
   analysisManager->FillH1(HNo, value);
   return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
