// header file for SensitiveDetector

#ifndef HRSDetSDR_h
#define HRSDetSDR_h 1

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;
class HallA;
class B1IO;

class HRSDetSDR:public G4VSensitiveDetector
{
public:

  HRSDetSDR(G4String SDname);

  ~HRSDetSDR();

public:

  G4bool ProcessHits(G4Step* step,G4TouchableHistory* hist);

  void EndOfEvent(G4HCofThisEvent* HCE);



};

#endif
