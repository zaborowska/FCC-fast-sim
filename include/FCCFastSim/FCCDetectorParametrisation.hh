#ifndef FCC_DETECTOR_PARAMETRSIATION_H
#define FCC_DETECTOR_PARAMETRSIATION_H

#include "globals.hh"

class FCCDetectorParametrisation
{
public:
   FCCDetectorParametrisation();
   ~FCCDetectorParametrisation();

   enum Parametrisation {eCMS, eATLAS, eALEPH, eATLFAST};
   enum Detector {eTracker,eEMCal, eHCal};

   G4double GetResolution(Detector det, Parametrisation par, G4double mom);
   G4double GetEfficiency(Detector det, Parametrisation par, G4double mom);

};

#endif
