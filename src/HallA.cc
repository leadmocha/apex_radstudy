#include "HallA.hh"
#include "RadConfig.hh"
#include "G4NistManager.hh"
#include "B1DetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4Sphere.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"

#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"

#include "Beamline.hh"
#include "BeamDump.hh"
#include "SeptaMagnet.hh"
#include "APEXTarget.hh"
#include "G4GDMLParser.hh"

#include "G4VisAttributes.hh"

#include "math.h"

#include "G4SDManager.hh"
#include "GenericSD.hh"
#include "UpstreamDetSD.hh"

#include "HRS.hh"
#include "APEXExtBox.hh"

#include "Cradle.hh"
#include "Gantry.hh"
#include "HRSLink.hh"

#include "RadBox1SD.hh"
#include "RadBox2SD.hh"

HallA::HallA(B1DetectorConstruction *dc, G4LogicalVolume *logicWorld):fDetCon(dc){

  G4bool checkOverlaps = true;

  const G4double conv = 2.54;

  G4double Tgrnd = 100*m;
  G4double Rspace = 2000*m;
  bmheight = 304.8*cm; //beam height 10 feet off the ground
  Rhall = 2651.76*cm;
  G4double Flrdeep = 1630*cm; //1051.56*cm;
  G4double Flrthick = 200*cm;
  G4double Thall = 40*cm;
  G4double TThall = 121.9*cm;


  G4double BPdiamIn = conv*2.4*cm;  //Beampipe diameters
  G4double BPdiamOut = conv*3*cm;

  BPAirTubeLength = 1050.4375*conv*cm; //Beam Pipe container tube length

  G4double SCdiamIn = 104.14*cm;
  G4double SCdiamOut = 114.3*cm;
  G4double SCheight = 99.1*cm;
  SCZ = 105*cm; //position of scattering chamber


  HBdiam = 189.22*cm;

  G4double par[10];
  G4double X,Y,Z;

  //Rotation Matrices
  G4RotationMatrix* rot90X = new G4RotationMatrix();
  rot90X->rotateX(90*deg);
  G4RotationMatrix* rot270X = new G4RotationMatrix();
  rot270X->rotateX(270*deg);
  G4RotationMatrix* rot180Z = new G4RotationMatrix();
  rot180Z->rotateZ(180*deg);
  G4RotationMatrix* rotTmp = 0;

  /////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////Beam Dump/////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  G4LogicalVolume *logicBeamDump = 0;
  G4ThreeVector originBeamDump;
  G4ThreeVector originBeamDumpLocal;
  BeamDump *beamDump = new BeamDump(dc,this);
  G4VSolid *solidBeamDump = 0;
  G4bool beamDumpOrigVersion = true;
  if( gRadConfig->BuildDetector("BeamDump") ) {
    if( gRadConfig->BeamDumpVersion == RadBeamDumpVersion::UPDATEDVERSION ) {
      logicBeamDump = beamDump->ConstructBeamDump();
      originBeamDumpLocal = beamDump->getLocalOrigin();
      originBeamDump = originBeamDumpLocal + G4ThreeVector(0.,0.,Rhall);
      G4VPhysicalVolume *physEnclosure = new G4PVPlacement(0,originBeamDump,
      logicBeamDump,"BeamDumpWorld",logicWorld,false,0,checkOverlaps);
      solidBeamDump = logicBeamDump->GetSolid();
      beamDumpOrigVersion = false;
    } else {
      beamDump->ConstructBeamDumpOrig(logicWorld);
    }
  }
  BmDEX  = beamDump->getBmDEX();
  BmDEY  = beamDump->getBmDEY();
  BmDEZ  = beamDump->getBmDEZ();
  G4double BmDmX  = beamDump->getBmDmX();
  BmDmY  = beamDump->getBmDmY();
  G4double BmDmZ  = beamDump->getBmDmZ();
  G4double BDWth  = beamDump->getBDWth();
  G4double BDWthz = beamDump->getBDWthz();
  G4double TgX    = beamDump->getTgX();


  ///////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////Ground/////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////

  G4double Ground = Tgrnd;
  par[1] = Rspace;
  par[2] = Ground/2;
  par[3] = Rspace;
  G4VSolid* solidGrnd = 0;
  G4Box* solidGrnd1 =  new G4Box("Grnd",par[1],par[2],par[3]);
  // May need to remove part of the ground so the beam dump fits
  if(solidBeamDump) {
    solidGrnd = new G4SubtractionSolid("GrndCut",
        solidGrnd1,solidBeamDump,0,
        originBeamDump+G4ThreeVector(0.,par[2]+bmheight,0.));
  } else {
    solidGrnd = solidGrnd1;
  }
  G4LogicalVolume* logicGrnd = new G4LogicalVolume(solidGrnd,GetMaterial("Ground"),"Grnd");
  X = 0;
  Y = -bmheight - Ground/2;
  Z = 0;
  G4VPhysicalVolume* physGrnd = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicGrnd,"Grnd",logicWorld, false,0,checkOverlaps);
  G4VisAttributes* Grndvis = new G4VisAttributes(0);
  Grndvis->SetForceWireframe(false);
  logicGrnd->SetVisAttributes(Grndvis);

  //////////////////Hall A Concrete Floor inside Ground (HACF)//////////////////

  par[1] = 0;
  par[2] = Rhall + Thall;
  par[3] = Flrthick/2;
  G4VSolid *solidHACF = 0;
  G4Tubs* solidHACF1 = new G4Tubs("HACF",par[1],par[2],par[3],0,2*pi);
  // May need to remove part of the floor so the beam dump fits
  if(solidBeamDump) {
    solidHACF = new G4SubtractionSolid("HACFCut",
        solidHACF1,solidBeamDump,rot270X,
        G4ThreeVector(originBeamDump.x(),-originBeamDump.z(),
            originBeamDump.y() + bmheight + Flrthick/2.));
  } else {
    solidHACF = solidHACF1;
  }
  G4LogicalVolume* logicHACF = new G4LogicalVolume(solidHACF,GetMaterial("Concrete"),"HACF");
  X = 0;
  Y = Ground/2 - Flrthick/2;
  Z = 0;
  G4VPhysicalVolume* physHACF = new G4PVPlacement(rot90X,G4ThreeVector(X,Y,Z),logicHACF,"HACF",logicGrnd, false,0,checkOverlaps);
  G4VisAttributes* HACFvis = new G4VisAttributes(1);
  HACFvis->SetColor(0.5,0.5,0.5);
  logicHACF->SetVisAttributes(HACFvis);


  /////////////////////////////////////////////////////////////////////////////
  ////////////////////Concrete Wall around the hall////////////////////////////
  /////////////////////////////////////////////////////////////////////////////

  par[1] = Rhall;
  par[2] = Rhall + Thall;
  par[3] = Flrdeep/2;
  G4Tubs* solidConWall = new G4Tubs("ConWall",par[1],par[2],par[3],0,2*pi);
  if(!solidBeamDump) {
    //Remove material for beam dump from Concrete Wall
    par[1] = BmDEX/2 + BDWth;
    par[2] = BmDEY/2 + BDWth;
    par[3] = BmDEZ/2 + 10*cm; //Extra 10*cm was added to make sure it cuts through
    solidBeamDump = new G4Box("BD1",par[1],par[2],par[3]);
    X = 0;
    Y = -(sqrt(pow(Rhall,2)-pow((BmDEX/2+abs(TgX)),2)) + BmDEZ/2);
    Z = - Flrdeep/2 + bmheight - 0.5*(BmDEY-BmDmY);
    //originBeamDump = X;
    originBeamDump.set(X,Z,Y);
  } else { // Do it the better way, just get the actual beam dump solid
    X = originBeamDump.x();
    //Y = -(sqrt(pow(Rhall,2)-pow((BmDEX/2+abs(TgX)),2)) + BmDEZ/2);
    // The 10cm offset is to make sure it cuts all the way through the wall
    Y = -originBeamDump.z()+10*cm;
    Z = -Flrdeep/2 + bmheight + originBeamDump.y();
  }

  G4SubtractionSolid* solidConWall_BD = new G4SubtractionSolid("conWall-BD1",
      solidConWall, solidBeamDump,rot270X,G4ThreeVector(X,Y,Z));

  G4LogicalVolume* logicConWall = new G4LogicalVolume(solidConWall_BD,GetMaterial("Concrete"),"ConWall");
  X = 0;
  Y = Flrdeep/2 - bmheight;
  Z = 0;
  G4VPhysicalVolume* physConWall = new G4PVPlacement(rot90X,G4ThreeVector(X,Y,Z),logicConWall,"ConWall",logicWorld, false,0,checkOverlaps);
  G4VisAttributes* ConWallvis = new G4VisAttributes(0);
  ConWallvis->SetColor(0.0,1.0,0.0);
  logicConWall->SetVisAttributes(ConWallvis);

  /////////////////////////////////////////////////////////////////////////////
  ///////////////////Concrete Wall thick part (HATW)///////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  par[1] = Rhall + Thall;
  par[2] = Rhall + Thall + TThall;
  par[3] = Flrdeep/2;
  G4Tubs* solidHATW = new G4Tubs("HATW",par[1],par[2],par[3],pi*250/180,pi*40/180);

  //Remove material for beam dump
  //X = 0;
  //Y = -(sqrt(pow(Rhall,2)-pow((BmDEX/2+abs(TgX)),2)) + BmDEZ/2);
  //Z = - Flrdeep/2 + bmheight - 0.5*(BmDEY-BmDmY);
  X = originBeamDump.x();
  Y = -originBeamDump.z();
  Z = -Flrdeep/2 + bmheight + originBeamDump.y();
  G4SubtractionSolid* solidHATW_BD = new G4SubtractionSolid("HATW-BD1",solidHATW,
      solidBeamDump,rot270X,G4ThreeVector(X,Y,Z));

  G4LogicalVolume* logicHATW = new G4LogicalVolume(solidHATW_BD,GetMaterial("Concrete"),"HATW");
  X = 0;
  Y = Flrdeep/2 - bmheight;
  Z = 0;
  G4VPhysicalVolume* physHATW = new G4PVPlacement(rot90X,G4ThreeVector(X,Y,Z),logicHATW,"HATW",logicWorld, false,0,checkOverlaps);
  G4VisAttributes* HATWvis = new G4VisAttributes(0);
  logicHATW->SetVisAttributes(HATWvis);


  /////////////////////////////////////////////////////////////////////////////
  ///////////////////Soil layer around the hall////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////

  /////////////////////////Thin soil layer////////////////////////////////////
  par[1] = Rhall + Thall;
  par[2] = Rhall + Thall + TThall;
  par[3] = Flrdeep/2;
  G4Tubs* solidSoilthin = new G4Tubs("Soilthin",par[1],par[2],par[3],-pi*70/180,pi*320/180);
  G4LogicalVolume* logicSoilthin = new G4LogicalVolume(solidSoilthin,GetMaterial("Ground"),"Soilthin");
  X = 0;
  Y = Flrdeep/2 - bmheight;
  Z = 0;
  G4VPhysicalVolume* physSoilthin = new G4PVPlacement(rot90X,G4ThreeVector(X,Y,Z),logicSoilthin,"Soilthin",logicWorld, false,0,checkOverlaps);
  G4VisAttributes* Soilthinvis = new G4VisAttributes(0);
  Soilthinvis->SetColor(0.8,0.5,0.2);
  logicSoilthin->SetVisAttributes(Soilthinvis);

  /////////////////////////Thick soil layer///////////////////////////////////
  par[1] = Rhall + Thall + TThall;
  par[2] = Rspace;
  par[3] = Flrdeep/2;
  G4Tubs* solidSoilthick = new G4Tubs("Soilthick",par[1],par[2],par[3],0,2*pi);


  //Remove material for beam dump from Soil
  G4VSolid *solidSoilthickCut = 0;
  if(beamDumpOrigVersion) {
    X = 0;
    Y = -(sqrt(pow(Rhall,2)-pow((BmDEX/2+abs(TgX)),2)) + BmDEZ/2);
    Z = - Flrdeep/2 + bmheight - 0.5*(BmDEY-BmDmY);
    solidSoilthickCut = new G4SubtractionSolid("Soilthick-BD1",solidSoilthick, solidBeamDump,rot270X,G4ThreeVector(X,Y,Z));

    //Remove material for beam dump part2 from Soil
    par[1] = BmDmX/2 + BDWth;
    par[2] = BmDmY/2 + BDWth;
    par[3] = BmDmZ/2 + BDWthz/2;
    solidBeamDump = new G4Box("BD2",par[1],par[2],par[3]);
    X = TgX;
    Y = -(sqrt(pow(Rhall,2)-pow(BmDEX/2+abs(TgX),2)) + BmDEZ + BmDmZ/2 + BDWthz/2);
    Z = - Flrdeep/2 + bmheight;
  } else {
    solidSoilthickCut = solidSoilthick;
    X = originBeamDump.x();
    Y = -originBeamDump.z();
    Z = originBeamDump.y() - Flrdeep/2. + bmheight;
  }
  G4SubtractionSolid* solidSoilthick_BD2 = new G4SubtractionSolid("Soilthick_BD1-BD2",solidSoilthickCut, solidBeamDump,rot270X,G4ThreeVector(X,Y,Z));
  G4LogicalVolume* logicSoilthick = new G4LogicalVolume(solidSoilthick_BD2,GetMaterial("Ground"),"Soilthick");
  X = 0;
  Y = Flrdeep/2 - bmheight;
  Z = 0;
  G4VPhysicalVolume* physSoil1 = new G4PVPlacement(rot90X,G4ThreeVector(X,Y,Z),logicSoilthick,"Soilthick",logicWorld, false,0,checkOverlaps);
  G4VisAttributes* Soilthickvis = new G4VisAttributes(0);
  Soilthickvis->SetColor(0.8,0.5,0.2);
  logicSoilthick->SetVisAttributes(Soilthickvis);


  /////////////////////////////////////////////////////////////////////////
  ///////////////////////////////Hall A Dome///////////////////////////////
  /////////////////////////////////////////////////////////////////////////

  par[1] = 5635*cm;
  par[2] = 5685*cm;
  par[3] = 0;
  par[4] = 2*pi;
  par[5] = 0;
  par[6] = 28*(pi/180);

  G4Sphere* solidDome = new G4Sphere("Dome",par[1],par[2],par[3],par[4],par[5],par[6]);
  G4LogicalVolume* logicDome = new G4LogicalVolume(solidDome,GetMaterial("Concrete"),"Dome");
  X = 0;
  Y = -bmheight - 33.42*m;
  Z = 0;
  G4VPhysicalVolume* physDome = new G4PVPlacement(rot90X,G4ThreeVector(X,Y,Z),logicDome,"Dome",logicWorld, false,0,checkOverlaps);
  G4VisAttributes* Domevis = new G4VisAttributes(0);
  logicDome->SetVisAttributes(Domevis);

  ////////////////////////////Soil outside roof///////////////////////////
  par[1] = 5685*cm;
  par[2] = 5750*cm;
  par[3] = 0;
  par[4] = 2*pi;
  par[5] = 0;
  par[6] = 28*(pi/180);

  G4Sphere* solidDomeSoil = new G4Sphere("DomeSoil",par[1],par[2],par[3],par[4],par[5],par[6]);
  G4LogicalVolume* logicDomeSoil = new G4LogicalVolume(solidDomeSoil,GetMaterial("Ground"),"DomeSoil");
  X = 0;
  Y = -bmheight - 33.42*m;
  Z = 0;
  G4VPhysicalVolume* physDomeSoil = new G4PVPlacement(rot90X,G4ThreeVector(X,Y,Z),logicDomeSoil,"DomeSoil",logicWorld, false,0,checkOverlaps);
  G4VisAttributes* DomeSoilvis = new G4VisAttributes(0);
  DomeSoilvis->SetColor(0.8,0.5,0.2);
  logicDomeSoil->SetVisAttributes(DomeSoilvis);


  //////////////////////////////////////////////////////////////////////////
  ////////////////////////Upstream Beamline/////////////////////////////////
  //////////////////////////////////////////////////////////////////////////

  /////////////////////Beam Pipe vacuum tube (BPTV)/////////////////////////
  par[1] = 0;
  par[2] = 0.5*BPdiamIn;
  par[3] = 0.5*(10*m - SCdiamOut/2 - SCZ);
  G4Tubs* solidBPTV = new G4Tubs("BPTV",par[1],par[2],par[3],0,2*pi);
  G4LogicalVolume* logicBPTV = new G4LogicalVolume(solidBPTV,GetMaterial("Vacuum"),"BPTV");
  X = 0;
  Y = 0;
  Z = -par[3] - SCdiamOut/2 - SCZ;
  G4VPhysicalVolume* physBPTV = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBPTV,"BPTV",logicWorld, false,0,checkOverlaps);
  G4VisAttributes* BPTVvis = new G4VisAttributes(1);
  BPTVvis->SetColor(0.0,1.0,0.0);
  BPTVvis->SetForceWireframe(false);
  BPTVvis->SetDaughtersInvisible(false);
  logicBPTV->SetVisAttributes(BPTVvis);

  /////////////////Beam Pipe Aluminum tube (BPTA)//////////////////////////
  par[1] = 0.5*BPdiamIn;
  par[2] = 0.5*BPdiamOut;
  par[3] = 0.5*(10*m - SCdiamOut/2 - SCZ);
  G4Tubs* solidBPTA = new G4Tubs("BPTA",par[1],par[2],par[3],0,2*pi);
  G4LogicalVolume* logicBPTA = new G4LogicalVolume(solidBPTA,GetMaterial("Aluminum"),"BPTA");
  X = 0;
  Y = 0;
  Z = -par[3] - SCdiamOut/2 - SCZ;
  G4VPhysicalVolume* physBPTA = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBPTA,"BPTA",logicWorld, false,0,checkOverlaps);
  G4VisAttributes* BPTAvis = new G4VisAttributes(1);
  BPTAvis->SetForceWireframe(true);
  logicBPTA->SetVisAttributes(BPTAvis);
  
  
  //////////////////////////////////////////////////////////////////////////
  ///////////////////Beamline Shield for upstream detector//////////////////
  //////////////////////////////////////////////////////////////////////////
  //Shield 1  
  G4double BS1Xlength = 0.5*conv*cm;
  G4double BS1Ylength = 60*conv*cm;
  G4double BS1Zlength = 62*conv*cm;
  par[1] = BS1Xlength/2;
  par[2] = BS1Ylength/2;
  par[3] = BS1Zlength/2;
  G4Box* solidBS1 = new G4Box("BS1",par[1],par[2],par[3]);
  G4LogicalVolume* logicBS1 = new G4LogicalVolume(solidBS1,GetMaterial("Iron"),"BS1");
  X = 0.5*62.99*conv*cm + BS1Xlength/2;
  Y = 0;
  Z = -105*cm + 57.15*cm + 0.75*conv*cm + 5.45*conv*cm + 0.25*conv*cm + 9.125*conv*cm + BS1Zlength/2;
  G4VPhysicalVolume* physBS1Copy1 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBS1,"BS1Copy1",logicWorld, false,0,checkOverlaps);
  G4VisAttributes* BS1vis = new G4VisAttributes(1);
  logicBS1->SetVisAttributes(BS1vis);
  X = -0.5*62.99*conv*cm - BS1Xlength/2;
  Y = 0;
  Z = -105*cm + 57.15*cm + 0.75*conv*cm + 5.45*conv*cm + 0.25*conv*cm + 9.125*conv*cm + BS1Zlength/2;
  G4VPhysicalVolume* physBS1Copy2 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBS1,"BS1Copy2",logicWorld, false,0,checkOverlaps);

  //Shield2
  G4double BS2Xlength = 0.5*(62.99*conv*cm - 25.6*conv*cm) + BS1Xlength;
  G4double BS2Ylength = BS1Ylength;
  G4double BS2Zlength = BS1Xlength;
  par[1] = BS2Xlength/2;
  par[2] = BS2Ylength/2;
  par[3] = BS2Zlength/2;
  G4Box* solidBS2 = new G4Box("BS2",par[1],par[2],par[3]);
  G4LogicalVolume* logicBS2 = new G4LogicalVolume(solidBS2,GetMaterial("Iron"),"BS2");
  X = 0.5*25.6*conv*cm + 0.5*BS2Xlength;
  Y = 0;
  Z = -105*cm + 57.15*cm + 0.75*conv*cm + 5.45*conv*cm + 0.25*conv*cm + 9.125*conv*cm - BS2Zlength/2;
  G4VPhysicalVolume* physBS2Copy1 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBS2,"BS2Copy1",logicWorld, false,0,checkOverlaps);
  G4VisAttributes* BS2vis = new G4VisAttributes(1);
  logicBS2->SetVisAttributes(BS2vis);
  X = -0.5*25.6*conv*cm - 0.5*BS2Xlength;;
  Y = 0;
  Z = -105*cm + 57.15*cm + 0.75*conv*cm + 5.45*conv*cm + 0.25*conv*cm + 9.125*conv*cm - BS2Zlength/2;
  G4VPhysicalVolume* physBS2Copy2 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBS2,"BS2Copy2",logicWorld, false,0,checkOverlaps);

  //Shield3 (Top/Bottom)
  G4double BS3Xlength = 62.99*conv*cm + 2*BS1Xlength;
  G4double BS3Ylength = BS1Xlength;
  G4double BS3Zlength = BS1Zlength + BS2Zlength;
  par[1] = BS3Xlength/2;
  par[2] = BS3Ylength/2;
  par[3] = BS3Zlength/2;
  G4Box* solidBS3 = new G4Box("BS3",par[1],par[2],par[3]);
  G4LogicalVolume* logicBS3 = new G4LogicalVolume(solidBS3,GetMaterial("Iron"),"BS3");
  X = 0;
  Y = 0.5*BS1Ylength + 0.5*BS3Ylength;
  Z = -105*cm + 57.15*cm + 0.75*conv*cm + 5.45*conv*cm + 0.25*conv*cm + 9.125*conv*cm + BS1Zlength/2 - BS2Zlength;
  G4VPhysicalVolume* physBS3Copy1 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBS3,"BS3Copy1",logicWorld, false,0,checkOverlaps);
  G4VisAttributes* BS3vis = new G4VisAttributes(0);
  logicBS3->SetVisAttributes(BS3vis);
  X = 0;
  Y = -0.5*BS1Ylength - 0.5*BS3Ylength;
  Z = -105*cm + 57.15*cm + 0.75*conv*cm + 5.45*conv*cm + 0.25*conv*cm + 9.125*conv*cm + BS1Zlength/2 - BS2Zlength;
  G4VPhysicalVolume* physBS3Copy2 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBS3,"BS3Copy2",logicWorld, false,0,checkOverlaps);


  //////////////////////////////////////////////////////////////////////////
  //////////////////////////////APEX Target/////////////////////////////////
  //////////////////////////////////////////////////////////////////////////

  if( gRadConfig->BuildDetector("Target") ) {
    //Target is described in APEXTarget.cc
    switch(gRadConfig->TargetType) {
      case RadTargetType::APEX:
        (void)new APEXTarget(dc,this,logicWorld);
        break;
      case RadTargetType::CARBON:
        MakeCarbonCrosscheckTarget(logicWorld);
        break;
      default:
        break;
    }
    //APEXTarget* target = new APEXTarget(dc,this,logicWorld);
  }


  //////////////////////////////////////////////////////////////////////////
  ////////////////////APEX Extension Box////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////

  APEXExtBox* extBox = new APEXExtBox(dc, this, logicWorld);


  //////////////////////////////////////////////////////////////////////////
  //////////////////////////////Septa Magnet////////////////////////////////
  //////////////////////////////////////////////////////////////////////////

  if( gRadConfig->BuildDetector("Septum") ) {
    //Septa described in SeptaMagnet.cc
    G4bool fieldON = 0;
    if(!gRadConfig->SeptaFieldFilename.isNull()) {
      fieldON = 1;
      G4cout << "Reading Septum Field from: "
        << gRadConfig->SeptaFieldFilename.data() << G4endl;
    } else {
      G4cout << "Disabling Semptum Field." << G4endl;
    }
    SeptaMagnet* septa = new SeptaMagnet(dc,this,logicWorld,fieldON);
  }


  ///////////////////////////////////////////////////////////////////////////
  /////////////////////////////Downstream Beamline///////////////////////////
  ///////////////////////////////////////////////////////////////////////////

  //Beam Line is described in Beamline.cc
  if( gRadConfig->BuildDetector("BeamlineDS") ) {
    Beamline* beamline = new Beamline(dc, this, logicWorld);
  }


  ///////////////////////////////////////////////////////////////////////////
  ///////////////////////////////HRS/////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////

  if(gRadConfig->BuildDetector("HRS")) {
    //Left HRS (described in HRS.cc)
    HRS* LHRS = new HRS(12.5*deg, dc, this, logicWorld); //send DetectorConstruction, logicAI01 and logical volumes. (+ angle)

    //Right HRS (described in HRS.cc)
    HRS* RHRS = new HRS(-12.5*deg, dc, this, logicWorld); //send DetectorConstruction, logicAI01 and logical volumes. (- angle)

    if( gRadConfig->Q1Version == RadQ1Version::APEX ) {
      G4cout << "Making Apex Q1 version (instead of super conducting one"
        << G4endl;
      MakeApexQ1(logicWorld);
    }

    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////Cradle//////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    //Left Cradle
    Cradle* LCradle = new Cradle(12.5*deg, dc, this, logicWorld);

    //Right Cradle
    Cradle* RCradle = new Cradle(-12.5*deg, dc, this, logicWorld);


    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////Gantry//////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    //Left Gantry
    Gantry* LGantry = new Gantry(12.5*deg, dc, this, logicWorld);

    //Right Gantry
    Gantry* RGantry = new Gantry(-12.5*deg, dc, this, logicWorld);


    ///////////////////////////////////////////////////////////////////////////
    /////////////////////////////HRS Link//////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    //Left Link
    HRSLink* LLink = new HRSLink(12.5*deg, dc, this, logicWorld);

    //Right Link
    HRSLink* RLink = new HRSLink(-12.5*deg, dc, this, logicWorld);
  }


  //////////////////////////////////////////////////////////
  ////////////Detector (Placed upstream)////////////////////
  //////////////////////////////////////////////////////////

  G4Tubs* solidUpstreamDet = new G4Tubs("UpstreamDet",0,15*m,2.5*cm,pi,pi);
  G4LogicalVolume* logicUpstreamDet = new G4LogicalVolume(solidUpstreamDet,GetMaterial("Vacuum"),"UpstreamDet");
  X = 0;
  Y = -bmheight;
  Z = -16*m;
  G4VPhysicalVolume* physUpstreamDet = new G4PVPlacement(rot180Z,G4ThreeVector(X,Y,Z),logicUpstreamDet,"UpstreamDet",logicWorld, false,0,checkOverlaps);
  G4VisAttributes* UpstreamDetvis = new G4VisAttributes(1);
  UpstreamDetvis->SetForceWireframe(false);
  logicUpstreamDet->SetVisAttributes(UpstreamDetvis);


  //UpstreamDetSD* upSD = new UpstreamDetSD("/UpDet");
  // (jc2 2017-04-27) changed to generic SD for simplicity
  GenericSD* upSD = new GenericSD("UpDet");
  G4SDManager* Upsdman = G4SDManager::GetSDMpointer();
  Upsdman->AddNewDetector(upSD);
  logicUpstreamDet->SetSensitiveDetector(upSD);


  //2 new volumes closer to the target to evaluate radiation levels
  
  //volume 1 placed above the target
  G4Box* solidRadBox1 = new G4Box("RadBox1",50*cm, 5*cm, 50*cm);
  G4LogicalVolume* logicRadBox1 = new G4LogicalVolume(solidRadBox1,GetMaterial("Vacuum"),"RadBox1");
  X = 0;
  Y = 1*m;
  Z = -105*cm;
  G4VPhysicalVolume* physRadBox1 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicRadBox1,"RadBox1",logicWorld, false,0,checkOverlaps);
  
  RadBox1SD* rad1 = new RadBox1SD("/rad1");
  G4SDManager* rad1sdman = G4SDManager::GetSDMpointer();
  rad1sdman->AddNewDetector(rad1);
  //logicRadBox1->SetSensitiveDetector(rad1);
  
  
  //volume 2 placed above the target
  G4Box* solidRadBox2 = new G4Box("RadBox2",5*cm, 50*cm, 50*cm);
  G4LogicalVolume* logicRadBox2 = new G4LogicalVolume(solidRadBox2,GetMaterial("Vacuum"),"RadBox2");
  X = -1*m;
  Y = 0;
  Z = -25*cm;
  G4VPhysicalVolume* physRadBox2 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicRadBox2,"RadBox2",logicWorld, false,0,checkOverlaps);
  
  RadBox2SD* rad2 = new RadBox2SD("/rad2");
  G4SDManager* rad2sdman = G4SDManager::GetSDMpointer();
  rad2sdman->AddNewDetector(rad2);
  //logicRadBox2->SetSensitiveDetector(rad2);

}

