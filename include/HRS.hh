#ifndef HRS_hh
#define HRS_hh

#include "G4Material.hh"
#include "HallA.hh"
#include "G4RotationMatrix.hh"

class G4LogicalVolume;
class B1DetectorConstruction;

class HRS{
public:

  HRS(G4double theta, B1DetectorConstruction *, HallA *, G4LogicalVolume *);
  ~HRS();

protected:

  B1DetectorConstruction* fDetCon;
  G4Material* GetMaterial(G4String);

private:
  G4double thetaHRS;
  void MakeQ1Superconducting(G4LogicalVolume *, G4double cut_angle);
  void MakeQ1Copper(G4LogicalVolume *);

  // (jc2): Moved rotTubeY here to make it available to the Q1 constructor
  // functions, which are now separate from the main class constructor.
  G4RotationMatrix* rotTubeY;
};

#endif
