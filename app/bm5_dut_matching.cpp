#include "./../include/sci_map.h"

#include <iostream>
#include <cmath>

#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>
#include <TCanvas.h>

int main(){
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

    std::vector<std::pair<double, double>> matched_times;

    TGraph* match_graph = new TGraph();
    TGraph* time_diff_graph = new TGraph();

    int clock_count = 0;
    double prev_tH = tH0;

    for(Long64_t iH=0; iH<htree_nentries; iH++){
        htree->GetEntry(iH);
        double tH = timestamps_r[0]*25;
        double dtH = tH - tH0;

        if(tH < prev_tH){
            clock_count++;
        }
        double tH_adjusted = tH + clock_count * std::pow(2, 30); // Adjust for clock cycles
        dtH = tH_adjusted - tH0;
        prev_tH = tH;

        for(Long64_t iC=0; iC<ctree_nentries; iC++){
            ctree->GetEntry(iC);
            double tC = pcnt*50000000 + fcnt*240;
            double dtC = tC - tC0;

            // std::cout << "H2GCROC time: " << dtH << " ns, CITIROC time: " << dtC << " ns" << std::endl;

            if(std::abs(dtH - dtC) < 240){
                std::cout << "Matched: H2GCROC time: " << dtH << " ns, CITIROC time: " << dtC << " ns" << std::endl;
                matched_times.emplace_back(dtH, dtC);
                match_graph->SetPoint(match_graph->GetN(), dtH, dtC);
                time_diff_graph->SetPoint(time_diff_graph->GetN(), dtH, dtH - dtC);
            }
        }

    }

    TCanvas* c1 = new TCanvas("c1", "Matched Times", 1600, 800);
    c1->Divide(2, 1);
    c1->cd(1);
    match_graph->SetTitle("Matched Times;H2GCROC Time [ns];CITIROC Time [ns]");
    match_graph->SetMarkerStyle(20);
    match_graph->Draw("AP");
    c1->cd(2);
    time_diff_graph->SetTitle("Time Differences;H2GCROC Time [ns];Time Difference [ns]");
    time_diff_graph->SetMarkerStyle(20);
    time_diff_graph->Draw("AP");

    c1->SaveAs("/home/motohashi/work/analysis_raris202607/result/time_distro/matched_times.png");

    return 0;
}