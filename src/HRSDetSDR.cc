#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "HRSDetSDR.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

#include<fstream>
#include<iostream>

using namespace std;

extern G4int pid;

HRSDetSDR::HRSDetSDR(G4String name): G4VSensitiveDetector(name)
{

}

G4bool HRSDetSDR::ProcessHits(G4Step *step, G4TouchableHistory *hist)
{

  static int num = 0; //number of hits
  G4ThreeVector vertexPos; 
  const G4LogicalVolume* logic; 
  G4String vertexName;
  G4int PDG;
  G4ThreeVector worldPos;
  G4TouchableHandle touchable;
  G4ThreeVector localPos;
  G4double Mom;
  G4double E;
  G4double eDep;
  G4ThreeVector momdir;
  G4double theta;
  G4double phi;
  G4int eid;
  G4bool atBoundary;
  G4double stepLength;
  G4int cpNo;
  G4ThreeVector centerPos;


  G4TouchableHistory* theTouchable = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
  G4VPhysicalVolume* physVol = theTouchable->GetVolume();

  if(strcmp(physVol->GetName(),"HED_Right")==0){

    //copy number of detector box
    cpNo = physVol->GetCopyNo();

      //Event number
      eid = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();

      //Vertex position
      vertexPos = step->GetTrack()->GetVertexPosition();

      //Volume name
      logic = step->GetTrack()->GetLogicalVolumeAtVertex();
      vertexName = logic->GetName();

      //Particle ID
      PDG = step->GetTrack()->GetDefinition()->GetPDGEncoding();

      //Hit Position
      worldPos = step->GetPreStepPoint()->GetPosition();
      touchable = step->GetPreStepPoint()->GetTouchableHandle();
      localPos = touchable->GetHistory()->GetTransform(1).TransformPoint(worldPos);

      //Momentum
      Mom = step->GetTrack()->GetMomentum().mag();
      if (Mom==0.) return false;

      //Energy
      E = step->GetTrack()->GetKineticEnergy();

      //Energy deposited by this step
      eDep = step->GetTotalEnergyDeposit();

      //Particle momentum direction
      momdir = step->GetTrack()->GetMomentumDirection();

      // //Kill particle
      // if(step->GetPostStepPoint()->GetStepStatus() == fGeomBoundary)
      // 	{
      // 	  step->GetTrack()->SetTrackStatus(fStopAndKill);
      // 	}

      //calculating theta and phi

      theta = fabs((acos(momdir.z())));
      phi = 0;
      if(theta!=0) phi = fabs(atan(momdir.y()/momdir.x()));

      //Step length
      stepLength = step->GetStepLength();

      //center of the current physical volume
      centerPos = step->GetPreStepPoint()->GetPhysicalVolume()->GetTranslation();

    

      //Saving in a text file

      std::ostringstream convert;
      convert << pid;
      std::string pid_string = convert.str();

      std::ofstream file;
      file.open("HRSSDRfile.txt", std::ios::app);
      //file.open(("HRSSDRfile_" + pid_string + ".txt").c_str(), std::ios::app);


      file<< std::setw(2) << eid <<" "
	  << std::setw(2) << E/MeV <<" "
	  << std::setw(2) << cpNo <<" "
	  << std::setw(2) << stepLength/cm <<" "
	//<< std::setw(1)  << atBoundary <<" "
	  << std::setw(2) << eDep/MeV <<" "
	  << std::setw(2) << theta/deg <<" "
	  << std::setw(2) << phi/deg <<" "
	  << std::setw(2) << centerPos.getX()/cm <<" "
	  << std::setw(2) << centerPos.getY()/cm <<" "
	  << std::setw(2) << centerPos.getZ()/cm <<" "
	  << std::setw(2) << vertexPos.getX()/cm <<" "
	  << std::setw(2) << vertexPos.getY()/cm <<" "
	  << std::setw(2) << vertexPos.getZ()/cm <<" "
	//<< std::setw(2) << vertexName <<" "
	  << std::setw(2) << PDG << std::endl;

    file.close();
  }
  
  return true;
  
}


void HRSDetSDR::EndOfEvent(G4HCofThisEvent*)
{

}



HRSDetSDR::~HRSDetSDR()
{
}
