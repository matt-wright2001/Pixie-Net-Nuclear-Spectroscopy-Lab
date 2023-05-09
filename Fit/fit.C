#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TRandom.h"
#include <string>
#include <iostream>

using namespace std;

// Simplified version of cernbuild.C.
// This macro to read data from an ascii file and
// create a root file with a TTree
void fit(){
  int maxchannels = 4;

  string filename;
  cout << "Which root file would you like to make histograms of?" << endl;
  getline(cin, filename);

  TFile *hfile = TFile::Open(filename.c_str());
  if (!hfile) {
    cout << "Error: could not open file " << filename << endl;
    return 1;
  }

  string answer;
  cout << "Would you like to draw a 2d histogram? (enter y or n)" << endl;
  getline(cin, answer);

  TTree *tree1 = (TTree*)hfile->Get("data");

  const int events = tree1->GetEntries();

  const int maxhits = 40;
  double Ecal[maxhits];
  int adcEnergy[maxhits];
  Long64_t hitTime[maxhits];
  int channel[maxhits];
  int eventHitCount;
  int Eadc0entries[events];
  int Eadc1entries[events];
  
  tree1->SetBranchAddress("adcEnergy", adcEnergy);
  tree1->SetBranchAddress("Ecal", Ecal);
  tree1->SetBranchAddress("hitTime", hitTime);
  tree1->SetBranchAddress("channel", channel);
  tree1->SetBranchAddress("eventHitCount", &eventHitCount);

  TFile *f = new TFile("histogram.root", "RECREATE");

  TH1D *Eadc0 = new TH1D("Eadc_chan0", "adcEnergy vs. counts", 700, 0, 7000);
  TH1D *Eadc1 = new TH1D("Eadc_chan1", "channel vs. counts", 700, 0, 7000);
  TH2D *Eadc2d = new TH2D("Eadc_2d", "eventhitCount vs. counts", 700, 0, 7000, 700, 0, 7000);

  TH1D *Ecal0 = new TH1D("Ecal_chan0", "calEnergy vs. counts", 1000, 0, 4000);
  TH1D *Ecal1 = new TH1D("Ecal_chan1", "channel vs. counts", 1000, 0, 4000);
  TH2D *Ecal2d = new TH2D("Ecal_2d", "eventhitCount vs. counts", 1000, 0, 4000, 1000, 0, 4000);

  //fill 1d histograms
  for (int i =0; i<events; i++){
    tree1->GetEntry(i);
    for (int j = 0; j<eventHitCount; j++){
      if (channel[j] == 0){
	Eadc0->Fill(adcEnergy[j]);	
	Ecal0->Fill(Ecal[j]);
	//	Eadc0entries[i] = adcEnergy;
      }
      if (channel[j] == 1){
	Eadc1->Fill(adcEnergy[j]);
	Ecal1->Fill(Ecal[j]);
	//	Eadc1entries[i] = adcEnergy;
      }
    }
    //filling 2d histograms
    for(int j = 0; j< eventHitCount; j++){
      if(adcEnergy[j] >10){
	for(int k= j+1; k <eventHitCount; k++){
	  if(adcEnergy[k] >10){
	    Eadc2d->Fill(adcEnergy[j], adcEnergy[k]);
	    Ecal2d->Fill(Ecal[j], Ecal[k]);
	  }
	}
      }
    }
  }
  
  Eadc0->Write();
  Eadc1->Write();
  Eadc2d->Write();

  Ecal0->Write();
  Ecal1->Write();
  Ecal2d->Write();
  if (answer == "y"){
    Ecal2d->Draw();
  }
  
}