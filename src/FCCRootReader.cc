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
// based on G4 examples/extended/eventgenerator/HepMC/HepMCEx01/src/HepMCG4AsciiReader.cc
//

#include "FCCRootReader.hh"
#include "FCCRootMessenger.hh"

#include <iostream>
#include <fstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCRootReader::FCCRootReader()
   : verbose(0)
{
   gSystem->Load("libHepMCdict");
   messenger= new FCCRootMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCRootReader::~FCCRootReader()
{
   //delete rootInput;
   delete messenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FCCRootReader::Initialize()
{
   G4cout << filename << G4endl;
   rootInput= new TFile(filename.c_str());
   if(verbose>0) rootInput->ls();
   rootLnk = (rootInput->GetListOfKeys())->FirstLink();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
HepMC::GenEvent* FCCRootReader::GenerateHepMCEvent()
{
   HepMC::GenEvent* evt;
   G4String keyName;
   TKey* key;
   while(rootLnk)
   {
      key = (TKey*)rootLnk->GetObject();
      evt = (HepMC::GenEvent*) rootInput->Get(key->GetName());
      if(verbose>0) G4cout << "Getting HepMC EVENT named: " << key->GetName() << G4endl;
      rootLnk=rootLnk->Next();
      if(verbose>0) evt->print();
   }
   //delete key;
   return evt;
}
