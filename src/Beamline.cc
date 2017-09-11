#include "Beamline.hh"
#include "G4NistManager.hh"
#include "B1DetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4VisAttributes.hh"

#include "G4ThreeVector.hh"

#include "HallA.hh"

//using namespace std;

Beamline::Beamline(B1DetectorConstruction *dc, HallA *ha, G4LogicalVolume *logicWorld):fDetCon(dc){

  //Variables of HallA class
  G4double Rhall = ha->getRhall();
  G4double bmheight = ha->getbmheight();
  G4double Tair1 = ha->getTair1();
  G4double BmDEX = ha->getBmDEX();
  G4double BmDEY = ha->getBmDEY();
  G4double BmDEZ = ha->getBmDEZ();
  G4double BPAirTubeLength = ha->getBPAirTubeLength();
  

 G4bool checkOverlaps = true;

 //dimentions are in inches

 const G4double conv = 2.54; //conversion from inches to cm

 G4double par[10];
 G4double X,Y,Z;

 //Rotation Matrices
  G4RotationMatrix* rot90X = new G4RotationMatrix();
  rot90X->rotateX(90*deg);
  G4RotationMatrix* rot270X = new G4RotationMatrix();
  rot270X->rotateX(270*deg);

  G4double TBLoffset = 132.2832*cm;    //Distance to beginning of telescopic beamline from hall center
  G4double TBLlength = 159.7863*cm;    //Length of Telescopic beamline
  G4double TBL_BLoffset = 21.0439*cm;  //Gap between Telescopic beamline and main beamline

 /////////////////////////////////////////////////////////////////
 ///////////////////////BeamLine - Target to Wall/////////////////
 /////////////////////////////////////////////////////////////////

 G4double BPtubeID[] = {8.02, 8.0, 9.75, 12.0, 24.0, 36.0, 36.0, 36.0}; //Inner diameters of beam line tubes in inches

 for(int i=0;i<8;i++) BPtubeID[i] *= conv*cm; //convert to cm

 G4double BPtubeOD[] = {13.5, 8.25, 10.0, 13.0, 25.0, 38.0, 38.0, 38.0}; //Outer diameters of beam line tubes in inches

 for(int i=0;i<8;i++) BPtubeOD[i] *= conv*cm; //convert to cm

 G4double BPtubeLength[] = {0.75, 36.875, 12.0, 41.8125, 217.0, 216.375, 405.1875, 120.4375}; //lengths of beam line tubes in inches

 for(int i=0;i<8;i++) BPtubeLength[i] *= conv*cm; //convert to cm

 G4double BLToffset = TBLoffset + TBLlength + TBL_BLoffset + 0.0010*cm; 
 // Length of tube 7 should be cut down a little to prevent entering beam
 // dump area
 G4double diff = 0;
 for(int i=0;i<7;i++) diff += BPtubeLength[i];
 diff += BLToffset - Rhall;
 BPtubeLength[6] -= diff;



 //Beam line flange towards the center of the hall, BLT1 (Beam Line Tube 1)
 G4Tubs* solidBLT1 = new G4Tubs("BLT1",0,BPtubeOD[0]/2,BPtubeLength[0]/2,0,2*pi);
 G4LogicalVolume* logicBLT1 = new G4LogicalVolume(solidBLT1,GetMaterial("Vacuum"),"BLT1");
 X = 0;
 Y = 0; 
 Z = BLToffset + BPtubeLength[0]/2;
 G4VPhysicalVolume* physBLT1 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBLT1,"BLT1",logicWorld,false,0,checkOverlaps);
 G4VisAttributes* BLT1vis = new G4VisAttributes(1);
 BLT1vis->SetColor(0.0,1.0,0.0);
 logicBLT1->SetVisAttributes(BLT1vis);

 G4Tubs* solidBLT1W = new G4Tubs("BLT1W",BPtubeID[0]/2,BPtubeOD[0]/2,BPtubeLength[0]/2,0,2*pi);
 G4LogicalVolume* logicBLT1W = new G4LogicalVolume(solidBLT1W,GetMaterial("Aluminum"),"BLT1W");
 X = 0;
 Y = 0; 
 Z = 0;
 G4VPhysicalVolume* physBLT1W = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBLT1W,"BLT1W",logicBLT1,false,0,checkOverlaps);
 G4VisAttributes* BLT1Wvis = new G4VisAttributes(1);
 BLT1Wvis->SetColor(0.0,1.0,0.0);
 logicBLT1W->SetVisAttributes(BLT1Wvis);

