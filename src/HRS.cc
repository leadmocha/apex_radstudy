#include "HRS.hh"
#include "G4NistManager.hh"
#include "B1DetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Trap.hh"
#include "G4SubtractionSolid.hh"
#include "G4GenericTrap.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4VisAttributes.hh"

#include "G4ThreeVector.hh"

#include "HallA.hh"

#include "G4SDManager.hh"
#include "HRSDetSDL.hh"
#include "HRSDetSDR.hh"

#include "RadConfig.hh"
#include "G4GDMLParser.hh"

HRS::HRS(G4double theta, B1DetectorConstruction *dc, HallA *ha, G4LogicalVolume *logicWorld):fDetCon(dc),thetaHRS(theta){

  //Variables of HallA class
  G4double Rhall = ha->getRhall();
  G4double bmheight = ha->getbmheight();
  G4double Tair1 = ha->getTair1();

  G4bool checkOverlaps = true;

  //dimentions are in inches
  const G4double conv = 2.54; //conversion from inches to cm
  G4double par[12];
  G4double X,Y,Z;

  // Build Q1

  // First, determine an angle to cut into the HRS to leave room for the
  // beamline:
  G4double cut_angle; // It's used to cut a piece away for the beamline
  if(thetaHRS > 0)
    {
      cut_angle = -12.5*deg;
    }
  else
    {
      cut_angle = 12.5*deg;
    }
  rotTubeY = new G4RotationMatrix();
  rotTubeY->rotateY(cut_angle);
  switch( gRadConfig->Q1Version ) {
    case RadQ1Version::COPPER:
      MakeQ1Copper(logicWorld);
      break;
    case RadQ1Version::SUPERCONDUCTING:
      MakeQ1Superconducting(logicWorld,cut_angle);
      break;
    default:
      break;
  }

 //////////////////////////////Q2//////////////////////////////////////////
  G4double Q2polelength = 180*cm;
  G4double Q2poleID = 115*cm;
  G4double Q2poleOD = 155*cm;
  par[1] = 0;
  par[2] = Q2poleOD/2;
  par[3] = Q2polelength/2;
  G4Tubs* solidQ2 = new G4Tubs("Q2",par[1],par[2],par[3],0,2*pi);

  //Remove part for beamline
  par[1] = 2*conv*cm;
  par[2] = 200*cm;
  par[3] = 200*cm;
  G4Box* solidQ2Cut1 = new G4Box("Q2Cut1",par[1],par[2],par[3]);
  if(thetaHRS > 0)
    {
      cut_angle = -12.5*deg;
      X = 462*cm*sin(cut_angle) - par[1];
    }
  else
    {
      cut_angle = 12.5*deg;
      X = 462*cm*sin(cut_angle) + par[1];
    }
  Y = 0;
  Z = 0;
  G4SubtractionSolid* solidQ2_Q2Cut1 = new G4SubtractionSolid("Q2-Q2Cut1",solidQ2, solidQ2Cut1,rotTubeY,G4ThreeVector(X,Y,Z));
  par[1] = 0;
  par[2] = 6.5*conv*cm;
  par[3] = 200*cm;
  G4Tubs* solidQ2Cut2 = new G4Tubs("Q2Cut2",par[1],par[2],par[3],0,2*pi);
  G4SubtractionSolid* solidQ2_Q2Cut1_Q2Cut2 = new G4SubtractionSolid("Q2Cut1-Q2Cut2",solidQ2_Q2Cut1, solidQ2Cut2,rotTubeY,G4ThreeVector(X,Y,Z));

  G4LogicalVolume* logicQ2 = new G4LogicalVolume(solidQ2_Q2Cut1_Q2Cut2, GetMaterial("Iron"),"Q2");
  G4RotationMatrix *Q2angle = new G4RotationMatrix;
  Q2angle->rotateX(180*deg);
  Q2angle->rotateY(thetaHRS);
  X = 462*cm*sin(thetaHRS);
  Y = 0;
  Z = 462*cm*cos(thetaHRS);
  G4VPhysicalVolume* physQ2 = new G4PVPlacement(Q2angle,G4ThreeVector(X,Y,Z),logicQ2,"Q2",logicWorld,false,0,checkOverlaps);
  G4VisAttributes* Q2vis = new G4VisAttributes(1);
  Q2vis->SetForceWireframe(false);
  logicQ2->SetVisAttributes(Q2vis);

  par[1] = 0;
  par[2] = Q2poleID/2;
  par[3] = Q2polelength/2;
  G4Tubs* solidQ2_vac = new G4Tubs("Q2_vac",par[1],par[2],par[3],0,2*pi);
  G4LogicalVolume* logicQ2_vac = new G4LogicalVolume(solidQ2_vac, GetMaterial("Vacuum"),"Q2_vac");
  X = 0;
  Y = 0;
  Z = 0;
  G4VPhysicalVolume* physQ2_vac = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicQ2_vac,"Q2_vac",logicQ2,false,0,checkOverlaps);
  G4VisAttributes* Q2_vacvis = new G4VisAttributes(1);
  Q2_vacvis->SetColor(0.0,1.0,0.0);
  Q2_vacvis->SetForceWireframe(false);
  logicQ2_vac->SetVisAttributes(Q2_vacvis);

 ////////////////////////////////Dipole////////////////////////////////////
  par[1] = 650*cm;
  par[2] = 1021.5*cm;
  par[3] = 218/2*cm;
  G4Tubs* solidDipCover = new G4Tubs("DipCover",par[1],par[2],par[3],0,pi/4);
  G4LogicalVolume* logicDipCover = new G4LogicalVolume(solidDipCover, GetMaterial("Iron"),"DipCover");
  X = 996*cm*sin(thetaHRS);
  Y = 840*cm;
  Z = 996*cm*cos(thetaHRS);

  G4RotationMatrix* rot90Y = new G4RotationMatrix();
  rot90Y->rotateY(90*deg);
  G4RotationMatrix* rot90X = new G4RotationMatrix();
  rot90X->rotateX(90*deg);

  G4RotationMatrix* rot = new G4RotationMatrix();
  rot->rotateX(270*deg);
  rot->rotateY(90*deg);
  rot->rotateX(thetaHRS);

  G4VPhysicalVolume* physDipCover = new G4PVPlacement(rot,G4ThreeVector(X,Y,Z),logicDipCover,"DipCover",logicWorld,false,0,checkOverlaps);
  G4VisAttributes* DipCovervis = new G4VisAttributes(1);
  DipCovervis->SetColor(0.0,1.0,0.0);
  DipCovervis->SetForceWireframe(false);
  logicDipCover->SetVisAttributes(DipCovervis);

  par[1] = 650*cm;
  par[2] = 1021.5*cm;
  par[3] = 76/2*cm;
  G4Tubs* solidRail = new G4Tubs("Rail",par[1],par[2],par[3],0,pi/4);
  G4LogicalVolume* logicRail = new G4LogicalVolume(solidRail, GetMaterial("Iron"),"Rail");
  X = 0*cm;
  Y = 0*cm;
  Z = 0*cm;
  G4VPhysicalVolume* physRail = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicRail,"Rail",logicDipCover,false,0,checkOverlaps);
  G4VisAttributes* Railvis = new G4VisAttributes(1);
  Railvis->SetColor(1.0,0.0,0.0);
  Railvis->SetForceWireframe(false);
  logicRail->SetVisAttributes(Railvis);

  par[1] = 726*cm;
  par[2] = 954*cm;
  par[3] = 76/2*cm;
  G4Tubs* solidAirgap = new G4Tubs("Airgap",par[1],par[2],par[3],0,pi/4);
  G4LogicalVolume* logicAirgap = new G4LogicalVolume(solidAirgap, GetMaterial("Vacuum"),"Airgap");
  X = 0*cm;
  Y = 0*cm;
  Z = 0*cm;
  G4VPhysicalVolume* physAirgap = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicAirgap,"Airgap",logicRail,false,0,checkOverlaps);
  G4VisAttributes* Airgapvis = new G4VisAttributes(1);
  Airgapvis->SetColor(0.0,0.0,1.0);
  Airgapvis->SetForceWireframe(false);
  logicAirgap->SetVisAttributes(Airgapvis);

  par[1] = 765*cm;
  par[2] = 915*cm;
  par[3] = 23.5/2*cm;
  G4Tubs* solidPole = new G4Tubs("Pole",par[1],par[2],par[3],0,pi/4);
  G4LogicalVolume* logicPole = new G4LogicalVolume(solidPole, GetMaterial("Iron"),"Pole");
  X = 0*cm;
  Y = 0*cm;
  Z = 76/2*cm - 23.5/2*cm;
  G4VPhysicalVolume* physPole1 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicPole,"Pole1",logicAirgap,false,0,checkOverlaps);
  G4VisAttributes* Polevis = new G4VisAttributes(1);
  Polevis->SetColor(0.0,1.0,1.0);
  Polevis->SetForceWireframe(false);
  logicPole->SetVisAttributes(Polevis);

  X = 0*cm;
  Y = 0*cm;
  Z = -76/2*cm + 23.5/2*cm;
  G4VPhysicalVolume* physPole2 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicPole,"Pole2",logicAirgap,false,0,checkOverlaps);


