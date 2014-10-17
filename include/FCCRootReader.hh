//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// based on G4 examples/eventgenerator/HepMC/HepMCEx01/include/HepMCG4AsciiReader.hh
//

#ifndef FCC_ROOT_READER_H
#define FCC_ROOT_READER_H

#include "FCCHepMCInterface.hh"
#include "TROOT.h"
#include "TFile.h"
#include "TList.h"
#include "TSystem.h"
#include "TKey.h"

class FCCRootMessenger;

class FCCRootReader : public FCCHepMCInterface {
protected:
  G4String filename;
   TFile* rootInput;
   TObjLink *rootLnk;

  G4int verbose;
  FCCRootMessenger* messenger;

  virtual HepMC::GenEvent* GenerateHepMCEvent();

public:
  FCCRootReader();
  ~FCCRootReader();

  // set/get methods
  void SetFileName(G4String name);
  G4String GetFileName() const;

  void SetVerboseLevel(G4int i);
  G4int GetVerboseLevel() const; 

  // methods...
  void Initialize();
};

// ====================================================================
// inline functions
// ====================================================================

inline void FCCRootReader::SetFileName(G4String name)
{
  filename= name;
}

inline G4String FCCRootReader::GetFileName() const
{
  return filename;
}

inline void FCCRootReader::SetVerboseLevel(G4int i)
{
  verbose= i;
}

inline G4int FCCRootReader::GetVerboseLevel() const
{
  return verbose;
}

#endif
