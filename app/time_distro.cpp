#include <iostream>
#include <array>
#include <cmath>

#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
int main(){
    TFile* hfile = TFile::Open("/home/motohashi/work/analysis_raris202607/data/Run343.root", "READ");
    TFile* cfile = TFile::Open("/home/motohashi/work/analysis_raris202607/data/Run0343_HV16_VF448_448_x10_090734.761Monitor5/T5_Sci.root", "READ");
    TFile* exfile = TFile::Open("/home/motohashi/work/analysis_raris202607/result/xydata/Run0343_HV16_VF448_448_x10_090734.761Monitor5/T5_xy.root", "READ");
    TTree* htree = (TTree*)hfile->Get("events");
    TTree* ctree = (TTree*)cfile->Get("t");
    TTree* extree = (TTree*)exfile->Get("t");

    std::array<UInt_t, 2> timestamps_r;
    int pcnt;
    int fcnt;
    int pcnt_ex;
    int fcnt_ex;

    htree->SetBranchAddress("timestamps_r", timestamps_r.data());
    ctree->SetBranchAddress("pcnt", &pcnt);
    ctree->SetBranchAddress("fcnt", &fcnt);
    extree->SetBranchAddress("pcnt", &pcnt_ex);
    extree->SetBranchAddress("fcnt", &fcnt_ex);

    Long64_t htree_nentries = htree->GetEntries();
    Long64_t ctree_nentries = ctree->GetEntries();
    Long64_t extree_nentries = extree->GetEntries();

    htree->GetEntry(0);
    double tH0 = timestamps_r[0] * 25.0; // ns
    ctree->GetEntry(0);
    double tC0 = pcnt * 50000000.0 + fcnt * 240.0; // ns
    extree->GetEntry(0);
    double tEx0 = pcnt_ex * 50000000.0 + fcnt_ex * 240.0; // ns


    int clock_count = 0;
    double prev_tH = tH0;
    TH1D* time_distro_H = new TH1D("time_distro", "Time Distribution of H2GCROC;Time [ns];Counts", 25, 0, 10000000000);
    for(Long64_t i=0; i<htree_nentries; i++){
        htree->GetEntry(i);
        double tH = timestamps_r[0] * 25.0; // ns
        if(tH < prev_tH){
            clock_count++;
        }
        double tH_adjusted = tH + clock_count * std::pow(2, 30);
        time_distro_H->Fill(tH_adjusted - tH0);
        if(i<10) std::cout << "H2GCROC: " << tH << " ns" << std::endl;
    }

    TH1D* time_distro_C = new TH1D("time_distro_C", "Time Distribution of CITIROC;Time [ns];Counts", 250, 0, 100000000000);
    for(Long64_t i=0; i<ctree_nentries; i++){
        ctree->GetEntry(i);
        double tC = pcnt * 50000000.0 + fcnt * 240.0; // ns
        time_distro_C->Fill(tC - tC0);
        if(i<10) std::cout << "CITIROC: " << tC << " ns" << std::endl;
    }

    TH1D* time_distro_ex = new TH1D("time_distro_ex", "Time Distribution of External;Time [ns];Counts", 250, 0, 100000000000);
    for(Long64_t i=0; i<extree_nentries; i++){
        extree->GetEntry(i);
        double tEx = pcnt_ex * 50000000.0 + fcnt_ex * 240.0; // ns
        time_distro_ex->Fill(tEx - tEx0);
        if(i<10) std::cout << "External: " << tEx << " ns" << std::endl;
    }


    TCanvas* c1 = new TCanvas("c1", "Time Distribution", 800, 600);
    TCanvas* c2 = new TCanvas("c2", "Time Distribution", 800, 600);
    TCanvas* c3 = new TCanvas("c3", "Time Distribution", 800, 600);
    gStyle->SetOptStat(0);
    time_distro_H->SetLineColor(kRed);
    time_distro_C->SetLineColor(kBlue);
    time_distro_ex->SetLineColor(kGreen);
    c1->cd();
    time_distro_H->Draw();
    c2->cd();
    time_distro_C->Draw();
    c3->cd();
    time_distro_H->Draw();
    time_distro_C->Draw("SAME");
    time_distro_ex->Draw("SAME");
    TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);
    legend->AddEntry(time_distro_H, "H2GCROC", "l");
    legend->AddEntry(time_distro_C, "CITIROC", "l");
    legend->AddEntry(time_distro_ex, "External", "l");
    legend->Draw();
    c1->Update();
    c2->Update();
    c3->Update();
    c1->SaveAs("./../result/time_distribution_H2GCROC.png");
    c2->SaveAs("./../result/time_distribution_CITIROC.png");
    c3->SaveAs("./../result/time_distribution_combined.png");
}