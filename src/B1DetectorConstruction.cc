//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: B1DetectorConstruction.cc 75117 2013-10-28 09:38:37Z gcosmo $
//
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4Paraboloid.hh"
#include "G4Cons.hh"
#include "G4Polycone.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Element.hh"

#include "G4VisAttributes.hh"

#include "G4PhysicalConstants.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"

#include "G4MagneticField.hh"
#include "MagneticField.hh"
#include "SeptaField.hh"
#include "G4FieldManager.hh"
#include "G4ChordFinder.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4ExplicitEuler.hh"
#include "G4HelixImplicitEuler.hh"

#include "HallA.hh"
#include <map>

using namespace std;

const G4double conv = 2.54;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{

  ConstructMaterials(); //Construct materials

 }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{

  G4NistManager *nist = G4NistManager::Instance();

  if( fMaterialsMap.empty() ) ConstructMaterials();
  //G4cout << *(G4Material::GetMaterialTable())<<G4endl;
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double Rspace = 2000*m;
  G4double Yspace = 50000*m;
  G4double Tgrnd = 100*m;

  G4Box* solidWorld = new G4Box("World",Rspace,Yspace/2,Rspace);

  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld, GetMaterial("Air"),"World");                                           
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                     


  //Hall A

  HallA* hallA = new HallA(this,logicWorld);


  //always return the physical World
  return physWorld;
}

