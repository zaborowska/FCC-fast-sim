#ifndef FCC_OUTPUT_H
#define FCC_OUTPUT_H

#include "G4ThreeVector.hh"
#include "globals.hh"

class FCCOutput
{
public:
   static FCCOutput* Instance();
   void SetFileName(G4String name);
   G4String GetFileName();
   void AppendName(G4bool app);
   void StartAnalysis(G4int runID);
   void EndAnalysis();
   void CreateNtuples();
   void CreateHistograms();
   void SaveTrack(G4bool HitDetector, G4int partID,  G4int PID,
                  G4ThreeVector momentum, G4double resolution = 0, G4double efficiency = 1) const;
   void FillHistogram(G4int HNo, G4double value) const;
   ~FCCOutput();
protected:
   FCCOutput();
private:
   static FCCOutput* fFCCOutput;
   G4String fFileName;
   G4bool fFileNameWithRunNo;
};

#endif
