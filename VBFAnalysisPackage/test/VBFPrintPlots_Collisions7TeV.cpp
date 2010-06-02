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
    std::cerr << ">>>>> VBFPrintPlots_Collisions7TeV::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
  
  std::string inputDir = gConfigParser -> readStringOption("Input::inputDir");
  
  std::string outputDir = gConfigParser -> readStringOption("Output::outputDir");

  int step = gConfigParser -> readIntOption("Options::step");  
  //float lumi = gConfigParser -> readFloatOption("Options::lumi");
  
  
  
  
  
  // draw plots
  drawTStack* stack = new drawTStack(inputDir, "listOfSamplesAndCrossSections.txt", "Collisions7TeVAnalysis", outputDir);
  
  std::string histoName;
  
  
  
  
  
  
  //---------------------------------------------  
  // Lepton
  //---------------------------------------------  
  
  histoName = "ele_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("n_{ele}");
  stack -> SetDrawLegend(false);
  stack -> DrawData(histoName, step, 1., true);
  
  histoName = "ele_pt";
  stack -> SetXaxisRange(0., 100.);
  stack -> SetXaxisTitle("pt_{ele}");
  stack -> SetDrawLegend(false);
  stack -> DrawData(histoName, step, 2., true);
  
  histoName = "ele_phi";
  stack -> SetXaxisRange(0., 3.5);
  stack -> SetXaxisTitle("#phi_{ele}");
  stack -> DrawData(histoName, step, 4., false);
  
  histoName = "ele_eta";
  stack -> SetXaxisRange(-3., 3.);
  stack -> SetXaxisTitle("#eta_{ele}");
  stack -> DrawData(histoName, step, 4., false);
  
  histoName = "ele_absEta";
  stack -> SetXaxisRange(0., 3.);
  stack -> SetXaxisTitle("|#eta_{ele}|");
  stack -> DrawData(histoName, step, 4., false);
  
  histoName = "eleMet_mt";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("mt_{ele+met}");
  stack -> DrawData(histoName, step, 4., true);
  
  
  
  
  
  
  histoName = "mu_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("n_{mu}");
  stack -> SetDrawLegend(false);
  stack -> DrawData(histoName, step, 1., true);
  
  histoName = "mu_pt";
  stack -> SetXaxisRange(0., 100.);
  stack -> SetXaxisTitle("pt_{mu}");
  stack -> SetDrawLegend(false);
  stack -> DrawData(histoName, step, 2., true);
  
  histoName = "mu_phi";
  stack -> SetXaxisRange(0., 3.5);
  stack -> SetXaxisTitle("#phi_{mu}");
  stack -> DrawData(histoName, step, 4., false);
  
  histoName = "mu_eta";
  stack -> SetXaxisRange(-3., 3.);
  stack -> SetXaxisTitle("#eta_{mu}");
  stack -> DrawData(histoName, step, 4., false);
  
  histoName = "mu_absEta";
  stack -> SetXaxisRange(0., 3.);
  stack -> SetXaxisTitle("|#eta_{mu}|");
  stack -> DrawData(histoName, step, 4., false);
  
  histoName = "muMet_mt";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("mt_{mu+met}");
  stack -> DrawData(histoName, step, 4., true);
  
  
  
  
  
  
  histoName = "lep_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("n_{lep}");
  stack -> SetDrawLegend(false);
  stack -> DrawData(histoName, step, 1., true);
  
  histoName = "lep_pt";
  stack -> SetXaxisRange(0., 100.);
  stack -> SetXaxisTitle("pt_{lep}");
  stack -> SetDrawLegend(false);
  stack -> DrawData(histoName, step, 2., true);
  
  histoName = "lep_phi";
  stack -> SetXaxisRange(0., 3.5);
  stack -> SetXaxisTitle("#phi_{lep}");
  stack -> DrawData(histoName, step, 4., false);
  
  histoName = "lep_eta";
  stack -> SetXaxisRange(-3., 3.);
  stack -> SetXaxisTitle("#eta_{lep}");
  stack -> DrawData(histoName, step, 4., false);
  
  histoName = "lep_absEta";
  stack -> SetXaxisRange(0., 3.);
  stack -> SetXaxisTitle("|#eta_{lep}|");
  stack -> DrawData(histoName, step, 4., false);
  
  histoName = "lepMet_mt";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("mt_{lep+met}");
  stack -> DrawData(histoName, step, 4., true);
  
  
  
  histoName = "lepJet_DR";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("DR_{lep-jet}");
  stack -> DrawData(histoName, step, 10., true);
