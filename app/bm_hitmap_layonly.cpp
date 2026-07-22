#include <filesystem>

#include <TFile.h>
#include <TTree.h>
#include <TH2.h>
#include <TCanvas.h>


int main(int argc, char* argv[]){
    TFile* file3 = TFile::Open((std::string(argv[1]) + "/T3_xy.root").c_str(), "READ");
    TFile* file4 = TFile::Open((std::string(argv[1]) + "/T4_xy.root").c_str(), "READ");
    TFile* file5 = TFile::Open((std::string(argv[1]) + "/T5_xy.root").c_str(), "READ");

    TTree* tree3 = (TTree*)file3->Get("t");
    TTree* tree4 = (TTree*)file4->Get("t");
    TTree* tree5 = (TTree*)file5->Get("t");

    double x3;
    double y3;
    double x4;
    double y4;
    double x5;
    double y5;

    tree3->SetBranchAddress("x", &x3);
    tree3->SetBranchAddress("y", &y3);
    tree4->SetBranchAddress("x", &x4);
    tree4->SetBranchAddress("y", &y4);
    tree5->SetBranchAddress("x", &x5);
    tree5->SetBranchAddress("y", &y5);

    TH2D* hist3 = new TH2D("hist3", "Hitmap for BM3 non match", 32, 1, 33, 32, 1, 33);
    TH2D* hist4 = new TH2D("hist4", "Hitmap for BM4 non match", 32, 1, 33, 32, 1, 33);
    TH2D* hist5 = new TH2D("hist5", "Hitmap for BM5 non match", 32, 1, 33, 32, 1, 33);

    for(Long64_t i=0; i<tree3->GetEntries(); i++){
        tree3->GetEntry(i);
        hist3->Fill(x3, y3);
    }

    for(Long64_t i=0; i<tree4->GetEntries(); i++){
        tree4->GetEntry(i);
        hist4->Fill(x4, y4);
    }

    for(Long64_t i=0; i<tree5->GetEntries(); i++){
        tree5->GetEntry(i);
        hist5->Fill(x5, y5);
    }

    TCanvas* c1 = new TCanvas("c1", "Hitmaps", 2400, 800);
    c1->Divide(3, 1);
    c1->cd(1);
    hist3->Draw("COLZ");
    c1->cd(2);
    hist4->Draw("COLZ");
    c1->cd(3);
    hist5->Draw("COLZ");


    std::filesystem::path run_path(argv[1]);
    std::string dirname = run_path.filename().string();

    std::string run_number = dirname.substr(0, dirname.find('_'));

    c1->SaveAs(("./../result/" + run_number + "_Hitmaps_nonmatch.png").c_str());

}