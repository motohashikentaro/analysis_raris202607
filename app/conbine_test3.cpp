
#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TAxis.h>

#include <array>
#include <iostream>
#include <cmath>
#include <limits>

struct H2gcTime{
    std::array<UInt_t, 2> timestamps_r;
};

struct CitiTime{
    int pcnt;
    int fcnt;
};

void conbine_test3(){

    TFile* hfile = TFile::Open("/home/motohashi/work/analysis_raris202607/data/Run340.root");
    TFile* cfile = TFile::Open("/home/motohashi/work/analysis_raris202607/data/Run0340_HV16_VF448_448_x10_085844.701Monitor5/T5_Sci.root");

    TTree* htree = (TTree*)hfile->Get("events");
    TTree* ctree = (TTree*)cfile->Get("t");

    H2gcTime h2gc_time;
    CitiTime citi_time;

    htree->SetBranchAddress("timestamps_r", &h2gc_time.timestamps_r);
    ctree->SetBranchAddress("pcnt", &citi_time.pcnt);
    ctree->SetBranchAddress("fcnt", &citi_time.fcnt);

    Long64_t htree_entries = htree->GetEntries();
    Long64_t ctree_entries = ctree->GetEntries();

    int sync_point_H = -1;
    int sync_point_C = -1;

    for(Long64_t iH = 0; iH < htree_entries - 1; iH++){

        htree->GetEntry(iH);
        double tH0 = h2gc_time.timestamps_r[0] * 25;

        htree->GetEntry(iH + 1);
        double tH1 = h2gc_time.timestamps_r[0] * 25;

        double dH = tH1 - tH0;

        for(Long64_t iC=0; iC<ctree_entries-1; iC++){
            ctree->GetEntry(iC);
            double tC0 = citi_time.pcnt * 50000000 + citi_time.fcnt * 240;

            ctree->GetEntry(iC + 1);
            double tC1 = citi_time.pcnt * 50000000 + citi_time.fcnt * 240;

            if(dH == tC1 - tC0){
                std::cout << "Sync point found!" << std::endl;
                std::cout << "H event : " << iH << " -> " << iH+1 << std::endl;
                std::cout << "C event : " << iC << " -> " << iC+1 << std::endl;

                sync_point_H = iH;
                sync_point_C = iC;

                break;
            }
        }
        if(sync_point_H != -1 && sync_point_C != -1){
            break;
        }
    }

    TGraph* graph1 = new TGraph();
    TGraph* graph2 = new TGraph();

    if(sync_point_H != -1 && sync_point_C != -1){
        htree->GetEntry(sync_point_H);
        double tH0 = h2gc_time.timestamps_r[0] * 25;

        htree->GetEntry(sync_point_H + 1);
        double tH_next = h2gc_time.timestamps_r[0] * 25;

        double dHnext = tH_next - tH0;

        htree->GetEntry(sync_point_H + 50);
        double tH_50 = h2gc_time.timestamps_r[0] * 25;

        double dH50 = tH_50 - tH0;

        for(Long64_t i=sync_point_C - 100; i<sync_point_C + 100; i++){
            ctree->GetEntry(sync_point_C);
            double tC0 = citi_time.pcnt * 50000000 + citi_time.fcnt * 240;

            ctree->GetEntry(i + 1);
            double tC_next = citi_time.pcnt * 50000000 + citi_time.fcnt * 240;

            double dC = tC_next - tC;

            ctree->GetEntry(i + 50);
            double tC_50 = citi_time.pcnt * 50000000 + citi_time.fcnt * 240;

            double dC50 = tC_50 - tC;

            double dt = dC - dHnext;
            double d50 = dC50 - dH50;

            graph1->SetPoint(graph1->GetN(), i-sync_point_C, dt);
            graph2->SetPoint(graph2->GetN(), i-sync_point_C, d50);
            // graph1->SetMarkerColor(kBlue);
            // graph2->SetMarkerColor(kBlue);
            // if(dt < 240){
            //     graph1->SetMarkerColor(kRed);
            // }
            // if(d50 < 240){
            //     graph2->SetMarkerColor(kRed);
            // }
            // if(dt == 0){
            //     graph1->SetMarkerColor(kGreen);
            // }
            // if(d50 == 0){
            //     graph2->SetMarkerColor(kGreen);
            // }
        }
    }

    TCanvas* c1 = new TCanvas("c1", "c1", 800, 600);
    c1->Divide(2, 1);
    c1->cd(1);
    graph1->SetMarkerStyle(20);
    graph1->SetMarkerSize(1.0);
    graph1->SetTitle("delta Time (CITIROC - H2GCROC) for Next Event;Event Index;Time Difference [ns]");
    graph1->Draw("AP");
    c1->cd(2);
    graph2->SetMarkerStyle(20);
    graph2->SetMarkerSize(1.0);
    graph2->SetTitle("delta Time (CITIROC - H2GCROC) for 50 Events;Event Index;Time Difference [ns]");
    graph2->Draw("AP");
}

    

