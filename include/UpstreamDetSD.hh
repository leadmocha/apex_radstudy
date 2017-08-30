// header file for SensitiveDetector

#ifndef UpstreamDetSD_h
#define UpstreamDetSD_h 1

#include "G4VSensitiveDetector.hh"
#include "RE01Trajectory.hh"

class G4Step;
class G4HCofThisEvent;
class HallA;
class B1IO;

class UpstreamDetSD:public G4VSensitiveDetector
{
public:

  UpstreamDetSD(G4String SDname);
  
  RE01Trajectory* GetParentTrajectory(G4int parentID);

  ~UpstreamDetSD();

public:

  G4bool ProcessHits(G4Step* step,G4TouchableHistory* hist);

  void EndOfEvent(G4HCofThisEvent* HCE);

  int fIOID;

};

#endif
