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
    eventHitCount = 0;
  }
}

// Main function
int main(int argc, char *argv[]) {
  // Declare variables
  string line, timeStamp, date;
  string data, junk, description;
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

  int counter_cout = 0; //counter for printing out stuff
  int cout_lines = 20;

  // Process input file line by line  
  // Skip the first three lines (header information)
  getline(infile, line);
  getline(infile, line);
  getline(infile, line);
  
  // Get the first line. Parse the line
  getline(infile, line);
  vect.clear();
  split2(line, vect, ',');
  cout << "first line = " << line << endl;
  
  channel[0] = atoi(vect[1].c_str());
  adcEnergy[0] = atoi(vect[5].c_str());
  unsigned int time_h = atoi(vect[3].c_str());
  unsigned int time_l = atoi(vect[4].c_str());
  long time_temp = time_h * pow(2, 32) + time_l;
  hitTime[0] = time_temp;

  // Get new line
  //int counter = 0;
  eventHitCount++;

  // Group hits into events
  while (infile) {
    //cout << line << endl;
    counter_cout++;
    vect.clear();
    split2(line, vect, ',');

    time_h = atoi(vect[3].c_str());
    time_l = atoi(vect[4].c_str());
    long time_temp = time_h * pow(2, 32) + time_l;
    if(counter_cout < cout_lines){
      cout << "time_temp = " << time_temp << ", time_h = " << time_h << ", time_l = " << time_l << endl;

      cout << "eventMaxTime = " << eventMaxTime << ", hitTime[0] = " << hitTime[0] << endl;
    }

    if (time_temp < (eventMaxTime + hitTime[0])) {
      if(counter_cout < cout_lines){
	cout << "in if..." << endl;
      }
      eventHitCount++;
      adcEnergy[eventHitCount] = atoi(vect[5].c_str());
      channel[eventHitCount] = atoi(vect[1].c_str());
      hitTime[eventHitCount] = time_temp;
      
    }else{
      if(counter_cout < cout_lines){
	cout << "in else..." << endl;
      }
      tree1->Fill();
      //seeing what we have in the variables
      for(int i = 0; i < eventHitCount; i++){
	if(counter_cout < cout_lines){
	  cout << "adcEnergy[" << i << "] = " << adcEnergy[i] << " ";
	  cout << endl;
	  cout << "channel[" << i << "] = " << channel[i] << ", ";
	  cout << endl;
	  cout << "hitTime[" << i << "] = " << hitTime[i] << ", ";
	  cout << endl;
	  cout << "eventHitCount = " << eventHitCount;
	  cout << endl;
	}
      }
      ResetTreeVariables();
      adcEnergy[eventHitCount] = atoi(vect[5].c_str());
      channel[eventHitCount] = atoi(vect[1].c_str());
      hitTime[eventHitCount] = time_temp;
      eventHitCount++;
    }
    
    getline(infile, line);
    if(counter_cout < cout_lines){
      cout << line << endl;
    }
  
  }
  //fill the last line
  tree1->Fill();

  // Write, print, and close output file
  tree1->Write();
  tree1->Print();
  fout->Write();
  fout->Close();
  
  return 0;
}

