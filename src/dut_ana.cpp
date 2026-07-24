#include "./../include/dut_ana.h"

#include <filesystem>
#include <string>
#include <cmath>
#include <limits>
#include <algorithm>
#include <iostream>

#include <TFile.h>
#include <TTree.h>

DutAna::DutAna(char* filename){
    file_ = TFile::Open(filename, "READ");
    tree_ = (TTree*)file_->Get("events");

    tree_->SetBranchAddress("event_number", &dut_data_.event_number);
    tree_->SetBranchAddress("timestamps", dut_data_.timestamps.data());
    tree_->SetBranchAddress("timestamps_r", dut_data_.timestamps_r.data());
    tree_->SetBranchAddress("num_samples", &dut_data_.num_samples);
    tree_->SetBranchAddress("adc", dut_data_.adc[0].data());
    tree_->SetBranchAddress("toa", dut_data_.toa[0].data());
    tree_->SetBranchAddress("tot", dut_data_.tot[0].data());
    tree_->SetBranchAddress("hamming", dut_data_.hamming[0].data());
    tree_->SetBranchAddress("hit_max", dut_data_.hit_max.data());
    tree_->SetBranchAddress("hit_pedestal", dut_data_.hit_pedestal.data());

    nentries_ = tree_->GetEntries();
    std::string basename = std::filesystem::path(filename).filename().string();

    run_number_ = basename.substr(3, basename.find(".root") - 3);
}

DutAna::~DutAna(){
    file_->Close();
    delete file_;
}

