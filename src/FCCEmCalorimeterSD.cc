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
// based on G4 examples/basic/B5/src/B5EmCalorimeterSD.cc
//

#include "FCCEmCalorimeterSD.hh"
#include "FCCEmCalorimeterHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FCCEmCalorimeterSD::FCCEmCalorimeterSD(G4String name)
: G4VSensitiveDetector(name), fHitsCollection(0), fHCID(-1)
{
    collectionName.insert("EMcalorimeterColl");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FCCEmCalorimeterSD::~FCCEmCalorimeterSD()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FCCEmCalorimeterSD::Initialize(G4HCofThisEvent* hce)
{
    fHitsCollection
      = new FCCEmCalorimeterHitsCollection(SensitiveDetectorName,collectionName[0]);
    if (fHCID<0)
    { fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection); }
    hce->AddHitsCollection(fHCID,fHitsCollection);

    // fill calorimeter hits with zero energy deposition
    for (G4int i=0;i<80;i++)
    {
        FCCEmCalorimeterHit* hit = new FCCEmCalorimeterHit(i);
        fHitsCollection->insert(hit);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool FCCEmCalorimeterSD::ProcessHits(G4Step*step, G4TouchableHistory*)
{
    G4double edep = step->GetTotalEnergyDeposit();
    if (edep==0.) return true;

    G4TouchableHistory* touchable
      = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
    G4VPhysicalVolume* physical = touchable->GetVolume();
    G4int copyNo = physical->GetCopyNo();

    FCCEmCalorimeterHit* hit = (*fHitsCollection)[copyNo];
    // check if it is first touch
    if (!(hit->GetLogV()))
    {
        // fill volume information
        hit->SetLogV(physical->GetLogicalVolume());
        G4AffineTransform transform
          = touchable->GetHistory()->GetTopTransform();
        transform.Invert();
        hit->SetRot(transform.NetRotation());
        hit->SetPos(transform.NetTranslation());
    }
    // add energy deposition
    hit->AddEdep(edep);

    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