//////////////////////////////Q3//////////////////////////////////////////
  G4double Q3polelength = 180*cm;
  G4double Q3poleID = 115*cm;
  G4double Q3poleOD = 155*cm;
  par[1] = 0;
  par[2] = Q3poleOD/2;
  par[3] = Q3polelength/2;
  G4Tubs* solidQ3 = new G4Tubs("Q3",par[1],par[2],par[3],0,2*pi);
  G4LogicalVolume* logicQ3 = new G4LogicalVolume(solidQ3, GetMaterial("Vacuum"),"Q3");
  G4RotationMatrix *Q3angle = new G4RotationMatrix;
  Q3angle->rotateY(-thetaHRS);
  Q3angle->rotateX(45*deg);
  X = 1767.4*cm*sin(thetaHRS);
  Y = 423.3*cm;
  Z = 1767.4*cm*cos(thetaHRS);
  G4VPhysicalVolume* physQ3 = new G4PVPlacement(Q3angle,G4ThreeVector(X,Y,Z),logicQ3,"Q3",logicWorld,false,0,checkOverlaps);
  G4VisAttributes* Q3vis = new G4VisAttributes(1);
  Q3vis->SetColor(0.0,1.0,0.0);
  Q3vis->SetForceWireframe(false);
  logicQ3->SetVisAttributes(Q3vis);

  par[1] = Q3poleID/2;
  par[2] = Q3poleOD/2;
  par[3] = Q3polelength/2;
  G4Tubs* solidQ3_wall = new G4Tubs("Q3_wall",par[1],par[2],par[3],0,2*pi);
  G4LogicalVolume* logicQ3_wall = new G4LogicalVolume(solidQ3_wall, GetMaterial("Iron"),"Q3_wall");
  X = 0;
  Y = 0;
  Z = 0;
  G4VPhysicalVolume* physQ3_wall = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicQ3_wall,"Q3_wall",logicQ3,false,0,checkOverlaps);


