#ifndef hColl_h
#define hColl_h

#include <TH1.h>
#include <TFile.h>
#include <THStack.h>
#include <vector>
#include <string>
#include <cmath>


#include "plotUtils.h"


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
