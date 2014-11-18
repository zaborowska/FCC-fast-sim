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
   void StartAnalysis(G4int runID);
   void EndAnalysis();
   void CreateNtuples();
   void SaveTrack(G4bool ifDetected, G4int partID, G4int PID, G4ThreeVector P) const;
protected:
   FCCOutput();
   ~FCCOutput();
private:
   static FCCOutput* fFCCOutput;
   G4String fFileName;
};

#endif
