#ifndef plotUtils_h
#define plotUtils_h

#include <string>
#include <map>
#include <fstream>
#include <iostream>

#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "THStack.h"
#include "TLegend.h"
#include "TAxis.h"



class drawTStack
{
 public:
    
  //! ctor
  drawTStack(const std::string& inputDir,
             const std::string& listFileName,
             const std::string& baseRootFileName,
             const std::string& jetAlgorithm,
             const std::string& outputDir);
  
  //! dtor
  ~drawTStack();
  
  
  //! methods
  void Draw(const std::string& histoName, const int& step,
            const int& rebin, const bool& logy);
  
  void SetXaxisRange(const double& xMin, const double& xMax);
  void SetXaxisTitle(const std::string& xTitle);

  void SetYaxisRange(const double& yMin, const double& yMax);
  void SetYaxisTitle(const std::string& yTitle);
    
  
  
 private:
  
  std::string m_inputDir;
  std::string m_listFileName;
  std::string m_baseRootFileName;
  std::string m_jetAlgorithm;
  std::string m_outputDir;
  
  std::map<std::string, std::string> m_list;
  std::map<std::string, double> m_crossSection;
  
  bool m_xAxisRange;
  double m_xRangeMin;
  double m_xRangeMax;
  bool m_xAxisTitle;
  std::string m_xTitle;
  
  bool m_yAxisRange;
  double m_yRangeMin;
  double m_yRangeMax;
  bool m_yAxisTitle;
  std::string m_yTitle;
  
  TCanvas* c1;
};

#endif

