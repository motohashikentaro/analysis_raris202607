#include "./../include/bm_hit_matching.h"

#include <iostream>
#include <filesystem>
#include <map>
#include <utility>

#include <TFile.h>
#include <TTree.h>


std::map<EventID, XYHit> ReadXY(TTree* t)
{

    std::map<EventID, XYHit> data;


    XYHit xy;


    t->SetBranchAddress("pcnt", &xy.pcnt);
    t->SetBranchAddress("fcnt", &xy.fcnt);

    t->SetBranchAddress("nx", &xy.nx);
    t->SetBranchAddress("ny", &xy.ny);

    t->SetBranchAddress("x", &xy.x);
    t->SetBranchAddress("y", &xy.y);



    Long64_t nentries = t->GetEntries();


    for(Long64_t i = 0; i < nentries; i++){

        t->GetEntry(i);


        EventID id = {
            xy.pcnt,
            xy.fcnt
        };


        data[id] = xy;

    }


    return data;

}



void MakeHitRoot(
        std::map<EventID,XYHit>& data3,
        std::map<EventID,XYHit>& data4,
        std::map<EventID,XYHit>& data5,
        const std::string& outfile)
{


    TFile* fout = new TFile(
        outfile.c_str(),
        "RECREATE"
    );


    TTree* tout = new TTree(
        "t",
        "3 layer matched"
    );



    MatchedHit hit;



    tout->Branch("pcnt",&hit.pcnt);
    tout->Branch("fcnt",&hit.fcnt);


    tout->Branch("x3",&hit.x3);
    tout->Branch("y3",&hit.y3);

    tout->Branch("x4",&hit.x4);
    tout->Branch("y4",&hit.y4);

    tout->Branch("x5",&hit.x5);
    tout->Branch("y5",&hit.y5);



    tout->Branch("nx3",&hit.nx3);
    tout->Branch("ny3",&hit.ny3);

    tout->Branch("nx4",&hit.nx4);
    tout->Branch("ny4",&hit.ny4);

    tout->Branch("nx5",&hit.nx5);
    tout->Branch("ny5",&hit.ny5);



    int count = 0;



    for(auto& [id, xy3] : data3){


        auto it4 = data4.find(id);
        auto it5 = data5.find(id);



        if(it4 == data4.end() ||
           it5 == data5.end()){

            continue;

        }



        XYHit xy4 = it4->second;
        XYHit xy5 = it5->second;



        hit.pcnt = id.pcnt;
        hit.fcnt = id.fcnt;



        hit.x3 = xy3.x;
        hit.y3 = xy3.y;

        hit.x4 = xy4.x;
        hit.y4 = xy4.y;

        hit.x5 = xy5.x;
        hit.y5 = xy5.y;



        hit.nx3 = xy3.nx;
        hit.ny3 = xy3.ny;

        hit.nx4 = xy4.nx;
        hit.ny4 = xy4.ny;

        hit.nx5 = xy5.nx;
        hit.ny5 = xy5.ny;



        tout->Fill();


        count++;

    }



    std::cout
        << "matched events : "
        << count
        << std::endl;



    tout->Write();

    fout->Close();

    delete fout;

}



int main(int argc,char* argv[])
{


    if(argc < 2){

        std::cout
            << "Usage : ./bm_hit_matching <run directory>"
            << std::endl;

        return 1;

    }



    std::string dirname = argv[1];



    std::string project_dir =
        "/home/motohashi/work/analysis_raris202607";



    std::string input_dir =
        project_dir
        + "/result/xydata/"
        + dirname;



    std::string output_dir =
        project_dir
        + "/result/hitdata/"
        + dirname;



    std::filesystem::create_directories(output_dir);




    TFile* f3 = TFile::Open(
        (input_dir+"/T3_xy.root").c_str()
    );


    TFile* f4 = TFile::Open(
        (input_dir+"/T4_xy.root").c_str()
    );


    TFile* f5 = TFile::Open(
        (input_dir+"/T5_xy.root").c_str()
    );



    TTree* t3 = (TTree*)f3->Get("t");
    TTree* t4 = (TTree*)f4->Get("t");
    TTree* t5 = (TTree*)f5->Get("t");



    std::cout
        << "branch extract successfully"
        << std::endl;



    auto data3 = ReadXY(t3);
    auto data4 = ReadXY(t4);
    auto data5 = ReadXY(t5);



    std::cout
        << "T3 : "
        << data3.size()
        << std::endl;

    std::cout
        << "T4 : "
        << data4.size()
        << std::endl;

    std::cout
        << "T5 : "
        << data5.size()
        << std::endl;




    MakeHitRoot(
        data3,
        data4,
        data5,
        output_dir+"/hit.root"
    );



    f3->Close();
    f4->Close();
    f5->Close();


    delete f3;
    delete f4;
    delete f5;


    return 0;

}