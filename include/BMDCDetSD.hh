// header file for SensitiveDetector

#ifndef BMDCDetSD_h
#define BMDCDetSD_h 1

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;
class HallA;

class BMDCDetSD:public G4VSensitiveDetector
{
public:

  BMDCDetSD(G4String SDname);

  ~BMDCDetSD();

public:

  G4bool ProcessHits(G4Step* step,G4TouchableHistory* hist);

  void EndOfEvent(G4HCofThisEvent* HCE);


};

#endif
