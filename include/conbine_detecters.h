#ifndef CONBINE_DETECTERS_H
#define CONBINE_DETECTERS_H

#include <array>

#include <TFile.h>
#include <TTree.h>

struct H2gcTime{
    std::array<UInt_t, 2> timestamps_r;
};

struct CitiTime{
    int pcnt;
    int fcnt;
};

class ConbineDetecters{
    public:
        ConbineDetecters();
        ~ConbineDetecters();

        void test();
    private:
        H2gcTime h2gc_time;
        CitiTime citi_time;
        TFile* hfile;
        TTree* htree;
        TFile* cfile;
        TTree* ctree;
        Long64_t htree_entries;
        Long64_t ctree_entries;
};

#endif