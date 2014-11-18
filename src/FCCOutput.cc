#include "FCCOutput.hh"
#include "FCCEventInformation.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "g4root.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCOutput* FCCOutput::fFCCOutput = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCOutput::FCCOutput()
{
   fFileName = "Output.root";
   G4cout  << "Constructed FCCOutput with default name: " <<"Output.root"<< G4endl;
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
G4String FCCOutput::GetFileName()
{
   return fFileName;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FCCOutput::StartAnalysis(G4int runID)
{
   G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
   fFileName +=  "_run";
   fFileName += G4UIcommand::ConvertToString(runID);
   analysisManager->SetVerboseLevel(1);
   analysisManager->SetFileName(fFileName);
   analysisManager->OpenFile();
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
  analysisManager->CreateNtuple(evName+"_MC", evName+"_MC");
  analysisManager->CreateNtupleIColumn("particleID");  // column Id = 0
  analysisManager->CreateNtupleIColumn("PID");  // column Id = 1
  analysisManager->CreateNtupleDColumn("pX");  // column Id = 2
  analysisManager->CreateNtupleDColumn("pY"); // column Id = 3
  analysisManager->CreateNtupleDColumn("pZ"); // column Id = 4
  analysisManager->FinishNtuple(2*event->GetEventID());

  if(((FCCEventInformation*)event->GetUserInformation())->GetDoSmearing())
  {
     analysisManager->CreateNtuple(evName+"_smeared", evName+"_smeared");
     analysisManager->CreateNtupleIColumn("particleID");  // column Id = 5
     analysisManager->CreateNtupleIColumn("PID");  // column Id = 6
     analysisManager->CreateNtupleDColumn("pX");  // column Id = 7
     analysisManager->CreateNtupleDColumn("pY"); // column Id = 8
     analysisManager->CreateNtupleDColumn("pZ"); // column Id = 9
     analysisManager->FinishNtuple(2*event->GetEventID() + 1);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FCCOutput::SaveTrack(G4bool HitDetector, G4int partID,  G4int PID, G4ThreeVector P) const
{
   const G4Event* event = G4RunManager::GetRunManager()->GetCurrentEvent();
   G4int evNo = event->GetEventID();
   G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
   G4int ntupleID = evNo+HitDetector; // if doSmearing == 0, only one TTree per event is created
   if (((FCCEventInformation*)event->GetUserInformation())->GetDoSmearing())
      ntupleID = 2* evNo+HitDetector; // if doSmearing == 1,
                                      // 2*evNo is TTree number for storing MC
                                      // 2*evNo+1 is TTree number for storing smeared particles that reached detector
   analysisManager->FillNtupleIColumn(ntupleID, 0, partID);
   analysisManager->FillNtupleIColumn(ntupleID, 1, PID);
   analysisManager->FillNtupleDColumn(ntupleID, 2, P.x());
   analysisManager->FillNtupleDColumn(ntupleID, 3, P.y());
   analysisManager->FillNtupleDColumn(ntupleID, 4, P.z());
   analysisManager->AddNtupleRow(ntupleID);
   return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
