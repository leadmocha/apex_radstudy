#include "TestDet.hh"
#include "G4NistManager.hh"
#include "B1DetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4SystemOfUnits.hh"

TestDet::TestDet(B1DetectorConstruction *dc, G4LogicalVolume *lv){

  G4NistManager* nist = G4NistManager::Instance();
 G4Material* det_mat = nist->FindOrBuildMaterial("G4_H");
 G4Box* solidBox = new G4Box("Box",0.5*20*cm,0.5*20*cm,0.5*20*cm);
 G4LogicalVolume* logicBox = new G4LogicalVolume(solidBox,det_mat,"Box");

G4bool checkOverlaps = true;
//G4VPhysicalVolume* physBox = new G4PVPlacement(0,G4ThreeVector(),logicBox,"Box",lv, false,0,checkOverlaps);

}

TestDet::~TestDet(){;}
