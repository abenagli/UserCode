#ifndef hColl_h
#define hColl_h

#include <TH1.h>
#include <TFile.h>
#include <THStack.h>
#include <vector>
#include <string>
#include <cmath>


template <class THisto>
void BinLogX (THisto* h)
{
 TAxis* axis = h -> GetXaxis();
 int bins = axis -> GetNbins();

 float from = axis -> GetXmin();
 float to = axis -> GetXmax();
 float width = (to - from) / bins;
 float* new_bins = new float[bins + 1] ;

 for(int i = 0; i <= bins; ++i)
   new_bins[i] = pow (10, from + i*width);

 axis -> Set(bins,new_bins);
 delete new_bins;
}


//PG --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


struct hColl
{
  hColl (std::string pN, int b, double m, double M, bool logBins = 0) ;
  hColl (std::string pN, int b, Float_t * xAxis) ;
  ~hColl () ;
  TH1F * addSample (std::string sampleName) ;
  void save (TFile * f) ;
  THStack makeStack () ;
  TH1F * findHisto (std::string name) ;
  void normToBinWidths () ;

  std::string plotName ;
  int bins ;
  double min ;
  double max ;
  Float_t * axis ;
  bool logB ;
  bool isBinWidthNormalized ;
  std::vector <TH1F *> collection ;
  std::vector<int> colors ;  

} ;


#endif