G4Material* HallA::GetMaterial(G4String mat){

  return fDetCon->GetMaterial(mat);

}


HallA::~HallA(){;}


void HallA::MakeCarbonCrosscheckTarget(G4LogicalVolume *logicWorld)
{
  G4bool checkOverlaps = true;

  // A Carbon target with radius of 5cm, and 0.193cm thick
  G4double thickness = 0.193*CLHEP::cm;
  G4double radius = 5*CLHEP::cm;

  //G4double SCZ = 105*cm; //distance between SC and pivot point
  G4double SC_dim = 4*radius+5*CLHEP::mm;

  G4Box *solCarbonSC = new G4Box("solCarbonSC",SC_dim/2.,SC_dim/2.,SC_dim/2.);
  G4LogicalVolume *logCarbonSC = new G4LogicalVolume(solCarbonSC,
      GetMaterial("Vacuum"),"logCarbonSC");
  G4VPhysicalVolume *physCarbonSC = new G4PVPlacement(0,
      G4ThreeVector(0.0,0.0,-SCZ),logCarbonSC,"physCarbonSC",logicWorld,
      false,0,checkOverlaps);

  // Use a 2mm thick hollow sphere that's 2*CarbonRadius in radius surrounding
  // the Carbon target to detect all particles that radiate out of the Carbon
  // target.
  G4double rMaxCarbonDetector = 1.5*radius;
  G4double rMinCarbonDetector = rMaxCarbonDetector-0.1*CLHEP::cm;
  G4Sphere *solCarbonDetector = new G4Sphere("solCarbonDetector",
      rMinCarbonDetector,rMaxCarbonDetector,0,2*CLHEP::pi,0,2*CLHEP::pi);
  G4LogicalVolume *logCarbonDetector = new G4LogicalVolume(solCarbonDetector,
      GetMaterial("Vacuum"),"logCarbonDetector");
  G4VPhysicalVolume *physCarbonDetector = new G4PVPlacement(0,
      G4ThreeVector(0.,0.,0.),logCarbonDetector,"physCarbonDetector",
      logCarbonSC,false,0,checkOverlaps);

  // Now place the Carbon target
  G4Tubs* solCarbonTarget = new G4Tubs("solCarbonTarget",0.,radius/2.,thickness/2.,
      0.,2*CLHEP::pi);
  G4LogicalVolume *logCarbonTarget = new G4LogicalVolume(solCarbonTarget,
      GetMaterial("Carbon"),"logCarbonTarget");
  G4VPhysicalVolume* physCarbonTarget = new G4PVPlacement(0,
      G4ThreeVector(0.0,0.0,0),logCarbonTarget,"physCarbonTarget",logCarbonSC,
      false,0,checkOverlaps);


  GenericSD* sdCarbonDetector = new GenericSD("CarbonDet");
  G4SDManager::GetSDMpointer()->AddNewDetector(sdCarbonDetector);
  logCarbonDetector->SetSensitiveDetector(sdCarbonDetector);


  G4VisAttributes *visCarbonSC = new G4VisAttributes(0);
  visCarbonSC->SetColor(0.5,0.25,0.75,0.0);
  logCarbonSC->SetVisAttributes(visCarbonSC);

  G4VisAttributes *visCarbonDetector = new G4VisAttributes(1);
  visCarbonDetector->SetColor(1.0,0.0,0.0,0.05);
  visCarbonDetector->SetForceWireframe(false);
  logCarbonDetector->SetVisAttributes(visCarbonDetector);
}

