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

// Generator action:
#include "FCCActionInitialization.hh"
// Parameterisation manager:
#include "G4GlobalFastSimulationManager.hh"
// Geometry:
#include "FCCDetectorConstruction.hh"
//Sensitive Detectors
#include "FCCSDInfo.hh"
// PhysicsList
#include "FTFP_BERT.hh"
// UI
#include "G4UImanager.hh"
#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif
#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif


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
   G4VUserDetectorConstruction* detector = new FCCDetectorConstruction(parser.GetWorldVolume());

   // --  The name passed must be the same passed to the
   // -- G4FastSimulationManagerProcess attached to the pions
   runManager->SetUserInitialization(detector);
  
   // PhysicsList (including G4FastSimulationManagerProcess)
   G4VUserPhysicsList* physicsList = new FTFP_BERT;// FCCPhysicsList;
   runManager->SetUserInitialization(physicsList);

   //-------------------------------
   // UserAction classes
   //-------------------------------
   runManager->SetUserInitialization( new FCCActionInitialization );

   // Initialize Run manager
   runManager->Initialize();


   //-------------------------------
   // Sensitive detectors
   //------------------------------------------------ 

   // const G4GDMLAuxMapType* auxmap = parser.GetAuxMap();
   // FCCSDInfo SensDetCreater(auxmap);

   //-------------------------------
   // UI
   //-------------------------------
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