//Beam Line Tube 2 (BLT2)
 G4Tubs* solidBLT2 = new G4Tubs("BLT2",0,BPtubeOD[1]/2,BPtubeLength[1]/2,0,2*pi);
 G4LogicalVolume* logicBLT2 = new G4LogicalVolume(solidBLT2,GetMaterial("Vacuum"),"BLT2");
 X = 0;
 Y = 0;
 Z = BLToffset + BPtubeLength[0] + BPtubeLength[1]/2;
 G4VPhysicalVolume* physBLT2 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBLT2,"BLT2",logicWorld,false,0,checkOverlaps);
 G4VisAttributes* BLT2vis = new G4VisAttributes(1);
 BLT2vis->SetColor(0.0,0.0,1.0);
 logicBLT2->SetVisAttributes(BLT2vis);

 G4Tubs* solidBLT2W = new G4Tubs("BLT2W",BPtubeID[1]/2,BPtubeOD[1]/2,BPtubeLength[1]/2,0,2*pi);
 G4LogicalVolume* logicBLT2W = new G4LogicalVolume(solidBLT2W,GetMaterial("Aluminum"),"BLT2W");
 X = 0;
 Y = 0; 
 Z = 0;
 G4VPhysicalVolume* physBLT2W = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBLT2W,"BLT2W",logicBLT2,false,0,checkOverlaps);
 G4VisAttributes* BLT2Wvis = new G4VisAttributes(1);
 BLT2Wvis->SetColor(0.0,0.0,1.0);
 logicBLT2W->SetVisAttributes(BLT2Wvis);

//Beam Line Tube 3 (BLT3)
 G4Tubs* solidBLT3 = new G4Tubs("BLT3",0,BPtubeOD[2]/2,BPtubeLength[2]/2,0,2*pi);
 G4LogicalVolume* logicBLT3 = new G4LogicalVolume(solidBLT3,GetMaterial("Vacuum"),"BLT3");
 X = 0;
 Y = 0;
 Z = BLToffset + BPtubeLength[0] + BPtubeLength[1] + BPtubeLength[2]/2;
 G4VPhysicalVolume* physBLT3 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBLT3,"BLT3",logicWorld,false,0,checkOverlaps);
 G4VisAttributes* BLT3vis = new G4VisAttributes(1);
 BLT3vis->SetColor(0.0,1.0,0.0);
 logicBLT3->SetVisAttributes(BLT3vis);

 G4Tubs* solidBLT3W = new G4Tubs("BLT3W",BPtubeID[2]/2,BPtubeOD[2]/2,BPtubeLength[2]/2,0,2*pi);
 G4LogicalVolume* logicBLT3W = new G4LogicalVolume(solidBLT3W,GetMaterial("Aluminum"),"BLT3W");
 X = 0;
 Y = 0; 
 Z = 0;
 G4VPhysicalVolume* physBLT3W = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBLT3W,"BLT3W",logicBLT3,false,0,checkOverlaps);
 G4VisAttributes* BLT3Wvis = new G4VisAttributes(1);
 BLT3Wvis->SetColor(0.0,1.0,0.0);
 logicBLT3W->SetVisAttributes(BLT3Wvis);

//Beam Line Tube 4 (BLT4)
 G4Tubs* solidBLT4 = new G4Tubs("BLT4",0,BPtubeOD[3]/2,BPtubeLength[3]/2,0,2*pi);
 G4LogicalVolume* logicBLT4 = new G4LogicalVolume(solidBLT4,GetMaterial("Vacuum"),"BLT4");
 X = 0;
 Y = 0;
 Z = BLToffset + BPtubeLength[0] + BPtubeLength[1] + BPtubeLength[2] + BPtubeLength[3]/2;
 G4VPhysicalVolume* physBLT4 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBLT4,"BLT4",logicWorld,false,0,checkOverlaps);
 G4VisAttributes* BLT4vis = new G4VisAttributes(1);
 BLT4vis->SetColor(1.0,1.0,0.0);
 logicBLT4->SetVisAttributes(BLT4vis);

 G4Tubs* solidBLT4W = new G4Tubs("BLT4W",BPtubeID[3]/2,BPtubeOD[3]/2,BPtubeLength[3]/2,0,2*pi);
 G4LogicalVolume* logicBLT4W = new G4LogicalVolume(solidBLT4W,GetMaterial("BLT4mat"),"BLT4W");
 X = 0;
 Y = 0; 
 Z = 0;
 G4VPhysicalVolume* physBLT4W = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBLT4W,"BLT4W",logicBLT4,false,0,checkOverlaps);
 G4VisAttributes* BLT4Wvis = new G4VisAttributes(1);
 BLT4Wvis->SetColor(1.0,1.0,0.0);
 logicBLT4W->SetVisAttributes(BLT4Wvis);

