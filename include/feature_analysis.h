#ifndef FEAUTURE_ANALYSIS_H
#define FEAUTURE_ANALYSIS_H

#include <./../include/dut_ana.h>

#include <string>

#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>

class FeatureAnalysis{
    public:
        FeatureAnalysis(char* filename);
        ~FeatureAnalysis();

        TH1D* PeakAdcDistro(int ch);
        TH1D* PeakTimeDistro(int ch);
        TH1D* PedestalDistro(int ch);
        std::vector<TH1D*> RaiseTimeDistro(int ch);
        std::vector<TH1D*> FallTimeDistro(int ch);
        std::vector<TH1D*> ChargeDistro(int ch);
        std::vector<TH1D*> TotDistro(int ch);

        std::string run_number_;

    private:
        OutputFeature feature_;
        TFile* file_;
        TTree* tree_;
        Long64_t nentries_;
        
};

#endif