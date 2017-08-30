#ifndef Messenger_HH
#define Messenger_HH

#include "G4UImessenger.hh"
#include "G4UIcommand.hh"

class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;
class CmdWithAStringAndBool;
class G4UIcmdWithABool;
class G4UIcmdWith3Vector;

class Messenger : public G4UImessenger {
public:
  Messenger();
  virtual ~Messenger();

  void SetNewValue(G4UIcommand* cmd, G4String newValue);

private:
  G4UIcmdWithAString *fieldCmd;
  G4UIcmdWithAString *rootfileCmd;
  G4UIcmdWithADoubleAndUnit *beamEnergyCmd;
  G4UIcmdWithAString *q1versionCmd;
  G4UIcmdWithAString *targetCmd;
  G4UIcmdWithADouble *targetThickRLCmd;
  CmdWithAStringAndBool *buildDetectorCmd;
};

#endif//Messenger_HH
