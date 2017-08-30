#include "TTree.h"
#include "TFile.h"
#include "iostream.h"
#include "TH1.h"

#define PI 3.14159265359

using namespace std;

void plot(void){

  TFile *f = new TFile("file.root");
  TTree *a = (TTree *)f->Get("t");
  a->StartViewer();

  TCanvas *myCanvas = new TCanvas();
  myCanvas->SetLogx();
  myCanvas->SetLogy();

  myCanvas->TPad::SetGridx(1);
  myCanvas->TPad::SetGridy(1);


  //Variable (logarithmic) bin sizes

  const Int_t nbins = 500;
  Double_t xmin = 1e-3;
  Double_t xmax = 1e5;
  Double_t logxmin = TMath::Log10(xmin);
  Double_t logxmax = TMath::Log10(xmax);
  Double_t binwidth = (logxmax-logxmin)/nbins;
  Double_t xbins[nbins+1];

  xbins[0] = xmin;
  for (Int_t i=1;i<=nbins;i++){
    xbins[i] = xmin + TMath::Power(10,logxmin+i*binwidth);
  }

  TH1F* h1 = new TH1F("h1","gamma",nbins,xbins);
  //TH1F* h1 = new TH1F("h1","Hist",1e+3,1e-3,1e+5);
  
  h1->SetMinimum(1e-10);
  h1->SetMaximum(1e+1);
  
  TCut cut1("PDG==22");
  TCut cut2("theta>=0 && theta<=10");
  a->Draw("E>>h1",cut1 && cut2,"");

  double Binwidth, Entry;
  double min, max;
  for (int i=0;i<=(h1->GetNbinsX());i++){
  
      Binwidth = h1->GetBinWidth(h1->GetBin(i));
      //cout<<Entry<<"\t"<<Binwidth<<endl;




      Entry = h1->GetBinContent(h1->GetBin(i));

      min = (PI/180)*0;
      max = (PI/180)*10;
      
      h1->SetBinContent(h1->GetBin(i),Entry/10000000/Binwidth/SolidAngle(min,max));
      //h1->SetBinContent(h1->GetBin(i),Entry);

      h1->SetXTitle("T (MeV)");
      h1->GetXaxis()->CenterTitle();
      h1->SetTitleOffset(1.2,"X");

      h1->SetYTitle("N^{-1}_{e}  dN/dT/d#Omega  (electron^{-1}  MeV^{-1}  sr^{-1})");
      h1->GetYaxis()->CenterTitle();
      h1->SetTitleOffset(1.2,"Y");

      

  }
  cout<<SolidAngle(min,max)<<endl;
    

  // TCanvas *myCanvas1 = new TCanvas();
  
  // myCanvas1->SetLogy();

  // TH1F* h11 = new TH1F("h11","Hist",180,0,180);
  
  // h11->SetMinimum(1e-7);
  // h11->SetMaximum(1e2);
  
  // //t->Draw("theta>>h11","PDG==22 && E>0.01");


  // for (int i=0;i<=(h11->GetNbinsX());i++){
  //     Entry = h11->GetBinContent(h11->GetBin(i,0,0));
  //     min = 0;
  //     max = (PI/180)*10;
  //     h11->SetBinContent(h11->GetBin(i,0,0),Entry/10000/SolidAngle(min,max));
      
  //     h11->SetXTitle("#Theta (degrees)");
  //     h11->GetXaxis()->CenterTitle();
  //     h11->SetTitleOffset(1.2,"X");

  //     h11->SetYTitle("N^{-1}_{e}  dN/d#Omega  (electron^{-1} sr^{-1})");
  //     h11->GetYaxis()->CenterTitle();
  //     h11->SetTitleOffset(1.2,"Y");

  //}

  
}


  double SolidAngle(double xmin, double xmax){

    double a1 = 2*PI*(-cos(xmax));
    double a2 = 2*PI*(-cos(xmin));
    return (a1-a2);

  }
