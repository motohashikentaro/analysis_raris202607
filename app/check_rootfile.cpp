#include <array>
#include <iostream>
#include <algorithm>

#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>
#include <TH1.h>
#include <TCanvas.h>

struct StructureData{
    int nsipm = 288;
    int nsample = 16;
};

struct EventData{
    UInt_t event_number;
    UInt_t timestamps[2];
    UInt_t timestamps_r[2];
    UInt_t num_samples;
    UInt_t adc[288][16];
    int toa[288][16];
    int tot[288][16];
    int hamming[288][16];
    UInt_t hit_max[288];
    UInt_t hit_pedestal[288];
};

int main(int argc, char* argv[]){
    TFile* file = TFile::Open(argv[1]);
    TTree* tree = (TTree*)file->Get("events");

    StructureData sd;
    EventData ed;
    tree->SetBranchAddress("event_number", &ed.event_number);
    tree->SetBranchAddress("timestamps", &ed.timestamps);
    tree->SetBranchAddress("timestamps_r", &ed.timestamps_r);
    tree->SetBranchAddress("num_samples", &ed.num_samples);
    tree->SetBranchAddress("adc", &ed.adc);
    tree->SetBranchAddress("toa", &ed.toa);
    tree->SetBranchAddress("tot", &ed.tot);
    tree->SetBranchAddress("hamming", &ed.hamming);
    tree->SetBranchAddress("hit_max", &ed.hit_max);
    tree->SetBranchAddress("hit_pedestal", &ed.hit_pedestal);

    Long64_t nentries = tree->GetEntries();

    TCanvas* canvas = new TCanvas("canvas", "Event Data", 800, 600);

    // TGraph* waveform = new TGraph();

    std::array<TGraph*, 500> waveforms;
    for(int i=0; i<500; i++){
        waveforms[i] = new TGraph();
    }

    int toa_f = 0;

    for(Long64_t i=0; i<nentries; i++){
        tree->GetEntry(i);

        // if(i != std::stoi(argv[2])) continue; // Only process the specified event
        for(int sipm=0; sipm<sd.nsipm; sipm++){

            // if(sipm != std::stoi(argv[3])) continue; // Only process the specified SiPM
            // for(int sample=0; sample<sd.nsample; sample++){
            //     waveform->SetPoint(waveform->GetN(), sample, static_cast<Double_t>(ed.adc[sipm][sample]) - static_cast<Double_t>(ed.hit_pedestal[sipm]));
            //     std::cout << "Event: " << ed.event_number 
            //             << ", SiPM: " << sipm 
            //             << ", Sample: " << sample 
            //             << ", ADC: " << ed.adc[sipm][sample] 
            //             << ", TOA: " << ed.toa[sipm][sample] 
            //             << ", TOT: " << ed.tot[sipm][sample] 
            //             << ", Hamming: " << ed.hamming[sipm][sample] 
            //             << ", Hit Max: " << ed.hit_max[sipm] 
            //             << ", Hit Pedestal: " << ed.hit_pedestal[sipm] 
            //             << std::endl;
            // }

            

            // if(std::any_of(std::begin(ed.toa[sipm]) + 1, std::end(ed.toa[sipm]), [](int toa_value){ return toa_value != 0; })){
            //     std::cout << "Event: " << ed.event_number 
            //             << ", SiPM: " << sipm 
            //             << ", TOA[0]: " << ed.toa[sipm][0] 
            //             << ", TOA[1]: " << ed.toa[sipm][1] 
            //             << std::endl;
            //     toa_f++;
            // }

            if(std::any_of(std::begin(ed.toa[sipm]), std::end(ed.toa[sipm]), [](int toa_value){ return toa_value != 0; })){
                for(int sample=0; sample<sd.nsample; sample++){
                    waveforms[toa_f]->SetPoint(waveforms[toa_f]->GetN(), sample, static_cast<Double_t>(ed.adc[sipm][sample]) - static_cast<Double_t>(ed.hit_pedestal[sipm]));
                }            
                toa_f++;
                if(toa_f >= 500) break; // Prevent exceeding the array size
            }
        }

    }

    std::cout << "Total events with matching TOA: " << toa_f << std::endl;

    for(int i=0; i<toa_f; i++){
        waveforms[i]->Draw("AL");
    }
    canvas->SaveAs("../result/waveform.png");
}