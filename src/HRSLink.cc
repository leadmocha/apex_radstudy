#include "HRSLink.hh"
#include "G4NistManager.hh"
#include "B1DetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4VisAttributes.hh"

#include "G4ThreeVector.hh"

#include "HallA.hh"

#include "G4SDManager.hh"

HRSLink::HRSLink(G4double theta1, B1DetectorConstruction *dc, HallA *ha, G4LogicalVolume *logicWorld):fDetCon(dc),thetaHRS(theta1){

 //Variables of HallA class
  G4double Rhall = ha->getRhall();
  G4double bmheight = ha->getbmheight();
  G4double Tair1 = ha->getTair1();
  

  G4bool checkOverlaps = true;

 //dimentions are in inches

  const G4double conv = 2.54; //conversion from inches to cm

  G4double par[12];
  G4double X,Y,Z;
  
  G4RotationMatrix* rot_thetaHRS = new G4RotationMatrix();
  rot_thetaHRS->rotateY(-thetaHRS);

  //HRS Link Container
  
  G4double theta = atan2(23,131);
  
  G4double GLCXlength = 152*conv*cm;
  G4double GLCYlength = 15*conv*cm;
  G4double GLCZlength = 155*conv*cm;

  par[1] = 0.5*GLCXlength;
  par[2] = 0.5*GLCYlength;
  par[3] = 0.5*GLCZlength;
  
  G4Box* solidGLCbox = new G4Box("GLCbox", par[1], par[2], par[3]);
  
  G4double GLCposX = (680*cm - 0.5*GLCZlength)*sin(thetaHRS);
  G4double GLCposZ = (680*cm - 0.5*GLCZlength)*cos(thetaHRS);
  G4double GLCposY;
  
  if(thetaHRS > 0) 
  {
    GLCposY = -bmheight + 99.61*cm - 0.5*GLCYlength;
  }
  else
  {
    GLCposY = -bmheight + 99.61*cm - 0.5*GLCYlength - GLCYlength;
  }
  

  //cutaway volume 1 from Gantry Link container
  
  G4double GLCcut1Xlength = 2*131*sin(theta)*conv*cm;
  G4double GLCcut1Ylength = GLCYlength + 1*cm;
  G4double GLCcut1Zlength = 300*conv*cm;
  
  par[1] = 0.5*GLCcut1Xlength;
  par[2] = 0.5*GLCcut1Ylength;
  par[3] = 0.5*GLCcut1Zlength;
  
  G4Box* solidGLCcut1 = new G4Box("GLCcut1", par[1], par[2], par[3]);
  
  X = 0.5*GLCXlength;
  Y = 0;
  Z = 0.5*GLCZlength;
  
  G4RotationMatrix* rot10Y = new G4RotationMatrix();
  rot10Y->rotateY(10*deg);
  
  G4SubtractionSolid* solidGLCbox_cut1 = new G4SubtractionSolid("GLCbox_cut1",solidGLCbox, solidGLCcut1,rot10Y,G4ThreeVector(X,Y,Z));

  
  //cutaway volume 2 from Gantry Link container
  
  X = -0.5*GLCXlength;
  Y = 0;
  Z = 0.5*GLCZlength;
  
  G4RotationMatrix* rot10_Y = new G4RotationMatrix();
  rot10_Y->rotateY(-10*deg);
  
  G4SubtractionSolid* solidGLCbox_cut2 = new G4SubtractionSolid("GLCbox_cut2",solidGLCbox_cut1, solidGLCcut1,rot10_Y,G4ThreeVector(X,Y,Z));
  
  
  G4LogicalVolume* logicGLCbox = new G4LogicalVolume(solidGLCbox_cut2, GetMaterial("Air"), "GLCbox");
  
  G4VisAttributes* GLCboxvis = new G4VisAttributes(0);
  GLCboxvis->SetForceWireframe(true);
  GLCboxvis->SetColor(1.0,1.0,1.0);
  logicGLCbox->SetVisAttributes(GLCboxvis);
  
  G4VPhysicalVolume* physGLCbox = new G4PVPlacement(rot_thetaHRS, G4ThreeVector(GLCposX,GLCposY,GLCposZ), logicGLCbox, "GLCbox", logicWorld, false, 0, checkOverlaps);
  
  
  //Placing Steel Plates
  
  G4double PlateXlength = 0;
  G4double PlateYlength = 0;
  G4double PlateZlength = 0;
  
  //1.75" thick Plate
  
  PlateXlength = 152*conv*cm;
  PlateYlength = 15*conv*cm;
  PlateZlength = 1.75*conv*cm;
  
  par[1] = 0.5*PlateXlength;
  par[2] = 0.5*PlateYlength;
  par[3] = 0.5*PlateZlength;
  
  G4Box* solidPlate1 = new G4Box("Plate1", par[1], par[2], par[3]);
  G4LogicalVolume* logicPlate1 = new G4LogicalVolume(solidPlate1, GetMaterial("Iron"), "Plate1");
  
  X = 0;
  Y = 0;
  Z = -0.5*GLCZlength + 0.5*PlateZlength;
  
  G4VPhysicalVolume* physPlate1 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicPlate1, "Plate1", logicGLCbox, false, 0, checkOverlaps);
  
  //1.5" thick plates
  
  PlateXlength = 152*conv*cm;
  PlateYlength = 15*conv*cm;
  PlateZlength = 1.5*conv*cm;
  
  par[1] = 0.5*PlateXlength;
  par[2] = 0.5*PlateYlength;
  par[3] = 0.5*PlateZlength;
  
  G4Box* solidPlate2 = new G4Box("Plate2", par[1], par[2], par[3]);
  G4LogicalVolume* logicPlate2 = new G4LogicalVolume(solidPlate2, GetMaterial("Iron"), "Plate2");
  
  X = 0;
  Y = 0;
  Z = -0.5*GLCZlength + 24*conv*cm - 0.5*PlateZlength;
  
  G4VPhysicalVolume* physPlate2 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicPlate2, "Plate2", logicGLCbox, false, 0, checkOverlaps);
  
  PlateXlength = 1.5*conv*cm;
  PlateYlength = 15*conv*cm;
  PlateZlength = 20.75*conv*cm;
  
  par[1] = 0.5*PlateXlength;
  par[2] = 0.5*PlateYlength;
  par[3] = 0.5*PlateZlength;
  
  G4Box* solidPlate3 = new G4Box("Plate3", par[1], par[2], par[3]);
  G4LogicalVolume* logicPlate3 = new G4LogicalVolume(solidPlate3, GetMaterial("Iron"), "Plate3");
  
  X = 0.5*GLCXlength - 0.5*PlateXlength;
  Y = 0;
  Z = -0.5*GLCZlength + 12*conv*cm + 0.125*conv*cm;
  
  G4VPhysicalVolume* physPlate3_copy1 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicPlate3, "Plate3", logicGLCbox, false, 0, checkOverlaps);
  
  X = 26.5*conv*cm + 0.5*PlateXlength;
  Y = 0;
  Z = -0.5*GLCZlength + 12*conv*cm + 0.125*conv*cm;
  
  G4VPhysicalVolume* physPlate3_copy2 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicPlate3, "Plate3", logicGLCbox, false, 0, checkOverlaps);
  
  X = -26.5*conv*cm - 0.5*PlateXlength;
  Y = 0;
  Z = -0.5*GLCZlength + 12*conv*cm + 0.125*conv*cm;
  
  G4VPhysicalVolume* physPlate3_copy3 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicPlate3, "Plate3", logicGLCbox, false, 0, checkOverlaps);
  
  X = -0.5*GLCXlength + 0.5*PlateXlength;
  Y = 0;
  Z = -0.5*GLCZlength + 12*conv*cm + 0.125*conv*cm;
  
  G4VPhysicalVolume* physPlate3_copy4 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicPlate3, "Plate3", logicGLCbox, false, 0, checkOverlaps);
  
  //Center large 1.0" plate
  
  PlateXlength = 152*conv*cm;
  PlateYlength = 1.0*conv*cm;
  PlateZlength = 131*conv*cm;
  
  par[1] = 0.5*PlateXlength;
  par[2] = 0.5*PlateYlength;
  par[3] = 0.5*PlateZlength;
  
  G4Box* solidPlate4 = new G4Box("Plate4", par[1], par[2], par[3]);
  
  //cutaway 1
  
  X = 0.5*PlateXlength;
  Y = 0;
  Z = 0.5*PlateZlength;
  
  G4SubtractionSolid* solidPlate4_cut1 = new G4SubtractionSolid("Plate4_cut1",solidPlate4, solidGLCcut1,rot10Y,G4ThreeVector(X,Y,Z));
  
  X = -0.5*PlateXlength;
  Y = 0;
  Z = 0.5*PlateZlength;
  
  G4SubtractionSolid* solidPlate4_cut2 = new G4SubtractionSolid("Plate4_cut2",solidPlate4_cut1, solidGLCcut1,rot10_Y,G4ThreeVector(X,Y,Z));
  
  G4LogicalVolume* logicPlate4 = new G4LogicalVolume(solidPlate4_cut2, GetMaterial("Iron"), "Plate4");
  
  X = 0;
  Y = 0;
  Z = 0.5*GLCZlength - 0.5*PlateZlength;
  
  G4VPhysicalVolume* physPlate4 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicPlate4, "Plate4", logicGLCbox, false, 0, checkOverlaps);
  
  
  //Angle plates 1.5" thick
  
  PlateXlength = 1.5*conv*cm;
  PlateYlength = 7*conv*cm;
  PlateZlength = 130*conv*cm;
  
  par[1] = 0.5*PlateXlength;
  par[2] = 0.5*PlateYlength;
  par[3] = 0.5*PlateZlength;
  
  G4Box* solidPlate5 = new G4Box("Plate5", par[1], par[2], par[3]);
  G4LogicalVolume* logicPlate5 = new G4LogicalVolume(solidPlate5, GetMaterial("Iron"), "Plate5");
  
  X = 0.5*GLCXlength - 11.5*conv*cm - 0.5*PlateXlength;
  Y = 0.5*GLCYlength - 0.5*PlateYlength;
  Z = 0.5*GLCZlength - 0.5*PlateZlength - 1.5*conv*cm;
  
  G4VPhysicalVolume* physPlate5_copy1 = new G4PVPlacement(rot10Y, G4ThreeVector(X,Y,Z), logicPlate5, "Plate5", logicGLCbox, false, 0, checkOverlaps);
  
  X = 26.5*conv*cm + 11.5*conv*cm + 0.5*PlateXlength;
  Y = 0.5*GLCYlength - 0.5*PlateYlength;
  Z = 0.5*GLCZlength - 0.5*PlateZlength - 1.5*conv*cm;
  
  G4VPhysicalVolume* physPlate5_copy2 = new G4PVPlacement(rot10_Y, G4ThreeVector(X,Y,Z), logicPlate5, "Plate5", logicGLCbox, false, 0, checkOverlaps);
  
  X = -0.5*GLCXlength + 11.5*conv*cm + 0.5*PlateXlength;
  Y = 0.5*GLCYlength - 0.5*PlateYlength;
  Z = 0.5*GLCZlength - 0.5*PlateZlength - 1.5*conv*cm;
  
  G4VPhysicalVolume* physPlate5_copy3 = new G4PVPlacement(rot10_Y, G4ThreeVector(X,Y,Z), logicPlate5, "Plate5", logicGLCbox, false, 0, checkOverlaps);
  
  X = -26.5*conv*cm - 11.5*conv*cm - 0.5*PlateXlength;
  Y = 0.5*GLCYlength - 0.5*PlateYlength;
  Z = 0.5*GLCZlength - 0.5*PlateZlength - 1.5*conv*cm;
  
  G4VPhysicalVolume* physPlate5_copy4 = new G4PVPlacement(rot10Y, G4ThreeVector(X,Y,Z), logicPlate5, "Plate5", logicGLCbox, false, 0, checkOverlaps);
  
  X = 0.5*GLCXlength - 11.5*conv*cm - 0.5*PlateXlength;
  Y = -0.5*GLCYlength + 0.5*PlateYlength;
  Z = 0.5*GLCZlength - 0.5*PlateZlength - 1.5*conv*cm;
  
  G4VPhysicalVolume* physPlate5_copy5 = new G4PVPlacement(rot10Y, G4ThreeVector(X,Y,Z), logicPlate5, "Plate5", logicGLCbox, false, 0, checkOverlaps);
  
  X = 26.5*conv*cm + 11.5*conv*cm + 0.5*PlateXlength;
  Y = -0.5*GLCYlength + 0.5*PlateYlength;
  Z = 0.5*GLCZlength - 0.5*PlateZlength - 1.5*conv*cm;
  
  G4VPhysicalVolume* physPlate5_copy6 = new G4PVPlacement(rot10_Y, G4ThreeVector(X,Y,Z), logicPlate5, "Plate5", logicGLCbox, false, 0, checkOverlaps);
  
  X = -0.5*GLCXlength + 11.5*conv*cm + 0.5*PlateXlength;
  Y = -0.5*GLCYlength + 0.5*PlateYlength;
  Z = 0.5*GLCZlength - 0.5*PlateZlength - 1.5*conv*cm;
  
  G4VPhysicalVolume* physPlate5_copy7 = new G4PVPlacement(rot10_Y, G4ThreeVector(X,Y,Z), logicPlate5, "Plate5", logicGLCbox, false, 0, checkOverlaps);
  
  X = -26.5*conv*cm - 11.5*conv*cm - 0.5*PlateXlength;
  Y = -0.5*GLCYlength + 0.5*PlateYlength;
  Z = 0.5*GLCZlength - 0.5*PlateZlength - 1.5*conv*cm;
  
  G4VPhysicalVolume* physPlate5_copy8 = new G4PVPlacement(rot10Y, G4ThreeVector(X,Y,Z), logicPlate5, "Plate5", logicGLCbox, false, 0, checkOverlaps);
  
  
  //Back plate
  
  PlateXlength = 105*conv*cm;
  PlateYlength = 7*conv*cm;
  PlateZlength = 1.5*conv*cm;
  
  par[1] = 0.5*PlateXlength;
  par[2] = 0.5*PlateYlength;
  par[3] = 0.5*PlateZlength;
  
  G4Box* solidPlate6 = new G4Box("Plate6", par[1], par[2], par[3]);
  G4LogicalVolume* logicPlate6 = new G4LogicalVolume(solidPlate6, GetMaterial("Iron"), "Plate6");
  
  X = 0;
  Y = 0.5*GLCYlength - 0.5*PlateYlength;
  Z = 0.5*GLCZlength - 0.5*PlateZlength;
  
  G4VPhysicalVolume* physPlate6_copy1 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicPlate6, "Plate6", logicGLCbox, false, 0, checkOverlaps);
  
  X = 0;
  Y = -0.5*GLCYlength + 0.5*PlateYlength;
  Z = 0.5*GLCZlength - 0.5*PlateZlength;
  
  G4VPhysicalVolume* physPlate6_copy2 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicPlate6, "Plate6", logicGLCbox, false, 0, checkOverlaps);
  
  
  //Another 1" plate at center
  
  PlateXlength = 25.5*conv*cm;
  PlateYlength = 7*conv*cm;
  PlateZlength = 1.0*conv*cm;
  
  par[1] = 0.5*PlateXlength;
  par[2] = 0.5*PlateYlength;
  par[3] = 0.5*PlateZlength;
  
  G4Box* solidPlate7 = new G4Box("Plate7", par[1], par[2], par[3]);
  G4LogicalVolume* logicPlate7 = new G4LogicalVolume(solidPlate7, GetMaterial("Iron"), "Plate7");
  
  X = 51.25*conv*cm;
  Y = 0.5*GLCYlength - 0.5*PlateYlength;
  Z = -0.5*GLCZlength + 81*conv*cm;
  
  G4VPhysicalVolume* physPlate7_copy1 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicPlate7, "Plate7", logicGLCbox, false, 0, checkOverlaps);
  
  X = -51.25*conv*cm;
  Y = 0.5*GLCYlength - 0.5*PlateYlength;
  Z = -0.5*GLCZlength + 81*conv*cm;
  
  G4VPhysicalVolume* physPlate7_copy2 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicPlate7, "Plate7", logicGLCbox, false, 0, checkOverlaps);
  
  X = 51.25*conv*cm;
  Y = -0.5*GLCYlength + 0.5*PlateYlength;
  Z = -0.5*GLCZlength + 81*conv*cm;
  
  G4VPhysicalVolume* physPlate7_copy3 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicPlate7, "Plate7", logicGLCbox, false, 0, checkOverlaps);
  
  X = -51.25*conv*cm;
  Y = -0.5*GLCYlength + 0.5*PlateYlength;
  Z = -0.5*GLCZlength + 81*conv*cm;
  
  G4VPhysicalVolume* physPlate7_copy4 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicPlate7, "Plate7", logicGLCbox, false, 0, checkOverlaps);
  
  
  //Another 1" plate towards back
  
  PlateXlength = 9.3*conv*cm;
  PlateYlength = 7*conv*cm;
  PlateZlength = 1.0*conv*cm;
  
  par[1] = 0.5*PlateXlength;
  par[2] = 0.5*PlateYlength;
  par[3] = 0.5*PlateZlength;
  
  G4Box* solidPlate8 = new G4Box("Plate8", par[1], par[2], par[3]);
  G4LogicalVolume* logicPlate8 = new G4LogicalVolume(solidPlate8, GetMaterial("Iron"), "Plate8");
  
  X = 51.25*conv*cm;
  Y = 0.5*GLCYlength - 0.5*PlateYlength;
  Z = 0.5*GLCZlength - 27*conv*cm;
  
  G4VPhysicalVolume* physPlate8_copy1 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicPlate8, "Plate8", logicGLCbox, false, 0, checkOverlaps);
  
  X = -51.25*conv*cm;
  Y = 0.5*GLCYlength - 0.5*PlateYlength;
  Z = 0.5*GLCZlength - 27*conv*cm;
  
  G4VPhysicalVolume* physPlate8_copy2 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicPlate8, "Plate8", logicGLCbox, false, 0, checkOverlaps);
  
  X = 51.25*conv*cm;
  Y = -0.5*GLCYlength + 0.5*PlateYlength;
  Z = 0.5*GLCZlength - 27*conv*cm;
  
  G4VPhysicalVolume* physPlate8_copy3 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicPlate8, "Plate8", logicGLCbox, false, 0, checkOverlaps);
  
  X = -51.25*conv*cm;
  Y = -0.5*GLCYlength + 0.5*PlateYlength;
  Z = 0.5*GLCZlength - 27*conv*cm;
  
  G4VPhysicalVolume* physPlate8_copy4 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicPlate8, "Plate8", logicGLCbox, false, 0, checkOverlaps);
  
  //1" Plate
  
  PlateXlength = 89*conv*cm;
  PlateYlength = 7*conv*cm;
  PlateZlength = 1.0*conv*cm;
  
  par[1] = 0.5*PlateXlength;
  par[2] = 0.5*PlateYlength;
  par[3] = 0.5*PlateZlength;
  
  G4Box* solidPlate9 = new G4Box("Plate9", par[1], par[2], par[3]);
  G4LogicalVolume* logicPlate9 = new G4LogicalVolume(solidPlate9, GetMaterial("Iron"), "Plate9");
  
  X = 0;
  Y = 0.5*GLCYlength - 0.5*PlateYlength;
  Z = 0.5*GLCZlength - 27*conv*cm;
  
  G4VPhysicalVolume* physPlate9_copy1 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicPlate9, "Plate9", logicGLCbox, false, 0, checkOverlaps);
  
  X = 0;
  Y = -0.5*GLCYlength + 0.5*PlateYlength;
  Z = 0.5*GLCZlength - 27*conv*cm;
  
  G4VPhysicalVolume* physPlate9_copy2 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicPlate9, "Plate9", logicGLCbox, false, 0, checkOverlaps);
  
  
  //0.75" Plates at the front
  
  PlateXlength = 0.75*conv*cm;
  PlateYlength = 15*conv*cm;
  PlateZlength = 20.75*conv*cm;
  
  par[1] = 0.5*PlateXlength;
  par[2] = 0.5*PlateYlength;
  par[3] = 0.5*PlateZlength;
  
  G4Box* solidPlate10 = new G4Box("Plate10", par[1], par[2], par[3]);
  G4LogicalVolume* logicPlate10 = new G4LogicalVolume(solidPlate10, GetMaterial("Iron"), "Plate10");
  
  G4double PlateLocation[12] = {34.375, 41.125, 47.875, 54.625, 61.375, 68.125, -34.375, -41.125, -47.875, -54.625, -61.375, -68.125};
  
  for(G4int i=0; i<12; i++)
  {
    
  X = PlateLocation[i]*conv*cm;
  Y = 0;
  Z = -0.5*GLCZlength + 12.125*conv*cm;
  
  G4VPhysicalVolume* physPlate10 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicPlate10, "Plate10", logicGLCbox, false, 0, checkOverlaps);
    
  }
  

  

}



G4Material* HRSLink::GetMaterial(G4String mat){

  return fDetCon->GetMaterial(mat);

}


HRSLink::~HRSLink(){;}
