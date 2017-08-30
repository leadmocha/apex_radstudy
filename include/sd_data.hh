#ifndef SD_DATA_HH
#define SD_DATA_HH

#include <vector>

struct sd_data {
  double E; // MeV
  double stepLength; // cm
  double eDep; // MeV
  double theta; // deg
  double phi; // deg
  double worldPosX; // cm
  double worldPosY; // cm
  double worldPosZ; // cm
  double vertexPosX; // cm
  double vertexPosY; // cm
  double vertexPosZ; // cm
  int PDG;
  int cpNo;
  int trackID;
  int parentID;
};

#endif // SD_DATA_HH
