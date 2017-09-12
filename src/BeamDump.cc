#include "BeamDump.hh"
#include "BMDCDetSD.hh"
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
#include "G4SubtractionSolid.hh"
#include "G4SDManager.hh"
#include <G4UnionSolid.hh>
#include <G4UserLimits.hh>
#include "RadConfig.hh"

#include "G4ThreeVector.hh"

#include "HallA.hh"

//using namespace std;

BeamDump::BeamDump(B1DetectorConstruction *dc, HallA *ha)
  : fDetCon(dc), fHallA(ha)
{
  BmDEX = 205*cm; 
  BmDEY = 271*cm;
  BmDEZ = 1838*cm;
  BmDmX = 205*cm;
  BmDmY = 205*cm;
  BmDmZ = 1205*cm;
  BDWth = 130*cm;
  BDWthz = 130*cm;
  BDWLZ = 380*cm; // Waterbarrel dimensions
  TgX = 0.0*cm; // I have no idea what this is, but it's zero anyways....
}

G4LogicalVolume* BeamDump::ConstructBeamDumpUpdated()
{
  G4bool checkOverlaps = true;
  //Variables of HallA class
  G4double Rhall = fHallA->getRhall();
  //G4double bmheight = fHallA->getbmheight();
  //G4double Tair1 = fHallA->getTair1();
  //G4double BPAirTubeLength = fHallA->getBPAirTubeLength();


  G4double in = 2.54*cm;
  G4double ft = 12*in;
  G4double dimFudgeFactor = 1*mm;

  G4double rlDiffuser = 8.897*cm; // Radiation length of Al Diffuser

  // Positions in Z from Pavel's Model (only key features listed, and position
  // indicated is the upstream side/start of each element)
  G4double posModelAperturePlateZ = 271.5*cm;
  G4double posModelAlWallZ        = 551.0*cm;
  G4double posModelDiffuserZ      = 591.0*cm;
  G4double posModelHeBeampipeZ    = 621.0*cm;
  G4double posModelStepupZ        = 1830.8*cm;
  G4double posModelDumpFaceZ      = 2068.0*cm; // this is the water barrel

  /////////////////////////////////////////////////////////////////////////////
  // Beam dump world can be divided into "two" parts. The tall (closest to
  // the entrance) and "short" section where the beam dump water barrel is at.
  // Let's construct this "simplified" model where the wall is uniformly thick
  // throughout the enclosure.
  G4double dimWallThick    = 5*ft;  // 5 feet according to engineering sheets

  // Define the tunnel dimensions (all stuff not walls)
  // Symmetric in X
  // The "tall" part
  G4double dimTunnelX      = 7*ft;
  G4double dimTunnel1Y     = 10*ft;
  G4double dimTunnel1Z     = posModelStepupZ;
  // The "short" part
  G4double dimTunnel2Y     = 7*ft;
  // no clue where this number comes from, it was part of original code
  G4double dimTunnel2Z     =  BmDmZ;
  // The total length of the tunnel (at longest part)
  G4double dimTunnelZ = dimTunnel1Z+dimTunnel2Z;

  // Define the largest dimension of the tunnel and will cut out
  // smaller parts later
  G4double dimWorldX = dimTunnelX  + 2*dimWallThick;
  G4double dimWorldY = dimTunnel1Y + 2*dimWallThick;
  G4double dimWorldZ = dimTunnel1Z + dimTunnel2Z + dimWallThick;
  // Part of the world that needs to be "cut" out
  G4double dimWorldCutY = dimTunnel1Y-dimTunnel2Y;
  G4double dimWorldCutZ = dimTunnel2Z;

  // Air part of the tunnel (according to Pavel's model and engineering sheets)
  // (I decided to ignore the 31cm of NATM that sticks out at the Al wall for
  // simplicity)
  // The x-dimensions are all symmetric and centered on the beamline, so fix
  // here...
  G4double dimAlWallZ       = 0.635*mm; // Al wall thickness
  G4double dimTunnelAirZ    = 551*cm - dimAlWallZ/2.; // Distance to Al wall
  // Region of Nitrogen atmosphere that sticks into the Air region
  //G4double dimTunnelAirCutZ = (551-520)*cm; // Distance from Pipe to Al wall
  // Now the Nitrogen atmosphere part of the tunnel
  // First section of Nitrogen Atmosphere
  G4double dimTunnelN1Z     = 1830.8*cm - dimTunnelAirZ - dimAlWallZ;
  // Second section of Nitrogen covers all of the 7' tall section of the dump
  G4double dimTunnelN2Z     = dimTunnel2Z;


  // Beampipe inside the Dump (1 in thick 38 in diameter)
  // (1 in thick seems too thick to me, but this is what was previously
  // coded in here, and I don't have any newer information.)
  G4double dimBeampipeThick  = 1*in;
  G4double dimBeampipeRadius = 38*in/2.;
  G4double dimBeampipeRadiusIn = dimBeampipeRadius-dimBeampipeThick;

  // Some dimensions of the aperture plate are "guessed" because I do not
  // know them in full detail.  Pavel's model shows the thick part as being
  // ~2mm thick. A presentation by Michalski shows the opening hole to be 3"
  // in diameter
  G4double dimAperturePlateThinZ = dimAlWallZ; // Make the thin part as thick as Al wall
  G4double dimAperturePlateZ = 2*cm;
  G4double dimAperturePlateHoleR = 3*in/2.;
  G4double dimAperturePlateR = dimBeampipeRadiusIn/2.; // Just a guess...

  // specified in "local" coordinates
  G4double posAperturePlateZ = posModelAperturePlateZ-dimTunnelAirZ/2.;
  G4double dimExitWindowZ    = 0.5*mm;  // According to Pavel's model
  G4double dimBeampipeHeZ    = posModelDumpFaceZ-posModelHeBeampipeZ;
  G4double posHeBeampipeZ    = -dimTunnelN1Z/2. + dimAlWallZ +
    (posModelHeBeampipeZ - posModelAlWallZ) + dimBeampipeHeZ/2.;
  G4double posDiffuserZ      = -dimTunnelN1Z/2. + dimAlWallZ +
    (posModelDiffuserZ - posModelAlWallZ);
  G4double posDumpFaceZ     = -dimTunnelN1Z/2. + dimAlWallZ +
    (posModelDumpFaceZ-posModelAlWallZ);

  /////////////////////////////////////////////////////////////////////////////
  /// Dump enclosure (Near Hall)

  G4double beamX = 0;
  G4double beamY = +dimWorldY/2. - dimWallThick - dimTunnel2Y/2.;
  G4double beamZ = -dimWorldZ/2.;

  // Set the "local" origin of this volume (see header for definition)
  fBDLocalOrigin.set(-beamX,-beamY,-beamZ);
      //-dimWorldY/2.+dimWallThick+dimTunnel2Y/2.,
      //+dimWorldZ/2.);
  // Set the "global" origin of this volume
  fBDWorldOrigin = fBDLocalOrigin + G4ThreeVector(0.,0.,Rhall);
  // Set the world dimensions (max)
  fBDWorldDimensions.set(dimWorldX,dimWorldY,dimWorldZ);

  // First, construct the whole beam dump world
  G4Box *solWorldPre = new G4Box("BeamDumpWorldPre",
    dimWorldX/2.,dimWorldY/2.,dimWorldZ/2.);
  G4Box *solWorldCut = new G4Box("BeamDumpWorldCut",
    dimWorldX/2.+dimFudgeFactor,
    dimWorldCutY/2.+dimFudgeFactor,
    dimWorldCutZ/2.+dimFudgeFactor);
  G4SubtractionSolid *solWorld = new G4SubtractionSolid("BeamDumpWorld",
    solWorldPre,solWorldCut,0,G4ThreeVector(0.,
      -(dimWorldY-dimWorldCutY)/2.,
      +(dimWorldZ-dimWorldCutZ)/2.));
  G4LogicalVolume *logicWorld = new G4LogicalVolume(solWorld,
      GetMaterial("Vacuum"),"BeamDumpWorld");
  //G4VisAttributes *visWorld = new G4VisAttributes(0);
  //visWorld->SetForceWireframe(false);
  //visWorld->SetColor(0.0,1.0,0.0);
  //visWorld->SetDaughtersInvisible(false);
  //logicWorld->SetVisAttributes(visWorld);
  //logicWorld->SetVisAttributes(G4VisAttributes::Invisible);

  // Now construct the tunnel solid (this is the beam dump world except
  // the walls)

  // First, construct the whole tunnel solid and volume (this defines the
  // "non" wall parts of the world
  G4Box *solTunnelTop = new G4Box("BeamDumpTunnelTop",
      dimTunnelX/2.  +dimFudgeFactor,
      dimTunnel2Y/2. +dimFudgeFactor,
      dimTunnelZ/2.);
  G4Box *solTunnelBottom  = new G4Box("BeamDumpTunnelBottom",
      dimTunnelX/2+dimFudgeFactor,
      (dimTunnel1Y-dimTunnel2Y)/2.+dimFudgeFactor,
      dimTunnel1Z/2.);
  G4UnionSolid *solTunnel = new G4UnionSolid("BeamDumpTunnel",
      solTunnelTop,solTunnelBottom,0,G4ThreeVector(0.,
        -dimTunnel1Y/2.,
        -dimTunnel2Z/2.));
  // Now construct the walls
  G4SubtractionSolid *solWall = new G4SubtractionSolid("BeamDumpWall",
      solWorld, solTunnel, 0, G4ThreeVector(0,beamY,
        (-dimWorldZ+dimTunnelZ)/2.));

  G4LogicalVolume *logicWall = new G4LogicalVolume(solWall,
      GetMaterial("Concrete"),"BeamDumpWall");
  (void*)new G4PVPlacement(0, G4ThreeVector(0,0,0),
      logicWall,"BeamDumpWall",logicWorld,false,0,checkOverlaps);

  //G4VisAttributes *visWall = new G4VisAttributes(G4Colour(0.0,1.0,0.0,0.65));
  G4VisAttributes *visWall = new G4VisAttributes(1);
  visWall->SetForceWireframe(false);
  visWall->SetColor(0.0,1.0,0.0,0.65);
  visWall->SetDaughtersInvisible(false);
  logicWall->SetVisAttributes(visWall);
  //logicWall->SetVisAttributes(new G4VisAttributes(0));

  // Now create the tunnel segments with Air and Nitrogen atmospherese
  G4Box *solTunnelAir = new G4Box("BeamDumpTunnelAir",
      dimTunnelX/2.,dimTunnel1Y/2.,dimTunnelAirZ/2.);
  G4LogicalVolume *logicTunnelAir = new G4LogicalVolume(solTunnelAir,
      GetMaterial("Air"),"BeamDumpTunnelAir");
  (void*)new G4PVPlacement(0, G4ThreeVector(
        0,0.,-(dimWorldZ-dimTunnelAirZ)/2.),
      logicTunnelAir,"BeamDumpTunnelAir",logicWorld,false,0,checkOverlaps);
  logicTunnelAir->SetVisAttributes(new G4VisAttributes(
        G4Colour(0.,0.,0.61,0.4)));

  // Now Nitrogen tunnel
  G4Box *solTunnelN1 = new G4Box("BeamDumpTunnelN1",
      dimTunnelX/2.,dimTunnel1Y/2.,dimTunnelN1Z/2.);
  G4Box *solTunnelN2 = new G4Box("BeamDumpTunnelN2",
      dimTunnelX/2.,dimTunnel2Y/2.,dimTunnelN2Z/2.);
  G4UnionSolid *solTunnelN = new G4UnionSolid("BeamDumpTunnelN",
      solTunnelN1,solTunnelN2,0,G4ThreeVector(0.,
        (dimTunnel1Y-dimTunnel2Y)/2.,(dimTunnelN1Z+dimTunnelN2Z-dimFudgeFactor)/2.));
  G4LogicalVolume *logicTunnelN = new G4LogicalVolume(solTunnelN,
      GetMaterial("Nitrogen"),"BeamDumpTunnelN");
  (void*)new G4PVPlacement(0, G4ThreeVector(
        0,0.,dimTunnelAirZ+dimAlWallZ-(dimWorldZ-dimTunnelN1Z)/2.),
      logicTunnelN,"BeamDumpTunnelN",logicWorld,false,0,checkOverlaps);
  logicTunnelN->SetVisAttributes(new G4VisAttributes(
        G4Colour(0.93,0.93,0.,0.4)));

  // Beampipe exit windows
  G4Tubs *solExitWindow = new G4Tubs("BeamDumpExitWindow",
      0,dimBeampipeRadiusIn,dimExitWindowZ/2.,0.,2*pi);
  G4LogicalVolume *logicExitWindow = new G4LogicalVolume(solExitWindow,
      GetMaterial("Aluminum"),"BeamDumpExitWindow");

  // Now the Al wall
  G4Box *solAlWall = new G4Box("BeamDumpAlWall",
      dimTunnelX/2.,dimTunnel1Y/2.,dimAlWallZ/2.);
  G4Tubs *solAlWallAirNCut = new G4Tubs("BeamDumpAlWallAirNCut",
      0.,dimBeampipeRadius, dimAlWallZ/2.,0.,2*pi);

  G4LogicalVolume *logicAlWall = new G4LogicalVolume(solAlWall,
      GetMaterial("Aluminum"),"BeamDumpAlWall");
  G4LogicalVolume *logicAlWallAirNCut = new G4LogicalVolume(solAlWallAirNCut,
      GetMaterial("Nitrogen"),"BeamDumpAlWallAirNCut");

  (void*)new G4PVPlacement(0, G4ThreeVector(
        0,beamY,0.),logicAlWallAirNCut,"BeamDumpAlWallAirNCut",
      logicAlWall,false,0,checkOverlaps);
  (void*)new G4PVPlacement(0, G4ThreeVector(
        0,0.,dimTunnelAirZ-(dimWorldZ-dimAlWallZ)/2.),
      logicAlWall,"BeamDumpAlWall",logicWorld,false,0,checkOverlaps);

  // Now place beampipe components on Air tunnel
  G4Tubs *solBeampipeAir = new G4Tubs("BeamDumpBeampipeAir",
      0.,dimBeampipeRadius, dimTunnelAirZ/2.,0.,2*pi);
  // And the vacuum inside the beampipe
  G4Tubs *solBeampipeAirVac = new G4Tubs("BeamDumpBeampipeAirVac",
      0.,dimBeampipeRadiusIn, dimTunnelAirZ/2.,0.,2*pi);
  G4LogicalVolume *logicBeampipeAir = new G4LogicalVolume(solBeampipeAir,
      GetMaterial("BLDmat"),"BeamDumpBeampipeAir");
  G4LogicalVolume *logicBeampipeAirVacc = new G4LogicalVolume(solBeampipeAirVac,
      GetMaterial("Vacuum"),"BeamDumpBeampipeAirVac");
  (void*)new G4PVPlacement(0,
      G4ThreeVector(0.,beamY,0.),logicBeampipeAir,"BeamDumpBeampipeAir",
      logicTunnelAir,false,0,checkOverlaps);
  (void*)new G4PVPlacement(0,
      G4ThreeVector(0.,0.,0.),logicBeampipeAirVacc,"BeamDumpBeampipeAirVacc",
      logicBeampipeAir,false,0,checkOverlaps);
  logicBeampipeAir->SetVisAttributes(G4VisAttributes::Invisible);
  logicBeampipeAirVacc->SetVisAttributes(new G4VisAttributes(
        G4Colour(0.5,0.5,0.5,0.15)));

  // Also place the exit window and Aperture plate inside the beampipe
  G4Tubs *solAperturePlateThin = new G4Tubs("BeamDumpAperturePlateThin",
      dimAperturePlateR,dimBeampipeRadiusIn,dimAperturePlateThinZ/2.,0.,2*pi);
  G4Tubs *solAperturePlate = new G4Tubs("BeamDumpAperturePlate",
      dimAperturePlateHoleR,dimAperturePlateR,dimAperturePlateZ/2.,0.,2*pi);

  G4LogicalVolume *logicAperturePlateThin = new G4LogicalVolume(solAperturePlateThin,
      GetMaterial("Aluminum"),"BeamDumpAperturePlateThin");
  G4LogicalVolume *logicAperturePlate = new G4LogicalVolume(solAperturePlate,
      GetMaterial("Aluminum"),"BeamDumpAperturePlate");

  (void*)new G4PVPlacement(0,
      G4ThreeVector(0,0,(dimTunnelAirZ-dimExitWindowZ)/2.),logicExitWindow,
      "BeamDumpExitWindowAirTunnel",
      logicBeampipeAirVacc,false,0,checkOverlaps);
  (void*)new G4PVPlacement(0,
      G4ThreeVector(0,0,posAperturePlateZ),logicAperturePlateThin,
      "BeamDumpAperturePlateThin",
      logicBeampipeAirVacc,false,0,checkOverlaps);
  (void*)new G4PVPlacement(0,
      G4ThreeVector(0,0,posAperturePlateZ),logicAperturePlate,
      "BeamDumpAperturePlate",
      logicBeampipeAirVacc,false,0,checkOverlaps);
  logicAperturePlateThin->SetVisAttributes(new G4VisAttributes(
        G4Colour(0.7,0.53,0.28,0.95)));
  logicAperturePlate->SetVisAttributes(new G4VisAttributes(
        G4Colour(0.7,0.53,0.28,0.95)));

  // Now place the Helium beampipe in Nitrogen tunnel
  G4Tubs *solBeampipeN = new G4Tubs("BeamDumpBeampipeN",
      0.,dimBeampipeRadius, dimBeampipeHeZ/2.,0.,2*pi);
  G4Tubs *solBeampipeHe = new G4Tubs("BeamDumpBeampipeHe",
      0.,dimBeampipeRadiusIn, dimBeampipeHeZ/2.,0.,2*pi);
  G4LogicalVolume *logicBeampipeN = new G4LogicalVolume(solBeampipeN,
      GetMaterial("BLDmat"),"BeamDumpBeampipeN");
  G4LogicalVolume *logicBeampipeHe = new G4LogicalVolume(solBeampipeHe,
      GetMaterial("Helium"),"BeamDumpBeampipeHe");
  (void*)new G4PVPlacement(0,
      G4ThreeVector(0.,beamY,posHeBeampipeZ),logicBeampipeN,"BeamDumpBeampipeN",
      logicTunnelN,false,0,checkOverlaps);
  (void*)new G4PVPlacement(0,
      G4ThreeVector(0.,0.,0.),logicBeampipeHe,"BeamDumpBeampipeHe",
      logicBeampipeN,false,0,checkOverlaps);
  logicBeampipeN->SetVisAttributes(G4VisAttributes::Invisible);
  logicBeampipeHe->SetVisAttributes(new G4VisAttributes(
        G4Colour(0.0,0.0,0.9,0.25)));

  // Finally, place the diffuser, which changes depending on the energy
  // From Pavel's model, it seems these are the selections:
  // 10% RL for Beam Energy > 2.4 GeV
  // 30% RL for Beam Energy > 5.4 GeV
  G4double dimDiffuserRLZ = 0;
  if( gRadConfig->BeamEnergy > 5.4*GeV ) {
    dimDiffuserRLZ = 0.3; // 30% r.l.
  } else if ( gRadConfig->BeamEnergy > 2.4*GeV ) {
    dimDiffuserRLZ = 0.1; // 10% r.l.
  } else { // Actually, I'm not sure what happens if the beam energy is lower than
    // both of these. Maybe there is no diffuser used?!?!?
    G4cerr << "No diffuser used?!?! I only have a model for the diffuser "
      << "for beam energy > 2.4 GeV" << G4endl;
  }

  if( dimDiffuserRLZ > 0) {
    G4double dimDiffuserZ = dimDiffuserRLZ*rlDiffuser;
    G4cout << "Placing Aluminum Diffuser with width: "
      << dimDiffuserZ/cm << " cm ( " << 100.*dimDiffuserRLZ << "% r.l. )"
      << G4endl;
    // Also don't have a model for the diffuser. I'm just going to make it
    // a cylinder with same cross section as beampipe
    G4Tubs *solDiffuser = new G4Tubs("BeamDumpDiffuser",
        0.,dimBeampipeRadius, dimDiffuserZ/2.,0,2*pi);
    G4LogicalVolume *logicDiffuser = new G4LogicalVolume(solDiffuser,
        GetMaterial("Aluminum"),"BeamDumpDiffuser");
    (void*)new G4PVPlacement(0,G4ThreeVector(0.,beamY,
          posDiffuserZ + dimDiffuserZ/2.),
        logicDiffuser,"BeamDumpDiffuser",logicTunnelN,false,0,checkOverlaps);
    logicDiffuser->SetVisAttributes(new G4VisAttributes(
          G4Colour(0.71,0.13,0.52,0.99)));
  }

  // Now build the water barrel
  BuildWaterBarrel(logicTunnelN,0,G4ThreeVector(0.,beamY,posDumpFaceZ));

  // Return this newly created world
  return logicWorld;
}

