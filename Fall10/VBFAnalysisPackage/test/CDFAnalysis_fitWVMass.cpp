#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "plotUtils.h"
#include "histoFunc.h"
#include "setTDRStyle.h"

#include <iostream>
#include <vector>
#include <map>

#include "TH1F.h"
#include "TF1.h"
#include "THStack.h"
#include "TLegend.h"



std::map<std::string,TF1*> bkgFuncSummed;
std::map<std::string,TF1*> sigFuncSummed;

double fitFunc(double* x, double* par);






int main(int argc, char** argv)
{
  setTDRStyle();
  
  
  
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> CDFAnalysis_fitWVMass::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
  
  
  //[Input]
  std::string baseDir         = gConfigParser -> readStringOption("Input::baseDir");
  std::string jetAlgorithm    = gConfigParser -> readStringOption("Input::jetAlgorithm");
  std::string jetType         = gConfigParser -> readStringOption("Input::jetType");
  std::string samplesListFile = gConfigParser -> readStringOption("Input::samplesListFile");
  float lumi = gConfigParser -> readFloatOption("Input::lumi");
  
  std::string inputFileName = gConfigParser -> readStringOption("Input::inputFileName");
  
  
  //[Output]
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  
  
  //[Options]
  int step        = gConfigParser -> readIntOption("Options::step");
  
  
  //[Cuts]
  float xFitMIN = gConfigParser -> readFloatOption("Cuts::xFitMIN");
  float xFitMAX = gConfigParser -> readFloatOption("Cuts::xFitMAX");
  
  int nBins = 30;
  float xMin = 0.;
  float xMax = 300.;
  float xWidth = (xMax-xMin)/nBins;
  
  int nBinsFit = int((xFitMAX-xFitMIN)/xWidth);
  
  
  
  
  
  
  // Define the histograms
  TH1F* h_WJJ_m = new TH1F("h_WJJ_m","",nBins,xMin,xMax);
  h_WJJ_m -> Sumw2();
  
  TH1F* h_mcSum_WJJ_m = new TH1F("h_mcSum_WJJ_m","",nBins,xMin,xMax);
  h_mcSum_WJJ_m -> Sumw2();
  
  TH1F* h_sig_WJJ_m = new TH1F("h_sig_WJJ_m","",nBins,xMin,xMax);
  h_sig_WJJ_m -> Sumw2();
  
  
  
  // Define the output file
  std::string outputRootFullFileName = outputRootFilePath + "/" + outputRootFileName + "_" + jetAlgorithm + ".root";
  TFile* outFile = new TFile(outputRootFullFileName.c_str(), "RECREATE");
  
  
  
  drawTStack* stack = new drawTStack(baseDir,samplesListFile,inputFileName,outputRootFilePath,"pdf");
  
  std::vector<std::string> variableNames(1);
  variableNames.at(0) = "WJJ_m";
  std::vector<std::string>* cuts = new std::vector<std::string>;
  cuts->push_back("chi2_KF < 10000.");
  stack -> SetXaxisRange(xMin,xMax);
  stack -> SetXaxisTitle("mass(WJet_{1}+WJet_{2})");
  stack -> SetUnit("GeV/c^{2}");
  stack -> MakeHistograms(variableNames,"WJJ_m","eventsScaled",lumi,step,nBins,true,true,true,cuts);
  
  THStack* bkgStack = stack -> GetBkgStack();
  TLegend* legend = stack -> GetLegend();
  
  std::map<std::string,TH1F*> bkgHistoSummed  = stack -> GetBkgHistoSummed();
  std::map<std::string,TH1F*> sigHistoSummed  = stack -> GetSigHistoSummed();
  
  TH1F* dataGlobalHisto = stack -> GetDataGlobalHisto();
  
  
  
  //------------
  // bkg samples
  for(std::map<std::string,TH1F*>::const_iterator mapIt = bkgHistoSummed.begin(); mapIt != bkgHistoSummed.end(); ++mapIt)
  {
    TH1F* dummyHisto = (TH1F*)( (mapIt->second)->Clone(("histoScaled_"+mapIt->first).c_str()) );
    dummyHisto -> Scale( 1./dummyHisto->Integral() );
    
    histoFunc* dummyHF = new histoFunc(dummyHisto);
    bkgFuncSummed[mapIt->first] = new TF1(("func"+mapIt->first).c_str(),dummyHF,xMin,xMax,3,"histoFunc");
    
    bkgFuncSummed[mapIt->first] -> SetParName(0,"N");
    bkgFuncSummed[mapIt->first] -> SetParName(1,"#alpha"); 
    bkgFuncSummed[mapIt->first] -> SetParName(1,"x_{0}"); 
    bkgFuncSummed[mapIt->first] -> FixParameter(1, 1);
    bkgFuncSummed[mapIt->first] -> FixParameter(2, 0.);
    
    bkgFuncSummed[mapIt->first] -> SetNpx(10000);
    bkgFuncSummed[mapIt->first] -> SetLineWidth(2);
    bkgFuncSummed[mapIt->first] -> SetLineColor(stack->GetColorSummed(mapIt->first));
    
    if( mapIt->first != "5_W+jets_madgraph" )
      bkgFuncSummed[mapIt->first] -> FixParameter(0,(mapIt->second)->Integral());
    else
      bkgFuncSummed[mapIt->first] -> SetParameter(0,(mapIt->second)->Integral());
  }
  
  
  //------------
  // sig samples
  for(std::map<std::string,TH1F*>::const_iterator mapIt = sigHistoSummed.begin(); mapIt != sigHistoSummed.end(); ++mapIt)
  {
    TH1F* dummyHisto = (TH1F*)( (mapIt->second)->Clone(("histoScaled_"+mapIt->first).c_str()) );
    dummyHisto -> Scale( 1./dummyHisto->Integral() );
    
    histoFunc* dummyHF = new histoFunc(dummyHisto);
    sigFuncSummed[mapIt->first] = new TF1(("func"+mapIt->first).c_str(),dummyHF,xMin,xMax,3,"histoFunc");
    
    sigFuncSummed[mapIt->first] -> SetParName(0,"N");
    sigFuncSummed[mapIt->first] -> SetParName(1,"#alpha"); 
    sigFuncSummed[mapIt->first] -> SetParName(1,"x_{0}"); 
    sigFuncSummed[mapIt->first] -> FixParameter(1, 1);
    sigFuncSummed[mapIt->first] -> FixParameter(2, 0.);
    
    sigFuncSummed[mapIt->first] -> SetNpx(10000);
    sigFuncSummed[mapIt->first] -> SetLineWidth(2);
    sigFuncSummed[mapIt->first] -> SetLineColor(stack->GetColorSummed(mapIt->first));
        
    sigFuncSummed[mapIt->first] -> SetParameter(0,(mapIt->second)->Integral());
  }
  
  
  
  //-------------
  // fit function
  TF1* func = new TF1("func",fitFunc,xMin,xMax,2);
  
  func -> SetParameter(0,(bkgHistoSummed["5_W+jets_madgraph"])->Integral());
  func -> SetParameter(1,(sigHistoSummed["6_WV"])->Integral());
  
  func -> SetParName(0,"N_W+jets");
  func -> SetParName(1,"N_WV");
  func -> SetNpx(10000);
  func -> SetLineColor(kRed);
  func -> SetLineWidth(2);
  
  dataGlobalHisto -> Fit("func","QRLS0+");
  
  
  
  //--------------
  // print results
  std::cout << "*******************" << std::endl;
  std::cout << ">>> FIT RESULTS <<<" << std::endl;
  std::cout << "*******************" << std::endl;
  std::cout << std::endl;
  std::cout << "Number of ---W+jets--- events in MC: " << (bkgHistoSummed["5_W+jets_madgraph"])->Integral() << std::endl;
  std::cout << "Number of   ---WV---   events in MC: " << (sigHistoSummed["6_WV"])->Integral() << std::endl;
  std::cout << std::endl;
  std::cout << "Number of ---W+jets--- events from the fit: " << func->GetParameter(0) << std::endl;
  std::cout << "Number of   ---WV---   events from the fit: " << func->GetParameter(1) << std::endl;
  
  
  
  //--------------------
  // subtract MC to data
  TH1F* dataGlobalHistoSub = (TH1F*)( dataGlobalHisto->Clone("dataGlobalHistoSub") );
  dataGlobalHistoSub -> Reset();
  for(int bin = 1; bin <= dataGlobalHistoSub->GetNbinsX(); ++bin)
  {
    float sumMC = 0;
    for(std::map<std::string,TF1*>::iterator mapIt = bkgFuncSummed.begin(); mapIt != bkgFuncSummed.end(); ++mapIt)
      sumMC += (mapIt->second) -> Eval(dataGlobalHistoSub->GetBinCenter(bin));
    
    dataGlobalHistoSub -> SetBinContent(bin,dataGlobalHisto->GetBinContent(bin)-sumMC);
    dataGlobalHistoSub -> SetBinError(bin,dataGlobalHisto->GetBinError(bin));
  }
  
  
  
  
  
  
  TCanvas* c1 = new TCanvas("c1","c1",600,600);
  
  bkgStack -> Draw("HIST");
  dataGlobalHisto -> Draw("same");
  legend -> Draw("same");
  func -> Draw("same");
  sigFuncSummed["6_WV"] -> Draw("same");
  
  c1 -> Print("fitWVMass.pdf","pdf");
  outFile -> cd();
  
  bkgStack -> Write();
  legend -> Write();
  
  outFile -> Close();
  
  
  
  TCanvas* c2 = new TCanvas("c2","c2",600,600);
  
  sigHistoSummed["6_WV"] -> Draw("HIST");
  sigHistoSummed["6_WV"] -> SetMinimum(-200.);
  sigHistoSummed["6_WV"] -> SetMaximum(+500.);
  sigHistoSummed["6_WV"] -> SetFillColor(stack->GetColorSummed("6_WV"));
  sigFuncSummed["6_WV"] -> SetLineColor(kBlue);
  sigFuncSummed["6_WV"] -> Draw("same");
  dataGlobalHistoSub -> Draw("P,same");
      
  c2 -> Print("fitWVMassSubtracted.pdf","pdf");
  
  
  
  return 0;
}






