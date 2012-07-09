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
    EfficiencyCorrector(std::string&, std::string&);
    ~EfficiencyCorrector();
        
    void  setPeriodEle(int thisPeriod);
    void  setPeriodMu(int thisPeriod);
    float getEleEff(float pt, float eta, const std::string& mode);
    float getMuEff(float pt, float eta, const std::string& mode);
    
  private:    
    //DATA MEMBERS
    std::vector<TString> periodNames;
    
    TFile* inFile_eleEff;
    TFile* inFile_muEff;
    
    static const int nPeriodsEle = 1;
    static const int nPeriodsMu = 1;
    TH2F* eleEffReco[nPeriodsEle];
    TH2F* eleEffIso[nPeriodsEle];
    TH2F* eleEffHlt[nPeriodsEle];

    TH2F* muEffIso[nPeriodsMu];
    TH2F* muEffHlt[nPeriodsMu];
    
    int thisPeriodEle;
    int thisPeriodMu;
    
};

#endif

