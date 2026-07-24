

struct H2gcTime{
    std::array<UInt_t, 2> timestamps_r;
};

struct CitiTime{
    int pcnt;
    int fcnt;
};

void conbine_test4(){
    TFile* hfile = TFile::Open("/home/motohashi/work/analysis_raris202607/data/Run343.root");
    TFile* cfile = TFile::Open("/home/motohashi/work/analysis_raris202607/data/Run0343_HV16_VF448_448_x10_090734.761Monitor5/T5_Sci.root");
    TFile* exfile = TFile::Open("/home/motohashi/work/analysis_raris202607/result/xydata/Run0343_HV16_VF448_448_x10_090734.761Monitor5/T5_xy.root");

    TTree* htree = (TTree*)hfile->Get("events");
    TTree* ctree = (TTree*)cfile->Get("t");
    TTree* extree = (TTree*)exfile->Get("t");

    H2gcTime h2gc_time;
    CitiTime citi_time;
    int pcnt_ex;
    int fcnt_ex;

    htree->SetBranchAddress("timestamps_r", &h2gc_time.timestamps_r);
    ctree->SetBranchAddress("pcnt", &citi_time.pcnt);
    ctree->SetBranchAddress("fcnt", &citi_time.fcnt);
    extree->SetBranchAddress("pcnt", &pcnt_ex);
    extree->SetBranchAddress("fcnt", &fcnt_ex);

    Long64_t htree_entries = htree->GetEntries();
    Long64_t ctree_entries = ctree->GetEntries();
    Long64_t extree_entries = extree->GetEntries();

    htree->GetEntry(0);
    double tH0 = h2gc_time.timestamps_r[0] * 25;

    ctree->GetEntry(0);
    double tC0 = citi_time.pcnt * 50000000 + citi_time.fcnt * 240;

    extree->GetEntry(0);
    double tEx0 = pcnt_ex * 50000000 + fcnt_ex * 240;

    htree->GetEntry(1);
    double tH1 = h2gc_time.timestamps_r[0] * 25;

    ctree->GetEntry(1);
    double tC1 = citi_time.pcnt * 50000000 + citi_time.fcnt * 240;

    extree->GetEntry(1);
    double tEx1 = pcnt_ex * 50000000 + fcnt_ex * 240;

    double dH = tH1 - tH0;
    double dC = tC1 - tC0;
    double dEx = tEx1 - tEx0;

    std::cout << "First Entry dH: " << dH << " dC: " << dC << " dEx: " << dEx << std::endl;
}