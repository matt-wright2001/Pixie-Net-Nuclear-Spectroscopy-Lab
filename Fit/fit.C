#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TRandom.h"
#include <iostream>

using namespace std;

// Simplified version of cernbuild.C.
// This macro to read data from an ascii file and
// create a root file with a TTree
void fit(){
  int maxchannels = 4;
  TFile *hfile = hfile = TFile::Open("coincidence.root");

  TTree *tree1 = (TTree*)hfile->Get("data");

  const int events = tree1->GetEntries();

  const int maxhits = 40;
  int adcEnergy[maxhits];
  Long64_t hitTime[maxhits];
  int channel[maxhits];
  int eventHitCount;
  int Eadc0entries[events];
  int Eadc1entries[events];
  
  tree1->SetBranchAddress("adcEnergy", adcEnergy);
  tree1->SetBranchAddress("hitTime", hitTime);
  tree1->SetBranchAddress("channel", channel);
  tree1->SetBranchAddress("eventHitCount", &eventHitCount);

  TFile *f = new TFile("histogram.root", "RECREATE");

  TH1F *Eadc0 = new TH1F("Eadc_chan0", "adcEnergy vs. counts", 700, 0, 7000);
  TH1F *Eadc1 = new TH1F("Eadc_chan1", "channel vs. counts", 700, 0, 7000);
  TH2F *Eadc2d = new TH2F("Eadc_2d", "eventhitCount vs. counts", 700, 0, 7000, 700, 0, 7000);

  //fill 1d histograms
  for (int i =0; i<events; i++){
    tree1->GetEntry(i);
    for (int j = 0; j<eventHitCount; j++){
      if (channel[j] == 0){
	Eadc0->Fill(adcEnergy[j]);
	//	Eadc0entries[i] = adcEnergy;
      }
      if (channel[j] == 1){
	Eadc1->Fill(adcEnergy[j]);
	//	Eadc1entries[i] = adcEnergy;
      }
    }
    //filling 2d histograms
    for(int j = 0; j< eventHitCount; j++){
      if(adcEnergy[j] >10){
	for(int k= j+1; k <eventHitCount; k++){
	  if(adcEnergy[k] >10){
	    Eadc2d->Fill(adcEnergy[j], adcEnergy[k]);
	  }
	}
      }
    }
  }
  
  Eadc0->Write();
  Eadc1->Write();
  Eadc2d->Write("colz");
}
