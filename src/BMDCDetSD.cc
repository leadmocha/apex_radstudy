#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "BMDCDetSD.hh"

#include<fstream>
#include<iostream>

using namespace std;

BMDCDetSD::BMDCDetSD(G4String name): G4VSensitiveDetector(name)
{

}

G4bool BMDCDetSD::ProcessHits(G4Step *step, G4TouchableHistory *hist)
{

  //Kill particle
  step->GetTrack()->SetTrackStatus(fStopAndKill);

   
}


void BMDCDetSD::EndOfEvent(G4HCofThisEvent*)
{

}



BMDCDetSD::~BMDCDetSD()
{
}
