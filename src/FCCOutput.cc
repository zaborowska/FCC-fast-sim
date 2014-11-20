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

void FCCOutput::SaveTrack(G4bool HitDetector, G4int partID,  G4int PID, G4double charge, G4double bField,
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

   // calculation of track parameter representation: from Atlfast::TrackTrajectory
    double pt = vertexMomentum.perp();
    double q  = charge;
    q /= abs(q); //  q = |1|

    // Working in MeV and mm
    double radius = pt/(2*0.149898*bField);
    double x0  = vertexPosition.x() + q*radius*sin(vertexMomentum.phi());
    double y0  = vertexPosition.y() - q*radius*cos(vertexMomentum.phi());
    G4ThreeVector hCentre(x0,y0);

    // calculate parameters
    // Tidier version of Fortran code zvhepa.F - Tarta
    double impactParameter = -q*(hCentre.perp() - radius);
    double theta = vertexMomentum.theta();
    double cotTheta = 1/tan(theta);
    double eta = vertexMomentum.pseudoRapidity();
    double zPerigee;
    double phi;

    //  Calculate Phi
    if (abs(vertexPosition.x()) < 1e-4 && abs(vertexPosition.y()) < 1e-4)
      {phi = vertexMomentum.phi();} else
	{if ( (vertexPosition.phi() - hCentre.phi()) > 0 )
	  { phi =  hCentre.phi() + M_PI/2; } else
	    { phi = hCentre.phi() - M_PI/2;} }

    double x1 =  sin(phi)*q*hCentre.perp()/(radius*radius);
    double y1 = -cos(phi)*q*hCentre.perp()/(radius*radius);
    if (x1*x0 < 0 || y1*y0 < 0) {phi += M_PI;}
    if (phi < 0) {phi += 2.*M_PI;} else
      { if(phi > 2.*M_PI) {phi -= 2.*M_PI;} }

    // z of perigee
    double dot = vertexPosition.x()*vertexMomentum.x() + vertexPosition.y()*vertexMomentum.y();
    double dotsign = dot/abs(dot) ;
    double dV = (vertexPosition.perp()*vertexPosition.perp()) -
      (impactParameter*impactParameter);
    double denom = hCentre.perp()/radius;

    // NAN protection
    if (dV < 0) {dV = 0;}
    if (abs(dotsign)!=1) dotsign=0;

    zPerigee = vertexPosition.z() - (2*q*radius)*dotsign*cotTheta*
       asin((sqrt(dV/(denom)))*q/(2*radius) );

    // track parametrisation:
    //  (eta, phi, pt, impactParameter, zPerigee, cotTheta, q/pt );

   analysisManager->FillNtupleDColumn(ntupID, 5, impactParameter);
   analysisManager->FillNtupleDColumn(ntupID, 6, zPerigee);
   analysisManager->FillNtupleDColumn(ntupID, 7, phi);
   analysisManager->FillNtupleDColumn(ntupID, 8, cotTheta);
   analysisManager->FillNtupleDColumn(ntupID, 9, q/pt);
   analysisManager->AddNtupleRow(ntupID);
   return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
