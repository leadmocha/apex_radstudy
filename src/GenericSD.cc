#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "GenericSD.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4TouchableHistory.hh"
#include "G4VPhysicalVolume.hh"
#include "IO.hh"

#include<fstream>
#include<iostream>

#include "G4TrajectoryContainer.hh"
#include "G4RunManager.hh"

using namespace std;

extern G4int pid;

GenericSD::GenericSD(G4String name, int data_type) : G4VSensitiveDetector(name),
  fDataType(data_type)
{
  fIOID = gIO->AddBranch(name);
}

G4bool GenericSD::ProcessHits(G4Step *step, G4TouchableHistory *hist)
{
  // Commenting out un-used variables (jc2)
  //G4ThreeVector vertexPos; 
  //G4ThreeVector hitPos;
  //const G4LogicalVolume* logic; 
  //G4String vertexName;
  //(moved)G4ThreeVector worldPos;
  //(moved)G4TouchableHandle touchable;
  //G4ThreeVector localPos;
  //(moved)G4double Mom;
  //(moved)G4ThreeVector momdir;
  //G4int eid;
  //G4bool atBoundary;
  //(moved)G4ThreeVector centerPos;
  sd_data data;


  G4TouchableHistory* theTouchable = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
  G4VPhysicalVolume* physVol = theTouchable->GetVolume();

  //copy number of detector box
  data.cpNo = physVol->GetCopyNo();

  //Track ID
  data.trackID = step->GetTrack()->GetTrackID();

  //Hit Position
  G4ThreeVector worldPos = step->GetPreStepPoint()->GetPosition();
  G4TouchableHandle touchable = step->GetPreStepPoint()->GetTouchableHandle();
  //localPos = touchable->GetHistory()->GetTransform(1).TransformPoint(worldPos);

  //Momentum
  //G4double Mom = step->GetPreStepPoint()->GetMomentum().mag();
  //if (Mom==0.) return false;
  // Ignore stationary particles (not sure how this can happen, but it was
  // coded in already, and I left it in (jc2)
  if(step->GetPreStepPoint()->GetMomentum().mag() == 0)
    return false;

  //Energy
  data.E = step->GetPreStepPoint()->GetKineticEnergy()/MeV;

  //Energy deposited by this step
  data.eDep = step->GetTotalEnergyDeposit()/MeV;

  //Particle momentum direction
  G4ThreeVector momdir = step->GetTrack()->GetMomentumDirection();

  //calculating theta and phi

  data.theta = fabs((acos(momdir.z())))/deg;
  data.phi = atan2(momdir.y(),momdir.x())/deg;

  //Step length
  data.stepLength = step->GetStepLength()/cm;
      
  //center of the current physical volume
  //(not used?)//G4ThreeVector centerPos = step->GetPreStepPoint()->GetPhysicalVolume()->GetTranslation();

  //Particle ID
  data.PDG = step->GetTrack()->GetDefinition()->GetPDGEncoding();
  // Track ID
  data.trackID = step->GetTrack()->GetTrackID();
  data.trackID = step->GetTrack()->GetParentID();
      
      
  //Get vertex point, 2nd interaction point after the target
  //(not used?)//G4int parentID = step->GetTrack()->GetParentID();

  // if(parentID==0 || parentID==1)
  //   {
  //     vertexPos = step->GetTrack()->GetVertexPosition();
  //   }
  
  // else
  //   {
  //     while(parentID!=1)
  // 	{
  //     	  RE01Trajectory* parentTrajectory = GetParentTrajectory(parentID);
  //     	  if(parentTrajectory==0)
  // 	    {
  // 	      G4cout << ">> Trajectory trace back failed - no parent found." << G4endl;
  // 	      break;
  // 	    }
  //     	  //parentTrajectory->ShowTrajectory();
	
  //     	  parentID = parentTrajectory->GetParentID();
  //     	  vertexPos = parentTrajectory->GetVertexPosition();
	
  //     	}
      
  //   }
  

  // //Get PDG and hitPos (position it hit before it enter detector)

  // G4String vertexVolumeName = step->GetTrack()->GetLogicalVolumeAtVertex()->GetName();
  // parentID = step->GetTrack()->GetParentID();
      
  // if(vertexVolumeName != "HED_Left")
  //   {
  //     PDG = step->GetTrack()->GetDefinition()->GetPDGEncoding();
  //     hitPos = step->GetTrack()->GetVertexPosition();
  //     trackID = step->GetTrack()->GetTrackID();
  //   }
  // else
  //   {
  //     while(parentID!=0 && vertexVolumeName=="HED_Left")
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
  
  /*
  vertexPos.setX(0);
  vertexPos.setY(0);
  vertexPos.setZ(0);
  */

  //Saving in a text file
  
  //if(step->GetPreStepPoint()->GetStepStatus() == fGeomBoundary || eDep != 0)
    {
      
      std::ostringstream convert;
      convert << pid;
      std::string pid_string = convert.str();
      
      //std::ofstream file;
      //file.open(("/scratch/mmk6qa/NewRun4GeV/HRSSDLfile_" + pid_string + ".txt").c_str(), std::ios::app);
      //file.open(("/home/cornejo/scratch/apex/test/HRSSDLfile_" + pid_string + ".txt").c_str(), std::ios::app);
      //file.open(("HRSSDLfile_" + pid_string + ".txt").c_str(), std::ios::app);
      
      
      data.worldPosX = worldPos.getX()/cm;
      data.worldPosY = worldPos.getY()/cm;
      data.worldPosZ = worldPos.getZ()/cm;
      //data.vertexPosX = vertexPos.getX()/cm;
      //data.vertexPosY = vertexPos.getY()/cm;
      //data.vertexPosZ = vertexPos.getZ()/cm;
      //file<< std::setw(2) << trackID <<" "
	  //<< std::setw(2) << E/MeV <<" "
	  //<< std::setw(2) << cpNo <<" "
	  //<< std::setw(2) << stepLength/cm <<" "
	  //<< std::setw(2) << eDep/MeV <<" "
	  //<< std::setw(2) << theta/deg <<" "
	  //<< std::setw(2) << phi/deg <<" "
	  //<< std::setw(2) << worldPos.getX()/cm <<" "
	  //<< std::setw(2) << worldPos.getY()/cm <<" "
	  //<< std::setw(2) << worldPos.getZ()/cm <<" "
	  //<< std::setw(2) << vertexPos.getX()/cm <<" "
	  //<< std::setw(2) << vertexPos.getY()/cm <<" "
	  //<< std::setw(2) << vertexPos.getZ()/cm <<" "
	  //<< std::setw(2) << PDG << std::endl;
      
     // file.close();
      
    }
    gIO->AddData(fIOID,data);
     //gIO->Fill(fIOID);
  
  
  return true;
}


RE01Trajectory* GenericSD::GetParentTrajectory(G4int parentID)
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


void GenericSD::EndOfEvent(G4HCofThisEvent*)
{
}

GenericSD::~GenericSD()
{
}
