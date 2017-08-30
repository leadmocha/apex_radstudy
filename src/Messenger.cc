#include "Messenger.hh"
#include "RadConfig.hh"

#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWith3Vector.hh"

#ifdef __APPLE__
#include <unistd.h>
#endif

Messenger::Messenger()
{
  fieldCmd = new G4UIcmdWithAString("/radStudy/field", this);
  fieldCmd->SetGuidance("TOSCA field map file");
  fieldCmd->SetParameterName("field", false);

  rootfileCmd = new G4UIcmdWithAString("/radStudy/rootfile", this);
  rootfileCmd->SetGuidance("File name of the output ROOT file");
  rootfileCmd->SetParameterName("rootfile", false);

  beamEnergyCmd = new G4UIcmdWithADoubleAndUnit("/radStudy/beamEnergy", this);
  beamEnergyCmd->SetGuidance("Beam Energy");
  beamEnergyCmd->SetParameterName("beamEnergy", false);

  q1versionCmd = new G4UIcmdWithAString("/radStudy/q1version", this);
  q1versionCmd->SetGuidance("Options: APEX, SuperConducting");
  q1versionCmd->SetParameterName("q1version", false);

  targetCmd = new G4UIcmdWithAString("/radStudy/target", this);
  targetCmd->SetGuidance("Options: APEX, Carbon, PREX, APEXCarbon ");
  targetCmd->SetParameterName("target", false);

  targetThickRLCmd = new G4UIcmdWithADouble("/radStudy/targetThickRL", this);
  targetThickRLCmd->SetGuidance("Target thickness in terms of fractional Radiation Lengths");
  targetThickRLCmd->SetParameterName("targetThickRL", false);
}

Messenger::~Messenger()
{
  delete fieldCmd;
  delete beamEnergyCmd;
  delete rootfileCmd;
  delete q1versionCmd;
  delete targetCmd;
  delete targetThickRLCmd;
}


void Messenger::SetNewValue(G4UIcommand* cmd, G4String newValue)
{
  if( cmd == fieldCmd ) {
    gRadConfig->SeptaFieldFilename = newValue;
  } else if ( cmd == beamEnergyCmd ) {
    gRadConfig->BeamEnergy = beamEnergyCmd->GetNewDoubleValue(newValue);
  } else if (cmd == rootfileCmd) {
    gRadConfig->RootFilename = newValue;
    G4cout << "Setting rootfile to: " << gRadConfig->RootFilename << G4endl;
  } else if ( cmd == q1versionCmd) {
    G4String q1ver = newValue;
    q1ver.toLower();
    if(q1ver == "apex") {
      gRadConfig->Q1Version = RadQ1Version::APEX;
    } else if ( q1ver == "superconducting" ) {
      gRadConfig->Q1Version = RadQ1Version::SUPERCONDUCTING;
    } else {
      G4cerr << "Not a valid Q1 version specified: " << newValue << G4endl;
    }
  } else if (cmd == targetCmd ) {
    G4String target = newValue;
    target.toLower();
    if(target == "apex") {
      gRadConfig->TargetType = RadTargetType::APEX;
    } else if (target == "carbon") {
      gRadConfig->TargetType = RadTargetType::CARBON;
    } else if (target == "prex") {
      gRadConfig->TargetType = RadTargetType::PREX;
    } else if (target == "apexcarbon") {
      gRadConfig->TargetType = RadTargetType::APEXCARBON;
    } else {
      G4cerr << "Not a valid target type specified: " << newValue << G4endl;
    }
  } else if (cmd == targetThickRLCmd ) {
    gRadConfig->TargetThickRL = targetThickRLCmd->GetNewDoubleValue(newValue);
  } else {
    G4cerr << "Unknown command!!" << G4endl;
    exit(-1);
  }
}