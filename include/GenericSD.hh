// Generic sensitive detector header file

#ifndef GENERICSD_HH
#define GENERICSD_HH

#include "G4VSensitiveDetector.hh"
#include "RE01Trajectory.hh"
#include "sd_data.hh"

class G4Step;
class G4HCofThisEvent;
class HallA;

class GenericSD : public G4VSensitiveDetector {
public:
  GenericSD(G4String SDname, int data_type = 0);
  
  RE01Trajectory* GetParentTrajectory(G4int parentID);

  virtual ~GenericSD();

  G4bool ProcessHits(G4Step* step,G4TouchableHistory* hist);
  void EndOfEvent(G4HCofThisEvent* HCE);


private:
  int fIOID;
  int fDataType;
  sd_data fData;
};

#endif // GENERICSD_HH
