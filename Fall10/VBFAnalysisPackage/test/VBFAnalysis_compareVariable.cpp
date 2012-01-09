#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "setTDRStyle.h"
#include "Functions.h"
#include "HiggsMassWindows.h"

#include <iostream>
#include <string>
#include <sstream>

#include "TH1F.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TF1.h"
#include "TLatex.h"



int nBins = 80;
float xMin = 100.;
float xMax = 900.;
float xWidth = (xMax-xMin)/nBins;

float lumi = 1.;

TFile* outFile;

float* xFitMin;
float* xFitMax;

void FillHistograms(TTree* tree_1, TTree* tree_2,
                    const std::string& label, std::map<std::string,TH1F*>& histoMap_1, std::map<std::string,TH1F*>& histoMap_2);

void DrawHistograms(std::map<std::string,TH1F*>& histoMap_1, std::map<std::string,TH1F*>& histoMap_2, const std::vector<int>& masses);






int main(int argc, char** argv)
{
  setTDRStyle();
  
  
  
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> VBFAnalysis_compareVariable::usage: " << argv[0] << " configFileName" << std::endl ;
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
  
  unsigned int nSigTrees  = inputSigDirs.size();
   
  
  //[Output]
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  
  // Define the output file
  std::string outputRootFullFileName = outputRootFilePath + "/" + outputRootFileName + "_" + jetAlgorithm + ".root";
  outFile = new TFile(outputRootFullFileName.c_str(), "RECREATE");
  
    
  //[Options]
  int step_1 = gConfigParser -> readIntOption("Options::step1");
  int step_2 = gConfigParser -> readIntOption("Options::step2");
  
  
  
  
  
  
  // define histograms
  std::vector<int> masses = GetMasses();
  unsigned int nMasses = masses.size();
  
  xFitMin = new float[nMasses];
  xFitMax = new float[nMasses];
  
  xFitMin[0] = 225.;
  xFitMin[1] = 265.;
  xFitMin[2] = 305.;
  xFitMin[3] = 345.;
  xFitMin[4] = 365.;
  xFitMin[5] = 395.;
  xFitMin[6] = 455.;
  xFitMin[7] = 475.;
  
  xFitMax[0] = 285.;
  xFitMax[1] = 345.;
  xFitMax[2] = 395.;
  xFitMax[3] = 455.;
  xFitMax[4] = 525.;
  xFitMax[5] = 565.;
  xFitMax[6] = 625.;
  xFitMax[7] = 700.;
  
  std::map<std::string,TH1F*> histoMap_1;
  std::map<std::string,TH1F*> histoMap_2;
  
  for(unsigned int iMass = 0; iMass < nMasses; ++iMass)
  {
    char labelName[50];
    sprintf(labelName,"M-%d",masses[iMass]);
    char histoName[50];
    
    sprintf(histoName,"histo_1_m%d",masses[iMass]);
    histoMap_1[std::string(labelName)] = new TH1F(histoName,"",nBins,xMin,xMax);
    histoMap_1[std::string(labelName)] -> Sumw2();
    histoMap_1[std::string(labelName)] -> GetXaxis() -> SetTitle("m(lepton+neutrino+WJets)   (GeV/c^{2})");
    histoMap_1[std::string(labelName)] -> GetYaxis() -> SetTitle("events");
    histoMap_1[std::string(labelName)] -> SetFillStyle(3001);
    histoMap_1[std::string(labelName)] -> SetLineColor(kBlue);
    histoMap_1[std::string(labelName)] -> SetLineWidth(2);
    
    sprintf(histoName,"histoMap_2_m%d",masses[iMass]);
    histoMap_2[std::string(labelName)] = new TH1F(histoName,"",nBins,xMin,xMax);
    histoMap_2[std::string(labelName)] -> Sumw2(); 
    histoMap_2[std::string(labelName)] -> GetXaxis() -> SetTitle("m(lepton+neutrino+WJets)   (GeV/c^{2})");
    histoMap_2[std::string(labelName)] -> GetYaxis() -> SetTitle("events");
    histoMap_2[std::string(labelName)] -> SetFillStyle(3001);
    histoMap_2[std::string(labelName)] -> SetLineColor(kRed);
    histoMap_2[std::string(labelName)] -> SetLineWidth(2);
  }
  
  
  
  
  
  // loop on sig samples
  for(unsigned int i = 0; i < nSigTrees; ++i)
  {
    // open root file
    std::string inputFullFileName;
    inputFullFileName = baseDir + "/" + inputSigDirs.at(i)   + "/" + inputFileName + ".root";
    TFile* inputFile = TFile::Open(inputFullFileName.c_str());
    std::cout << ">>>>>> VBFAnalysis_compareVariable::sig tree in " << inputSigDirs.at(i) << " opened" << std::endl;
    
    
    // get the tree at nth step
    TTree* tree_1 = NULL;
    char treeName_1[50];
    sprintf(treeName_1, "ntu_%d", step_1);
    inputFile -> GetObject(treeName_1, tree_1);
    if ( tree_1 -> GetEntries() == 0 ) continue;
    
    TTree* tree_2 = NULL;
    char treeName_2[50];
    sprintf(treeName_2, "ntu_%d", step_2);
    inputFile -> GetObject(treeName_2, tree_2);
    if ( tree_2 -> GetEntries() == 0 ) continue;
    
    
    std::string token1,token2,token3;
    std::istringstream iss(inputSigDirs.at(i));
    getline(iss,token1,'_');
    getline(iss,token2,'_');
    getline(iss,token3,'_');
    
    // fill histograms
    if( (token2 == "M-250") || (token3 == "M-250") )
      FillHistograms(tree_1, tree_2,"M-250",histoMap_1,histoMap_2);
    if( (token2 == "M-300") || (token3 == "M-300") )
      FillHistograms(tree_1, tree_2,"M-300",histoMap_1,histoMap_2);
    if( (token2 == "M-350") || (token3 == "M-350") )
      FillHistograms(tree_1, tree_2,"M-350",histoMap_1,histoMap_2);
    if( (token2 == "M-400") || (token3 == "M-400") )
      FillHistograms(tree_1, tree_2,"M-400",histoMap_1,histoMap_2);
    if( (token2 == "M-450") || (token3 == "M-450") )
      FillHistograms(tree_1, tree_2,"M-450",histoMap_1,histoMap_2);
    if( (token2 == "M-500") || (token3 == "M-500") )
      FillHistograms(tree_1, tree_2,"M-500",histoMap_1,histoMap_2);
    if( (token2 == "M-550") || (token3 == "M-550") )
      FillHistograms(tree_1, tree_2,"M-550",histoMap_1,histoMap_2);
    if( (token2 == "M-600") || (token3 == "M-600") )
      FillHistograms(tree_1, tree_2,"M-600",histoMap_1,histoMap_2);
  }
  
  
  
  DrawHistograms(histoMap_1,histoMap_2,masses);
  return 0;
}






