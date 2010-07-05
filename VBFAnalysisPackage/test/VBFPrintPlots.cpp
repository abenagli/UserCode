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
  float lumi = gConfigParser -> readFloatOption("Options::lumi");
  
  
  
  
  
  // draw plots
  drawTStack* stack = new drawTStack(inputDir, "listOfSamplesAndCrossSections.txt", "VBFHiggsToWWTolnujjAnalysis", outputDir);
  
  std::string histoName;
  std::vector<std::string> histoNames;
  histoNames.push_back("");
  
  
  
  
  
  
  //---------------------------------------------  
  // tag jets
  //---------------------------------------------  
  
  histoNames.at(0) = "tagJJ_m";
  stack -> SetXaxisRange(0., 3000.);
  stack -> SetXaxisTitle("m_{jj tag}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 20., false);
  
  histoNames.at(0) = "tagJJ_Deta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#Delta#eta_{jj tag}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., false);

  histoNames.at(0) = "tagJJ_etaProd";
  stack -> SetXaxisRange(-25., 25.);
  stack -> SetXaxisTitle("#etaProd_{jj tag}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 20., false);
    
  histoNames.at(0) = "tagJJ_Dphi";
  stack -> SetXaxisRange(0., 3.);
  stack -> SetXaxisTitle("#Delta#phi_{jj tag}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., false);
  
  histoNames.at(0) = "tagJJ_max_absEta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("|#eta_{tag j}| max");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., false);
  
  histoNames.at(0) = "tagJJ_min_absEta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("|#eta_{tag j}| min");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., false);
  
  histoNames.at(0) = "tagJJ_max_et";
  stack -> SetXaxisRange(0., 400.);
  stack -> SetXaxisTitle("et_{j tag} max");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., false);
  
  histoNames.at(0) = "tagJJ_min_et";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("et_{j tag} min");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., false);
  
  histoNames.at(0) = "tagJJ_max_e";
  stack -> SetXaxisRange(0., 2000.);
  stack -> SetXaxisTitle("e_{j tag} max");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 20., false);
  
  histoNames.at(0) = "tagJJ_min_e";
  stack -> SetXaxisRange(0., 1000.);
  stack -> SetXaxisTitle("e_{j tag} min");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 10., false);
  
  histoNames.at(0) = "tagJJ_bTag";
  stack -> SetXaxisTitle("b-tag");
  stack -> SetXaxisRange(-10., 30.);
  stack -> SetXaxisTitle("btag_{j tag}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5, true);
  
//  histoNames.at(0) = "tagJJ_bTagSum";
//  stack -> SetXaxisTitle("b-tag");
//  stack -> SetXaxisRange(-10., 30.);
//  stack -> SetXaxisTitle("sum btag_{j tag}");
//  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5, true);
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // Wjets
  //---------------------------------------------  
  
  histoNames.at(0) = "WJJ_m";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("m_{Wjj}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., false);
  
  histoNames.at(0) = "WJJ_et";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("et_{Wjj}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5, false);
  
  histoNames.at(0) = "WJJ_absEta";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("|#eta_{Wjj}|");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., false);
  
  histoNames.at(0) = "WJJ_zepp";
  stack -> SetXaxisRange(0., 2.);
  stack -> SetXaxisTitle("z_{Wjj}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., false);
  
  histoNames.at(0) = "WJJ_Dphi";
  stack -> SetXaxisRange(0., 3.);
  stack -> SetXaxisTitle("#Delta#phi_{Wjj}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., false);
  
  histoNames.at(0) = "WJJ_Deta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#Delta#eta_{Wjj}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., false);
  
  histoNames.at(0) = "WJJ_DR";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#DeltaR_{Wjj}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., false);
  
  
  
  histoNames.at(0) = "WJJ_max_et";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("et_{Wj} max");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5, false);  
  
  histoNames.at(0) = "WJJ_min_et";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("et_{Wj} min");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5, false);
  
  histoNames.at(0) = "WJJ_max_e";
  stack -> SetXaxisRange(0., 2000.);
  stack -> SetXaxisTitle("e_{Wj} max");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 20, false);  
  
  histoNames.at(0) = "WJJ_min_e";
  stack -> SetXaxisRange(0., 1000.);
  stack -> SetXaxisTitle("e_{Wj} min");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 10, false);
  
  histoNames.at(0) = "WJJ_max_absEta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("|#eta_{Wj}| max");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., false);
  
  histoNames.at(0) = "WJJ_min_absEta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("|#eta_{Wj}| min");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., false);
  
  histoNames.at(0) = "WJJ_max_zepp";
  stack -> SetXaxisRange(0., 2.);
  stack -> SetXaxisTitle("z_{Wj} max");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5, false);  
  
  histoNames.at(0) = "WJJ_min_zepp";
  stack -> SetXaxisRange(0., 2.);
  stack -> SetXaxisTitle("z_{Wj} min");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5, false);
  
  histoNames.at(0) = "WJJ_bTag";
  stack -> SetXaxisRange(-10., 30.);
  stack -> SetXaxisTitle("b-tag_{W j}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5, true);
  
//  histoNames.at(0) = "WJJ_bTagSum";
//  stack -> SetXaxisRange(-10., 30.);
//  stack -> SetXaxisTitle("sum b-tag_{W j}");
//  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5, true);
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // Lepton
  //---------------------------------------------  
  
  histoNames.at(0) = "lep_pt";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("pt_{lep}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., false);
  
  histoNames.at(0) = "lep_absEta";
  stack -> SetXaxisRange(0., 3.);
  stack -> SetXaxisTitle("|#eta_{lep}|");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 2., false);
  
  histoNames.at(0) = "lep_zepp";
  stack -> SetXaxisRange(0., 1.);
  stack -> SetXaxisTitle("z_{lep}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., false);
  
  histoNames.at(0) = "lep_etaGap_min";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("#eta gap_{lep}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., false);
  
  histoNames.at(0) = "lep_etaGap_max";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#eta gap_{lep}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., false);
  
  histoNames.at(0) = "lepMet_mt";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("mt_{lep+met}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., false);
  
  histoNames.at(0) = "lepMet_Dphi";
  stack -> SetXaxisRange(0., 3.);
  stack -> SetXaxisTitle("#Delta#phi_{lep-met}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., false);
  
  
  
    histoNames.at(0) = "ele_tkIso";
  stack -> SetXaxisRange(0., 0.1);
  stack -> SetXaxisTitle("#Sigma pt_{tracks}/pt_{ele}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 1., true);
  
  histoNames.at(0) = "ele_emIso";
  stack -> SetXaxisRange(0., 0.1);
  stack -> SetXaxisTitle("#Sigma et_{em}/et_{ele}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 1., true);  
  
  histoNames.at(0) = "ele_hadIso";
  stack -> SetXaxisRange(0., 0.1);
  stack -> SetXaxisTitle("#Sigma et_{had}/et_{ele}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 1., true);  
  
  
  
  histoNames.at(0) = "mu_tkIso";
  stack -> SetXaxisRange(0., 0.1);
  stack -> SetXaxisTitle("#Sigma pt_{tracks}/pt_{mu}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 1., true);
  
  histoNames.at(0) = "mu_emIso";
  stack -> SetXaxisRange(0., 0.1);
  stack -> SetXaxisTitle("#Sigma et_{em}/et_{mu}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 1., true);  
  
  histoNames.at(0) = "mu_hadIso";
  stack -> SetXaxisRange(0., 0.1);
  stack -> SetXaxisTitle("#Sigma et_{had}/et_{mu}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 1., true);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // Met
  //---------------------------------------------
  
  histoNames.at(0) = "met_et";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("et_{met}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5, false);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // Lepton + met + W
  //---------------------------------------------  
  
  histoNames.at(0) = "lepMetW_mt";
  stack -> SetXaxisRange(0., 1000.);
  stack -> SetXaxisTitle("mt_{lep+W+met}");
  stack -> Draw(histoNames, "eventsScaled", lumi, step, 10., false);
  
  histoNames.at(0) = "lepMetW_pt";
  stack -> SetXaxisRange(0., 500.);
  stack -> SetXaxisTitle("pt_{lep+W+met}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5, false);
  
  histoNames.at(0) = "lepMetW_Dphi";
  stack -> SetXaxisRange(0., 3.);
  stack -> SetXaxisTitle("#Delta#phi_{lep+met-W}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., false);
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // pt ordering
  //---------------------------------------------  
  
  histoNames.at(0) = "tagWJ_et1";
  stack -> SetXaxisRange(0., 200.);
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 10, false);

  histoNames.at(0) = "tagWJ_et2";
  stack -> SetXaxisRange(0., 200.);
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 10, false);
  
  histoNames.at(0) = "tagWJ_et3";
  stack -> SetXaxisRange(0., 200.);
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 10, false);
  
  histoNames.at(0) = "tagWJ_et4";
  stack -> SetXaxisRange(0., 200.);
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 10, false);
  
//  histoNames.at(0) = "tagWJ_bTagSum";
//  stack -> SetXaxisRange(-20., 50.);
//  stack -> SetXaxisTitle("sum b-tag_{tag/W j}");
//  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 2, true);
//  
  histoNames.at(0) = "tagWJ_bTag1";
  stack -> SetXaxisRange(-10., 20.);
  stack -> SetXaxisTitle("b-tag1_{tag/W j}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 2, true);
  
  histoNames.at(0) = "tagWJ_bTag2";
  stack -> SetXaxisRange(-10., 20.);
  stack -> SetXaxisTitle("b-tag2_{tag/W j}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 2, true);
  
  histoNames.at(0) = "tagWJ_bTag3";
  stack -> SetXaxisRange(-10., 20.);
  stack -> SetXaxisTitle("b-tag3_{tag/W j}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 2, true);

  histoNames.at(0) = "tagWJ_bTag4";
  stack -> SetXaxisRange(-10., 20.);
  stack -> SetXaxisTitle("b-tag4_{tag/W j}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 2, true);  
  
  
  
  
  
  
  histoNames.at(0) = "lepWJ_pt1";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("pt1_{lep-W}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 10., false);

  histoNames.at(0) = "lepWJ_pt2";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("pt2_{lep-W}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 10., false);
  
  histoNames.at(0) = "lepWJ_pt3";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("pt3_{lep-W}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 10., false);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // other jets
  //---------------------------------------------  
  
  histoNames.at(0) = "otherJ_et30_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 1, true);
  
  histoNames.at(0) = "otherJ_et25_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 1, true);
  
  histoNames.at(0) = "otherJ_et20_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 1, true);
  
  histoNames.at(0) = "otherJ_et15_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 1, true);
  
  
  
  histoNames.at(0) = "centralJ_et30_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 1, true);
  
  histoNames.at(0) = "centralJ_et25_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 1, true);
  
  histoNames.at(0) = "centralJ_et20_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 1, true);
  
  histoNames.at(0) = "centralJ_et15_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 1, true);
  
  
  
 // all jets
 //histoNames.at(0) = "allJ_n";
 //stack -> SetXaxisRange(4., 10.);
 //stack -> SetXaxisTitle("jet multiplicity");
 //stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 1, true);
}
