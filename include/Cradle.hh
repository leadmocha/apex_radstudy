#ifndef Cradle_hh
#define Cradle_hh

#include "G4Material.hh"
#include "HallA.hh"

class G4LogicalVolume;
class B1DetectorConstruction;

class Cradle{
public:

  Cradle(G4double theta, B1DetectorConstruction *, HallA *, G4LogicalVolume *);
  ~Cradle();

protected:

  B1DetectorConstruction* fDetCon;
  G4Material* GetMaterial(G4String);

private:
  G4double thetaHRS;

};

#endif
