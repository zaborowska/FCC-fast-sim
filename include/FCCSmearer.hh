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
   G4Track* Smear(const G4Track* aTrack);
protected:
   FCCSmearer();
   ~FCCSmearer();
private:
   static FCCSmearer* fFCCSmearer;
   Atlfast::ElectronMatrixManager* fElectronManager;
   Atlfast::PionMatrixManager* fPionManager;
   Atlfast::MuonMatrixManager* fMuonManager;
};

#endif
