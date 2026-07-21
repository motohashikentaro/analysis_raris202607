#include "./../include/bm_hit_matching.h"

#include <iostream>
#include <algorithm>

#include <TFile.h>
#include <TTree.h>

void HitMatch::BranchExtract(){

    this->f3 = TFile::Open("/home/motohashi/work/analysis_raris202607/data/Run0226_HV16_VF360_360_x10_HVScan_46V_124637.627Monitor5/T3_Sci.root");
    this->f4 = TFile::Open("/home/motohashi/work/analysis_raris202607/data/Run0226_HV16_VF360_360_x10_HVScan_46V_124637.627Monitor5/T4_Sci.root");
    this->f5 = TFile::Open("/home/motohashi/work/analysis_raris202607/data/Run0226_HV16_VF360_360_x10_HVScan_46V_124637.627Monitor5/T5_Sci.root");

    this->t3 = (TTree*)f3->Get("t");
    this->t4 = (TTree*)f4->Get("t");
    this->t5 = (TTree*)f5->Get("t");

    t3->SetBranchAddress("pcnt", &this->bm3.id.pcnt);
    t3->SetBranchAddress("fcnt", &this->bm3.id.fcnt);
    t3->SetBranchAddress("nHits", &this->bm3.nhits);
    t3->SetBranchAddress("channel", &this->bm3.channel);
    t3->SetBranchAddress("ADC", &this->bm3.adc);
    t3->SetBranchAddress("ADCmax", &this->bm3.adc_max);
    t3->SetBranchAddress("iX", &this->bm3.ix);
    t3->SetBranchAddress("iY", &this->bm3.iy);

    t4->SetBranchAddress("pcnt", &this->bm4.id.pcnt);
    t4->SetBranchAddress("fcnt", &this->bm4.id.fcnt);
    t4->SetBranchAddress("nHits", &this->bm4.nhits);
    t4->SetBranchAddress("channel", &this->bm4.channel);
    t4->SetBranchAddress("ADC", &this->bm4.adc);
    t4->SetBranchAddress("ADCmax", &this->bm4.adc_max);
    t4->SetBranchAddress("iX", &this->bm4.ix);
    t4->SetBranchAddress("iY", &this->bm4.iy);

    t5->SetBranchAddress("pcnt", &this->bm5.id.pcnt);
    t5->SetBranchAddress("fcnt", &this->bm5.id.fcnt);
    t5->SetBranchAddress("nHits", &this->bm5.nhits);
    t5->SetBranchAddress("channel", &this->bm5.channel);
    t5->SetBranchAddress("ADC", &this->bm5.adc);
    t5->SetBranchAddress("ADCmax", &this->bm5.adc_max);
    t5->SetBranchAddress("iX", &this->bm5.ix);
    t5->SetBranchAddress("iY", &this->bm5.iy);

    nentries3 = t3->GetEntries();
    nentries4 = t4->GetEntries();
    nentries5 = t5->GetEntries();

    std::cout << "branch extract successfully" << std::endl;

}

void HitMatch::CreateMatchFile(){
    this->match_file = TFile::Open("/home/motohashi/work/analysis_raris202607/data/match_run226.root", "RECREATE");
    this->match_tree = new TTree("match", "Matched Hits");

    match_tree->Branch("pcnt", &this->bm3.id.pcnt, "pcnt/I");
    match_tree->Branch("fcnt", &this->bm3.id.fcnt, "fcnt/I");
    match_tree->Branch("nHits3", &this->bm3.nhits, "nHits3/I");
    match_tree->Branch("channel3", &this->bm3.channel);
    match_tree->Branch("ADC3", &this->bm3.adc);
    match_tree->Branch("ADCmax3", &this->bm3.adc_max, "ADCmax3/I");
    match_tree->Branch("iX3", &this->bm3.ix);
    match_tree->Branch("iY3", &this->bm3.iy);
    match_tree->Branch("nHits4", &this->bm4.nhits, "nHits4/I");
    match_tree->Branch("channel4", &this->bm4.channel);
    match_tree->Branch("ADC4", &this->bm4.adc);
    match_tree->Branch("ADCmax4", &this->bm4.adc_max, "ADCmax4/I");
    match_tree->Branch("iX4", &this->bm4.ix);
    match_tree->Branch("iY4", &this->bm4.iy);
    match_tree->Branch("nHits5", &this->bm5.nhits, "nHits5/I");
    match_tree->Branch("channel5", &this->bm5.channel);
    match_tree->Branch("ADC5", &this->bm5.adc);
    match_tree->Branch("ADCmax5", &this->bm5.adc_max, "ADCmax5/I");
    match_tree->Branch("iX5", &this->bm5.ix);
    match_tree->Branch("iY5", &this->bm5.iy);

    std::cout << "make file & branch successfully" << std::endl;
}

