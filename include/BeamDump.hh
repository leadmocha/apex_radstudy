#ifndef BeamDump_hh
#define BeamDump_hh

#include "G4Material.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

class G4LogicalVolume;
class B1DetectorConstruction;
class HallA;

class BeamDump{
public:

  BeamDump(B1DetectorConstruction *, HallA *);
  ~BeamDump();
  void ConstructBeamDumpOrig(G4LogicalVolume*);
  G4LogicalVolume* ConstructBeamDumpUpdated();
  G4LogicalVolume* ConstructBeamDump(){
    // Calling this method returns only the updated beam dump geometry
    // (for the originally coded version call ConstructBeamDumpOrig() )
    return ConstructBeamDumpUpdated();
  };

  G4double getBmDEX(){return BmDEX;}
  G4double getBmDEY(){return BmDEY;}
  G4double getBmDEZ(){return BmDEZ;}
  G4double getBmDmX(){return BmDmX;}
  G4double getBmDmY(){return BmDmY;}
  G4double getBmDmZ(){return BmDmZ;}
  G4double getBDWth(){return BDWth;}
  G4double getBDWthz(){return BDWthz;}
  G4double getTgX(){return TgX;}


  G4ThreeVector getBeamDumpWorldOrigin() { return fBDWorldOrigin; };
  // The local origin is measured from:
  // x,y: is beam center to the x,y center of the beam dump coordinates
  // z: the inside face of the Hall Wall to the z center of the beam dump
  // coordinates
  G4ThreeVector getLocalOrigin() { return fBDLocalOrigin; };
  G4ThreeVector getWorldDimensions() { return fBDWorldDimensions; };

protected:

  B1DetectorConstruction* fDetCon;
  HallA *fHallA;
  G4Material* GetMaterial(G4String);
  G4LogicalVolume *fVolBeamDumpWorld;

private:

  G4double BPAirTubeLength;
  G4double BmDEX;   // Beam Dump Tunnel X
  G4double BmDEY;   // Beam Dump Tunnel Y
  G4double BmDEZ;   // Beam Dump Tunnel Z
  G4double BmDmX;   // Beam Dump Enclosure X (back of beam dump)
  G4double BmDmY;   // Beam Dump Enclosure Y (back of beam dump)
  G4double BmDmZ;   // Beam Dump Enclosure Z (back of beam dump)
  G4double BDWth;   // ????
  G4double BDWthz;  // ????
  G4double TgX;     // ????
  G4double BDWLZ;    // Water Barrel length in Z

  G4ThreeVector fBDWorldOrigin;
  G4ThreeVector fBDLocalOrigin;
  G4ThreeVector fBDWorldDimensions;

  void BuildWaterBarrel(G4LogicalVolume *logicWorld,
      G4RotationMatrix *rot, G4ThreeVector pos);

};

#endif
