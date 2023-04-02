// GPT
// M.S. Wright, B.R. Clark, D.C. Heson, B.P. Crider, J.A. Winger
// Convert raw XIA Pixie-Net data files into a TTree

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
    hitTime[i] = -1;
  }
}

// Main function
int main(int argc, char *argv[]) {
  // Declare variables
  string line, timeStamp, date;
  string data, junk, description;
  int linenum = 0;
  vector<string> vect;

  // Check input arguments
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " <input_file>" << endl;
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
  tree1->Branch("hitTime", hitTime, "hitTime[eventHitCount]/L"); // Changed from 'time' to 'hitTime'
  tree1->Branch("channel", channel, "channel[eventHitCount]/I");

  // Process input file line by line
  while (infile) {
    getline(infile, line);

    // Skip the first two lines (header information)
    if (linenum < 2) {
      linenum++;
      continue;
    }

    // Parse the line
    vect.clear();
    split2(line, vect, ',');

    channel[0] = atoi(vect[1].c_str());
    adcEnergy[0] = atoi(vect[5].c_str());
    int time_h = atoi(vect[3].c_str());
    int time_l = atoi(vect[4].c_str());
    long time_temp = time_h * pow(2, 32) + time_l;
    hitTime[0] = time_temp;

    // Get new line
    int counter = 0;

    // Group hits into events
    while (infile) {
      getline(infile, line);
      vect.clear();
      split2(line, vect, ',');

      time_h = atoi(vect[3].c_str());
      time_l = atoi(vect[4].c_str());
      long time_temp = time_h * pow(2, 32) + time_l;

      if (time_temp < (eventMaxTime + hitTime[0])) {
        counter++;
        adcEnergy[counter] = atoi(vect[5].c_str());
        channel[counter] = atoi(vect[1].c_str());
        hitTime[counter] = time_temp;
      } else {
        tree1->Fill();
        ResetTreeVariables();
        counter = 0;
        adcEnergy[counter] = atoi(vect[5].c_str());
        channel[counter] = atoi(vect[1].c_str());
        hitTime[counter] = time_temp;
      }
    }
    tree1->Fill();
  }

  // Write, print, and close output file
  tree1->Write();
  tree1->Print();
  fout->Write();
  fout->Close();

  return 0;
}
