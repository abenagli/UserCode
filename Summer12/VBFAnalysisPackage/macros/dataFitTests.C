#include "../interface/Functions.h"

double xMin = 0.;
double xMax = 1000.;
double xWidth = 5;
int nBins = int((xMax-xMin)/xWidth);
double xFitMIN1 = 225.;
double xFitMAX2 = 700.;

double fitFunc(double* x, double* par);
std::string fitMethod = "doubleExponential";
int nPar = 4;






void dataFitTests(const std::string& inFileName)
{
  TFile* f = TFile::Open(inFileName.c_str(),"READ");
  f -> cd();
  
  TTree* t = (TTree*)( f->Get("ntu_15") );
  TH1F* h = new TH1F("h","",nBins,xMin,xMax);
  h -> Sumw2();
  
  TCanvas* c1 = new TCanvas("c1","");
  c1 -> cd();
  
  t -> Draw("lepNuW_m_KF >>+ h");
  c1 -> Update();
  
  
  
  TF1* func = new TF1("func",fitFunc,0.,1000.,nPar);
  if( fitMethod == "doubleExponential" )
  {
    //-------------------
    // double exponential
    func -> SetParameter(0,10.);
    func -> SetParameter(1,0.015);
    func -> SetParameter(2,9.8);
    func -> SetParameter(3,0.011);
    
    func -> SetParLimits(0,-100.,100.);
    func -> SetParLimits(1,0.,1.);
    func -> SetParLimits(2,-100.,100.);
    func -> SetParLimits(3,0.,1.);
    
    func -> SetParName(0,"N1");
    func -> SetParName(1,"#lambda1");
    func -> SetParName(2,"N2");
    func -> SetParName(3,"#lambda2");
  }
  
  if( fitMethod == "attenuatedDoubleExponential" )
  {
    //------------------------------
    // attenuated double exponential
    
    func -> SetParameter(0,190.);
    func -> SetParameter(1,20.);
    func -> SetParameter(2,10.);
    func -> SetParameter(3,0.013);
    func -> SetParameter(4,5.);
    func -> SetParameter(5,0.005);
    
    func -> SetParLimits(0,50.,500.);
    func -> SetParLimits(1,0.,100.);
    func -> SetParLimits(2,0.,100.);
    func -> SetParLimits(3,0.,1.);
    func -> SetParLimits(4,0.,100.);
    func -> SetParLimits(5,0.,1.);
    
    func -> SetParName(0,"#mu");
    func -> SetParName(1,"kT");
    func -> SetParName(2,"N1");
    func -> SetParName(3,"#lambda1");
    func -> SetParName(4,"N2");
    func -> SetParName(5,"#lambda2");
  }
  

  
  TVirtualFitter::SetDefaultFitter("Minuit2");
  func -> SetNpx(10000);
  func -> SetLineWidth(2);
  func -> SetLineColor(kRed);
  int counter = 0;
  while( counter < 10  )
  {
    TFitResultPtr fitResultPtr = h -> Fit("func","LRS+","",xFitMIN1,xFitMAX2);
    int fitStatus = (int)(fitResultPtr);
    if( fitStatus == 0 ) break;
    ++counter;
  }
  std::cout << "fitStatus: " << fitStatus << "   counter: " << counter << std::endl;
}






double fitFunc(double* x, double* par)
{
  // variable
  double xx = x[0];
  
  if( (fitMethod == "doubleExponential") || (fitMethod == "doubleExponentialNoHoles") )
    return doubleExponential(x,par);
  if( (fitMethod == "attenuatedDoubleExponential") || (fitMethod == "attenuatedDoubleExponentialNoHoles") )
    return attenuatedDoubleExponential(x,par);
  else
    return -1.;
}
