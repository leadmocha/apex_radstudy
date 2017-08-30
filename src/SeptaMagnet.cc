#include "SeptaMagnet.hh"
#include "G4NistManager.hh"
#include "B1DetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Trd.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4VisAttributes.hh"

#include "G4MagneticField.hh"
#include "MagneticField.hh"
#include "SeptaField.hh"
#include "G4FieldManager.hh"
#include "G4ChordFinder.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4ExplicitEuler.hh"
#include "G4HelixImplicitEuler.hh"

#include "HallA.hh"

const G4double conv = 2.54;

SeptaMagnet::SeptaMagnet(B1DetectorConstruction *dc, HallA *ha, G4LogicalVolume *logicWorld, G4bool fieldON):fDetCon(dc),fieldON(fieldON){

  G4double Tair1 = ha->getTair1();
  G4double bmheight = ha->getbmheight();
  G4double X, Y, Z;

  G4bool checkOverlaps = true;

  //Rotation Matrices
  G4RotationMatrix* rot90X = new G4RotationMatrix();
  rot90X->rotateX(90*deg);
  G4RotationMatrix* rot270X = new G4RotationMatrix();
  rot270X->rotateX(270*deg);

  //Mother Volume for Septa Magnet

  G4Box* ContainerBox = new G4Box("Box",0.5*62.99*conv*cm,0.5*60*conv*cm,0.5*45*conv*cm);
  G4LogicalVolume* logicBox = new G4LogicalVolume(ContainerBox,GetMaterial("Vacuum"),"Box");

  G4RotationMatrix *Box_rm = new G4RotationMatrix();
  Box_rm->rotateY(0*deg);
  X = 0;
  Y = 0;
  Z = 70*cm;
  G4ThreeVector Septa_pos = G4ThreeVector(X,Y,Z);

  if(fieldON)
    {
      SeptaField *ffield = new SeptaField(Septa_pos,*Box_rm);
      G4Mag_UsualEqRhs* fequation = new G4Mag_UsualEqRhs(ffield);
      G4MagIntegratorStepper *stepper = new G4HelixImplicitEuler(fequation);
      G4FieldManager *fm = new G4FieldManager(ffield);
      new G4ChordFinder(ffield,1.0*nm,stepper);
      logicWorld->SetFieldManager(fm,true);

      double Bfield[3]={0,0,0};
      const double Point[3]={70*cm,-9*cm,254*cm};
      ffield->GetFieldValue(Point,Bfield);
    }

  G4VisAttributes* visibility = new G4VisAttributes(0);
  visibility->SetForceWireframe(true);
  visibility->SetDaughtersInvisible(false);
  logicBox->SetVisAttributes(visibility);


  //Upper & Lower Yoke

  G4Box* yoke_solid = new G4Box("yoke",0.5*33.07*conv*cm,0.5*12.15*conv*cm,0.5*62.99*conv*cm);
  G4LogicalVolume* yoke_logic = new G4LogicalVolume(yoke_solid,GetMaterial("Iron"),"yoke");
  G4VisAttributes* yoke_color = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  yoke_logic -> SetVisAttributes(yoke_color);

  // Magnet Core 

  G4Box* magnetcore_solid1 = new G4Box("magnetcore1",0.5*33.07*conv*cm,0.5*16.73*conv*cm,0.5*12.80*conv*cm);
  G4Box* magnetcore_solid2 = new G4Box("magnetcore2",0.5*70*conv*cm,0.5*16.74*conv*cm,0.5*12.80*conv*cm);
  G4RotationMatrix* magnetcore_Rot = new G4RotationMatrix();
  magnetcore_Rot->rotateY(5*deg);

  G4SubtractionSolid* magnetcore_solid = new G4SubtractionSolid("core1-core2",magnetcore_solid1,magnetcore_solid2,magnetcore_Rot,G4ThreeVector(-8.265*conv*cm,0,-12.80*conv*cm));

  G4LogicalVolume* magnetcore_logic = new G4LogicalVolume(magnetcore_solid,GetMaterial("Iron"),"magnetcore");
  G4VisAttributes* magnetcore_color = new G4VisAttributes(G4Colour(0.0,1.0,0.0));
  magnetcore_logic -> SetVisAttributes(magnetcore_color);

  //Inner Pole
 
  G4Tubs* innerpole_part1 = new G4Tubs("InnerPole1",247.80*conv*cm,257.64*conv*cm,0.5*5.217*conv*cm,0,0.134);
  G4Box* innerpole_part2 = new G4Box("Innerpole2",0.5*24*conv*cm,0.5*1.4*conv*cm,0.5*5.22*conv*cm);
  G4SubtractionSolid* innerpole_solid = new G4SubtractionSolid("part1-part2",innerpole_part1,innerpole_part2,0,G4ThreeVector(251*conv*cm,33.771*conv*cm,0));
  G4LogicalVolume* innerpole_logic = new G4LogicalVolume(innerpole_solid,GetMaterial("Iron"),"innerpole");
  G4VisAttributes* innerpole_color = new G4VisAttributes(G4Colour(0.0,1.0,0.0));
  innerpole_logic-> SetVisAttributes(innerpole_color);

  //Coils

  G4Box* Box1 = new G4Box("Box1",0.5*42.31*conv*cm,0.5*23.21*conv*cm,0.5*4.76*conv*cm);
  G4Box* Box2 = new G4Box("Box2",0.5*34.2*conv*cm,0.5*15.09*conv*cm,0.5*4.8*conv*cm);
  G4SubtractionSolid* solidLCoil = new G4SubtractionSolid("Box1-Box2",Box1,Box2);
 
  G4Box* Box3 = new G4Box("Box3",0.5*39.46*conv*cm,0.5*24.04*conv*cm,0.5*1.36*conv*cm);
  G4Box* Box4 = new G4Box("Box4",0.5*33.72*conv*cm,0.5*18.3*conv*cm,0.5*1.4*conv*cm);
  G4SubtractionSolid* solidSCoil = new G4SubtractionSolid("Box3-Box4",Box3,Box4);

  G4LogicalVolume* logicLCoil = new G4LogicalVolume(solidLCoil, GetMaterial("Copper"), "LCoil");

  G4LogicalVolume* logicSCoil = new G4LogicalVolume(solidSCoil, GetMaterial("Copper"), "SCoil");

  G4VisAttributes* LCoil = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  G4VisAttributes* SCoil = new G4VisAttributes(G4Colour(1.0,0.0,0.0));

  logicLCoil -> SetVisAttributes(LCoil);
  logicSCoil -> SetVisAttributes(SCoil);



  //Placements

  //Container
  G4VPhysicalVolume* physBox = new G4PVPlacement(0,Septa_pos,logicBox,"Box",logicWorld, false,0,checkOverlaps);
  
  //Upper Yoke
  G4RotationMatrix* yoke_Rot = new G4RotationMatrix();
  yoke_Rot->rotateY(90*deg);
  G4VPhysicalVolume* yoke_upper_phys = new G4PVPlacement(yoke_Rot,G4ThreeVector(0,14.44*conv*cm,0),yoke_logic,"yoke",logicBox, false,0,checkOverlaps);

  //Lower Yoke
  G4VPhysicalVolume* yoke_lower_phys = new G4PVPlacement(yoke_Rot,G4ThreeVector(0,-14.44*conv*cm,0),yoke_logic,"yoke",logicBox, false,0,checkOverlaps);

  //Left Magnetcore
  G4RotationMatrix* magnetcore_left_Rot = new G4RotationMatrix();
  magnetcore_left_Rot->rotateY(90*deg);
  magnetcore_left_Rot->rotateX(180*deg);
  G4VPhysicalVolume* magnetcore_left_phys = new G4PVPlacement(magnetcore_left_Rot,G4ThreeVector(25.095*conv*cm,0,0),magnetcore_logic,"Core",logicBox, false,0,checkOverlaps);


  //Right Magnetcore
  G4RotationMatrix* magnetcore_right_Rot = new G4RotationMatrix();
  magnetcore_right_Rot->rotateY(90*deg);
  G4VPhysicalVolume* magnetcore_right_phys = new G4PVPlacement(magnetcore_right_Rot,G4ThreeVector(-25.095*conv*cm,0,0),magnetcore_logic,"Core",logicBox, false,0,checkOverlaps);

  //inner pole1
  G4RotationMatrix* innerpole_Rot1 = new G4RotationMatrix();
  innerpole_Rot1->rotateX(-90*deg);
  G4RotationMatrix* innerpole_Rot2 = new G4RotationMatrix();
  innerpole_Rot2->rotateY(180*deg);
  innerpole_Rot2->rotateX(90*deg);
  G4VPhysicalVolume* innerpole_phys1 = new G4PVPlacement(innerpole_Rot1,G4ThreeVector(-260.50*conv*cm,-5.7565*conv*cm,-16.535*conv*cm),innerpole_logic,"innerpole1",logicBox,false,0,checkOverlaps);
  G4VPhysicalVolume* innerpole_phys2 = new G4PVPlacement(innerpole_Rot1,G4ThreeVector(-260.50*conv*cm,5.7565*conv*cm,-16.535*conv*cm),innerpole_logic,"innerpole2",logicBox,false,0,checkOverlaps);
  G4VPhysicalVolume* innerpole_phys3 = new G4PVPlacement(innerpole_Rot2,G4ThreeVector(260.50*conv*cm,-5.7565*conv*cm,-16.535*conv*cm),innerpole_logic,"innerpole3",logicBox,false,0,checkOverlaps);
  G4VPhysicalVolume* innerpole_phys4 = new G4PVPlacement(innerpole_Rot2,G4ThreeVector(260.50*conv*cm,5.7565*conv*cm,-16.535*conv*cm),innerpole_logic,"innerpole4",logicBox,false,0,checkOverlaps);

  //Large Coil 1
  G4RotationMatrix* LCoilRot1 = new G4RotationMatrix();
  LCoilRot1->rotateY((5+90)*deg);
  new G4PVPlacement(LCoilRot1,G4ThreeVector(-16.57*conv*cm,13.48*conv*cm,0),logicLCoil,"LCoil",logicBox,false,0,checkOverlaps);
  new G4PVPlacement(LCoilRot1,G4ThreeVector(-16.57*conv*cm,-13.48*conv*cm,0),logicLCoil,"LCoil",logicBox,false,0,checkOverlaps);

  //Large Coil 2
  G4RotationMatrix* LCoilRot2 = new G4RotationMatrix();
  LCoilRot2->rotateY((-5+90)*deg);
  new G4PVPlacement(LCoilRot2,G4ThreeVector(16.57*conv*cm,13.48*conv*cm,0),logicLCoil,"LCoil",logicBox,false,0,checkOverlaps);
  new G4PVPlacement(LCoilRot2,G4ThreeVector(16.57*conv*cm,-13.48*conv*cm,0),logicLCoil,"LCoil",logicBox,false,0,checkOverlaps);

  //Small Coil 1
  G4RotationMatrix* SCoilRot1 = new G4RotationMatrix();
  SCoilRot1->rotateY((2+90)*deg);
  new G4PVPlacement(SCoilRot1,G4ThreeVector(-3*conv*cm,12.02*conv*cm,0),logicSCoil,"SCoil",logicBox,false,2,checkOverlaps);
  new G4PVPlacement(SCoilRot1,G4ThreeVector(-3*conv*cm,-12.02*conv*cm,0),logicSCoil,"SCoil",logicBox,false,2,checkOverlaps);

  //Small Coil 2
  G4RotationMatrix* SCoilRot2 = new G4RotationMatrix();
  SCoilRot2->rotateY((-2+90)*deg);
  new G4PVPlacement(SCoilRot2,G4ThreeVector(3*conv*cm,12.02*conv*cm,0),logicSCoil,"SCoil",logicBox,false,2,checkOverlaps);
  new G4PVPlacement(SCoilRot2,G4ThreeVector(3*conv*cm,-12.02*conv*cm,0),logicSCoil,"SCoil",logicBox,false,2,checkOverlaps);


  //////////////////////////////////////////////////////////////////////
  /////////////////Pipe through Septa Magnet////////////////////////////
  //////////////////////////////////////////////////////////////////////

  //Tube Shielding
  
  G4Trd* tube_shield_part1 = new G4Trd("tube_shield",1.334*conv*cm,2.763*conv*cm,2.01*conv*cm,2.01*conv*cm,0.5*41.41*conv*cm);
  G4LogicalVolume* tube_shield_logic = new G4LogicalVolume(tube_shield_part1,GetMaterial("Iron"),"tube_shield");
  G4VPhysicalVolume* tube_shield_phys = new G4PVPlacement(0,G4ThreeVector(),tube_shield_logic,"tube_shield",logicBox, false,0,checkOverlaps);
  G4VisAttributes* tube_shield_vis = new G4VisAttributes(1);
  tube_shield_vis->SetColor(0.0,1.0,1.0);
  tube_shield_logic-> SetVisAttributes(tube_shield_vis);

  //Inner Beam Tube Wall
  G4Cons* beam_tube_wall_solid = new G4Cons("beam_tube_wall",0,1.299*conv*cm,0,1.9685*conv*cm,0.5*41.41*conv*cm,0,2*pi);
  G4LogicalVolume* beam_tube_wall_logic = new G4LogicalVolume(beam_tube_wall_solid,GetMaterial("Iron"),"beam_tube_wall");
  G4VPhysicalVolume* beam_tube_wall_phys = new G4PVPlacement(0,G4ThreeVector(),beam_tube_wall_logic,"beam_tube_wall",tube_shield_logic,false,0,checkOverlaps);
  G4VisAttributes* beam_tube_wall_vis = new G4VisAttributes(1);
  beam_tube_wall_logic-> SetVisAttributes(beam_tube_wall_vis);

  //Inner Beam Tube Vacuum
  
  G4Cons* beam_tube_solid = new G4Cons("beam_tube",0,0.9845*conv*cm,0,1.638*conv*cm,0.5*41.41*conv*cm,0,2*pi);
  G4LogicalVolume* beam_tube_logic = new G4LogicalVolume(beam_tube_solid,GetMaterial("Vacuum"),"beam_tube");
  G4VPhysicalVolume* beam_tube_phys = new G4PVPlacement(0,G4ThreeVector(),beam_tube_logic,"beam_tube",beam_tube_wall_logic,false,0,checkOverlaps);
  G4VisAttributes* beam_tube_vis = new G4VisAttributes(0);
  beam_tube_vis->SetColor(0.0,0.0,1.0);
  beam_tube_logic-> SetVisAttributes(beam_tube_vis);

  //Fill gaps

  G4Tubs* solidFill = new G4Tubs("Fill",0 ,0.9845*conv*cm, 0.5*1.795*conv*cm,0,2*pi);
  G4LogicalVolume* logicFill = new G4LogicalVolume(solidFill,GetMaterial("Vacuum"),"Fill");
  X = 0;
  Y = 0; 
  Z = -0.5*45*conv*cm + 0.5*1.795*conv*cm;
  G4VPhysicalVolume* physFill_1 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicFill,"Fill_1",logicBox, false,0,checkOverlaps);
  X = 0;
  Y = 0; 
  Z = 0.5*45*conv*cm - 0.5*1.795*conv*cm;
  G4VPhysicalVolume* physFill_2 = new G4PVPlacement(0,G4ThreeVector(X,Y,Z),logicFill,"Fill_2",logicBox, false,0,checkOverlaps);


  //////////////////////////////////////////////////////////////////////


}



G4Material* SeptaMagnet::GetMaterial(G4String mat){

  return fDetCon->GetMaterial(mat);

}


SeptaMagnet::~SeptaMagnet(){;}
