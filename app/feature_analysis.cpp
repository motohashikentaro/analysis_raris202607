#include "./../include/feature_analysis.h"

#include <string>
#include <filesystem>
#include <iostream>
#include <vector>

#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TColor.h>

int main(int argc, char* argv[]){
    FeatureAnalysis analysis(argv[1]);

    TCanvas* c1 = new TCanvas("c1", "Feature Analysis", 2400, 800);
    TCanvas* c2 = new TCanvas("c2", "Feature Analysis", 800, 800);
    TCanvas* c3 = new TCanvas("c3", "Feature Analysis", 800, 800);
    TCanvas* c4 = new TCanvas("c4", "Feature Analysis", 800, 800);
    TCanvas* c5 = new TCanvas("c5", "Feature Analysis", 800, 800);

    gStyle->SetOptStat(0);
    TLatex* latex = new TLatex();
    latex->SetNDC();
    
    c1->Divide(3, 1);
    c1->cd(1);
    latex->DrawLatex(0.1, 0.9, ("Run Number: " + std::string(analysis.run_number_.c_str())).c_str());
    TH1D* hist_peak_adc_ch0 = analysis.PeakAdcDistro(0);
    hist_peak_adc_ch0->Draw();
    c1->cd(2);
    TH1D* hist_peak_time_ch0 = analysis.PeakTimeDistro(0);
    hist_peak_time_ch0->Draw();
    c1->cd(3);
    TH1D* hist_pedestal_ch0 = analysis.PedestalDistro(0);
    hist_pedestal_ch0->Draw();

    c2->cd();
    latex->DrawLatex(0.1, 0.9, ("Run Number: " + std::string(analysis.run_number_.c_str())).c_str());
    std::vector<TH1D*> hist_raise_time_ch0 = analysis.RaiseTimeDistro(0);
    bool first = true;
    int color_index = 1;
    for(auto& hist : hist_raise_time_ch0){
        if(first){
            hist->SetLineColor(color_index);
            hist->Draw();
            first = false;
        } else {
            hist->Draw("same");
        }
        color_index++;
    }

    c3->cd();
    latex->DrawLatex(0.1, 0.9, ("Run Number: " + std::string(analysis.run_number_.c_str())).c_str());
    std::vector<TH1D*> hist_fall_time_ch0 = analysis.FallTimeDistro(0);
    first = true;
    color_index = 1;
    for(auto& hist : hist_fall_time_ch0){
        if(first){
            hist->SetLineColor(color_index);
            hist->Draw();
            first = false;
        } else {
            hist->Draw("same");
        }
        color_index++;
    }

    c4->cd();
    latex->DrawLatex(0.1, 0.9, ("Run Number: " + std::string(analysis.run_number_.c_str())).c_str());
    std::vector<TH1D*> hist_charge_ch0 = analysis.ChargeDistro(0);
    first = true;
    color_index = 1;
    for(auto& hist : hist_charge_ch0){
        if(first){
            hist->SetLineColor(color_index);
            hist->Draw();
            first = false;
        } else {
            hist->Draw("same");
        }
        color_index++;
    }

    c5->cd();
    latex->DrawLatex(0.1, 0.9, ("Run Number: " + std::string(analysis.run_number_.c_str())).c_str());
    std::vector<TH1D*> hist_tot_ch0 = analysis.TotDistro(0);
    first = true;
    color_index = 1;
    for(auto& hist : hist_tot_ch0){
        if(first){
            hist->SetLineColor(color_index);
            hist->Draw();
            first = false;
        } else {
            hist->Draw("same");
        }
        color_index++;
    }

    c1->SaveAs((std::string("./../result/") + analysis.run_number_ + "_feature_analysis_summary.png").c_str());
    c2->SaveAs((std::string("./../result/") + analysis.run_number_ + "_feature_analysis_raise_time.png").c_str());
    c3->SaveAs((std::string("./../result/") + analysis.run_number_ + "_feature_analysis_fall_time.png").c_str());
    c4->SaveAs((std::string("./../result/") + analysis.run_number_ + "_feature_analysis_charge.png").c_str());
    c5->SaveAs((std::string("./../result/") + analysis.run_number_ + "_feature_analysis_tot.png").c_str());

    return 0;
}