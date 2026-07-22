#ifndef BM_HIT_MATCHING_H
#define BM_HIT_MATCHING_H


#include <vector>
#include <string>

#include <TFile.h>
#include <TTree.h>


struct EventID{
    int pcnt;
    int fcnt;

    bool operator<(const EventID& rhs) const{
        if(pcnt != rhs.pcnt)
            return pcnt < rhs.pcnt;

        return fcnt < rhs.fcnt;
    }
};


struct XYHit{

    int pcnt;
    int fcnt;

    int nx;
    int ny;

    double x;
    double y;

};



struct MatchedHit{

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



struct HitMatch{

    TFile* f3;
    TFile* f4;
    TFile* f5;

    TTree* t3;
    TTree* t4;
    TTree* t5;

    XYHit xy3;
    XYHit xy4;
    XYHit xy5;

    Long64_t nentries3;
    Long64_t nentries4;
    Long64_t nentries5;

};



#endif