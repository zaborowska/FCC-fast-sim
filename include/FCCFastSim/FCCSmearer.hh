#ifndef FCC_SMEARER_H
#define FCC_SMEARER_H

#include "FCCOutput.hh"
#include "globals.hh"
#include "G4Track.hh"
#include "CLHEP/Random/JamesRandom.h"
#include "CLHEP/Random/RandGauss.h"
#include "AtlfastMuonMatrixManager.hh"
#include "AtlfastPionMatrixManager.hh"
#include "AtlfastElectronMatrixManager.hh"

/**
	@brief     Smearing of the particle momentum or energy.
   @details   A singleton class used to smear (alter) the particle momentum (for tracking detectors) and energy (for calorimeters). In case the resolution is given, the momentum (energy) is smeared with Gaussian distribution. In case of AtlFast-like smearing, the perigee parameters are smeared separately.
   @author    Anna Zaborowska
*/

class FCCSmearer
{
public:

   static FCCSmearer* Instance();
   void MakeManagers();

   G4ThreeVector SmearMomentum(const G4Track* aTrack, G4double resolution = -1, FCCOutput::SaveType aSavePerigee  = FCCOutput::eNoSave);
   G4double SmearEnergy(const G4Track* aTrack, G4double resolution = -1, FCCOutput::SaveType aSavePerigee  = FCCOutput::eNoSave);

   G4ThreeVector SmearGaussian(const G4Track* aTrackOriginal, G4double stdDev);
   G4double Gauss(G4double mean, G4double stdDev);

   G4ThreeVector SmearPerigee(const G4Track* aTrackOriginal, FCCOutput::SaveType aSavePerigee  = FCCOutput::eNoSave);
   G4double* Atlfast(const G4Track* aTrackOriginal);
   G4double* ComputeTrackParams(G4double charge, G4ThreeVector vertexMomentum, G4ThreeVector vertexPosition);
   G4ThreeVector ComputePosFromParams(G4double* params, G4double phiVertex);
   G4ThreeVector ComputeMomFromParams(G4double* params);
   G4double CheckPhi(G4double Phi);

protected:
   FCCSmearer();
   ~FCCSmearer();

private:
   static FCCSmearer* fFCCSmearer;
   CLHEP::HepRandomEngine*    fRandomEngine;
   CLHEP::RandGauss*    fRandomGauss;
   Atlfast::ElectronMatrixManager* fElectronManager;
   Atlfast::PionMatrixManager* fPionManager;
   Atlfast::MuonMatrixManager* fMuonManager;
};

#endif
