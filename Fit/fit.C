#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TRandom.h"

using namespace std;

// Simplified version of cernbuild.C.
// This macro to read data from an ascii file and
// create a root file with a TTree
void fit(){
  const int maxhits = 40;
  int adcEnergy[maxhits];
  Long64_t hitTime[maxhits];
  int channel[maxhits];
  int eventHitCount;

  TFile *hfile = hfile = TFile::Open("False_Coincidence.root");

  TTree *tree = (TTree*)hfile->Get("data");

  tree->SetBranchAddress("adcEnergy", adcEnergy);
  tree->SetBranchAddress("hitTime", hitTime);
  tree->SetBranchAddress("channel", channel);
  tree->SetBranchAddress("eventHitCount", &eventHitCount);

  TFile *f = new TFile("histogram.root", "RECREATE");

  TH1F *Eadc = new TH1F("Eadc", "adcEnergy vs. counts", 700, 0, 7000);
  TH1F *hTime = new TH1F("hTime", "hitTime vs. counts", 3500000000, 0, 350000000000);
  TH1F *chan = new TH1F("chan", "channel vs. counts", 4, 0, 4);
  TH1F *eventhitCt = new TH1F("eventhitCt", "eventhitCount vs. counts", 2, 0, 2);

  for (int i =0; i<maxhits; i++){
    Eadc->Fill(adcEnergy[i]);
    hTime->Fill(hitTime[i]);
    chan->Fill(channel[i]);
  }
  eventhitCt->Fill(eventHitCount);

  Eadc->Write();
  hTime->Write();
  chan->Write();
  eventhitCt->Write();
}
