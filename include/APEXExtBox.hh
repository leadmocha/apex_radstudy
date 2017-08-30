#ifndef APEXExtBox_hh
#define APEXExtBox_hh

#include "G4Material.hh"
#include "HallA.hh"

class G4LogicalVolume;
class B1DetectorConstruction;

class APEXExtBox{
public:

  APEXExtBox(B1DetectorConstruction *, HallA *, G4LogicalVolume *);
  ~APEXExtBox();

protected:

  B1DetectorConstruction* fDetCon;
  G4Material* GetMaterial(G4String);

};

#endif
