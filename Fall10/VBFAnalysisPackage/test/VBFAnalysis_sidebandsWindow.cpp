#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "plotUtils.h"
#include "HiggsMassWindows.h"
#include "Functions.h"

#include <iostream>
#include <string>
#include <sstream>

#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TVirtualFitter.h"
#include "TRandom3.h"

using namespace std;

void Chi2ArrayPrint(TH2F * Chi2Array);


string varName = "lepNuW_m_KF";
string method = "";
string analysisMethod = "";

float LowSbcutMin = 30.;
float LowSbcutMax = 65.;
float LowSbcutStep = 5.;
int nLowSbCuts = int((LowSbcutMax - LowSbcutMin)/LowSbcutStep);  
  
float UpSbcutMin = 95.;
float UpSbcutMax = 150.;
float UpSbcutStep = 5.;
int nUpSbCuts = int((UpSbcutMax - UpSbcutMin)/UpSbcutStep);  






int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    cerr << ">>>>> VBFAnalysis_sidebandsWindow::usage: " << argv[0] << " configFileName" << endl ;
    return 1;
  }
  
  
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
  
  
  //[Input]
  string baseDir      = gConfigParser -> readStringOption("Input::baseDir");
  string jetAlgorithm = gConfigParser -> readStringOption("Input::jetAlgorithm");
  float lumi = gConfigParser -> readFloatOption("Input::lumi");
  int higgsMass = gConfigParser -> readIntOption("Input::higgsMass");
  char higgsMassChar[50];
  sprintf(higgsMassChar,"%d",higgsMass);
  
  string inputFileName = gConfigParser -> readStringOption("Input::inputFileName");
  vector<string> inputSigDirs  = gConfigParser -> readStringListOption("Input::inputSigDirs");
  vector<string> inputBkgDirs  = gConfigParser -> readStringListOption("Input::inputBkgDirs");
  vector<string> inputDataDirs = gConfigParser -> readStringListOption("Input::inputDataDirs");
  
  unsigned int nSigTrees  = inputSigDirs.size();
  unsigned int nBkgTrees  = inputBkgDirs.size();
  unsigned int nDataTrees = inputDataDirs.size();
  
  
  //[Output]
  string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  
  
  //[Options]
  int step        = gConfigParser -> readIntOption("Options::step");
  method          = gConfigParser -> readStringOption("Options::method");
  analysisMethod  = gConfigParser -> readStringOption("Options::analysisMethod");
  
  
  int nBins = 200;
  float xMin = 0.;
  float xMax = 1000.;
  float xWidth = (xMax-xMin)/nBins;
  
  
  
  
  
  // Define the output file
  string outputRootFullFileName = outputRootFilePath + "/" + outputRootFileName + "_" + jetAlgorithm + "_H" + string(higgsMassChar) + ".root";
  TFile* outFile = new TFile(outputRootFullFileName.c_str(), "RECREATE");
  
  
  
  
  
  
  // define the the signal region (which remains fixed)
  string generalCut = " ( " + varName + " > 180. ) ";
  string sigRegion = "( (WJJ_m >= 65.) && (WJJ_m < 95.) )";
  
  
  
  // Define the histograms
  TH1F* h_mcSum_sigRegion = new TH1F("h_mcSum_sigRegion","",nBins,xMin,xMax);
  h_mcSum_sigRegion -> Sumw2();
  TH1F* h_mcSum_sbRegion = new TH1F("h_mcSum_sbRegion","",nBins,xMin,xMax);
  h_mcSum_sbRegion -> Sumw2();

  
    
  TH2F * Chi2Array = new TH2F ("Chi2Array","",nLowSbCuts+1,LowSbcutMin,LowSbcutMax+LowSbcutStep,
                                              nUpSbCuts+1, UpSbcutMin, UpSbcutMax+UpSbcutStep);

  for (int iLowSbcut = 0; iLowSbcut <= nLowSbCuts; iLowSbcut++)
  {
    
    float LowSbcut = LowSbcutMax - iLowSbcut * LowSbcutStep;
  
    char LowSbcutChar[10];
    sprintf(LowSbcutChar,"%.1f",LowSbcut);
     
    
    for (int iUpSbcut = 0; iUpSbcut <= nUpSbCuts; iUpSbcut++)
    {
      
      float UpSbcut = UpSbcutMax - iUpSbcut * UpSbcutStep;
  
      char UpSbcutChar[10];
      sprintf(UpSbcutChar,"%.1f",UpSbcut);
      
            
      string sbRegion = "( ( (WJJ_m >= " + string(LowSbcutChar) + ") && (WJJ_m < 65.) ) || ( (WJJ_m >= 95.) && (WJJ_m < " + string(UpSbcutChar) + " ) ) )"; 
      
       cout << ">>>>>>>>>  sideband Region: " << sbRegion << endl;
      
      
      //-------------------------------
      // ON MC - FILL MASS DISTRIBUTION
      //-------------------------------
      
      // loop on the samples
      for(unsigned int i = 0; i < nBkgTrees; ++i)
      {
        // open root file
        string inputFullFileName;
        inputFullFileName = baseDir + "/" + inputBkgDirs.at(i)   + "/" + inputFileName + ".root";
        TFile* inputFile = TFile::Open(inputFullFileName.c_str());
        cout << ">>>>>> VBFAnalysis_sidebandsWindow::mc tree in " << inputBkgDirs.at(i) << " opened" << endl;
    
    
        // get the tree at nth step
        TTree* tree = NULL;
        char treeName[50];
        sprintf(treeName, "ntu_%d", step);
        inputFile -> GetObject(treeName, tree);
        if ( tree -> GetEntries() == 0 ) continue; 
    
        outFile -> cd();

        stringstream weight;
        weight << "( 1000 * " << lumi << " * 1. * eventWeight / totEvents * crossSection * PUWeight )";
    
    
        string sigCut = weight.str() + " * " + sigRegion + " * " + generalCut;
        string sbCut = weight.str() + " * " + sbRegion + " * " + generalCut;
    
        tree -> Draw((varName+" >>+ h_mcSum_sigRegion").c_str(),   sigCut.c_str(),"goff"); 
        tree -> Draw((varName+" >>+ h_mcSum_sbRegion").c_str(),   sbCut.c_str(),"goff"); 
       
        inputFile -> Close();
      }
  
      double Chi2 = h_mcSum_sigRegion->Chi2Test(h_mcSum_sbRegion, "WW,CHI2/NDF"); 
      Chi2Array -> Fill(LowSbcut + LowSbcutStep/2., UpSbcut + UpSbcutStep/2., Chi2);
      
      
      if(iLowSbcut == 0 && iUpSbcut == 0) h_mcSum_sigRegion -> Write("h_mcSum_sigRegion");
      h_mcSum_sbRegion  -> Write(("h_mcSum_sbRegion_Low" + string(LowSbcutChar) + "to65_Up95to" + string(UpSbcutChar)).c_str());
      
      
      h_mcSum_sigRegion -> Reset();
      h_mcSum_sbRegion  -> Reset();
      
    }  
  }
  
  
  
  //-----------------
  // Save the results
  
  outFile -> cd();

  Chi2ArrayPrint(Chi2Array);
  Chi2Array -> Write();
  
  outFile -> Close();

  
  return 0;
}




