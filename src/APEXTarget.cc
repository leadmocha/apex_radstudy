#include "APEXTarget.hh"
#include "RadConfig.hh"
#include "B1DetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4RotationMatrix.hh"
#include "G4VisAttributes.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4Tubs.hh"
#include "G4Box.hh"

#include "G4SubtractionSolid.hh"


APEXTarget::APEXTarget(B1DetectorConstruction *dc, HallA *ha, G4LogicalVolume *logicWorld):fDetCon(dc){

const G4double conv = 2.54;
G4double par[10];
G4double X,Y,Z;

//Rotation Matrices
 G4RotationMatrix* rot90X = new G4RotationMatrix();
 rot90X->rotateX(90*deg);

 G4bool checkOverlaps = true;

 ////////////////////////////////////////////////////////////////////////
 /////////////////////////Scattering Chamber/////////////////////////////
 ////////////////////////////////////////////////////////////////////////
 
 G4double SCZ = 105*cm; //distance between SC and pivot point

 //Outer shell
 G4double SCID = 104.14*cm;
 G4double SCOD = 114.3*cm;
 G4double SCheight = 121.92*cm + 2*conv*cm; //extra 2 inches for top/bottom covers

 par[1] = 0;
 par[2] = SCOD/2;
 par[3] = SCheight/2;

 G4Tubs* solidSCOS_vac = new G4Tubs("SCOS_vac",par[1],par[2],par[3],0,2*pi);

 G4LogicalVolume* logicSCOS_vac = new G4LogicalVolume(solidSCOS_vac, GetMaterial("Vacuum"), "SCOS_vac");
 X = 0;
 Y = 0;
 Z = -SCZ;
 G4VPhysicalVolume* physSCOS_vac = new G4PVPlacement(rot90X,G4ThreeVector(X,Y,Z),logicSCOS_vac,"SCOS_vac",logicWorld,false,0,checkOverlaps);
 G4VisAttributes* SCOS_vacvis = new G4VisAttributes(0);
 SCOS_vacvis->SetForceWireframe(false);
 logicSCOS_vac->SetVisAttributes(SCOS_vacvis);

 //Aluminum cover
 SCheight = 121.92*cm;
 par[1] = SCID/2;
 par[2] = SCOD/2;
 par[3] = SCheight/2;

 G4Tubs* solidSCOS = new G4Tubs("SCOS",par[1],par[2],par[3],0,2*pi);

 //Cut hole for upstream beam tube
 G4double SCCut1_OD = 6*cm;
 G4double SCCut1_length = 10.16*cm;
 par[1] = 0;
 par[2] = SCCut1_OD/2;
 par[3] = SCCut1_length/2;
 G4Tubs* solidSCCut1 = new G4Tubs("SCCut1",par[1],par[2],par[3],0,2*pi);
 X = 0;
 Y = SCOD/2;
 Z = 0;
 G4SubtractionSolid* solidSC_Cut1 = new G4SubtractionSolid("SCOS-Cut1",solidSCOS, solidSCCut1,rot90X,G4ThreeVector(X,Y,Z));

 //Cut hole for outgoing particles
 G4double SCCut2Xlength = 10.412*conv*cm;
 G4double SCCut2Ylength = 6.690*conv*cm;
 G4double SCCut2Zlength = 10.16*cm;
 par[1] = 0.5*SCCut2Xlength;
 par[2] = 0.5*SCCut2Zlength; //X,Y switched because of rotation
 par[3] = 0.5*SCCut2Ylength;
 G4Box* solidSCCut2 = new G4Box("SCCut2", par[1], par[2], par[3]);
 X = 0;
 Y = -SCOD/2;
 Z = 0;
 G4SubtractionSolid* solidSC_Cut2 = new G4SubtractionSolid("SCOS-Cut2",solidSC_Cut1, solidSCCut2,0,G4ThreeVector(X,Y,Z));

 G4LogicalVolume* logicSCOS = new G4LogicalVolume(solidSC_Cut2, GetMaterial("Aluminum"), "SCOS");
 X = 0;
 Y = 0;
 Z = 0;
 G4VPhysicalVolume* physSCOS = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicSCOS,"SCOS",logicSCOS_vac,false,0,checkOverlaps);
 G4VisAttributes* SCOSvis = new G4VisAttributes(1);
 SCOSvis->SetForceWireframe(false);
 SCOSvis->SetColor(0.0,0.5,0.5);
 logicSCOS->SetVisAttributes(SCOSvis);

 //Inside Vacuum
 par[1] = 0;
 par[2] = SCID/2;
 par[3] = SCheight/2;
 G4Tubs* solidSCIV = new G4Tubs("SCIV",par[1],par[2],par[3],0,2*pi);

 G4LogicalVolume* logicSCIV = new G4LogicalVolume(solidSCIV, GetMaterial("Vacuum"),"SCIV");
 X = 0;
 Y = 0;
 Z = 0;
 G4VPhysicalVolume* physSCIV = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicSCIV,"SCIV",logicSCOS_vac,false,0,checkOverlaps);
 G4VisAttributes* SCIVvis = new G4VisAttributes(1);
 SCIVvis->SetForceWireframe(false);
 SCIVvis->SetColor(0.5,0.5,0.5,0.1);
 logicSCIV->SetVisAttributes(SCIVvis);

 //Top/Bottom covers
 G4double TBCoverOD = 114.3*cm;
 G4double TBCoverthk = 1*conv*cm;
 par[1] = 0;
 par[2] = 0.5*TBCoverOD;
 par[3] = 0.5*TBCoverthk/2;
 G4Tubs* solidTBCover = new G4Tubs("TBCover",par[1],par[2],par[3],0,2*pi);
 G4LogicalVolume* logicTBCover = new G4LogicalVolume(solidTBCover, GetMaterial("Iron"), "TBCover");
 X = 0;
 Y = 0;
 Z = SCheight/2 + TBCoverthk/2;
 G4VisAttributes* TBCovervis = new G4VisAttributes(1);
 TBCovervis->SetColor(0.0,0.2,0.2);
 logicTBCover->SetVisAttributes(TBCovervis);
 G4VPhysicalVolume* physTCover = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicTBCover,"TCover",logicSCOS_vac,false,0,checkOverlaps);
 X = 0;
 Y = 0;
 Z = -SCheight/2 - TBCoverthk/2;
 G4VPhysicalVolume* physBCover = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicTBCover,"BCover",logicSCOS_vac,false,0,checkOverlaps);

 //Fill Gap between SC and Ext. Box
 G4double FGSEXlength = 56.5*cm;
 G4double FGSEYlength = 25*cm;
 G4double FGSEZlength = 5*cm;
 par[1] = 0.5*FGSEXlength;
 par[2] = 0.5*FGSEYlength;
 par[3] = 0.5*FGSEZlength;
 G4Box* solidFGSE = new G4Box("FGSE", par[1], par[2], par[3]);
 par[1] = 0;
 par[2] = 0.5*SCOD;
 par[3] = 0.5*FGSEYlength;
 G4Tubs* solidFGSECut = new G4Tubs("FGSECut",par[1],par[2],par[3],0,2*pi);
 X = 0;
 Y = 0;
 Z = -0.5*SCOD + 0.5*FGSEZlength;
 G4SubtractionSolid* solidFGSE_Cut = new G4SubtractionSolid("FGSE_Cut",solidFGSE, solidFGSECut,rot90X,G4ThreeVector(X,Y,Z));
 G4LogicalVolume* logicFGSE = new G4LogicalVolume(solidFGSE_Cut, GetMaterial("Vacuum"),"FGSE");
 X = 0;
 Y = 0;
 Z = -SCZ - FGSEZlength/2 + SCOD/2;
 G4VPhysicalVolume* physFGSE = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicFGSE,"FGSE",logicWorld,false,0,checkOverlaps);
 G4VisAttributes* FGSEvis = new G4VisAttributes(1);
 FGSEvis->SetForceWireframe(false);
 FGSEvis->SetColor(0.0,0.5,0.5);
 logicFGSE->SetVisAttributes(FGSEvis);

 //Add Aluminum cover
 X = 0;
 Y = 0;
 Z = 0;
 G4SubtractionSolid* solidFGSE_Al_Cut = new G4SubtractionSolid("FGSE_Al_Cut",solidFGSE_Cut, solidSCCut2,0,G4ThreeVector(X,Y,Z));
 G4LogicalVolume* logicFGSE_Al = new G4LogicalVolume(solidFGSE_Al_Cut, GetMaterial("Aluminum"),"FGSE_Al");
 X = 0;
 Y = 0;
 Z = 0;
 G4VPhysicalVolume* physFGSE_Al = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicFGSE_Al,"FGSE_Al",logicFGSE,false,0,checkOverlaps);

 /////////////////////////////////////////////////////////////////////////
 //////////////////////APEX W Target GeV////////////////////////////
 /////////////////////////////////////////////////////////////////////////

 G4double targetWidth = 2.5*mm;
 G4double tungstenRadiationLength = 0.3504*CLHEP::cm; // From PDG
 //G4double targetThick = 0.001855*cm;
 G4double targetTotalThick = gRadConfig->TargetThickRL*tungstenRadiationLength;
 // The specified RL thickness corresponds to the thickness of all ribbons.
 // There are 10 ribbons, so divide by 10 to get the thickness of
 // each individual ribbon.
 G4double targetThick = targetTotalThick/10.;
 // Apparently it's only a 4%r.l. target for 2.2 GeV
 //if(abs(gRadConfig->BeamEnergy - 2.2*GeV) < 0.1*GeV) {
 //  targetThick = 0.00098*cm;
 //  targetRL = 4;
 //}
 G4cout << "APEX Tungsten target (total) thickness: " << targetTotalThick/CLHEP::mm << " mm ("
   << gRadConfig->TargetThickRL/CLHEP::perCent << "% r.l.) for beam energy of "
   << gRadConfig->BeamEnergy/CLHEP::GeV << " GeV" << G4endl;
 //G4cout << "Target Radiation length set to ~" << targetRL << "% for beam "
 //  << " energy of " << gRadConfig->BeamEnergy/CLHEP::GeV << " GeV" << G4endl;
 G4double targetHeight = 8*cm;
 G4double targetGap = 5.5*cm;
 G4double TBoxlength = targetGap*9 + targetThick;
 G4String TName[] = {"TWire1", "TWire2", "TWire3", "TWire4", "TWire5", "TWire6", "TWire7", "TWire8", "TWire9", "TWire10"};

 G4Box* solidTWire = new G4Box("Twire",targetWidth/2,targetHeight/2,targetThick/2);
 G4LogicalVolume* logicTWire = new G4LogicalVolume(solidTWire,GetMaterial("Tungsten"),"TWire");

 for(G4int i=0; i<10; i++){

   X = 0;
   Y = -TBoxlength/2 + targetThick/2 + i*targetGap;
   Z = 0;
   G4ThreeVector Tpos = G4ThreeVector(X,Y,Z);

   G4VPhysicalVolume* physTWire = new G4PVPlacement(rot90X,Tpos,logicTWire,TName[i],logicSCIV, false,0,checkOverlaps);
 }

}


G4Material* APEXTarget::GetMaterial(G4String mat){

  return fDetCon->GetMaterial(mat);

}


APEXTarget::~APEXTarget(){;}
