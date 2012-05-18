#ifndef EfficiencyCorrector_h
#define EfficiencyCorrector_h

#include <iostream>
#include <vector>
#include <string>

#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"

class EfficiencyCorrector{
  
  public:
    EfficiencyCorrector(std::string&, std::string&, std::string&, std::string&, std::string&);
    ~EfficiencyCorrector();
        
    void  setPeriodEle(int thisPeriod);
    void  setPeriodMu(int thisPeriod);
    float getMetEff(float met);
    float getMtEff(float mt);
    float getJetEff(std::vector<float>& jet_pt, std::vector<float>& jet_eta);
    float getEleEff(float pt, float eta, const std::string& mode);
    float getMuEff(float pt, float eta, const std::string& mode);
    
  private:    
    //DATA MEMBERS
    std::vector<TString> periodNames;
    
    TFile* inFile_eleEff;
    TFile* inFile_muEff;
    TFile* inFile_metEff;
    TFile* inFile_jetEff;
    TFile* inFile_mtEff;
    
    static const int nPeriodsEle = 5;
    static const int nPeriodsMu = 3;
    TH2F* eleEffReco[nPeriodsEle];
    TH2F* eleEffIso[nPeriodsEle];
    TH2F* muEffIso[nPeriodsMu];
    TH2F* eleEffHlt[nPeriodsEle];
    TH2F* mtEffHlt[nPeriodsEle];
    TH2F* muEffHlt[nPeriodsMu];
    
    TH1F* metEffHlt[nPeriodsEle];
    TH2F* jetEffHlt[nPeriodsEle];
    
    int thisPeriodEle;
    int thisPeriodMu;
    
};

#endif

