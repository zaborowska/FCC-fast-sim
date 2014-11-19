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
   double originPhi = aTrackOriginal->GetMomentum().phi();
   double originTheta = aTrackOriginal->GetMomentum().theta();
   double originCharge = aTrackOriginal->GetDynamicParticle()->GetCharge();
   double originPt = aTrackOriginal->GetMomentum().perp();

   // Atlfast smeared variables
   double impactParameter; // [0]
   double zPerigee; //[1]
   double Phi = originPhi +  smearVariables[2]; //[2]
   if(Phi<-M_PI)
      while(Phi<M_PI)
         Phi+=2*M_PI;
   else if(Phi>M_PI)
      while(Phi>M_PI)
         Phi-=2*M_PI;
   double cotTheta = 1/tan(originTheta) + smearVariables[3] ; //[3]
   double invPtCharge = (double)((double)originCharge/(abs((double)originCharge)*originPt)) +  smearVariables[4]; // q/pT where q = q/|q| (just sign) //[4]

   G4ThreeVector pos( aTrackOriginal->GetPosition().x(),aTrackOriginal->GetPosition().y(),aTrackOriginal->GetPosition().z()) ; // TO REPAIR  FROM ABOVE !!! NOT SMEARED NOW !!

   // back to P
   double Px = abs(1./invPtCharge)*sin(Phi);
   double Py = abs(1./invPtCharge)*cos(Phi);
   double Pz = abs(1./invPtCharge)/sin( atan(1./cotTheta) );

   G4ThreeVector P (Px,Py,Pz);
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
