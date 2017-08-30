#ifndef CmdWithAStringAndBool_H
#define CmdWithAStringAndBool_H 1

#include "G4UIcommand.hh"


// class description:
//  A concrete class of G4UIcommand. The command defined by this class
// takes a boolean value. Boolean value can be the following notations.
//    TRUE : 
//       1 t T true TRUE
//    FALSE : 
//       0 f F false FALSE
//  General information of G4UIcommand is given in G4UIcommand.hh.

class CmdWithAStringAndBool : public G4UIcommand
{
  public: // with description
    CmdWithAStringAndBool
    (const char * theCommandPath,G4UImessenger * theMessenger);
    //  Constructor. The command string with full path directory
    // and the pointer to the messenger must be given.
    void SetParameterName(const char * theName,G4bool omittable,
                          G4bool currentAsDefault=false);
    //  Set the parameter name for a boolean parameter.
    //  If "omittable" is set as true, the user of this command can ommit
    // the value when he/she applies the command. If "omittable" is false,
    // the user must supply a boolean value.
    //  "currentAsDefault" flag is valid only if "omittable" is true. If this
    // flag is true, the current value is used as the default value when the 
    // user ommit the parameter. If this flag is false, the value given by the 
    // next SetDefaultValue() method is used.
    void SetDefaultValue(G4bool defVal);
    //  Set the default value of the parameter. This default value is used
    // when the user of this command ommits the parameter value, and
    // "ommitable" is true and "currentAsDefault" is false.
    std::pair<G4String,G4bool> GetResult(G4String pList);
};

#endif
