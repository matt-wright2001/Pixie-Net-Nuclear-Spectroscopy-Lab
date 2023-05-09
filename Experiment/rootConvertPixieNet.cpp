/******************************************************************************
 * File: rootConvertPixieNet.cpp
 * Author(s):
 *      Student: M.S. Wright, B.R. Clark, D.C. Heson
 *      Faculty Advisors: B.P. Crider, J.A. Winger
 * Organization: Mississippi State University, Dept. of Physics and Astronomy 
 * Date: April 1, 2023 
 *
 * Description:
 * This program converts raw XIA Pixie-Net data files into a ROOT TTree format
 * for nuclear spectroscopy data analysis and visualization. It is part of a
 * senior class project led by students M.S. Wright, B.R. Clark, and D.C. Heson
 * with guidance from Professors B.P. Crider and J.A. Winger.
 *
 * The program processes raw data from a .dat file acquired using the XIA Pixie-Net
 * data acquisition module, groups hits into events based on their timestamps,
 * and stores the processed data in a ROOT TTree format. The output file is saved
 * with a .root extension, preserving the original filename. The resulting TTree
 * can be used for further analysis, such as energy calibration, peak fitting, and
 * visualization.
 *****************************************************************************/

#include "rootConvertPixieNet.h"
#include <TCanvas.h>
#include <TF1.h>
#include <TGraph.h>
#include <TMath.h>
#include <vector>
#include <random>


// Define external variables
int adcEnergy[maxhits];
double Ecal[maxhits];
long hitTime[maxhits];
int channel[maxhits];
int eventHitCount;
float slope[4];
float yint[4];

using namespace std;

// Function to strip the path and extension off of the filename
string GetFilename(string filename) {
  const size_t last_slash_idx = filename.find_last_of("\\/");
  if (std::string::npos != last_slash_idx) {
    filename.erase(0, last_slash_idx + 1);
  }

  const size_t period_idx = filename.rfind('.');
  if (std::string::npos != period_idx) {
    filename.erase(period_idx);
  }
  return filename;
}

// Function to get the file extension
string GetFileExtension(string filename) {
  const size_t last_slash_idx = filename.find_last_of("\\/");
  if (std::string::npos != last_slash_idx) {
    filename.erase(0, last_slash_idx + 1);
  }

  const size_t period_idx = filename.rfind('.');
  std::string extension = filename.substr(period_idx, std::string::npos);
  return extension;
}

// Function to split strings
template <class Container>
void split2(const std::string &str, Container &cont, char delim = '\t') {
  std::stringstream ss(str);
  std::string token;
  while (std::getline(ss, token, delim)) {
    cont.push_back(token);
  }
}

// Function to reset tree variables
void ResetTreeVariables() {
  for (int i = 0; i < maxhits; i++) {
    channel[i] = -1;
    adcEnergy[i] = -1;
    Ecal[i] = -1;
    hitTime[i] = -1;
  }
  eventHitCount = 0;
}

void fitGraphs(TGraph* grArray[4]) {
    // Create a new ROOT file to store the TCanvas objects
    TFile* file = new TFile("calibration_Graph.root", "RECREATE");

    // Loop over the four TGraphs
    for (int i = 0; i < 4; i++) {
        TGraph* gr = grArray[i];

        // Create a TF1 object for a first order polynomial
        TF1* func = new TF1("func", "[0] + [1]*x", gr->GetXaxis()->GetXmin(), gr->GetXaxis()->GetXmax());

        // Set some initial parameter values and ranges
        func->SetParameter(0, 0);
        func->SetParameter(1, 1);
        func->SetParLimits(0, -100, 100);
        func->SetParLimits(1, -10, 10);

        // Fit the TGraph with the polynomial function
        gr->Fit(func, "Q");

        // Get the fit parameters and their errors
        double param0 = func->GetParameter(0);
        double param1 = func->GetParameter(1);
	slope[i] = param1;
	yint[i] = param0;
        double param0Err = func->GetParError(0);
        double param1Err = func->GetParError(1);

        // Print the fit results
	cout << "Fit results for graph " << i << ":" <<  endl;
	cout << "  Parameter 0: " << param0 << " +/- " << param0Err <<  endl;
	cout << "  Parameter 1: " << param1 << " +/- " << param1Err <<  endl;

        // Draw the TGraph and the fit function
        TCanvas* c1 = new TCanvas(Form("c%d", i), Form("Graph and Fit %d", i), 800, 600);
	gr->SetMarkerStyle(20);
        gr->SetMarkerSize(1.2);
        gr->Draw("AP");
        func->Draw("same");

        // Write the TCanvas to the ROOT file
        c1->Write();
    }

    // Close the ROOT file
    file->Close();
}

