#ifndef FCCTrackingAction_h
#define FCCTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class FCCTrackingAction : public G4UserTrackingAction
{
  public:
    FCCTrackingAction();
    virtual ~FCCTrackingAction();

   virtual void  PostUserTrackingAction(const G4Track* track);

  private:
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


