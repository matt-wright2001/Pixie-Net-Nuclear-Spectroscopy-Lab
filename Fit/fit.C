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

  TFile *hfile = hfile = TFile::Open("coincidence.root");

  TTree *tree = (TTree*)hfile->Get("data");

  

  tree->SetBranchAddress("adcEnergy", adcEnergy);
  tree->SetBranchAddress("hitTime", hitTime);
  tree->SetBranchAddress("channel", channel);
  tree->SetBranchAddress("eventHitCount", &eventHitCount);

  TFile *f = new TFile("histogram.root");

  TH1F *adcEnergy = new TH1F("adc",,)
}