// Main function
int main(int argc, char *argv[]) {
  // Declare variables
  string line;
  vector<string> vect;

  // Read in the line values to calibrate each channel
  ifstream calFile("calibration.dat");
  if (!calFile){
    cerr << "Error: calibration.dat could not be opened" << endl;
    exit(1);
  }
  
  //float channelnum[7];
  //  float energy[7];
  TGraph* grArray[4];
  vector<float> channelnum;
  vector<float> energy;
  string num;
  string filler;

  getline(calFile, filler);
  filler.clear();
  getline(calFile, filler);
  filler.clear();
  getline(calFile, filler);
  filler.clear();

  // Read the file and calculate the slopes and y intercepts for each channel
  
  for (int i = 0; i < 4; i++){
    channelnum.clear();
    energy.clear();
   for (int j = 0; j < 7; j++){
        calFile >> num;
        calFile >> num;
        if (num == "none"){
            calFile >> num;
        }
        else{
	  cout << num << " : ";
            energy.push_back(stof(num));
            calFile >> num;
	    cout << num << endl;
            channelnum.push_back(stof(num));
        }
    }
    grArray[i] = new TGraph(energy.size(), channelnum.data(), energy.data());
}

fitGraphs(grArray);

    /*
      calFile >> num;
      calFile >> num;
      energy[1] = num;
      calFile >> num;
      channelnum[1] = num;
      slope[i] = ((energy[1]-energy[0])/(channelnum[1] - channelnum[0]));
      yint[i] = energy[0] - slope[i]*channelnum[0];
      cout << slope[i] << " " << yint[i] << endl;
    */
    // Check input arguments
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " <input_file>" << endl;
    return 1;
  }

  // Verify input file has the correct extension
  if (GetFileExtension(argv[1]) != ".dat") {
    cout << "Error: Invalid input file extension. Please provide a .dat file." << endl;
    return 1;
  }

  // Open input file
  ifstream infile(argv[1]);
  if (!infile) {
    cout << "Error: unable to open input file: " << argv[1] << endl;
    return 1;
  }

  // Create output file
  string output_filename = GetFilename(argv[1]) + ".root";
  TFile *fout = new TFile(output_filename.c_str(), "RECREATE");
  if (!fout->IsOpen()) {
    cout << "Error: unable to create output file: " << output_filename << endl;
    return 1;
  }

  // Output TTree branches
  TTree *tree1 = new TTree("data", "data");
  tree1->Branch("eventHitCount", &eventHitCount, "eventHitCount/I");
  tree1->Branch("adcEnergy", adcEnergy, "adcEnergy[eventHitCount]/I");
  tree1->Branch("Ecal", Ecal, "Ecal[eventHitCount]/D");
  tree1->Branch("hitTime", hitTime, "hitTime[eventHitCount]/L");
  tree1->Branch("channel", channel, "channel[eventHitCount]/I");

  
  // Process input file line by line
  // Skip the first three lines (header information)
  for (int i = 0; i < 3; ++i) {
    getline(infile, line);
  }

  int filecounter = 0;
  // Loop through the input file
  while (getline(infile, line)) {
    filecounter++;
    vect.clear();
    split2(line, vect, ',');

    // Check for valid line format
    if (vect.size() != 6) {
      cerr << "Warning: Invalid line format: " << line << endl;
      continue;
    }

    // Parse line content
    unsigned int time_h = atoi(vect[3].c_str());
    unsigned int time_l = atoi(vect[4].c_str());
    long time_temp = time_h * pow(2, 32) + time_l;

    // Group hits into events
    if (time_temp < (eventMaxTime + hitTime[0])) {
      adcEnergy[eventHitCount] = (int)atoi(vect[5].c_str());
      channel[eventHitCount] = (int)atoi(vect[1].c_str());
      //use the appropriate slope and yintercept for the channel to calibrate
      Ecal[eventHitCount] = adcEnergy[eventHitCount]*slope[channel[eventHitCount]] + yint[channel[eventHitCount]];
      hitTime[eventHitCount] = time_temp;
      eventHitCount++;
    } else {
      tree1->Fill();

      ResetTreeVariables();
      adcEnergy[eventHitCount] = (int)atoi(vect[5].c_str());
      channel[eventHitCount] = (int)atoi(vect[1].c_str());
      Ecal[eventHitCount] = adcEnergy[eventHitCount]*slope[channel[eventHitCount]] + yint[channel[eventHitCount]];
      hitTime[eventHitCount] = time_temp;
      eventHitCount++;
    }
  }

  // Fill the last event
  tree1->Fill();

  // Write, print, and close output file
  tree1->Write();
  tree1->Print();
  fout->Write();
  fout->Close();

  return 0;
}