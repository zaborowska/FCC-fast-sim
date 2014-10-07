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
//
// $Id: examplePar01.cc 77659 2013-11-27 08:57:46Z gcosmo $
//
// 
// --------------------------------------------------------------
//      GEANT 4 - examplePar01
// --------------------------------------------------------------
// Comments
//
// Example of a main program making use of parameterisation
// i.e. "Fast Simulation"
//-------------------------------------------------------------------

//-------------------
// Generator action:
//-------------------
#include "H02PrimaryGeneratorAction.hh"

//---------------------------
// Parameterisation manager:
//---------------------------
#include "G4GlobalFastSimulationManager.hh"

//------------
// Geometries:
//------------
#include "G01DetectorConstruction.hh"
#include "G4GDMLParser.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4TransportationManager.hh"

//-----------------------------------
// Par01PhysicsList (makes use of the
// G4ParameterisationManagerProcess):
//-----------------------------------
#include "Par01PhysicsList.hh"

// Sensitive Detector
#include "B2TrackerSD.hh"
#include "G4SDManager.hh"

#include "G4UImanager.hh"
#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif
#include "Par01ActionInitialization.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif


#include "FTFP_BERT.hh"

int main(int argc, char** argv)
{
   if (argc<2)
   {
      G4cout << G4endl;
      G4cout << "Error! Mandatory input file is not specified!" << G4endl;
      G4cout << G4endl;
      G4cout << "Usage: "<<argv[0]<<" <intput_gdml:mandatory>"
             << " <macro_with_pythia_settings:optional>" << G4endl;
      G4cout << G4endl;
      return -1;
   }
   G4GDMLParser parser;
   // Load geometry
   G4cout << "Geometry loaded from  file ......." << argv[1]<<G4endl;
   parser.Read(argv[1]);
   //-------------------------------
   // Initialization of Run manager
   //-------------------------------
#ifdef G4MULTITHREADED
   G4MTRunManager * runManager = new G4MTRunManager;
   runManager->SetNumberOfThreads(4);
   G4cout<<"+-------------------------------------------------------+"<<G4endl;
   G4cout<<"|              Constructing MT run manager              |"<<G4endl;
   G4cout<<"+-------------------------------------------------------+"<<G4endl;
#else
   G4RunManager * runManager = new G4RunManager;
   G4cout<<"+-------------------------------------------------------+"<<G4endl;
   G4cout<<"|        Constructing sequential run manager            |"<<G4endl;
   G4cout<<"+-------------------------------------------------------+"<<G4endl;
#endif

   // Detector/mass geometry and parallel geometry(ies):
   G4VUserDetectorConstruction* detector = new G01DetectorConstruction(parser.GetWorldVolume());

   // --  The name passed must be the same passed to the
   // -- G4FastSimulationManagerProcess attached to the pions
   runManager->SetUserInitialization(detector);
  
   // PhysicsList (including G4FastSimulationManagerProcess)
   G4VUserPhysicsList* physicsList = new Par01PhysicsList;//FTFP_BERT;
   runManager->SetUserInitialization(physicsList);

   //-------------------------------
   // UserAction classes
   //-------------------------------
   runManager->SetUserInitialization( new Par01ActionInitialization );

   // Initialize Run manager
   runManager->Initialize();


   //------------------------------------------------ 
   // Sensitive detectors
   //------------------------------------------------ 
   
   G4SDManager* SDman = G4SDManager::GetSDMpointer();
   

   G4String trackerChamberSDname = "Tracker";
   B2TrackerSD* aTrackerSD = new B2TrackerSD(trackerChamberSDname, "Tracker");
   SDman->AddNewDetector( aTrackerSD );


    ///////////////////////////////////////////////////////////////////////
    //
    // Example how to retrieve Auxiliary Information
    //


   const G4GDMLAuxMapType* auxmap = parser.GetAuxMap();
   std::cout << "Found " << auxmap->size()
             << " volume(s) with auxiliary information."
             << G4endl << G4endl;

   // The same as above, but now we are looking for
   // sensitive detectors setting them for the volumes

   for(G4GDMLAuxMapType::const_iterator iter=auxmap->begin();
       iter!=auxmap->end(); iter++) 
   {
     G4cout << "Volume " << ((*iter).first)->GetName()
            << " has the following list of auxiliary information: "
            << G4endl << G4endl;
     for (G4GDMLAuxListType::const_iterator vit=(*iter).second.begin();
          vit!=(*iter).second.end();vit++)
     {
       if ((*vit).type=="SensDet")
       {
         G4cout << "Attaching sensitive detector " << (*vit).value
                << " to volume " << ((*iter).first)->GetName()
                <<  G4endl << G4endl;

         G4VSensitiveDetector* mydet = SDman->FindSensitiveDetector((*vit).value);
         if(mydet) 
         {
           G4LogicalVolume* myvol = (*iter).first;
           myvol->SetSensitiveDetector(mydet);
         }
         else
         {
           G4cout << (*vit).value << " detector not found" << G4endl;
         }
       }
     }
   }
    //
    // End of Auxiliary Information block
    //
    ////////////////////////////////////////////////////////////////////////


   G4UImanager* UImanager = G4UImanager::GetUIpointer();

   if(argc==2)
   {
      //--------------------------
      // Define (G)UI
      //--------------------------
#ifdef G4UI_USE
      G4UIExecutive * ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
      G4VisManager* visManager = new G4VisExecutive;
      visManager->Initialize();
      UImanager->ApplyCommand("/control/execute vis.mac");
#endif
      ui->SessionStart();
#ifdef G4VIS_USE
      delete visManager;
#endif
      delete ui;
#endif
   }
   else
   {
      G4String command = "/control/execute ";
      G4String fileName = argv[2];
      UImanager->ApplyCommand(command+fileName);
   }

   // Free the store: user actions, physics_list and detector_description are
   //                 owned and deleted by the run manager, so they should not
   //                 be deleted in the main() program !

   delete runManager;

   return 0;
}
