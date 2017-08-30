#ifndef SeptaMagnet_hh
#define SeptaMagnet_hh

#include "G4Material.hh"
#include "HallA.hh"

class G4LogicalVolume;
class B1DetectorConstruction;

class SeptaMagnet{
public:

  SeptaMagnet(B1DetectorConstruction *, HallA *, G4LogicalVolume *, G4bool fieldON);
  ~SeptaMagnet();

protected:

  B1DetectorConstruction* fDetCon;
  G4bool fieldON;
  G4Material* GetMaterial(G4String);

};

#endif