//Beam Line Tube 5 (BLT5)
 G4Tubs* solidBLT5 = new G4Tubs("BLT5",0,BPtubeOD[4]/2,BPtubeLength[4]/2,0,2*pi);
 G4LogicalVolume* logicBLT5 = new G4LogicalVolume(solidBLT5,GetMaterial("Vacuum"),"BLT5");
 X = 0;
 Y = 0;
 Z = BLToffset + BPtubeLength[0] + BPtubeLength[1] + BPtubeLength[2] + BPtubeLength[3] + BPtubeLength[4]/2;
 G4VPhysicalVolume* physBLT5 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBLT5,"BLT5",logicWorld,false,0,checkOverlaps);
 G4VisAttributes* BLT5vis = new G4VisAttributes(1);
 BLT5vis->SetColor(0.0,1.0,1.0);
 logicBLT5->SetVisAttributes(BLT5vis);

 G4Tubs* solidBLT5W = new G4Tubs("BLT5W",BPtubeID[4]/2,BPtubeOD[4]/2,BPtubeLength[4]/2,0,2*pi);
 G4LogicalVolume* logicBLT5W = new G4LogicalVolume(solidBLT5W,GetMaterial("BLT5mat"),"BLT5W");
 X = 0;
 Y = 0; 
 Z = 0;
 G4VPhysicalVolume* physBLT5W = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBLT5W,"BLT5W",logicBLT5,false,0,checkOverlaps);
 G4VisAttributes* BLT5Wvis = new G4VisAttributes(1);
 BLT5Wvis->SetColor(0.0,1.0,1.0);
 logicBLT5W->SetVisAttributes(BLT5Wvis);

//Beam Line Tube 6 (BLT6)
 G4Tubs* solidBLT6 = new G4Tubs("BLT6",0,BPtubeOD[5]/2,BPtubeLength[5]/2,0,2*pi);
 G4LogicalVolume* logicBLT6 = new G4LogicalVolume(solidBLT6,GetMaterial("Vacuum"),"BLT6");
 X = 0;
 Y = 0;
 Z = BLToffset + BPtubeLength[0] + BPtubeLength[1] + BPtubeLength[2] + BPtubeLength[3] + BPtubeLength[4] + BPtubeLength[5]/2;
 G4VPhysicalVolume* physBLT6 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBLT6,"BLT6",logicWorld,false,0,checkOverlaps);
 G4VisAttributes* BLT6vis = new G4VisAttributes(1);
 BLT6vis->SetColor(1.0,0.0,1.0);
 logicBLT6->SetVisAttributes(BLT6vis);

 G4Tubs* solidBLT6W = new G4Tubs("BLT6W",BPtubeID[5]/2,BPtubeOD[5]/2,BPtubeLength[5]/2,0,2*pi);
 G4LogicalVolume* logicBLT6W = new G4LogicalVolume(solidBLT6W,GetMaterial("BLT6mat"),"BLT6W");
 X = 0;
 Y = 0; 
 Z = 0;
 G4VPhysicalVolume* physBLT6W = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBLT6W,"BLT6W",logicBLT6,false,0,checkOverlaps);
 G4VisAttributes* BLT6Wvis = new G4VisAttributes(1);
 BLT6Wvis->SetColor(1.0,0.0,1.0);
 logicBLT6W->SetVisAttributes(BLT6Wvis);

