#ifndef FCC_SMEARER_H
#define FCC_SMEARER_H

#include "AtlfastMuonMatrixManager.hh"
#include "AtlfastPionMatrixManager.hh"
#include "AtlfastElectronMatrixManager.hh"
#include "globals.hh"
#include "G4Track.hh"

class FCCSmearer
{
public:
   static FCCSmearer* Instance();
   void MakeManagers();
   double* Smear(const G4Track* aTrack);
   G4double* ComputeTrackParams(G4double charge, G4ThreeVector vertexMomentum, G4ThreeVector vertexPosition);
   G4ThreeVector ComputePosFromParams(G4double* params, G4double diffAngle);
   G4ThreeVector ComputeMomFromParams(G4double* params);
   G4double CheckPhi(G4double Phi);
protected:
   FCCSmearer();
   ~FCCSmearer();
private:
   static FCCSmearer* fFCCSmearer;
   Atlfast::ElectronMatrixManager* fElectronManager;
   Atlfast::PionMatrixManager* fPionManager;
   Atlfast::MuonMatrixManager* fMuonManager;

   G4double bField;
};

#endif
