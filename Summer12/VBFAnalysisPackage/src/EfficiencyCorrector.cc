#include "EfficiencyCorrector.h"

EfficiencyCorrector::EfficiencyCorrector(std::string& eleEffFileName,
                			                   std::string& muEffFileName)
{

  periodNames.push_back("0");
//  periodNames.push_back("1");

  inFile_eleEff = TFile::Open(eleEffFileName.c_str(),"READ");
  inFile_muEff  = TFile::Open(muEffFileName.c_str(),"READ");
  
  for ( int iPeriod = 0; iPeriod < nPeriodsEle; iPeriod++ ) {
    // ele
    eleEffReco[iPeriod] = (TH2F*)( inFile_eleEff->Get("eleScaleFactorReco"+periodNames[iPeriod]) );
    eleEffIso[iPeriod] = (TH2F*)( inFile_eleEff->Get("eleScaleFactorIso"+periodNames[iPeriod]) );
    eleEffHlt[iPeriod] = (TH2F*)( inFile_eleEff->Get("eleScaleFactorHLT"+periodNames[iPeriod]) );    
  }

  for ( int iPeriod = 0; iPeriod < nPeriodsMu; iPeriod++ ) {
    // mu
    muEffIso[iPeriod] = (TH2F*)( inFile_muEff->Get("muScaleFactorIso"+periodNames[iPeriod]) );
    muEffHlt[iPeriod] = (TH2F*)( inFile_muEff->Get("muScaleFactorHLT"+periodNames[iPeriod]) );
  }

}

EfficiencyCorrector::~EfficiencyCorrector()
{
  
  for ( int iPeriod = 0; iPeriod < nPeriodsEle; iPeriod++ ) {
    // ele
    delete[] eleEffReco[iPeriod];
    delete[] eleEffIso[iPeriod];
    delete[] eleEffHlt[iPeriod];
  }

  for ( int iPeriod = 0; iPeriod < nPeriodsMu; iPeriod++ ) {
    // mu
    delete[] muEffIso[iPeriod];
    delete[] muEffHlt[iPeriod];
  }

  inFile_eleEff -> Close();
  inFile_muEff -> Close();

}

void EfficiencyCorrector::setPeriodEle(int iPeriod) { thisPeriodEle = iPeriod; }
void EfficiencyCorrector::setPeriodMu(int iPeriod) { thisPeriodMu = iPeriod; }
  
float EfficiencyCorrector::getEleEff(float pt, float eta, const std::string& mode) {

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

float EfficiencyCorrector::getMuEff(float pt, float eta, const std::string& mode) {

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
