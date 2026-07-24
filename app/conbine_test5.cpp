

void conbine_test5(){
    TFile* hfile = TFile::Open("/home/motohashi/work/analysis_raris202607/data/Run340.root");
    TFile* cfile = TFile::Open("/home/motohashi/work/analysis_raris202607/data/Run0340_HV16_VF448_448_x10_085844.701Monitor5/T5_Sci.root");

    TTree* htree = (TTree*)hfile->Get("events");
    TTree* ctree = (TTree*)cfile->Get("t");

    std::array<UInt_t, 2> timestamps_r;
    int pcnt;
    int fcnt;

    htree->SetBranchAddress("timestamps_r", &timestamps_r);
    ctree->SetBranchAddress("pcnt", &pcnt);
    ctree->SetBranchAddress("fcnt", &fcnt);

    Long64_t htree_entries = htree->GetEntries();
    Long64_t ctree_entries = ctree->GetEntries();

    htree->GetEntry(0);
    double tH0 = timestamps_r[0] * 25;

    ctree->GetEntry(0);
    double tC0 = pcnt * 50000000 + fcnt * 240;

    TGraph* graph1 = new TGraph();

    // for(Long64_t iH=0; iH<htree_entries; iH++){
        int iH = 10120;
        htree->GetEntry(iH);
        double tHi = timestamps_r[0] * 25;

        double tHi_p = tHi - tH0;

        for(Long64_t iC=0; iC<ctree_entries; iC++){
            ctree->GetEntry(iC);
            double tCi = pcnt * 50000000 + fcnt * 240;
            
            double tCi_p = tCi - tC0;

            double diff = std::abs(tCi_p - tHi_p);

            graph1->SetPoint(graph1->GetN(), iC, diff);
        }
    // }

    TCanvas* c1 = new TCanvas("c1", "c1", 800, 600);
    graph1->SetMarkerStyle(20);
    graph1->SetMarkerSize(1.0);
    graph1->SetTitle(Form("tCi_p - tHi(%d);Event Index;Time Difference [ns]", iH));
    graph1->Draw("AP");
    graph1->GetYaxis()->SetRangeUser(0, 10000);

}