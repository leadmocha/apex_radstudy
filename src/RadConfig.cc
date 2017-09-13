#include "RadConfig.hh"
#include <G4SystemOfUnits.hh>

RadConfig* RadConfig::fInstance = new RadConfig();

RadConfig::RadConfig()
{
  SeptaFieldFilename = "";
  RootFilename = "radStudy_output.root";
  BeamEnergy = 1.0*CLHEP::GeV;
  TargetType = RadTargetType::APEX; // APEX target as default
  //TargetType = RadTargetType::CARBON;
  Q1Version = RadQ1Version::APEX;
  //Q1Version = RadQ1Version::SUPERCONDUCTING;
  TargetThickRL = 0.053; // 5.3% target thickness, assuming APEX W
  BeamDumpVersion = RadBeamDumpVersion::UPDATEDVERSION;
  // By default do not kill the particles at the Dump (Water Barrel)
  // (But the user can turn it off to make it faster. Previous iterations
  // of this code just killed all particles that passed through the water
  // barrel, but it's possible some stuff may make it back into the hall,
  // so I leave it on by default).
  StopParticluesAtDump = false;
  PrimaryVertex = G4ThreeVector(0.,0.,-10*m);
  PrimaryDirection = G4ThreeVector(0.,0.,1.0);

  // Generate BuildDetector
  AddDetectorBuild("HallA",true,true);
  AddDetectorBuild("HallAFloor",true,true);
  AddDetectorBuild("HRS",true,true);
  AddDetectorBuild("Septum",true,true);
  AddDetectorBuild("Target",true,true);
  AddDetectorBuild("BeamDump",true,true);
  AddDetectorBuild("BeamlineDS",true,true);
}

RadConfig::~RadConfig()
{
}

RadConfig* RadConfig::GetInstance()
{
  return fInstance;
}

RadConfigDetector* RadConfig::GetDetector(G4String name)
{
  name.toLower();
  std::map<G4String,RadConfigDetector>::iterator it =
    ConfigDetectorMap.find(name);
  if( it == ConfigDetectorMap.end() )
    return 0;

  return &(it->second);
}

G4bool RadConfig::SetDetectorBuild(G4String name, G4bool build)
{
  RadConfigDetector* det = GetDetector(name);
  if(!det)
    return false;

  det->build = build;
  return true;
}

G4bool RadConfig::BuildDetector(G4String name)
{
  RadConfigDetector* det = GetDetector(name);
  if(!det)
    return false;
  return det->build;
}


