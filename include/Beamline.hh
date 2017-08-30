#ifndef Beamline_hh
#define Beamline_hh

#include "G4Material.hh"
#include "HallA.hh"

class G4LogicalVolume;
class B1DetectorConstruction;

class Beamline{
public:

  Beamline(B1DetectorConstruction *, HallA *, G4LogicalVolume *);
  ~Beamline();

protected:

  B1DetectorConstruction* fDetCon;
  G4Material* GetMaterial(G4String);

private:

  G4double BPAirTubeLength;

};

#endif
