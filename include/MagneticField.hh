#ifndef MagneticField_hh
#define MagneticField_hh

#include "globals.hh"
#include "G4MagneticField.hh"
#include "G4RotationMatrix.hh"

class MagneticField : public G4MagneticField {
    public:
	MagneticField(G4ThreeVector, G4RotationMatrix);
	virtual ~MagneticField();

	virtual void GetFieldValue( const  double Point[3], double *Bfield ) const = 0;

	void InvertField( G4bool b ){ fInverted = b; }

	void SetOffset( G4ThreeVector off ){ fOffset = off; }
	void SetRM( G4RotationMatrix rm ){ frm = rm; }

	G4bool fInverted;
    protected:

	G4ThreeVector fOffset;
	G4RotationMatrix frm;
};


////////////////////////////////////////////////////////////////////////////

class MappedField: public MagneticField {
    public:
	MappedField(G4ThreeVector, G4RotationMatrix,  const char *);
	virtual ~MappedField();
	void SetFilename(const char *);

    protected:
	virtual void ReadField() = 0;

	char fFilename[1024];

	int fN[3];
	double fMin[3], fMax[3];

	double ****fFieldVal;
};

#endif//MagneticField_hh
