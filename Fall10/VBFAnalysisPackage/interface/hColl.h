#ifndef hColl_h
#define hColl_h

#include <TH1.h>
#include <TFile.h>
#include <THStack.h>
#include <vector>
#include <string>


struct hColl
{
  hColl (std::string pN, int b, double m, double M) ;
  ~hColl () ;
  TH1F * addSample (std::string sampleName) ;
  void save (TFile * f) ;
  THStack makeStack () ;


  std::string plotName ;
  int bins ;
  double min ;
  double max ;
  std::vector <TH1F *> collection ;
  std::vector<int> colors ;  

} ;


#endif
