#ifndef ROOTCONVERTPIXIENET_H
#define ROOTCONVERTPIXIENET_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <cstddef>

#include <TChain.h>
#include <TTree.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TRandom.h>
#include <TF1.h>
#include <TF2.h>

#include "math.h"
#include "TRandom3.h"

using namespace std;

// TTree variables
const int maxhits = 40;

// Variables for each hit
extern int adcEnergy[maxhits];
extern long hitTime[maxhits];
extern int channel[maxhits];
extern int eventHitCount;

// 1000 ns event window
const int eventMaxTime = 1000;

// Function to reset tree variables
void ResetTreeVariables();

#endif // ROOTCONVERTPIXIENET_H

