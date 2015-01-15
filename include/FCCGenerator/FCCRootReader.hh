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

#ifndef FCC_ROOT_READER_H
#define FCC_ROOT_READER_H

#include "FCCHepMCInterface.hh"
#include "TROOT.h"
#include "TFile.h"
#include "TList.h"
#include "TSystem.h"
#include "TKey.h"

class FCCRootMessenger;

/**
   @brief     Reader of a ROOT file containing events.
   @details   A reader of the ROOT file with an event stored in a HepMC format. Based on G4 examples/extended/eventgenerator/HepMC/HepMCEx01/include/HepMCG4AsciiReader.hh.
 	@author    Anna Zaborowska
*/

class FCCRootReader : public FCCHepMCInterface {
protected:
   /**
      A name of the ROOT file with the event.
   */
   G4String filename;
   /**
      A pointer to the ROOT file with events.
   */
   TFile* rootInput;
   /**
      A link to the file content (set to the next-to-use event).
   */
   TObjLink *rootLnk;
   /**
      A verbosity level.
   */
   G4int verbose;
   /**
      A pointer to the messenger that connects the UI with this class.
   */
   FCCRootMessenger* messenger;
   /**
      Reads an event saved in a HepMC standard format, in the ROOT file.
   */
   virtual HepMC::GenEvent* GenerateHepMCEvent();

public:
   /**
      A default constructor.
   */
  FCCRootReader();
  ~FCCRootReader();
   /**
      Sets the name of the ROOT file with events.
      @param name A file name.
   */
  void SetFileName(G4String name);
   /**
      Gets the name of the ROOT file with events.
   */
  G4String GetFileName() const;
   /**
      Sets the verbosity level.
      @param name A verbosity level.
   */
  void SetVerboseLevel(G4int i);
   /**
      Gets the verbosity level.
   */
  G4int GetVerboseLevel() const;
   /**
      Opens the file and sets the link to the first event in the file.
   */
  void Initialize();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

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
