#include "FCCSmearer.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
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
   // delete fParams;
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

G4ThreeVector FCCSmearer::Smear(const G4Track* aTrackOriginal, G4double aResolution)
{
   // original track position, momentum and charge
   G4ThreeVector originP = aTrackOriginal->GetMomentum();
   G4ThreeVector originPos = aTrackOriginal->GetPosition();
   G4double rdm = Gauss(1,aResolution);
   G4ThreeVector smearedMom (originP.x()*rdm,  originP.y()*rdm,  originP.z()*rdm);

   return smearedMom;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double FCCSmearer::Smear(const G4double aEnergyOrg, G4double aResolution)
{
   G4double newE = -1;
   while (newE < 0) // to ensure the resulting value is not negative (vital for energy smearing, not changing directin for momentum smearing)
   {
      newE = aEnergyOrg * Gauss(1,aResolution);
   }
   return newE;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double FCCSmearer::Gauss(G4double aMean, G4double aStandardDeviation)
{
   return fRandomGauss->fire(aMean, aStandardDeviation);
}
