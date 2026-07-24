#include "./../include/conbine_detecters.h"

#include <iostream>
#include <array>

#include <TFile.h>
#include <TTree.h>

ConbineDetecters::ConbineDetecters() {
    hfile = TFile::Open("/home/motohashi/work/analysis_raris202607/data/Run340.root", "READ");
    cfile = TFile::Open("/home/motohashi/work/analysis_raris202607/data/Run0340_HV16_VF448_448_x10_085844.701Monitor5/T5_Sci.root", "READ");
    htree = (TTree*)hfile->Get("events");
    ctree = (TTree*)cfile->Get("t");

    htree->SetBranchAddress("timestamps_r", &h2gc_time.timestamps_r);
    ctree->SetBranchAddress("pcnt", &citi_time.pcnt);
    ctree->SetBranchAddress("fcnt", &citi_time.fcnt);

    htree_entries = htree->GetEntries();
    ctree_entries = ctree->GetEntries();


}

ConbineDetecters::~ConbineDetecters() {
    hfile->Close();
    cfile->Close();
}

void ConbineDetecters::test(){

    Long64_t iH = 0;
    Long64_t iC = 0;

    double prevH = 0;
    double prevC = 0;

    bool firstH = true;
    bool firstC = true;

    while(iH < htree_entries && iC < ctree_entries){

        htree->GetEntry(iH);
        ctree->GetEntry(iC);

        double tH = h2gc_time.timestamps_r[0] * 25.0; // ns
        double tC = citi_time.fcnt * 240.0;            // ns

        if(firstH){
            prevH = tH;
            firstH = false;
        }
        else{
            std::cout 
                << "H2GCROC: "
                << tH - prevH
                << " ns  ";
            prevH = tH;
        }


        if(firstC){
            prevC = tC;
            firstC = false;
        }
        else{
            std::cout 
                << "CITIROC: "
                << tC - prevC
                << " ns"
                << std::endl;

            prevC = tC;
        }

        // if(tH-prevH == tC-prevC){
        //     std::cout << "match!" << std::endl;
        //     break;
        // }


        iH++;
        iC++;
    }
}