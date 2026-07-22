#ifndef BM_XY_MATCHING_H
#define BM_XY_MATCHING_H

#include <vector>
#include <string>

#include <TFile.h>
#include <TTree.h>

struct HitId{

    int pcnt;
    int fcnt;

};

struct HitFeature{

    HitId id;

    int nhits;

    std::vector<int>* channel = nullptr;
    std::vector<int>* adc = nullptr;

    int adc_max;

    std::vector<int>* ix = nullptr;
    std::vector<int>* iy = nullptr;

};

struct XYHit{

    int pcnt;
    int fcnt;

    int nx;
    int ny;

    double x;
    double y;

    int adc_x;
    int adc_y;

};

struct HitMatch{

    TFile* f3;
    TFile* f4;
    TFile* f5;

    TTree* t3;
    TTree* t4;
    TTree* t5;

    Long64_t nentries3;
    Long64_t nentries4;
    Long64_t nentries5;

    HitFeature bm3;
    HitFeature bm4;
    HitFeature bm5;

};

void MakeXYRoot(
    TTree* tin,
    HitFeature& hit,
    Long64_t nentries,
    const std::string& outfile
);

#endif