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
// based on G4 examples/basic/B5/include/B5EmCalorimeterHit.hh
//

#ifndef FCC_EM_CALORIMETER_HIT_H
#define FCC_EM_CALORIMETER_HIT_H

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

class G4AttDef;
class G4AttValue;

/// EM Calorimeter hit
///
/// It records:
/// - the cell ID
/// - the energy deposit
/// - the cell logical volume, its position and rotation

class FCCEmCalorimeterHit : public G4VHit
{
public:
    FCCEmCalorimeterHit();
    FCCEmCalorimeterHit(G4int z);
    FCCEmCalorimeterHit(const FCCEmCalorimeterHit &right);
    virtual ~FCCEmCalorimeterHit();

    const FCCEmCalorimeterHit& operator=(const FCCEmCalorimeterHit &right);
    int operator==(const FCCEmCalorimeterHit &right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);

    virtual void Draw();
    virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
    virtual std::vector<G4AttValue>* CreateAttValues() const;
    virtual void Print();

    void SetCellID(G4int z) { fCellID = z; }
    G4int GetCellID() const { return fCellID; }

    void SetEdep(G4double de) { fEdep = de; }
    void AddEdep(G4double de) { fEdep += de; }
    G4double GetEdep() const { return fEdep; }

    void SetPos(G4ThreeVector xyz) { fPos = xyz; }
    G4ThreeVector GetPos() const { return fPos; }

    void SetRot(G4RotationMatrix rmat) { fRot = rmat; }
    G4RotationMatrix GetRot() const { return fRot; }

    void SetLogV(G4LogicalVolume* val) { fPLogV = val; }
    const G4LogicalVolume* GetLogV() const { return fPLogV; }

private:
    G4int fCellID;
    G4double fEdep;
    G4ThreeVector fPos;
    G4RotationMatrix fRot;
    const G4LogicalVolume* fPLogV;
};

typedef G4THitsCollection<FCCEmCalorimeterHit> FCCEmCalorimeterHitsCollection;

extern G4ThreadLocal G4Allocator<FCCEmCalorimeterHit>* FCCEmCalorimeterHitAllocator;

inline void* FCCEmCalorimeterHit::operator new(size_t)
{
    if (!FCCEmCalorimeterHitAllocator)
        FCCEmCalorimeterHitAllocator = new G4Allocator<FCCEmCalorimeterHit>;
    return (void*)FCCEmCalorimeterHitAllocator->MallocSingle();
}

inline void FCCEmCalorimeterHit::operator delete(void* aHit)
{
    FCCEmCalorimeterHitAllocator->FreeSingle((FCCEmCalorimeterHit*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
