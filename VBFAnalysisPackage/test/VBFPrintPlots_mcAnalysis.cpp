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
    std::cerr << ">>>>> VBFPrintPlots_noSelection_mcAnalysis::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
  
  std::string inputDir = gConfigParser -> readStringOption("Input::inputDir");
  
  std::string outputDir = gConfigParser -> readStringOption("Output::outputDir");

  int step = gConfigParser -> readIntOption("Options::step");  
  float lumi = gConfigParser -> readFloatOption("Options::lumi");
  
  
  
  
  
  // draw plots
  drawTStack* stack = new drawTStack(inputDir, "listOfSamplesAndCrossSections.txt", "VBFHiggsToWWTolnujjMcAnalysis", outputDir);
  stack -> SetDrawLegend(true);
  
  std::string histoName;
  
  
  
  
  
  
  //---------------------------------------------  
  // mc Higgs
  //---------------------------------------------  

  histoName = "mcH_m";
  stack -> SetXaxisRange(0., 500.);
  stack -> SetXaxisTitle("m_{H}");
  stack -> Draw(histoName, "events", lumi, step, 1, true);
  
  histoName = "mcH_pt";
  stack -> SetXaxisRange(0., 500.);
  stack -> SetXaxisTitle("pt_{H}");
  stack -> Draw(histoName, "events", lumi, step, 5, false);
  
  histoName = "mcH_pl";
  stack -> SetXaxisRange(0., 500.);
  stack -> SetXaxisTitle("pl_{H}");
  stack -> Draw(histoName, "events", lumi, step, 5, false);
  
  histoName = "mcH_zepp";
  stack -> SetXaxisRange(0., 2.);
  stack -> SetXaxisTitle("z_{H}");
  stack -> Draw(histoName, "events", lumi, step, 10, false);
  
  histoName = "mcH_absEta";
  stack -> SetXaxisRange(-6., 6.);
  stack -> SetXaxisTitle("|#eta_{H}|");
  stack -> Draw(histoName, "events", lumi, step, 5, false);  
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // tag jets
  //---------------------------------------------  
  
  histoName = "mcTagQQ_m";
  stack -> SetXaxisRange(0., 3000.);
  stack -> SetXaxisTitle("m_{qq tag}");
  stack -> Draw(histoName, "events", lumi, step, 10., false);

  histoName = "tagJJ_m";
  stack -> SetXaxisRange(0., 3000.);
  stack -> SetXaxisTitle("m_{jj tag}");
  stack -> Draw(histoName, "events", lumi, step, 10., false);
  
  
  
  histoName = "mcTagQQ_Deta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#Delta#eta_{qq tag}");
  stack -> Draw(histoName, "events", lumi, step, 5., false);

  histoName = "tagJJ_Deta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#Delta#eta_{jj tag}");
  stack -> Draw(histoName, "events", lumi, step, 5., false);
  
  
  
  histoName = "mcTagQQ_Dphi";
  stack -> SetXaxisRange(0., 3.);
  stack -> SetXaxisTitle("#Delta#phi_{qq tag}");
  stack -> Draw(histoName, "events", lumi, step, 5., false);

  histoName = "tagJJ_Dphi";
  stack -> SetXaxisRange(0., 3.);
  stack -> SetXaxisTitle("#Delta#phi_{jj tag}");
  stack -> Draw(histoName, "events", lumi, step, 5., false);
  
  
  
  histoName = "mcTagQQ_max_absEta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("|#eta_{tag q}| max");
  stack -> Draw(histoName, "events", lumi, step, 5., false);

  histoName = "tagJJ_max_absEta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("|#eta_{tag j}| max");
  stack -> Draw(histoName, "events", lumi, step, 5., false);
  
  
    
  histoName = "mcTagQQ_min_absEta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("|#eta_{tag q}| min");
  stack -> Draw(histoName, "events", lumi, step, 5., false);

  histoName = "tagJJ_min_absEta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("|#eta_{tag j}| min");
  stack -> Draw(histoName, "events", lumi, step, 5., false);
  
  
  
  histoName = "mcTagQQ_max_pt";
  stack -> SetXaxisRange(0., 80.);
  stack -> SetXaxisTitle("pt_{q tag} max");
  stack -> Draw(histoName, "events", lumi, step, 2., false);

  histoName = "tagJJ_max_et";
  stack -> SetXaxisRange(0., 80.);
  stack -> SetXaxisTitle("et_{j tag} max");
  stack -> Draw(histoName, "events", lumi, step, 2., false);
  
  
  
  histoName = "mcTagQQ_min_pt";
  stack -> SetXaxisRange(0., 80.);
  stack -> SetXaxisTitle("pt_{q tag} min");
  stack -> Draw(histoName, "events", lumi, step, 2., false);

  histoName = "tagJJ_min_et";
  stack -> SetXaxisRange(0., 80.);
  stack -> SetXaxisTitle("et_{j tag} min");
  stack -> Draw(histoName, "events", lumi, step, 2., false);  
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // WQQ / WJJ
  //---------------------------------------------  
  
  histoName = "mcWQQ_m";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("m_{Wqq}");
  stack -> Draw(histoName, "events", lumi, step, 1., false);
  
  histoName = "WJJ_m";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("m_{Wjj}");
  stack -> Draw(histoName, "events", lumi, step, 1., false);
  
  
  
  histoName = "mcWQQ_pt";
  stack -> SetXaxisRange(0., 80.);
  stack -> SetXaxisTitle("pt_{Wqq}");
  stack -> Draw(histoName, "events", lumi, step, 2., false);

  histoName = "WJJ_pt";
  stack -> SetXaxisRange(0., 80.);
  stack -> SetXaxisTitle("pt_{Wjj}");
  stack -> Draw(histoName, "events", lumi, step, 2., false);
  
  
    
  histoName = "mcWQQ_absEta";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("|#eta_{Wqq}|");
  stack -> Draw(histoName, "events", lumi, step, 5., false);

  histoName = "WJJ_absEta";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("|#eta_{Wjj}|");
  stack -> Draw(histoName, "events", lumi, step, 5., false);
  
  
  
  histoName = "mcWQQ_zepp";
  stack -> SetXaxisRange(0., 2.);
  stack -> SetXaxisTitle("z_{Wqq}");
  stack -> Draw(histoName, "events", lumi, step, 5., false);  
  
  histoName = "WJJ_zepp";
  stack -> SetXaxisRange(0., 2.);
  stack -> SetXaxisTitle("z_{Wjj}");
  stack -> Draw(histoName, "events", lumi, step, 5., false);
  
  
  
  histoName = "mcWQQ_Dphi";
  stack -> SetXaxisRange(0., 3.);
  stack -> SetXaxisTitle("#Delta#phi_{Wqq}");
  stack -> Draw(histoName, "events", lumi, step, 5., false);
  
  histoName = "WJJ_Dphi";
  stack -> SetXaxisRange(0., 3.);
  stack -> SetXaxisTitle("#Delta#phi_{Wjj}");
  stack -> Draw(histoName, "events", lumi, step, 5., false);
  
  
  
  histoName = "mcWQQ_Deta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#Delta#eta_{Wqq}");
  stack -> Draw(histoName, "events", lumi, step, 5., false);
  
  histoName = "WJJ_Deta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#Delta#eta_{Wjj}");
  stack -> Draw(histoName, "events", lumi, step, 5., false);
  
  
  
  histoName = "mcWQQ_DR";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#DeltaR_{Wqq}");
  stack -> Draw(histoName, "events", lumi, step, 5., false);
  
  histoName = "WJJ_DR";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#DeltaR_{Wjj}");
  stack -> Draw(histoName, "events", lumi, step, 5., false);
  
  
  
  histoName = "mcLepMetW_Dphi";
  stack -> SetXaxisRange(0., 3.);
  stack -> SetXaxisTitle("#Delta#phi_{lep+met-W}");
  stack -> Draw(histoName, "events", lumi, step, 5., false);  
  
  histoName = "lepMetW_Dphi";
  stack -> SetXaxisRange(0., 3.);
  stack -> SetXaxisTitle("#Delta#phi_{lep+met-W}");
  stack -> Draw(histoName, "events", lumi, step, 5., false);
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // W jets
  //---------------------------------------------  
  
  histoName = "mcWQQ_max_pt";
  stack -> SetXaxisRange(0., 80.);
  stack -> SetXaxisTitle("pt_{Wq} max");
  stack -> Draw(histoName, "events", lumi, step, 2., false);

  histoName = "WJJ_max_et";
  stack -> SetXaxisRange(0., 80.);
  stack -> SetXaxisTitle("et_{Wj} max");
  stack -> Draw(histoName, "events", lumi, step, 2., false);
  
  
  
  histoName = "mcWQQ_min_pt";
  stack -> SetXaxisRange(0., 80.);
  stack -> SetXaxisTitle("pt_{Wq} min");
  stack -> Draw(histoName, "events", lumi, step, 2., false);  

  histoName = "WJJ_min_et";
  stack -> SetXaxisRange(0., 80.);
  stack -> SetXaxisTitle("et_{Wj} min");
  stack -> Draw(histoName, "events", lumi, step, 2., false);
  
  
    
  histoName = "mcWQQ_max_absEta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("|#eta_{Wq}| max");
  stack -> Draw(histoName, "events", lumi, step, 5., false);
  
  histoName = "WJJ_max_absEta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("|#eta_{Wj}| max");
  stack -> Draw(histoName, "events", lumi, step, 5., false);
  
  
  
  histoName = "mcWQQ_min_absEta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("|#eta_{Wq}| min");
  stack -> Draw(histoName, "events", lumi, step, 5., false);  

  histoName = "WJJ_min_absEta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("|#eta_{Wj}| min");
  stack -> Draw(histoName, "events", lumi, step, 5., false);
  
  
    
  histoName = "mcWQQ_max_zepp";
  stack -> SetXaxisRange(0., 2.);
  stack -> SetXaxisTitle("z_{Wq} max");
  stack -> Draw(histoName, "events", lumi, step, 5., false);

  histoName = "WJJ_max_zepp";
  stack -> SetXaxisRange(0., 2.);
  stack -> SetXaxisTitle("z_{Wj} max");
  stack -> Draw(histoName, "events", lumi, step, 5., false);
  
  
  
  histoName = "mcWQQ_min_zepp";
  stack -> SetXaxisRange(0., 2.);
  stack -> SetXaxisTitle("z_{Wq} min");
  stack -> Draw(histoName, "events", lumi, step, 5., false);  

  histoName = "WJJ_min_zepp";
  stack -> SetXaxisRange(0., 2.);
  stack -> SetXaxisTitle("z_{Wj} min");
  stack -> Draw(histoName, "events", lumi, step, 5., false);  
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // Lepton
  //---------------------------------------------  
  
  histoName = "mcLep_pt";
  stack -> SetXaxisRange(0., 80.);
  stack -> SetXaxisTitle("pt_{lep}");
  stack -> Draw(histoName, "events", lumi, step, 2., false);
  
  histoName = "lep_pt";
  stack -> SetXaxisRange(0., 80.);
  stack -> SetXaxisTitle("pt_{lep}");
  stack -> Draw(histoName, "events", lumi, step, 2., false);
  
  
  
  histoName = "mcLep_absEta";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("|#eta_{lep}|");
  stack -> Draw(histoName, "events", lumi, step, 5., false);
  
  histoName = "lep_absEta";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("|#eta_{lep}|");
  stack -> Draw(histoName, "events", lumi, step, 5., false);
  
  
  
  histoName = "mcLep_zepp";
  stack -> SetXaxisRange(0., 2.);
  stack -> SetXaxisTitle("z_{lep}");
  stack -> Draw(histoName, "events", lumi, step, 5., false);
  
  histoName = "lep_zepp";
  stack -> SetXaxisRange(0., 2.);
  stack -> SetXaxisTitle("z_{lep}");
  stack -> Draw(histoName, "events", lumi, step, 5., false);
  
  
  
  histoName = "mcLepMet_mt";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("mt_{lep+met}");
  stack -> Draw(histoName, "events", lumi, step, 1., false);

  histoName = "lepMet_mt";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("mt_{lep+met}");
  stack -> Draw(histoName, "events", lumi, step, 2., false);
  
  
    
  histoName = "mcLepMet_Dphi";
  stack -> SetXaxisRange(0., 3.);
  stack -> SetXaxisTitle("#Delta#phi_{lep-met}");
  stack -> Draw(histoName, "events", lumi, step, 5., false);

  histoName = "lepMet_Dphi";
  stack -> SetXaxisRange(0., 3.);
  stack -> SetXaxisTitle("#Delta#phi_{lep-met}");
  stack -> Draw(histoName, "events", lumi, step, 5., false);
  
  
  
  
  
  
  histoName = "ele_tkIso";
  stack -> SetXaxisRange(0., 0.1);
  stack -> SetXaxisTitle("#Sigma pt_{tracks}/pt_{ele}");
  stack -> Draw(histoName, "events", lumi, step, 1., true);
  
  histoName = "ele_emIso";
  stack -> SetXaxisRange(0., 0.1);
  stack -> SetXaxisTitle("#Sigma et_{em}/et_{ele}");
  stack -> Draw(histoName, "events", lumi, step, 1., true);  
  
  histoName = "ele_hadIso";
  stack -> SetXaxisRange(0., 0.1);
  stack -> SetXaxisTitle("#Sigma et_{had}/et_{ele}");
  stack -> Draw(histoName, "events", lumi, step, 1., true);  
  
  
  
  histoName = "mu_tkIso";
  stack -> SetXaxisRange(0., 0.1);
  stack -> SetXaxisTitle("#Sigma pt_{tracks}/pt_{mu}");
  stack -> Draw(histoName, "events", lumi, step, 1., true);
  
  histoName = "mu_emIso";
  stack -> SetXaxisRange(0., 0.1);
  stack -> SetXaxisTitle("#Sigma et_{em}/et_{mu}");
  stack -> Draw(histoName, "events", lumi, step, 1., true);  
  
  histoName = "mu_hadIso";
  stack -> SetXaxisRange(0., 0.1);
  stack -> SetXaxisTitle("#Sigma et_{had}/et_{mu}");
  stack -> Draw(histoName, "events", lumi, step, 1., true);
  
  
  
  
  
  
  histoName = "mcLepMetW_mt";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("mt_{lep+W+met}");
  stack -> Draw(histoName, "events", lumi, step, 5., false);

  histoName = "lepMetW_mt";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("mt_{lep+W+met}");
  stack -> Draw(histoName, "events", lumi, step, 5., false);  
  
  
  
  
  
  
  
  
  
  
  
  
  histoName = "mcTagWQ_pt1";
  stack -> SetXaxisRange(0., 80.);
  stack -> SetXaxisTitle("pt1_{tag-W}");
  stack -> Draw(histoName, "events", lumi, step, 2., false);

  histoName = "tagWJ_et1";
  stack -> SetXaxisRange(0., 80.);
  stack -> SetXaxisTitle("et1_{tag-W}");
  stack -> Draw(histoName, "events", lumi, step, 2., false);
  
  
    
  histoName = "mcTagWQ_pt2";
  stack -> SetXaxisRange(0., 80.);
  stack -> SetXaxisTitle("pt2_{tag-W}");
  stack -> Draw(histoName, "events", lumi, step, 2., false);

  histoName = "tagWJ_et2";
  stack -> SetXaxisRange(0., 80.);
  stack -> SetXaxisTitle("et2_{tag-W}");
  stack -> Draw(histoName, "events", lumi, step, 2., false);
  
  
    
  histoName = "mcTagWQ_pt3";
  stack -> SetXaxisRange(0., 80.);
  stack -> SetXaxisTitle("pt3_{tag-W}");
  stack -> Draw(histoName, "events", lumi, step, 2., false);

  histoName = "tagWJ_et3";
  stack -> SetXaxisRange(0., 80.);
  stack -> SetXaxisTitle("et3_{tag-W}");
  stack -> Draw(histoName, "events", lumi, step, 2., false);
  
  
    
  histoName = "mcTagWQ_pt4";
  stack -> SetXaxisRange(0., 80.);
  stack -> SetXaxisTitle("pt4_{tag-W}");
  stack -> Draw(histoName, "events", lumi, step, 2., false);

  histoName = "tagWJ_et4";
  stack -> SetXaxisRange(0., 80.);
  stack -> SetXaxisTitle("et4_{tag-W}");
  stack -> Draw(histoName, "events", lumi, step, 2., false);
  
  
  
  
  
  
  
  
  
  
  
  
  histoName = "mcLepWQ_pt1";
  stack -> SetXaxisRange(0., 80.);
  stack -> SetXaxisTitle("pt1_{lep-W}");
  stack -> Draw(histoName, "events", lumi, step, 2., false);

  histoName = "lepWJ_pt1";
  stack -> SetXaxisRange(0., 80.);
  stack -> SetXaxisTitle("pt1_{lep-W}");
  stack -> Draw(histoName, "events", lumi, step, 2., false);
  
  
  
  histoName = "mcLepWQ_pt2";
  stack -> SetXaxisRange(0., 80.);
  stack -> SetXaxisTitle("pt2_{lep-W}");
  stack -> Draw(histoName, "events", lumi, step, 2., false);

  histoName = "lepWJ_pt2";
  stack -> SetXaxisRange(0., 80.);
  stack -> SetXaxisTitle("pt2_{lep-W}");
  stack -> Draw(histoName, "events", lumi, step, 2., false);
  
  
  
  histoName = "mcLepWQ_pt3";
  stack -> SetXaxisRange(0., 80.);
  stack -> SetXaxisTitle("pt3_{lep-W}");
  stack -> Draw(histoName, "events", lumi, step, 2., false);

  histoName = "lepWJ_pt3";
  stack -> SetXaxisRange(0., 80.);
  stack -> SetXaxisTitle("pt3_{lep-W}");
  stack -> Draw(histoName, "events", lumi, step, 2., false);
}
