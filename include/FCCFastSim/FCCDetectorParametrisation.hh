#ifndef FCC_DETECTOR_PARAMETRSIATION_H
#define FCC_DETECTOR_PARAMETRSIATION_H

#include "globals.hh"

/**
	@brief     Definition of detector resolution and efficiency.
   @details   A simple class used to provide the detector resolution and efficiency (dependent on the detector, parametrisation type and particle momentum).
   @author    Anna Zaborowska
*/

class FCCDetectorParametrisation
{
public:
   /**
      A default constructor.
    */
   FCCDetectorParametrisation();
   ~FCCDetectorParametrisation();
   /**
      A parametrisation type (CMS, ATLAS, ALEPH or AtlFast-like).
    */
   enum Parametrisation {eCMS, eATLAS, eALEPH, eATLFAST};
   /**
      A detector type (tracking detector, electromagnetic calorimeter or hadronic calorimeter).
    */
   enum Detector {eTracker,eEMCal, eHCal};
   /**
      Gets the resolution of a detector for a given particle.
      @param det A detector type.
      @param par A parametrisation type.
      @param mom A particle momentum.
    */
   G4double GetResolution(Detector det, Parametrisation par, G4double mom);
   /**
      Gets the efficiency of a detector for a given particle.
      @param det A detector type.
      @param par A parametrisation type.
      @param mom A particle momentum.
    */
   G4double GetEfficiency(Detector det, Parametrisation par, G4double mom);

};

#endif
