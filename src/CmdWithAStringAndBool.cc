#include "CmdWithAStringAndBool.hh"
#include <G4Tokenizer.hh>

CmdWithAStringAndBool::CmdWithAStringAndBool
(const char * theCommandPath,G4UImessenger * theMessenger)
:G4UIcommand(theCommandPath,theMessenger)
{
  G4UIparameter *detParam = new G4UIparameter('s');
  SetParameter(detParam);
  G4UIparameter *boolParam = new G4UIparameter('b');
  SetParameter(boolParam);
  boolParam->SetParameterName("boolean");
}

std::pair<G4String,G4bool> CmdWithAStringAndBool::GetResult(G4String pList)
{
  G4Tokenizer tkn(pList);
  std::pair<G4String,G4bool> res;
  res.first = tkn();
  res.second = false;
  G4String str = tkn();
  if( str == "" ) { // return the default value
    str = GetParameter(1)->GetDefaultValue();
  }
  res.second = ConvertToBool(str);
  return res;
}

void CmdWithAStringAndBool::SetParameterName
(const char * theName,G4bool omittable,G4bool currentAsDefault)
{
  G4UIparameter *detParam = GetParameter(0);
  G4UIparameter *boolParam = GetParameter(1);
  detParam->SetParameterName(theName);
  boolParam->SetOmittable(omittable);
  boolParam->SetCurrentAsDefault(currentAsDefault);
}

void CmdWithAStringAndBool::SetDefaultValue(G4bool defVal)
{
  G4UIparameter * theParam = GetParameter(0);
  theParam->SetDefaultValue(defVal);
}

