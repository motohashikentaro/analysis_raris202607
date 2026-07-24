#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TCanvas.h>

int main(int argc, char* argv[]){

    TFile* f = TFile::Open(argv[1], "READ");
    TTree* t = (TTree*)f->Get("events");

    int adc[288][16];
    t->SetBranchAddress("adc", adc);

    std::cout << "SetBranch OK" << std::endl;

    bool used[288] = {false};

    Long64_t nentries = t->GetEntries();

    for(Long64_t entry=0; entry<nentries; entry++){
        t->GetEntry(entry);

        for(int ch=0; ch<288; ch++){
            for(int sample=0; sample<16; sample++){
                if(adc[ch][sample] != 0){
                    used[ch] = true;
                    break;
                }
            }
        }
    }

    TH1D* h = new TH1D("h",
        "Used Channel;Channel;Used",
        288, 0.5, 288.5);

    for(int ch=0; ch<288; ch++){
        if(used[ch]){
            h->SetBinContent(ch+1, 1);
        }
    }

    

    h->SetMinimum(0);
    h->SetMaximum(1.2);
    h->SetStats(0);
    h->SetFillColor(kAzure+1);

    TCanvas* c = new TCanvas("c","c",1200,400);
    h->Draw("HIST");

    c->SaveAs("./../result/used_channel.png");

    return 0;
}

// #include <iostream>

// #include <TFile.h>
// #include <TTree.h>

// int main(int argc, char* argv[]) {

//     TFile* f = TFile::Open(argv[1], "READ");
//     TTree* t = (TTree*)f->Get("events");

//     int adc[288][16];
//     t->SetBranchAddress("adc", adc);

//     bool used[288] = {false};

//     Long64_t nentries = t->GetEntries();

//     for(Long64_t entry = 0; entry < nentries; entry++) {
//         t->GetEntry(entry);

//         for(int ch = 0; ch < 288; ch++) {
//             for(int sample = 0; sample < 16; sample++) {
//                 if(adc[ch][sample] != 0) {
//                     used[ch] = true;
//                     break;
//                 }
//             }
//         }
//     }

//     std::cout << "Used channels\n";
//     for(int ch = 0; ch < 288; ch++) {
//         if(used[ch])
//             std::cout << ch << std::endl;
//     }

//     std::cout << "\nUnused channels\n";
//     for(int ch = 0; ch < 288; ch++) {
//         if(!used[ch])
//             std::cout << ch << std::endl;
//     }

//     return 0;
// }