void B1DetectorConstruction::ConstructMaterials(){

  G4NistManager *nist = G4NistManager::Instance();

  G4double fractionmass;
  G4double density;
  G4String name, symbol;
  G4int nel, natoms, ncomponents;
  G4double a, iz, z;

  map<G4String,G4Material*>::iterator itest;

  G4Material* Vacuum = new G4Material(name="Vacuum", z=1., a=1.0*g/mole, density=1e-9*g/cm3);
  //fMaterialsMap["Vacuum"] = Vacuum;
  fMaterialsMap["Vacuum"] = nist->FindOrBuildMaterial("G4_Galactic");

  G4Material* Carbon = nist->FindOrBuildMaterial("G4_C");
  fMaterialsMap["Carbon"] = Carbon;

  G4Material* Silicon = nist->FindOrBuildMaterial("G4_Si");
  fMaterialsMap["Silicon"] = Silicon;

  G4Material* Tungsten = nist->FindOrBuildMaterial("G4_W");
  fMaterialsMap["Tungsten"] = Tungsten;

  G4Material* Iron = nist->FindOrBuildMaterial("G4_Fe");
  fMaterialsMap["Iron"] = Iron;

  G4Material* Aluminum = nist->FindOrBuildMaterial("G4_Al");
  fMaterialsMap["Aluminum"] = Aluminum;

  G4Material* Lead = nist->FindOrBuildMaterial("G4_Pb");
  fMaterialsMap["Lead"] = Lead;

  density = 0.3761*g/cm3;
  G4Material* BLT4mat = nist->BuildMaterialWithNewDensity("BLT4mat","G4_Al",density);
  fMaterialsMap["BLT4mat"] = BLT4mat; //Material for Beam Line Tube 4

  density = 0.3718*g/cm3;
  G4Material* BLT5mat = nist->BuildMaterialWithNewDensity("BLT5mat","G4_Al",density);
  fMaterialsMap["BLT5mat"] = BLT5mat; //Material for Beam Line Tube 5

  density = 0.2124*g/cm3;
  G4Material* BLT6mat = nist->BuildMaterialWithNewDensity("BLT6mat","G4_Al",density);
  fMaterialsMap["BLT6mat"] = BLT6mat; //Material for Beam Line Tube 6

  density = 0.2124*g/cm3;
  G4Material* BLT7mat = nist->BuildMaterialWithNewDensity("BLT7mat","G4_Al",density);
  fMaterialsMap["BLT7mat"] = BLT7mat; //Material for Beam Line Tube 7

  density = 0.2124*g/cm3;
  G4Material* BLT8mat = nist->BuildMaterialWithNewDensity("BLT8mat","G4_Al",density);
  fMaterialsMap["BLT8mat"] = BLT8mat; //Material for Beam Line Tube 8

  G4Material* Copper = nist->FindOrBuildMaterial("G4_Cu");
  fMaterialsMap["Copper"] = Copper;


  //Ground
  //WinGrnd = Weight(H20)/Weight(SiO2) = 0.175
  G4double WinGrnd = 0.175;
  G4double aH=1.00794,aO=16.0,aSi=28.0855;
  G4double WGR[3],wsumGR;
  WGR[0] = 2*aH*WinGrnd*(aSi+2*aO)/(2*aH+aO);
  WGR[1] = aO*(2+WGR[0]/(2*aH));
  WGR[2] = aSi;

  wsumGR = WGR[0]+WGR[1]+WGR[2];
  WGR[0] = (WGR[0]/wsumGR);
  WGR[1] = (WGR[1]/wsumGR);
  WGR[2] = (WGR[2]/wsumGR);

  a = 1.00794*g/mole;
  G4Element* elH = new G4Element(name="Hydrogen",symbol="H",z=1,a);

  a = 16.0*g/mole;
  G4Element* elO = new G4Element(name="Oxygen",symbol="O",z=8.,a);

  a = 28.0855*g/mole;
  G4Element* elSi = new G4Element(name="Silicone",symbol="Si",z=14,a);

  density = 2.0023*g/cm3;
  G4Material* Ground = new G4Material(name="Ground",density,ncomponents=3);
  Ground->AddElement(elH,fractionmass = WGR[0]);
  Ground->AddElement(elO,fractionmass = WGR[1]);
  Ground->AddElement(elSi,fractionmass = WGR[2]);

  fMaterialsMap["Ground"] = Ground;

  //Concrete
  a = 27*g/mole;
  G4Element* elAl = new G4Element(name="Aluminum",symbol="Al",z=13,a);
  a = 40.078*g/mole;
  G4Element* elCa = new G4Element(name="Calcium",symbol="Ca",z=20,a);
  a = 55.847*g/mole;
  G4Element* elFe = new G4Element(name="Iron",symbol="Fe",z=26,a);
  density = 2.3227*g/cm3;
  G4Material* Concrete = new G4Material(name="Concrete",density,ncomponents=6);
  Concrete->AddElement(elH,fractionmass = 0.006);
  Concrete->AddElement(elO,fractionmass = 0.509);
  Concrete->AddElement(elAl,fractionmass = 0.034);
  Concrete->AddElement(elSi,fractionmass = 0.345);
  Concrete->AddElement(elCa,fractionmass = 0.070);
  Concrete->AddElement(elFe,fractionmass = 0.036);

  fMaterialsMap["Concrete"] = Concrete;

  G4Material* Air = nist->FindOrBuildMaterial("G4_AIR");
  fMaterialsMap["Air"] = Air;

  G4Material* Water = nist->FindOrBuildMaterial("G4_WATER");
  fMaterialsMap["Water"] = Water;
  
  //Plastic Scintillator

  a = 12.0107*g/mole;
  G4Element* elC = new G4Element(name="Carbon",symbol="C",z=6,a);
  density = 1.032*g/cm3;
  G4Material* Scintillator = new G4Material(name="Scintillator",density,ncomponents=2);
  Scintillator->AddElement(elH,fractionmass = 0.085);
  Scintillator->AddElement(elC,fractionmass = 0.915);
  
  fMaterialsMap["Scintillator"] = Scintillator;


}

G4Material* B1DetectorConstruction::GetMaterial(G4String name){

  if(fMaterialsMap.empty()) ConstructMaterials();

  map<G4String,G4Material*>::iterator it = fMaterialsMap.find(name);

  if(it!=fMaterialsMap.end()){
    return fMaterialsMap[name];
  }else{
    fprintf(stderr, "ERROR %s:%d - Material %s not found\n", __FILE__, __LINE__, name.data());
    exit(1);
    return NULL;
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
