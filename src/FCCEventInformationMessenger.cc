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

#include "FCCEventInformationMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCEventInformationMessenger::FCCEventInformationMessenger
                            (FCCEventInformation* evinfo)
  : eventInfo(evinfo)
{
  dir= new G4UIdirectory("/fastsim/");
  dir-> SetGuidance("Control commands for parametric fast simulation");

  //verbose= new G4UIcmdWithAnInteger("/generator/verbose", this);
  //verbose-> SetGuidance("set verbose level (0,1,2)");
  //verbose-> SetParameterName("verbose", false, false);
  //verbose-> SetDefaultValue(0);
  //verbose-> SetRange("verbose>=0 && verbose<=2");

  smear= new G4UIcmdWithABool("/fastsim/smear", this);
  smear-> SetGuidance("choose if smearing is done");
  smear-> SetParameterName("smear", false, false);
  smear->SetDefaultValue(TRUE);

  tracker= new G4UIcmdWithAString("/fastsim/tracker", this);
  tracker-> SetGuidance("select tracker parametrisation");
  tracker-> SetParameterName("tracker_param", false, false);
  tracker-> SetCandidates("ATLAS CMS APHEA");

  emcal= new G4UIcmdWithAString("/fastsim/EMcal", this);
  emcal-> SetGuidance("select electromagnetic calorimeter parametrisation");
  emcal-> SetParameterName("emcal_param", false, false);
  emcal-> SetCandidates("ATLAS CMS APHEA");

  hcal= new G4UIcmdWithAString("/fastsim/Hcal", this);
  hcal-> SetGuidance("select hadron calorimeter parametrisation");
  hcal-> SetParameterName("hcal_param", false, false);
  hcal-> SetCandidates("ATLAS CMS APHEA");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCEventInformationMessenger::~FCCEventInformationMessenger()
{
   //delete verbose;
   delete smear;
   delete tracker;
   delete emcal;
   delete hcal;
   delete dir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FCCEventInformationMessenger::SetNewValue(G4UIcommand* command,
                                              G4String newValues)
{
  if ( command==smear) {
    eventInfo-> SetDoSmearing(newValues);
    G4cout << "Setting smearing in detector: "
            << eventInfo-> GetDoSmearing() << G4endl;
  } else if ( command==tracker) {
    eventInfo-> SetTrackerParametrisation(newValues);
    G4cout << "Setting tracker parametrisation: "
            << eventInfo-> GetTrackerParametrisation() << G4endl;
  } else if ( command==emcal) {
    eventInfo-> SetEMCalParametrisation(newValues);
    G4cout << "Setting EM calorimeter parametrisation: "
            << eventInfo-> GetEMCalParametrisation() << G4endl;
  } else if ( command==hcal) {
    eventInfo-> SetHCalParametrisation(newValues);
    G4cout << "Setting H calorimeter parametrisation: "
            << eventInfo-> GetHCalParametrisation() << G4endl;
  } else {
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4String FCCEventInformationMessenger::GetCurrentValue(G4UIcommand* command)
{
   G4String cv;
   if ( command==smear) {
      cv = G4UIcommand::ConvertToString( eventInfo-> GetDoSmearing());
   } else if ( command==tracker) {
      cv = eventInfo-> GetTrackerParametrisation();
   } else if ( command==emcal) {
      cv = eventInfo-> GetEMCalParametrisation();
   } else if ( command==hcal) {
      eventInfo-> SetHCalParametrisation(newValues);
      cv = eventInfo-> GetHCalParametrisation();
   }

   return cv;
}
