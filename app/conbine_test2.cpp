// ROOT macro

#include "../include/conbine_detecters.h"

#include <iostream>
#include <array>

void conbine_test2(){
    TFile* hfile = TFile::Open("/home/motohashi/work/analysis_raris202607/data/Run340.root", "READ");
    TFile* cfile = TFile::Open("/home/motohashi/work/analysis_raris202607/data/Run0340_HV16_VF448_448_x10_085844.701Monitor5/T5_Sci.root", "READ");
    TTree* htree = (TTree*)hfile->Get("events");
    TTree* ctree = (TTree*)cfile->Get("t");

    H2gcTime h2gc_time;
    CitiTime citi_time;
    
    htree->SetBranchAddress("timestamps_r", &h2gc_time.timestamps_r);
    ctree->SetBranchAddress("pcnt", &citi_time.pcnt);
    ctree->SetBranchAddress("fcnt", &citi_time.fcnt);

    Long64_t htree_entries = htree->GetEntries();
    Long64_t ctree_entries = ctree->GetEntries();

    Long64_t n = std::min(htree_entries, ctree_entries);

    double prevH = 0;
    double prevC = 0;

    for(Long64_t i=0; i<htree_entries; i++){
        htree->GetEntry(i);

        double tH = h2gc_time.timestamps_r[0] * 25; // ns
        double tC = citi_time.pcnt*50000000 + citi_time.fcnt * 240; // ns

        if(i == 0){
            prevH = tH;
            prevC = tC;
        }else{

            double dH = tH - prevH;
            double dC = tC - prevC;

            std::cout<<"dH: "<<dH<<" dC: "<<dC<<std::endl;

            prevH = tH;
            prevC = tC;
        }
    }
}