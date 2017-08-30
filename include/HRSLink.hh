#ifndef HRSLink_hh
#define HRSLink_hh

#include "G4Material.hh"
#include "HallA.hh"

class G4LogicalVolume;
class B1DetectorConstruction;

class HRSLink{
public:

  HRSLink(G4double theta, B1DetectorConstruction *, HallA *, G4LogicalVolume *);
  ~HRSLink();

protected:

  B1DetectorConstruction* fDetCon;
  G4Material* GetMaterial(G4String);

private:
  G4double thetaHRS;

};

#endif
