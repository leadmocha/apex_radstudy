#ifndef HallA_hh
#define HallA_hh

#include "G4Material.hh"

class G4LogicalVolume;
class B1DetectorConstruction;

class HallA{
public:

  HallA(B1DetectorConstruction *,G4LogicalVolume *);
  ~HallA();

  G4double getRhall(){return Rhall;}
  G4double getbmheight(){return bmheight;}
  G4double getTair1(){return Tair1;}
  G4double getHBdiam(){return HBdiam;}
  G4double getHBlength(){return HBlength;}
  G4double getSCZ(){return SCZ;}
  G4double getBmDEX(){return BmDEX;}
  G4double getBmDEY(){return BmDEY;}
  G4double getBmDEZ(){return BmDEZ;}
  G4double getBmDmY(){return BmDmY;}
  G4double getHB5AYpos(){return 0.5*(BmDEY-BmDmY);}
  G4double getBPAirTubeLength(){return BPAirTubeLength;}

protected:

  B1DetectorConstruction* fDetCon;
  G4Material* GetMaterial(G4String);

private: 

  void MakeCarbonCrosscheckTarget(G4LogicalVolume*);
  void MakeApexQ1(G4LogicalVolume*);

  G4double Rhall, bmheight, Tair1, HBdiam, HBlength, SCZ, BmDEX, BmDEY, BmDEZ, BmDmY, BPAirTubeLength;

};

#endif
