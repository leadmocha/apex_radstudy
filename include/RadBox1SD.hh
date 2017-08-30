// header file for SensitiveDetector

#ifndef RadBox1SD_h
#define RadBox1SD_h 1

#include "G4VSensitiveDetector.hh"
#include "RE01Trajectory.hh"

class G4Step;
class G4HCofThisEvent;
class HallA;
class B1IO;

class RadBox1SD:public G4VSensitiveDetector
{
public:

  RadBox1SD(G4String SDname);
  
  RE01Trajectory* GetParentTrajectory(G4int parentID);

  ~RadBox1SD();

public:

  G4bool ProcessHits(G4Step* step,G4TouchableHistory* hist);

  void EndOfEvent(G4HCofThisEvent* HCE);



};

#endif
