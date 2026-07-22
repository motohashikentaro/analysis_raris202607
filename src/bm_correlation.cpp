#include "./../include/bm_correlation.h"

#include <iostream>
#include <regex>

#include <TFile.h>
#include <TTree.h>
#include <TH2.h>
#include <TH1.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TLatex.h>

BmCorrela::BmCorrela(const char* filepath) {
    file_ = TFile::Open(filepath, "READ");
    tree_ = (TTree*)file_->Get("t");

    tree_->SetBranchAddress("pcnt", &data_.pcnt);
    tree_->SetBranchAddress("fcnt", &data_.fcnt);
    tree_->SetBranchAddress("x3", &data_.x3);
    tree_->SetBranchAddress("y3", &data_.y3);
    tree_->SetBranchAddress("x4", &data_.x4);
    tree_->SetBranchAddress("y4", &data_.y4);
    tree_->SetBranchAddress("x5", &data_.x5);
    tree_->SetBranchAddress("y5", &data_.y5);
    tree_->SetBranchAddress("nx3", &data_.nx3);
    tree_->SetBranchAddress("ny3", &data_.ny3);
    tree_->SetBranchAddress("nx4", &data_.nx4);
    tree_->SetBranchAddress("ny4", &data_.ny4);
    tree_->SetBranchAddress("nx5", &data_.nx5);
    tree_->SetBranchAddress("ny5", &data_.ny5);

    nentries_ = tree_->GetEntries();

    run_name_ = "RunUnknown";

    std::regex pattern("Run[0-9]+");
    std::smatch match;

    std::string filename(filepath);

    if(std::regex_search(filename, match, pattern)){
        run_name_ = match.str();
    }
}

BmCorrela::~BmCorrela() {
    file_->Close();
    delete file_;
}

void BmCorrela::Hitmap(){
    TH2D* map_3 = new TH2D("map3", "Hitmap for BM3", 32, 1, 33, 32, 1, 33);
    TH2D* map_4 = new TH2D("map4", "Hitmap for BM4", 32, 1, 33, 32, 1, 33);
    TH2D* map_5 = new TH2D("map5", "Hitmap for BM5", 32, 1, 33, 32, 1, 33);

    for (Long64_t i = 0; i < nentries_; ++i) {
        tree_->GetEntry(i);
        map_3->Fill(data_.x3, data_.y3);
        map_4->Fill(data_.x4, data_.y4);
        map_5->Fill(data_.x5, data_.y5);
    }

    TCanvas* c1 = new TCanvas("c1", "Hitmaps", 2400, 800);
    c1->Divide(3, 1);
    c1->cd(1);
    map_3->Draw("COLZ");
    c1->cd(2);
    map_4->Draw("COLZ");
    c1->cd(3);
    map_5->Draw("COLZ");
        
    std::string save_name = "./../result/" + run_name_ + "_Hitmaps.png";

    c1->SaveAs(save_name.c_str());
}

void BmCorrela::XyCorrelation(){
    TH2D* corr_34x = new TH2D("corr_34x", "X Correlation between BM3 and BM4", 32, 1, 33, 32, 1, 33);
    TH2D* corr_34y = new TH2D("corr_34y", "Y Correlation between BM3 and BM4", 32, 1, 33, 32, 1, 33);
    TH2D* corr_45x = new TH2D("corr_45x", "X Correlation between BM4 and BM5", 32, 1, 33, 32, 1, 33);
    TH2D* corr_45y = new TH2D("corr_45y", "Y Correlation between BM4 and BM5", 32, 1, 33, 32, 1, 33);
    TH2D* corr_35x = new TH2D("corr_35x", "X Correlation between BM3 and BM5", 32, 1, 33, 32, 1, 33);
    TH2D* corr_35y = new TH2D("corr_35y", "Y Correlation between BM3 and BM5", 32, 1, 33, 32, 1, 33);

    for(Long64_t i = 0; i < nentries_; ++i) {
        tree_->GetEntry(i);
        corr_34x->Fill(data_.x3, data_.x4);
        corr_34y->Fill(data_.y3, data_.y4);
        corr_45x->Fill(data_.x4, data_.x5);
        corr_45y->Fill(data_.y4, data_.y5);
        corr_35x->Fill(data_.x3, data_.x5);
        corr_35y->Fill(data_.y3, data_.y5);
    }

    TCanvas* c2 = new TCanvas("c2", "Correlations", 2400, 1600);
    c2->Divide(3, 2);
    c2->cd(1);
    corr_34x->Draw("COLZ");
    c2->cd(2);
    corr_34y->Draw("COLZ");
    c2->cd(3);
    corr_45x->Draw("COLZ");
    c2->cd(4);
    corr_45y->Draw("COLZ");
    c2->cd(5);
    corr_35x->Draw("COLZ");
    c2->cd(6);
    corr_35y->Draw("COLZ");

    std::string save_name = "./../result/" + run_name_ + "_Correlations.png";
    c2->SaveAs(save_name.c_str());
}

