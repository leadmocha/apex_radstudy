#ifndef APEXTarget_hh
#define APEXTarget_hh

#include "G4Material.hh"
#include "HallA.hh"

class G4LogicalVolume;
class B1DetectorConstruction;

class APEXTarget{
public:

  APEXTarget(B1DetectorConstruction *, HallA *, G4LogicalVolume *);
  ~APEXTarget();

protected:

  B1DetectorConstruction* fDetCon;
  G4Material* GetMaterial(G4String);

};

#endif
