/******************************************************************************
 * File: rootConvertPixieNet.cpp
 * Author(s):
 *      Students: M.S. Wright, B.R. Clark, D.C. Heson
 *      Faculty Advisors: B.P. Crider, J.A. Winger
 * Organization: Mississippi State University, Dept. of Physics and Astronomy
 * Date: April 19, 2023
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
int channelCount[4];

using namespace std;

int main(int argc, char *argv[]);

string GetFilename(string filename);
string GetFileExtension(string filename);
template <class Container>
void split2(const std::string &str, Container &cont, char delim);

void ResetTreeVariables();

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

  TTree *channelTrees[4];
  for (int i = 0; i < 4; i++) {
    string treeName = "channel" + to_string(i);
    string treeTitle = "Channel " + to_string(i) + " Data Tree";
    channelTrees[i] = new TTree(treeName.c_str(), treeTitle.c_str());
    channelTrees[i]->Branch("eventHitCount", &channelCount[i], "eventHitCount/I");
    channelTrees[i]->Branch("adcEnergy", adcEnergy, "adcEnergy[eventHitCount]/I");
    channelTrees[i]->Branch("hitTime", hitTime, "hitTime[eventHitCount]/L");
    channelTrees[i]->Branch("channel", channel, "channel[eventHitCount]/I");
  }

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
      int ch = atoi(vect[1].c_str());
      adcEnergy[eventHitCount] = atoi(vect[5].c_str());
      channel[eventHitCount] = ch;
      hitTime[eventHitCount] = time_temp;
      eventHitCount++;
    } else {
      masterTree->Fill();
      for (int i = 0; i < eventHitCount; i++) {
        int ch = channel[i];
        channelCount[ch] = 1;
        adcEnergy[0] = adcEnergy[i];
        hitTime[0] = hitTime[i];
        channelTrees[ch]->Fill();
        channelCount[ch] = 0;
      }

      ResetTreeVariables();
      int ch = atoi(vect[1].c_str());
      adcEnergy[eventHitCount] = atoi(vect[5].c_str());
      channel[eventHitCount] = ch;
      hitTime[eventHitCount] = time_temp;
      eventHitCount++;
    }
  }

  // Fill the last event
  masterTree->Fill();
  for (int i = 0; i < eventHitCount; i++) {
    int ch = channel[i];
    channelCount[ch] = 1;
    adcEnergy[0] = adcEnergy[i];
    hitTime[0] = hitTime[i];
    channelTrees[ch]->Fill();
    channelCount[ch] = 0;
  }

  // Write, print, and close output file
  fout->cd();
  masterTree->Write();
  masterTree->Print();
  for (int i = 0; i < 4; i++) {
    channelTrees[i]->Write();
    channelTrees[i]->Print();
  }
  fout->Close();

  // Close input file
  infile.close();

  return 0;
}


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
void split2(const std::string &str, Container &cont, char delim) {
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
  for (int i = 0; i < 4; i++) {
    channelCount[i] = 0;
  }
}

