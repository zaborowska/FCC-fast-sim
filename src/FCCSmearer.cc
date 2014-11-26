#include "FCCSmearer.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCSmearer* FCCSmearer::fFCCSmearer = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCSmearer::FCCSmearer(): bField(1.)
{}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCSmearer::~FCCSmearer()
{
   delete fElectronManager;
   delete fPionManager;
   delete fMuonManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCSmearer* FCCSmearer::Instance()
{
   if(!fFCCSmearer)
   {
      fFCCSmearer = new FCCSmearer();
   }
   return fFCCSmearer;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FCCSmearer::MakeManagers()
{
  //Create mangers for smeaing muons, pions and electrons
  fMuonManager = new Atlfast::MuonMatrixManager("data/Atlfast_MuonResParam_CSC.dat",time(NULL));
  fPionManager = new Atlfast::PionMatrixManager("data/Atlfast_PionResParam_DC1_NewUnits.dat",time(NULL));
  fElectronManager = new Atlfast::ElectronMatrixManager("data/Atlfast_ElectronResParam_CSC.dat","data/Atlfast_ElectronBremParam_CSC.dat",time(NULL));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double* FCCSmearer::Smear(const G4Track* aTrackOriginal)
{
   CLHEP::HepSymMatrix sigma; // smear matrix for track
   std::vector<double> smearVariables;   // Vector of correlated gaussian variables

   G4int PID = aTrackOriginal->GetDynamicParticle()->GetPDGcode();

   if(abs(PID) == 11)
      smearVariables = fElectronManager->getVariables( *aTrackOriginal, sigma );
   else if(abs(PID) == 13)
      smearVariables = fMuonManager->getVariables( *aTrackOriginal, sigma );
   else if(abs(PID) == 211)
      smearVariables = fPionManager->getVariables( *aTrackOriginal, sigma );
   else return NULL;
/*
   // original track position, momentum and charge
   G4ThreeVector originP = aTrackOriginal->GetMomentum();
   double originCharge = aTrackOriginal->GetDynamicParticle()->GetCharge();
   G4ThreeVector originPos = aTrackOriginal->GetPosition();

   G4double* originParams;
   originParams = ComputeTrackParams(originCharge, originP, originPos);
 G4cout<<"_________OLD: "<<G4endl
        <<"\td0: "<<G4BestUnit(originParams[0],"Length")<<"\tz0: "<<G4BestUnit(originParams[1],"Length")<<"\tphi0: "<<G4BestUnit(originParams[2],"Angle")<<"\tcottheta: "<<originParams[3]<<"\tpT: "<<G4BestUnit(1./originParams[4],"Energy")<<G4endl;

   // // Atlfast smeared variables
   // G4double smearUnits[] = {um, um, mrad, 1e-3, 1./TeV };
 double impactParameter = originParams[0] + smearVariables[0] ;// * smearUnits[0]; // [0]
 double zPerigee = originParams[1]  + smearVariables[1] ;// * smearUnits[1]; //[1]
 double Phi = CheckPhi(originParams[2] + smearVariables[2]);// * smearUnits[2]); //[2]
 double cotTheta = originParams[3] + smearVariables[3];// * smearUnits[3]; //[3]
 double invPtCharge = originParams[4] +  smearVariables[4];// * smearUnits[4]; // q/pT where q = q/|q| (just sign) //[4]
 G4cout<<"_________NEW: "<<G4endl
        <<"\td0: "<<G4BestUnit(impactParameter,"Length")<<"\tz0: "<<G4BestUnit(zPerigee,"Length")<<"\tphi0: "<<G4BestUnit(Phi,"Angle")<<"\tcottheta: "<<cotTheta<<"\tpT: "<<G4BestUnit(1./invPtCharge,"Energy")<<G4endl;

   G4double* params = new G4double[5];
   params[0] = impactParameter;
   params[1] = zPerigee;
   params[2] = Phi;
   params[3] = cotTheta;
   params[4] = invPtCharge;
*/
   G4double* params = new G4double[5];
   params[0] = smearVariables[0] ;//* smearUnits[0]; // [0]
   params[1] = smearVariables[1] ;//* smearUnits[1]; //[1]
   params[2] = CheckPhi(smearVariables[2]) ;//* smearUnits[2]); //[2]
   params[3] = smearVariables[3] ;//* smearUnits[3]; //[3]
   params[4] = smearVariables[4];// * smearUnits[4]; // q/pT where q = q/|q| (just sign) //[4]

   return params;

   // G4ThreeVector pos = ComputePosFromParams(originParams, originP.phi());
   // G4ThreeVector P = ComputeMomFromParams (originParams);

   // double m = aTrackOriginal->GetDynamicParticle()->GetDefinition()->GetPDGMass();
   // aTrackOriginal->SetKineticEnergy(sqrt(P.mag2()-m*m)-m);
   // aTrackOriginal->SetMomentumDirection(P.unit());
   // aTrackOriginal->SetPosition(pos);
   // aTrackOriginal->SetVertexKineticEnergy(sqrt(P.mag2()-m*m)-m);
   // aTrackOriginal->SetVertexMomentumDirection(P.unit());
   // aTrackOriginal->SetVertexPosition(pos);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4double* FCCSmearer::ComputeTrackParams(G4double charge, G4ThreeVector vertexMomentum, G4ThreeVector vertexPosition)
{
   // calculation of track parameter representation: implemented in Atlfast::TrackTrajectory
   double pt = vertexMomentum.perp();
   double q  = charge;
   q /= abs(q); //  |q| = 1

   // Working in MeV and mm
   double radius = pt/(2*0.149898*bField);
   double x0  = vertexPosition.x() - q* radius*sin(vertexMomentum.phi());
   double y0  = vertexPosition.y() + q* radius*cos(vertexMomentum.phi());
   G4ThreeVector hCentre(x0,y0);

   // calculate parameters
   double impactParameter = -q*(hCentre.perp() - radius);
   double theta = vertexMomentum.theta();
   double cotTheta = 1/tan(theta);
   double eta = vertexMomentum.pseudoRapidity();
   double zPerigee;
   double phi;

   //  Calculate Phi
   phi = CheckPhi(hCentre.phi() + M_PI/2.);

   zPerigee = vertexPosition.z() + cotTheta*radius*(phi-vertexMomentum.phi());

   G4double* params = new G4double[5];
   params[0] = impactParameter;
   params[1] = zPerigee;
   params[2] = phi;
   params[3] = cotTheta;
   params[4] = q/pt;

   return params;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector FCCSmearer::ComputePosFromParams(G4double* params, G4double phiVertex)
{
   G4double x,y,z;
   double radius = 1./(2*0.149898*bField*params[4]);
   if(radius<0) radius = -1* radius;
   double q = (params[4] > 0) - (params[4] < 0); // returns signum(charge)

   // helix centre
   double xC = ( params[0] - q * radius) *sin(params[2]);
   double yC = ( q * params[0] + q * radius) *cos(params[2]);

   // vertex point
   x = xC + q * radius * sin(phiVertex);
   y = yC - q * radius * cos(phiVertex);
   z = params[1] + params[3]*radius*(params[2]-phiVertex);

   G4ThreeVector pos (x,y,z);
   return pos;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector FCCSmearer::ComputeMomFromParams(G4double* params)
{
   double Px = abs(1./params[4])*sin(params[2]);
   double Py = abs(1./params[4])*cos(params[2]);
   double Pz = abs(1./params[4])/sin( atan(1./params[3]) );

   G4ThreeVector P (Px,Py,Pz);
   return P;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double FCCSmearer::CheckPhi(G4double Phi)
{
   if(Phi<-M_PI)
      while(Phi<-M_PI)
         Phi+=2*M_PI;
   else if(Phi>M_PI)
      while(Phi>M_PI)
         Phi-=2*M_PI;
   return Phi;
}
