#include "FCCDetectorParametrisation.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

FCCDetectorParametrisation::FCCDetectorParametrisation(){}
FCCDetectorParametrisation::~FCCDetectorParametrisation(){}

G4double FCCDetectorParametrisation::GetResolution(Detector det, Parametrisation par, G4double mom)
{
   mom /= GeV; // to make sure momentum's unit is GeV
   G4double res = 1;
   if(par == eCMS)
   {
      switch(det)
      {
      case FCCDetectorParametrisation::eTracker:
         res = 0.013;
         break;
      case FCCDetectorParametrisation::eEMCal:
         res = sqrt(pow(0.03/sqrt(mom),2) // stochastic
                    + pow(0.12/mom, 2)    // noise
                    + pow(0.003,2));      // constant
         break;
      case FCCDetectorParametrisation::eHCal:
         res = sqrt(pow(1.1/sqrt(mom),2) // stochastic
                    + pow(0.09,2));      // constant
         break;
      }
   }
   else if(par == eATLAS)
   {
      switch(det)
      {
      case FCCDetectorParametrisation::eTracker:
         res = 0.01;
         break;
      case FCCDetectorParametrisation::eEMCal:
         res = sqrt(pow(0.18/sqrt(mom),2) // stochastic
                    + pow(0.009,2));      // constant
         break;
      case FCCDetectorParametrisation::eHCal:
         res = 0.85/sqrt(mom); // stochastic
         break;
      }
   }
   else if(par == eALEPH)
   {
      switch(det)
      {
      case FCCDetectorParametrisation::eTracker:
         res = 0.01;
         break;
      case FCCDetectorParametrisation::eEMCal:
         res = sqrt(pow(0.1/sqrt(mom),2) // stochastic
                    + pow(0.17,2));      // constant
         break;
      case FCCDetectorParametrisation::eHCal:
         res = sqrt(pow(0.55/sqrt(mom),2) // stochastic
                    + pow(0.06,2));      // constant
         break;
      }
   }
   else if(par == eATLFAST)
   {
      res = -1;
   }
   return res;
}

G4double FCCDetectorParametrisation::GetEfficiency(Detector det, Parametrisation par, G4double mom)
{
   G4double eff = 1;
   switch(det)
   {
   case FCCDetectorParametrisation::eTracker:
      eff = 1;
         break;
   case FCCDetectorParametrisation::eEMCal:
      eff = 1;
         break;
   case FCCDetectorParametrisation::eHCal:
      eff = 1;
         break;
   }
   return eff;
}