double fitFunc(double* x, double* par)
{
  // variable
  double xx = x[0];
  // parameters
  double N_WJets = par[0];
  double N_WV = par[1];
  //std::cout << "x: " << xx << "   N_WJets: " << N_WJets << "   N_Wv: " << N_WV << std::endl;
  
  
  // set parameters
  for(std::map<std::string,TF1*>::iterator mapIt = bkgFuncSummed.begin();
      mapIt != bkgFuncSummed.end(); ++mapIt)
  {
    if( mapIt->first == "5_W+jets_madgraph")
      (mapIt->second) -> SetParameter(0,N_WJets);
  }
  
  for(std::map<std::string,TF1*>::iterator mapIt = sigFuncSummed.begin();
      mapIt != sigFuncSummed.end(); ++mapIt)
  {
    (mapIt->second) -> SetParameter(0,N_WV);
  }
  
  
  // return value
  double val = 0.;
  
  for(std::map<std::string,TF1*>::iterator mapIt = bkgFuncSummed.begin();
      mapIt != bkgFuncSummed.end(); ++mapIt)
    val += (mapIt->second) -> Eval(xx);
  
  for(std::map<std::string,TF1*>::iterator mapIt = sigFuncSummed.begin();
      mapIt != sigFuncSummed.end(); ++mapIt)
    val += (mapIt->second) -> Eval(xx);
  
  return val;
}