/////////////////////////////////Box Beam/////////////////////////////////

  //Box beam part1
  G4double bbp1_dx1W = 48.0;  //part1 x length at y=-dy1 and at -dz
  G4double bbp1_dx2W = 48.0;  //part1 x length at y=+dy1 and at -dz
  G4double bbp1_dx3W = 92.0;  //part1 x length at y=-dy2 and at +dz 
  G4double bbp1_dx4W = 92.0;  //part1 x length at y=+dy2 and at +dz 
  G4double bbp1_dy1W = 13.0; //length at -dz 
  G4double bbp1_dy2W = 88.0; //length at +dz 
  G4double bbp1_dzW = 364.75; //part1 z length
  G4double bbp1_thetaW = 3.5*deg; //polar angle of the center line joining faces
  G4double bbp1_phiW = 0*deg;          //azimuthal angle
  G4double bbp1_Alp1W= 0, bbp1_Alp2W = 0;

  par[1] = 0.5*conv*bbp1_dzW*cm;
  par[2] = bbp1_thetaW;
  par[3] = bbp1_phiW;
  par[4] = 0.5*conv*bbp1_dy1W*cm;
  par[5] = 0.5*conv*bbp1_dx1W*cm;
  par[6] = 0.5*conv*bbp1_dx2W*cm;
  par[7] = bbp1_Alp1W;
  par[8] = 0.5*conv*bbp1_dy2W*cm;
  par[9] = 0.5*conv*bbp1_dx3W*cm;
  par[10] = 0.5*conv*bbp1_dx4W*cm;
  par[11] = bbp1_Alp2W;

  G4Trap* bbp1_wall = new G4Trap("bbp1_wall",par[1],par[2],par[3],par[4],par[5],par[6],par[7],par[8],par[9],par[10],par[11]);
  G4LogicalVolume* logicbbp1_wall = new G4LogicalVolume(bbp1_wall, GetMaterial("Iron"),"bbp1_wall");
  G4RotationMatrix* bbangle = new G4RotationMatrix();
  bbangle->rotateZ(270*deg);
  bbangle->rotateY(0*deg);
  bbangle->rotateX(-thetaHRS);
  X = (0.5*bbp1_dzW*conv*cm + 110*cm)*sin(thetaHRS);
  Y = 0.5*70*conv*cm + 205*cm;
  Z = (0.5*bbp1_dzW*conv*cm + 110*cm)*cos(thetaHRS);
  G4VPhysicalVolume* physbbp1_wall = new G4PVPlacement(bbangle,G4ThreeVector(X,Y,Z),logicbbp1_wall,"bbp1_wall",logicWorld,false,0,checkOverlaps);


  //Inside of box beam
  G4double bbp1_dx1 = 46.0;  //part1 x length at y=-dy1 and at -dz
  G4double bbp1_dx2 = 46.0;  //part1 x length at y=+dy1 and at -dz
  G4double bbp1_dx3 = 90.0;  //part1 x length at y=-dy2 and at +dz
  G4double bbp1_dx4 = 90.0;  //part1 x length at y=+dy2 and at +dz
  G4double bbp1_dy1 = 11.0; //length at -dz
  G4double bbp1_dy2 = 86.0; //length at +dz
  G4double bbp1_dz = 362.75; //part1 z length
  G4double bbp1_theta = 3.5*deg; //polar angle of the center line joining faces
  G4double bbp1_phi = 0*deg;          //azimuthal angle
  G4double bbp1_Alp1=0, bbp1_Alp2 =0;


  par[1] = 0.5*conv*bbp1_dz*cm;
  par[2] = bbp1_theta;
  par[3] = bbp1_phi;
  par[4] = 0.5*conv*bbp1_dy1*cm;
  par[5] = 0.5*conv*bbp1_dx1*cm;
  par[6] = 0.5*conv*bbp1_dx2*cm;
  par[7] = bbp1_Alp1;
  par[8] = 0.5*conv*bbp1_dy2*cm;
  par[9] = 0.5*conv*bbp1_dx3*cm;
  par[10] = 0.5*conv*bbp1_dx4*cm;
  par[11] = bbp1_Alp2;

  G4Trap* bbp1 = new G4Trap("bbp1",par[1],par[2],par[3],par[4],par[5],par[6],par[7],par[8],par[9],par[10],par[11]);
  G4LogicalVolume* logicbbp1 = new G4LogicalVolume(bbp1, GetMaterial("Air"),"bbp1");
  G4VPhysicalVolume* physbbp1 = new G4PVPlacement(0,G4ThreeVector(),logicbbp1,"bbp1",logicbbp1_wall,false,0,checkOverlaps);


  G4VisAttributes* bbp1vis = new G4VisAttributes(1);
  bbp1vis->SetColor(0.0,0.0,1.0);
  bbp1vis->SetForceWireframe(false);
  logicbbp1->SetVisAttributes(bbp1vis);
  logicbbp1_wall->SetVisAttributes(bbp1vis);

  //Concrete part

  std::vector<G4TwoVector> vertices;
  vertices.push_back(G4TwoVector( -110*conv*cm, -12*conv*cm));
  vertices.push_back(G4TwoVector( -110*conv*cm,  12*conv*cm));
  vertices.push_back(G4TwoVector(  168*conv*cm,  41.5*conv*cm));
  vertices.push_back(G4TwoVector(  168*conv*cm, -41.5*conv*cm));
  vertices.push_back(G4TwoVector( -51*conv*cm,  -18*conv*cm));
  vertices.push_back(G4TwoVector( -51*conv*cm,   18*conv*cm));
  vertices.push_back(G4TwoVector( -25*conv*cm,   21*conv*cm));
  vertices.push_back(G4TwoVector( -25*conv*cm,  -21*conv*cm));     

  G4double pDz = 36;
  G4GenericTrap* trap = new G4GenericTrap("trap",0.5*pDz*conv*cm,vertices);
  G4LogicalVolume* logicTrap = new G4LogicalVolume(trap, GetMaterial("Concrete"),"trap");
  G4RotationMatrix* trapRot = new G4RotationMatrix();
  X = (27 - pDz/2)*conv*cm; //28.5 is the half height of the box beam at the maximum concrete height.
  Y = 0;
  Z = 0;
  trapRot->rotateY(270*deg);
  trapRot->rotateZ(180*deg);
  G4VPhysicalVolume* physTrap = new G4PVPlacement(trapRot,G4ThreeVector(X,Y,Z),logicTrap,"trap",logicbbp1,false,0,checkOverlaps);


  std::vector<G4TwoVector> vertices1;
  vertices1.push_back(G4TwoVector( -110*conv*cm, -12*conv*cm));
  vertices1.push_back(G4TwoVector( -110*conv*cm,  12*conv*cm));
  vertices1.push_back(G4TwoVector(  168*conv*cm,  41.5*conv*cm));
  vertices1.push_back(G4TwoVector(  168*conv*cm, -41.5*conv*cm));
  vertices1.push_back(G4TwoVector( -110*conv*cm, -12*conv*cm));
  vertices1.push_back(G4TwoVector( -110*conv*cm,  12*conv*cm));
  vertices1.push_back(G4TwoVector(  168*conv*cm,  41.5*conv*cm));
  vertices1.push_back(G4TwoVector(  168*conv*cm, -41.5*conv*cm));     

  pDz = 24.5;
  G4GenericTrap* trap1 = new G4GenericTrap("trap1",0.5*pDz*conv*cm,vertices1);
  G4LogicalVolume* logicTrap1 = new G4LogicalVolume(trap1, GetMaterial("Concrete"),"trap1");
  G4RotationMatrix* trapRot1 = new G4RotationMatrix();
  X = -(33.5 - pDz/2)*conv*cm; //33.5 is other half height
  Y = 0;
  Z = 0;
  trapRot1->rotateY(270*deg);
  trapRot1->rotateZ(180*deg);
  G4VPhysicalVolume* physTrap1 = new G4PVPlacement(trapRot1,G4ThreeVector(X,Y,Z),logicTrap1,"trap1",logicbbp1,false,0,checkOverlaps);

  std::vector<G4TwoVector> vertices2;
  vertices2.push_back(G4TwoVector( -32.5*conv*cm, -20.9*conv*cm));
  vertices2.push_back(G4TwoVector( -32.5*conv*cm,  20.9*conv*cm));
  vertices2.push_back(G4TwoVector(  32.5*conv*cm,  20.9*conv*cm));
  vertices2.push_back(G4TwoVector(  32.5*conv*cm, -20.9*conv*cm));
  vertices2.push_back(G4TwoVector( -42.7*conv*cm, -5.1*conv*cm));
  vertices2.push_back(G4TwoVector( -42.7*conv*cm,  20.9*conv*cm));
  vertices2.push_back(G4TwoVector(  42.7*conv*cm,  20.9*conv*cm));
  vertices2.push_back(G4TwoVector(  42.7*conv*cm, -5.1*conv*cm));     

  pDz = 100;
  G4GenericTrap* trap2 = new G4GenericTrap("trap2",0.5*pDz*conv*cm,vertices2);
  G4LogicalVolume* logicTrap2 = new G4LogicalVolume(trap2, GetMaterial("Concrete"),"trap2");
  G4RotationMatrix* trapRot2 = new G4RotationMatrix();
  trapRot2->rotateY(-thetaHRS);
  X = (182.375*conv*cm + 39*conv*cm + 112*conv*cm)*sin(thetaHRS);
  Y = 205*cm - 20.9*conv*cm; //81.4375*conv*cm - 20.9*conv*cm;
  Z = (182.375*conv*cm + 39*conv*cm + 112*conv*cm)*cos(thetaHRS);

  G4VPhysicalVolume* physTrap2 = new G4PVPlacement(trapRot2,G4ThreeVector(X,Y,Z),logicTrap2,"trap2",logicWorld,false,0,checkOverlaps);


 ////////////////////////Shield Hut/////////////////////////////////////


  G4double th_l_w = 16.52; //Thickness of the left concrete wall
  G4double th_r_w = 31.50; //Thickness of the right concrete wal
  G4double th_ru_w = 16.52; //Thickness of the right wall (upper)
  G4double th_door = 31.50; //Thickness of the door concrete
  G4double th_floor = 31.50; //Thickness of the floor concrete
  G4double th_rear = 7.94; //Thickness of the rear wall concrete
  G4double th_ceiling = 10.81; //Thickness of the ceiling concrete
  G4double slr_walls = 148.500; //Space between side walls
  G4double sfr_walls = 294.625; //Space between front-rear walls
  G4double sud_walls = 322; //Space between up-down walls
  G4double th_steel = 3.0; //Thickness of the steel plates
  G4double th_lead_s = 1.0; //Thickn. of the lead plates (sides)
  G4double th_lead_f = 3.0; //Thickn. of the lead plates (front)
  G4double th_lead_b = 1.0; //Thickn. of the lead plates (bottom)
  G4double h_thin_rw = 175.6; //Height of thin part of the right wall

  G4double xshut, yshut, zshut;

  //Shielding House Box (concrete)
  par[1] = 0.5*conv*(slr_walls + th_l_w+th_r_w + 2*(th_steel+th_lead_s))*cm;
  par[2] = 0.5*conv*(sud_walls + th_floor+th_ceiling + 2*th_steel+th_lead_b)*cm;
  par[3] = 0.5*conv*(sfr_walls + th_door + th_rear + 2*th_steel + th_lead_f)*cm;
  xshut = par[1];
  yshut = par[2];
  zshut = par[3];
  G4Box* solidShut = new G4Box("shut", par[1], par[2], par[3]);
 
  //Remove upper right or upper left airbox (Air)
  par[1] = 0.5*conv*(th_r_w - th_ru_w)*cm;
  par[2] = 0.5*conv*h_thin_rw*cm;
  par[3] = 0.5*conv*(sfr_walls + th_door + th_rear + 2*th_steel + th_lead_f)*cm + 0.05*cm;
  G4Box* solidSHRU = new G4Box("SHRU", par[1], par[2], par[3]);
  if(thetaHRS>0)
    {
      X = -xshut + par[1] - 0.05*cm;
    }
  else
    {
      X = -(-xshut + par[1] - 0.05*cm);
    }
  Y = yshut - par[2];
  Z = 0;
  G4SubtractionSolid* solidShut_SHRU = new G4SubtractionSolid("Shut-SHRU",solidShut, solidSHRU,0,G4ThreeVector(X,Y,Z));

  //Remove top corner
  par[1] = 0.5*conv*(slr_walls + th_l_w+th_r_w + 2*(th_steel+th_lead_s))*cm + 0.1*cm;
  par[2] = 0.5*conv*60*cm;
  par[3] = 0.5*conv*60*cm;
  G4Box* solidSHTC = new G4Box("SHTC", par[1], par[2], par[3]);
  X = 0.1*cm;
  Y = 0.5*conv*(sud_walls + th_floor+th_ceiling + 2*th_steel+th_lead_b)*cm;
  Z = 0.5*conv*(sfr_walls + th_door + th_rear + 2*th_steel + th_lead_f)*cm;
  G4RotationMatrix* rot45X = new G4RotationMatrix();
  rot45X->rotateX(45*deg);
  G4SubtractionSolid* solidShut_SHRU_SHTC = new G4SubtractionSolid("Shut-SHRU-SHTC",solidShut_SHRU, solidSHTC,rot45X,G4ThreeVector(X,Y,Z));

  //Remove bottom center front for Q3
  G4double Q3Box_x = 200*cm;
  G4double Q3Box_y = 200*cm;
  G4double Q3Box_z = 200*cm;
  par[1] = Q3Box_x/2;
  par[2] = Q3Box_y/2;
  par[3] = Q3Box_z/2;
  G4Box* solidQ3Box = new G4Box("Q3Box",par[1],par[2],par[3]);
  X = 0;
  Y = -0.5*conv*(sud_walls + th_floor+th_ceiling + 2*th_steel+th_lead_b)*cm + Q3Box_y/2;
  Z = -0.5*conv*(sfr_walls + th_door + th_rear + 2*th_steel + th_lead_f)*cm + Q3Box_z/2 - 0.01*cm;

  G4SubtractionSolid* solidShut_SHRU_SHTC_Q3Box = new G4SubtractionSolid("Shut-SHRU-SHTC-Q3Box",solidShut_SHRU_SHTC, solidQ3Box,0,G4ThreeVector(X,Y,Z));

  G4LogicalVolume* logicShut = new G4LogicalVolume(solidShut_SHRU_SHTC_Q3Box, GetMaterial("Concrete"),"Shut");
  G4VisAttributes* Shutvis = new G4VisAttributes(1);
  Shutvis->SetColor(0.0,1.0,1.0);
  Shutvis->SetForceWireframe(false);
  logicShut->SetVisAttributes(Shutvis);
  X = 866*conv*cm*sin(thetaHRS); 
  Y = -bmheight + 99.61*cm + 58*conv*cm - (156 - 81)*conv*cm + 229*conv*cm + 0.5*conv*(sud_walls + th_floor+th_ceiling + 2*th_steel+th_lead_b)*cm + 10*cm;
  Z = 866*conv*cm*cos(thetaHRS);
  G4RotationMatrix* Shutangle = new G4RotationMatrix();
  Shutangle->rotateY(-thetaHRS);
  G4VPhysicalVolume* physShut = new G4PVPlacement(Shutangle,G4ThreeVector(X,Y,Z),logicShut,"Shut-SHTC",logicWorld,false,0,checkOverlaps);

 //iron layers inside concrete box (Iron)
  par[1] = 0.5*conv*(slr_walls + 2.*(th_steel + th_lead_s))*cm;
  par[2] = 0.5*conv*(sud_walls + 2.*th_steel + th_lead_b)*cm;
  par[3] = 0.5*conv*(sfr_walls + 2.*th_steel + th_lead_f)*cm;
  G4Box* solidSHFE = new G4Box("SHFE", par[1], par[2], par[3]);
 //Remove top corner from Iron layer
  par[1] = 0.5*conv*(slr_walls + th_l_w+th_r_w + 2*(th_steel+th_lead_s))*cm;
  par[2] = 0.5*conv*65*cm;
  par[3] = 0.5*conv*65*cm;
  G4Box* solidFETC = new G4Box("FETC", par[1], par[2], par[3]);
  X = 0;
  Y = 0.5*conv*(sud_walls + th_floor+th_ceiling + 2*th_steel+th_lead_b)*cm;
  Z = 0.5*conv*(sfr_walls + th_door + th_rear + 2*th_steel + th_lead_f - th_door - th_rear)*cm;
  G4SubtractionSolid* solidSHFE_FETC = new G4SubtractionSolid("SHFE_FETC",solidSHFE, solidFETC,rot45X,G4ThreeVector(X,Y,Z));
  //Remove space for Q3. solidQ3Box defined earlier.
  G4double offset;
  if(thetaHRS>0)
    {
      offset = 0.5*conv*(th_r_w - th_l_w)*cm;
    }
  else
    {
      offset = -(0.5*conv*(th_r_w - th_l_w)*cm);
    }
  X = 0 - offset;
  Y = -0.5*conv*(sud_walls + th_floor+th_ceiling + 2*th_steel+th_lead_b)*cm + Q3Box_y/2 - 0.5*conv*(th_floor - th_ceiling)*cm;
  Z = -0.5*conv*(sfr_walls + th_door + th_rear + 2*th_steel + th_lead_f)*cm + Q3Box_z/2 - 0.5*conv*(th_door - th_rear)*cm;

  G4SubtractionSolid* solidSHFE_FETC_Q3Box = new G4SubtractionSolid("SHFE-FETC-Q3Box",solidSHFE_FETC, solidQ3Box,0,G4ThreeVector(X,Y,Z));

  G4LogicalVolume* logicSHFE = new G4LogicalVolume(solidSHFE_FETC_Q3Box, GetMaterial("Iron"),"SHFE"); 
  if(thetaHRS>0)
    {
      X = 0.5*conv*(th_r_w - th_l_w)*cm;
    }
  else
    {
      X = -(0.5*conv*(th_r_w - th_l_w)*cm);
    }
  Y = 0.5*conv*(th_floor - th_ceiling)*cm;
  Z = 0.5*conv*(th_door - th_rear)*cm;
  G4VPhysicalVolume* physSHFE = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicSHFE,"SHFE",logicShut,false,0,checkOverlaps);
  G4VisAttributes* SHFEvis = new G4VisAttributes(1);
  SHFEvis->SetColor(0.0,1.0,0.0);
  SHFEvis->SetForceWireframe(false);
  logicSHFE->SetVisAttributes(SHFEvis);


