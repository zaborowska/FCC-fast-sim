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
/// \file eventgenerator/HepMC/HepMCEx01/src/HepMCG4RootReader.cc
/// \brief Implementation of the HepMCG4RootReader class
//
//

#include "HepMCG4RootReader.hh"
#include "HepMCG4RootReaderMessenger.hh"

#include <iostream>
#include <fstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
HepMCG4RootReader::HepMCG4RootReader()
   : filename("hepmc.root"), verbose(0)
{
   gSystem->Load("libCintex");
   gSystem->Load("libmyHepMC");
   ROOT::Cintex::Cintex::Enable();
   messenger= new HepMCG4RootReaderMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
HepMCG4RootReader::~HepMCG4RootReader()
{
   delete rootInput;
   delete messenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HepMCG4RootReader::Initialize()
{
   // delete TFile and TList !!!  TODO
   G4cout << filename << G4endl;
   rootInput= new TFile(filename.c_str());
   if(verbose>0) rootInput->ls();
   //lnk = (rootInput->GetListOfKeys())->FirstLink();
   //rootIter = new rootIter(rootInput->GetListOfKeys());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
HepMC::GenEvent* HepMCG4RootReader::GenerateHepMCEvent()
{
// TODO: rewrite so that it can outoput events one by one!
   HepMC::GenEvent* evt =  new HepMC::GenEvent();
   // TKey *key;
 // if (lnk)
   //  {
   //  key = (TKey*)rootIter();// lnk->GetObject();
   //HepMC::GenEvent* evt = new HepMC::GenEvent()//*) key->ReadObj();//rootInput->Get(key->GetName());
      rootInput->GetObject("Event_0",evt);
      // G4cout << "Getting HepMC EVENT: " << key->GetName() << G4endl;
     // lnk=lnk->Next();
      if(verbose>0) evt->print();
      //}
   G4cout << "End of GenerateEvent........................... "  << G4endl;
   //delete key;
   return evt;
}