void HallA::MakeApexQ1(G4LogicalVolume *logicWorld)
{
  G4bool checkOverlaps = true;

  G4double X,Y,Z;
  // Use GDML parser to read in in STEP file
  G4GDMLParser parser;
  parser.Read("/home/cornejo/apex/radStudy/simulation/q1.gdml");
  G4LogicalVolume *logCadQ1 = parser.GetVolume("logCadQ1");
  G4LogicalVolume *logCadQ1Coils = parser.GetVolume("logCadQ1Coils");
  logCadQ1->SetVisAttributes(G4Color(0.,0.22,0.58));
  logCadQ1Coils->SetVisAttributes(G4Color(1,1,0));

  // Now place the read in structure
  G4RotationMatrix *Q1angle = new G4RotationMatrix;
  Q1angle->rotateX(180*deg);
  G4double thetaHRS = 0;
  Q1angle->rotateY(thetaHRS);
  X = 207*cm*sin(thetaHRS);
  Y = 0;
  Z = 207*cm*cos(thetaHRS);
  X = 0;
  Y = 0;
  Z = 207*cm-80*cm+10*cm;
  G4VPhysicalVolume *physQ1 = new G4PVPlacement(Q1angle,G4ThreeVector(X,Y,Z),
      logCadQ1,"Q1",logicWorld,false,0,checkOverlaps);
  G4VPhysicalVolume *physQ1Coils = new G4PVPlacement(Q1angle,G4ThreeVector(X,Y,Z),
      logCadQ1Coils,"Q1Coils",logicWorld,false,0,checkOverlaps);
}
