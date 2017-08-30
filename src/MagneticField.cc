#include "MagneticField.hh"

#define MAXBUFF 1024

MagneticField::MagneticField(G4ThreeVector off, G4RotationMatrix rm) {
    fOffset = off;
    frm = rm;

    fInverted= false;
}


MagneticField::~MagneticField() {
}

//////////////////////////////////////////////////////////////////////////////////////////

MappedField::MappedField(G4ThreeVector off, G4RotationMatrix rm, const char *fn)
   : MagneticField(off,rm) {

    strcpy( fFilename, fn);
    fFieldVal = NULL;
    fN[0] = fN[1] = fN[2] = 0;
}


MappedField::~MappedField() {
}

void MappedField::SetFilename(const char *fn)
{
    strcpy( fFilename, fn);
}
