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
#include "FCCOutputMessenger.hh"
#include "G4UIparameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCOutputMessenger::FCCOutputMessenger (FCCOutput* output)
  : fOutput(output)
{
  dir= new G4UIdirectory("/output/");
  dir-> SetGuidance("Control commands for output control");

  select= new G4UIcmdWithAString("/output/filename", this);
  select-> SetGuidance("set output filename (without '.root' extension)");
  select-> SetParameterName("file_name", false, false);
  select-> SetDefaultValue("SimpleOutput");

  append= new G4UIcmdWithABool("/output/appendFileName", this);
  append-> SetGuidance("choose if to fileName '_runX' should be added");
  append-> SetParameterName("append", false, false);
  append-> SetDefaultValue(false);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FCCOutputMessenger::~FCCOutputMessenger()
{
  delete select;
  delete append;
  delete dir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FCCOutputMessenger::SetNewValue(G4UIcommand* command,
                                              G4String newValues)
{
  if ( command==select)
  {
    fOutput-> SetFileName(newValues);
    G4cout << "current output file name: "
            << fOutput-> GetFileName() << G4endl;
  }
  else if ( command==append)
  {
    fOutput-> AppendName(newValues);
    G4cout << "current output file name: "
            << fOutput-> GetFileName() << G4endl;
  }
  else
  {; }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4String FCCOutputMessenger::GetCurrentValue(G4UIcommand* command)
{
  G4String cv, st;
  if (command == select) {
    cv= fOutput-> GetFileName();
  }
 return cv;
}
