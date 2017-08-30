#ifndef SeptaField_hh
#define SeptaField_hh

#include "globals.hh"
#include "MagneticField.hh"
#include "G4MagneticField.hh"
#include "G4RotationMatrix.hh"


class SeptaField : public MappedField {
    public:
	SeptaField(G4ThreeVector, G4RotationMatrix);
	~SeptaField();

	void GetFieldValue( const double Point[3], double *Bfield ) const;
    private:
	void ReadField();
};

#endif//SeptaField_hh
