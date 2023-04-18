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

// Define external variables
int adcEnergy[maxhits];
long hitTime[maxhits];
int channel[maxhits];
int eventHitCount;

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
void split2(const std::string &str, Container &cont, char delim = ',') {
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
    hitTime[i] = -1;
  }
  eventHitCount = 0;
}

// Main function
int main(int argc, char *argv[]) {
  // Declare variables
  string line;
  vector<string> vect;

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
  TTree *masterTree = new TTree("data", "Master Data Tree");
  masterTree->Branch("eventHitCount", &eventHitCount, "eventHitCount/I");
  masterTree->Branch("adcEnergy", adcEnergy, "adcEnergy[eventHitCount]/I");
  masterTree->Branch("hitTime", hitTime, "hitTime[eventHitCount]/L");
  masterTree->Branch("channel", channel, "channel[eventHitCount]/I");

  TTree *tree0 = new TTree("channel0", "Channel 0 Data Tree");
  tree0->Branch("eventHitCount", &eventHitCount, "eventHitCount/I");
  tree0->Branch("adcEnergy", adcEnergy, "adcEnergy[eventHitCount]/I");
  tree0->Branch("hitTime", hitTime, "hitTime[eventHitCount]/L");
  tree0->Branch("channel", channel, "channel[eventHitCount]/I");

  TTree *tree1 = new TTree("channel1", "Channel 1 Data Tree");
  tree1->Branch("eventHitCount", &eventHitCount, "eventHitCount/I");
  tree1->Branch("adcEnergy", adcEnergy, "adcEnergy[eventHitCount]/I");
  tree1->Branch("hitTime", hitTime, "hitTime[eventHitCount]/L");
  tree1->Branch("channel", channel, "channel[eventHitCount]/I");

  // Process input file line by line
  // Skip the first three lines (header information)
  for (int i = 0; i < 3; ++i) {
    getline(infile, line);
  }


  // Loop through the input file
  while (getline(infile, line)) {
    vect.clear();
    split2(line, vect);

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
    if (time_temp < (eventMaxTime + hitTime[eventHitCount])) {
      adcEnergy[eventHitCount] = atoi(vect[5].c_str());
      channel[eventHitCount] = atoi(vect[1].c_str());
      hitTime[eventHitCount] = time_temp;
      eventHitCount++;
    } else {
      masterTree->Fill();

      if (channel[eventHitCount - 1] == 0) {
        tree0->Fill();
      }

      if (channel[eventHitCount - 1] == 1) {
        tree1->Fill();
      }

      ResetTreeVariables();
      adcEnergy[eventHitCount] = atoi(vect[5].c_str());
      channel[eventHitCount] = atoi(vect[1].c_str());
      hitTime[eventHitCount] = time_temp;
      eventHitCount++;
    }
  }

  // Fill the last event
  masterTree->Fill();

  if (channel[eventHitCount - 1] == 0) {
    tree0->Fill();
  }

  if (channel[eventHitCount - 1] == 1) {
    tree1->Fill();
  }

  // Write, print, and close output file
  fout->cd();
  masterTree->Write();
  masterTree->Print();
  tree0->Write();
  tree0->Print();
  tree1->Write();
  tree1->Print();
  fout->Close();

  // Close input file
  infile.close();

  return 0;
}

