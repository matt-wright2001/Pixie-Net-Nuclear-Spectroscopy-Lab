#include "TH1.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TF1.h"
#include "TPolyMarker.h"
#include "TRandom.h"
#include "TFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TExec.h"
#include "TPaveText.h"

#include <vector>
#include "time.h"
#include <iostream>
#include <fstream>
#include <sstream>

//run number for Clover_Energy_Spectra_Run_#.root file
int clover_runNum = 1240;

//define global things
TH1D *h1;
TCanvas *c1;
vector<double> Peak_Locations;
int x_up;
int x_down;

//Counters
int counter;
int marker_counter=0;
int fit_counter=0;

//Makers
TPolyMarker *pm[100];

//Fit Parameters
double Sigma[100];
double Sigma_Error[100];
double Mean[100];
double Mean_Error[100];
double Height[100];
double Height_Error[100];
double Area[100];
double Area_Error[100];
double Resolution[100];
double Resolution_Error[100];
double Efficiency[100];
double Efficiency_Error[100];
double Slope;
double Slope_Error;
double Intercept;
double Intercept_Error;

//Turn on and off testing
bool Program_Test=false;
bool MCA_Data=false;

//Safeguard against saving under stupidity or without results
bool Dont_Save=true;

//Time stuff;
struct tm source_date;
struct tm run_date;

//Source Stuff
int N_Components;
double Half_Lives[100];  //in days

int N_Lines;
double Energy[100]; //in keV
double Energy_Unc[100]; //in keV
double CPS[100]; //CPS of the source
double CPS_Unc[100]; //in %
int Source_Comp[100]; 

double CPS_Decay_Corr[100]; // Decay corrected source intensities (CPS)
double CPS_Error_Decay_Corr[100]; //Decay corrected source intensity errors (CPS)
double Decay_Corr[100]; //factor to multiply by for decay corr

double Counts_Expected[100];
double Counts_Expected_Error[100];

//Run Stuff
double Run_Length; //in minutes
double Live_Time; //percentage

//Fit Functions
TF1 *f1[100];
TF1 *f2[100];

