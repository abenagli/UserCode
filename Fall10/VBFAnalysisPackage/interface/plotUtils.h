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
#include "TGraph.h"
#include "TTree.h"
#include "THStack.h"
#include "TLegend.h"
#include "TAxis.h"
#include "TLatex.h"


void normalizeToBinWidths (TH1F * input) ;



double MyGetMinimum(const TH1F* histo, const double& minval, int binMin=-1, int binMax=-1);
double MyGetMaximum(const TH1F* histo, const double& maxval, int binMin=-1, int binMax=-1);

void MyDraw(THStack* hs, const std::string& option = "");

void DrawTStackError(THStack* hs, bool poisson = false, double syst = 0.);

TH1F* DrawTStackDataMCRatio(THStack* hs, TH1F* dataGlobalHisto,
                            TGraph* ratioGraph1s, TGraph* ratioGraph2s);

void setErrorForEmptyBins (TH1F * input);






class drawTStack
{
 public:
    
  //! ctor
  drawTStack(const std::string& inputDir,
             const std::string& listFileName,
             const std::string& baseRootFileName,
             const std::string& outputDir,
             const std::string& imgFormat);
  
  //! dtor
  ~drawTStack();
  
  
  //! methods
  void Initialize();
  
  int MakeHistograms(std::vector<std::string>& variableNames, const std::string& histoName,
                     const std::string& mode,
                     const float& lumi, const int& step,
                     const int& nBins,
                     const bool& PURescale = true,
                     const bool& weightEvent = true,
                     const bool& stackSig = false,
                     std::vector<std::string>* cut = NULL);
                      
  void FindMinimumMaximum(const std::string& mode);  
  
  void Draw(std::vector<std::string>& variableNames, const std::string& histoName,
            const std::string& mode,
            const float& lumi, const int& step,
            const int& nBins,
            const bool& PURescale = true,
            const bool& weightEvent = true,
            const bool& stackSig = false,
            std::vector<std::string>* cut = NULL);
  
  void Draw(TCanvas* c, const std::string& histoName, const std::string& mode, const bool& stackSig, const bool& logy, const float& lumi);
  
  void DrawEvents(const std::string& mode,
                  const float& lumi,
                  const int& step,
                  const bool& logy,
                  const bool& PURescale = true,
                  const bool& stackSig = false);
  
  void CloseRootFiles();
  
  void SetGeneralCut(const std::string& generalCut);
  
  void SetXaxisRange(const double& xMin, const double& xMax);
  void SetXaxisTitle(const std::string& xTitle);

  void SetYaxisRange(const double& yMin, const double& yMax);
  void SetYaxisTitle(const std::string& yTitle);

  void SetDrawLegend(const bool& drawLegend);
  void SetXLegend(const double& xLow, const double& xHigh);
  void SetYLegend(const double& yLow, const double& yHigh);

  void SetUnit(const std::string& unit);  
  
  
  
  int GetColorSummed(const std::string& name) { return m_color_summed[name]; };
  
  TH1F* GetGlobalHisto()     { return m_globalHisto;     };
  TH1F* GetBkgGlobalHisto()  { return m_bkgGlobalHisto;  };
  TH1F* GetSigGlobalHisto()  { return m_sigGlobalHisto;  };
  TH1F* GetDataGlobalHisto() { return m_dataGlobalHisto; };
  
  THStack* GetStack()    { return m_stack;    };
  THStack* GetBkgStack() { return m_bkgStack; };
  THStack* GetSigStack() { return m_sigStack; };
  
  std::map<std::string, TH1F*> GetHistoSummed()     { return m_histo_summed; };
  std::map<std::string, TH1F*> GetBkgHistoSummed()  { return m_bkgHisto_summed; };
  std::map<std::string, TH1F*> GetSigHistoSummed()  { return m_sigHisto_summed; };
  std::map<std::string, TH1F*> GetDataHistoSummed() { return m_dataHisto_summed; };
  
  TLegend* GetLegend() { return m_legend; }; 
  
  
  
 private:
  
  std::string m_inputDir;
  std::string m_listFileName;
  std::string m_baseRootFileName;
  std::string m_outputDir;
  std::string m_imgFormat;
  
  std::vector<TFile*> m_rootFiles;
    
  std::vector<std::pair<std::string, std::string> > m_list;
  std::vector<std::pair<std::string, std::string> > m_jetAlgorithm;
  
  std::map<std::string, double> m_crossSection;
  std::map<std::string, int> m_color;
  std::map<std::string, int> m_linestyle;
  std::map<std::string, int> m_fillstyle;
  std::map<std::string, double> m_mH;
  std::map<std::string, int> m_dataFlag;
  
  std::map<std::string, double> m_crossSection_summed;
  std::map<std::string, int> m_color_summed;
  std::map<std::string, int> m_linestyle_summed;
  std::map<std::string, int> m_fillstyle_summed;
  std::map<std::string, double> m_mH_summed;
  std::map<std::string, int> m_dataFlag_summed;
  
  std::map<std::string, TH1F*> m_histo_summed;
  std::map<std::string, TH1F*> m_bkgHisto_summed;
  std::map<std::string, TH1F*> m_sigHisto_summed;
  std::map<std::string, TH1F*> m_dataHisto_summed;

  TH1F* m_globalHisto;  
  TH1F* m_bkgGlobalHisto;
  TH1F* m_sigGlobalHisto;
  TH1F* m_dataGlobalHisto;
  
  THStack* m_stack;
  THStack* m_bkgStack;
  THStack* m_sigStack;
  
  TLegend* m_legend;
  
  double m_globalIntegral;  
  double m_bkgGlobalIntegral;
  double m_sigGlobalIntegral;
  double m_dataGlobalIntegral;
  
  double m_globalMinimum;
  double m_globalMaximum;
  
  std::string m_generalCut;
  
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
  std::string m_unit;
  
  bool m_drawLegend;
  double m_xLowLegend;
  double m_yLowLegend;
  double m_xHighLegend;
  double m_yHighLegend;
  
  TCanvas* c1;
  TCanvas* c2;
};

#endif
