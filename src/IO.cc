#include "IO.hh"
#include <TFile.h>
#include <TTree.h>

IO* IO::fInstance = new IO();

IO::IO() : fInitialized(false)
{
}


IO::~IO()
{
}

void IO::Close()
{
  fFile->Write();
  fFile->Close();
}

void IO::Fill()
{
  //if(!fInitialized)
  //  Init();
  fTree->Fill();
}

void IO::Init()
{
  fFile = new TFile(gRadConfig->RootFilename,"RECREATE");
  fTree = new TTree("T","Tree for Sensitive Detector");
  for(size_t id = 0; id < fNames.size(); id++) {
    fTree->Branch(TString::Format("%s.E",fNames[id].data()),&(fData[id].E));
    fTree->Branch(TString::Format("%s.stepLength",fNames[id].data()),&(fData[id].stepLength));
    fTree->Branch(TString::Format("%s.eDep",fNames[id].data()),&(fData[id].eDep));
    fTree->Branch(TString::Format("%s.theta",fNames[id].data()),&(fData[id].theta));
    fTree->Branch(TString::Format("%s.phi",fNames[id].data()),&(fData[id].phi));
    fTree->Branch(TString::Format("%s.worldPosX",fNames[id].data()),&(fData[id].worldPosX));
    fTree->Branch(TString::Format("%s.worldPosY",fNames[id].data()),&(fData[id].worldPosY));
    fTree->Branch(TString::Format("%s.worldPosZ",fNames[id].data()),&(fData[id].worldPosZ));
    fTree->Branch(TString::Format("%s.PDG",fNames[id].data()),&(fData[id].PDG));
    fTree->Branch(TString::Format("%s.trackID",fNames[id].data()),&(fData[id].trackID));
    fTree->Branch(TString::Format("%s.parentID",fNames[id].data()),&(fData[id].parentID));
  }
  fInitialized = true;
  fEventNum = 0;

  /*
  fFile = new TFile(gRadConfig->RootFilename,"RECREATE");
  for(size_t i = 0; i < fNames.size(); i++) {
    TTree *tree = new TTree(fNames[i].data(),"Tree for Sensitive Detector");
    tree->Branch("E",&(fData[i].E));
    tree->Branch("stepLength",&(fData[i].stepLength));
    tree->Branch("eDep",&(fData[i].eDep));
    tree->Branch("theta",&(fData[i].theta));
    tree->Branch("phi",&(fData[i].phi));
    tree->Branch("worldPosX",&(fData[i].worldPosX));
    tree->Branch("worldPosY",&(fData[i].worldPosY));
    tree->Branch("worldPosZ",&(fData[i].worldPosZ));
    tree->Branch("PDG",&(fData[i].PDG));
    tree->Branch("trackID",&(fData[i].trackID));
    tree->Branch("event_num",&fEventNum);
    fTrees.push_back(tree);
  }
  fInitialized = true;
  fEventNum = 0;
  */
}

int IO::AddBranch(G4String name)
{
  fData.push_back(sd_data_tree());
  fNames.push_back(name);
  return fNames.size()-1;
}

//int IO::AddTree(G4String name)
//{
  //fData.push_back(sd_data());
  //fNames.push_back(name);
  //return fNames.size()-1;
  //fTree->Branch(name.data(),(*data.E),"E/D:cpNo/I:stepLength/I:eDep/D:theta/D:phi/D:worldPosX/D:worldPosY/D:worldPosZ/D:vertexPosX/D:vertexPosY/D:vertexPosZ/D:PDG/I");
//}

IO* IO::GetInstance()
{
  return fInstance;
}

void IO::AddData(int id, sd_data data)
{
  fData[id].E->push_back(data.E);
  fData[id].stepLength->push_back(data.stepLength);
  fData[id].eDep->push_back(data.eDep);
  fData[id].theta->push_back(data.theta);
  fData[id].phi->push_back(data.phi);
  fData[id].worldPosX->push_back(data.worldPosX);
  fData[id].worldPosY->push_back(data.worldPosY);
  fData[id].worldPosZ->push_back(data.worldPosZ);
  fData[id].PDG->push_back(data.PDG);
  fData[id].trackID->push_back(data.trackID);
  fData[id].parentID->push_back(data.parentID);
}

void IO::EndOfEvent()
{
  Fill();
  for(size_t id = 0; id < fData.size(); id++) {
    fData[id].E->clear();
    fData[id].stepLength->clear();
    fData[id].eDep->clear();
    fData[id].theta->clear();
    fData[id].phi->clear();
    fData[id].worldPosX->clear();
    fData[id].worldPosY->clear();
    fData[id].worldPosZ->clear();
    fData[id].PDG->clear();
    fData[id].trackID->clear();
    fData[id].parentID->clear();
  }
}