//lead layers inside the iron box (Lead)
  par[1] = 0.5*conv*(slr_walls + 2*th_lead_s)*cm;
  par[2] = 0.5*conv*(sud_walls + th_lead_b)*cm;
  par[3] = 0.5*conv*(sfr_walls + th_lead_f)*cm;
  G4Box* solidSHPB = new G4Box("SHPB", par[1], par[2], par[3]);
  X = 0;
  Y = 0.5*conv*(sud_walls + th_floor+th_ceiling + 2*th_steel+th_lead_b)*cm;
  Z = 0.5*conv*(sfr_walls + th_door + th_rear + 2*th_steel + th_lead_f - th_door - th_rear)*cm;
  //Remove Top Corner
  G4SubtractionSolid* solidSHPB_FETC = new G4SubtractionSolid("SHPB_FETC",solidSHPB, solidFETC,rot45X,G4ThreeVector(X,Y,Z));
  //Remove space for Q3. solidQ3Box defined earlier.
  X = 0 - offset;
  Y = -0.5*conv*(sud_walls + th_floor+th_ceiling + 2*th_steel+th_lead_b)*cm + Q3Box_y/2 -  0.5*conv*(th_floor - th_ceiling)*cm;
  Z = -0.5*conv*(sfr_walls + th_door + th_rear + 2*th_steel + th_lead_f)*cm + Q3Box_z/2 - 0.5*conv*(th_door - th_rear)*cm;

  G4SubtractionSolid* solidSHPB_FETC_Q3Box = new G4SubtractionSolid("SHPB-FETC-Q3Box",solidSHPB_FETC, solidQ3Box,0,G4ThreeVector(X,Y,Z));

  G4LogicalVolume* logicSHPB = new G4LogicalVolume(solidSHPB_FETC_Q3Box, GetMaterial("Lead"),"SHPB"); 
  X = 0;
  Y = 0;
  Z = 0;
  G4VPhysicalVolume* physSHPB = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicSHPB,"SHPB",logicSHFE,false,0,checkOverlaps);
  G4VisAttributes* SHPBvis = new G4VisAttributes(1);
  SHPBvis->SetColor(1.0,0.0,0.0);
  SHPBvis->SetForceWireframe(false);
  logicSHPB->SetVisAttributes(SHPBvis);


