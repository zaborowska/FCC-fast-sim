#include "FCCSmearer.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCSmearer* FCCSmearer::fFCCSmearer = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCSmearer::FCCSmearer()
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

void FCCSmearer::Smear(G4Track* aTrackOriginal)
{
   double bField = 1.; // CONSIDER WHERE TO TAKE IT FORM !!

   CLHEP::HepSymMatrix sigma; // smear matrix for track
   std::vector<double> smearVariables;   // Vector of correlated gaussian variables

   G4int PID = aTrackOriginal->GetDynamicParticle()->GetPDGcode();

   if(abs(PID) == 11)
      smearVariables = fElectronManager->getVariables( *aTrackOriginal, sigma );
   else if(abs(PID) == 13)
      smearVariables = fMuonManager->getVariables( *aTrackOriginal, sigma );
   else if(abs(PID) == 211)
      smearVariables = fPionManager->getVariables( *aTrackOriginal, sigma );
   else return;

   // helpful variables
   G4ThreeVector originP = aTrackOriginal->GetMomentum();
   double originCharge = aTrackOriginal->GetDynamicParticle()->GetCharge();
   G4ThreeVector originPos = aTrackOriginal->GetPosition();

   G4double* originParams;
   originParams = ComputeTrackParams(originCharge, bField, originP, originPos);

   // Atlfast smeared variables
   double impactParameter = originParams[0] + smearVariables[0]; // [0]
   double zPerigee = originParams[1]  + smearVariables[1]; //[1]
   double Phi = originParams[2] + smearVariables[2]; //[2]
   if(Phi<-M_PI)
      while(Phi<M_PI)
         Phi+=2*M_PI;
   else if(Phi>M_PI)
      while(Phi>M_PI)
         Phi-=2*M_PI;
   double cotTheta = originParams[3] + smearVariables[3] ; //[3]
   double invPtCharge = originParams[4] +  smearVariables[4]; // q/pT where q = q/|q| (just sign) //[4]

   G4ThreeVector pos( aTrackOriginal->GetPosition().x(),aTrackOriginal->GetPosition().y(),aTrackOriginal->GetPosition().z()) ; // TO REPAIR  FROM ABOVE !!! NOT SMEARED NOW !!
   G4ThreeVector P = aTrackOriginal->GetMomentum();// TO REPAIR  FROM ABOVE !!! NOT SMEARED NOW !!
   double m = aTrackOriginal->GetDynamicParticle()->GetDefinition()->GetPDGMass();

   // G4cout<<"........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......"<<G4endl;
   // G4cout<<"_______ BEFORE ____ "<<G4endl<<G4endl;
   //    aTrackOriginal->GetDynamicParticle()->DumpInfo(); G4cout<<G4endl;
   //    G4cout<<" Vertex Info: \n ___Mom direction: "<<aTrackOriginal->GetVertexMomentumDirection()
   //          <<G4endl<<"___Mom: "<<aTrackOriginal->GetVertexMomentumDirection()*sqrt(aTrackOriginal->GetVertexKineticEnergy()*aTrackOriginal->GetVertexKineticEnergy()+2*aTrackOriginal->GetVertexKineticEnergy()*m)
   //          <<G4endl<<"___Ekin: "<<aTrackOriginal->GetVertexKineticEnergy()
   //          <<G4endl<<"___pos: "<<aTrackOriginal->GetVertexPosition()<<G4endl<<G4endl;


   aTrackOriginal->SetKineticEnergy(sqrt(P.mag2()-m*m)-m);
   aTrackOriginal->SetMomentumDirection(P.unit());
   aTrackOriginal->SetPosition(pos);
   aTrackOriginal->SetVertexKineticEnergy(sqrt(P.mag2()-m*m)-m);
   aTrackOriginal->SetVertexMomentumDirection(P.unit());
   aTrackOriginal->SetVertexPosition(pos);

   // G4cout<<"........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......"<<G4endl;
   // G4cout<<"_______ AFTEr ____ "<<G4endl<<G4endl;
   //    aTrackOriginal->GetDynamicParticle()->DumpInfo(); G4cout<<G4endl;
   //    G4cout<<" Vertex Info: \n ___Mom direction: "<<aTrackOriginal->GetVertexMomentumDirection()
   //          <<G4endl<<"___Mom: "<<aTrackOriginal->GetVertexMomentumDirection()*sqrt(aTrackOriginal->GetVertexKineticEnergy()*aTrackOriginal->GetVertexKineticEnergy()+2*aTrackOriginal->GetVertexKineticEnergy()*m)
   //          <<G4endl<<"___Ekin: "<<aTrackOriginal->GetVertexKineticEnergy()
   //          <<G4endl<<"___pos: "<<aTrackOriginal->GetVertexPosition()<<G4endl<<G4endl;

   // G4cout<<"........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......"<<G4endl;

   // G4cout<<"____Current particle : "<<aTrackOriginal->GetDefinition()->GetParticleName()<<" with ID  "<<aTrackOriginal->GetTrackID()<<G4endl;
   // G4cout<<" PHI : "<<originPhi<<" ->  "<<Phi<<G4endl;
   // G4cout<<" THETA : "<<originTheta<<" ->  "<<atan(1./cotTheta)<<G4endl;
   // G4cout<<" invPtCharge : "<<originPt<<" ->  "<<((double)(1./(double)invPtCharge))<<G4endl<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4double* FCCSmearer::ComputeTrackParams(G4double charge, G4double bField,
                          G4ThreeVector vertexMomentum, G4ThreeVector vertexPosition)
{
   // calculation of track parameter representation: implemented in Atlfast::TrackTrajectory
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

   G4double* params = new G4double[5];
   params[0] = impactParameter;
   params[1] = zPerigee;
   params[2] = phi;
   params[3] = cotTheta;
   params[4] = q/pt;
   return params;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector FCCSmearer::ComputePosFromParams(G4double* params)
{
   G4double x,y,z;
   G4ThreeVector pos (x,y,z);
   return pos;
}//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector FCCSmearer::ComputeMomFromParams(G4double* params)
{
   double Px = abs(1./params[4])*sin(params[2]);
   double Py = abs(1./params[4])*cos(params[2]);
   double Pz = abs(1./params[4])/sin( atan(1./params[3]) );

   G4ThreeVector P (Px,Py,Pz);
   return P;
}//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
