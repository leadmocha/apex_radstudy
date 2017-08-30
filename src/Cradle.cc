#include "Cradle.hh"
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

Cradle::Cradle(G4double theta1, B1DetectorConstruction *dc, HallA *ha, G4LogicalVolume *logicWorld):fDetCon(dc),thetaHRS(theta1){

 //Variables of HallA class
  G4double Rhall = ha->getRhall();
  G4double bmheight = ha->getbmheight();
  G4double Tair1 = ha->getTair1();
  

  G4bool checkOverlaps = true;

 //dimentions are in inches

  const G4double conv = 2.54; //conversion from inches to cm

  G4double par[12];
  G4double X,Y,Z;

  
  //Cradle Enclosure. Cradle front and back parts are located in this volume.
  
  G4double CEXlength = 215.5*conv*cm + 2*cm;
  G4double CEYlength = 58*conv*cm;
  G4double CEZlength = 468*conv*cm;
  
  par[1] = 0.5*CEXlength;
  par[2] = 0.5*CEYlength;
  par[3] = 0.5*CEZlength;
  
  G4Box* solidCEbox = new G4Box("CEbox", par[1], par[2], par[3]);
  
  G4RotationMatrix* rot_thetaHRS = new G4RotationMatrix();
  rot_thetaHRS->rotateY(-thetaHRS);
  
  G4double CEposX = (680*cm + 0.5*CEZlength)*sin(thetaHRS);
  G4double CEposY = -bmheight + 99.61*cm + 0.5*CEYlength;
  G4double CEposZ = (680*cm + 0.5*CEZlength)*cos(thetaHRS);
  
  //cutaway volume 1 from cradle enclosure
  
  G4double cutCE1Xlength = 50*conv*cm;
  G4double cutCE1Ylength = 58*conv*cm + 1*cm;
  G4double cutCE1Zlength = 500*conv*cm;
  
  par[1] = 0.5*cutCE1Xlength;
  par[2] = 0.5*cutCE1Ylength;
  par[3] = 0.5*cutCE1Zlength;
  
  G4Box* solidCEcut1 = new G4Box("CEcut1", par[1], par[2], par[3]);
  
  G4RotationMatrix* rot_5Y = new G4RotationMatrix();
  rot_5Y->rotateY(-5*deg);
  
  X = 0.5*CEXlength + 0.5*cutCE1Xlength - 18.25*conv*cm;
  Y = 0;
  Z = 0.5*CEZlength - 346*conv*cm;
  
  G4SubtractionSolid* solidCEbox_cut1 = new G4SubtractionSolid("CEbox_cut1",solidCEbox, solidCEcut1,rot_5Y,G4ThreeVector(X,Y,Z));
  
  G4RotationMatrix* rot5Y = new G4RotationMatrix();
  rot5Y->rotateY(5*deg);
  
  X = -0.5*CEXlength - 0.5*cutCE1Xlength + 18.25*conv*cm;
  Y = 0;
  Z = 0.5*CEZlength - 346*conv*cm;
  
  G4SubtractionSolid* solidCEbox_cut2 = new G4SubtractionSolid("CEbox_cut2",solidCEbox_cut1, solidCEcut1,rot5Y,G4ThreeVector(X,Y,Z));
  
  //cutaway volume center
  
  G4double cutCE3Xlength = 113*conv*cm;
  G4double cutCE3Ylength = 58*conv*cm + 1*cm;
  G4double cutCE3Zlength = 301.5*conv*cm;
  
  par[1] = 0.5*cutCE3Xlength;
  par[2] = 0.5*cutCE3Ylength;
  par[3] = 0.5*cutCE3Zlength;
  
  G4Box* solidCEcut3 = new G4Box("CEcut3", par[1], par[2], par[3]);
  
  X = 0;
  Y = 0;
  Z = 0.5*CEZlength - 0.5*cutCE3Zlength - 44.5*conv*cm;
  
  G4SubtractionSolid* solidCEbox_cut3 = new G4SubtractionSolid("CEbox_cut3",solidCEbox_cut2, solidCEcut3,0,G4ThreeVector(X,Y,Z));
  
  
  //cutaway volume 4 and 5
  
  G4double cutCE4Xlength = 2*(0.5*CEXlength - 57.375*conv*cm)*cos(12*deg);
  G4double cutCE4Ylength = CEYlength + 1*cm;
  G4double cutCE4Zlength = 400*conv*cm;
  
  par[1] = 0.5*cutCE4Xlength;
  par[2] = 0.5*cutCE4Ylength;
  par[3] = 0.5*cutCE4Zlength;
  
  G4Box* solidCEcut4 = new G4Box("CEcut4", par[1], par[2], par[3]);
  
  G4RotationMatrix* rot_12Y = new G4RotationMatrix();
  rot_12Y->rotateY(-12*deg);
  
  X = 0.5*CEXlength;
  Y = 0;
  Z = -0.5*CEZlength;
  
  G4SubtractionSolid* solidCEbox_cut4 = new G4SubtractionSolid("CBbox_cut4",solidCEbox_cut3, solidCEcut4,rot_12Y,G4ThreeVector(X,Y,Z));
  
  G4RotationMatrix* rot12Y = new G4RotationMatrix();
  rot12Y->rotateY(12*deg);
  
  X = -0.5*CEXlength;
  Y = 0;
  Z = -0.5*CEZlength;
  
  G4SubtractionSolid* solidCEbox_cut5 = new G4SubtractionSolid("CBbox_cut5",solidCEbox_cut4, solidCEcut4,rot12Y,G4ThreeVector(X,Y,Z));
  
  
  G4LogicalVolume* logicCEbox = new G4LogicalVolume(solidCEbox_cut5, GetMaterial("Air"), "CEbox_cut3");
  
  G4VisAttributes* CEboxvis = new G4VisAttributes(0);
  CEboxvis->SetForceWireframe(true);
  CEboxvis->SetColor(1.0,1.0,0.0);
  logicCEbox->SetVisAttributes(CEboxvis);
  
  G4VPhysicalVolume* physCEbox = new G4PVPlacement(rot_thetaHRS, G4ThreeVector(CEposX,CEposY,CEposZ), logicCEbox, "CEbox", logicWorld, false, 0, checkOverlaps);
  
  
  
  //Place Cradle backpart inside cradle enclosure
  
  //Cradle - back part
  
  G4double CBXlength = 107.75*conv*cm;
  G4double CBYlength = 58*conv*cm;
  G4double CBZlength = 346*conv*cm;
  
  par[1] = 0.5*CBXlength;
  par[2] = 0.5*CBYlength;
  par[3] = 0.5*CBZlength;
  
  G4Box* solidCBbox = new G4Box("CBbox", par[1], par[2], par[3]);
  
  G4double CBposX = 0;
  G4double CBposY = 0;
  G4double CBposZ = 0;
  
  //cutaway volume 1 from cradle back part
  
  G4double cut1Xlength = CBXlength - 51.25*conv*cm + 1*cm;
  G4double cut1Ylength = CBYlength + 1*cm;
  G4double cut1Zlength = CBZlength - 44.5*conv*cm + 1*cm;
  
  par[1] = 0.5*cut1Xlength;
  par[2] = 0.5*cut1Ylength;
  par[3] = 0.5*cut1Zlength;
  
  G4Box* solidCBcut1 = new G4Box("CBcut1", par[1], par[2], par[3]);
  
  X = -0.5*CBXlength + 0.5*cut1Xlength - 1*cm;
  Y = 0;
  Z = -0.5*CBZlength + 0.5*cut1Zlength -1*cm;
  
  G4SubtractionSolid* solidCBbox_cut1 = new G4SubtractionSolid("CBbox_cut1",solidCBbox, solidCBcut1,0,G4ThreeVector(X,Y,Z));
  
  //cutaway volume 2 from cradle back part
  
  G4double cut2Xlength = 2*(51.25 - 33)*conv*cm;
  G4double cut2Ylength = CBYlength + 1*cm;
  G4double cut2Zlength = 500*conv*cm;
  
  par[1] = 0.5*cut2Xlength;
  par[2] = 0.5*cut2Ylength;
  par[3] = 0.5*cut2Zlength;
  
  G4Box* solidCBcut2 = new G4Box("CBcut2", par[1], par[2], par[3]);
  
  X = 0.5*CBXlength;
  Y = 0;
  Z = -0.5*CBZlength;
  
  G4SubtractionSolid* solidCBbox_cut2 = new G4SubtractionSolid("CBbox_cut2",solidCBbox_cut1, solidCBcut2,rot_5Y,G4ThreeVector(X,Y,Z));
  
  //cutaway volume 3
  
  G4double cut3Xlength = 2*5.5*cos(12*deg)*conv*cm;
  G4double cut3Ylength = CBYlength + 1*cm;
  G4double cut3Zlength = 150*conv*cm;
  
  par[1] = 0.5*cut3Xlength;
  par[2] = 0.5*cut3Ylength;
  par[3] = 0.5*cut3Zlength;
  
  G4Box* solidCBcut3 = new G4Box("CBcut3", par[1], par[2], par[3]);
  
  X = 0.5*CBXlength - (51.25 - 32)*conv*cm ;
  Y = 0;
  Z = -0.5*CBZlength;
  
  G4SubtractionSolid* solidCBbox_cut3 = new G4SubtractionSolid("CBbox_cut3",solidCBbox_cut2, solidCBcut3,rot_12Y,G4ThreeVector(X,Y,Z));
  
  
  //Make an outbox to hold only the back part of the cradle
  G4LogicalVolume* logicCBOutbox = new G4LogicalVolume(solidCBbox_cut3, GetMaterial("Air"), "CBbox_cut3Outbox");
  
  G4double CBOutbox1posX = 0.5*CEXlength - 0.5*CBXlength - 1*cm;
  G4double CBOutbox1posY = 0.5*CEYlength - 0.5*CBYlength;
  G4double CBOutbox1posZ = 0.5*CEZlength - 0.5*CBZlength;
  
  G4VisAttributes* CBOutboxvis = new G4VisAttributes(0);
  CBOutboxvis->SetForceWireframe(false);
  CBOutboxvis->SetColor(0.0,0.0,1.0);
  logicCBOutbox->SetVisAttributes(CBOutboxvis);
  
  //Placing 2 copies of the outbox. one rotated 180 degrees around Z axis.
  
  G4VPhysicalVolume* physCBOutbox1 = new G4PVPlacement(0, G4ThreeVector(CBOutbox1posX,CBOutbox1posY,CBOutbox1posZ), logicCBOutbox, "CBOutbox1", logicCEbox, false, 0, checkOverlaps);
  
  G4double CBOutbox2posX = -0.5*CEXlength + 0.5*CBXlength + 1*cm;
  G4double CBOutbox2posY = 0.5*CEYlength - 0.5*CBYlength;
  G4double CBOutbox2posZ = 0.5*CEZlength - 0.5*CBZlength;
  
  G4RotationMatrix* rot180Z = new G4RotationMatrix();
  rot180Z->rotateZ(180*deg);
  
  G4VPhysicalVolume* physCBOutbox2 = new G4PVPlacement(rot180Z, G4ThreeVector(CBOutbox2posX,CBOutbox2posY,CBOutbox2posZ), logicCBOutbox, "CBOutbox2", logicCEbox, false, 0, checkOverlaps);
  
  G4LogicalVolume* logicCBbox = new G4LogicalVolume(solidCBbox_cut3, GetMaterial("Iron"), "CBbox_cut3");
  
  G4VisAttributes* CBboxvis = new G4VisAttributes(1);
  CBboxvis->SetForceWireframe(false);
  CBboxvis->SetColor(0.0,1.0,0.0);
  logicCBbox->SetVisAttributes(CBboxvis);
  
  G4VPhysicalVolume* physCBbox = new G4PVPlacement(0, G4ThreeVector(CBposX,CBposY,CBposZ), logicCBbox, "CBbox", logicCBOutbox, false, 0, checkOverlaps);
  
  //Crate - back part - inside Air volume
  
  G4double CBVXlength = CBXlength - 1.5*conv*cm;
  G4double CBVYlength = CBYlength - 1.5*conv*cm;
  G4double CBVZlength = CBZlength - 1.5*conv*cm;
  
  par[1] = 0.5*CBVXlength;
  par[2] = 0.5*CBVYlength;
  par[3] = 0.5*CBVZlength;
  
  G4Box* solidCBVbox = new G4Box("CBVbox", par[1], par[2], par[3]);
  
  G4double CBVposX = 0;
  G4double CBVposY = 0;
  G4double CBVposZ = 0;
  
  //cutaway volume 1 from cradle back part - Air volume
  
  G4double cut1VXlength = CBVXlength - 49.75*conv*cm + 1*cm;
  G4double cut1VYlength = CBVYlength + 1*cm;
  G4double cut1VZlength = CBVZlength - 43*conv*cm + 1*cm;
  
  par[1] = 0.5*cut1VXlength;
  par[2] = 0.5*cut1VYlength;
  par[3] = 0.5*cut1VZlength;
  
  G4Box* solidCBVcut1 = new G4Box("CBVcut1", par[1], par[2], par[3]);
  
  X = -0.5*CBVXlength + 0.5*cut1VXlength - 1*cm;
  Y = 0;
  Z = -0.5*CBVZlength + 0.5*cut1VZlength - 1*cm;
  
  G4SubtractionSolid* solidCBVbox_cut1 = new G4SubtractionSolid("CBVbox_cut1",solidCBVbox, solidCBVcut1,0,G4ThreeVector(X,Y,Z));
  
  //cutaway volume 2 from cradle back part - Air volume
  
  G4double cut2VXlength = 2*(51.25 - 33)*conv*cm;
  G4double cut2VYlength = CBVYlength + 1*cm;
  G4double cut2VZlength = 500*conv*cm;
  
  par[1] = 0.5*cut2VXlength;
  par[2] = 0.5*cut2VYlength;
  par[3] = 0.5*cut2VZlength;
  
  G4Box* solidCBVcut2 = new G4Box("CBVcut2", par[1], par[2], par[3]);
  
  X = 0.5*CBVXlength;
  Y = 0;
  Z = -0.5*CBVZlength;
  
  G4SubtractionSolid* solidCBVbox_cut2 = new G4SubtractionSolid("CBVbox_cut2",solidCBVbox_cut1, solidCBVcut2,rot_5Y,G4ThreeVector(X,Y,Z));
  
  //cutaway volume 3 from cradle back part - Air volume
  
  G4double cut3VXlength = 2*5.5*cos(12*deg)*conv*cm;
  G4double cut3VYlength = CBVYlength + 1*cm;
  G4double cut3VZlength = 150*conv*cm;
  
  par[1] = 0.5*cut3VXlength;
  par[2] = 0.5*cut3VYlength;
  par[3] = 0.5*cut3VZlength;
  
  G4Box* solidCBVcut3 = new G4Box("CBVcut3", par[1], par[2], par[3]);
  
  X = 0.5*CBVXlength - (51.25 - 32)*conv*cm ;
  Y = 0;
  Z = -0.5*CBVZlength;
  
  G4SubtractionSolid* solidCBVbox_cut3 = new G4SubtractionSolid("CBVbox_cut3",solidCBVbox_cut2, solidCBVcut3,rot_12Y,G4ThreeVector(X,Y,Z));
  
  G4LogicalVolume* logicCBVbox = new G4LogicalVolume(solidCBVbox_cut3, GetMaterial("Air"), "CBVbox_cut3");
  
  G4VisAttributes* CBVboxvis = new G4VisAttributes(1);
  CBVboxvis->SetForceWireframe(false);
  CBVboxvis->SetColor(0.0,0.0,0.0);
  logicCBVbox->SetVisAttributes(CBVboxvis);
  
  G4VPhysicalVolume* physCBVbox = new G4PVPlacement(0, G4ThreeVector(CBVposX,CBVposY,CBVposZ), logicCBVbox, "CBVbox", logicCBbox, false, 0, checkOverlaps);
  
  //Steel plates are located in this Air volume
  
  const G4int n = 12;
  
  G4double PlateLength[n] = {49.75, 49.75, 49.75, 49.75, 49.75, 46.75, 43.5, 40, 36.75, 29, 43, 43};
  G4double PlateZpos[n] = {21.25, 42.75, 72.5, 99.5, 131.5, 166.75, 202.75, 242.75, 282.75, 323.5, 18, 54};
  G4Box* solidPlate[n];
  G4LogicalVolume* logicPlate[n];
  G4VPhysicalVolume* physPlate[n];
  G4String PlateName[n] = {"P1", "P2", "P3", "P4", "P5", "P6", "P7", "P8", "P9", "P10", "P11", "P12"};
  
  G4VisAttributes* Platevis = new G4VisAttributes(1);
  Platevis->SetForceWireframe(true);
  Platevis->SetColor(7.0,0.0,0.0);
  
  G4RotationMatrix* rot90Y = new G4RotationMatrix();
  rot90Y->rotateY(90*deg);
  
  for(G4int i=0; i<n; i++)
  {
    
    solidPlate[i] = new G4Box(PlateName[i], 0.5*PlateLength[i]*conv*cm, 0.5*56.5*conv*cm, 0.5*0.75*conv*cm);
    logicPlate[i] = new G4LogicalVolume(solidPlate[i], GetMaterial("Iron"), PlateName[i]);
    
    logicPlate[i]->SetVisAttributes(Platevis);
    
    if(i<10)
    {
      X = 0.5*CBVXlength + 0.5*PlateLength[i]*conv*cm - (51.25 - 1.5)*conv*cm;
      Y = 0;
      Z = 0.5*CBVZlength - PlateZpos[i]*conv*cm;
    
      physPlate[i] = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicPlate[i], PlateName[i], logicCBVbox, false, 0, checkOverlaps);
    }
    else
    {
      X = -0.5*CBVXlength + PlateZpos[i]*conv*cm;
      Y = 0;
      Z = 0.5*CBVZlength - 0.5*43*conv*cm;
      
      physPlate[i] = new G4PVPlacement(rot90Y, G4ThreeVector(X,Y,Z), logicPlate[i], PlateName[i], logicCBVbox, false, 0, checkOverlaps);
      
    }
    
  }  
  
  //Place Cradle front part inside the Enclosure
  