//Executive Function
void Position_Selector() {
  TObject *selected = gPad->GetSelected();
  if(!selected) return;
  if (!selected->InheritsFrom(TAxis::Class())) {
    gPad->SetUniqueID(0);
    return;
    
  }
  gPad->GetCanvas()->FeedbackMode(kTRUE);
  
  int event = gPad->GetEvent();
  int eventx = gPad->GetEventX();

  //cout << "event = " << event <<", eventx = " << eventx <<endl;
  
  if(event ==1) {
    x_down = eventx;
  }
  if(event == 11) {
    x_up = eventx;
  }

  if(event == 11 && (x_up == x_down)) {
   
    double px =0;
    double py =0;
    //look before and after the peak for the height (only for asthetics)
    //for(int k= gPad->PixeltoX(eventx)-25; k<gPad->PixeltoX(eventx)+25; k++) {
    for(int k= gPad->PixeltoX(eventx)-30; k<gPad->PixeltoX(eventx)+30; k++) {
      if(h1->GetBinContent(k) > py) {
	py = h1->GetBinContent(k);
	px = k;
      }
    }
    py = 1.03*py;
    pm[marker_counter] = new TPolyMarker(1,&px,&py);
    pm[marker_counter]->SetMarkerStyle(20);
    pm[marker_counter]->SetMarkerColor(2);
    pm[marker_counter]->Draw();
    gPad->Update();

    //  Peak_Locations.push_back(gPad->PixeltoX(eventx));
    Peak_Locations.push_back(px);
    cout<< marker_counter <<"  "<<px<<endl;
    marker_counter++;
    
  }
  
  //if you press "s" you can view the peaks selected
  if(event == 24 && eventx == 115) {
    
    if(Dont_Save) {
      cout<<"I refuse to save! Check the efficency and energy calibration histograms"<<endl;
    }
    else {
      printf("Saving to ''new_results_upstream/Clover_%d_Results.txt''",counter);
      stringstream ss;
      ss.str();
      string s1 = "new_results_upstream/Clover_";
      string s2 = "_Results.txt";
      
      ss<<s1<<counter<<s2;
      ofstream out;
      out.open(ss.str().c_str());
      out<<Slope<<"  "<<Slope_Error<<"\n";
      out<<Intercept<<"  "<<Intercept_Error<<"\n";
      
      for(int n =0; n<N_Lines; n++) {
	out<<Energy[n]<<"   "<<Mean[n]<<"  "<<Mean_Error[n]<<"   "<<Area[n]<<"  "<<Area_Error[n]<<"   "<<Efficiency[n]<<"  "<<Efficiency_Error[n]<<"\n";
      }
      
      cout<<endl;
      for(int n = 0; n<Peak_Locations.size(); n++) {
	cout<<n<<"  "<<Peak_Locations[n]<<endl;
      }
    }
  }

  //if you press "f" you can fit all the peaks selected
  if(event == 24 && eventx == 102) {
    fit_counter=0;

    if(Peak_Locations.size() != N_Lines) {
      cout<<"Warning!!!!"<<endl;
      cout<<"The number of peaks selected from the spectrum doesn't match the source definition!"<<endl;
      cout<<"The results will be worthless.  This will not Save!"<<endl;
      Dont_Save=true;
    }
    else {
      Dont_Save=false;
    }
    
    
   
    //bubble sort so they in order of lowest to highest
    for(int m=0; m<Peak_Locations.size(); m++) {
      for(int n = 0; n<Peak_Locations.size()-1; n++) {
	if(Peak_Locations[n+1]<Peak_Locations[n]) {
	  cout<<"Ordering the locations appropriately"<<endl;
	  double temp = Peak_Locations[n];
	  Peak_Locations[n]=Peak_Locations[n+1];
	  Peak_Locations[n+1]=temp;
	}
      }
    }
    
    //Fit the histogram

    h1->GetXaxis()->SetRangeUser(Peak_Locations[0]-200, Peak_Locations[Peak_Locations.size()-1]+200);
    gPad->Modified();
    gPad->Update();
    
    //Make energy calibrations
    TGraph *gr1 = new TGraphErrors(N_Lines, Mean, Energy, Mean_Error, Energy_Unc);
    gr1->SetName("En_Cal");
    gr1->SetTitle("Energy vs Channel");
    gr1->GetXaxis()->SetTitle("Channels (Arb. Units)");
    gr1->GetYaxis()->SetTitle("Energy (keV)");
    gr1->SetMarkerStyle(20);
    
    TCanvas *c2 = new TCanvas("Results","Results",640,480);
    c2->Divide(3,1);
    
    c2->cd(1);
    gr1->Draw("AP");
    //TF1 *en_fit = new TF1("en_fit","pol1(0)",(Peak_Locations[3]),(Peak_Locations[Peak_Locations.size()-1]+200));

     TF1 *en_fit = new TF1("en_fit","pol1(0)",(Peak_Locations[0]-200),(Peak_Locations[Peak_Locations.size()-1]+200));
    en_fit->SetParameter(1,0.3);
    en_fit->SetParameter(0,0.2);
    gr1->Fit("en_fit","RMLQ");
    gr1->Fit("en_fit","RMLQ");
    gr1->Fit("en_fit","RMLQ");

    Slope = en_fit->GetParameter(1);
    Intercept = en_fit->GetParameter(0);

    Slope_Error = en_fit->GetParError(1);
    Intercept_Error = en_fit->GetParError(0);

    cout<<"Energy Calibration"<<endl;
    cout<<"Slope: "<<Slope<<" +/- "<<Slope_Error<<endl;
    cout<<"Intercept: "<<Intercept<<" +/- "<<Intercept_Error<<endl;
    
    c2->cd(1);
    TPaveText *pten = new TPaveText(200,1400,3500,1600);
    pten->SetFillColor(0);
    pten->SetTextAlign(12);
    pten->AddText(Form("Slope: %3.5f +/- %3.5f",Slope, Slope_Error));
    pten->AddText(Form("Intercept: %3.5f +/- %3.5f",Intercept, Intercept_Error));
    pten->Draw("same");
    c2->Modified();
    c2->Update();
    
   

    TPaveText *ptres = new TPaveText(400,1,1600,5);
    ptres->SetFillColor(0);
    ptres->SetTextAlign(32);
    ptres->AddText("Energy (keV)    Resolution (%)");
    // for(i=3; i<N_Lines; i++) {
    for(int i=0; i<N_Lines; i++) {
      ptres->AddText(Form("%4.1f     %3.4f +/- %3.4f",Energy[i],Resolution[i],Resolution_Error[i]));      
    }
    ptres->Draw("same");
    c2->Modified();
    c2->Update();
    
    c2->Modified();
    c2->Update();      

  }

  //if you press "v" you can view the peaks selected
  if(event == 24 && eventx == 118) {
    
    //Bubble Sort
    for(int m=0; m<Peak_Locations.size(); m++) {
      for(int n = 0; n<Peak_Locations.size()-1; n++) {
	if(Peak_Locations[n+1]<Peak_Locations[n]) {
	  double temp = Peak_Locations[n];
	  Peak_Locations[n]=Peak_Locations[n+1];
	  Peak_Locations[n+1]=temp;
	}
      }
    }
    
    cout<<"In the terminal press ''n'' for next or ''q'' quit"<<endl;
    for(int n = 0; n<Peak_Locations.size()+1; n++) {
      
      if(n<Peak_Locations.size()) {
	h1->GetXaxis()->SetRangeUser(Peak_Locations[n]-200, Peak_Locations[n]+200);
	gPad->Modified();
	gPad->Update();
	
	cout<<"Peak Number: "<<n<<" Located at: "<<Peak_Locations[n]<<endl;
	
	string text;
	cin>>text;
	
	if(text=="n") continue;
	if(text=="q") {
	  h1->GetXaxis()->SetRangeUser(Peak_Locations[0]-200, Peak_Locations[Peak_Locations.size()-1]+200);
	  gPad->Modified();
	  gPad->Update();
	  return;
	}      
      }
      else {
	h1->GetXaxis()->SetRangeUser(Peak_Locations[0]-200, Peak_Locations[Peak_Locations.size()-1]+200);
	gPad->Modified();
	gPad->Update();
	return;
      }
    }
  }
  
  //pressing "r" removes the last peak
  if(event == 24 && eventx == 114) {
    if(Peak_Locations.size() > 0) {
      cout<<"Removing Last Point"<<endl;
      Peak_Locations.pop_back();
      
      marker_counter--;
      cout<<"Marker "<<marker_counter<<" delete" <<endl;
      TList *li = gPad->GetListOfPrimitives();
      li->Remove(pm[marker_counter]);
      //   pm[marker_counter]->Delete();
      gPad->Modified();
      gPad->Update();
    
    }
    else {
      cout<<"Cant remove points because there is nothing selected!"<<endl;
    }
  }
}

