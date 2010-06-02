#include "setTDRStyle.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "plotUtils.h"

#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"

#include <vector>
#include <iostream>
#include <string>
#include <fstream>






int main(int argc, char** argv)
{
  setTDRStyle();
   
  
  
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> VBFPrintPlots::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
  
  std::string inputDir = gConfigParser -> readStringOption("Input::inputDir");
  
  std::string outputDir = gConfigParser -> readStringOption("Output::outputDir");

  int step = gConfigParser -> readIntOption("Options::step");  
  //float lumi = gConfigParser -> readFloatOption("Options::lumi");
  
  
  
  
  
  // draw plots
  drawTStack* stack = new drawTStack(inputDir, "listOfSamplesAndCrossSections.txt", "VBFHiggsToWWTolnujjAnalysis", outputDir);
  
  std::string histoName;
  
  
  
  
  
  
  //---------------------------------------------  
  // tag jets
  //---------------------------------------------  
  
  histoName = "tagJJ_m";
  stack -> SetXaxisRange(0., 3000.);
  stack -> SetXaxisTitle("m_{jj tag}");
  stack -> Draw(histoName, step, 20., false);
  
  histoName = "tagJJ_Deta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#Delta#eta_{jj tag}");
  stack -> Draw(histoName, step, 5., false);

  histoName = "tagJJ_etaProd";
  stack -> SetXaxisRange(-25., 25.);
  stack -> SetXaxisTitle("#etaProd_{jj tag}");
  stack -> Draw(histoName, step, 20., false);
    
  histoName = "tagJJ_Dphi";
  stack -> SetXaxisRange(0., 3.);
  stack -> SetXaxisTitle("#Delta#phi_{jj tag}");
  stack -> Draw(histoName, step, 5., false);
  
  histoName = "tagJJ_max_absEta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("|#eta_{tag j}| max");
  stack -> Draw(histoName, step, 5., false);
  
  histoName = "tagJJ_min_absEta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("|#eta_{tag j}| min");
  stack -> Draw(histoName, step, 5., false);
  
  histoName = "tagJJ_max_et";
  stack -> SetXaxisRange(0., 400.);
  stack -> SetXaxisTitle("et_{j tag} max");
  stack -> Draw(histoName, step, 5., false);
  
  histoName = "tagJJ_min_et";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("et_{j tag} min");
  stack -> Draw(histoName, step, 5., false);
  
  histoName = "tagJJ_max_e";
  stack -> SetXaxisRange(0., 2000.);
  stack -> SetXaxisTitle("e_{j tag} max");
  stack -> Draw(histoName, step, 20., false);
  
  histoName = "tagJJ_min_e";
  stack -> SetXaxisRange(0., 1000.);
  stack -> SetXaxisTitle("e_{j tag} min");
  stack -> Draw(histoName, step, 10., false);
  
  histoName = "tagJJ_bTag";
  stack -> SetXaxisTitle("b-tag");
  stack -> SetXaxisRange(-10., 30.);
  stack -> SetXaxisTitle("btag_{j tag}");
  stack -> Draw(histoName, step, 5, true);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // Wjets
  //---------------------------------------------  
  
  histoName = "WJJ_m";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("m_{Wjj}");
  stack -> Draw(histoName, step, 5., false);
  
  histoName = "WJJ_et";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("et_{Wjj}");
  stack -> Draw(histoName, step, 5, false);
  
  histoName = "WJJ_absEta";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("|#eta_{Wjj}|");
  stack -> Draw(histoName, step, 5., false);
  
  histoName = "WJJ_zepp";
  stack -> SetXaxisRange(0., 2.);
  stack -> SetXaxisTitle("z_{Wjj}");
  stack -> Draw(histoName, step, 5., false);
  
  histoName = "WJJ_Dphi";
  stack -> SetXaxisRange(0., 3.);
  stack -> SetXaxisTitle("#Delta#phi_{Wjj}");
  stack -> Draw(histoName, step, 5., false);
  
  histoName = "WJJ_Deta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#Delta#eta_{Wjj}");
  stack -> Draw(histoName, step, 5., false);
  
  histoName = "WJJ_DR";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#DeltaR_{Wjj}");
  stack -> Draw(histoName, step, 5., false);
  
  
  
  histoName = "WJJ_max_et";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("et_{Wj} max");
  stack -> Draw(histoName, step, 5, false);  
  
  histoName = "WJJ_min_et";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("et_{Wj} min");
  stack -> Draw(histoName, step, 5, false);
  
  histoName = "WJJ_max_e";
  stack -> SetXaxisRange(0., 2000.);
  stack -> SetXaxisTitle("e_{Wj} max");
  stack -> Draw(histoName, step, 20, false);  
  
  histoName = "WJJ_min_e";
  stack -> SetXaxisRange(0., 1000.);
  stack -> SetXaxisTitle("e_{Wj} min");
  stack -> Draw(histoName, step, 10, false);
  
  histoName = "WJJ_max_absEta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("|#eta_{Wj}| max");
  stack -> Draw(histoName, step, 5., false);
  
  histoName = "WJJ_min_absEta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("|#eta_{Wj}| min");
  stack -> Draw(histoName, step, 5., false);
  
  histoName = "WJJ_max_zepp";
  stack -> SetXaxisRange(0., 2.);
  stack -> SetXaxisTitle("z_{Wj} max");
  stack -> Draw(histoName, step, 5, false);  
  
  histoName = "WJJ_min_zepp";
  stack -> SetXaxisRange(0., 2.);
  stack -> SetXaxisTitle("z_{Wj} min");
  stack -> Draw(histoName, step, 5, false);
  
  histoName = "WJJ_bTag";
  stack -> SetXaxisRange(-10., 30.);
  stack -> SetXaxisTitle("b-tag_{W j}");
  stack -> Draw(histoName, step, 5, true);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // Lepton
  //---------------------------------------------  
  
  histoName = "lep_pt";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("pt_{lep}");
  stack -> Draw(histoName, step, 5., false);
  
  histoName = "lep_absEta";
  stack -> SetXaxisRange(0., 3.);
  stack -> SetXaxisTitle("|#eta_{lep}|");
  stack -> Draw(histoName, step, 2., false);
  
  histoName = "lep_zepp";
  stack -> SetXaxisRange(0., 1.);
  stack -> SetXaxisTitle("z_{lep}");
  stack -> Draw(histoName, step, 5., false);
  
  histoName = "lep_etaGap_min";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("#eta gap_{lep}");
  stack -> Draw(histoName, step, 5., false);
  
  histoName = "lep_etaGap_max";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#eta gap_{lep}");
  stack -> Draw(histoName, step, 5., false);
  
  histoName = "lepMet_mt";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("mt_{lep+met}");
  stack -> Draw(histoName, step, 5., false);
  
  histoName = "lepMet_Dphi";
  stack -> SetXaxisRange(0., 3.);
  stack -> SetXaxisTitle("#Delta#phi_{lep-met}");
  stack -> Draw(histoName, step, 5., false);
  
  
  
    histoName = "ele_tkIso";
  stack -> SetXaxisRange(0., 0.1);
  stack -> SetXaxisTitle("#Sigma pt_{tracks}/pt_{ele}");
  stack -> Draw(histoName, step, 1., true);
  
  histoName = "ele_emIso";
  stack -> SetXaxisRange(0., 0.1);
  stack -> SetXaxisTitle("#Sigma et_{em}/et_{ele}");
  stack -> Draw(histoName, step, 1., true);  
  
  histoName = "ele_hadIso";
  stack -> SetXaxisRange(0., 0.1);
  stack -> SetXaxisTitle("#Sigma et_{had}/et_{ele}");
  stack -> Draw(histoName, step, 1., true);  
  
  
  
  histoName = "mu_tkIso";
  stack -> SetXaxisRange(0., 0.1);
  stack -> SetXaxisTitle("#Sigma pt_{tracks}/pt_{mu}");
  stack -> Draw(histoName, step, 1., true);
  
  histoName = "mu_emIso";
  stack -> SetXaxisRange(0., 0.1);
  stack -> SetXaxisTitle("#Sigma et_{em}/et_{mu}");
  stack -> Draw(histoName, step, 1., true);  
  
  histoName = "mu_hadIso";
  stack -> SetXaxisRange(0., 0.1);
  stack -> SetXaxisTitle("#Sigma et_{had}/et_{mu}");
  stack -> Draw(histoName, step, 1., true);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // Met
  //---------------------------------------------
  
  histoName = "met_et";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("et_{met}");
  stack -> Draw(histoName, step, 5, false);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // Lepton + met + W
  //---------------------------------------------  
  
  histoName = "lepMetW_mt";
  stack -> SetXaxisRange(0., 1000.);
  stack -> SetXaxisTitle("mt_{lep+W+met}");
  stack -> Draw(histoName, step, 10., false);
  
  histoName = "lepMetW_pt";
  stack -> SetXaxisRange(0., 500.);
  stack -> SetXaxisTitle("pt_{lep+W+met}");
  stack -> Draw(histoName, step, 5, false);
  
  histoName = "lepMetW_Dphi";
  stack -> SetXaxisRange(0., 3.);
  stack -> SetXaxisTitle("#Delta#phi_{lep+met-W}");
  stack -> Draw(histoName, step, 5., false);
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // pt ordering
  //---------------------------------------------  
  
  histoName = "tagWJ_et1";
  stack -> SetXaxisRange(0., 200.);
  stack -> Draw(histoName, step, 10, false);

  histoName = "tagWJ_et2";
  stack -> SetXaxisRange(0., 200.);
  stack -> Draw(histoName, step, 10, false);
  
  histoName = "tagWJ_et3";
  stack -> SetXaxisRange(0., 200.);
  stack -> Draw(histoName, step, 10, false);
  
  histoName = "tagWJ_et4";
  stack -> SetXaxisRange(0., 200.);
  stack -> Draw(histoName, step, 10, false);  
  
  
  
  
  
  
  histoName = "lepWJ_pt1";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("pt1_{lep-W}");
  stack -> Draw(histoName, step, 10., false);

  histoName = "lepWJ_pt2";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("pt2_{lep-W}");
  stack -> Draw(histoName, step, 10., false);
  
  histoName = "lepWJ_pt3";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("pt3_{lep-W}");
  stack -> Draw(histoName, step, 10., false);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // other jets
  //---------------------------------------------  
  
  histoName = "otherJ_et30_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> Draw(histoName, step, 1, true);
  
  histoName = "otherJ_et25_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> Draw(histoName, step, 1, true);
  
  histoName = "otherJ_et20_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> Draw(histoName, step, 1, true);
  
  histoName = "otherJ_et15_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> Draw(histoName, step, 1, true);
  
  
  
  histoName = "centralJ_et30_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> Draw(histoName, step, 1, true);
  
  histoName = "centralJ_et25_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> Draw(histoName, step, 1, true);
  
  histoName = "centralJ_et20_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> Draw(histoName, step, 1, true);
  
  histoName = "centralJ_et15_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> Draw(histoName, step, 1, true);
  
  
  
 // all jets
 //histoName = "allJ_n";
 //stack -> SetXaxisRange(4., 10.);
 //stack -> SetXaxisTitle("jet multiplicity");
 //stack -> Draw(histoName, step, 1, true);
}
