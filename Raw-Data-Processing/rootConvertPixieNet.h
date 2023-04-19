#ifndef ROOTCONVERTPIXIENET_H
#define ROOTCONVERTPIXIENET_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
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

// Constants
const int maxhits = 40; // Maximum number of hits allowed per event
const int eventMaxTime = 5000; // 10 microsecond event window

// Variables for each hit
extern int adcEnergy[maxhits];
extern long hitTime[maxhits];
extern int channel[maxhits];
extern int eventHitCount;

// Function prototypes
string GetFilename(string filename);
string GetFileExtension(string filename);
template <class Container>
void split2(const std::string &str, Container &cont, char delim = ',');
void ResetTreeVariables();

#endif // ROOTCONVERTPIXIENET_H

