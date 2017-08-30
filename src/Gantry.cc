#include "Gantry.hh"
#include "GenericSD.hh"
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
#include "HRSDetSDL.hh"
#include "HRSDetSDR.hh"

#include "G4SDManager.hh"

Gantry::Gantry(G4double theta, B1DetectorConstruction *dc, HallA *ha, G4LogicalVolume *logicWorld):fDetCon(dc),thetaHRS(theta){

 //Variables of HallA class
  G4double Rhall = ha->getRhall();
  G4double bmheight = ha->getbmheight();
  G4double Tair1 = ha->getTair1();
  

  G4bool checkOverlaps = true;

 //dimentions are in inches

  const G4double conv = 2.54; //conversion from inches to cm

  G4double par[12];
  G4double X,Y,Z;

  
  //HRS Gantry container
  
  G4double GCXlength = 266*conv*cm;
  G4double GCYlength = 229*conv*cm;
  G4double GCZlength = 320*conv*cm;

  par[1] = 0.5*GCXlength;
  par[2] = 0.5*GCYlength;
  par[3] = 0.5*GCZlength;
  
  G4Box* solidGCbox = new G4Box("GCbox", par[1], par[2], par[3]);
  
  G4double GCposX = (690*conv*cm + 0.5*GCZlength)*sin(thetaHRS);
  G4double GCposY = -bmheight + 99.61*cm + 58*conv*cm - (156 - 81)*conv*cm + 0.5*GCYlength;
  G4double GCposZ = (690*conv*cm + 0.5*GCZlength)*cos(thetaHRS);
  
  //cutaway volume 1 from Gantry container
  
  G4double GCcut1Xlength = 266*conv*cm + 1*cm;
  G4double GCcut1Ylength = 75*conv*cm + 1*cm;
  G4double GCcut1Zlength = 72.625*conv*cm + 1*cm;
  
  par[1] = 0.5*GCcut1Xlength;
  par[2] = 0.5*GCcut1Ylength;
  par[3] = 0.5*GCcut1Zlength;
  
  G4Box* solidGCcut1 = new G4Box("GCcut1", par[1], par[2], par[3]);
  
  X = 0;
  Y = - 0.5*GCYlength + 0.5*GCcut1Ylength - 1*cm;
  Z = - 0.5*GCZlength + 0.5*GCcut1Zlength - 1*cm;
  
  G4SubtractionSolid* solidGCbox_cut1 = new G4SubtractionSolid("GCbox_cut1",solidGCbox, solidGCcut1,0,G4ThreeVector(X,Y,Z));
  
  //cutaway volume 2 from Gantry container
  
  G4double GCcut2Xlength = 120*conv*cm;
  G4double GCcut2Ylength = 229*conv*cm + 1*cm;
  G4double GCcut2Zlength = 110*conv*cm + 1*cm;
  
  par[1] = 0.5*GCcut2Xlength;
  par[2] = 0.5*GCcut2Ylength;
  par[3] = 0.5*GCcut2Zlength;
  
  G4Box* solidGCcut2 = new G4Box("GCcut2", par[1], par[2], par[3]);
  
  X = 0;
  Y = 0;
  Z = - 0.5*GCZlength + 0.5*GCcut2Zlength - 1*cm;
  
  G4SubtractionSolid* solidGCbox_cut2 = new G4SubtractionSolid("GCbox_cut2",solidGCbox_cut1, solidGCcut2,0,G4ThreeVector(X,Y,Z));
  
  
  G4LogicalVolume* logicGCbox = new G4LogicalVolume(solidGCbox_cut2, GetMaterial("Air"), "GCbox");
  
  G4VisAttributes* GCboxvis = new G4VisAttributes(0);
  GCboxvis->SetForceWireframe(false);
  GCboxvis->SetColor(1.0,1.0,1.0);
  logicGCbox->SetVisAttributes(GCboxvis);
  
  G4RotationMatrix* rot_thetaHRS = new G4RotationMatrix();
  rot_thetaHRS->rotateY(-thetaHRS);
  
  G4VPhysicalVolume* physGCbox = new G4PVPlacement(rot_thetaHRS, G4ThreeVector(GCposX,GCposY,GCposZ), logicGCbox, "GCbox", logicWorld, false, 0, checkOverlaps);
  
  
  
  //Gantry top layer
  
  G4double GTXlength = 266*conv*cm;
  G4double GTYlength = 73*conv*cm;
  G4double GTZlength = 320*conv*cm;

  par[1] = 0.5*GTXlength;
  par[2] = 0.5*GTYlength;
  par[3] = 0.5*GTZlength;
  
  G4Box* solidGTbox = new G4Box("GTbox", par[1], par[2], par[3]);
  
  G4double GTposX = 0;
  G4double GTposY = 0.5*GCYlength - 0.5*GTYlength;
  G4double GTposZ = 0;
  
  //cutaway volume from Gantry Top part
  
  G4double cutXlength = 120*conv*cm;
  G4double cutYlength = 73*conv*cm + 1*cm;
  G4double cutZlength = 110*conv*cm + 1*cm;
  
  par[1] = 0.5*cutXlength;
  par[2] = 0.5*cutYlength;
  par[3] = 0.5*cutZlength;
  
  G4Box* solidGTcut = new G4Box("GTcut", par[1], par[2], par[3]);
  
  X = 0;
  Y = 0;
  Z = -0.5*GTZlength + 0.5*cutZlength -1*cm;
  
  G4SubtractionSolid* solidGTbox_cut = new G4SubtractionSolid("GTbox_cut",solidGTbox, solidGTcut,0,G4ThreeVector(X,Y,Z));
  
  G4LogicalVolume* logicGTbox = new G4LogicalVolume(solidGTbox_cut, GetMaterial("Iron"), "GTbox_cut");
  
  G4VisAttributes* GTboxvis = new G4VisAttributes(1);
  GTboxvis->SetForceWireframe(false);
  GTboxvis->SetColor(1.0,1.0,1.0);
  logicGTbox->SetVisAttributes(GTboxvis);
  
  G4VPhysicalVolume* physGTbox = new G4PVPlacement(0, G4ThreeVector(GTposX,GTposY,GTposZ), logicGTbox, "GTbox", logicGCbox, false, 0, checkOverlaps);
  
  //Air volume inside the gantry top part
  //leaving 1.0" and 1.5" wall around gantry top.
  
  G4double GTAXlength = 264*conv*cm;
  G4double GTAYlength = 70*conv*cm;
  G4double GTAZlength = 318*conv*cm;

  par[1] = 0.5*GTAXlength;
  par[2] = 0.5*GTAYlength;
  par[3] = 0.5*GTAZlength;
  
  G4Box* solidGTAbox = new G4Box("GTAbox", par[1], par[2], par[3]);
  
  G4double GTAposX = 0;
  G4double GTAposY = 0;
  G4double GTAposZ = 0;
  
  //cutaway volume from Gantry Top Air part
  
  G4double cutAXlength = 122*conv*cm;
  G4double cutAYlength = 70*conv*cm + 1*cm;
  G4double cutAZlength = 110*conv*cm + 1*cm;
  
  par[1] = 0.5*cutAXlength;
  par[2] = 0.5*cutAYlength;
  par[3] = 0.5*cutAZlength;
  
  G4Box* solidGTAcut = new G4Box("GTAcut", par[1], par[2], par[3]);
  
  X = 0;
  Y = 0;
  Z = -0.5*GTAZlength + 0.5*cutAZlength -1*cm;
  
  G4SubtractionSolid* solidGTAbox_cut = new G4SubtractionSolid("GTAbox_cut",solidGTAbox, solidGTAcut,0,G4ThreeVector(X,Y,Z));
  
  G4LogicalVolume* logicGTAbox = new G4LogicalVolume(solidGTAbox_cut, GetMaterial("Air"), "GTAbox_cut");
  
  G4VisAttributes* GTAboxvis = new G4VisAttributes(1);
  GTAboxvis->SetForceWireframe(false);
  GTAboxvis->SetColor(1.0,1.0,1.0);
  logicGTAbox->SetVisAttributes(GTAboxvis);
  
  G4VPhysicalVolume* physGTAbox = new G4PVPlacement(0, G4ThreeVector(GTAposX,GTAposY,GTAposZ), logicGTAbox, "GTAbox", logicGTbox, false, 0, checkOverlaps);
  
  
  //Place steel plates inside Air volume
  
  //First 4 plates in x direction
  
  const G4int nxx = 5;
  
  G4double XPlateLength[nxx] = {71, 71, 264, 264, 264};
  G4double XPlateXpos[nxx] = {96.5, -96.5, 0, 0, 0};
  G4double XPlateZpos[nxx] = {66.5, 66.5, 113.5, 182, 250};
  
  G4String XPlateName[nxx] = {"XPlate1", "XPlate2", "XPlate3", "XPlate4", "XPlate5"};
  
  for(G4int i=0; i<nxx; i++)
  {
    par[1] = 0.5*XPlateLength[i]*conv*cm;
    par[2] = 0.5*70*conv*cm;
    par[3] = 0.5*1.0*conv*cm;
    
    G4Box* solidXPlate = new G4Box("XPlate", par[1], par[2], par[3]);
    
    G4LogicalVolume* logicXPlate = new G4LogicalVolume(solidXPlate, GetMaterial("Iron"), "XPlate");
    
    X = GTAposX + XPlateXpos[i]*conv*cm;
    Y = GTAposY;
    Z = GTAposZ - 0.5*GTAZlength + XPlateZpos[i]*conv*cm;
    
    G4VPhysicalVolume* physXPlate = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicXPlate, XPlateName[i], logicGTAbox, false, 0, checkOverlaps);
    
  }
  
  //Other plates in z direction
  
  const G4int nzz = 25;
  
  G4double ZPlateLength[nzz] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  G4double ZPlateXpos[nzz] = {0, 21, -21, 60.375, -60.375, 96, -96, 0, 21, -21, 60.375, -60.375, 96, -96, 0, 21, -21, 60.375, -60.375, 96, -96, 96, -96, 96, -96};
  G4double ZPlateZpos[nzz] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  
  ZPlateLength[0] = XPlateZpos[3] - XPlateZpos[2] - 1;
  ZPlateZpos[0] = XPlateZpos[2] + 0.5*ZPlateLength[0] + 0.5;
  
  ZPlateLength[1] = XPlateZpos[3] - XPlateZpos[2] - 1;
  ZPlateZpos[1] = XPlateZpos[2] + 0.5*ZPlateLength[1] + 0.5;
  ZPlateXpos[1] = ZPlateXpos[1];
  
  ZPlateLength[2] = XPlateZpos[3] - XPlateZpos[2] - 1;
  ZPlateZpos[2] = XPlateZpos[2] + 0.5*ZPlateLength[2] + 0.5;
  ZPlateXpos[2] = ZPlateXpos[2];
  
  ZPlateLength[3] = XPlateZpos[3] - XPlateZpos[2] - 1;
  ZPlateZpos[3] = XPlateZpos[2] + 0.5*ZPlateLength[3] + 0.5;
  ZPlateXpos[3] = ZPlateXpos[3];
  
  ZPlateLength[4] = XPlateZpos[3] - XPlateZpos[2] - 1;
  ZPlateZpos[4] = XPlateZpos[2] + 0.5*ZPlateLength[4] + 0.5;
  ZPlateXpos[4] = ZPlateXpos[4];
  
  ZPlateLength[5] = XPlateZpos[3] - XPlateZpos[2] - 1;
  ZPlateZpos[5] = XPlateZpos[2] + 0.5*ZPlateLength[5] + 0.5;
  ZPlateXpos[5] = ZPlateXpos[5];
  
  ZPlateLength[6] = XPlateZpos[3] - XPlateZpos[2] - 1;
  ZPlateZpos[6] = XPlateZpos[2] + 0.5*ZPlateLength[6] + 0.5;
  ZPlateXpos[6] = ZPlateXpos[6];
  
  ZPlateLength[7] = XPlateZpos[4] - XPlateZpos[3] - 1;
  ZPlateZpos[7] = XPlateZpos[3] + 0.5*ZPlateLength[7] + 0.5;
  ZPlateXpos[7] = ZPlateXpos[7];
  
  ZPlateLength[8] = XPlateZpos[4] - XPlateZpos[3] - 1;
  ZPlateZpos[8] = XPlateZpos[3] + 0.5*ZPlateLength[8] + 0.5;
  ZPlateXpos[8] = ZPlateXpos[8];
  
  ZPlateLength[9] = XPlateZpos[4] - XPlateZpos[3] - 1;
  ZPlateZpos[9] = XPlateZpos[3] + 0.5*ZPlateLength[9] + 0.5;
  ZPlateXpos[9] = ZPlateXpos[9];
  
  ZPlateLength[10] = XPlateZpos[4] - XPlateZpos[3] - 1;
  ZPlateZpos[10] = XPlateZpos[3] + 0.5*ZPlateLength[10] + 0.5;
  ZPlateXpos[10] = ZPlateXpos[10];
  
  ZPlateLength[11] = XPlateZpos[4] - XPlateZpos[3] - 1;
  ZPlateZpos[11] = XPlateZpos[3] + 0.5*ZPlateLength[11] + 0.5;
  ZPlateXpos[11] = ZPlateXpos[11];
  
  ZPlateLength[12] = XPlateZpos[4] - XPlateZpos[3] - 1;
  ZPlateZpos[12] = XPlateZpos[3] + 0.5*ZPlateLength[12] + 0.5;
  ZPlateXpos[12] = ZPlateXpos[12];
  
  ZPlateLength[13] = XPlateZpos[4] - XPlateZpos[3] - 1;
  ZPlateZpos[13] = XPlateZpos[3] + 0.5*ZPlateLength[13] + 0.5;
  ZPlateXpos[13] = ZPlateXpos[13];
  
  ZPlateLength[14] = 318 - XPlateZpos[4] - 0.5;
  ZPlateZpos[14] = XPlateZpos[4] + 0.5*ZPlateLength[14] + 0.5;
  ZPlateXpos[14] = ZPlateXpos[14];
  
  ZPlateLength[15] = 318 - XPlateZpos[4] - 0.5;
  ZPlateZpos[15] = XPlateZpos[4] + 0.5*ZPlateLength[15] + 0.5;
  ZPlateXpos[15] = ZPlateXpos[15];
  
  ZPlateLength[16] = 318 - XPlateZpos[4] - 0.5;
  ZPlateZpos[16] = XPlateZpos[4] + 0.5*ZPlateLength[16] + 0.5;
  ZPlateXpos[16] = ZPlateXpos[16];
  
  ZPlateLength[17] = 318 - XPlateZpos[4] - 0.5;
  ZPlateZpos[17] = XPlateZpos[4] + 0.5*ZPlateLength[17] + 0.5;
  ZPlateXpos[17] = ZPlateXpos[17];
  
  ZPlateLength[18] = 318 - XPlateZpos[4] - 0.5;
  ZPlateZpos[18] = XPlateZpos[4] + 0.5*ZPlateLength[18] + 0.5;
  ZPlateXpos[18] = ZPlateXpos[18];
  
  ZPlateLength[19] = 318 - XPlateZpos[4] - 0.5;
  ZPlateZpos[19] = XPlateZpos[4] + 0.5*ZPlateLength[19] + 0.5;
  ZPlateXpos[19] = ZPlateXpos[19];
  
  ZPlateLength[20] = 318 - XPlateZpos[4] - 0.5;
  ZPlateZpos[20] = XPlateZpos[4] + 0.5*ZPlateLength[20] + 0.5;
  ZPlateXpos[20] = ZPlateXpos[20];
  
  ZPlateLength[21] = XPlateZpos[0] - 0.5;
  ZPlateZpos[21] = 0.5*ZPlateLength[21];
  ZPlateXpos[21] = ZPlateXpos[21];
  
  ZPlateLength[22] = XPlateZpos[0] - 0.5;
  ZPlateZpos[22] = 0.5*ZPlateLength[22];
  ZPlateXpos[22] = ZPlateXpos[22];
  
  ZPlateLength[23] = XPlateZpos[2] - XPlateZpos[1] - 1;
  ZPlateZpos[23] = XPlateZpos[0] + 0.5*ZPlateLength[23] + 0.5;
  ZPlateXpos[23] = ZPlateXpos[23];
  
  ZPlateLength[24] = XPlateZpos[2] - XPlateZpos[1] - 1;
  ZPlateZpos[24] = XPlateZpos[0] + 0.5*ZPlateLength[24] + 0.5;
  ZPlateXpos[24] = ZPlateXpos[24];
  
  G4String ZPlateName[nzz] = {"ZPlate1", "ZPlate2", "ZPlate3", "ZPlate4", "ZPlate5", "ZPlate6", "ZPlate7", "ZPlate8", "ZPlate9", "ZPlate10", "ZPlate11", "ZPlate12", "ZPlate13", "ZPlate14", "ZPlate15", "ZPlate16", "ZPlate17", "ZPlate18", "ZPlate19", "ZPlate20", "ZPlate21", "ZPlate22", "ZPlate23", "ZPlate24", "ZPlate25"};
  
  for(G4int i=0; i<nzz; i++)
  {
    par[1] = 0.5*1*conv*cm;
    par[2] = 0.5*70*conv*cm;
    par[3] = 0.5*ZPlateLength[i]*conv*cm;
    
    G4Box* solidZPlate = new G4Box("ZPlate", par[1], par[2], par[3]);
    
    G4LogicalVolume* logicZPlate = new G4LogicalVolume(solidZPlate, GetMaterial("Iron"), "ZPlate");
    
    X = GTAposX + ZPlateXpos[i]*conv*cm;
    Y = GTAposY;
    Z = GTAposZ - 0.5*GTAZlength + ZPlateZpos[i]*conv*cm;
    
    G4VPhysicalVolume* physZPlate = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicZPlate, ZPlateName[i], logicGTAbox, false, 0, checkOverlaps);
    
  }
  
  
  //HRS Gantry back leg
  
  G4double GBLIXlength = 40*conv*cm;
  G4double GBLIYlength = 156*conv*cm;
  G4double GBLIZlength = 70*conv*cm;
  
  par[1] = 0.5*GBLIXlength;
  par[2] = 0.5*GBLIYlength;
  par[3] = 0.5*GBLIZlength;

  G4Box* solidGBLI = new G4Box("GBLI", par[1], par[2], par[3]);
  
  G4LogicalVolume* logicGBLI = new G4LogicalVolume(solidGBLI, GetMaterial("Iron"), "GBLI");
  
  G4VisAttributes* GBLIvis = new G4VisAttributes(1);
  GBLIvis->SetForceWireframe(false);
  GBLIvis->SetColor(1.0,1.0,1.0);
  logicGBLI->SetVisAttributes(GBLIvis);
  
  X = -0.5*GCXlength + 0.5*GBLIXlength;
  Y = -0.5*GCYlength + 0.5*GBLIYlength;
  Z = 0.5*GCZlength - 0.5*GBLIZlength;
  
  G4VPhysicalVolume* physGBLI1 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicGBLI, "GBLI1", logicGCbox, false, 0, checkOverlaps);
  
  X = 0.5*GCXlength - 0.5*GBLIXlength;
  Y = -0.5*GCYlength + 0.5*GBLIYlength;
  Z = 0.5*GCZlength - 0.5*GBLIZlength;
  
  G4VPhysicalVolume* physGBLI2 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicGBLI, "GBLI2", logicGCbox, false, 0, checkOverlaps);
  
  
  
  //Air inside HRS Gantry back leg
  
  G4double GBLAXlength = 38*conv*cm;
  G4double GBLAYlength = 153*conv*cm;
  G4double GBLAZlength = 68*conv*cm;
  
  par[1] = 0.5*GBLAXlength;
  par[2] = 0.5*GBLAYlength;
  par[3] = 0.5*GBLAZlength;

  G4Box* solidGBLA = new G4Box("GBLA", par[1], par[2], par[3]);
  
  G4LogicalVolume* logicGBLA = new G4LogicalVolume(solidGBLA, GetMaterial("Air"), "GBLA");
  
  X = 0;
  Y = 1.5*conv*cm;
  Z = 0;
  
  G4VisAttributes* GBLAvis = new G4VisAttributes(1);
  GBLAvis->SetForceWireframe(false);
  GBLAvis->SetColor(1.0,1.0,1.0);
  logicGBLA->SetVisAttributes(GBLAvis);
  
  G4VPhysicalVolume* physGBLA = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicGBLA, "GBLA", logicGBLI, false, 0, checkOverlaps);
  
  //Plates in Gantry leg
  
  G4double GBLPlateXlength = GBLAXlength;
  G4double GBLPlateYlength = 1.0*conv*cm;
  G4double GBLPlateZlength = GBLAZlength;
  
  par[1] = 0.5*GBLPlateXlength;
  par[2] = 0.5*GBLPlateYlength;
  par[3] = 0.5*GBLPlateZlength;

  G4Box* solidGBLPlate = new G4Box("GBLPlate", par[1], par[2], par[3]);
  
  G4LogicalVolume* logicGBLPlate = new G4LogicalVolume(solidGBLPlate, GetMaterial("Iron"), "GBLPlate");
  
  X = 0;
  Y = 0.5*GBLAYlength - 62*conv*cm;
  Z = 0;
  
  G4VPhysicalVolume* physGBLPlate1 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicGBLPlate, "GBLPlate1", logicGBLA, false, 0, checkOverlaps);
  
  X = 0;
  Y = 0.5*GBLAYlength - 124*conv*cm;
  Z = 0;
  
  G4VPhysicalVolume* physGBLPlate2 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicGBLPlate, "GBLPlate2", logicGBLA, false, 0, checkOverlaps);
  
  
 
  //HRS Gantry front leg
  
  G4double GFLIXlength = 73*conv*cm;
  G4double GFLIYlength = 80*conv*cm;
  G4double GFLIZlength = 72.625*conv*cm;
  
  par[1] = 0.5*GFLIXlength;
  par[2] = 0.5*GFLIYlength;
  par[3] = 0.5*GFLIZlength;

  G4Box* solidGFLI = new G4Box("GFLI", par[1], par[2], par[3]);
  
  G4LogicalVolume* logicGFLI = new G4LogicalVolume(solidGFLI, GetMaterial("Iron"), "GFLI");
  
  G4VisAttributes* GFLIvis = new G4VisAttributes(1);
  GFLIvis->SetForceWireframe(false);
  GFLIvis->SetColor(1.0,1.0,1.0);
  logicGFLI->SetVisAttributes(GFLIvis);
  
  X = -0.5*GCXlength + 0.5*GFLIXlength;
  Y = 0.5*GCYlength - 0.5*GFLIYlength - 73*conv*cm;
  Z = -0.5*GCZlength + 0.5*GFLIZlength;
  
  G4VPhysicalVolume* physGFLI1 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicGFLI, "GFLI1", logicGCbox, false, 0, checkOverlaps);
  
  X = 0.5*GCXlength - 0.5*GFLIXlength;
  Y = 0.5*GCYlength - 0.5*GFLIYlength - 73*conv*cm;
  Z = -0.5*GCZlength + 0.5*GFLIZlength;
  
  G4VPhysicalVolume* physGFLI2 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicGFLI, "GFLI2", logicGCbox, false, 0, checkOverlaps);
  
  //Air inside HRS Gantry front leg
  
  G4double GFLAXlength = 71*conv*cm;
  G4double GFLAYlength = 77*conv*cm;
  G4double GFLAZlength = 70.625*conv*cm;
  
  par[1] = 0.5*GFLAXlength;
  par[2] = 0.5*GFLAYlength;
  par[3] = 0.5*GFLAZlength;

  G4Box* solidGFLA = new G4Box("GFLA", par[1], par[2], par[3]);
  
  G4LogicalVolume* logicGFLA = new G4LogicalVolume(solidGFLA, GetMaterial("Air"), "GFLA");
  
  X = 0;
  Y = 1.5*conv*cm;
  Z = 0;
  
  G4VisAttributes* GFLAvis = new G4VisAttributes(1);
  GFLAvis->SetForceWireframe(false);
  GFLAvis->SetColor(1.0,1.0,1.0);
  logicGFLA->SetVisAttributes(GFLAvis);
  
  G4VPhysicalVolume* physGFLA = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicGFLA, "GFLA", logicGFLI, false, 0, checkOverlaps);
  
  
  
  //Gantry lower back section
  
  G4double GLBIXlength = 186*conv*cm;
  G4double GLBIYlength = 32*conv*cm;
  G4double GLBIZlength = 69.5*conv*cm;
  
  par[1] = 0.5*GLBIXlength;
  par[2] = 0.5*GLBIYlength;
  par[3] = 0.5*GLBIZlength;

  G4Box* solidGLBI = new G4Box("GLBI", par[1], par[2], par[3]);
  
  G4LogicalVolume* logicGLBI = new G4LogicalVolume(solidGLBI, GetMaterial("Iron"), "GLBI");
  
  G4VisAttributes* GLBIvis = new G4VisAttributes(1);
  GLBIvis->SetForceWireframe(false);
  GLBIvis->SetColor(1.0,1.0,1.0);
  logicGLBI->SetVisAttributes(GLBIvis);
  
  X = 0;
  Y = -0.5*GCYlength + 0.5*GLBIYlength;
  Z = 0.5*GCZlength - 0.5*GLBIZlength;
  
  G4VPhysicalVolume* physGLBI = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicGLBI, "GLBI", logicGCbox, false, 0, checkOverlaps);
  
  //Gantry lower back section - Air
  
  G4double GLBAXlength = 186*conv*cm;
  G4double GLBAYlength = 30*conv*cm;
  G4double GLBAZlength = 67.5*conv*cm;
  
  par[1] = 0.5*GLBAXlength;
  par[2] = 0.5*GLBAYlength;
  par[3] = 0.5*GLBAZlength;

  G4Box* solidGLBA = new G4Box("GLBA", par[1], par[2], par[3]);
  
  G4LogicalVolume* logicGLBA = new G4LogicalVolume(solidGLBA, GetMaterial("Air"), "GLBA");
  
  G4VisAttributes* GLBAvis = new G4VisAttributes(1);
  GLBAvis->SetForceWireframe(false);
  GLBAvis->SetColor(1.0,1.0,1.0);
  logicGLBA->SetVisAttributes(GLBAvis);
  
  X = 0;
  Y = 0;
  Z = 0;
  
  G4VPhysicalVolume* physGLBA = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicGLBA, "GLBA", logicGLBI, false, 0, checkOverlaps);
  
  //Plates in Gantry lower back section
  
  G4double GLBPlateXlength = 1.0*conv*cm;
  G4double GLBPlateYlength = GLBAYlength;
  G4double GLBPlateZlength = GLBAZlength;
  
  par[1] = 0.5*GLBPlateXlength;
  par[2] = 0.5*GLBPlateYlength;
  par[3] = 0.5*GLBPlateZlength;

  G4Box* solidGLBPlate = new G4Box("GLBPlate", par[1], par[2], par[3]);
  
  G4LogicalVolume* logicGLBPlate = new G4LogicalVolume(solidGLBPlate, GetMaterial("Iron"), "GLBPlate");
  
  X = 0;
  Y = 0;
  Z = 0;
  
  G4VPhysicalVolume* physGLBPlate1 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicGLBPlate, "GLBPlate1", logicGLBA, false, 0, checkOverlaps);

  X = -30*conv*cm;
  Y = 0;
  Z = 0;
  
  G4VPhysicalVolume* physGLBPlate2 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicGLBPlate, "GLBPlate2", logicGLBA, false, 0, checkOverlaps);
  
  X = 30*conv*cm;
  Y = 0;
  Z = 0;
  
  G4VPhysicalVolume* physGLBPlate3 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicGLBPlate, "GLBPlate3", logicGLBA, false, 0, checkOverlaps);
  
  
  //New HRS electronics detector
  G4int nx = 10;
  G4int ny = 1;
  G4int nz = 40;
  G4int copyNo;
  
  G4double HEDBoxXlength = 200*cm;
  G4double HEDBoxYlength = 200*cm;
  G4double HEDBoxZlength = 200*cm;
  
  par[1] = 0.5*HEDBoxXlength;
  par[2] = 0.5*HEDBoxYlength;
  par[3] = 0.5*HEDBoxZlength;
  
  G4Box* solidHEDBox = new G4Box("HEDBox", par[1], par[2], par[3]);
  G4LogicalVolume* logicHEDBox = new G4LogicalVolume(solidHEDBox, GetMaterial("Vacuum"),"HEDBox");
  
  X = 0;
  Y = 0;
  Z = 0;
  
  //G4VPhysicalVolume* physHEDBox = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicHEDBox,"HEDBox",logicGCbox,false,0,checkOverlaps);
  
  G4double HEDXlength = HEDBoxXlength/nx;
  G4double HEDYlength = HEDBoxYlength/ny;
  G4double HEDZlength = HEDBoxZlength/nz;
  
  par[1] = 0.5*HEDXlength;
  par[2] = 0.5*HEDYlength;
  par[3] = 0.5*HEDZlength;
  
  G4Box* solidHED = new G4Box("HED", par[1], par[2], par[3]);
  
  if(thetaHRS>0)
    {
      G4VPhysicalVolume* physHEDBox = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicHEDBox,"HEDBox",logicGCbox,false,0,checkOverlaps);

      G4LogicalVolume* logicHED_Left = new G4LogicalVolume(solidHED, GetMaterial("Scintillator"),"HED_Left");
      G4VisAttributes* HED_Leftvis = new G4VisAttributes(1);
      HED_Leftvis->SetForceWireframe(false);
      HED_Leftvis->SetColor(0.0,0.0,1.0);
      logicHED_Left->SetVisAttributes(HED_Leftvis);
      
      copyNo = 0;

      for(int i=0; i<nx; i++)
	{
	  for(int j=0; j<ny; j++)
	    {
	      for(int k=0; k<nz; k++)
		{
		  X = -HEDBoxXlength/2 + (2*i+1)*HEDXlength/2;
		  Y = -HEDBoxYlength/2 + (2*j+1)*HEDYlength/2;
		  Z = -HEDBoxZlength/2 + (2*k+1)*HEDZlength/2;
		  //G4VPhysicalVolume* physHED_Left = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicHED_Left,"HED_Left",logicHEDBox,false,copyNo,checkOverlaps);
		  copyNo++;
		}

	    }
	}

      //HRSDetSDL* HRSSD_Left = new HRSDetSDL("/HRSSD_Left");
      //(jc2 2017-04-27) changed it to a generic detector for simplicity
      GenericSD* HRSSD_Left = new GenericSD("HRSSD_Left");
      G4SDManager* HRSsdman_Left = G4SDManager::GetSDMpointer();
      HRSsdman_Left->AddNewDetector(HRSSD_Left);
      logicHEDBox->SetSensitiveDetector(HRSSD_Left);
	
    }
  else
    {
      
      G4LogicalVolume* logicHED_Right = new G4LogicalVolume(solidHED, GetMaterial("Scintillator"),"HED_Right");
      G4VisAttributes* HED_Rightvis = new G4VisAttributes(1);
      HED_Rightvis->SetForceWireframe(false);
      HED_Rightvis->SetColor(1.0,0.0,0.0);
      logicHED_Right->SetVisAttributes(HED_Rightvis);

      copyNo = 0;
      
      for(int i=0; i<nx; i++)
	{
	  for(int j=0; j<ny; j++)
	    {
	      for(int k=0; k<nz; k++)
		{
		  X = -HEDBoxXlength/2 + (2*i+1)*HEDXlength/2;
		  Y = -HEDBoxYlength/2 + (2*j+1)*HEDYlength/2;
		  Z = -HEDBoxZlength/2 + (2*k+1)*HEDZlength/2;
		  //G4VPhysicalVolume* physHED_Right = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicHED_Right,"HED_Right",logicHEDBox,false,copyNo,checkOverlaps);
		  copyNo++;
		}

	    }
	}

      HRSDetSDR* HRSSD_Right = new HRSDetSDR("/HRSSD_Right");
      G4SDManager* HRSsdman_Right = G4SDManager::GetSDMpointer();
      HRSsdman_Right->AddNewDetector(HRSSD_Right);
      //logicHED_Right->SetSensitiveDetector(HRSSD_Right);

    }
  


}



G4Material* Gantry::GetMaterial(G4String mat){

  return fDetCon->GetMaterial(mat);

}


Gantry::~Gantry(){;}