//Air volume inside (Air)
  par[1] = 0.5*conv*slr_walls*cm;
  par[2] = 0.5*conv*sud_walls*cm;
  par[3] = 0.5*conv*sfr_walls*cm;
  G4Box* solidSHAI = new G4Box("SHAI", par[1], par[2], par[3]);
  X = 0;
  Y = 0.5*conv*(sud_walls + th_floor+th_ceiling + 2*th_steel+th_lead_b)*cm;
  Z = 0.5*conv*(sfr_walls + th_door + th_rear + 2*th_steel + th_lead_f - th_door - th_rear - th_lead_f - 1)*cm;
  //Remove Top Corner
  G4SubtractionSolid* solidSHAI_FETC = new G4SubtractionSolid("SHAI_FETC",solidSHAI, solidFETC,rot45X,G4ThreeVector(X,Y,Z));
  //Remove space for Q3. solidQ3Box defined earlier.
  X = 0 - offset;
  Y = -0.5*conv*(sud_walls + th_floor+th_ceiling + 2*th_steel+th_lead_b)*cm + Q3Box_y/2 -  0.5*conv*(th_floor - th_ceiling)*cm;
  Z = -0.5*conv*(sfr_walls + th_door + th_rear + 2*th_steel + th_lead_f)*cm + Q3Box_z/2 - 0.5*conv*(th_door - th_rear)*cm;

  G4SubtractionSolid* solidSHAI_FETC_Q3Box = new G4SubtractionSolid("SHAI-FETC-Q3Box",solidSHAI_FETC, solidQ3Box,0,G4ThreeVector(X,Y,Z));

  G4LogicalVolume* logicSHAI = new G4LogicalVolume(solidSHAI_FETC_Q3Box, GetMaterial("Air"),"SHAI"); 
  X = 0;
  Y = 0.5*conv*th_lead_b*cm;
  Z = 0.5*conv*th_lead_f*cm;
  G4VPhysicalVolume* physSHAI = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicSHAI,"SHAI",logicSHPB,false,0,checkOverlaps);
  G4VisAttributes* SHAIvis = new G4VisAttributes(1);
  SHAIvis->SetColor(1.0,1.0,0.0);
  SHAIvis->SetForceWireframe(false);
  logicSHAI->SetVisAttributes(SHAIvis);


}