// bool HitMatch::IsMatch()
// {
//     return
//     (bm3.id.pcnt == bm4.id.pcnt) &&
//     (bm3.id.pcnt == bm5.id.pcnt) &&
//     (bm3.id.fcnt == bm4.id.fcnt) &&
//     (bm3.id.fcnt == bm5.id.fcnt);
// }
// bool HitMatch::IsMatch()
// {
//     const int fcnt_buffer = 1000;

//     return
//         (bm3.id.pcnt == bm4.id.pcnt) &&
//         (bm3.id.pcnt == bm5.id.pcnt) &&
//         (std::abs(bm3.id.fcnt - bm4.id.fcnt) <= fcnt_buffer) &&
//         (std::abs(bm3.id.fcnt - bm5.id.fcnt) <= fcnt_buffer);
// }
bool HitMatch::IsMatch()
{
    return
    (bm3.id.pcnt == bm4.id.pcnt) &&
    (bm3.id.pcnt == bm5.id.pcnt);
}

// bool HitMatch::IsLess(const HitId& a, const HitId& b)
// {
//     if(a.pcnt != b.pcnt){
//         return a.pcnt < b.pcnt;
//     }

//     return a.fcnt < b.fcnt;
// }
// bool HitMatch::IsLess(const HitId& a, const HitId& b)
// {
//     if(a.pcnt != b.pcnt){
//         return a.pcnt < b.pcnt;
//     }

//     return a.fcnt < b.fcnt;
// }
bool HitMatch::IsLess(const HitId& a, const HitId& b)
{
    return a.pcnt < b.pcnt;
}

void HitMatch::HitMatching(){
    Long64_t i3 = 0;
    Long64_t i4 = 0;
    Long64_t i5 = 0;

    t3->GetEntry(i3);
    t4->GetEntry(i4);
    t5->GetEntry(i5);

    // while(i3 < nentries3 && i4 < nentries4 && i5 < nentries5){
    //     if(IsMatch()){
    //         match_tree->Fill();
    //         i3++;
    //         i4++;
    //         i5++;
    //         if(i3 < nentries3) t3->GetEntry(i3);
    //         if(i4 < nentries4) t4->GetEntry(i4);
    //         if(i5 < nentries5) t5->GetEntry(i5);
    //     }else{
    //         if(IsLess(bm3.id,bm4.id) &&
    //            IsLess(bm3.id,bm5.id)){

    //             i3++;
    //             t3->GetEntry(i3);

    //         }
    //         else if(IsLess(bm4.id,bm3.id) &&
    //                 IsLess(bm4.id,bm5.id)){

    //             i4++;
    //             t4->GetEntry(i4);

    //         }
    //         else{

    //             i5++;
    //             t5->GetEntry(i5);

    //         }
    //     }
    // }
    while(i3 < nentries3 &&
          i4 < nentries4 &&
          i5 < nentries5){


        int p3 = bm3.id.pcnt;
        int p4 = bm4.id.pcnt;
        int p5 = bm5.id.pcnt;


        if(p3 == p4 && p3 == p5){

            match_tree->Fill();

            i3++;
            i4++;
            i5++;

            t3->GetEntry(i3);
            t4->GetEntry(i4);
            t5->GetEntry(i5);
        }
        else{

            int min_pcnt = std::min({p3,p4,p5});


            if(p3 == min_pcnt){
                i3++;
                t3->GetEntry(i3);
            }

            if(p4 == min_pcnt){
                i4++;
                t4->GetEntry(i4);
            }

            if(p5 == min_pcnt){
                i5++;
                t5->GetEntry(i5);
            }
        }
    }    

    std::cout << "hit matching successfully" << std::endl;
}

void HitMatch::SaveMatchFile()
{
    match_file->cd();
    match_tree->Write();
    match_file->Close();

    std::cout << "save match file successfully" << std::endl;
}