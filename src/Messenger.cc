#include "Messenger.hh"
#include "RadConfig.hh"

#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"
#include <CmdWithAStringAndBool.hh>

#ifdef __APPLE__
#include <unistd.h>
#endif

Messenger::Messenger()
{
  fieldCmd = new G4UIcmdWithAString("/radStudy/field", this);
  fieldCmd->SetGuidance("TOSCA field map file");
  fieldCmd->SetParameterName("field", true);
  fieldCmd->SetDefaultValue("");

  setPrimaryVertexCmd = new G4UIcmdWith3VectorAndUnit(
      "/radStudy/setPrimaryVertex",this);
  setPrimaryVertexCmd->SetGuidance("Set Primary (electron) Vertex");
  setPrimaryVertexCmd->SetParameterName("X","Y","Z",false);

  setPrimaryDirectionCmd = new G4UIcmdWith3Vector(
      "/radStudy/setPrimaryDirection",this);
  setPrimaryDirectionCmd->SetGuidance("Set Primary (electron) Direction");
  setPrimaryDirectionCmd->SetParameterName("X","Y","Z",false);

  buildDetectorCmd = new CmdWithAStringAndBool("/radStudy/buildDetector", this);
  buildDetectorCmd->SetGuidance("Build Detector detector");
  buildDetectorCmd->SetParameterName("enableDetector", false);

  stopDumpParticlesCmd = new G4UIcmdWithABool("/radStudy/stopDumpParticles", this);
  stopDumpParticlesCmd->SetGuidance("Stop particles at dump window? (false will stop them at the end, not at the start)");
  //stopDumpParticlesCmd->SetParameterName("stopDumpParticles", false);

  rootfileCmd = new G4UIcmdWithAString("/radStudy/rootfile", this);
  rootfileCmd->SetGuidance("File name of the output ROOT file");
  rootfileCmd->SetParameterName("rootfile", false);

  beamEnergyCmd = new G4UIcmdWithADoubleAndUnit("/radStudy/beamEnergy", this);
  beamEnergyCmd->SetGuidance("Beam Energy");
  beamEnergyCmd->SetParameterName("beamEnergy", false);

  q1versionCmd = new G4UIcmdWithAString("/radStudy/q1version", this);
  q1versionCmd->SetGuidance("Options: APEX, SuperConducting");
  q1versionCmd->SetParameterName("q1version", false);

  beamDumpVersionCmd = new G4UIcmdWithAString("/radStudy/beamDumpVersion", this);
  beamDumpVersionCmd->SetGuidance("Options: ORIGCODED, Updated");
  beamDumpVersionCmd->SetParameterName("beamDumpVersion", false);

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
  delete buildDetectorCmd;
}


void Messenger::SetNewValue(G4UIcommand* cmd, G4String newValue)
{
  G4bool success = true;
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
  } else if ( cmd == beamDumpVersionCmd) {
    G4String beamDumpVer = newValue;
    beamDumpVer.toLower();
    if(beamDumpVer == "origcoded") {
      gRadConfig->BeamDumpVersion = RadBeamDumpVersion::ORIGCODED;
    } else if ( beamDumpVer == "updated" ) {
      gRadConfig->BeamDumpVersion = RadBeamDumpVersion::UPDATEDVERSION;
    } else {
      success = false;
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
  } else if (cmd == buildDetectorCmd ) {
    //G4String detName = buildDetectorCmd->GetParamString(newValue,0);
    //G4bool build = buildDetectorCmd->GetNewbuildDetectorCmd->GetParamString(newValue,0);
    std::pair<G4String,G4bool> result = buildDetectorCmd->GetResult(
        newValue);
    success = gRadConfig->SetDetectorBuild(result.first,result.second);
  } else if (cmd == stopDumpParticlesCmd) {
    gRadConfig->StopParticluesAtDump =
      stopDumpParticlesCmd->GetNewBoolValue(newValue);
  } else if (cmd == setPrimaryVertexCmd) {
    gRadConfig->PrimaryVertex = setPrimaryVertexCmd->GetNew3VectorValue(
        newValue);
  } else if (cmd == setPrimaryDirectionCmd) {
    gRadConfig->PrimaryDirection = setPrimaryDirectionCmd->GetNew3VectorValue(
        newValue);
  } else {
    G4cerr << "Unknown command!!" << G4endl;
    exit(-1);
  }

  if(!success) {
    G4cerr << "Command failed, exiting!!!" << G4endl;
    exit(-1);
  }
}
