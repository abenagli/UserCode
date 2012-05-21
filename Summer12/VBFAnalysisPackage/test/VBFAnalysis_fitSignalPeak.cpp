#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "setTDRStyle.h"
#include "Functions.h"

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

int nMasses = 8;
int* masses = new int[nMasses];
float* widths = new float[nMasses];
float* xFitMin = new float[nMasses];
float* xFitMax = new float[nMasses];



void FillHistograms(TTree* tree,
                    const std::string& label, std::map<std::string,TH1F*>& h_lepNuW_m, std::map<std::string,TH1F*>& h_lepNuW_m_KF);

void DrawHistograms(std::map<std::string,TH1F*>& h_lepNuW_m, std::map<std::string,TH1F*>& h_lepNuW_m_KF);






int main(int argc, char** argv)
{
  setTDRStyle();
  
  
  
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> VBFAnalysis_fitSignalPeak::usage: " << argv[0] << " configFileName" << std::endl ;
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
  int step = gConfigParser -> readIntOption("Options::step");
  
  
  
  
  
  
  // define histograms
  masses[0] = 250;
  masses[1] = 300;
  masses[2] = 350;
  masses[3] = 400;
  masses[4] = 450;
  masses[5] = 500;
  masses[6] = 550;
  masses[7] = 600;
  
  widths[0] = 4.04;
  widths[1] = 8.43;
  widths[2] = 15.2;
  widths[3] = 29.2;
  widths[4] = 47.0;
  widths[5] = 68.0;
  widths[6] = 93.2;
  widths[7] = 123.;
  
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
  
  
  
  std::map<std::string,TH1F*> h_lepNuW_m;
  std::map<std::string,TH1F*> h_lepNuW_m_KF;
  
  for(int iMass = 0; iMass < nMasses; ++iMass)
  {
    char labelName[50];
    sprintf(labelName,"M-%d",masses[iMass]);
    char histoName[50];
    
    sprintf(histoName,"h_lepNuW_m%d",masses[iMass]);
    h_lepNuW_m[std::string(labelName)] = new TH1F(histoName,"",nBins,xMin,xMax);
    h_lepNuW_m[std::string(labelName)] -> Sumw2();
    h_lepNuW_m[std::string(labelName)] -> GetXaxis() -> SetTitle("m(lepton+neutrino+WJets)   (GeV/c^{2})");
    h_lepNuW_m[std::string(labelName)] -> GetYaxis() -> SetTitle("events");
    h_lepNuW_m[std::string(labelName)] -> SetFillStyle(3001);
    h_lepNuW_m[std::string(labelName)] -> SetFillColor(41);
    h_lepNuW_m[std::string(labelName)] -> SetLineColor(kRed);
    
    sprintf(histoName,"h_lepNuW_m%d_KF",masses[iMass]);
    h_lepNuW_m_KF[std::string(labelName)] = new TH1F(histoName,"",nBins,xMin,xMax);
    h_lepNuW_m_KF[std::string(labelName)] -> Sumw2(); 
    h_lepNuW_m_KF[std::string(labelName)] -> GetXaxis() -> SetTitle("m(lepton+neutrino+WJets)   (GeV/c^{2})");
    h_lepNuW_m_KF[std::string(labelName)] -> GetYaxis() -> SetTitle("events");
    h_lepNuW_m_KF[std::string(labelName)] -> SetFillStyle(3001);
    h_lepNuW_m_KF[std::string(labelName)] -> SetLineColor(kBlue);
  }
  
  
  
  
  
  // loop on sig samples
  for(unsigned int i = 0; i < nSigTrees; ++i)
  {
    // open root file
    std::string inputFullFileName;
    inputFullFileName = baseDir + "/" + inputSigDirs.at(i)   + "/" + inputFileName + ".root";
    TFile* inputFile = TFile::Open(inputFullFileName.c_str());
    std::cout << ">>>>>> VBFAnalysis_fitSignalPeak::sig tree in " << inputSigDirs.at(i) << " opened" << std::endl;
    
    
    // get the tree at nth step
    TTree* tree = NULL;
    char treeName[50];
    sprintf(treeName, "ntu_%d", step);
    inputFile -> GetObject(treeName, tree);
    if ( tree -> GetEntries() == 0 ) continue;
    
    
    std::string token1,token2,token3;
    std::istringstream iss(inputSigDirs.at(i));
    getline(iss,token1,'_');
    getline(iss,token2,'_');
    getline(iss,token3,'_');
    
    // fill histograms
    if( (token2 == "M-250") || (token3 == "M-250") )
      FillHistograms(tree,"M-250",h_lepNuW_m,h_lepNuW_m_KF);
    if( (token2 == "M-300") || (token3 == "M-300") )
      FillHistograms(tree,"M-300",h_lepNuW_m,h_lepNuW_m_KF);
    if( (token2 == "M-350") || (token3 == "M-350") )
      FillHistograms(tree,"M-350",h_lepNuW_m,h_lepNuW_m_KF);
    if( (token2 == "M-400") || (token3 == "M-400") )
      FillHistograms(tree,"M-400",h_lepNuW_m,h_lepNuW_m_KF);
    if( (token2 == "M-450") || (token3 == "M-450") )
      FillHistograms(tree,"M-450",h_lepNuW_m,h_lepNuW_m_KF);
    if( (token2 == "M-500") || (token3 == "M-500") )
      FillHistograms(tree,"M-500",h_lepNuW_m,h_lepNuW_m_KF);
    if( (token2 == "M-550") || (token3 == "M-550") )
      FillHistograms(tree,"M-550",h_lepNuW_m,h_lepNuW_m_KF);
    if( (token2 == "M-600") || (token3 == "M-600") )
      FillHistograms(tree,"M-600",h_lepNuW_m,h_lepNuW_m_KF);
  }
  
  
  
  DrawHistograms(h_lepNuW_m,h_lepNuW_m_KF);
  
  outFile -> Close();
  
  return 0;
}






