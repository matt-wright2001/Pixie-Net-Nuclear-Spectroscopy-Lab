#include "rootConvertPixieNet.h"

using namespace std;

//for stripping path and extension off of filename
string GetFilename(string filename){
  // Remove directory if present.
  // Do this before extension removal in case directory has a period character.
  const size_t last_slash_idx = filename.find_last_of("\\/");
  if (std::string::npos != last_slash_idx){
    filename.erase(0, last_slash_idx + 1);
  }
  
  // Remove extension if present.
  const size_t period_idx = filename.rfind('.');
  if (std::string::npos != period_idx){
    filename.erase(period_idx);
  }
  return filename;
}

// Get extension for determining the file extension only 
// Discriminate between binary and ASCII files
string GetFileExtension(string filename){
  // Remove directory if present.
  // Do this before extension removal in case directory has a period character.
  const size_t last_slash_idx = filename.find_last_of("\\/");
  if (std::string::npos != last_slash_idx){
    filename.erase(0, last_slash_idx + 1);
  }
  
  // Get extension if present.
  const size_t period_idx = filename.rfind('.');
  std::string extension = filename.substr(period_idx,std::string::npos);
  return extension;
}

// Read strings from input file
template <class Container>
void split2(const std::string& str, Container& cont, char delim = '\t')
{
  std::stringstream ss(str);
  std::string token;
  while (std::getline(ss, token, delim)) 
    {
      cont.push_back(token);
    }
}

// Reset variables defined in rootConvertPixieNet.h
void ResetTreeVariables(){
	for(int i = 0; i < maxhits; i++){
		channel[i] = -1;	
		adcEnergy[i] = -1;
		time[i] = -1;
	}
}
// ... (the rest of the script remains the same)

// Loop through dumped data
int main(int argc, char *argv[]) {
  // Declare variables
  string line, timeStamp, date;
  string data, junk, description;
  int linenum = 0;
  vector<string> vect;

  // ... (the rest of the script remains the same)

  // Output TTree branches
  // All variables declared in the header need to be added here if they are to be written to the output root file
  TTree *tree1 = new TTree("data","data");
  tree1->Branch("eventHitCount",&eventHitCount,"eventHitCount/I");
  tree1->Branch("adcEnergy",&adcEnergy,"adcEnergy[eventHitCount]/I");
  tree1->Branch("time",time,"time[eventHitCount]/L");
  tree1->Branch("channel",&channel,"channel[eventHitCount]/I");

  // ... (the rest of the script remains the same)

  //parse the line
  channel[0] = atoi(vect[1].c_str());
  adcEnergy[0] = atoi(vect[5].c_str());
  int time_h = atoi(vect[3].c_str());
  int time_l = atoi(vect[4].c_str());
  long time_temp = time_h * pow(2, 32) + time_l;
  time[0] = time_temp;

   //get new line
      int counter = 0;
	
      // Group hits into events
      while(infile){
	      getline(infile,line);
	      vect.clear();
      	split2(line,vect,',');

      	time_h = atoi(vect[3].c_str());
      	time_l = atoi(vect[4].c_str());
	      long time_temp = time_h * pow(2,32) + time_l;
	
	      if(time_temp < (eventMaxTime + time[0])){
	        counter++;
	        adcEnergy[counter] = atoi(vect[5].c_str());
	        channel[counter] = atoi(vect[1].c_str());
	        time[counter] = time_temp;
	      } else{
	          tree1->Fill();
	          ResetTreeVariables();
  	        counter=0;
      	    adcEnergy[counter] = atoi(vect[5].c_str());
	          channel[counter] = atoi(vect[1].c_str());
	         time[counter] = time_temp;
	        }
        }
      tree1->Fill();
    }
  }
  
  tree1->Write();
  tree1->Print();
  
  fout->Write();
  fout->Close();
  
  return 0;
  
}

                                               
