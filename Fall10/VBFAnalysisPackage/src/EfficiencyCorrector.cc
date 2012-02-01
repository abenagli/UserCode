#include "EfficiencyCorrector.h"

EfficiencyCorrector::EfficiencyCorrector(std::string& eleEffFileName,
                			                   std::string& muEffFileName,
                                         std::string& metEffFileName,
                                         std::string& jetEffFileName)
{

  periodNames.push_back("0");
  periodNames.push_back("1");
  periodNames.push_back("2");
  periodNames.push_back("3");
  periodNames.push_back("4");

  inFile_eleEff = TFile::Open(eleEffFileName.c_str(),"READ");
  inFile_muEff = TFile::Open(muEffFileName.c_str(),"READ");
  inFile_metEff = TFile::Open(metEffFileName.c_str(),"READ");
  inFile_jetEff = TFile::Open(jetEffFileName.c_str(),"READ");
  
  for ( int iPeriod = 0; iPeriod < nPeriodsEle; iPeriod++ ) {
    // ele
    eleEffReco[iPeriod] = (TH2F*)( inFile_eleEff->Get("eleEffReco"+periodNames[iPeriod]) );
    eleEffIso[iPeriod] = (TH2F*)( inFile_eleEff->Get("eleEffIso"+periodNames[iPeriod]) );
    eleEffHlt[iPeriod] = (TH2F*)( inFile_eleEff->Get("eleEffHLT"+periodNames[iPeriod]) );    
    // met
    metEffHlt[iPeriod] = (TH1F*)( inFile_metEff->Get("metEffHLT"+periodNames[iPeriod]) );
    // jet
    jetEffHlt[iPeriod] = (TH2F*)( inFile_jetEff->Get("jetEffHLT"+periodNames[iPeriod]) );
  }

  for ( int iPeriod = 0; iPeriod < nPeriodsMu; iPeriod++ ) {
    // mu
    muEffIso[iPeriod] = (TH2F*)( inFile_muEff->Get("muEffIso"+periodNames[iPeriod]) );
    muEffHlt[iPeriod] = (TH2F*)( inFile_muEff->Get("muEffHLT"+periodNames[iPeriod]) );
  }

}

EfficiencyCorrector::~EfficiencyCorrector()
{
  
  for ( int iPeriod = 0; iPeriod < nPeriodsEle; iPeriod++ ) {
    // ele
    delete[] eleEffReco[iPeriod];
    delete[] eleEffIso[iPeriod];
    delete[] eleEffHlt[iPeriod];
    // met
    delete[] metEffHlt[iPeriod];
    // jet
    delete[] jetEffHlt[iPeriod];
  }

  for ( int iPeriod = 0; iPeriod < nPeriodsMu; iPeriod++ ) {
    // mu
    delete[] muEffIso[iPeriod];
    delete[] muEffHlt[iPeriod];
  }

  inFile_eleEff -> Close();
  inFile_muEff -> Close();
  inFile_metEff -> Close();
  inFile_jetEff -> Close();

}

void EfficiencyCorrector::setPeriodEle(int iPeriod) { thisPeriodEle = iPeriod; }
void EfficiencyCorrector::setPeriodMu(int iPeriod) { thisPeriodMu = iPeriod; }
  
float EfficiencyCorrector::getMetEff(float met) {
 
  // boundaries check
  float xMin = metEffHlt[thisPeriodEle] -> GetXaxis() -> GetXmin();
  float xMax = metEffHlt[thisPeriodEle] -> GetXaxis() -> GetXmax();
  if ( met < xMin ) met = xMin; //underflow
  if ( met > xMax ) met = xMax - 0.001; //overflow
  int thisBin = metEffHlt[thisPeriodEle] -> FindBin(met);  
  return metEffHlt[thisPeriodEle] -> GetBinContent(thisBin);    
  
}

