// header file for SensitiveDetector

#ifndef HRSDetSDL_h
#define HRSDetSDL_h 1

#include "G4VSensitiveDetector.hh"
#include "RE01Trajectory.hh"
#include "sd_data.hh"

class G4Step;
class G4HCofThisEvent;
class HallA;
class B1IO;

class HRSDetSDL:public G4VSensitiveDetector
{
public:

  HRSDetSDL(G4String SDname);
  
  RE01Trajectory* GetParentTrajectory(G4int parentID);

  ~HRSDetSDL();

public:

  G4bool ProcessHits(G4Step* step,G4TouchableHistory* hist);

  void EndOfEvent(G4HCofThisEvent* HCE);


private:
  int fIOID;
  sd_data fData;
};

#endif
