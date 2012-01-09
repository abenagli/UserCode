#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "setTDRStyle.h"
#include "HiggsMassWindows.h"

#include <iostream>
#include <string>
#include <sstream>

#include "TH1F.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TLegend.h"



int nBins = 320;
float xMin = 0.;
float xMax = 800.;
float xWidth = (xMax-xMin)/nBins;

float lumi = 1.;
int step;

std::string outputRootFilePath;
std::string outputRootFileName;
TFile* outFile;



void DrawCutPlot(TTree* tree,
                 const std::string& varName, const std::vector<std::string>& cutValues, const std::string& cutType,
                 float& lumi);

std::vector<TH1F*> InitializeHistograms(const std::string& prefix,
                                        const std::string& cutName, const std::string& varName, const std::vector<std::string>& cutValues, const std::string& cutType);

void FillHistograms(std::map<std::string,std::vector<TH1F*> >& histoMap,
                    TTree* tree,
                    const std::string& varName, const std::vector<std::string>& cutValues, const std::string& cutType);

void NormalizeHistograms(std::map<std::string,std::vector<TH1F*> >& histoMap,
                         const std::string& varName, const std::vector<std::string>& cutValues);

void DrawHistograms(std::map<std::string,std::vector<TH1F*> >& histoMap,
                    const std::string& cutName, const std::string& varName, const std::vector<std::string>& cutValues, const std::string& cutType);

void DrawSignificance(std::map<int,std::map<std::string,std::vector<TH1F*> > >& sigHistoMap, std::map<std::string,std::vector<TH1F*> >& bkgHistoMap,
                      const std::string& cutName, const std::string& varName, const std::vector<std::string>& cutValues, const std::string& cutType,
                      const std::vector<int>& masses);