void DutAna::ExtractFeatures(){
    std::filesystem::create_directories("./../data/feature");
    TFile* fout = new TFile(("./../data/feature/feature_" + run_number_ + ".root").c_str(), "RECREATE");
    TTree* tout = new TTree("tree", "Waveform Features");

    OutputFeature output_feature;

    tout->Branch("event_number", &output_feature.event_number);
    tout->Branch("timestamps", output_feature.timestamps.data());
    tout->Branch("timestamps_r", output_feature.timestamps_r.data());

    tout->Branch("peak_adc", output_feature.peak_adc.data());
    tout->Branch("peak_time", output_feature.peak_time.data());
    tout->Branch("pedestal", output_feature.pedestal.data());

    std::cout << "Starting feature extraction for run number: " << run_number_ << std::endl;

    for(size_t ithres=0; ithres<nthres; ithres++){
        tout->Branch(
            ("raise_time_th" + std::to_string(thresholds[ithres])).c_str(),
            output_feature.raise_times[ithres].data()
        );
        tout->Branch(
            ("fall_time_th" + std::to_string(thresholds[ithres])).c_str(),
            output_feature.fall_times[ithres].data()
        );
        tout->Branch(
            ("charge_th" + std::to_string(thresholds[ithres])).c_str(),
            output_feature.charges[ithres].data()
        );
        tout->Branch(
            ("tot_th" + std::to_string(thresholds[ithres])).c_str(),
            output_feature.tots[ithres].data()
        );
    }

    for(Long64_t entry=0; entry<nentries_; entry++){
        tree_->GetEntry(entry);

        output_feature.pedestal.fill(0.0);
        output_feature.peak_adc.fill(0.0);
        output_feature.peak_time.fill(-1);

        output_feature.event_number = dut_data_.event_number;
        output_feature.timestamps = dut_data_.timestamps;
        output_feature.timestamps_r = dut_data_.timestamps_r;

        for(int ch=0; ch<288; ch++){
            
            // +----------------+
            // | pedestal calc  |
            // +----------------+
            output_feature.pedestal[ch] = dut_data_.hit_pedestal[ch];

            // +----------------+
            // | peak value     |
            // +----------------+
            output_feature.peak_adc[ch] = dut_data_.hit_max[ch] - output_feature.pedestal[ch];
            output_feature.peak_time[ch] = std::distance(dut_data_.adc[ch].begin(),std::max_element(dut_data_.adc[ch].begin(), dut_data_.adc[ch].end()));            

            // calculate feature that depends on threshold
            for(size_t ithres=0; ithres<nthres; ithres++){
                output_feature.raise_times[ithres][ch] = -1.0;
                output_feature.fall_times[ithres][ch] = -1.0;
                output_feature.charges[ithres][ch] = 0.0;
                output_feature.tots[ithres][ch] = 0.0;

                int threshold = thresholds[ithres];
                
                if(output_feature.peak_adc[ch] < threshold) continue;

                // +--------------+
                // | raise time   |
                // +--------------+
                for(int sample=output_feature.peak_time[ch]; sample>0; sample--){
                    double adc1 = dut_data_.adc[ch][sample] - output_feature.pedestal[ch];
                    double adc2 = dut_data_.adc[ch][sample-1] - output_feature.pedestal[ch];
                    if(adc1 > threshold && adc2 <= threshold){
                        double frac = (threshold - adc2) / (adc1 - adc2);
                        output_feature.raise_times[ithres][ch] = sample - 1 + frac;
                        break;
                    }
                }

                // +--------------+
                // | fall time    |
                // +--------------+
                for(int sample=output_feature.peak_time[ch]; sample<static_cast<int>(dut_data_.num_samples)-1; sample++){
                    double adc1 = dut_data_.adc[ch][sample] - output_feature.pedestal[ch];
                    double adc2 = dut_data_.adc[ch][sample+1] - output_feature.pedestal[ch];
                    if(adc1 > threshold && adc2 <= threshold){
                        double frac = (threshold - adc1) / (adc2 - adc1);
                        output_feature.fall_times[ithres][ch] = sample + frac;
                        break;
                    }
                }

                // // +--------------+
                // // | charge & tot |
                // // +--------------+
                // double start = output_feature.raise_times[ithres][ch];
                // double end = output_feature.fall_times[ithres][ch];

                // if(start < 0.0 || end < 0.0) continue;

                // output_feature.tots[ithres][ch] = end - start;

                // double charge = 0.0;
                // int idx_start = static_cast<int>(std::ceil(start));
                // int idx_end = static_cast<int>(std::floor(end));
                // // start edge charge
                // double w_start = idx_start - start;
                // double h_start_edge = 0.0;
                // double h_start_idx = (dut_data_.adc[ch][idx_start] - output_feature.pedestal[ch]) - threshold;
                // charge = charge + (w_start*(h_start_edge + h_start_idx) / 2.0);
                // // middle charge
                // for(int sample=idx_start; sample<idx_end; sample++){
                //     double h1 = (dut_data_.adc[ch][sample] - output_feature.pedestal[ch]) - threshold;
                //     double h2 = (dut_data_.adc[ch][sample + 1] - output_feature.pedestal[ch]) - threshold;
                //     charge = charge + ((h1 + h2) / 2.0);
                // }
                // // end edge charge
                // double w_end = end - idx_end;
                // double h_end_edge = 0.0;
                // double h_end_idx = (dut_data_.adc[ch][idx_end] - output_feature.pedestal[ch]) - threshold;
                // charge = charge + (w_end*(h_end_edge + h_end_idx) / 2.0);
                // output_feature.charges[ithres][ch] = charge;
                
                // +--------------+
                // | charge & tot |
                // +--------------+
                double start = output_feature.raise_times[ithres][ch];
                double end = output_feature.fall_times[ithres][ch];

                if (start < 0.0 || end < 0.0 || end <= start) continue;

                output_feature.tots[ithres][ch] = end - start;

                // --- 簡潔で安全な積分（台形法） ---
                // 1. 各サンプルの「しきい値を超えた高さ (V - threshold)」を配列/ベクトル化
                std::vector<double> h(dut_data_.num_samples, 0.0);
                for (size_t s = 0; s < dut_data_.num_samples; ++s) {
                    double val = (dut_data_.adc[ch][s] - output_feature.pedestal[ch]) - threshold;
                    h[s] = (val > 0.0) ? val : 0.0; // しきい値以下は 0 にクランプ
                }

                // 2. start から end までの区間を微小ステップ（例: 0.05 サンプル刻み）で数値積分
                // ※解析解を厳密に書くより、補間して細かく足し合わせる方がバグが起きません
                double charge = 0.0;
                int n_steps = 100; // 積分分解能
                double dt = (end - start) / n_steps;

                for (int step = 0; step < n_steps; ++step) {
                    double t1 = start + step * dt;
                    double t2 = start + (step + 1) * dt;

                    // t1, t2 における値を線形補間
                    auto sample_val = [&](double t) {
                        int i = static_cast<int>(std::floor(t));
                        if (i < 0) return 0.0;
                        if (i >= static_cast<int>(dut_data_.num_samples) - 1) return 0.0;
                        double frac = t - i;
                        return (1.0 - frac) * h[i] + frac * h[i + 1];
                    };

                    double h1 = sample_val(t1);
                    double h2 = sample_val(t2);

                    charge += (h1 + h2) * dt / 2.0; // 台形公式
                }

                output_feature.charges[ithres][ch] = charge;
            }
        }
        tout->Fill();

        std::cout << "Processed entry: " << entry + 1 << "/" << nentries_ << "\r" << std::flush;
    }

    std::cout << std::endl << "Feature extraction completed." << std::endl;

    fout->cd();
    tout->Write();
    fout->Close();

    delete fout;
}