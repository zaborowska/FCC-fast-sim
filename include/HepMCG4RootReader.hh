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
/// \file eventgenerator/HepMC/HepMCEx03/include/HepMCG4RootReader.hh
/// \brief Definition of the HepMCG4RootReader class
//
//

#ifndef HEPMC_G4_ROOT_READER_H
#define HEPMC_G4_ROOT_READER_H

#include "HepMCG4Interface.hh"
#include "TROOT.h"
#include "TFile.h"
#include "TList.h"
#include "TSystem.h"
#include "TKey.h"

class HepMCG4RootReaderMessenger;

class HepMCG4RootReader : public HepMCG4Interface {
protected:
  G4String filename;
   TFile* rootInput;
   TObjLink *lnk;
   TIter* rootIter;

  G4int verbose;
  HepMCG4RootReaderMessenger* messenger;

  virtual HepMC::GenEvent* GenerateHepMCEvent();

public:
  HepMCG4RootReader();
  ~HepMCG4RootReader();

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

inline void HepMCG4RootReader::SetFileName(G4String name)
{
  filename= name;
}

inline G4String HepMCG4RootReader::GetFileName() const
{
  return filename;
}

inline void HepMCG4RootReader::SetVerboseLevel(G4int i)
{
  verbose= i;
}

inline G4int HepMCG4RootReader::GetVerboseLevel() const
{
  return verbose;
}

#endif
