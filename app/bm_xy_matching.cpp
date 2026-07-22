#include "./../include/bm_xy_matching.h"

#include <iostream>
#include <algorithm>
#include <filesystem>

#include <TFile.h>
#include <TTree.h>

void MakeXYRoot(TTree* tin,
                HitFeature& hit,
                Long64_t nentries,
                const std::string& outfile){

    TFile* fout = new TFile(outfile.c_str(), "RECREATE");
    TTree* tout = new TTree("t", "xy matched");

    XYHit xy;

    tout->Branch("pcnt", &xy.pcnt);
    tout->Branch("fcnt", &xy.fcnt);

    tout->Branch("nx", &xy.nx);
    tout->Branch("ny", &xy.ny);

    tout->Branch("x", &xy.x);
    tout->Branch("y", &xy.y);

    bool first = true;

    int current_pcnt = 0;
    int current_fcnt = 0;

    double x_sum = 0;
    double y_sum = 0;

    int nx = 0;
    int ny = 0;

    for(Long64_t i = 0; i < nentries; i++){

        tin->GetEntry(i);

        std::cout << "nhits = " << hit.nhits
          << " ix = " << hit.ix->size()
          << " iy = " << hit.iy->size()
          << std::endl;

        int pcnt = hit.id.pcnt;
        int fcnt = hit.id.fcnt;

        if(!first &&
           (pcnt != current_pcnt ||
            fcnt != current_fcnt)){

            if(nx > 0 &&
               ny > 0){

                xy.pcnt = current_pcnt;
                xy.fcnt = current_fcnt;

                xy.nx = nx;
                xy.ny = ny;

                xy.x = x_sum / nx;
                xy.y = y_sum / ny;

                tout->Fill();

            }

            x_sum = 0;
            y_sum = 0;

            nx = 0;
            ny = 0;

        }

        first = false;

        current_pcnt = pcnt;
        current_fcnt = fcnt;

        for(int ihit = 0; ihit < hit.nhits; ihit++){

            int ix = hit.ix->at(ihit);
            int iy = hit.iy->at(ihit);

            if(ix != 0){

                x_sum += ix;
                nx++;

            }

            if(iy != 0){

                y_sum += iy;
                ny++;

            }

        }

    }

    if(nx > 0 &&
       ny > 0){

        xy.pcnt = current_pcnt;
        xy.fcnt = current_fcnt;

        xy.nx = nx;
        xy.ny = ny;

        xy.x = x_sum / nx;
        xy.y = y_sum / ny;

        tout->Fill();

    }

    std::cout << "loop complete" << std::endl;

    tout->Write();

    fout->Close();

    // delete tout;
    delete fout;

}

int main(int argc, char* argv[]){

    if(argc < 2){

        std::cout << "Usage: ./bm_hit_matching <run directory>" << std::endl;
        return 1;

    }


    std::string dirname = argv[1];


    HitMatch hm;


    hm.f3 = TFile::Open(
        ("/home/motohashi/work/analysis_raris202607/data/" 
        + dirname + "/T3_Sci.root").c_str()
    );

    hm.f4 = TFile::Open(
        ("/home/motohashi/work/analysis_raris202607/data/" 
        + dirname + "/T4_Sci.root").c_str()
    );

    hm.f5 = TFile::Open(
        ("/home/motohashi/work/analysis_raris202607/data/" 
        + dirname + "/T5_Sci.root").c_str()
    );


    hm.t3 = (TTree*)hm.f3->Get("t");
    hm.t4 = (TTree*)hm.f4->Get("t");
    hm.t5 = (TTree*)hm.f5->Get("t");



    hm.t3->SetBranchAddress("pcnt", &hm.bm3.id.pcnt);
    hm.t3->SetBranchAddress("fcnt", &hm.bm3.id.fcnt);
    hm.t3->SetBranchAddress("nHits", &hm.bm3.nhits);

    hm.t3->SetBranchAddress("channel", &hm.bm3.channel);
    hm.t3->SetBranchAddress("ADC", &hm.bm3.adc);

    hm.t3->SetBranchAddress("ADCmax", &hm.bm3.adc_max);

    hm.t3->SetBranchAddress("iX", &hm.bm3.ix);
    hm.t3->SetBranchAddress("iY", &hm.bm3.iy);



    hm.t4->SetBranchAddress("pcnt", &hm.bm4.id.pcnt);
    hm.t4->SetBranchAddress("fcnt", &hm.bm4.id.fcnt);
    hm.t4->SetBranchAddress("nHits", &hm.bm4.nhits);

    hm.t4->SetBranchAddress("channel", &hm.bm4.channel);
    hm.t4->SetBranchAddress("ADC", &hm.bm4.adc);

    hm.t4->SetBranchAddress("ADCmax", &hm.bm4.adc_max);

    hm.t4->SetBranchAddress("iX", &hm.bm4.ix);
    hm.t4->SetBranchAddress("iY", &hm.bm4.iy);



    hm.t5->SetBranchAddress("pcnt", &hm.bm5.id.pcnt);
    hm.t5->SetBranchAddress("fcnt", &hm.bm5.id.fcnt);
    hm.t5->SetBranchAddress("nHits", &hm.bm5.nhits);

    hm.t5->SetBranchAddress("channel", &hm.bm5.channel);
    hm.t5->SetBranchAddress("ADC", &hm.bm5.adc);

    hm.t5->SetBranchAddress("ADCmax", &hm.bm5.adc_max);

    hm.t5->SetBranchAddress("iX", &hm.bm5.ix);
    hm.t5->SetBranchAddress("iY", &hm.bm5.iy);



    hm.nentries3 = hm.t3->GetEntries();
    hm.nentries4 = hm.t4->GetEntries();
    hm.nentries5 = hm.t5->GetEntries();


    std::cout << "branch extract successfully" << std::endl;



    std::string project_dir = "/home/motohashi/work/analysis_raris202607";

    std::string result_dir = project_dir
                        + "/result/xydata/"
                        + dirname;


    std::filesystem::create_directories(result_dir);



    MakeXYRoot(
        hm.t3,
        hm.bm3,
        hm.nentries3,
        result_dir + "/T3_xy.root"
    );


    MakeXYRoot(
        hm.t4,
        hm.bm4,
        hm.nentries4,
        result_dir + "/T4_xy.root"
    );


    MakeXYRoot(
        hm.t5,
        hm.bm5,
        hm.nentries5,
        result_dir + "/T5_xy.root"
    );

    hm.f3->Close();
    hm.f4->Close();
    hm.f5->Close();


    return 0;

}