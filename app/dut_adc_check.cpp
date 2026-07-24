#include "./../include/sci_map.h"

#include <array>

#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TCanvas.h>

int main(int argc, char** argv){
    int target_sci = std::stoi(argv[2]);

    TFile* file = TFile::Open(argv[1], "READ");
    TTree* tree = (TTree*)file->Get("events");

    std::array<UInt_t, 288> hit_max;
    std::array<UInt_t, 288> hit_pedestal;

    tree->SetBranchAddress("hit_max", hit_max.data());

    Long64_t entries = tree->GetEntries();

    TH1D* hist_full = new TH1D("hist_full", "hist_full", 100, 0, 300);
    TH1D* hist_3by3 = new TH1D("hist_3by3", "hist_3by3", 100, 0, 300);
    TH1D* hist_1by1 = new TH1D("hist_1by1", "hist_1by1", 100, 0, 300);

    for(Long64_t i=0; i<entries; i++){
        tree->GetEntry(i);

        std::array<int, 16> sci_hit_max = {0};
        std::array<int, 16> sci_hit_pedestal = {0};

        for(int ch=0; ch<288; ch++){
            sci_hit_max[ch_to_sci(ch)] += hit_max[ch];
            sci_hit_pedestal[ch_to_sci(ch)] += hit_pedestal[ch];
        }

        int max_sci_id = -1;
        int max_sci_adc = -1;
        for(int sci=1; sci<=16; sci++){
            int value = sci_hit_max[sci];
            if(value > max_sci_adc){
                max_sci_adc = value;
                max_sci_id = sci;
            }
            hist_full->Fill(value);
            if(sci == target_sci) hist_1by1->Fill(value);
            if(is_near_sci(target_sci, sci)) hist_3by3->Fill(value);
        }
    }

    TCanvas* c1 = new TCanvas("c1", "c1", 800, 600);
    hist_full->Draw();

}