float EfficiencyCorrector::getJetEff(std::vector<float>& jet_pt, std::vector<float>& jet_eta) {

  float xMin = jetEffHlt[thisPeriodEle] -> GetXaxis() -> GetXmin();
  float xMax = jetEffHlt[thisPeriodEle] -> GetXaxis() -> GetXmax();

  // if there are less than 2 offline jets return zero as jet HLT efficiency
  int nJets = jet_pt.size();
  if ( nJets < 2 ) return 0;
  
  std::vector<float> theSingleJetNotEff;
  // hardcoded way to get efficiency inside the good turn on range [10,100] + find the efficiencies
  for ( unsigned int iJet = 0; iJet < jet_pt.size(); iJet++ ) {
    // boundaries check
    if ( jet_pt[iJet] < xMin ) jet_pt[iJet] = xMin; //underflow
    if ( jet_pt[iJet] > xMax ) jet_pt[iJet] = xMax - 0.001; //overflow
    int thisBin = jetEffHlt[thisPeriodEle] -> FindBin( jet_pt[iJet], jet_eta[iJet] );
    theSingleJetNotEff.push_back( 1 - jetEffHlt[thisPeriodEle] -> GetBinContent(thisBin) );
  }
  // use as reference the formula reported at sec 8.1 number 7
  float secondTerm = 1.;
  float thirdTerm = 0.;
  for ( unsigned int iJet = 0; iJet < jet_pt.size(); iJet++ ) {
    secondTerm *= theSingleJetNotEff[iJet];
    float thirdTermAuxiliary = 1.;
    for ( unsigned int lJet = 0; lJet < jet_pt.size(); lJet++ ) if ( iJet != lJet ) thirdTermAuxiliary *= theSingleJetNotEff[lJet];
    thirdTerm += thirdTermAuxiliary;
  }
  return 1 + (nJets-1)*secondTerm - thirdTerm;
          
}

float EfficiencyCorrector::getEleEff(float pt, float eta, std::string& mode) {

  // boundaries check
  float xMin = eleEffReco[thisPeriodEle] -> GetXaxis() -> GetXmin();
  float xMax = eleEffReco[thisPeriodEle] -> GetXaxis() -> GetXmax();
  if ( eta < xMin ) eta = xMin; //underflow
  if ( eta > xMax ) eta = xMax - 0.001; //overflow
  float yMin = eleEffReco[thisPeriodEle] -> GetYaxis() -> GetXmin();
  float yMax = eleEffReco[thisPeriodEle] -> GetYaxis() -> GetXmax();
  if ( pt < yMin ) pt = yMin; //underflow
  if ( pt > yMax ) pt = yMax - 0.001; //overflow
  
  // default values in case of no correction
  float theRecoEff = 1.;
  float theIsoEff = 1.;
  float theHltEff = 1.;
  float theTotalEff = 1.;
  int thisBin = -1;
  
  thisBin = eleEffReco[thisPeriodEle] -> FindBin(eta, pt);
  theRecoEff = eleEffReco[thisPeriodEle] -> GetBinContent(thisBin);
  thisBin = eleEffIso[thisPeriodEle] -> FindBin(eta, pt);
  theIsoEff = eleEffIso[thisPeriodEle] -> GetBinContent(thisBin);
  thisBin = eleEffHlt[thisPeriodEle] -> FindBin(eta, pt);
  theHltEff = eleEffHlt[thisPeriodEle] -> GetBinContent(thisBin);
  
  // return the correct efficiency correction
  if ( mode.find("R") >= 0 ) theTotalEff *= theRecoEff;
  if ( mode.find("I") >= 0 ) theTotalEff *= theIsoEff;
  if ( mode.find("H") >= 0 ) theTotalEff *= theHltEff;
  
  return theTotalEff;
    
}

float EfficiencyCorrector::getMuEff(float pt, float eta, std::string& mode) {

  // boundaries check
  float xMin = muEffIso[thisPeriodMu] -> GetXaxis() -> GetXmin();
  float xMax = muEffIso[thisPeriodMu] -> GetXaxis() -> GetXmax();
  if ( eta < xMin ) eta = xMin; //underflow
  if ( eta > xMax ) eta = xMax - 0.001; //overflow
  float yMin = muEffIso[thisPeriodMu] -> GetYaxis() -> GetXmin();
  float yMax = muEffIso[thisPeriodMu] -> GetYaxis() -> GetXmax();
  if ( pt < yMin ) pt = yMin; //underflow
  if ( pt > yMax ) pt = yMax - 0.001; //overflow
  
  // default values in case of no correction
  float theRecoEff = 1.;
  float theIsoEff = 1.;
  float theHltEff = 1.;
  float theTotalEff = 1.;
  int thisBin = -1;
  
  thisBin = muEffIso[thisPeriodMu] -> FindBin(eta, pt);
  theIsoEff = muEffIso[thisPeriodMu] -> GetBinContent(thisBin);
  thisBin = muEffHlt[thisPeriodMu] -> FindBin(eta, pt);
  theHltEff = muEffHlt[thisPeriodMu] -> GetBinContent(thisBin);
  
  // return the correct efficiency correction
  if ( mode.find("R") >= 0 ) theTotalEff *= theRecoEff;
  if ( mode.find("I") >= 0 ) theTotalEff *= theIsoEff;
  if ( mode.find("H") >= 0 ) theTotalEff *= theHltEff;
  
  return theTotalEff;
  
}
