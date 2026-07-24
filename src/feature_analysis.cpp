#include "./../include/feature_analysis.h"

#include <string>
#include <filesystem>
#include <iostream>

FeatureAnalysis::FeatureAnalysis(char* filename){
    file_ = TFile::Open(filename, "READ");
    tree_ = (TTree*)file_->Get("tree");

    tree_->SetBranchAddress("event_number", &feature_.event_number);
    tree_->SetBranchAddress("timestamps", feature_.timestamps.data());
    tree_->SetBranchAddress("timestamps_r", feature_.timestamps_r.data());
    tree_->SetBranchAddress("peak_adc", feature_.peak_adc.data());
    tree_->SetBranchAddress("peak_time", feature_.peak_time.data());
    tree_->SetBranchAddress("pedestal", feature_.pedestal.data());

    for(size_t i=0; i<thresholds.size(); i++){
        tree_->SetBranchAddress(
            ("raise_time_th" + std::to_string(thresholds[i])).c_str(),
            feature_.raise_times[i].data()
        );
        tree_->SetBranchAddress(
            ("fall_time_th" + std::to_string(thresholds[i])).c_str(),
            feature_.fall_times[i].data()
        );
        tree_->SetBranchAddress(
            ("charge_th" + std::to_string(thresholds[i])).c_str(),
            feature_.charges[i].data()
        );
        tree_->SetBranchAddress(
            ("tot_th" + std::to_string(thresholds[i])).c_str(),
            feature_.tots[i].data()
        );
    }
    nentries_ = tree_->GetEntries();

    std::string basename = std::filesystem::path(filename).filename().string();
    run_number_ = basename.substr(3, basename.find(".root") - 8);

}

FeatureAnalysis::~FeatureAnalysis(){
    file_->Close();
    delete file_;
}

TH1D* FeatureAnalysis::PeakAdcDistro(int ch){
    TH1D* hist = new TH1D(("peak_adc_ch" + std::to_string(ch)).c_str(), ("Peak ADC Distribution for Channel " + std::to_string(ch)).c_str(), 10, 0, 50);
    for(Long64_t entry=0; entry<nentries_; entry++){
        tree_->GetEntry(entry);
        hist->Fill(feature_.peak_adc[ch]);
    }
    hist->GetXaxis()->SetTitle("Peak ADC Value");
    hist->GetYaxis()->SetTitle("Entries");
    return hist;
}

TH1D* FeatureAnalysis::PeakTimeDistro(int ch){
    TH1D* hist = new TH1D(("peak_time_ch" + std::to_string(ch)).c_str(), ("Peak Time Distribution for Channel " + std::to_string(ch)).c_str(), 16, 0, 16);
    for(Long64_t entry=0; entry<nentries_; entry++){
        tree_->GetEntry(entry);
        hist->Fill(feature_.peak_time[ch]);
    }
    hist->GetXaxis()->SetTitle("Peak Time Value");
    hist->GetYaxis()->SetTitle("Entries");
    return hist;
}

TH1D* FeatureAnalysis::PedestalDistro(int ch){
    TH1D* hist = new TH1D(("pedestal_ch" + std::to_string(ch)).c_str(), ("Pedestal Distribution for Channel " + std::to_string(ch)).c_str(), 100, 50, 150);
    for(Long64_t entry=0; entry<nentries_; entry++){
        tree_->GetEntry(entry);
        hist->Fill(feature_.pedestal[ch]);
    }
    hist->GetXaxis()->SetTitle("Pedestal Value");
    hist->GetYaxis()->SetTitle("Entries");
    return hist;
}

std::vector<TH1D*> FeatureAnalysis::RaiseTimeDistro(int ch){
    std::vector<TH1D*> hists;
    for(size_t i=0; i<thresholds.size(); i++){
        TH1D* hist = new TH1D(("raise_time_ch" + std::to_string(ch) + "_th" + std::to_string(thresholds[i])).c_str(), ("Raise Time Distribution for Channel " + std::to_string(ch) + " at Threshold " + std::to_string(thresholds[i])).c_str(), 16, 0, 16);
        hists.push_back(hist);
    }
    for(Long64_t entry=0; entry<nentries_; entry++){
        tree_->GetEntry(entry);
        for(size_t i=0; i<hists.size(); i++){
            hists[i]->Fill(feature_.raise_times[i][ch]);
        }
    }
    for(size_t i=0; i<hists.size(); i++){
        hists[i]->GetXaxis()->SetTitle("Raise Time Value");
        hists[i]->GetYaxis()->SetTitle("Entries");
    }
    return hists;
}

std::vector<TH1D*> FeatureAnalysis::FallTimeDistro(int ch){
    std::vector<TH1D*> hists;
    for(size_t i=0; i<thresholds.size(); i++){
        TH1D* hist = new TH1D(("fall_time_ch" + std::to_string(ch) + "_th" + std::to_string(thresholds[i])).c_str(), ("Fall Time Distribution for Channel " + std::to_string(ch) + " at Threshold " + std::to_string(thresholds[i])).c_str(), 16, 0, 16);
        hists.push_back(hist);
    }
    for(Long64_t entry=0; entry<nentries_; entry++){
        tree_->GetEntry(entry);
        for(size_t i=0; i<hists.size(); i++){
            hists[i]->Fill(feature_.fall_times[i][ch]);
        }
    }
    for(size_t i=0; i<hists.size(); i++){
        hists[i]->GetXaxis()->SetTitle("Fall Time Value");
        hists[i]->GetYaxis()->SetTitle("Entries");
    }
    return hists;
}

std::vector<TH1D*> FeatureAnalysis::ChargeDistro(int ch){
    std::vector<TH1D*> hists;
    for(size_t i=0; i<thresholds.size(); i++){
        TH1D* hist = new TH1D(("charge_ch" + std::to_string(ch) + "_th" + std::to_string(thresholds[i])).c_str(), ("Charge Distribution for Channel " + std::to_string(ch) + " at Threshold " + std::to_string(thresholds[i])).c_str(), 100, 0, 100);
        hists.push_back(hist);
    }
    for(Long64_t entry=0; entry<nentries_; entry++){
        tree_->GetEntry(entry);
        for(size_t i=0; i<hists.size(); i++){
            hists[i]->Fill(feature_.charges[i][ch]);
        }
    }
    for(size_t i=0; i<hists.size(); i++){
        hists[i]->GetXaxis()->SetTitle("Charge Value");
        hists[i]->GetYaxis()->SetTitle("Entries");
    }
    return hists;
}

std::vector<TH1D*> FeatureAnalysis::TotDistro(int ch){
    std::vector<TH1D*> hists;
    for(size_t i=0; i<thresholds.size(); i++){
        TH1D* hist = new TH1D(("tot_ch" + std::to_string(ch) + "_th" + std::to_string(thresholds[i])).c_str(), ("Time Over Threshold Distribution for Channel " + std::to_string(ch) + " at Threshold " + std::to_string(thresholds[i])).c_str(), 16, 0, 16);
        hists.push_back(hist);
    }
    for(Long64_t entry=0; entry<nentries_; entry++){
        tree_->GetEntry(entry);
        for(size_t i=0; i<hists.size(); i++){
            hists[i]->Fill(feature_.tots[i][ch]);
        }
    }
    for(size_t i=0; i<hists.size(); i++){
        hists[i]->GetXaxis()->SetTitle("Time Over Threshold Value");
        hists[i]->GetYaxis()->SetTitle("Entries");
    }
    return hists;
}