//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********4************************************************************
//
// $Id: exampleB1.cc 75216 2013-10-29 16:08:11Z gcosmo $
//
/// \file exampleB1.cc
/// \brief Main program of the B1 example

#include "Messenger.hh"
#include "IO.hh"
#include "B1DetectorConstruction.hh"
#include "B1ActionInitialization.hh"
//#include "B1PrimaryGeneratorAction.hh" //Added
//#include "B1RunAction.hh" //added
//#include "B1EventAction.hh" //added
//#include "B1SteppingAction.hh" //added

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
//#include "QBBC.hh"
//#include "G4PhysListFactory.hh"
#include "G4VModularPhysicsList.hh"
#include "G4SBSPhysicsList.hh"
#include "QGSP_BERT_HP.hh"


#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "Randomize.hh"

#include <sys/types.h>
#include <unistd.h>

//Global variable
G4int pid = (int) getpid();

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Function copied from g4sbs.cc which parses command line arguments
bool parseArgument(std::string arg, std::string &name, std::string &value)
{
  size_t length = arg.length();

  // There are two possible types of parameters.
  // 1) Parameters with a value (--name=value)
  // 2) Parameters with no value (--name), also known as flags.

  // Minimum size for the argument is 3, two dashes and at least one
  // other character
  if(length<3)
    return false; // Not long enough to be a valid parameter

  // Now, look for the double dash at the beginning that will signal
  // that a parameter is being specified
  size_t pos1 = arg.find_first_of("--");
  if(pos1 != std::string::npos && pos1 == 0) { // Double dash found

    // First, identify if it is parameter type 1 by looking for the equal sign
    size_t pos2 = arg.find_first_of("=");
    if(pos2 != std::string::npos && pos2 < length-1 && pos2>0) { // Param type 1
      // Split name from value using on both sides of the equal sign
      name = arg.substr(2,pos2-2);
      value = arg.substr(pos2+1,length);
      return true;
    } else { // No equal sign means parameter type 2
      name = arg.substr(2,length);
      return true;
    }
  }
  return false;
}

// (jc2): Copied from g4sbs.cc
void executeMacro(G4String macro, G4UImanager *UImanager)
{
  if(macro.length()>0) {
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command+macro);
  }
}


int main(int argc,char** argv)
{
  G4String config_macro("radStudy.conf");
  G4String post_macro("");
  G4String vis_macro("init_vis.mac");
  // Parse command line options here
  // (jc2): Copied over from g4sbs.cc
  for(int i = 1; i < argc; i++) {
    std::string paramName;
    std::string paramValue;
    bool validParam = parseArgument(argv[i],paramName,paramValue);
    if(validParam) {
      if(paramName.compare("config") == 0) {
        config_macro = paramValue;
      } else if (paramName.compare("post") == 0) {
        post_macro = paramValue;
      } else if (paramName.compare("vis") == 0) {
        vis_macro = paramValue;
      } else {
        G4cerr << "Unknown command: " << paramName << G4endl;
        exit(-1);
      }
    }
  }

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Start the global radStudy messenger
  Messenger *messenger = new Messenger();
  if(!config_macro.isNull())
    executeMacro(config_macro,UImanager);

  // Choose the Random engine
  //
  //G4Random::setTheEngine(new CLHEP::RanecuEngine);

  // Initialize the CLHEP random engine used by
    // "shoot" type functions

    unsigned int seed = time(0) + (int) getpid();

    unsigned int devrandseed = 0;
    //  /dev/urandom doens't block
    FILE *fdrand = fopen("/dev/urandom", "r");
    if( fdrand ){
  	fread(&devrandseed, sizeof(int), 1, fdrand);
  	seed += devrandseed;
  	fclose(fdrand);
    }

    CLHEP::HepRandom::createInstance();
    CLHEP::HepRandom::setTheSeed(seed);


  // //Another method to set the seed
  // CLHEP::RanluxEngine defaultEngine( 1234567, 4 ); 
  // G4Random::setTheEngine( &defaultEngine ); 
  // G4int seed = time( NULL ); 
  // G4Random::setTheSeed( seed ); 

  
  // Construct the default run manager
  //
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
#else
  G4RunManager* runManager = new G4RunManager;
#endif

  // Set mandatory initialization classes
  //
  // Detector construction
  runManager->SetUserInitialization(new B1DetectorConstruction());

  // Physics list
  // G4VModularPhysicsList* physicsList = new QBBC;
  // physicsList->SetVerboseLevel(1);
  // runManager->SetUserInitialization(physicsList);

  //using G4SBS physics list
  //G4VModularPhysicsList *physicslist = new G4SBSPhysicsList;
  //runManager->SetUserInitialization(physicslist);

  //QGSP_BERT_HP
  G4VModularPhysicsList* myPL = new QGSP_BERT_HP;
  runManager->SetUserInitialization(myPL);
    
  // User action initialization
  runManager->SetUserInitialization(new B1ActionInitialization());

  //Primary generator Action //added

  //runManager->SetUserAction(new B1PrimaryGeneratorAction());

  //Set user action classes
  //
 //Event Action
  //B1EventAction* eventAction = new B1EventAction;
  //runManager->SetUserAction(eventAction);

  //Stepping Action
  //runManager->SetUserAction(new B1SteppingAction(eventAction));

  //Run action
  //runManager->SetUserAction(new B1RunAction());

  // Initialize G4 kernel
  //
  runManager->Initialize();
  
#ifdef G4VIS_USE
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
#endif

  //gIO->Init();

  if (!post_macro.isNull()) {
    // batch mode
    executeMacro(post_macro, UImanager);
    //G4String command = "/control/execute ";
    //G4String fileName = argv[1];
    //UImanager->ApplyCommand(command+fileName);
  }
  else {
    // interactive mode : define UI session
#ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute " + vis_macro); 
#else
    UImanager->ApplyCommand("/control/execute init.mac"); 
#endif
    ui->SessionStart();
    delete ui;
#endif
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !
  
#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  gIO->Close();
  delete messenger;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