int main(int argc, char** argv)
{
  setTDRStyle();
  
  
  
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> VBFAnalysis_testCuts::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
  
  
  //[Input]
  std::string baseDir      = gConfigParser -> readStringOption("Input::baseDir");
  std::string jetAlgorithm = gConfigParser -> readStringOption("Input::jetAlgorithm");
  std::string jetType      = gConfigParser -> readStringOption("Input::jetType");
  lumi = gConfigParser -> readFloatOption("Input::lumi");
  
  std::string inputFileName = gConfigParser -> readStringOption("Input::inputFileName");
  std::vector<std::string> inputSigDirs  = gConfigParser -> readStringListOption("Input::inputSigDirs");
  std::vector<std::string> inputBkgDirs  = gConfigParser -> readStringListOption("Input::inputBkgDirs");
  
  unsigned int nSigTrees  = inputSigDirs.size();
  unsigned int nBkgTrees  = inputBkgDirs.size();
   
  
  //[Output]
  outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  
  // Define the output file
  std::string outputRootFullFileName = outputRootFilePath + "/" + outputRootFileName + "_" + jetAlgorithm + ".root";
  outFile = new TFile(outputRootFullFileName.c_str(), "RECREATE");
  
    
  //[Options]
  step = gConfigParser -> readIntOption("Options::step");
  
  
  
  
  
  
  // define histograms
  std::vector<int> masses = GetMasses();
  
  std::map<std::string,std::string> cutNames;
  std::map<std::string,std::string> cutTypes;
  std::map<std::string,std::vector<std::string> > cutValues;
  std::map<int,std::map<std::string,std::vector<TH1F*> > > sigHistoMap;
  std::map<std::string,std::vector<TH1F*> > sig200HistoMap;
  std::map<std::string,std::vector<TH1F*> > sig250HistoMap;
  std::map<std::string,std::vector<TH1F*> > sig300HistoMap;
  std::map<std::string,std::vector<TH1F*> > sig350HistoMap;
  std::map<std::string,std::vector<TH1F*> > sig400HistoMap;
  std::map<std::string,std::vector<TH1F*> > sig450HistoMap;
  std::map<std::string,std::vector<TH1F*> > sig500HistoMap;
  std::map<std::string,std::vector<TH1F*> > sig550HistoMap;
  std::map<std::string,std::vector<TH1F*> > sig600HistoMap;
  std::map<std::string,std::vector<TH1F*> > bkgHistoMap;
  std::map<std::string,std::vector<TH1F*> > bkgNormHistoMap;
  

  std::string dummyCutName;
  std::string dummyVarName;
  std::string dummyCutType;
  std::vector<std::string> dummyCutValues;  
  std::vector<TH1F*> dummyHistos;
  
  
  
  
  
  
  //----------------
  // cut on WJJ_Dphi
  dummyVarName = "WJJ_Dphi";
  dummyCutType = "<";
  dummyCutValues.clear();
  dummyCutValues.push_back("2.5");
  dummyCutValues.push_back("2.0");
  dummyCutValues.push_back("1.5");
  
  dummyCutName = "WJJ_Dphi";
  cutNames[dummyVarName] = dummyCutName;
  cutValues[dummyVarName] = dummyCutValues;
  cutTypes[dummyVarName] = dummyCutType;
  sig200HistoMap[dummyVarName]  = InitializeHistograms("sig200", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig250HistoMap[dummyVarName]  = InitializeHistograms("sig250", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig300HistoMap[dummyVarName]  = InitializeHistograms("sig300", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig350HistoMap[dummyVarName]  = InitializeHistograms("sig350", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig400HistoMap[dummyVarName]  = InitializeHistograms("sig400", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig450HistoMap[dummyVarName]  = InitializeHistograms("sig450", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig500HistoMap[dummyVarName]  = InitializeHistograms("sig500", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig550HistoMap[dummyVarName]  = InitializeHistograms("sig550", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig600HistoMap[dummyVarName]  = InitializeHistograms("sig600", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  bkgHistoMap[dummyVarName]     = InitializeHistograms("bkg",    dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  bkgNormHistoMap[dummyVarName] = InitializeHistograms("bkgNorm",dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  
  //----------------
  // cut on WJJ_Deta
  dummyVarName = "WJJ_Deta";
  dummyCutType = "<";
  dummyCutValues.clear();
  dummyCutValues.push_back("2.5");
  dummyCutValues.push_back("2.0");
  dummyCutValues.push_back("1.5");
  
  dummyCutName = "WJJ_Deta";
  cutNames[dummyVarName] = dummyCutName;
  cutValues[dummyVarName] = dummyCutValues;
  cutTypes[dummyVarName] = dummyCutType;
  sig200HistoMap[dummyVarName]  = InitializeHistograms("sig200", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig250HistoMap[dummyVarName]  = InitializeHistograms("sig250", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig300HistoMap[dummyVarName]  = InitializeHistograms("sig300", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig350HistoMap[dummyVarName]  = InitializeHistograms("sig350", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig400HistoMap[dummyVarName]  = InitializeHistograms("sig400", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig450HistoMap[dummyVarName]  = InitializeHistograms("sig450", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig500HistoMap[dummyVarName]  = InitializeHistograms("sig500", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig550HistoMap[dummyVarName]  = InitializeHistograms("sig550", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig600HistoMap[dummyVarName]  = InitializeHistograms("sig600", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  bkgHistoMap[dummyVarName]     = InitializeHistograms("bkg",    dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  bkgNormHistoMap[dummyVarName] = InitializeHistograms("bkgNorm",dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  
  //----------------
  // cut on WJJ_pt
  dummyVarName = "WJJ_pt";
  dummyCutType = ">";
  dummyCutValues.clear();
  dummyCutValues.push_back("20.0");
  dummyCutValues.push_back("30.0");
  dummyCutValues.push_back("40.0");
  
  dummyCutName = "WJJ_pt";
  cutNames[dummyVarName] = dummyCutName;
  cutValues[dummyVarName] = dummyCutValues;
  cutTypes[dummyVarName] = dummyCutType;
  sig200HistoMap[dummyVarName]  = InitializeHistograms("sig200", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig250HistoMap[dummyVarName]  = InitializeHistograms("sig250", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig300HistoMap[dummyVarName]  = InitializeHistograms("sig300", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig350HistoMap[dummyVarName]  = InitializeHistograms("sig350", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig400HistoMap[dummyVarName]  = InitializeHistograms("sig400", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig450HistoMap[dummyVarName]  = InitializeHistograms("sig450", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig500HistoMap[dummyVarName]  = InitializeHistograms("sig500", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig550HistoMap[dummyVarName]  = InitializeHistograms("sig550", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig600HistoMap[dummyVarName]  = InitializeHistograms("sig600", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  bkgHistoMap[dummyVarName]     = InitializeHistograms("bkg",    dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  bkgNormHistoMap[dummyVarName] = InitializeHistograms("bkgNorm",dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  
  //----------------
  // cut on lepMet_pt
  dummyVarName = "lepMet_pt";
  dummyCutType = ">";
  dummyCutValues.clear();
  dummyCutValues.push_back("20.0");
  dummyCutValues.push_back("30.0");
  dummyCutValues.push_back("40.0");
  
  dummyCutName = "lepMet_pt";
  cutNames[dummyVarName] = dummyCutName;
  cutValues[dummyVarName] = dummyCutValues;
  cutTypes[dummyVarName] = dummyCutType;
  sig200HistoMap[dummyVarName]  = InitializeHistograms("sig200", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig250HistoMap[dummyVarName]  = InitializeHistograms("sig250", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig300HistoMap[dummyVarName]  = InitializeHistograms("sig300", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig350HistoMap[dummyVarName]  = InitializeHistograms("sig350", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig400HistoMap[dummyVarName]  = InitializeHistograms("sig400", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig450HistoMap[dummyVarName]  = InitializeHistograms("sig450", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig500HistoMap[dummyVarName]  = InitializeHistograms("sig500", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig550HistoMap[dummyVarName]  = InitializeHistograms("sig550", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig600HistoMap[dummyVarName]  = InitializeHistograms("sig600", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  bkgHistoMap[dummyVarName]     = InitializeHistograms("bkg",    dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  bkgNormHistoMap[dummyVarName] = InitializeHistograms("bkgNorm",dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  
  //----------------
  // cut on lepMet_mt
  dummyVarName = "lepMet_mt";
  dummyCutType = ">";
  dummyCutValues.clear();
  dummyCutValues.push_back("30.0");
  dummyCutValues.push_back("40.0");
  dummyCutValues.push_back("50.0");
  
  dummyCutName = "lepMet_mt";
  cutNames[dummyVarName] = dummyCutName;
  cutValues[dummyVarName] = dummyCutValues;
  cutTypes[dummyVarName] = dummyCutType;
  sig200HistoMap[dummyVarName]  = InitializeHistograms("sig200", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig250HistoMap[dummyVarName]  = InitializeHistograms("sig250", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig300HistoMap[dummyVarName]  = InitializeHistograms("sig300", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig350HistoMap[dummyVarName]  = InitializeHistograms("sig350", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig400HistoMap[dummyVarName]  = InitializeHistograms("sig400", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig450HistoMap[dummyVarName]  = InitializeHistograms("sig450", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig500HistoMap[dummyVarName]  = InitializeHistograms("sig500", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig550HistoMap[dummyVarName]  = InitializeHistograms("sig550", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig600HistoMap[dummyVarName]  = InitializeHistograms("sig600", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  bkgHistoMap[dummyVarName]     = InitializeHistograms("bkg",    dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  bkgNormHistoMap[dummyVarName] = InitializeHistograms("bkgNorm",dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  
  //----------------
  // cut on lep_eta
  dummyVarName = "lep_eta";
  dummyCutType = "<,>";
  dummyCutValues.clear();
  dummyCutValues.push_back("2.0,-2.0");
  dummyCutValues.push_back("1.5,-1.5");
  dummyCutValues.push_back("1.0,-1.0");
  
  dummyCutName = "lep_eta";
  cutNames[dummyVarName] = dummyCutName;
  cutValues[dummyVarName] = dummyCutValues;
  cutTypes[dummyVarName] = dummyCutType;
  sig200HistoMap[dummyVarName]  = InitializeHistograms("sig200", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig250HistoMap[dummyVarName]  = InitializeHistograms("sig250", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig300HistoMap[dummyVarName]  = InitializeHistograms("sig300", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig350HistoMap[dummyVarName]  = InitializeHistograms("sig350", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig400HistoMap[dummyVarName]  = InitializeHistograms("sig400", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig450HistoMap[dummyVarName]  = InitializeHistograms("sig450", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig500HistoMap[dummyVarName]  = InitializeHistograms("sig500", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig550HistoMap[dummyVarName]  = InitializeHistograms("sig550", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig600HistoMap[dummyVarName]  = InitializeHistograms("sig600", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  bkgHistoMap[dummyVarName]     = InitializeHistograms("bkg",    dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  bkgNormHistoMap[dummyVarName] = InitializeHistograms("bkgNorm",dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  
  
  
  /*
  //----------------
  // cut WJJ_m
  dummyVarName = "WJJ_m";
  dummyCutType = ">,<";
  dummyCutValues.clear();
  dummyCutValues.push_back("70.,90.");
  dummyCutValues.push_back("65.,95.");
  dummyCutValues.push_back("60.,100.");
  dummyCutValues.push_back("55.,110.");
  
  dummyCutName = "WJJ_m";
  cutNames[dummyVarName] = dummyCutName;
  cutValues[dummyVarName] = dummyCutValues;
  cutTypes[dummyVarName] = dummyCutType;
  sig200HistoMap[dummyVarName]  = InitializeHistograms("sig200", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig250HistoMap[dummyVarName]  = InitializeHistograms("sig250", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig300HistoMap[dummyVarName]  = InitializeHistograms("sig300", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig350HistoMap[dummyVarName]  = InitializeHistograms("sig350", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig400HistoMap[dummyVarName]  = InitializeHistograms("sig400", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig450HistoMap[dummyVarName]  = InitializeHistograms("sig450", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig500HistoMap[dummyVarName]  = InitializeHistograms("sig500", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig550HistoMap[dummyVarName]  = InitializeHistograms("sig550", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig600HistoMap[dummyVarName]  = InitializeHistograms("sig600", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  bkgHistoMap[dummyVarName]     = InitializeHistograms("bkg",    dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  bkgNormHistoMap[dummyVarName] = InitializeHistograms("bkgNorm",dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  */
  
  
  
  /*
  //-------------
  // cut on nJets
  dummyVarName = " ";
  dummyCutType = " ";
  dummyCutValues.clear();
  dummyCutValues.push_back("nJets_cnt_pt30 < 4");
  dummyCutValues.push_back("nJets_pt30 < 4");
  
  dummyCutName = "nJets";
  cutNames[dummyVarName] = dummyCutName;
  cutValues[dummyVarName] = dummyCutValues;
  cutTypes[dummyVarName] = dummyCutType;
  sig200HistoMap[dummyVarName]  = InitializeHistograms("sig200", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig250HistoMap[dummyVarName]  = InitializeHistograms("sig250", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig300HistoMap[dummyVarName]  = InitializeHistograms("sig300", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig350HistoMap[dummyVarName]  = InitializeHistograms("sig350", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig400HistoMap[dummyVarName]  = InitializeHistograms("sig400", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig450HistoMap[dummyVarName]  = InitializeHistograms("sig450", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig500HistoMap[dummyVarName]  = InitializeHistograms("sig500", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig550HistoMap[dummyVarName]  = InitializeHistograms("sig550", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  sig600HistoMap[dummyVarName]  = InitializeHistograms("sig600", dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  bkgHistoMap[dummyVarName]     = InitializeHistograms("bkg",    dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  bkgNormHistoMap[dummyVarName] = InitializeHistograms("bkgNorm",dummyCutName,dummyVarName,dummyCutValues,dummyCutType);
  */
  
  
  
  
  
  
  // loop on sig samples
  for(unsigned int i = 0; i < nSigTrees; ++i)
  {
    // open root file
    std::string inputFullFileName;
    inputFullFileName = baseDir + "/" + inputSigDirs.at(i)   + "/" + inputFileName + ".root";
    TFile* inputFile = TFile::Open(inputFullFileName.c_str());
    std::cout << ">>>>>> VBFAnalysis_testCuts::sig tree in " << inputSigDirs.at(i) << " opened" << std::endl;
    
    
    // get the tree at nth step
    TTree* tree = NULL;
    char treeName[50];
    sprintf(treeName, "ntu_%d", step);
    inputFile -> GetObject(treeName, tree);
    if ( tree -> GetEntries() == 0 ) continue;
    
    
    for(std::map<std::string,std::string>::const_iterator mapIt = cutTypes.begin();
        mapIt != cutTypes.end(); ++mapIt)
    {
      dummyVarName = mapIt -> first;
      dummyCutType = cutTypes[dummyVarName];
      dummyCutValues = cutValues[dummyVarName];
      
      std::string token1,token2,token3;
      std::istringstream iss(inputSigDirs.at(i));
      getline(iss,token1,'_');
      getline(iss,token2,'_');
      getline(iss,token3,'_');
      
      // fill histograms
      if( (token2 == "M-200") || (token3 == "M-200") )
        FillHistograms(sig200HistoMap,tree,dummyVarName,dummyCutValues,dummyCutType);
      if( (token2 == "M-250") || (token3 == "M-250") )
        FillHistograms(sig250HistoMap,tree,dummyVarName,dummyCutValues,dummyCutType);
      if( (token2 == "M-300") || (token3 == "M-300") )
        FillHistograms(sig300HistoMap,tree,dummyVarName,dummyCutValues,dummyCutType);
      if( (token2 == "M-350") || (token3 == "M-350") )
        FillHistograms(sig350HistoMap,tree,dummyVarName,dummyCutValues,dummyCutType);
      if( (token2 == "M-400") || (token3 == "M-400") )
        FillHistograms(sig400HistoMap,tree,dummyVarName,dummyCutValues,dummyCutType);
      if( (token2 == "M-450") || (token3 == "M-450") )
        FillHistograms(sig450HistoMap,tree,dummyVarName,dummyCutValues,dummyCutType);
      if( (token2 == "M-500") || (token3 == "M-500") )
        FillHistograms(sig500HistoMap,tree,dummyVarName,dummyCutValues,dummyCutType);
      if( (token2 == "M-550") || (token3 == "M-550") )
        FillHistograms(sig550HistoMap,tree,dummyVarName,dummyCutValues,dummyCutType);
      if( (token2 == "M-600") || (token3 == "M-600") )
        FillHistograms(sig600HistoMap,tree,dummyVarName,dummyCutValues,dummyCutType);
    }
  }
  
  sigHistoMap[200] = sig200HistoMap;
  sigHistoMap[250] = sig250HistoMap;
  sigHistoMap[300] = sig300HistoMap;
  sigHistoMap[350] = sig350HistoMap;
  sigHistoMap[400] = sig400HistoMap;
  sigHistoMap[450] = sig450HistoMap;
  sigHistoMap[500] = sig500HistoMap;
  sigHistoMap[550] = sig550HistoMap;
  sigHistoMap[600] = sig600HistoMap;
  
  
  
  // loop on bkg samples
  for(unsigned int i = 0; i < nBkgTrees; ++i)
  {
    // open root file
    std::string inputFullFileName;
    inputFullFileName = baseDir + "/" + inputBkgDirs.at(i)   + "/" + inputFileName + ".root";
    TFile* inputFile = TFile::Open(inputFullFileName.c_str());
    std::cout << ">>>>>> VBFAnalysis_testCuts::bkg tree in " << inputBkgDirs.at(i) << " opened" << std::endl;
    
    
    // get the tree at nth step
    TTree* tree = NULL;
    char treeName[50];
    sprintf(treeName, "ntu_%d", step);
    inputFile -> GetObject(treeName, tree);
    if ( tree -> GetEntries() == 0 ) continue;
    
    
    for(std::map<std::string,std::string>::const_iterator mapIt = cutTypes.begin();
        mapIt != cutTypes.end(); ++mapIt)
    {
      dummyVarName = mapIt -> first;
      dummyCutType = cutTypes[dummyVarName];
      dummyCutValues = cutValues[dummyVarName];
      
      // fill histograms
      FillHistograms(bkgHistoMap,    tree,dummyVarName,dummyCutValues,dummyCutType);
      FillHistograms(bkgNormHistoMap,tree,dummyVarName,dummyCutValues,dummyCutType);
    }
  }
  
  
  
  
  
  
  // draw plots
  for(std::map<std::string,std::string>::const_iterator mapIt = cutTypes.begin();
      mapIt != cutTypes.end(); ++mapIt)
  {
    dummyVarName = mapIt -> first;
    dummyCutName = cutNames[dummyVarName];
    dummyCutType = cutTypes[dummyVarName];
    dummyCutValues = cutValues[dummyVarName];
    
    NormalizeHistograms(bkgNormHistoMap,dummyVarName,cutValues[dummyVarName]);
    DrawHistograms(bkgNormHistoMap,dummyCutName,dummyVarName,cutValues[dummyVarName],cutTypes[dummyVarName]);
    DrawSignificance(sigHistoMap,bkgHistoMap,dummyCutName,dummyVarName,cutValues[dummyVarName],cutTypes[dummyVarName],masses);
  }
  
  
  
  return 0;
}






std::vector<TH1F*> InitializeHistograms(const std::string& prefix,
                                        const std::string& cutName, const std::string& varName, const std::vector<std::string>& cutValues, const std::string& cutType)

{
  std::vector<TH1F*> histos;
  
  
  //-------
  // no cut
  std::string histoName = "h_" + prefix + "_" + cutName;
  //std::cout << "histoName: " << histoName << std::endl;
  histos.push_back( new TH1F(histoName.c_str(),"",nBins,xMin,xMax) );
  histos[0] -> Sumw2();
  
  histos[0] -> GetXaxis() -> SetTitle("m(lepton+neutrino+WJets)   (GeV/c^{2})");
  histos[0] -> GetYaxis() -> SetTitle("events");
  
  histos[0] -> SetFillStyle(3001);
  histos[0] -> SetFillColor(41);
  histos[0] -> SetLineColor(41);
  
  
  //-----------
  // apply cuts
  for(unsigned int iCut = 0; iCut < cutValues.size(); ++iCut)
  {
    std::string cut = varName + cutType + cutValues[iCut];
    
    histoName = "h_" + prefix + "_" + cut;
    //std::cout << "histoName: " << histoName << std::endl;
    histos.push_back( new TH1F(histoName.c_str(),"",nBins,xMin,xMax) );
    histos[iCut+1] -> Sumw2();
    
    histos[iCut+1] -> GetXaxis() -> SetTitle("m(lepton+neutrino+WJets)   (GeV/c^{2})");
    histos[iCut+1] -> GetYaxis() -> SetTitle("events");
    
    histos[iCut+1] -> SetMarkerStyle(1);
    histos[iCut+1] -> SetLineColor(51+6*iCut);
  }
  
  
  return histos;
}







void FillHistograms(std::map<std::string,std::vector<TH1F*> >& histoMap,
                    TTree* tree,
                    const std::string& varName, const std::vector<std::string>& cutValues, const std::string& cutType)
{
  outFile -> cd();
  
  
  // define common cut and weight
  //std::string commonCut = "(1 == 1)";
  std::string commonCut = "(1 == 1) * ((WJJ_m >= 65.) && (WJJ_m < 95.))";
  char lumiChar[50]; sprintf(lumiChar,"%f",lumi);
  std::string weight = "( 1000 * " + std::string(lumiChar) + " * 1. / totEvents * crossSection * eventWeight * PUWeight )";
  
  
  //-------
  // no cut
  std::string histoName = (histoMap[varName].at(0)) -> GetName();
  tree -> Draw(("lepNuW_m_KF >>+ "+histoName).c_str(),(weight+" * "+commonCut).c_str(),"goff");
  
  
  //-----------
  // apply cuts
  for(unsigned int iCut = 0; iCut < cutValues.size(); ++iCut)
  {
    std::string cut;
    bool isFirst = true;
    
    std::string tokenType;
    std::istringstream issType(cutType);
    std::string tokenValue;
    std::istringstream issValue(cutValues[iCut]);
    //std::cout << "cutVal: " << cutValues[iCut] << std::endl;
    while( getline(issType,tokenType,',') )
    {
      getline(issValue,tokenValue,',');
      if( isFirst == true )
      {
        cut += "(" + varName + " " + tokenType + " " + tokenValue + ")";
        isFirst = false;
      }
      else
        cut += " && (" + varName + " " + tokenType + " " + tokenValue + ")";
    }    
    
    std::string extendedCut = commonCut + " * (" + cut + ")";
    //std::cout << "extendedCut: " << extendedCut << std::endl;
    
    histoName = (histoMap[varName].at(iCut+1)) -> GetName();
    tree -> Draw(("lepNuW_m_KF >>+ "+histoName).c_str(),(weight+" * "+extendedCut).c_str(),"goff");
  }
  
  
  return;
}






void NormalizeHistograms(std::map<std::string,std::vector<TH1F*> >& histoMap,
                         const std::string& varName, const std::vector<std::string>& cutValues)
{
  //-------
  // no cut
  (histoMap[varName]).at(0) -> Scale(1./((histoMap[varName]).at(0))->Integral());
  
  
  //-----------
  // apply cuts
  for(unsigned int iCut = 0; iCut < cutValues.size(); ++iCut)
  {
    (histoMap[varName]).at(iCut+1) -> Scale(1./((histoMap[varName]).at(iCut+1))->Integral());
  }
  
  
  return;
}






void DrawHistograms(std::map<std::string,std::vector<TH1F*> >& histoMap,
                    const std::string& cutName, const std::string& varName, const std::vector<std::string>& cutValues, const std::string& cutType)
{
  // define canvas
  TCanvas* c = new TCanvas(("c_"+varName).c_str());
  c -> cd();
  c -> SetGridx();
  c -> SetGridy();
  
  
  float maximum = -1;
  
  
  // define legend
  TLegend* legend = new TLegend(0.55,0.60,0.70,0.90);
  legend -> SetFillColor(kWhite);
  legend -> SetFillStyle(1001);
  legend -> SetTextFont(42);
  legend -> SetTextSize(0.025);
  
  
  
  //--------
  // no cuts
  (histoMap[varName]).at(0) -> Draw("HISTO"); 
  legend -> AddEntry((histoMap[varName]).at(0),"no cut","F");
  
  if( (histoMap[varName]).at(0)->GetMaximum() > maximum ) maximum = (histoMap[varName]).at(0) -> GetMaximum();
  
  
  //-----------
  // apply cuts
  for(unsigned int iCut = 0; iCut < cutValues.size(); ++iCut)
  {
    std::string cut = varName + " " + cutType + " " + cutValues[iCut];
    ((histoMap[varName]).at(iCut+1)) -> Draw("HISTO,same");
    ((histoMap[varName]).at(iCut+1)) -> Draw("P,same");
    
    legend -> AddEntry(((histoMap[varName]).at(iCut+1)),cut.c_str(),"L");
    
    if( ((histoMap[varName]).at(iCut+1))->GetMaximum() > maximum ) maximum = ((histoMap[varName]).at(iCut+1)) -> GetMaximum();
  }
  
  
  (histoMap[varName]).at(0) -> GetYaxis() -> SetRangeUser(0.,1.1*maximum);
  legend -> Draw("same");
  
  
  std::stringstream outFileName;
  outFileName << outputRootFileName << "_step" << step << "_" << cutName << ".pdf";
  c -> Print(outFileName.str().c_str(),"pdf");
  
  
  return;
}






void DrawSignificance(std::map<int,std::map<std::string,std::vector<TH1F*> > >& sigHistoMap, std::map<std::string,std::vector<TH1F*> >& bkgHistoMap,
                      const std::string& cutName, const std::string& varName, const std::vector<std::string>& cutValues, const std::string& cutType,
                      const std::vector<int>& masses)
{
  // define canvas
  TCanvas* c = new TCanvas(("c_significance_"+varName).c_str());
  c -> cd();
  c -> SetGridx();
  c -> SetGridy();
  
  
  // define legend
  TLegend* legend = new TLegend(0.55,0.60,0.70,0.90);
  legend -> SetFillColor(kWhite);
  legend -> SetFillStyle(1001);
  legend -> SetTextFont(42);
  legend -> SetTextSize(0.025);
  
  
  // define output TGraphs
  std::vector<TGraph*> g_significance;
  
  
  
  //--------
  // no cuts
  g_significance.push_back( new TGraph() );
  g_significance.at(0) -> SetMarkerStyle(20);
  g_significance.at(0) -> SetMarkerSize(1.3);
  g_significance.at(0) -> SetMarkerColor(41);
  g_significance.at(0) -> SetLineColor(41);
  
  legend -> AddEntry(g_significance.at(0),"no cut","PL");
  
  
  //-----------
  // apply cuts
  for(unsigned int iCut = 0; iCut < cutValues.size(); ++iCut)
  {
    std::string cut = varName + " " + cutType + " " + cutValues[iCut];
    
    g_significance.push_back( new TGraph() );
    g_significance.at(iCut+1) -> SetMarkerStyle(20);
    g_significance.at(iCut+1) -> SetMarkerSize(0.7);
    g_significance.at(iCut+1) -> SetMarkerColor(51+6*iCut);
    g_significance.at(iCut+1) -> SetLineColor(51+6*iCut);
    
    legend -> AddEntry(g_significance.at(iCut+1),cut.c_str(),"PL");
  }
  
  
  
  // loop on masses and fill the graphs
  unsigned int nMasses = masses.size();
  for(unsigned int iMass = 0; iMass < nMasses; ++iMass)
  {
    int mass = masses[iMass];
    
    
    // get integration limits
    int binMin = -1;
    int binMax = -1;
    for(int bin = 1; bin <= nBins; ++bin)
    {
      float binCenter = xMin + 0.5*xWidth + xWidth*(bin-1);
      
      if( (binCenter >= GetLepNuWMMIN(mass)) && (binMin == -1) ) binMin = bin;
      if( (binCenter >= GetLepNuWMMIN(mass)) && (binCenter < GetLepNuWMMAX(mass)) ) binMax = bin;
    }
    
    
    //--------
    // no cuts
    TH1F* sigHisto = ((sigHistoMap[mass])[varName]).at(0);
    TH1F* bkgHisto = (bkgHistoMap[varName]).at(0);
    
    float S = sigHisto -> Integral(binMin,binMax); 
    float B = bkgHisto -> Integral(binMin,binMax); 
    
    g_significance.at(0) -> SetPoint(iMass,mass,S/sqrt(S+B));
    
    
    //-----------
    // apply cuts
    for(unsigned int iCut = 0; iCut < cutValues.size(); ++iCut)
    {
      sigHisto = ((sigHistoMap[mass])[varName]).at(iCut+1);
      bkgHisto = (bkgHistoMap[varName]).at(iCut+1);
      
      S = sigHisto -> Integral(binMin,binMax);
      B = bkgHisto -> Integral(binMin,binMax);
      
      g_significance.at(iCut+1) -> SetPoint(iMass,mass,S/sqrt(S+B));  
    }
  }
  
  
  
  g_significance.at(0) -> GetXaxis() -> SetTitle("Higgs mass   (GeV/c^{2})");
  g_significance.at(0) -> GetYaxis() -> SetTitle("S / #sqrt{S+B}");
  g_significance.at(0) -> SetMinimum(0.);
  g_significance.at(0) -> SetMaximum(3.);
  g_significance.at(0) -> Draw("APL");
  
  for(unsigned int iCut = 0; iCut < cutValues.size(); ++iCut)
  {
    g_significance.at(iCut+1) -> Draw("PL,same");
  }
  
  legend -> Draw("same");  
  
  
  
  std::stringstream outFileName;
  outFileName << outputRootFileName << "_step" << step << "_" << cutName << "_significance"<< ".pdf";
  c -> Print(outFileName.str().c_str(),"pdf");  
  
  
  
  return;
}
