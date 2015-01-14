#ifndef FCC_SMEARER_H
#define FCC_SMEARER_H

#include "globals.hh"
#include "G4Track.hh"
#include "CLHEP/Random/JamesRandom.h"
#include "CLHEP/Random/RandGauss.h"

/**
	@brief     Smears the particle momentum or energy based on given resolution.
   @details   A singleton class used to smear (alter) the particle momentum (for tracking detectors) and energy (for calorimeters). In case the resolution is given, the momentum (energy) is smeared with Gaussian distribution. In case of AtlFast-like smearing, the perigee parameters are smeared separately.
   @author    Anna Zaborowska
*/

class FCCSmearer
{
public:

   static FCCSmearer* Instance();
   G4ThreeVector Smear(const G4Track* aTrack, G4double resolution);
   G4double Smear(const G4double energy, G4double resolution);

   G4double Gauss(G4double mean, G4double stdDev);
protected:
   FCCSmearer();
   ~FCCSmearer();

private:
   static FCCSmearer* fFCCSmearer;
    CLHEP::HepRandomEngine*    fRandomEngine;
    CLHEP::RandGauss*    fRandomGauss;
};

#endif
