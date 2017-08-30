#ifndef IO_HH
#define IO_HH

#include "RadConfig.hh"
#include "sd_data.hh"
#include <vector>

class TFile;
class TTree;

// Vector version of the sd_data structure, which we'll actually
// store on the root tree
struct sd_data_tree {
  std::vector<double> *E; // MeV
  std::vector<double> *stepLength; // cm
  std::vector<double> *eDep; // MeV
  std::vector<double> *theta; // deg
  std::vector<double> *phi; // deg
  std::vector<double> *worldPosX; // cm
  std::vector<double> *worldPosY; // cm
  std::vector<double> *worldPosZ; // cm
  std::vector<double> *vertexPosX; // cm
  std::vector<double> *vertexPosY; // cm
  std::vector<double> *vertexPosZ; // cm
  std::vector<int> *PDG;
  std::vector<int> *cpNo;
  std::vector<int> *trackID;
  std::vector<int> *parentID;
};





class IO {
public:
  IO();
  virtual ~IO();
  //sd_data& operator[](unsigned int i) { return fData[i]; }
  //sd_data operator[](unsigned int i) const { return fData[i]; }
  void EndOfEvent();

  //int AddTree(G4String name);
  void AddData(int id, sd_data);
  int AddBranch(G4String name);
  void Fill();
  void Init();
  void Close();
private:
  TFile *fFile;
  TTree *fTree;
  //std::vector<TTree*> fTrees;
  std::vector<sd_data_tree> fData;
  std::vector<G4String> fNames;
  bool fInitialized;
  int fEventNum;

public:
  static IO* GetInstance();
  static IO *fInstance;
};

#define gIO IO::GetInstance()

#endif // IO_HH