void BeamDump::ConstructBeamDumpOrig(G4LogicalVolume *logicWorld)
{
  //G4double BDWRin = 53*cm; //Beam dump water barrel params
  //G4double BDWRout = 54.8*cm;
  //G4double BDWLZ = 380*cm;
  //G4double REVBD1 = 30.5*cm; //Inner radius for barrel end cap (only 1 side)

  //Variables of HallA class
  G4double Rhall = fHallA->getRhall();
  //G4double bmheight = fHallA->getbmheight();
  //G4double Tair1 = fHallA->getTair1();
  //G4double BPAirTubeLength = fHallA->getBPAirTubeLength();

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



  /////////////////////////////////////////////////////////////////////////////
  /// Dump enclosure
  /////////////////////Beam dump entrance concrete (BDEC)//////////////////////
  par[1] = BmDEX/2 + BDWth;
  par[2] = BmDEY/2 + BDWth;
  par[3] = BmDEZ/2;
  G4Box* solidBDECwall = new G4Box("BDECwall",par[1],par[2],par[3]);

  //Remove core leaving the concrete walls
  par[1] = BmDEX/2;
  par[2] = BmDEY/2;
  par[3] = BmDEZ/2 + 1*cm;
  G4Box* solidBMDE = new G4Box("BMDE",par[1],par[2],par[3]);
  G4SubtractionSolid* solidBDEC = new G4SubtractionSolid("BDECwall-BMDE",solidBDECwall, solidBMDE,0,G4ThreeVector());
  G4LogicalVolume* logicBDEC = new G4LogicalVolume(solidBDEC,GetMaterial("Concrete"),"BDEC");
  X = 0.0;
  Y = -0.5*(BmDEY-BmDmY);
  Z = (sqrt(pow(Rhall,2)-pow((BmDEX/2+abs(TgX)),2)) + BmDEZ/2);
  (void*)new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBDEC,"BDEC",logicWorld, false,0,checkOverlaps);
  G4VisAttributes* BDECvis = new G4VisAttributes(1);
  BDECvis->SetForceWireframe(false);
  BDECvis->SetColor(0.0,1.0,0.0);
  BDECvis->SetDaughtersInvisible(false);
  logicBDEC->SetVisAttributes(BDECvis);

  /////////////////////////Beam Dump concrete (BMDC)//////////////////////////
  par[1] = BmDmX/2 + BDWth;
  par[2] = BmDmY/2 + BDWth;
  par[3] = BmDmZ/2 + BDWthz/2;
  G4Box* solidBMDCwall = new G4Box("BMDC",par[1],par[2],par[3]);
  //Remove core leaving the concrete walls
  par[1] = BmDmX/2;
  par[2] = BmDmY/2;
  par[3] = BmDmZ/2 + 1*cm;  //sometimes you need to cut more to see it on the surface
  G4Box* solidBMDM = new G4Box("BMDM",par[1],par[2],par[3]);
  G4SubtractionSolid* solidBMDC = new G4SubtractionSolid("BMDCwall-BMDM",solidBMDCwall, solidBMDM,0,G4ThreeVector(0,0,-BDWthz/2));
  G4LogicalVolume* logicBMDC = new G4LogicalVolume(solidBMDC,GetMaterial("Concrete"),"BMDC");
  X = 0;
  Y = 0;
  Z = (sqrt(pow(Rhall,2)-pow(BmDEX/2+abs(TgX),2)) + BmDEZ + BmDmZ/2 + BDWthz/2);
  (void*)new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBMDC,"BMDC",logicWorld, false,0,checkOverlaps);
  G4VisAttributes* BMDCvis = new G4VisAttributes(1);
  BMDCvis->SetColor(0.0,1.0,0.0);
  BMDCvis->SetForceWireframe(false);
  logicBMDC->SetVisAttributes(BMDCvis);

  /////////////////////////////////////////////////////////////////////////////
  ////////////////////////Beam Dump Water barrel///////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  //G4double BDWBposZ = 2570*cm; //Distance from Wall
  BuildWaterBarrel(logicWorld,0,G4ThreeVector(0.,0.,Rhall + 2570*cm -
        BDWLZ/2.));

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



 //Beam Line Tube 8 (BLT8)
 G4Tubs* solidBLT8 = new G4Tubs("BLT8",0,BPtubeOD[7]/2,BPtubeLength[7]/2,0,2*pi);
 G4LogicalVolume* logicBLT8 = new G4LogicalVolume(solidBLT8,GetMaterial("Vacuum"),"BLT8");
 X = 0;
 Y = 0;
 Z = BLToffset + BPtubeLength[0] + BPtubeLength[1] + BPtubeLength[2] + BPtubeLength[3] + BPtubeLength[4] + BPtubeLength[5] + BPtubeLength[6] + BPtubeLength[7]/2;
 (void*)new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBLT8,"BLT8",logicWorld,false,0,checkOverlaps);
 G4VisAttributes* BLT8vis = new G4VisAttributes(1);
 BLT8vis->SetColor(0.0,1.0,0.0);
 logicBLT8->SetVisAttributes(BLT8vis);

 G4Tubs* solidBLT8W = new G4Tubs("BLT8W",BPtubeID[7]/2,BPtubeOD[7]/2,BPtubeLength[7]/2,0,2*pi);
 G4LogicalVolume* logicBLT8W = new G4LogicalVolume(solidBLT8W,GetMaterial("BLT8mat"),"BLT8W");
 X = 0;
 Y = 0; 
 Z = 0;
 (void*)new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBLT8W,"BLT8W",logicBLT8,false,0,checkOverlaps);
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
 (void*)new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBWBT,"BWBT",logicWorld,false,0,checkOverlaps);
 G4VisAttributes* BWBTvis = new G4VisAttributes(1);
 BWBTvis->SetColor(0.0,1.0,0.0);
 logicBWBT->SetVisAttributes(BWBTvis);

 G4Tubs* solidBWBTW = new G4Tubs("BWBTW",BWBT_ID/2,BWBT_OD/2,BWBT_length/2,0,2*pi);
 G4LogicalVolume* logicBWBTW = new G4LogicalVolume(solidBWBTW,GetMaterial("BLT8mat"),"BWBTW");
 X = 0;
 Y = 0; 
 Z = 0;
 (void*)new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBWBTW,"BWBTW",logicBWBT,false,0,checkOverlaps);
 G4VisAttributes* BWBTWvis = new G4VisAttributes(1);
 BWBTWvis->SetColor(0.0,1.0,0.0);
 logicBWBTW->SetVisAttributes(BWBTWvis);

}

