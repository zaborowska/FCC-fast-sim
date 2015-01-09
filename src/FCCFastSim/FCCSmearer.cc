#include "FCCSmearer.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4TransportationManager.hh"
#include "G4FieldManager.hh"
#include "G4UniformMagField.hh"
#include <ctime>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCSmearer* FCCSmearer::fFCCSmearer = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCSmearer::FCCSmearer()
{
   // unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
   unsigned seed = time(NULL);
   fRandomEngine = new CLHEP::HepJamesRandom( seed );
   fRandomGauss = new CLHEP::RandGauss(fRandomEngine);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCSmearer::~FCCSmearer()
{
   if(fElectronManager) delete fElectronManager;
   if(fPionManager) delete fPionManager;
   if(fMuonManager) delete fMuonManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCSmearer* FCCSmearer::Instance()
{
   if(!fFCCSmearer)
   {
      fFCCSmearer = new FCCSmearer();
      fFCCSmearer->MakeManagers();
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

G4ThreeVector FCCSmearer::SmearMomentum(const G4Track* aTrackOriginal, G4double resolution = -1)
{
   if(resolution != -1)
   {
      return SmearGaussian(aTrackOriginal, resolution);
   }
   else
   {
      return SmearPerigee(aTrackOriginal);
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double FCCSmearer::SmearEnergy(const G4Track* aTrackOriginal, G4double resolution = -1)
{
   G4double newE = -1;
   while (newE < 0) // to ensure the resulting value is not negative (vital for energy smearing, does not change direction for momentum smearing)
   {
      if(resolution != -1)
         newE = aTrackOriginal->GetKineticEnergy() * Gauss(1,resolution);
      else
      {
         G4ThreeVector mom = SmearPerigee(aTrackOriginal);
         newE = sqrt(mom.mag2()+pow(aTrackOriginal->GetDynamicParticle()->GetDefinition()->GetPDGMass(),2) )-aTrackOriginal->GetDynamicParticle()->GetDefinition()->GetPDGMass();
      }
   }
   return newE;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector FCCSmearer::SmearGaussian(const G4Track* aTrackOriginal, G4double resolution)
{
   G4ThreeVector originP = aTrackOriginal->GetMomentum();
   double originCharge = aTrackOriginal->GetDynamicParticle()->GetCharge();
   G4ThreeVector originPos = aTrackOriginal->GetPosition();
   G4double rdm = Gauss(1,resolution);
   G4ThreeVector smearedMom (originP.x()*rdm,  originP.y()*rdm,  originP.z()*rdm);
   return smearedMom;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double FCCSmearer::Gauss(G4double mean, G4double stdDev)
{
   return fRandomGauss->fire(mean, stdDev);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector FCCSmearer::SmearPerigee(const G4Track* aTrackOriginal)
{
   G4double* params = Atlfast(aTrackOriginal);
   G4ThreeVector smearedMom =FCCSmearer::Instance()->ComputeMomFromParams(params);
   return smearedMom;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double* FCCSmearer::Atlfast(const G4Track* aTrackOriginal)
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

   // original track position, momentum and charge
   G4ThreeVector originP = aTrackOriginal->GetMomentum();
   double originCharge = aTrackOriginal->GetDynamicParticle()->GetCharge();
   G4ThreeVector originPos = aTrackOriginal->GetPosition();

   G4double* originParams;
   originParams = ComputeTrackParams(originCharge, originP, originPos);

   // Atlfast smeared variables
   double impactParameter = originParams[0] + smearVariables[0]; // [0]
   double zPerigee = originParams[1]  + smearVariables[1]; //[1]
   double Phi = CheckPhi(originParams[2] + smearVariables[2]); //[2]
   double cotTheta = originParams[3] + smearVariables[3] ; //[3]
   double invPtCharge = originParams[4] +  smearVariables[4]; // q/pT where q = q/|q| (just sign) //[4]

   G4double* params = new G4double[5];
   params[0] = impactParameter;
   params[1] = zPerigee;
   params[2] = Phi;
   params[3] = cotTheta;
   params[4] = invPtCharge;
   return params;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4double* FCCSmearer::ComputeTrackParams(G4double charge, G4ThreeVector vertexMomentum, G4ThreeVector vertexPosition)
{
   // calculation of track parameter representation: implemented in Atlfast::TrackTrajectory
   double pt = vertexMomentum.perp();
   double q  = charge;
   q /= abs(q); //  |q| = 1

   G4double bField =
      ((G4UniformMagField*)(G4TransportationManager::GetTransportationManager()->GetFieldManager()->GetDetectorField())) ->GetConstantFieldValue().mag();

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
   G4double bField =
      ((G4UniformMagField*)(G4TransportationManager::GetTransportationManager()->GetFieldManager()->GetDetectorField())) ->GetConstantFieldValue().mag();
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
