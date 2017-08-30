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
}

RadConfig::~RadConfig()
{
}

RadConfig* RadConfig::GetInstance()
{
  return fInstance;
}
