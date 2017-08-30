#ifndef Gantry_hh
#define Gantry_hh

#include "G4Material.hh"
#include "HallA.hh"

class G4LogicalVolume;
class B1DetectorConstruction;

class Gantry{
public:

  Gantry(G4double theta, B1DetectorConstruction *, HallA *, G4LogicalVolume *);
  ~Gantry();

protected:

  B1DetectorConstruction* fDetCon;
  G4Material* GetMaterial(G4String);

private:
  G4double thetaHRS;

};

#endif
