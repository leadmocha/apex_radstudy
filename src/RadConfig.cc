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

  // Generate BuildDetector
  AddDetectorBuild("HallA",true,true);
  AddDetectorBuild("HRS",true,true);
  AddDetectorBuild("Septum",true,true);
  AddDetectorBuild("Target",true,true);
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


