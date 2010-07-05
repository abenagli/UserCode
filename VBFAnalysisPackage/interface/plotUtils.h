#ifndef plotUtils_h
#define plotUtils_h

#include <cstdlib>
#include <string>
#include <map>
#include <utility>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <sys/stat.h> 

#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "THStack.h"
#include "TLegend.h"
#include "TAxis.h"



double MyGetMinimum(const TH1F* histo, const double& minval, int binMin=-1, int binMax=-1);



class drawTStack
{
 public:
    
  //! ctor
  drawTStack(const std::string& inputDir,
             const std::string& listFileName,
             const std::string& baseRootFileName,
             const std::string& outputDir);
  
  //! dtor
  ~drawTStack();
  
  
  //! methods
  void Draw(const std::vector<std::string>& histoNames, const std::string& mode,
            const float& lumi, const int& step,
            const int& rebin, const bool& logy);
  void DrawEvents(const std::string& mode, const float& lumi, const int& step, const bool& logy);
  
  void SetXaxisRange(const double& xMin, const double& xMax);
  void SetXaxisTitle(const std::string& xTitle);

  void SetYaxisRange(const double& yMin, const double& yMax);
  void SetYaxisTitle(const std::string& yTitle);

  void SetDrawLegend(const bool& drawLegend);
  void SetXLegend(const double& xLow, const double& xHigh);
  void SetYLegend(const double& yLow, const double& yHigh);
  
  
 private:
  
  std::string m_inputDir;
  std::string m_listFileName;
  std::string m_baseRootFileName;
  std::string m_outputDir;
  
  std::vector<std::pair<std::string, std::string> > m_list;
  std::map<std::string, int> m_dataFlag;
  std::map<std::string, double> m_mH;
  std::map<std::string, double> m_crossSection;
  std::vector<std::pair<std::string, std::string> > m_jetAlgorithm;
  
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
  
  bool m_drawLegend;
  double m_xLowLegend;
  double m_yLowLegend;
  double m_xHighLegend;
  double m_yHighLegend;
  
  TCanvas* c1;
};

#endif

