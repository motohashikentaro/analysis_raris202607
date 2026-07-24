#ifndef DUT_ANA_H
#define DUT_ANA_H

#include <array>
#include <string>

#include <TFile.h>
#include <TTree.h>

struct DutData{
    UInt_t event_number;
    std::array<UInt_t, 2> timestamps;
    std::array<UInt_t, 2> timestamps_r;
    UInt_t num_samples;
    std::array<std::array<UInt_t, 16>, 288> adc;
    std::array<std::array<UInt_t, 16>, 288> toa;
    std::array<std::array<UInt_t, 16>, 288> tot;
    std::array<std::array<UInt_t, 16>, 288> hamming;
    std::array<UInt_t, 288> hit_max;
    std::array<UInt_t, 288> hit_pedestal;
};

inline constexpr std::array<int, 3> thresholds = {50,
                                                  100,
                                                  150,};

inline constexpr size_t nthres = thresholds.size();

inline constexpr int nch = 288;

struct OutputFeature{
    UInt_t event_number;
    std::array<UInt_t, 2> timestamps;
    std::array<UInt_t, 2> timestamps_r;

    std::array<double, nch> peak_adc;
    std::array<UInt_t, nch> peak_time;
    std::array<UInt_t, nch> pedestal;

    std::array<std::array<double, nch>, nthres> raise_times;
    std::array<std::array<double, nch>, nthres> fall_times;
    std::array<std::array<double, nch>, nthres> charges;
    std::array<std::array<double, nch>, nthres> tots;
};

class DutAna{
    public:
        DutAna(char* filename);
        ~DutAna();

        void ExtractFeatures();

    private:
        DutData dut_data_;
        TFile* file_;
        TTree* tree_;
        Long64_t nentries_;

        std::string run_number_;
};

#endif