G4Material* BeamDump::GetMaterial(G4String mat){

  return fDetCon->GetMaterial(mat);

}

BeamDump::~BeamDump(){;}

void BeamDump::BuildWaterBarrel(G4LogicalVolume *logicWorld,
    G4RotationMatrix *rot, G4ThreeVector pos)
{

  /////////////////////////////////////////////////////////////////////////////
  ////////////////////////Beam Dump Water barrel///////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  G4bool checkOverlaps = true;
  const G4double conv = 2.54; //conversion from inches to cm

  G4double par[10];
  G4double X,Y,Z;
  G4double BDWRin = 53*cm; //Beam dump water barrel params
  G4double BDWRout = 54.8*cm;
  //G4double BDWLZ = 380*cm;
  //G4double REVBD1 = 30.5*cm; //Inner radius for barrel end cap (only 1 side)

  //Water volume in the Beamdump water barrel - WBWV
  par[1] = 0;
  par[2] = BDWRout;
  par[3] = BDWLZ/2;
  G4Tubs* solidWBWV = new G4Tubs("WBWV",par[1],par[2],par[3],0,2*pi);
  G4LogicalVolume* logicWBWV = new G4LogicalVolume(solidWBWV,GetMaterial("Water"),"WBWV");
  //G4double BDWBposZ = 2570*cm; //Distance from Wall
  X = pos.x();
  Y = pos.y();
  Z = pos.z() + par[3];
  (void*)new G4PVPlacement(rot,G4ThreeVector(X,Y,Z),logicWBWV,"WBWV",logicWorld,
      false,0,checkOverlaps);
  G4VisAttributes* WBWVvis = new G4VisAttributes(1);
  WBWVvis->SetForceWireframe(false);
  WBWVvis->SetDaughtersInvisible(false);
  WBWVvis->SetColor(0.0,1.0,1.0);
  logicWBWV->SetVisAttributes(WBWVvis);

  //Beamdump water barrel walls - BDWB
  par[1] = BDWRin;
  par[2] = BDWRout;
  par[3] = BDWLZ/2;
  G4Tubs* solidBDWB = new G4Tubs("BDWB",par[1],par[2],par[3],0,2*pi);
  G4LogicalVolume* logicBDWB = new G4LogicalVolume(solidBDWB,GetMaterial("Aluminum"),"BDWB");
  X = 0;
  Y = 0;
  Z = 0;
  (void*)new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBDWB,"BDWB",logicWBWV, false,0,checkOverlaps);
  G4VisAttributes* BDWBvis = new G4VisAttributes(1);
  BDWBvis->SetForceWireframe(false);
  BDWBvis->SetColor(1.0,1.0,1.0);
  BDWBvis->SetDaughtersInvisible(false);
  logicBDWB->SetVisAttributes(BDWBvis);

  //Beam dump Exchanger tube - BDET
  //G4double BDETRin = 9.63*conv*cm;
  G4double BDETRout = 10.005*conv*cm;
  G4double BDETlength = 126*conv*cm;
  par[1] = 0;
  par[2] = BDETRout;
  par[3] = BDETlength/2;
  G4Tubs* solidBDET = new G4Tubs("BDET",par[1],par[2],par[3],0,2*pi);
  G4LogicalVolume* logicBDET = new G4LogicalVolume(solidBDET,GetMaterial("Water"),"BDET");
  X = 0;
  Y = 0;
  Z = BDWLZ/2 - BDETlength/2;
  (void*)new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBDET,"BDET",logicWBWV, false,0,checkOverlaps);
  G4VisAttributes* BDETvis = new G4VisAttributes(1);
  BDETvis->SetForceWireframe(false);
  BDETvis->SetColor(0.0,0.0,1.0);
  BDETvis->SetDaughtersInvisible(false);
  logicBDET->SetVisAttributes(BDETvis);

  //Up/Down Plates - BDUP
  G4double BDUPRin = 0;
  G4double BDUPRout = 9.625*conv*cm;
  G4double BDUPlength = 13.38*conv*cm;
  //G4double AlTotlength = 42.4*conv*cm; //BDUPlength + BDFBlength
  par[1] = BDUPRin;
  par[2] = BDUPRout;
  par[3] = BDUPlength/2;
  G4Tubs* solidBDUP = new G4Tubs("BDUP",par[1],par[2],par[3],0,2*pi);
  G4LogicalVolume* logicBDUP = new G4LogicalVolume(solidBDUP,GetMaterial("Aluminum"),"BDUP");
  X = 0;
  Y = 0;
  Z = (BDETlength/2 - BDUPlength/2);
  (void*)new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBDUP,"BDUP",logicBDET, false,0,checkOverlaps);
  G4VisAttributes* BDUPvis = new G4VisAttributes(1);
  BDUPvis->SetForceWireframe(false);
  BDUPvis->SetDaughtersInvisible(false);
  logicBDUP->SetVisAttributes(BDUPvis);

  //Fin Plate block - BDFB
  G4double BDFBx = 14*conv*cm;
  G4double BDFBy = 14*conv*cm;
  G4double BDFBlength = 29.02*conv*cm;
  par[1] = BDFBx/2;
  par[2] = BDFBy/2;
  par[3] = BDFBlength/2;
  G4Box* solidBDFB = new G4Box("BDFB",par[1],par[2],par[3]);
  G4LogicalVolume* logicBDFB = new G4LogicalVolume(solidBDFB,GetMaterial("Aluminum"),"BDFB");
  X = 0;
  Y = 0;
  Z = (BDETlength/2 - BDUPlength - BDFBlength/2);
  (void*)new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicBDFB,"BDFB",logicBDET, false,0,checkOverlaps);
  G4VisAttributes* BDFBvis = new G4VisAttributes(1);
  BDFBvis->SetForceWireframe(false);
  BDFBvis->SetDaughtersInvisible(false);
  logicBDFB->SetVisAttributes(BDFBvis);

  //////////////Kill particles at BMDC water barrel (Beam Dump)////////////

  /* Should we kill particles at the water barrel? */
  if( gRadConfig->StopParticluesAtDump ) {
    G4cout << "Stopping particles *at* dump water barrel" << G4endl;
    BMDCDetSD* BMDCDet = new BMDCDetSD("/BMDCDet");
    G4SDManager* BMDCsdman = G4SDManager::GetSDMpointer();
    BMDCsdman->AddNewDetector(BMDCDet);
    logicWBWV->SetSensitiveDetector(BMDCDet);
    logicBDWB->SetSensitiveDetector(BMDCDet);
    logicBDET->SetSensitiveDetector(BMDCDet);
    logicBDUP->SetSensitiveDetector(BMDCDet);
    logicBDFB->SetSensitiveDetector(BMDCDet);
  } else { // Otherwise they'll be killed at the end of the water barrel
    // just to prevent tracking through all the soil behind the beam dump
    G4cout << "Stopping particles *after* dump water barrel" << G4endl;
    G4Tubs *solParticleStop = new G4Tubs("BeamDumpParticleStop",
        0.,BDWRout,1*mm/2.,0.,2*pi);
    G4LogicalVolume *logicParticleStop = new G4LogicalVolume(solParticleStop,
        GetMaterial("Nitrogen"),"BeamDumpParticleStop");
    (void*)new G4PVPlacement(0, G4ThreeVector(pos.x(),pos.y(),
          pos.z() + BDWLZ + 1.0*mm ),
        logicParticleStop,"BeamDumpParticleStop",logicWorld,
        false,0,checkOverlaps);
    logicParticleStop->SetVisAttributes(new G4VisAttributes(
          G4Colour(1.,0.,0.,0.99)));
    // Finally, stop the particles by placing a step limiter
    logicParticleStop->SetUserLimits(new G4UserLimits(0.,0.,0.,DBL_MAX,
          DBL_MAX));
  }
}