void Chi2ArrayPrint(TH2F * Chi2Array)
{
  
  for (int iLowSbcut = 0; iLowSbcut <= nLowSbCuts; iLowSbcut++)
  {
  
    for (int iUpSbcut = 0; iUpSbcut <= nUpSbCuts; iUpSbcut++)
    {
    
       float LowSbcut = LowSbcutMax - iLowSbcut * LowSbcutStep;
       char LowSbcutChar[10];
       sprintf(LowSbcutChar,"%.0f",LowSbcut);
  
       int iBinX = Chi2Array -> GetXaxis() -> FindBin(LowSbcut);
       Chi2Array -> GetXaxis() -> SetBinLabel(iBinX, ("[" + string(LowSbcutChar) + ",65]").c_str());
       
       
       float UpSbcut = UpSbcutMax - iUpSbcut * UpSbcutStep;
       char UpSbcutChar[10];
       sprintf(UpSbcutChar,"%.0f",UpSbcut);

       int iBinY = Chi2Array -> GetYaxis() -> FindBin(UpSbcut);
       Chi2Array -> GetYaxis() -> SetBinLabel(iBinY, ("[95," + string(UpSbcutChar) + "]").c_str());
  
    }
  }
  

  Chi2Array -> GetXaxis() -> SetTitle("Lower sb window");
  Chi2Array -> GetYaxis() -> SetTitle("Upper sb window");
  Chi2Array -> SetStats(0);
  Chi2Array -> SetContour(99);

  TCanvas *c1 = new TCanvas("c1","c1",600,400);
  Chi2Array -> Draw ("colz,text");
  c1->Print ("SidebandWindow_chi2.pdf", "pdf");
  
}