void FillHistograms(TTree* tree, 
                    const std::string& label, std::map<std::string,TH1F*>& h_lepNuW_m, std::map<std::string,TH1F*>& h_lepNuW_m_KF)
{
  outFile -> cd();
  
  // define common cut and weight
  std::string commonCut = "(1 == 1) * ((WJJ_m >= 65.) && (WJJ_m < 95.))";
  char lumiChar[50]; sprintf(lumiChar,"%f",lumi);
  std::string weight = "( 1000 * " + std::string(lumiChar) + " * 1. / totEvents * crossSection * eventWeight * PUWeight )";
  
    
  std::string histoName = (h_lepNuW_m[label]) -> GetName();
  tree -> Draw(("lepNuW_m >>+ "+histoName).c_str(),(weight+" * "+commonCut).c_str(),"goff");
  
  std::string histoName_KF = (h_lepNuW_m_KF[label]) -> GetName();
  tree -> Draw(("lepNuW_m_KF >>+ "+histoName_KF).c_str(),(weight+" * "+commonCut).c_str(),"goff");
}






void DrawHistograms(std::map<std::string,TH1F*>& h_lepNuW_m, std::map<std::string,TH1F*>& h_lepNuW_m_KF)
{
  int iMass = 0;
  for(std::map<std::string,TH1F*>::const_iterator mapIt = h_lepNuW_m.begin(); mapIt != h_lepNuW_m.end(); ++mapIt)
  {
    std::string label = mapIt -> first;
    
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
    h_lepNuW_m[label] -> Draw("HISTO");
    legend -> AddEntry(h_lepNuW_m[label],"before KF","F");
    if( h_lepNuW_m[label]->GetMaximum() > maximum ) maximum = h_lepNuW_m[label] -> GetMaximum();
    
    h_lepNuW_m_KF[label] -> Draw("HISTO,same");
    legend -> AddEntry(h_lepNuW_m_KF[label],"after KF","L");
    if( h_lepNuW_m_KF[label]->GetMaximum() > maximum ) maximum = h_lepNuW_m_KF[label] -> GetMaximum();
    
    h_lepNuW_m[label] -> GetYaxis() -> SetRangeUser(0.,1.1*maximum);
    legend -> Draw("same");
    
    
    // fit histograms
    TF1* fitFunc = new TF1(("fitFunc_"+label).c_str(),breitWigner_gaussian,xFitMin[iMass],xFitMax[iMass],5);
    
    fitFunc -> SetParameter(0,h_lepNuW_m[label]->GetMaximum());
    fitFunc -> FixParameter(1,masses[iMass]);
    fitFunc -> FixParameter(2,widths[iMass]);
    fitFunc -> SetParameter(3,0.);
    fitFunc -> SetParameter(4,20.);
    
    fitFunc -> SetNpx(10000);
    fitFunc -> SetLineColor(kRed);
    
    h_lepNuW_m[label] -> Fit(("fitFunc_"+label).c_str(),"NRQ+");
    fitFunc -> Draw("same");
    
    
    TF1* fitFunc_KF = new TF1(("fitFunc_KF_"+label).c_str(),breitWigner_gaussian,xFitMin[iMass],xFitMax[iMass],5);
    
    fitFunc_KF -> SetParameter(0,h_lepNuW_m_KF[label]->GetMaximum());
    fitFunc_KF -> FixParameter(1,masses[iMass]);
    fitFunc_KF -> FixParameter(2,widths[iMass]);
    fitFunc_KF -> SetParameter(3,0.);
    fitFunc_KF -> SetParameter(4,20.);
    
    fitFunc_KF -> SetNpx(10000);
    fitFunc_KF -> SetLineColor(kBlue);
    
    h_lepNuW_m_KF[label] -> Fit(("fitFunc_KF_"+label).c_str(),"QNR+");
    fitFunc_KF -> Draw("same");
    
    
    
    char latexBuffer[250];
    
    sprintf(latexBuffer,"M = %d GeV/c^{2}   #Gamma = %.2e",masses[iMass],widths[iMass]);
    TLatex* latex_bw = new TLatex(0.13,0.96,latexBuffer);

    latex_bw -> SetNDC();
    latex_bw -> SetTextFont(42);
    latex_bw -> SetTextSize(0.03);
    latex_bw -> Draw("same");
    
    sprintf(latexBuffer,"#mu = %.1f GeV/c^{2}   #sigma = %.2e GeV/c^{2} (%.1f %%)",fitFunc->GetParameter(3),fitFunc->GetParameter(4),fitFunc->GetParameter(4)/masses[iMass]*100);
    TLatex* latex = new TLatex(0.15,0.92,latexBuffer);
    latex -> SetNDC();
    latex -> SetTextFont(42);
    latex -> SetTextColor(kRed);
    latex -> SetTextSize(0.02);
    latex -> Draw("same");  
    
    sprintf(latexBuffer,"#mu = %.1f GeV/c^{2}   #sigma = %.2e GeV/c^{2} (%.1f %%)",fitFunc_KF->GetParameter(3),fitFunc_KF->GetParameter(4),fitFunc_KF->GetParameter(4)/masses[iMass]*100);
    TLatex* latex_KF = new TLatex(0.15,0.89,latexBuffer);
    latex_KF -> SetNDC();
    latex_KF -> SetTextFont(42);
    latex_KF -> SetTextColor(kBlue);
    latex_KF -> SetTextSize(0.02);
    latex_KF -> Draw("same");  


    outFile -> cd();
    
    h_lepNuW_m[label] -> Write();
    h_lepNuW_m_KF[label] -> Write();
    fitFunc -> Write();
    fitFunc_KF -> Write();
    
    c -> Print(("fitSignalPeak_"+label+".pdf").c_str(),"pdf");
    
    
    ++iMass;
  }
  
}
