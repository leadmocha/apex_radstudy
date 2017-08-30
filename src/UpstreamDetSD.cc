#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "UpstreamDetSD.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4TouchableHistory.hh"
#include "G4VPhysicalVolume.hh"

#include<fstream>
#include<iostream>

#include "G4TrajectoryContainer.hh"
#include "G4RunManager.hh"
#include "IO.hh"

using namespace std;

extern G4int pid;

UpstreamDetSD::UpstreamDetSD(G4String name): G4VSensitiveDetector(name)
{
  //fIOID = gIO->AddTree(name);

}

G4bool UpstreamDetSD::ProcessHits(G4Step *step, G4TouchableHistory *hist)
{

  static int num = 0; //number of hits
  G4ThreeVector vertexPos; 
  G4ThreeVector hitPos;
  const G4LogicalVolume* logic; 
  G4String vertexName;
  G4ThreeVector worldPos;
  G4TouchableHandle touchable;
  G4ThreeVector localPos;
  G4double Mom;
  G4ThreeVector momdir;
  G4int eid;
  G4bool atBoundary;
  G4ThreeVector centerPos;


  G4TouchableHistory* theTouchable = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
  G4VPhysicalVolume* physVol = theTouchable->GetVolume();

  //copy number of detector box
  //(*gIO)[fIOID].cpNo = physVol->GetCopyNo();

  //Track ID
  //(*gIO)[fIOID].trackID = step->GetTrack()->GetTrackID();
  
  //Hit Position
  worldPos = step->GetPreStepPoint()->GetPosition();
  touchable = step->GetPreStepPoint()->GetTouchableHandle();
  //localPos = touchable->GetHistory()->GetTransform(1).TransformPoint(worldPos);
  
  //Momentum
  Mom = step->GetPreStepPoint()->GetMomentum().mag();
  if (Mom==0.) return false;
  
  //Energy
  //(*gIO)[fIOID].E = step->GetPreStepPoint()->GetKineticEnergy();

  //Energy deposited by this step
  //(*gIO)[fIOID].eDep = step->GetTotalEnergyDeposit();
  
  //Particle momentum direction
  momdir = step->GetTrack()->GetMomentumDirection();
  
  //calculating theta and phi
  
  //(*gIO)[fIOID].theta = fabs((acos(momdir.z())));
  //(*gIO)[fIOID].phi = atan2(momdir.y(),momdir.x());
  
  //Step length
  //(*gIO)[fIOID].stepLength = step->GetStepLength();
  
  //center of the current physical volume
  centerPos = step->GetPreStepPoint()->GetPhysicalVolume()->GetTranslation();
  
  //Particle ID
  //(*gIO)[fIOID].PDG = step->GetTrack()->GetDefinition()->GetPDGEncoding();
  
  // //Get vertex point, 2nd interaction point after the target
  // G4int parentID = step->GetTrack()->GetParentID();
  
  // if(parentID==0 || parentID==1)
  //   {
  //     vertexPos = step->GetTrack()->GetVertexPosition();
  //   }
      
  // else
  //   {
  //     while(parentID!=1)
  // 	{
  // 	  RE01Trajectory* parentTrajectory = GetParentTrajectory(parentID);
  // 	  if(parentTrajectory==0)
  // 	    {
  // 	      G4cout << ">> Trajectory trace back failed - no parent found." << G4endl;
  // 	      break;
  // 	    }
  // 	  //parentTrajectory->ShowTrajectory();
	  
  // 	  parentID = parentTrajectory->GetParentID();
  // 	  vertexPos = parentTrajectory->GetVertexPosition();
	  
  // 	}
      
  //   }


  // //Get PDG and hitPos (position it hit before it enter detector)
  
  // G4String vertexVolumeName = step->GetTrack()->GetLogicalVolumeAtVertex()->GetName();
  // parentID = step->GetTrack()->GetParentID();
  
  // if(vertexVolumeName != "UpstreamDet")
  //   {
  //     PDG = step->GetTrack()->GetDefinition()->GetPDGEncoding();
  //     hitPos = step->GetTrack()->GetVertexPosition();
  //     trackID = step->GetTrack()->GetTrackID();
  //   }
  // else
  //   {
  //     while(parentID!=0 && vertexVolumeName=="UpstreamDet")
  // 	{
  // 	  RE01Trajectory* parentTrajectory = GetParentTrajectory(parentID);
  // 	  if(parentTrajectory==0)
  // 	    {
  // 	      G4cout << ">> Trajectory trace back failed - no parent found." << G4endl;
  // 	      break;
  // 	    }
	  
  // 	  parentID = parentTrajectory->GetParentID();
  // 	  vertexVolumeName = parentTrajectory->GetVolume()->GetName();
  // 	  PDG = parentTrajectory->GetPDGEncoding();
  // 	  hitPos = parentTrajectory->GetVertexPosition();
  // 	  trackID = parentTrajectory->GetTrackID();

  // 	}
  //   }

  vertexPos.setX(0);
  vertexPos.setY(0);
  vertexPos.setZ(0);
  

  //(*gIO)[fIOID].worldPosX = worldPos.getX()/cm;
  //(*gIO)[fIOID].worldPosY = worldPos.getY()/cm;
  //(*gIO)[fIOID].worldPosZ = worldPos.getZ()/cm;
  //(*gIO)[fIOID].vertexPosX = vertexPos.getX()/cm;
  //(*gIO)[fIOID].vertexPosY = vertexPos.getY()/cm;
  //(*gIO)[fIOID].vertexPosZ = vertexPos.getZ()/cm;
  //(*gIO)[fIOID].vertexPosX = vertexPos.getX()/cm;
  //(*gIO)[fIOID].vertexPosY = vertexPos.getY()/cm;
  //(*gIO)[fIOID].vertexPosZ = vertexPos.getZ()/cm;

  //gIO->Fill(fIOID);
  //Saving in a text file
 /* 
  //if(step->GetPreStepPoint()->GetStepStatus() == fGeomBoundary || eDep != 0)
    {
      
      std::ostringstream convert;
      convert << pid;
      std::string pid_string = convert.str();
      
      std::ofstream file;
      //file.open(("/scratch/mmk6qa/NewRun4GeV/UpSDfile_" + pid_string + ".txt").c_str(), std::ios::app);
      file.open(("/home/cornejo/scratch/apex/test/UpSDfile_" + pid_string + ".txt").c_str(), std::ios::app);
      //file.open(("UpSDfile_" + pid_string + ".txt").c_str(), std::ios::app);
      
      
      file<< std::setw(2) << trackID <<" "
	  << std::setw(2) << E/MeV <<" "
	  << std::setw(2) << cpNo <<" "
	  << std::setw(2) << stepLength/cm <<" "
	  << std::setw(2) << eDep/MeV <<" "
	  << std::setw(2) << theta/deg <<" "
	  << std::setw(2) << phi/deg <<" "
	  << std::setw(2) << worldPos.getX()/cm <<" "
	  << std::setw(2) << worldPos.getY()/cm <<" "
	  << std::setw(2) << worldPos.getZ()/cm <<" "
	  << std::setw(2) << vertexPos.getX()/cm <<" "
	  << std::setw(2) << vertexPos.getY()/cm <<" "
	  << std::setw(2) << vertexPos.getZ()/cm <<" "
	  << std::setw(2) << PDG << std::endl;
      
      file.close();
      
    }
    */
  
  return true;
}


RE01Trajectory* UpstreamDetSD::GetParentTrajectory(G4int parentID)
{
  G4TrajectoryContainer* container = G4RunManager::GetRunManager()->GetCurrentEvent()->GetTrajectoryContainer();
  if(container==0) return 0;
  size_t nTraj = container->size();
  
  for(size_t i=0;i<nTraj;i++)
    {
      RE01Trajectory* tr1 = (RE01Trajectory*)((*container)[i]);
      if(tr1->GetTrackID()==parentID) return tr1;
    }
  return 0;
}


void UpstreamDetSD::EndOfEvent(G4HCofThisEvent*)
{
  
}



UpstreamDetSD::~UpstreamDetSD()
{
}
