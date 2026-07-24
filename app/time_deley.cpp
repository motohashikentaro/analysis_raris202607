#include <iostream>
#include <array>
#include <cmath>

#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>

void time_deley(){
    TFile* hfile = TFile::Open("/home/motohashi/work/analysis_raris202607/data/Run343.root", "READ");
    TFile* cfile = TFile::Open("/home/motohashi/work/analysis_raris202607/data/Run0343_HV16_VF448_448_x10_090734.761Monitor5/T5_Sci.root", "READ");

    TTree* htree = (TTree*)hfile->Get("events");
    TTree* ctree = (TTree*)cfile->Get("t");

    std::array<UInt_t, 2> timestamps_r;
    int pcnt;
    int fcnt;
    htree->SetBranchAddress("timestamps_r", timestamps_r.data());
    ctree->SetBranchAddress("pcnt", &pcnt);
    ctree->SetBranchAddress("fcnt", &fcnt);

    Long64_t htree_nentries = htree->GetEntries();
    Long64_t ctree_nentries = ctree->GetEntries();

    htree->GetEntry(0);
    double tH0 = timestamps_r[0] * 25.0; // ns
    ctree->GetEntry(0);
    double tC0 = pcnt * 50000000.0 + fcnt * 240.0; // ns

    TGraph* time_graph = new TGraph();

    int clock_count = 0;
    double prev_tH = tH0;

    for(Long64_t i=0; i<htree_nentries; i++){
        htree->GetEntry(i);
        ctree->GetEntry(i);
        double tH = timestamps_r[0] * 25.0; // ns
        double tC = pcnt * 50000000.0 + fcnt * 240.0; // ns
        // time_graph->SetPoint(i, tH-tH0,(tH - tH0)-(tC - tC0));
        if(tH < prev_tH){
            clock_count++;
        }
        double tH_adjusted = tH + clock_count * std::pow(2, 30); // Adjust for clock cycles
        // time_graph->SetPoint(i, tH_adjusted-tH0,(tH_adjusted - tH0)-(tC - tC0));
        // time_graph->SetPoint(i, tH-tH0, tC - tC0);
        time_graph->SetPoint(i, tH_adjusted-tH0, tC - tC0);
        prev_tH = tH;
    }

    // time_graph->Fit("pol1");

    TCanvas* c1 = new TCanvas("c1", "Time Graph", 800, 600);
    time_graph->Draw("AP");
    time_graph->GetXaxis()->SetTitle("tH-tH0 [ns]");
    time_graph->GetYaxis()->SetTitle("Time Difference tC-tC0 [ns]");

    c1->SaveAs("/home/motohashi/work/analysis_raris202607/result/time_correla_adjusted.png");
}