void FillHistograms(TTree* tree_1, TTree* tree_2, 
                    const std::string& label, std::map<std::string,TH1F*>& histoMap_1, std::map<std::string,TH1F*>& histoMap_2)
{
  outFile -> cd();
  
  // define common cut and weight
  std::string commonCut = "(1 == 1) * ((WJJ_m >= 65.) && (WJJ_m < 95.))";
  char lumiChar[50]; sprintf(lumiChar,"%f",lumi);
  std::string weight = "( 1000 * " + std::string(lumiChar) + " * 1. / totEvents * crossSection * PURescaleFactor(PUit_n) )";
  
    
  std::string histoName_1 = (histoMap_1[label]) -> GetName();
  tree_1 -> Draw(("lepNuW_m_KF >>+ "+histoName_1).c_str(),(weight+" * "+commonCut).c_str(),"goff");
  
  std::string histoName_2 = (histoMap_2[label]) -> GetName();
  tree_2 -> Draw(("lepNuW_m_KF >>+ "+histoName_2).c_str(),(weight+" * "+commonCut).c_str(),"goff");
}






void DrawHistograms(std::map<std::string,TH1F*>& histoMap_1, std::map<std::string,TH1F*>& histoMap_2, const std::vector<int>& masses)
{
  int iMass = 0;
  for(std::map<std::string,TH1F*>::const_iterator mapIt = histoMap_1.begin(); mapIt != histoMap_1.end(); ++mapIt)
  {
    std::string label = mapIt -> first;
    int mass = masses[iMass];
    
    // define canvas
    TCanvas* c = new TCanvas(("c_"+label).c_str());
    c -> cd();
    c -> SetGridx();
    c -> SetGridy();
    
    float maximum = -1;
    
    
    // define legend
    TLegend* legend = new TLegend(0.55,0.75,0.74,0.90);
    legend -> SetFillColor(kWhite);
    legend -> SetLineColor(kWhite);
    legend -> SetFillStyle(1001);
    legend -> SetTextFont(42);
    legend -> SetTextSize(0.025);
    
    
    // draw plots
    histoMap_1[label] -> Draw("HISTO");
    legend -> AddEntry(histoMap_1[label],"FNAL KF","L");
    if( histoMap_1[label]->GetMaximum() > maximum ) maximum = histoMap_1[label] -> GetMaximum();
    
    histoMap_2[label] -> Draw("HISTO,same");
    legend -> AddEntry(histoMap_2[label],"MIB KF","L");
    if( histoMap_2[label]->GetMaximum() > maximum ) maximum = histoMap_2[label] -> GetMaximum();
    
    histoMap_1[label] -> GetYaxis() -> SetRangeUser(0.,1.1*maximum);
    legend -> Draw("same");
    
    
    // fit histograms
    TF1* fitFunc_1 = new TF1(("fitFunc_1_"+label).c_str(),breitWigner_gaussian,xFitMin[iMass],xFitMax[iMass],5);
    
    fitFunc_1 -> SetParameter(0,histoMap_1[label]->GetMaximum());
    fitFunc_1 -> FixParameter(1,mass);
    fitFunc_1 -> FixParameter(2,GetHiggsWidth(mass));
    fitFunc_1 -> SetParameter(3,0.);
    fitFunc_1 -> SetParameter(4,20.);
    
    fitFunc_1 -> SetNpx(10000);
    fitFunc_1 -> SetLineColor(kBlue);
    
    histoMap_1[label] -> Fit(("fitFunc_1_"+label).c_str(),"NRQ+");
    fitFunc_1 -> Draw("same");
    
    
    TF1* fitFunc_2 = new TF1(("fitFunc_2_"+label).c_str(),breitWigner_gaussian,xFitMin[iMass],xFitMax[iMass],5);
    
    fitFunc_2 -> SetParameter(0,histoMap_2[label]->GetMaximum());
    fitFunc_2 -> FixParameter(1,mass);
    fitFunc_2 -> FixParameter(2,GetHiggsWidth(mass));
    fitFunc_2 -> SetParameter(3,0.);
    fitFunc_2 -> SetParameter(4,20.);
    
    fitFunc_2 -> SetNpx(10000);
    fitFunc_2 -> SetLineColor(kRed);
    
    histoMap_2[label] -> Fit(("fitFunc_2_"+label).c_str(),"QNR+");
    fitFunc_2 -> Draw("same");
    
    
    
    char latexBuffer[250];
    
    sprintf(latexBuffer,"M = %d GeV/c^{2}   #Gamma = %.2e",masses[iMass],GetHiggsWidth(mass));
    TLatex* latex_bw = new TLatex(0.13,0.96,latexBuffer);

    latex_bw -> SetNDC();
    latex_bw -> SetTextFont(42);
    latex_bw -> SetTextSize(0.03);
    latex_bw -> Draw("same");
    
    sprintf(latexBuffer,"#mu = %.1f GeV/c^{2}   #sigma = %.2e GeV/c^{2} (%.1f %%)",fitFunc_1->GetParameter(3),fitFunc_1->GetParameter(4),fitFunc_1->GetParameter(4)/masses[iMass]*100);
    TLatex* latex_1 = new TLatex(0.15,0.90,latexBuffer);
    latex_1 -> SetNDC();
    latex_1 -> SetTextFont(42);
    latex_1 -> SetTextColor(kBlue);
    latex_1 -> SetTextSize(0.02);
    latex_1 -> Draw("same");  
    
    sprintf(latexBuffer,"#mu = %.1f GeV/c^{2}   #sigma = %.2e GeV/c^{2} (%.1f %%)",fitFunc_2->GetParameter(3),fitFunc_2->GetParameter(4),fitFunc_2->GetParameter(4)/masses[iMass]*100);
    TLatex* latex_2 = new TLatex(0.15,0.85,latexBuffer);
    latex_2 -> SetNDC();
    latex_2 -> SetTextFont(42);
    latex_2 -> SetTextColor(kRed);
    latex_2 -> SetTextSize(0.02);
    latex_2 -> Draw("same");  


    c -> Print(("compareVariable_"+label+".pdf").c_str(),"pdf");
    
    
    ++iMass;
  }
  
}
