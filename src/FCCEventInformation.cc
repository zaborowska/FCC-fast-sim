#include "FCCEventInformation.hh"

FCCEventInformation::FCCEventInformation() :fDoSmearing(true), fEnergy(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCEventInformation::FCCEventInformation(G4bool aSmear, G4double aEnergy): fDoSmearing(aSmear)
{
   SetEnergy(aEnergy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCEventInformation::~FCCEventInformation(){}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FCCEventInformation::SetDoSmearing(G4bool aSmear)
{
   fDoSmearing = aSmear;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool FCCEventInformation::GetDoSmearing()
{
   return fDoSmearing;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FCCEventInformation::SetEnergy(G4double aEnergy)
{
   fEnergy = aEnergy;
   if(fEnergy) fEnergyUse = true;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4double FCCEventInformation::GetEnergy()
{
   return fEnergy;
}//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool FCCEventInformation::GetUseEnergy()
{
   return fEnergyUse;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FCCEventInformation::Print() const
{
   G4cout<<"FCCEventInformation: "<<G4endl
         <<"do smearing: "<<fDoSmearing<<G4endl
         <<"particle gun energy: "<<fEnergy<<G4endl;
}
