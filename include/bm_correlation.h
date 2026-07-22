#ifndef BM_CORRELATION_H
#define BM_CORRELATION_H

#include <string>

#include <TFile.h>
#include <TTree.h>

struct CorrelaData{
    int pcnt;
    int fcnt;
    double x3;
    double y3;
    double x4;
    double y4;
    double x5;
    double y5;
    int nx3;
    int ny3;
    int nx4;
    int ny4;
    int nx5;
    int ny5;
};

class BmCorrela{
    public:
        BmCorrela(const char* filepath);
        ~BmCorrela();

        void Hitmap();
        void XyCorrelation();

    private:
        CorrelaData data_;
        TFile* file_;
        TTree* tree_;
        Long64_t nentries_;
        std::string run_name_;
};



#endif