//Beam Line Tube 7 (BLT7)
 G4Tubs* solidBLT7 = new G4Tubs("BLT7",0,BPtubeOD[6]/2,BPtubeLength[6]/2,0,2*pi);
 G4LogicalVolume* logicBLT7 = new G4LogicalVolume(solidBLT7,GetMaterial("Vacuum"),"BLT7");
 X = 0;
 Y = 0;
 Z = BLToffset + BPtubeLength[0] + BPtubeLength[1] + BPtubeLength[2] + BPtubeLength[3] + BPtubeLength[4] + BPtubeLength[5] + BPtubeLength[6]/2;
 G4VPhysicalVolume* physBLT7 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBLT7,"BLT7",logicWorld,false,0,checkOverlaps);
 G4VisAttributes* BLT7vis = new G4VisAttributes(1);
 BLT7vis->SetColor(1.0,0.0,0.0);
 logicBLT7->SetVisAttributes(BLT7vis);

 G4Tubs* solidBLT7W = new G4Tubs("BLT7W",BPtubeID[6]/2,BPtubeOD[6]/2,BPtubeLength[6]/2,0,2*pi);
 G4LogicalVolume* logicBLT7W = new G4LogicalVolume(solidBLT7W,GetMaterial("BLT7mat"),"BLT7W");
 X = 0;
 Y = 0; 
 Z = 0;
 G4VPhysicalVolume* physBLT7W = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBLT7W,"BLT7W",logicBLT7,false,0,checkOverlaps);
 G4VisAttributes* BLT7Wvis = new G4VisAttributes(1);
 BLT7Wvis->SetColor(1.0,0.0,0.0);
 logicBLT7W->SetVisAttributes(BLT7Wvis);


 // Removed the beamline parts within the dump from this function.
 // They now are pushed to the Dump specific class
 /*
//Beam Line Tube 8 (BLT8)
 G4Tubs* solidBLT8 = new G4Tubs("BLT8",0,BPtubeOD[7]/2,BPtubeLength[7]/2,0,2*pi);
 G4LogicalVolume* logicBLT8 = new G4LogicalVolume(solidBLT8,GetMaterial("Vacuum"),"BLT8");
 X = 0;
 Y = 0;
 Z = BLToffset + BPtubeLength[0] + BPtubeLength[1] + BPtubeLength[2] + BPtubeLength[3] + BPtubeLength[4] + BPtubeLength[5] + BPtubeLength[6] + BPtubeLength[7]/2;
 G4VPhysicalVolume* physBLT8 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBLT8,"BLT8",logicWorld,false,0,checkOverlaps);
 G4VisAttributes* BLT8vis = new G4VisAttributes(1);
 BLT8vis->SetColor(0.0,1.0,0.0);
 logicBLT8->SetVisAttributes(BLT8vis);

 G4Tubs* solidBLT8W = new G4Tubs("BLT8W",BPtubeID[7]/2,BPtubeOD[7]/2,BPtubeLength[7]/2,0,2*pi);
 G4LogicalVolume* logicBLT8W = new G4LogicalVolume(solidBLT8W,GetMaterial("BLT8mat"),"BLT8W");
 X = 0;
 Y = 0; 
 Z = 0;
 G4VPhysicalVolume* physBLT8W = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBLT8W,"BLT8W",logicBLT8,false,0,checkOverlaps);
 G4VisAttributes* BLT8Wvis = new G4VisAttributes(1);
 BLT8Wvis->SetColor(0.0,1.0,0.0);
 logicBLT8W->SetVisAttributes(BLT8Wvis);

//Beam Line Tube connects upto Water barrel (BWBT)
 G4double BWBT_ID = 36*conv*cm;
 G4double BWBT_OD = 38*conv*cm;
 G4double BWBT_length = 807*conv*cm;
 G4Tubs* solidBWBT = new G4Tubs("BWBT",0,BWBT_OD/2,BWBT_length/2,0,2*pi);
 G4LogicalVolume* logicBWBT = new G4LogicalVolume(solidBWBT,GetMaterial("Vacuum"),"BWBT");
 X = 0;
 Y = 0;
 Z = BLToffset + BPtubeLength[0] + BPtubeLength[1] + BPtubeLength[2] + BPtubeLength[3] + BPtubeLength[4] + BPtubeLength[5] + BPtubeLength[6] + BPtubeLength[7] + BWBT_length/2;
 G4VPhysicalVolume* physBWBT = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBWBT,"BWBT",logicWorld,false,0,checkOverlaps);
 G4VisAttributes* BWBTvis = new G4VisAttributes(1);
 BWBTvis->SetColor(0.0,1.0,0.0);
 logicBWBT->SetVisAttributes(BWBTvis);

 G4Tubs* solidBWBTW = new G4Tubs("BWBTW",BWBT_ID/2,BWBT_OD/2,BWBT_length/2,0,2*pi);
 G4LogicalVolume* logicBWBTW = new G4LogicalVolume(solidBWBTW,GetMaterial("BLT8mat"),"BWBTW");
 X = 0;
 Y = 0; 
 Z = 0;
 G4VPhysicalVolume* physBWBTW = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBWBTW,"BWBTW",logicBWBT,false,0,checkOverlaps);
 G4VisAttributes* BWBTWvis = new G4VisAttributes(1);
 BWBTWvis->SetColor(0.0,1.0,0.0);
 logicBWBTW->SetVisAttributes(BWBTWvis);
 */


 /////////////////////////////////////////////////////////////////
 ////Connection between Telescopic pipe and main fixed Beamline///
 /////////////////////////////////////////////////////////////////
 G4double FGvac1length = TBL_BLoffset; //8.285*conv*cm
 G4Cons* soildTeltoBl = new G4Cons("TeltoBl",0,13.5/2*conv*cm,0,13.5/2*conv*cm,0.5*FGvac1length,0,2*pi);
 G4LogicalVolume* logicTeltoBl = new G4LogicalVolume(soildTeltoBl,GetMaterial("Vacuum"),"TeltoBl");
 X = 0;
 Y = 0;
 Z = TBLoffset + TBLlength + FGvac1length/2;
 G4VPhysicalVolume* physTeltoBl = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicTeltoBl,"TeltoBl",logicWorld,false,0,checkOverlaps);


 ////////////////////////////////////////////////////////////////
 ///////////////////Telescoping Beam Line ////////////////////////
 ////////////////////////////////////////////////////////////////

 //Inner radius, Outer radius and Length of each tube parts
 G4double InRad[] = {2.75/2, 3.01/2, 3.26/2, 3.50/2, 3.76/2, 4.76/2, 5.76/2, 7.834/2, 7.834/2}; //in inches
 for(int i=0;i<9;i++) InRad[i] *= conv*cm;                               //convert to cm
 G4double OutRad[] = {8.50/2, 3.25/2, 3.50/2, 3.75/2, 4.0/2, 5.0/2, 6.0/2, 8.0/2, 13.5/2}; //in inches
 for(int i=0;i<9;i++) OutRad[i] *= conv*cm;                              //convert to cm
 G4double Length[] = {0.5, 10.531, 7.484, 6.0, 14.578, 9.219, 10.875, 3.22, 0.5}; //in inches
 for(int i=0;i<9;i++) Length[i] *= conv*cm;                              //convert to cm


 //Flange (Red) TBLF1 - Telescoping Beam Line Flange 1
 G4Tubs* solidTBLF1 = new G4Tubs("TBLF1",0,OutRad[0],Length[0]/2,0,2*pi);
 G4LogicalVolume* logicTBLF1 = new G4LogicalVolume(solidTBLF1,GetMaterial("Vacuum"),"TBLF1");
 X = 0;
 Y = 0; 
 Z = TBLoffset + Length[0]/2;
 G4VPhysicalVolume* physTBLF1 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicTBLF1,"TBLF1",logicWorld, false,0,checkOverlaps);
 G4VisAttributes* TBLF1vis = new G4VisAttributes(1);
 TBLF1vis->SetColor(1.0,0.0,0.0);
 logicTBLF1->SetVisAttributes(TBLF1vis);

 //Al Wall for TBLF1
 G4Tubs* solidTBLF1W = new G4Tubs("TBLF1W",InRad[0],OutRad[0],Length[0]/2,0,2*pi);
 G4LogicalVolume* logicTBLF1W = new G4LogicalVolume(solidTBLF1W,GetMaterial("Aluminum"),"TBLF1W");
 X = 0;
 Y = 0; 
 Z = 0;
 G4VPhysicalVolume* physTBLF1W = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicTBLF1W,"TBLF1W",logicTBLF1, false,0,checkOverlaps);
 G4VisAttributes* TBLF1Wvis = new G4VisAttributes(1);
 TBLF1Wvis->SetColor(1.0,0.0,0.0);
 logicTBLF1W->SetVisAttributes(TBLF1Wvis);
 
 //Pipe1 (Orange) TBLP1 - Telescoping Beam Line Pipe 1
  G4Tubs* solidTBLP1 = new G4Tubs("TBLP1",0,OutRad[1],Length[1]/2,0,2*pi);
  G4LogicalVolume* logicTBLP1 = new G4LogicalVolume(solidTBLP1,GetMaterial("Vacuum"),"TBLP1");
  X = 0;
  Y = 0;
  Z = TBLoffset + Length[0] + Length[1]/2;
  G4VPhysicalVolume* physTBLP1 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicTBLP1,"TBLP1",logicWorld, false,0,checkOverlaps);
  G4VisAttributes* TBLP1vis = new G4VisAttributes(1);
  TBLP1vis->SetColor(1.0,0.5,0.0);
  logicTBLP1->SetVisAttributes(TBLP1vis);

  //Pipe1 wall for TBLP1
  G4Tubs* solidTBLP1W = new G4Tubs("TBLP1W",InRad[1],OutRad[1],Length[1]/2,0,2*pi);
  G4LogicalVolume* logicTBLP1W = new G4LogicalVolume(solidTBLP1W,GetMaterial("Aluminum"),"TBLP1W");
  X = 0;
  Y = 0; 
  Z = 0;
  G4VPhysicalVolume* physTBLP1W = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicTBLP1W,"TBLP1W",logicTBLP1, false,0,checkOverlaps);
  G4VisAttributes* TBLP1Wvis = new G4VisAttributes(1);
  TBLP1Wvis->SetColor(1.0,0.5,0.0);
  logicTBLP1W->SetVisAttributes(TBLP1Wvis);

  //Pipe2 (Cyan) TBLP2
  G4Tubs* solidTBLP2 = new G4Tubs("TBLP2",0,OutRad[2],Length[2]/2,0,2*pi);
  G4LogicalVolume* logicTBLP2 = new G4LogicalVolume(solidTBLP2,GetMaterial("Vacuum"),"TBLP2");
  X = 0;
  Y = 0;
  Z = TBLoffset + Length[0] + Length[1] + Length[2]/2; 
  G4VPhysicalVolume* physTBLP2 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicTBLP2,"TBLP2",logicWorld, false,0,checkOverlaps);
  G4VisAttributes* TBLP2vis = new G4VisAttributes(1);
  TBLP2vis->SetColor(0.0,1.0,1.0);
  logicTBLP2->SetVisAttributes(TBLP2vis);

 //Pipe2 wall for TBLP2
  G4Tubs* solidTBLP2W = new G4Tubs("TBLP2W",InRad[2],OutRad[2],Length[2]/2,0,2*pi);
  G4LogicalVolume* logicTBLP2W = new G4LogicalVolume(solidTBLP2W,GetMaterial("Aluminum"),"TBLP2W");
  X = 0;
  Y = 0; 
  Z = 0;
  G4VPhysicalVolume* physTBLP2W = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicTBLP2W,"TBLP2W",logicTBLP2, false,0,checkOverlaps);
  G4VisAttributes* TBLP2Wvis = new G4VisAttributes(1);
  TBLP2Wvis->SetColor(0.0,1.0,1.0);
  logicTBLP2W->SetVisAttributes(TBLP2Wvis);

 //Pipe3 (Yellow) TBLP3
  G4Tubs* solidTBLP3 = new G4Tubs("TBLP3",0,OutRad[3],Length[3]/2,0,2*pi);
  G4LogicalVolume* logicTBLP3 = new G4LogicalVolume(solidTBLP3,GetMaterial("Vacuum"),"TBLP3");
  X = 0;
  Y = 0;
  Z = TBLoffset + Length[0] + Length[1] + Length[2] + Length[3]/2;
  G4VPhysicalVolume* physTBLP3 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicTBLP3,"TBLP3",logicWorld, false,0,checkOverlaps);
  G4VisAttributes* TBLP3vis = new G4VisAttributes(1);
  TBLP3vis->SetColor(1.0,1.0,0.0);
  logicTBLP3->SetVisAttributes(TBLP3vis);

 //Pipe3 wall for TBLP3
  G4Tubs* solidTBLP3W = new G4Tubs("TBLP3W",InRad[3],OutRad[3],Length[3]/2,0,2*pi);
  G4LogicalVolume* logicTBLP3W = new G4LogicalVolume(solidTBLP3W,GetMaterial("Aluminum"),"TBLP3W");
  X = 0;
  Y = 0; 
  Z = 0;
  G4VPhysicalVolume* physTBLP3W = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicTBLP3W,"TBLP3W",logicTBLP3, false,0,checkOverlaps);
  G4VisAttributes* TBLP3Wvis = new G4VisAttributes(1);
  TBLP3Wvis->SetColor(1.0,1.0,0.0);
  logicTBLP3W->SetVisAttributes(TBLP3Wvis);

 //Pipe4 (red end) TBLP4
  G4Tubs* solidTBLP4 = new G4Tubs("TBLP4",0,OutRad[4],Length[4]/2,0,2*pi);
  G4LogicalVolume* logicTBLP4 = new G4LogicalVolume(solidTBLP4,GetMaterial("Vacuum"),"TBLP4");
  X = 0;
  Y = 0;
  Z = TBLoffset + Length[0] + Length[1] + Length[2] + Length[3] + Length[4]/2;
  G4VPhysicalVolume* physTBLP4 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicTBLP4,"TBLP4",logicWorld, false,0,checkOverlaps);
  G4VisAttributes* TBLP4vis = new G4VisAttributes(1);
  TBLP4vis->SetColor(1.0,0.0,0.0);
  logicTBLP4->SetVisAttributes(TBLP4vis);

 //Pipe4 wall for TBLP4
  G4Tubs* solidTBLP4W = new G4Tubs("TBLP4W",InRad[4],OutRad[4],Length[4]/2,0,2*pi);
  G4LogicalVolume* logicTBLP4W = new G4LogicalVolume(solidTBLP4W,GetMaterial("Aluminum"),"TBLP4W");
  X = 0;
  Y = 0; 
  Z = 0;
  G4VPhysicalVolume* physTBLP4W = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicTBLP4W,"TBLP4W",logicTBLP4, false,0,checkOverlaps);
  G4VisAttributes* TBLP4Wvis = new G4VisAttributes(1);
  TBLP4Wvis->SetColor(1.0,0.0,0.0);
  logicTBLP4W->SetVisAttributes(TBLP4Wvis);

 //Pipe5 (blue end) TBLP5
  G4Tubs* solidTBLP5 = new G4Tubs("TBLP5",0,OutRad[5],Length[5]/2,0,2*pi);
  G4LogicalVolume* logicTBLP5 = new G4LogicalVolume(solidTBLP5,GetMaterial("Vacuum"),"TBLP5");
  X = 0;
  Y = 0;
  Z = TBLoffset + Length[0] + Length[1] + Length[2] + Length[3] + Length[4] + Length[5]/2;
  G4VPhysicalVolume* physTBLP5 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicTBLP5,"TBLP5",logicWorld, false,0,checkOverlaps);
  G4VisAttributes* TBLP5vis = new G4VisAttributes(1);
  TBLP5vis->SetColor(0.0,0.0,1.0);
  logicTBLP5->SetVisAttributes(TBLP5vis);

 //Pipe5 wall for TBLP5
  G4Tubs* solidTBLP5W = new G4Tubs("TBLP5W",InRad[5],OutRad[5],Length[5]/2,0,2*pi);
  G4LogicalVolume* logicTBLP5W = new G4LogicalVolume(solidTBLP5W,GetMaterial("Aluminum"),"TBLP5W");
  X = 0;
  Y = 0; 
  Z = 0;
  G4VPhysicalVolume* physTBLP5W = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicTBLP5W,"TBLP5W",logicTBLP5, false,0,checkOverlaps);
  G4VisAttributes* TBLP5Wvis = new G4VisAttributes(1);
  TBLP5Wvis->SetColor(0.0,0.0,1.0);
  logicTBLP5W->SetVisAttributes(TBLP5Wvis);

  //Pipe6 (magenta) TBLP6
  G4Tubs* solidTBLP6 = new G4Tubs("TBLP6",0,OutRad[6],Length[6]/2,0,2*pi);
  G4LogicalVolume* logicTBLP6 = new G4LogicalVolume(solidTBLP6,GetMaterial("Vacuum"),"TBLP6");
  X = 0;
  Y = 0;
  Z = TBLoffset + Length[0] + Length[1] + Length[2] + Length[3] + Length[4] + Length[5] + Length[6]/2;
  G4VPhysicalVolume* physTBLP6 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicTBLP6,"TBLP6",logicWorld, false,0,checkOverlaps);
  G4VisAttributes* TBLP6vis = new G4VisAttributes(1);
  TBLP6vis->SetColor(1.0,0.0,1.0);
  logicTBLP6->SetVisAttributes(TBLP6vis);

 //Pipe6 wall for TBLP6
  G4Tubs* solidTBLP6W = new G4Tubs("TBLP6W",InRad[6],OutRad[6],Length[6]/2,0,2*pi);
  G4LogicalVolume* logicTBLP6W = new G4LogicalVolume(solidTBLP6W,GetMaterial("Aluminum"),"TBLP6W");
  X = 0;
  Y = 0; 
  Z = 0;
  G4VPhysicalVolume* physTBLP6W = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicTBLP6W,"TBLP6W",logicTBLP6, false,0,checkOverlaps);
  G4VisAttributes* TBLP6Wvis = new G4VisAttributes(1);
  TBLP6Wvis->SetColor(1.0,0.0,1.0);
  logicTBLP6W->SetVisAttributes(TBLP6Wvis);

 //Pipe7 (dark orange) TBLP7
  G4Tubs* solidTBLP7 = new G4Tubs("TBLP7",0,OutRad[7],Length[7]/2,0,2*pi);
  G4LogicalVolume* logicTBLP7 = new G4LogicalVolume(solidTBLP7,GetMaterial("Vacuum"),"TBLP7");
  X = 0;
  Y = 0;
  Z = TBLoffset + Length[0] + Length[1] + Length[2] + Length[3] + Length[4] + Length[5] + Length[6] + Length[7]/2;
  G4VPhysicalVolume* physTBLP7 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicTBLP7,"TBLP7",logicWorld, false,0,checkOverlaps);
  G4VisAttributes* TBLP7vis = new G4VisAttributes(1);
  TBLP7vis->SetColor(1.0,0.5,0.0);
  logicTBLP7->SetVisAttributes(TBLP7vis);

 //Pipe7 wall for TBLP7
  G4Tubs* solidTBLP7W = new G4Tubs("TBLP7W",InRad[7],OutRad[7],Length[7]/2,0,2*pi);
  G4LogicalVolume* logicTBLP7W = new G4LogicalVolume(solidTBLP7W,GetMaterial("Aluminum"),"TBLP7W");
  X = 0;
  Y = 0; 
  Z = 0;
  G4VPhysicalVolume* physTBLP7W = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicTBLP7W,"TBLP7W",logicTBLP7, false,0,checkOverlaps);
  G4VisAttributes* TBLP7Wvis = new G4VisAttributes(1);
  TBLP7Wvis->SetColor(1.0,0.5,0.0);
  logicTBLP7W->SetVisAttributes(TBLP7Wvis);

 //Pipe8 (red) TBLP8
  G4Tubs* solidTBLP8 = new G4Tubs("TBLP8",0,OutRad[8],Length[8]/2,0,2*pi);
  G4LogicalVolume* logicTBLP8 = new G4LogicalVolume(solidTBLP8,GetMaterial("Vacuum"),"TBLP8");
  X = 0;
  Y = 0;
  Z = TBLoffset + Length[0] + Length[1] + Length[2] + Length[3] + Length[4] + Length[5] + Length[6] + Length[7] + Length[8]/2;
  G4VPhysicalVolume* physTBLP8 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicTBLP8,"TBLP8",logicWorld, false,0,checkOverlaps);
  G4VisAttributes* TBLP8vis = new G4VisAttributes(1);
  TBLP8vis->SetColor(1.0,0.0,0.0);
  logicTBLP8->SetVisAttributes(TBLP8vis);

 //Pipe8 wall for TBLP8
  G4Tubs* solidTBLP8W = new G4Tubs("TBLP8W",InRad[8],OutRad[8],Length[8]/2,0,2*pi);
  G4LogicalVolume* logicTBLP8W = new G4LogicalVolume(solidTBLP8W,GetMaterial("Aluminum"),"TBLP8W");
  X = 0;
  Y = 0; 
  Z = 0;
  G4VPhysicalVolume* physTBLP8W = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicTBLP8W,"TBLP8W",logicTBLP8, false,0,checkOverlaps);
  G4VisAttributes* TBLP8Wvis = new G4VisAttributes(1);
  TBLP8Wvis->SetColor(1.0,0.0,0.0);
  logicTBLP8W->SetVisAttributes(TBLP8Wvis);


  /////////////////////////////////////////////////////////////////
  ////Connection between Telescopic pipe and magnet pipe///////////
  /////////////////////////////////////////////////////////////////
  G4double FGvac2length = 5.1332*cm;
  G4Tubs* soildTeltoMag = new G4Tubs("TeltoMag",0,InRad[0],FGvac2length/2,0,2*pi);
  G4LogicalVolume* logicTeltoMag = new G4LogicalVolume(soildTeltoMag,GetMaterial("Vacuum"),"TeltoMag");
  X = 0;
  Y = 0;
  Z = TBLoffset - FGvac2length/2;
  G4VPhysicalVolume* physTeltoMag = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicTeltoMag,"TeltoMag",logicWorld,false,0,checkOverlaps);


 /////////////////////////////////////////////////////////////////
 //////Connection between magnet pipe and Extension box///////////
 /////////////////////////////////////////////////////////////////

  G4double FGvac3length = (175 - 57.15 - conv*40.365)*cm; //Need dimensions of distance between SC and Septa, SC diameter, Ext. Box length and septa z length
  G4Tubs* soildExttoMag = new G4Tubs("ExttoMag",0,InRad[0],FGvac3length/2,0,2*pi);
  G4LogicalVolume* logicExttoMag = new G4LogicalVolume(soildExttoMag,GetMaterial("Vacuum"),"ExttoMag");
  X = 0;
  Y = 0;
  Z = 70*cm - 22.5*conv*cm - FGvac3length/2;
  G4VPhysicalVolume* physExttoMag = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicExttoMag,"ExttoMag",logicWorld,false,0,checkOverlaps);


}

G4Material* Beamline::GetMaterial(G4String mat){

  return fDetCon->GetMaterial(mat);

}

Beamline::~Beamline(){;}
