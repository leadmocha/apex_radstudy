#include "APEXExtBox.hh"
#include "B1DetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4RotationMatrix.hh"
#include "G4VisAttributes.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Trd.hh"

#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"


APEXExtBox::APEXExtBox(B1DetectorConstruction *dc, HallA *ha, G4LogicalVolume *logicWorld):fDetCon(dc){

  const G4double conv = 2.54;
  G4double par[10];
  G4double X,Y,Z;
  
  G4bool checkOverlaps = true;

  ////////////////////////////////////////////////////////////////////////
  /////////////////////////APEX Extension Box/////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  //APEX Extension Box Container
  G4double AEBCXlength = 25.6*conv*cm;
  G4double AEBCYlength = 11.81*conv*cm;
  G4double AEBCZlength = (6.45 + 9.125 + 1.5 + 0.79)*conv*cm;
  par[1] = 0.5*AEBCXlength;
  par[2] = 0.5*AEBCYlength;
  par[3] = 0.5*AEBCZlength;
  G4Box* solidAEBC = new G4Box("AEBC", par[1], par[2], par[3]);

  G4LogicalVolume* logicAEBC = new G4LogicalVolume(solidAEBC, GetMaterial("Vacuum"),"AEBC");

  G4VisAttributes* AEBCvis = new G4VisAttributes(0);
  AEBCvis->SetForceWireframe(true);
  AEBCvis->SetColor(0.0,1.0,0.5);
  logicAEBC->SetVisAttributes(AEBCvis);
  X = 0;
  Y = 0;
  Z = -105*cm + 57.15*cm + AEBCZlength/2;
  G4VPhysicalVolume* physAEBC = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicAEBC,"AEBC",logicWorld,false,0,checkOverlaps);

  //Extension Box Target Flange
  G4double EBTFXlength = 19.84*conv*cm;
  G4double EBTFYlength = 11.81*conv*cm;
  G4double EBTFZlength = 0.75*conv*cm;
  par[1] = 0.5*EBTFXlength;
  par[2] = 0.5*EBTFYlength;
  par[3] = 0.5*EBTFZlength;
  G4Box* solidEBTF = new G4Box("EBTF", par[1], par[2], par[3]);

  //Cut from Ext. Box Target Flange
  G4double EBTFCutXlength = 15*conv*cm;
  G4double EBTFCutYlength = 7*conv*cm;
  G4double EBTFCutZlength = 0.8*conv*cm;
  par[1] = 0.5*EBTFCutXlength;
  par[2] = 0.5*EBTFCutYlength;
  par[3] = 0.5*EBTFCutZlength;
  G4Box* solidEBTFCut = new G4Box("EBTFCut", par[1], par[2], par[3]);
  X = 0;
  Y = 0;
  Z = 0;
  G4SubtractionSolid* solidEBTF_Cut = new G4SubtractionSolid("EBTF-Cut",solidEBTF, solidEBTFCut,0,G4ThreeVector(X,Y,Z));
  G4LogicalVolume* logicEBTF = new G4LogicalVolume(solidEBTF_Cut, GetMaterial("Iron"),"EBTF");

  X = 0;
  Y = 0;
  Z = -0.5*AEBCZlength + 0.5*EBTFZlength;

  G4VisAttributes* EBTFvis = new G4VisAttributes(1);
  EBTFvis->SetForceWireframe(false);
  EBTFvis->SetColor(0.5,0.0,0.5);
  logicEBTF->SetVisAttributes(EBTFvis);
  G4VPhysicalVolume* physEBTF = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicEBTF,"EBTF",logicAEBC,false,0,checkOverlaps);

  //Extension box tube
  G4double EBTXlength = 15.5*conv*cm;
  G4double EBTYlength = 7.5*conv*cm;
  G4double EBTZlength = 5.45*conv*cm;
  par[1] = 0.5*EBTXlength;
  par[2] = 0.5*EBTYlength;
  par[3] = 0.5*EBTZlength;
  G4Box* solidEBT = new G4Box("EBT", par[1], par[2], par[3]);

  //Cut from Ext. Box Tube
  G4double EBTCutXlength = 15*conv*cm;
  G4double EBTCutYlength = 7*conv*cm;
  G4double EBTCutZlength = 5.5*conv*cm;
  par[1] = 0.5*EBTCutXlength;
  par[2] = 0.5*EBTCutYlength;
  par[3] = 0.5*EBTCutZlength;
  G4Box* solidEBTCut = new G4Box("EBTCut", par[1], par[2], par[3]);
  X = 0;
  Y = 0;
  Z = 0;
  G4SubtractionSolid* solidEBT_Cut = new G4SubtractionSolid("EBT-Cut",solidEBT, solidEBTCut,0,G4ThreeVector(X,Y,Z));
  G4LogicalVolume* logicEBT = new G4LogicalVolume(solidEBT_Cut, GetMaterial("Iron"),"EBT");

  X = 0;
  Y = 0;
  Z = -0.5*AEBCZlength + EBTFZlength + 0.5*EBTZlength;
  G4VisAttributes* EBTvis = new G4VisAttributes(1);
  EBTvis->SetForceWireframe(false);
  EBTvis->SetColor(0.0,1.0,1.0);
  logicEBT->SetVisAttributes(EBTvis);
  G4VPhysicalVolume* physEBT = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicEBT,"EBT",logicAEBC,false,0,checkOverlaps);

  //Upstream Extension box plate
  G4double UEBPXlength = 25.1*conv*cm;
  G4double UEBPYlength = 9.5*conv*cm;
  G4double UEBPZlength = 0.25*conv*cm;
  par[1] = 0.5*UEBPXlength;
  par[2] = 0.5*UEBPYlength;
  par[3] = 0.5*UEBPZlength;
  G4Box* solidUEBP = new G4Box("UEBP", par[1], par[2], par[3]);

  //Cut from Upstream Extension box plate
  G4double UEBPCutXlength = 15*conv*cm;
  G4double UEBPCutYlength = 7*conv*cm;
  G4double UEBPCutZlength = 0.3*conv*cm;
  par[1] = 0.5*UEBPCutXlength;
  par[2] = 0.5*UEBPCutYlength;
  par[3] = 0.5*UEBPCutZlength;
  G4Box* solidUEBPCut = new G4Box("UEBPCut", par[1], par[2], par[3]);
  X = 0;
  Y = 0;
  Z = 0;
  G4SubtractionSolid* solidUEBP_Cut = new G4SubtractionSolid("UEBP-Cut",solidUEBP, solidUEBPCut,0,G4ThreeVector(X,Y,Z));
  G4LogicalVolume* logicUEBP = new G4LogicalVolume(solidUEBP_Cut, GetMaterial("Iron"),"UEBP");

  X = 0;
  Y = 0;
  Z = -0.5*AEBCZlength + EBTFZlength + EBTZlength + 0.5*UEBPZlength;
  G4VisAttributes* UEBPvis = new G4VisAttributes(1);
  UEBPvis->SetForceWireframe(false);
  UEBPvis->SetColor(1.0,1.0,0.0);
  logicUEBP->SetVisAttributes(UEBPvis);
  G4VPhysicalVolume* physUEBP = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicUEBP,"UEBP",logicAEBC,false,0,checkOverlaps);

  //Tob/Bottom Extension box plate
  G4double EBTBXlength = 25.1*conv*cm;
  G4double EBTBYlength = 0.25*conv*cm;
  G4double EBTBZlength = 9.125*conv*cm;
  par[1] = 0.5*EBTBXlength;
  par[2] = 0.5*EBTBYlength;
  par[3] = 0.5*EBTBZlength;
  G4Box* solidEBTB = new G4Box("EBTB", par[1], par[2], par[3]);
  G4LogicalVolume* logicEBTB = new G4LogicalVolume(solidEBTB, GetMaterial("Iron"),"EBTB");
  X = 0;
  Y = 4.75*conv*cm - 0.5*EBTBYlength;
  Z = -0.5*AEBCZlength + EBTFZlength + EBTZlength + UEBPZlength + 0.5*EBTBZlength;
  G4VisAttributes* EBTBvis = new G4VisAttributes(1);
  EBTBvis->SetForceWireframe(false);
  EBTBvis->SetColor(0.0,1.0,0.8);
  logicEBTB->SetVisAttributes(EBTBvis);
  G4VPhysicalVolume* physEBTBT = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicEBTB,"EBTBT",logicAEBC,false,0,checkOverlaps);

  X = 0;
  Y = -4.75*conv*cm + 0.5*EBTBYlength;
  Z = -0.5*AEBCZlength + EBTFZlength + EBTZlength + UEBPZlength + 0.5*EBTBZlength;
  G4VPhysicalVolume* physEBTBB = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicEBTB,"EBTBB",logicAEBC,false,0,checkOverlaps);


  //Side Extension box plate
  G4double SEBPXlength = 0.25*conv*cm;
  G4double SEBPYlength = 9.5*conv*cm;
  G4double SEBPZlength = 9.375*conv*cm;
  par[1] = 0.5*SEBPXlength;
  par[2] = 0.5*SEBPYlength;
  par[3] = 0.5*SEBPZlength;
  G4Box* solidSEBP = new G4Box("SEBP", par[1], par[2], par[3]);
  G4LogicalVolume* logicSEBP = new G4LogicalVolume(solidSEBP, GetMaterial("Iron"),"SEBP");
  X = -0.5*UEBPXlength - 0.5*SEBPXlength;
  Y = 4.75*conv*cm - 0.5*SEBPYlength;
  Z = -0.5*AEBCZlength + EBTFZlength + EBTZlength + 0.5*UEBPZlength + 0.5*EBTBZlength;
  G4VisAttributes* SEBPvis = new G4VisAttributes(1);
  SEBPvis->SetForceWireframe(false);
  SEBPvis->SetColor(0.0,1.0,0.8);
  logicSEBP->SetVisAttributes(SEBPvis);
  G4VPhysicalVolume* physSEBPR = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicSEBP,"SEBPR",logicAEBC,false,0,checkOverlaps);

  X = 0.5*UEBPXlength + 0.5*SEBPXlength;
  Y = 4.75*conv*cm - 0.5*SEBPYlength;
  Z = -0.5*AEBCZlength + EBTFZlength + EBTZlength + 0.5*UEBPZlength + 0.5*EBTBZlength;
  G4VPhysicalVolume* physSEBPL = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicSEBP,"SEBPL",logicAEBC,false,0,checkOverlaps);


  //////////////////////////////////////////////////////////////////////////
  ///////////////////////Exit box flange////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////

  G4Box* exitBox_flange_part1 = new G4Box("exitBox_flange_part1",0.5*25.1*conv*cm,0.5*9.5*conv*cm,0.5*0.5*conv*cm);
  G4Box* exitBox_flange_part2 = new G4Box("exitBox_flange_part2",0.5*15.5*conv*cm,0.5*9.5*conv*cm,0.5*0.5*conv*cm);
  G4Trd* exitBox_flange_part3 = new G4Trd("exitBox_flange_part3",0.5*14.23*conv*cm,0.5*2.8*conv*cm,0.5*9.5*conv*cm,0.5*9.5*conv*cm,0.5*0.5*conv*cm);
  G4UnionSolid* exitBox_flange_union1 = new G4UnionSolid("part1+part2",exitBox_flange_part1,exitBox_flange_part2,0,G4ThreeVector(0,0,0.5*conv*cm)); 
  G4UnionSolid* exitBox_flange_union2 = new G4UnionSolid("union1+part3",exitBox_flange_union1,exitBox_flange_part3,0,G4ThreeVector(0,0,1.0*conv*cm));
  G4Tubs* exitBox_flange_part4= new G4Tubs("exitBox_flange_part4",0,1.25*conv*cm,1.6*conv*cm,0,2*pi);
  G4SubtractionSolid* exitBox_flange_subtract1 = new G4SubtractionSolid("union2-part4",exitBox_flange_union2,exitBox_flange_part4,0,G4ThreeVector(0,0,0));
  G4Box* exitBox_flange_part5 = new G4Box("exitBox_flange_part5",0.5*3.15*conv*cm,0.5*4.88*conv*cm,0.5*3*conv*cm);
  G4RotationMatrix* exitBox_flange_part5_Rot1 = new G4RotationMatrix();
  exitBox_flange_part5_Rot1->rotateY(5*deg);
  G4SubtractionSolid* exitBox_flange_subtract2 = new G4SubtractionSolid("subtract1-part5",exitBox_flange_subtract1,exitBox_flange_part5,exitBox_flange_part5_Rot1,G4ThreeVector(-3.966*conv*cm,0,0));
  G4RotationMatrix* exitBox_flange_part5_Rot2 = new G4RotationMatrix();
  exitBox_flange_part5_Rot2->rotateY(-5*deg);
  G4SubtractionSolid* exitBox_flange = new G4SubtractionSolid("subtract2-part5",exitBox_flange_subtract2,exitBox_flange_part5,exitBox_flange_part5_Rot2,G4ThreeVector(3.966*conv*cm,0,0));
  G4LogicalVolume* exitBox_flange_logic = new G4LogicalVolume(exitBox_flange,GetMaterial("Iron"),"exitBox_flange");
  X = 0;
  Y = 0;
  Z = -0.5*AEBCZlength + EBTFZlength + EBTZlength + UEBPZlength + EBTBZlength + 0.5*0.5*conv*cm;
  G4VPhysicalVolume* exitBox_flange_phys = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),exitBox_flange_logic,"exitBox_flange",logicAEBC, false,0,checkOverlaps);
  G4VisAttributes* exitBox_flangevis = new G4VisAttributes(1);
  exitBox_flangevis->SetColor(1.0,1.0,0.0);
  exitBox_flange_logic-> SetVisAttributes(exitBox_flangevis);

  //Cut Tube
  G4Tubs* cutTube_part1 = new G4Tubs("cutTube_part1",1.185*conv*cm,1.25*conv*cm,8.9325*conv*cm,0,2*pi);
  G4Box* cutTube_part2 = new G4Box("cutTube_part2",0.5*2.51*conv*cm,0.5*1.38*conv*cm,0.5*5.365*conv*cm);
  G4SubtractionSolid* cutTube = new G4SubtractionSolid("cutTube_part1-cutTube_part2",cutTube_part1,cutTube_part2,0,G4ThreeVector(0,0, -6.255*conv*cm));
  G4LogicalVolume* cutTube_logic = new G4LogicalVolume(cutTube,GetMaterial("Iron"),"cutTube");
  X = 0;
  Y = 0;
  Z = 0.5*AEBCZlength - 8.9325*conv*cm;
  G4VPhysicalVolume* cutTube_phys = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),cutTube_logic,"cutTube",logicAEBC, false,0,checkOverlaps);
  G4VisAttributes* cutTubevis = new G4VisAttributes(1);
  cutTubevis->SetColor(1.0,1.0,0.0);
  cutTube_logic-> SetVisAttributes(cutTubevis);


}


G4Material* APEXExtBox::GetMaterial(G4String mat){

  return fDetCon->GetMaterial(mat);

}


APEXExtBox::~APEXExtBox(){;}