//Cradle - front part
  
  G4double CFXlength = 84*conv*cm;
  G4double CFYlength = 58*conv*cm;
  G4double CFZlength = 122*conv*cm;
  
  par[1] = 0.5*CFXlength;
  par[2] = 0.5*CFYlength;
  par[3] = 0.5*CFZlength;
  
  G4Box* solidCFbox = new G4Box("CFbox", par[1], par[2], par[3]);
  
  G4double CFposX = 0;
  G4double CFposY = 0;
  G4double CFposZ = 0;
  
  //cutaway volume 1 from cradle front part
  
  G4double cutCF1Xlength = 2*(CFXlength - 57.375*conv*cm);
  G4double cutCF1Ylength = CFYlength + 1*cm;
  G4double cutCF1Zlength = 2*CFZlength + 1*cm;
  
  par[1] = 0.5*cutCF1Xlength;
  par[2] = 0.5*cutCF1Ylength;
  par[3] = 0.5*cutCF1Zlength;
  
  G4Box* solidCFcut1 = new G4Box("CFcut1", par[1], par[2], par[3]);
  
  X = 0.5*CFXlength;
  Y = 0;
  Z = -0.5*CFZlength;
  
  G4SubtractionSolid* solidCFbox_cut1 = new G4SubtractionSolid("CFbox_cut1",solidCFbox, solidCFcut1,rot_12Y,G4ThreeVector(X,Y,Z));
  
  
  //Make an out box for cradle front part
  
  G4LogicalVolume* logicCFOutbox = new G4LogicalVolume(solidCFbox_cut1, GetMaterial("Air"), "CFOutbox_cut1");
  
  G4double CFOutbox1posX = 0.5*CFXlength;
  G4double CFOutbox1posY = 0.5*CEYlength - 0.5*CFYlength;
  G4double CFOutbox1posZ = -0.5*CEZlength + 0.5*CFZlength;
  
  G4VisAttributes* CFOutboxvis = new G4VisAttributes(0);
  CFOutboxvis->SetForceWireframe(false);
  CFOutboxvis->SetColor(0.0,0.0,1.0);
  logicCFOutbox->SetVisAttributes(CFOutboxvis);
  
  //Placing 2 copies of the outbox. one rotated 180 degrees around Z axis.
  
  G4VPhysicalVolume* physCFOutbox1 = new G4PVPlacement(0, G4ThreeVector(CFOutbox1posX,CFOutbox1posY,CFOutbox1posZ), logicCFOutbox, "CFOutbox1", logicCEbox, false, 0, checkOverlaps);
  
  G4double CFOutbox2posX = -0.5*CFXlength;
  G4double CFOutbox2posY = 0.5*CEYlength - 0.5*CFYlength;
  G4double CFOutbox2posZ = -0.5*CEZlength + 0.5*CFZlength;
  
  G4VPhysicalVolume* physCFOutbox2 = new G4PVPlacement(rot180Z, G4ThreeVector(CFOutbox2posX,CFOutbox2posY,CFOutbox2posZ), logicCFOutbox, "CFOutbox2", logicCEbox, false, 0, checkOverlaps);
  
  G4LogicalVolume* logicCFbox = new G4LogicalVolume(solidCFbox_cut1, GetMaterial("Iron"), "CBbox_cut1");
  
  G4VisAttributes* CFboxvis = new G4VisAttributes(1);
  CFboxvis->SetForceWireframe(false);
  CFboxvis->SetColor(0.0,1.0,0.0);
  logicCFbox->SetVisAttributes(CFboxvis);
  
  G4VPhysicalVolume* physCFbox = new G4PVPlacement(0, G4ThreeVector(CFposX,CFposY,CFposZ), logicCFbox, "CFbox", logicCFOutbox, false, 0, checkOverlaps);
  
  //Cradle - front part - inside Air volume
  
  G4double CFVXlength = CFXlength - 0.75*conv*cm;
  G4double CFVYlength = CFYlength - 1.5*conv*cm;
  G4double CFVZlength = CFZlength - 0.75*conv*cm;
  
  par[1] = 0.5*CFVXlength;
  par[2] = 0.5*CFVYlength;
  par[3] = 0.5*CFVZlength;
  
  G4Box* solidCFVbox = new G4Box("CFVbox", par[1], par[2], par[3]);
  
  G4double CFVposX = -0.5*CFXlength + 0.5*CFVXlength;
  G4double CFVposY = 0;
  G4double CFVposZ = 0.5*CFZlength - 0.5*CFVZlength;
  
  //cutaway volume 1 from cradle front part - Air volume
  
  G4double cutCF1VXlength = 2*(CFVXlength - 57.375*conv*cm);
  G4double cutCF1VYlength = CFVYlength + 1*cm;
  G4double cutCF1VZlength = 2*CFVZlength + 1*cm;
  
  par[1] = 0.5*cutCF1VXlength;
  par[2] = 0.5*cutCF1VYlength;
  par[3] = 0.5*cutCF1VZlength;
  
  G4Box* solidCFVcut1 = new G4Box("CFVcut1", par[1], par[2], par[3]);
  
  X = 0.5*CFVXlength;
  Y = 0;
  Z = -0.5*CFVZlength;
  
  G4SubtractionSolid* solidCFVbox_cut1 = new G4SubtractionSolid("CFVbox_cut1",solidCFVbox, solidCFVcut1,rot_12Y,G4ThreeVector(X,Y,Z));
  
  G4LogicalVolume* logicCFVbox = new G4LogicalVolume(solidCFVbox_cut1, GetMaterial("Air"), "CFVbox_cut1");
  
  G4VisAttributes* CFVboxvis = new G4VisAttributes(1);
  CFVboxvis->SetForceWireframe(false);
  CFVboxvis->SetColor(0.0,0.0,0.0);
  logicCFVbox->SetVisAttributes(CFVboxvis);
  
  G4VPhysicalVolume* physCFVbox = new G4PVPlacement(0, G4ThreeVector(CFVposX,CFVposY,CFVposZ), logicCFVbox, "CFVbox", logicCFbox, false, 0, checkOverlaps);
  
  //Steel plates are located in this Air volume
  
  const G4int fn = 19;
  
  G4double fPlateZpos[fn] = {19.25, 46.25, 84.25, 102, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  G4double fPlateXpos[fn] = {0, 0, 0, 0, 7, 25.5, 47, 7, 25.5, 47, 7, 25.5, 47, 7, 25.5, 47, 7, 25.5, 47};
  G4double fPlateLength[fn] = {77.75, 72, 64, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  

  //First 4 elements of the array defines plates in z direction
  //Other values are for plates in x direction and they are calculated below
  
  fPlateLength[4] = (fPlateZpos[0] - 0.375);
  fPlateZpos[4] = 0.5*fPlateLength[4];
    
  fPlateLength[5] = (fPlateZpos[0] - 0.375);
  fPlateZpos[5] = 0.5*fPlateLength[5];
    
  fPlateLength[6] = (fPlateZpos[0] - 0.375);
  fPlateZpos[6] = 0.5*fPlateLength[6];
    
  fPlateLength[7] = (fPlateZpos[1] - fPlateZpos[0] - 0.75);
  fPlateZpos[7] = (fPlateZpos[1] - 0.375 - 0.5*fPlateLength[7]);
  
  fPlateLength[8] = (fPlateZpos[1] - fPlateZpos[0] - 0.75);
  fPlateZpos[8] = (fPlateZpos[1] - 0.375 - 0.5*fPlateLength[8]);
  
  fPlateLength[9] = (fPlateZpos[1] - fPlateZpos[0] - 0.75);
  fPlateZpos[9] = (fPlateZpos[1] - 0.375 - 0.5*fPlateLength[9]);
  
  fPlateLength[10] = (fPlateZpos[2] - fPlateZpos[1] - 0.75);
  fPlateZpos[10] = (fPlateZpos[2] - 0.375 - 0.5*fPlateLength[10]);
    
  fPlateLength[11] = (fPlateZpos[2] - fPlateZpos[1] - 0.75);
  fPlateZpos[11] = (fPlateZpos[2] - 0.375 - 0.5*fPlateLength[11]);
  
  fPlateLength[12] = (fPlateZpos[2] - fPlateZpos[1] - 0.75);
  fPlateZpos[12] = (fPlateZpos[2] - 0.375 - 0.5*fPlateLength[12]);
  
  fPlateLength[13] = (fPlateZpos[3] - fPlateZpos[2] - 0.75);
  fPlateZpos[13] = (fPlateZpos[3] - 0.375 - 0.5*fPlateLength[13]);
  
  fPlateLength[14] = (fPlateZpos[3] - fPlateZpos[2] - 0.75);
  fPlateZpos[14] = (fPlateZpos[3] - 0.375 - 0.5*fPlateLength[14]);
  
  fPlateLength[15] = (fPlateZpos[3] - fPlateZpos[2] - 0.75);
  fPlateZpos[15] = (fPlateZpos[3] - 0.375 - 0.5*fPlateLength[15]);
  
  fPlateLength[16] = 18.5;
  fPlateZpos[16] = fPlateZpos[3] + 0.375 + 0.5*fPlateLength[16];
  
  fPlateLength[17] = 18.5;
  fPlateZpos[17] = fPlateZpos[3] + 0.375 + 0.5*fPlateLength[17];
  
  fPlateLength[18] = 18.5;
  fPlateZpos[18] = fPlateZpos[3] + 0.375 + 0.5*fPlateLength[18];
  
  
  G4Box* solidfPlate[fn];
  G4LogicalVolume* logicfPlate[fn];
  G4VPhysicalVolume* physfPlate[fn];
  G4String PlatefName[fn] = {"fP1", "fP2", "fP3", "fP4", "fP5", "fP6", "fP7", "fP8", "fP9", "fP10", "fP11", "fP12", "fP13", "fP14", "fP15", "fP16", "fP17", "fP18", "fP19"};
  
  G4VisAttributes* fPlatevis = new G4VisAttributes(1);
  fPlatevis->SetForceWireframe(true);
  fPlatevis->SetColor(7.0,0.0,0.0);
  
  for(G4int i=0; i<fn; i++)
  {
    solidfPlate[i] = new G4Box(PlatefName[i], 0.5*fPlateLength[i]*conv*cm, 0.5*56.5*conv*cm, 0.5*0.75*conv*cm);
    logicfPlate[i] = new G4LogicalVolume(solidfPlate[i], GetMaterial("Iron"), PlatefName[i]);
    
    logicfPlate[i]->SetVisAttributes(fPlatevis);
    
    if(i<4)
    {
      X = -0.5*CFVXlength + 0.5*fPlateLength[i]*conv*cm;
      Y = 0;
      Z = 0.5*CFVZlength - fPlateZpos[i]*conv*cm;
    
      physfPlate[i] = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicfPlate[i], PlatefName[i], logicCFVbox, false, 0, checkOverlaps);
    }
    else
    {
     
      X = -0.5*CFVXlength + fPlateXpos[i]*conv*cm;
      Y = 0;
      Z = 0.5*CFVZlength - fPlateZpos[i]*conv*cm;
      
      
      physfPlate[i] = new G4PVPlacement(rot90Y, G4ThreeVector(X,Y,Z), logicfPlate[i], PlatefName[i], logicCFVbox, false, 0, checkOverlaps);
      
    }
    
  }
  
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////Dipole Support Beam/////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  G4double theta = atan2(69.75,83);
  
  G4double DSBXlength = 188*conv*cm;
  G4double DSBYlength = 69.75*conv*cm;
  G4double DSBZlength = 83*conv*cm;

  par[1] = 0.5*DSBXlength;
  par[2] = 0.5*DSBYlength;
  par[3] = 0.5*DSBZlength;
  
  G4Box* solidDSBbox = new G4Box("DSBbox", par[1], par[2], par[3]);
  
  G4double DSBposX = (1750*cm - 0.5*DSBZlength)*sin(thetaHRS);
  G4double DSBposY = -bmheight + 99.61*cm + CEYlength + 0.5*DSBYlength;
  G4double DSBposZ = (1750*cm - 0.5*DSBZlength)*cos(thetaHRS);;
  
  //cutaway volume 1 from Dipole Support Beam
  
  G4double DSBcut1Xlength = DSBXlength + 1*cm;
  G4double DSBcut1Ylength = 2*83*sin(theta)*conv*cm;
  G4double DSBcut1Zlength = 150*conv*cm;
  
  par[1] = 0.5*DSBcut1Xlength;
  par[2] = 0.5*DSBcut1Ylength;
  par[3] = 0.5*DSBcut1Zlength;
  
  G4Box* solidDSBcut1 = new G4Box("DSBcut1", par[1], par[2], par[3]);
  
  X = 0;
  Y = 0.5*DSBYlength;
  Z = -0.5*DSBZlength - 8*conv*cm;
  
  G4RotationMatrix* rotThetaY = new G4RotationMatrix();
  rotThetaY->rotateX(theta);
  
  G4SubtractionSolid* solidDSBbox_cut1 = new G4SubtractionSolid("DSBbox_cut1",solidDSBbox, solidDSBcut1,rotThetaY,G4ThreeVector(X,Y,Z));

  G4LogicalVolume* logicDSBbox = new G4LogicalVolume(solidDSBbox_cut1, GetMaterial("Air"), "DSBbox");
  
  G4VisAttributes* DSBboxvis = new G4VisAttributes(1);
  DSBboxvis->SetForceWireframe(false);
  DSBboxvis->SetColor(1.0,0.0,0.0);
  logicDSBbox->SetVisAttributes(DSBboxvis);
  
  G4VPhysicalVolume* physDSBbox = new G4PVPlacement(rot_thetaHRS, G4ThreeVector(DSBposX,DSBposY,DSBposZ), logicDSBbox, "DSBbox", logicWorld, false, 0, checkOverlaps);
  
  //Lay plates inside this volume
  
  G4double PlateXlength = 0;
  G4double PlateYlength = 0;
  G4double PlateZlength = 0;
  
 //
  PlateXlength = DSBXlength - 4*conv*cm;
  PlateYlength = 3.0*conv*cm;
  PlateZlength = 83*conv*cm;
  
  par[1] = 0.5*PlateXlength;
  par[2] = 0.5*PlateYlength;
  par[3] = 0.5*PlateZlength;
  
  G4Box* solidPlate2 = new G4Box("Plate2", par[1], par[2], par[3]);
  G4LogicalVolume* logicPlate2 = new G4LogicalVolume(solidPlate2, GetMaterial("Iron"), "Plate2");

  X = 0;
  Y = -0.5*DSBYlength + 0.5*PlateYlength;
  Z = 0;
  
  G4VPhysicalVolume* physPlate2 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicPlate2, "Plate2", logicDSBbox, false, 0, checkOverlaps);
  
  //
  PlateXlength = DSBXlength - 4*conv*cm;
  PlateYlength = 69.75*conv*cm - 3*conv*cm;
  PlateZlength = 1.5*conv*cm;
  
  par[1] = 0.5*PlateXlength;
  par[2] = 0.5*PlateYlength;
  par[3] = 0.5*PlateZlength;
  
  G4Box* solidPlate3 = new G4Box("Plate3", par[1], par[2], par[3]);
  G4LogicalVolume* logicPlate3 = new G4LogicalVolume(solidPlate3, GetMaterial("Iron"), "Plate3");

  X = 0;
  Y = 0.5*DSBYlength - 0.5*PlateYlength;
  Z = 0.5*DSBZlength - 0.5*PlateZlength;
  
  G4VPhysicalVolume* physPlate3 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicPlate3, "Plate3", logicDSBbox, false, 0, checkOverlaps);
  
  //Side Plates
  PlateXlength = 2*conv*cm;
  PlateYlength = 69.75*conv*cm;
  PlateZlength = 83*conv*cm;
  
  par[1] = 0.5*PlateXlength;
  par[2] = 0.5*PlateYlength;
  par[3] = 0.5*PlateZlength;
  
  G4Box* solidPlate4 = new G4Box("Plate4", par[1], par[2], par[3]);
  
  X = 0;
  Y = 0.5*PlateYlength;
  Z = -0.5*PlateZlength;
  
  G4SubtractionSolid* solidPlate4_cut1 = new G4SubtractionSolid("solidPlate4_cut1",solidPlate4, solidDSBcut1,rotThetaY,G4ThreeVector(X,Y,Z));

  G4LogicalVolume* logicPlate4 = new G4LogicalVolume(solidPlate4_cut1, GetMaterial("Iron"), "Plate4");
  
  X = -0.5*DSBXlength + 0.5*PlateXlength;
  Y = 0;
  Z = 0;
  
  G4VPhysicalVolume* physPlate4_copy1 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicPlate4, "Plate4", logicDSBbox, false, 0, checkOverlaps);
  
  X = 0.5*DSBXlength - 0.5*PlateXlength;
  Y = 0;
  Z = 0;
  
  G4VPhysicalVolume* physPlate4_copy2 = new G4PVPlacement(0, G4ThreeVector(X,Y,Z), logicPlate4, "Plate4", logicDSBbox, false, 0, checkOverlaps);
  
  //
  PlateXlength = DSBXlength;
  PlateYlength = 3.5*conv*cm;
  PlateZlength = 98*conv*cm;
  
  par[1] = 0.5*PlateXlength;
  par[2] = 0.5*PlateYlength;
  par[3] = 0.5*PlateZlength;
  
  G4Box* solidPlate1 = new G4Box("Plate1", par[1], par[2], par[3]);
  G4LogicalVolume* logicPlate1 = new G4LogicalVolume(solidPlate1, GetMaterial("Iron"), "Plate1");

  X = 0;
  Y = 5*cm;
  Z = -0.5*PlateYlength + 3*cm;
  
  G4VPhysicalVolume* physPlate1 = new G4PVPlacement(rotThetaY, G4ThreeVector(X,Y,Z), logicPlate1, "Plate1", logicDSBbox, false, 0, checkOverlaps);
  

}



G4Material* Cradle::GetMaterial(G4String mat){

  return fDetCon->GetMaterial(mat);

}


Cradle::~Cradle(){;}
