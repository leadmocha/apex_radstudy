#ifndef RadConfig_HH
#define RadConfig_HH

#include "globals.hh"

// Note, this only works in C++11 (and higher)
enum class RadTargetType { APEX, CARBON, PREX, APEXCARBON };
enum class RadQ1Version{
  SUPERCONDUCTING, // 6 GeV era
  COPPER, // Water cooled "normal" Cu magnets
  APEX // Copper Q1, but basically both defined in a gdml file
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

  // Define a singleton we can use to configure the whole simulation
public:
  static RadConfig* GetInstance();
  static RadConfig *fInstance;
};

// Get static configuration variable
#define gRadConfig RadConfig::GetInstance()

#endif//RadConfig_HH
