#include "./../include/gain_calibration.h"

#include <filesystem>
#include <string>
#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TCanvas.h>

void GainCalibration::BranchExtraction(FileData& data){

    data.tree->SetBranchAddress("ADC", &data.gd.adc);
    data.tree->SetBranchAddress("ADCmax", &data.gd.adc_max);
    data.tree->SetBranchAddress("iX", &data.gd.ix);
    data.tree->SetBranchAddress("iY", &data.gd.iy);

    data.nentries = data.tree->GetEntries();

    std::cout << "Branch extraction completed" << std::endl;
}

GainCalibration::GainCalibration(char* input_path){

    data3_.file = TFile::Open((input_path + std::string("/T3_Sci.root")).c_str(), "READ");
    data3_.tree = (TTree*)data3_.file->Get("t");

    data4_.file = TFile::Open((input_path + std::string("/T4_Sci.root")).c_str(), "READ");
    data4_.tree = (TTree*)data4_.file->Get("t");

    data5_.file = TFile::Open((input_path + std::string("/T5_Sci.root")).c_str(), "READ");
    data5_.tree = (TTree*)data5_.file->Get("t");
        

    BranchExtraction(data3_);
    BranchExtraction(data4_);
    BranchExtraction(data5_);


    std::string path(input_path);
    run_dir_ = std::filesystem::path(path).filename().string();

    if (run_dir_.starts_with("Run")) {
        run_number_ = run_dir_.substr(3, 4);
    }

    std::cout << "GainCalibration initialized for " << run_number_ << std::endl;
}

GainCalibration::~GainCalibration(){
    if (data3_.file) {
        data3_.file->Close();
        delete data3_.file;
        data3_.file = nullptr;
    }

    if (data4_.file) {
        data4_.file->Close();
        delete data4_.file;
        data4_.file = nullptr;
    }

    if (data5_.file) {
        data5_.file->Close();
        delete data5_.file;
        data5_.file = nullptr;
    }   
}

FileData* GainCalibration::GetFileData(int nlayer){
    switch(nlayer){

    case BM3_X:
    case BM3_Y:
        return &data3_;

    case BM4_X:
    case BM4_Y:
        return &data4_;

    case BM5_X:
    case BM5_Y:
        return &data5_;

    default:
        return nullptr;
    }
}

void GainCalibration::AdcPlot(int nlayer, int nstrip){

    FileData* data = GetFileData(nlayer);

    TH1D* hist = new TH1D(Form("adc_hist_%d_%d", nlayer, nstrip), Form("ADC Histogram for Layer %d, Strip %d", nlayer, nstrip), 100, -1000, 4000);

    std::cout << "Generating ADC histogram for Layer " << nlayer << ", Strip " << nstrip << std::endl;

    for(Long64_t i = 0; i<data->nentries; i++){
        data->tree->GetEntry(i);

        for(size_t j = 0; j<data->gd.adc->size(); j++){
            bool isX = data->gd.ix->at(j) > 0;
            bool isY = data->gd.iy->at(j) > 0;

            if(nlayer % 2 == 0){
                if(!isX) continue;
            }else{
                if(!isY) continue;
            }

            int strip;
            if(nlayer % 2 == 0){
                strip = data->gd.ix->at(j);
            }else{
                strip = data->gd.iy->at(j);
            }

            if(strip == nstrip){
                hist->Fill(data->gd.adc->at(j));
            }
        }
    }

    std::cout << "Histogram filled for Layer " << nlayer << ", Strip " << nstrip << std::endl;

    hist->GetXaxis()->SetTitle("ADC Value");
    hist->GetYaxis()->SetTitle("Entries");

    TCanvas* canvas = new TCanvas(Form("canvas_%d_%d", nlayer, nstrip), Form("ADC Histogram for Layer %d, Strip %d", nlayer, nstrip), 800, 600);
    hist->Draw();
    
    std::string project_dir = "/home/motohashi/work/analysis_raris202607";
    std::string output_dir = project_dir + "/result/adc_plot/" + run_dir_ + Form("/layer%d", nlayer);
    std::filesystem::create_directories(output_dir);

    std::string output_path = output_dir + Form("/adc_hist_%d_%d.png", nlayer, nstrip);
    canvas->SaveAs(output_path.c_str());

    delete canvas;
    delete hist;
}