void BmCorrela::BeamProfile(){
    TH1D* profile_3x = new TH1D("profile_3x", "Beam Profile for BM3 (X)", 32, 1, 33);
    TH1D* profile_3y = new TH1D("profile_3y", "Beam Profile for BM3 (Y)", 32, 1, 33);
    TH1D* profile_4x = new TH1D("profile_4x", "Beam Profile for BM4 (X)", 32, 1, 33);
    TH1D* profile_4y = new TH1D("profile_4y", "Beam Profile for BM4 (Y)", 32, 1, 33);
    TH1D* profile_5x = new TH1D("profile_5x", "Beam Profile for BM5 (X)", 32, 1, 33);
    TH1D* profile_5y = new TH1D("profile_5y", "Beam Profile for BM5 (Y)", 32, 1, 33);

    TF1* fit_3x = new TF1("fit_3x", "gaus", 1, 33);
    TF1* fit_3y = new TF1("fit_3y", "gaus", 1, 33);
    TF1* fit_4x = new TF1("fit_4x", "gaus", 1, 33);
    TF1* fit_4y = new TF1("fit_4y", "gaus", 1, 33);
    TF1* fit_5x = new TF1("fit_5x", "gaus", 1, 33);
    TF1* fit_5y = new TF1("fit_5y", "gaus", 1, 33);

    for(Long64_t i = 0; i < nentries_; ++i) {
        tree_->GetEntry(i);
        profile_3x->Fill(data_.x3);
        profile_3y->Fill(data_.y3);
        profile_4x->Fill(data_.x4);
        profile_4y->Fill(data_.y4);
        profile_5x->Fill(data_.x5);
        profile_5y->Fill(data_.y5);
    }

    profile_3x->Fit(fit_3x, "QR");
    profile_3y->Fit(fit_3y, "QR");
    profile_4x->Fit(fit_4x, "QR");
    profile_4y->Fit(fit_4y, "QR");
    profile_5x->Fit(fit_5x, "QR");
    profile_5y->Fit(fit_5y, "QR");

    profile_3x->GetXaxis()->SetTitle("Index X");
    profile_3y->GetXaxis()->SetTitle("Index Y");
    profile_4x->GetXaxis()->SetTitle("Index X");
    profile_4y->GetXaxis()->SetTitle("Index Y");
    profile_5x->GetXaxis()->SetTitle("Index X");
    profile_5y->GetXaxis()->SetTitle("Index Y");

    profile_3x->GetYaxis()->SetTitle("Entries");
    profile_3y->GetYaxis()->SetTitle("Entries");
    profile_4x->GetYaxis()->SetTitle("Entries");
    profile_4y->GetYaxis()->SetTitle("Entries");
    profile_5x->GetYaxis()->SetTitle("Entries");
    profile_5y->GetYaxis()->SetTitle("Entries");

    TLatex* latex = new TLatex();
    latex->SetTextSize(0.04);
    latex->SetNDC();

    TCanvas* c3 = new TCanvas("c3", "Beam Profiles", 2400, 1600);
    c3->Divide(3, 2);
    c3->cd(1);
    profile_3x->Draw();
    latex->DrawLatex(0.15, 0.8, Form("Mean: %.2f", fit_3x->GetParameter(1)));
    latex->DrawLatex(0.15, 0.75, Form("Sigma: %.2f", fit_3x->GetParameter(2)));
    c3->cd(2);
    profile_3y->Draw();
    latex->DrawLatex(0.15, 0.8, Form("Mean: %.2f", fit_3y->GetParameter(1)));
    latex->DrawLatex(0.15, 0.75, Form("Sigma: %.2f", fit_3y->GetParameter(2)));
    c3->cd(3);
    profile_4x->Draw();
    latex->DrawLatex(0.15, 0.8, Form("Mean: %.2f", fit_4x->GetParameter(1)));
    latex->DrawLatex(0.15, 0.75, Form("Sigma: %.2f", fit_4x->GetParameter(2)));
    c3->cd(4);
    profile_4y->Draw();
    latex->DrawLatex(0.15, 0.8, Form("Mean: %.2f", fit_4y->GetParameter(1)));
    latex->DrawLatex(0.15, 0.75, Form("Sigma: %.2f", fit_4y->GetParameter(2)));
    c3->cd(5);
    profile_5x->Draw();
    latex->DrawLatex(0.55, 0.8, Form("Mean: %.2f", fit_5x->GetParameter(1)));
    latex->DrawLatex(0.55, 0.75, Form("Sigma: %.2f", fit_5x->GetParameter(2)));
    c3->cd(6);
    profile_5y->Draw();
    latex->DrawLatex(0.15, 0.8, Form("Mean: %.2f", fit_5y->GetParameter(1)));
    latex->DrawLatex(0.15, 0.75, Form("Sigma: %.2f", fit_5y->GetParameter(2)));

    std::string save_name = "./../result/" + run_name_ + "_BeamProfiles.png";
    c3->SaveAs(save_name.c_str());
}