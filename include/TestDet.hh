#ifndef TestDet_hh
#define TestDet_hh

class G4LogicalVolume;
class B1DetectorConstruction;

class TestDet{
public:

  TestDet(B1DetectorConstruction *,G4LogicalVolume *);
  ~TestDet();

private:

};

#endif
