#ifndef BM_HIT_MATCHING_H
#define BM_HIT_MATCHING_H

#include <vector>

#include <TFile.h>
#include <TTree.h>

struct HitId{
    int pcnt;  // rough count
    int fcnt;  // fine count
};

struct HitFeature{
    HitId id;
    int nhits; // number of hits
    std::vector<int>* channel = nullptr;
    std::vector<int>* adc = nullptr;
    int adc_max;
    std::vector<int>* ix = nullptr;
    std::vector<int>* iy = nullptr;
};

class HitMatch{
    public:
        void BranchExtract();
        void HitMatching();
        bool IsMatch();
        bool IsLess(const HitId& a, const HitId& b);
        void CreateMatchFile();
        void SaveMatchFile();

    private:
        HitFeature bm3;
        HitFeature bm4;
        HitFeature bm5;
        TFile* f3;
        TFile* f4;
        TFile* f5;
        TTree* t3;
        TTree* t4;
        TTree* t5;
        Long64_t nentries3;
        Long64_t nentries4;
        Long64_t nentries5;

        TFile* match_file;
        TTree* match_tree;
};




#endif