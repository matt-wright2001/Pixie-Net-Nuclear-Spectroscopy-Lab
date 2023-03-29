#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>

#include <TChain.h>
#include <TTree.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TRandom.h>
#include <TF1.h>
#include <TF2.h>

#include <cstddef>

#include "math.h"

#include "TRandom3.h"

using namespace std;

//define things

//tree variables
void ResetTreeVariables();
	const Int_t maxhits = 40;

	int adcEnergy[maxhits];
	long time[maxhits];
	int channel[maxhits];
	int eventHitCount;


	// 1000 ns event window
	const int eventMaxTime = 1000;