//  
//  histoName = "lepMet_Dphi";
//  stack -> SetXaxisRange(0., 3.);
//  stack -> SetXaxisTitle("#Delta#phi_{lep-met}");
//  stack -> DrawData(histoName, step, 5., false);
//  
  
  
//  histoName = "ele_tkIso";
//  stack -> SetXaxisRange(0., 0.1);
//  stack -> SetXaxisTitle("#Sigma pt_{tracks}/pt_{ele}");
//  stack -> DrawData(histoName, step, 1., true);
//  
//  histoName = "ele_emIso";
//  stack -> SetXaxisRange(0., 0.1);
//  stack -> SetXaxisTitle("#Sigma et_{em}/et_{ele}");
//  stack -> DrawData(histoName, step, 1., true);  
//  
//  histoName = "ele_hadIso";
//  stack -> SetXaxisRange(0., 0.1);
//  stack -> SetXaxisTitle("#Sigma et_{had}/et_{ele}");
//  stack -> DrawData(histoName, step, 1., true);  
//  
//  
//  
//  histoName = "mu_tkIso";
//  stack -> SetXaxisRange(0., 0.1);
//  stack -> SetXaxisTitle("#Sigma pt_{tracks}/pt_{mu}");
//  stack -> DrawData(histoName, step, 1., true);
//  
//  histoName = "mu_emIso";
//  stack -> SetXaxisRange(0., 0.1);
//  stack -> SetXaxisTitle("#Sigma et_{em}/et_{mu}");
//  stack -> DrawData(histoName, step, 1., true);  
//  
//  histoName = "mu_hadIso";
//  stack -> SetXaxisRange(0., 0.1);
//  stack -> SetXaxisTitle("#Sigma et_{had}/et_{mu}");
//  stack -> DrawData(histoName, step, 1., true);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // Met
  //---------------------------------------------
  
  histoName = "met_et";
  stack -> SetXaxisRange(0., 100.);
  stack -> SetXaxisTitle("et_{met}");
  stack -> DrawData(histoName, step, 2, true);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // Jets
  //---------------------------------------------
  
  histoName = "jet_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("n_{jet}");
  stack -> SetDrawLegend(false);
  stack -> DrawData(histoName, step, 1., true);
  
  histoName = "jet_et";
  stack -> SetXaxisRange(0., 100.);
  stack -> SetXaxisTitle("et_{jet}");
  stack -> SetDrawLegend(false);
  stack -> DrawData(histoName, step, 2., true);
  
  histoName = "jet_phi";
  stack -> SetXaxisRange(0., 3.5);
  stack -> SetXaxisTitle("#phi_{jet}");
  stack -> DrawData(histoName, step, 4., false);
  
  histoName = "jet_eta";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta_{jet}");
  stack -> DrawData(histoName, step, 4., false);
  
  histoName = "jet_absEta";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("|#eta_{jet}|");
  stack -> DrawData(histoName, step, 4., false);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // tag Jets
  //---------------------------------------------
  
  histoName = "tagJJ_min_et";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("et_{tagJJ} min");
  stack -> SetDrawLegend(false);
  stack -> DrawData(histoName, step, 4., false);
  
  histoName = "tagJJ_max_et";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("et_{tagJJ} max");
  stack -> SetDrawLegend(false);
  stack -> DrawData(histoName, step, 4., false);
  
  histoName = "tagJJ_phi";
  stack -> SetXaxisRange(0., 3.5);
  stack -> SetXaxisTitle("#phi_{tagJJ}");
  stack -> DrawData(histoName, step, 4., false);
  
  histoName = "tagJJ_max_eta";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta_{tagJJ} max");
  stack -> DrawData(histoName, step, 4., false);
  
  histoName = "tagJJ_min_eta";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta_{tagJJ} min");
  stack -> DrawData(histoName, step, 4., false);
  
  histoName = "tagJJ_absEta";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("|#eta_{tagJJ}|");
  stack -> DrawData(histoName, step, 4., false);
  
  histoName = "tagJJ_absEta";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("|#eta_{tagJJ}|");
  stack -> DrawData(histoName, step, 4., false);
  
  
  
  histoName = "tagJJ_m";
  stack -> SetXaxisRange(0., 1000.);
  stack -> SetXaxisTitle("m_{tagJJ}");
  stack -> DrawData(histoName, step, 20., false);
  
  histoName = "tagJJ_Dphi";
  stack -> SetXaxisRange(0., 3.);
  stack -> SetXaxisTitle("#Delta#phi_{tagJJ}");
  stack -> DrawData(histoName, step, 4., false);
  
  histoName = "tagJJ_Deta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#Delta#eta_{tagJJ}");
  stack -> DrawData(histoName, step, 4., false);
  
  histoName = "tagJJ_DR";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#DeltaR_{tagJJ}");
  stack -> DrawData(histoName, step, 4., false);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // W Jets
  //---------------------------------------------
  
  histoName = "WJJ_et";
  stack -> SetXaxisRange(0., 100.);
  stack -> SetXaxisTitle("et_{WJJ}");
  stack -> SetDrawLegend(false);
  stack -> DrawData(histoName, step, 4., false);
  
  histoName = "WJJ_phi";
  stack -> SetXaxisRange(0., 3.5);
  stack -> SetXaxisTitle("#phi_{WJJ}");
  stack -> DrawData(histoName, step, 4., false);
  
  histoName = "WJJ_eta";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta_{WJJ}");
  stack -> DrawData(histoName, step, 4., false);
  
  histoName = "WJJ_absEta";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("|#eta_{WJJ}|");
  stack -> DrawData(histoName, step, 4., false);
  
  histoName = "WJJ_absEta";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("|#eta_{WJJ}|");
  stack -> DrawData(histoName, step, 4., false);
  
  
  
  histoName = "WJJ_m";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("m_{Wjj}");
  stack -> DrawData(histoName, step, 4., false);
  
  histoName = "WJJ_Dphi";
  stack -> SetXaxisRange(0., 3.);
  stack -> SetXaxisTitle("#Delta#phi_{Wjj}");
  stack -> DrawData(histoName, step, 4., false);
  
  histoName = "WJJ_Deta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#Delta#eta_{Wjj}");
  stack -> DrawData(histoName, step, 4., false);
  
  histoName = "WJJ_DR";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#DeltaR_{Wjj}");
  stack -> DrawData(histoName, step, 4., false);
}
