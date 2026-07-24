#include "./../include/conbine_detecters.h"

#include <iostream>
#include <array>

#include <TFile.h>
#include <TTree.h>

ConbineDetecters::ConbineDetecters() {
    hfile = TFile::Open("/home/motohashi/work/analysis_raris202607/data/Run340.root", "READ");
    cfile = TFile::Open("/home/motohashi/work/analysis_raris202607/data/Run0340_HV16_VF448_448_x10_085844.701Monitor5/T5_Sci.root", "READ");
    htree = (TTree*)hfile->Get("events");
    ctree = (TTree*)cfile->Get("t");

    htree->SetBranchAddress("timestamps_r", &h2gc_time.timestamps_r);
    ctree->SetBranchAddress("pcnt", &citi_time.pcnt);
    ctree->SetBranchAddress("fcnt", &citi_time.fcnt);

    htree_entries = htree->GetEntries();
    ctree_entries = ctree->GetEntries();
}

ConbineDetecters::~ConbineDetecters() {
    hfile->Close();
    cfile->Close();
}


void ConbineDetecters::test(){

    for(Long64_t iH = 0; iH < htree_entries - 1; iH++){

        htree->GetEntry(iH);
        double tH0 = h2gc_time.timestamps_r[0] * 25.0;

        htree->GetEntry(iH + 1);
        double tH1 = h2gc_time.timestamps_r[0] * 25.0;

        double deltaH = tH1 - tH0;


        for(Long64_t iC = 0; iC < ctree_entries - 1; iC++){

            ctree->GetEntry(iC);
            double tC0 = citi_time.fcnt * 240.0;

            ctree->GetEntry(iC + 1);
            double tC1 = citi_time.fcnt * 240.0;

            double deltaC = tC1 - tC0;


            if(deltaH == deltaC){

                std::cout << "Sync point found!" << std::endl;

                std::cout << "H event : "
                          << iH << " -> " << iH+1
                          << std::endl;

                std::cout << "C event : "
                          << iC << " -> " << iC+1
                          << std::endl;

                std::cout << "deltaH = "
                          << deltaH
                          << " ns"
                          << std::endl;

                std::cout << "deltaC = "
                          << deltaC
                          << " ns"
                          << std::endl;

                return;
            }
        }
    }

    std::cout << "No sync point found" << std::endl;
}

// void ConbineDetecters::test(){

//     Long64_t syncH = -1;
//     Long64_t syncC = -1;

//     double syncTH = 0;
//     double syncTC = 0;


//     // ============================
//     // Find synchronization point
//     // ============================

//     for(Long64_t iH = 0; iH < htree_entries - 1; iH++){

//         htree->GetEntry(iH);
//         double tH0 = h2gc_time.timestamps_r[0] * 25.0;

//         htree->GetEntry(iH + 1);
//         double tH1 = h2gc_time.timestamps_r[0] * 25.0;

//         double deltaH = tH1 - tH0;


//         for(Long64_t iC = 0; iC < ctree_entries - 1; iC++){

//             ctree->GetEntry(iC);
//             double tC0 = citi_time.fcnt * 240.0;

//             ctree->GetEntry(iC + 1);
//             double tC1 = citi_time.fcnt * 240.0;

//             double deltaC = tC1 - tC0;

//             std::cout<<"deltaH: "<<deltaH<<" deltaC: "<<deltaC<<std::endl;

//             break;


            // if(deltaH == deltaC){

            //     std::cout << "Sync point found" << std::endl;

            //     syncH = iH;
            //     syncC = iC;

            //     syncTH = tH0;
            //     syncTC = tC0;

            //     break;
            // }
    //     }

    //     // if(syncH >= 0)
    //     //     break;
    // }


    // if(syncH < 0){
    //     std::cout << "No sync point found" << std::endl;
    //     return;
    // }


    // ============================
    // Check next 100 events
    // ============================

    // std::cout << "Check next 100 events" << std::endl;


    // Long64_t ncheck = 50;


    // for(Long64_t i=0; i<ncheck; i++){

    //     Long64_t iH = syncH + i;
    //     Long64_t iC = syncC + i;


    //     if(iH >= htree_entries ||
    //        iC >= ctree_entries)
    //         break;


    //     htree->GetEntry(iH);
    //     ctree->GetEntry(iC);


    //     double tH = h2gc_time.timestamps_r[0] * 25.0;
    //     double tC = citi_time.fcnt * 240.0;


    //     double tHp = tH - syncTH;
    //     double tCp = tC - syncTC;


    //     double diff = std::abs(tCp - tHp);


    //     std::cout
    //         << "event "
    //         << i
    //         << "  "
    //         << "|tC'-tH'| = "
    //         << diff
    //         << " ns";


    //     if(diff <= 240.0){
    //         std::cout << "  OK";
    //     }
    //     else{
    //         std::cout << "  NG";
    //     }

    //     std::cout << std::endl;
    // }
// }

int main(){
    ConbineDetecters conbine_detecters;
    conbine_detecters.test();

    return 0;
}