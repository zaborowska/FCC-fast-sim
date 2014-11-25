#include "FCCOutput.hh"
#include "FCCSmearer.hh"
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
  G4int ntupID = 2*event->GetEventID();

  analysisManager->CreateNtuple(evName+"_MC", evName+"_MC");
  analysisManager->CreateNtupleIColumn("particleID");  // column Id = 0
  analysisManager->CreateNtupleIColumn("PID");  // column Id = 1
  analysisManager->CreateNtupleDColumn("pX");  // column Id = 2
  analysisManager->CreateNtupleDColumn("pY"); // column Id = 3
  analysisManager->CreateNtupleDColumn("pZ"); // column Id = 4
  analysisManager->CreateNtupleDColumn("d0");  // column Id = 5
  analysisManager->CreateNtupleDColumn("z0"); // column Id = 6
  analysisManager->CreateNtupleDColumn("phi0"); // column Id = 7
  analysisManager->CreateNtupleDColumn("cottheta");  // column Id = 8
  analysisManager->CreateNtupleDColumn("qpT"); // column Id = 9
  analysisManager->FinishNtuple(ntupID);

     analysisManager->CreateNtuple(evName+"_det", evName+"_det");
     analysisManager->CreateNtupleIColumn("particleID");  // column Id = 0
     analysisManager->CreateNtupleIColumn("PID");  // column Id = 1
     analysisManager->CreateNtupleDColumn("pX");  // column Id = 2
     analysisManager->CreateNtupleDColumn("pY"); // column Id = 3
     analysisManager->CreateNtupleDColumn("pZ"); // column Id = 4
     analysisManager->CreateNtupleDColumn("d0");  // column Id = 5
     analysisManager->CreateNtupleDColumn("z0"); // column Id = 6
     analysisManager->CreateNtupleDColumn("phi0"); // column Id = 7
     analysisManager->CreateNtupleDColumn("cottheta");  // column Id = 8
     analysisManager->CreateNtupleDColumn("qpT"); // column Id = 9
     analysisManager->FinishNtuple(ntupID + 1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FCCOutput::SaveTrack(G4bool HitDetector, G4int partID,  G4int PID, G4double charge,
                          G4ThreeVector vertexMomentum, G4ThreeVector vertexPosition) const
{
   const G4Event* event = G4RunManager::GetRunManager()->GetCurrentEvent();
   G4int evNo = event->GetEventID();
   G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
   G4int ntupID = 2* evNo+HitDetector; // 2*evNo is TTree number for storing MC at vertex, 2*evNo+1 is TTree number for storing detected particles (can be smeared)
   analysisManager->FillNtupleIColumn(ntupID, 0, partID);
   analysisManager->FillNtupleIColumn(ntupID, 1, PID);
   analysisManager->FillNtupleDColumn(ntupID, 2, vertexMomentum.x());
   analysisManager->FillNtupleDColumn(ntupID, 3, vertexMomentum.y());
   analysisManager->FillNtupleDColumn(ntupID, 4, vertexMomentum.z());

   G4double* params;
   params = FCCSmearer::Instance()->ComputeTrackParams(charge, vertexMomentum, vertexPosition);

    // track parametrisation:
    //  (eta, phi, pt, impactParameter, zPerigee, cotTheta, q/pt );

   analysisManager->FillNtupleDColumn(ntupID, 5, params[0]);
   analysisManager->FillNtupleDColumn(ntupID, 6, params[1]);
   analysisManager->FillNtupleDColumn(ntupID, 7, params[2]);
   analysisManager->FillNtupleDColumn(ntupID, 8, params[3]);
   analysisManager->FillNtupleDColumn(ntupID, 9, params[4]);
   analysisManager->AddNtupleRow(ntupID);

   delete params;
   return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FCCOutput::SaveTrack(G4bool HitDetector, G4int partID,  G4int PID, G4double charge,
                          G4double* params) const
{
   const G4Event* event = G4RunManager::GetRunManager()->GetCurrentEvent();
   G4int evNo = event->GetEventID();
   G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
   G4int ntupID = 2* evNo+HitDetector; // 2*evNo is TTree number for storing MC at vertex, 2*evNo+1 is TTree number for storing detected particles (can be smeared)

   G4ThreeVector vertexMomentum = FCCSmearer::Instance()->ComputeMomFromParams(params);
   analysisManager->FillNtupleIColumn(ntupID, 0, partID);
   analysisManager->FillNtupleIColumn(ntupID, 1, PID);
   analysisManager->FillNtupleDColumn(ntupID, 2, vertexMomentum.x());
   analysisManager->FillNtupleDColumn(ntupID, 3, vertexMomentum.y());
   analysisManager->FillNtupleDColumn(ntupID, 4, vertexMomentum.z());

    // track parametrisation:
    //  (eta, phi, pt, impactParameter, zPerigee, cotTheta, q/pt );
   analysisManager->FillNtupleDColumn(ntupID, 5, params[0]);
   analysisManager->FillNtupleDColumn(ntupID, 6, params[1]);
   analysisManager->FillNtupleDColumn(ntupID, 7, params[2]);
   analysisManager->FillNtupleDColumn(ntupID, 8, params[3]);
   analysisManager->FillNtupleDColumn(ntupID, 9, params[4]);
   analysisManager->AddNtupleRow(ntupID);
   return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