void autoaim() {
  
  cout<<"What Clover do you want to analyze"<<endl;
  cin>>counter;
  cout<<"Detector Number: "<<counter<<" Will be Analyzed"<<endl;
  TExec *ex1;
  
  
  if(Program_Test) {
     h1 = new TH1D(Form("Clovers/Raw/Clover_Eraw_%d",counter), Form("Clovers/Raw/Clover_Eraw_%d",counter), 31768, 0, 31768);
    //Make a fake histogram for testing purposes
    
    for(int j=0; j<32768; j++) {
      //  h1->Fill(j,gRandom->Uniform(1.0,5.0));
    }
    for(int j=0; j<2000; j++) {
      h1->Fill(gRandom->Gaus(1500.0, 5.0));
    }
    for(int j=0; j<1000; j++) {
      h1->Fill(gRandom->Gaus(5500.0,5.0));
    }
    for(int j=0; j<1500; j++) {
      h1->Fill(gRandom->Gaus(10000.0, 5.0));
    }
    for(int j=0; j<100000; j++) {
      h1->Fill(gRandom->Exp(2000.0));
    }
  }
  else {
    if(MCA_Data) {
      h1 = new TH1D(Form("Clovers/Raw/Clover_Eraw_%d",counter), Form("Clovers/Raw/Clover_Eraw_%d",counter), 8192, 0, 8192);
      
      ifstream mca_data;
      mca_data.open("MCA_Clover_9_Ch1.txt");
      int temp;
      for(int i=0; i<8192; i++) {
	mca_data>>temp;
	h1->Fill(i,temp);
      }      
    }

    else{
      //Get the histogram from the file
      TFile *fin = new TFile(Form("run_%d_gamma_spectra_rebinned.root", clover_runNum));
      h1 = (TH1D*)fin->Get(Form("Clovers/Raw/Clover_Eraw_%d",counter));
    }
  }

  //Draw the histogram;  
  c1 = new TCanvas("spec","Spectrum",1024,768);      
  //cout<<"Made Canvas "<<h1<<endl;
  // c1->cd();
  h1->Draw("hist");
  // cout<<"Draw Hist "<<h1<<endl;

  h1->GetXaxis()->SetTitle("Energy (Channels)");
  h1->GetYaxis()->SetTitle("Counts / Channel");
  gPad->AddExec(Form("Exec_%d",counter),"Position_Selector()");
  
  
}
