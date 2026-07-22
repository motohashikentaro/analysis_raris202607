#ifndef GAIN_CALIBRATION_H
#define GAIN_CALIBRATION_H

#include <vector>
#include <string>

#include <TFile.h>
#include <TTree.h>

struct GainData{
    std::vector<int>* adc = nullptr;
    int adc_max;
    std::vector<int>* ix = nullptr;
    std::vector<int>* iy = nullptr;
};

struct FileData{
    TFile* file;
    TTree* tree;
    Long64_t nentries;
    GainData gd;
};

enum Layer {
    BM3_X = 0,
    BM3_Y = 1,
    BM4_X = 2,
    BM4_Y = 3,
    BM5_X = 4,
    BM5_Y = 5
};

class GainCalibration{
public:
    GainCalibration(char* input_path);
    ~GainCalibration();

    void BranchExtraction(FileData& data);
    FileData* GetFileData(int nlayer);
    void AdcPlot(int nlayer, int nstrip);

private:
    FileData data3_;
    FileData data4_;
    FileData data5_;

    std::string run_dir_;
    std::string run_number_;
};

#endif