G4Material* HRS::GetMaterial(G4String mat){

  return fDetCon->GetMaterial(mat);

}


HRS::~HRS(){;}

void HRS::MakeQ1Superconducting(G4LogicalVolume *logicWorld,
    G4double cut_angle)
{
  G4bool checkOverlaps = true;
  // dimensions are in inches
  const G4double conv = 2.54; //conversion from inches to cm
  G4double par[12];
  G4double X,Y,Z;

  ////////////////////////////////Q1//////////////////////////////////////////
  G4double Q1polelength = 81*cm;
  G4double Q1poleID = 30*cm;
  G4double Q1poleOD = 96*cm;
  par[1] = 0;
  par[2] = Q1poleOD/2;
  par[3] = Q1polelength/2;
  G4Tubs* solidQ1 = new G4Tubs("Q1",par[1],par[2],par[3],0,2*pi);

  //Remove part for beamline
  par[1] = 5*conv*cm;
  par[2] = 100*cm;
  par[3] = 100*cm;
  G4Box* solidQ1Cut1 = new G4Box("Q1Cut1",par[1],par[2],par[3]);
  // (jc2): Left in the constructor
  //if(thetaHRS > 0)
  //  {
  //    cut_angle = -12.5*deg;
  //    X = 207*cm*sin(cut_angle) - 6*cm;
  //  }
  //else
  //  {
  //    cut_angle = 12.5*deg;
  //    X = 207*cm*sin(cut_angle) + 6*cm;
  //  }
  if(cut_angle < 0) {
    X = 207*cm*sin(cut_angle) - 6*cm;
  } else {
    X = 207*cm*sin(cut_angle) + 6*cm;
  }
  Y = 0;
  Z = 0;
  //G4RotationMatrix* rotTubeY = new G4RotationMatrix();
  //rotTubeY->rotateY(cut_angle);
  G4SubtractionSolid* solidQ1_Q1Cut1 = new G4SubtractionSolid("Q1-Q1Cut1",solidQ1, solidQ1Cut1,rotTubeY,G4ThreeVector(X,Y,Z));
  par[1] = 0;
  par[2] = 5*conv*cm;
  par[3] = 100*cm;
  G4Tubs* solidQ1Cut2 = new G4Tubs("Q1Cut2",par[1],par[2],par[3],0,2*pi);
  G4SubtractionSolid* solidQ1_Q1Cut1_Q1Cut2 = new G4SubtractionSolid("Q1Cut1-Q1Cut2",solidQ1_Q1Cut1, solidQ1Cut2,rotTubeY,G4ThreeVector(X,Y,Z));

  G4LogicalVolume* logicQ1 = new G4LogicalVolume(solidQ1_Q1Cut1_Q1Cut2, GetMaterial("Iron"),"Q1");
  G4RotationMatrix *Q1angle = new G4RotationMatrix;
  Q1angle->rotateX(180*deg);
  Q1angle->rotateY(thetaHRS);
  X = 207*cm*sin(thetaHRS);
  Y = 0;
  Z = 207*cm*cos(thetaHRS);
  G4VPhysicalVolume* physQ1 = new G4PVPlacement(Q1angle,G4ThreeVector(X,Y,Z),logicQ1,"Q1",logicWorld,false,0,checkOverlaps);
  G4VisAttributes* Q1vis = new G4VisAttributes(1);
  Q1vis->SetForceWireframe(false);
  logicQ1->SetVisAttributes(Q1vis);

  par[1] = 0;
  par[2] = Q1poleID/2;
  par[3] = Q1polelength/2;
  G4Tubs* solidQ1_vac = new G4Tubs("Q1_vac",par[1],par[2],par[3],0,2*pi);
  G4LogicalVolume* logicQ1_vac = new G4LogicalVolume(solidQ1_vac, GetMaterial("Vacuum"),"Q1_vac");
  X = 0;
  Y = 0;
  Z = 0;
  G4VPhysicalVolume* physQ1_vac = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicQ1_vac,"Q1_vac",logicQ1,false,0,checkOverlaps);
  G4VisAttributes* Q1_vacvis = new G4VisAttributes(1);
  Q1_vacvis->SetColor(1.0,0.0,0.0);
  Q1_vacvis->SetForceWireframe(false);
  logicQ1_vac->SetVisAttributes(Q1_vacvis);

}

void HRS::MakeQ1Copper(G4LogicalVolume *logicWorld)
{
  /*
  G4bool checkOverlaps;

  G4double X,Y,Z;
  // Use GDML parser to read in in STEP file
  G4GDMLParser parser;
  parser.Read("/home/cornejo/apex/radStudy/simulation/q1.gdml");

  // Now place the read in structure
  G4RotationMatrix *Q1angle = new G4RotationMatrix;
  Q1angle->rotateX(180*deg);
  Q1angle->rotateY(thetaHRS);
  X = 207*cm*sin(thetaHRS);
  Y = 0;
  Z = 207*cm*cos(thetaHRS);
  G4VPhysicalVolume *physQ1 = new G4PVPlacement(Q1angle,G4ThreeVector(X,Y,Z),
      parser.GetWorldVolume()->GetLogicalVolume(),"Q1",logicWorld,
      false,0,checkOverlaps);
      */
}
