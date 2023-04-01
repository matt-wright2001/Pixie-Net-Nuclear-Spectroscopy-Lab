void plot(){
    TFile *_file0 = TFile::Open("Co60_60s.root");
    TTree *tree = (TTree *)_file0->Get("data");
    //tree->Draw("hitTime[0]>>h1(1e4, 0, 1e10)", "", "");
    tree->Draw("adcEnergy[0]>>h1(4000, 0, 4000)", "", "");



}
