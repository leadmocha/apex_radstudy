#ifndef RadConfig_HH
#define RadConfig_HH

#include "globals.hh"
#include <map>

// Note, this only works in C++11 (and higher)
enum class RadTargetType { APEX, CARBON, PREX, APEXCARBON };
enum class RadQ1Version{
  SUPERCONDUCTING, // 6 GeV era
  COPPER, // Water cooled "normal" Cu magnets
  APEX // Copper Q1, but basically both defined in a gdml file
};
enum class RadBeamDumpVersion {
  ORIGCODED, // Originally coded version in this code. Though, I do not know
             // who coded it and where the numbers came from
  UPDATEDVERSION // (jc2) Updated Version based on notes from George Kharashvili
};

struct RadConfigDetector {
  G4bool build;
  G4bool enabled;
};

class RadConfig {
public:
  RadConfig();
  virtual ~RadConfig();


  // Define the variables containing the configuration (see RadConfig.cc for
  // the default values used)
public:
  G4String        SeptaFieldFilename;
  G4double        BeamEnergy;
  G4String        RootFilename;
  RadTargetType   TargetType;
  RadQ1Version    Q1Version;
  G4double        TargetThickRL;
  RadBeamDumpVersion BeamDumpVersion;
  G4bool          StopParticluesAtDump;

  std::map<G4String,RadConfigDetector> ConfigDetectorMap;
  void AddDetectorBuild(G4String name, G4bool build = true,
      G4bool enable = true) {
    name.toLower();
    RadConfigDetector config;
    config.build = build;
    config.enabled = enable;
    ConfigDetectorMap[name] = config;
  }

  // Check if this detector should be built
  G4bool BuildDetector(G4String name);
  // Set status of a previously defined detector
  G4bool SetDetectorBuild(G4String name, G4bool build);

  // Define a singleton we can use to configure the whole simulation
public:
  static RadConfig* GetInstance();
  static RadConfig *fInstance;
  RadConfigDetector* GetDetector(G4String name);
};

// Get static configuration variable
#define gRadConfig RadConfig::GetInstance()

#endif//RadConfig_HH
