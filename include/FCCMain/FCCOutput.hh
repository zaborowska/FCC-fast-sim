#ifndef FCC_OUTPUT_H
#define FCC_OUTPUT_H

#include "G4ThreeVector.hh"
#include "globals.hh"

class FCCOutput
{
public:
   enum SaveType {eMC, eTracker, eEMCal, eHCal};

   static FCCOutput* Instance();
   void SetFileName(G4String name);
   G4String GetFileName();
   void AppendName(G4bool app);
   void StartAnalysis(G4int runID);
   void EndAnalysis();
   void CreateNtuples();
   void CreateHistograms();
   void SaveTrack(SaveType what, G4int partID,  G4int PID,
                  G4ThreeVector vec, G4double resolution = 0, G4double efficiency = 1, G4double energgy = 0);
   void FillHistogram(G4int HNo, G4double value) const;
   ~FCCOutput();
protected:
   FCCOutput();
private:
   static FCCOutput* fFCCOutput;
   G4String fFileName;
   G4bool fFileNameWithRunNo;
   G4int fCurrentID;
};

#endif
