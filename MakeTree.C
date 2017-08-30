#include "Riostream.h"
#include "TFile.h"
#include "TTree.h"

using namespace std;

void MakeTree(void)
{
// Example of a script which reads some data and fills a TTree
// with a branch for each variable describing the data

   //declaration of a structure for the data
   struct data_t{
     Int_t Eid;
     Double_t wPosx;
     Double_t wPosy;
     Double_t wPosz;
     Double_t lPosx;
     Double_t lPosy;
     Double_t lPosz;
     // Double_t vPosx;
     // Double_t vPosy;
     // Double_t vPosz;
     Double_t E;
     Double_t p;
     Double_t Momdirx;
     Double_t Momdiry;
     Double_t Momdirz;
     Double_t theta;
     Double_t phi;
     Double_t eDep;
     Int_t PDG;
   };
   
   data_t data;
	
   // open the data file for reading
   ifstream file;
   file.open("file.txt");
      
   //UInt_t nevents = 100;
   
   //open ROOT file to save the TTree in
   TFile *ftree = new TFile("file.root","recreate");
   
   //creation of TTree
   TTree *t = new TTree("t", "TTree avec une structure");
   
   //creation of branches to hold the variables of the structure
   t->Branch("Eid", &data.Eid, "Eid/I");
   t->Branch("wPosx", &data.wPosx, "wPosx/D");
   t->Branch("wPosy", &data.wPosy, "wPosy/D");
   t->Branch("wPosz", &data.wPosz, "wPosz/D");
   t->Branch("lPosx", &data.lPosx, "lPosx/D");
   t->Branch("lPosy", &data.lPosy, "lPosy/D");
   t->Branch("lPosz", &data.lPosz, "lPosz/D");
   // t->Branch("vPosx", &data.vPosx, "vPosx/D");
   // t->Branch("vPosy", &data.vPosy, "vPosy/D");
   // t->Branch("vPosz", &data.vPosz, "vPosz/D");
   t->Branch("E", &data.E, "E/D");
   t->Branch("p", &data.p, "p/D");
   t->Branch("Momdirx", &data.Momdirx, "Momdirx/D");
   t->Branch("Momdiry", &data.Momdiry, "Momdiry/D");
   t->Branch("Momdirz", &data.Momdirz, "Momdirz/D");
   t->Branch("theta", &data.theta, "theta/D");
   t->Branch("phi", &data.phi, "phi/D");
   t->Branch("eDep", &data.eDep, "eDep/D");
   t->Branch("PDG",&data.PDG, "PDG/I");
   
   //loop over the 'nevents' events
   //while( (nevents--) > 0 ){
   	
      //read the multiplicity of the event
     // file >> event.Mult;
      
      //loop over the particles of the event
   
   while(!file.eof()){
      	//read the properties of each particle
        file >> data.Eid;
	file >> data.wPosx;
	file >> data.wPosy;
	file >> data.wPosz;
	file >> data.lPosx;
	file >> data.lPosy;
	file >> data.lPosz;
	// file >> data.vPosx;
	// file >> data.vPosy;
	// file >> data.vPosz;
	file >> data.E;
	file >> data.p;
	file >> data.Momdirx;
	file >> data.Momdiry;
	file >> data.Momdirz;
	file >> data.theta;
	file >> data.phi;
	file >> data.eDep;
	file >> data.PDG;
	t->Fill();
   }
      
      //fill the tree
      // t->Fill();
         
      //if( !(nevents%1000) ) cout << "Events left = " << nevents << endl;
      //}
   
   file.close();
   
   //write and close the ROOT file
   t->Write();
   delete ftree;
}
