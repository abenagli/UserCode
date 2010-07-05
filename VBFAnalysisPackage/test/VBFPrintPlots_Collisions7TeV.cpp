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
  float lumi = gConfigParser -> readFloatOption("Options::lumi");
  
  
  
  
  
  // draw plots
  drawTStack* stack = new drawTStack(inputDir, "listOfSamplesAndCrossSections.txt", "Collisions7TeVAnalysis", outputDir);
  
  std::string histoName;
  std::vector<std::string> histoNames;
  histoNames.push_back("");
  std::vector<std::string> histoNames2;
  histoNames2.push_back("");
  histoNames2.push_back("");
  
  
  
  
  
  //---------------------------------------------  
  // Lepton
  //---------------------------------------------  
  
  //histoNames.at(0) = "ele_n";
  //stack -> SetXaxisRange(0., 10.);
  //stack -> SetXaxisTitle("n_{ele}");
  //stack -> DrawData(histoName, step, 1., true);
  
  //histoNames.at(0) = "ele_pt";
  //stack -> SetXaxisRange(0., 100.);
  //stack -> SetXaxisTitle("pt_{ele}");
  //stack -> DrawData(histoName, step, 2., true);
  
  //histoNames.at(0) = "ele_phi";
  //stack -> SetXaxisRange(0., 3.5);
  //stack -> SetXaxisTitle("#phi_{ele}");
  //stack -> Draw(histoNames, "sameAreaStack", lumi,step, 4., false);
  
  //histoNames.at(0) = "ele_eta";
  //stack -> SetXaxisRange(-3., 3.);
  //stack -> SetXaxisTitle("#eta_{ele}");
  //stack -> Draw(histoNames, "sameAreaStack", lumi,step, 4., false);
  
  //histoNames.at(0) = "ele_absEta";
  //stack -> SetXaxisRange(0., 3.);
  //stack -> SetXaxisTitle("|#eta_{ele}|");
  //stack -> Draw(histoNames, "sameAreaStack", lumi,step, 4., false);
  
  //histoNames.at(0) = "eleMet_mt";
  //stack -> SetXaxisRange(0., 200.);
  //stack -> SetXaxisTitle("mt_{ele+met}");
  //stack -> DrawData(histoName, step, 4., true);
  
  
  
  
  
  
  //histoNames.at(0) = "mu_n";
  //stack -> SetXaxisRange(0., 10.);
  //stack -> SetXaxisTitle("n_{mu}");
  //stack -> DrawData(histoName, step, 1., true);
  
  //histoNames.at(0) = "mu_pt";
  //stack -> SetXaxisRange(0., 100.);
  //stack -> SetXaxisTitle("pt_{mu}");
  //stack -> DrawData(histoName, step, 2., true);
  
  //histoNames.at(0) = "mu_phi";
  //stack -> SetXaxisRange(0., 3.5);
  //stack -> SetXaxisTitle("#phi_{mu}");
  //stack -> Draw(histoNames, "sameAreaStack", lumi,step, 4., false);
  
  //histoNames.at(0) = "mu_eta";
  //stack -> SetXaxisRange(-3., 3.);
  //stack -> SetXaxisTitle("#eta_{mu}");
  //stack -> Draw(histoNames, "sameAreaStack", lumi,step, 4., false);
  
  //histoNames.at(0) = "mu_absEta";
  //stack -> SetXaxisRange(0., 3.);
  //stack -> SetXaxisTitle("|#eta_{mu}|");
  //stack -> Draw(histoNames, "sameAreaStack", lumi,step, 4., false);
  
  //histoNames.at(0) = "muMet_mt";
  //stack -> SetXaxisRange(0., 200.);
  //stack -> SetXaxisTitle("mt_{mu+met}");
  //stack -> DrawData(histoName, step, 4., true);
  
  
  
  
  
  
  //histoNames.at(0) = "lep_n";
  //stack -> SetXaxisRange(0., 10.);
  //stack -> SetXaxisTitle("n_{lep}");
  //stack -> DrawData(histoName, step, 1., true);
  
  histoNames.at(0) = "lep_pt";
  stack -> SetXaxisRange(0., 100.);
  stack -> SetXaxisTitle("pt_{lep}");
  stack -> Draw(histoNames, "sameAreaStack", lumi, step, 5., true);
  
  histoNames.at(0) = "lep_phi";
  stack -> SetXaxisRange(-3.5, 3.5);
  stack -> SetXaxisTitle("#phi_{lep}");
  stack -> Draw(histoNames, "sameAreaStack", lumi,step, 5., false);
  
  histoNames.at(0) = "lep_eta";
  stack -> SetXaxisRange(-3., 3.);
  stack -> SetXaxisTitle("#eta_{lep}");
  stack -> Draw(histoNames, "sameAreaStack", lumi,step, 5., false);
//  
//  histoNames.at(0) = "lep_absEta";
//  stack -> SetXaxisRange(0., 3.);
//  stack -> SetXaxisTitle("|#eta_{lep}|");
//  stack -> Draw(histoNames, "sameAreaStack", lumi,step, 4., false);
//  
//  
//  
//  
//  //histoNames.at(0) = "lepJet_DR";
//  //stack -> SetXaxisRange(0., 5.);
//  //stack -> SetXaxisTitle("DR_{lep-jet}");
//  //stack -> DrawData(histoName, step, 10., true);
////  
////  histoNames.at(0) = "lepMet_Dphi";
////  stack -> SetXaxisRange(0., 3.);
////  stack -> SetXaxisTitle("#Delta#phi_{lep-met}");
////  stack -> DrawData(histoName, step, 5., false);
////  
//  
//  
  histoNames.at(0) = "ele_tkIso_EB";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("EB #Sigma pt_{tracks}");
  stack -> Draw(histoNames, "sameAreaStack", lumi, step, 20., true);
  
  histoNames.at(0) = "ele_emIso_EB";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("EB #Sigma et_{em}");
  stack -> Draw(histoNames, "sameAreaStack", lumi, step, 20., true);
  
  histoNames.at(0) = "ele_hadIso_EB";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("EB #Sigma et_{had}");
  stack -> Draw(histoNames, "sameAreaStack", lumi, step, 20., true);
  
  histoNames.at(0) = "ele_fbrem_EB";
  stack -> SetXaxisRange(0., 1.);
  stack -> SetXaxisTitle("EB f_{brem}");
  stack -> Draw(histoNames, "sameAreaStack", lumi, step, 20., true);
  
  histoNames.at(0) = "ele_HoverE_EB";
  stack -> SetXaxisRange(0., 1.);
  stack -> SetXaxisTitle("EB H/E");
  stack -> Draw(histoNames, "sameAreaStack", lumi, step, 10., true);
  
  histoNames.at(0) = "ele_DphiIn_EB";
  stack -> SetXaxisRange(-0.1, 0.1);
  stack -> SetXaxisTitle("EB #Delta#phi_{in}");
  stack -> Draw(histoNames, "sameAreaStack", lumi, step, 20., true);
  
  histoNames.at(0) = "ele_DetaIn_EB";
  stack -> SetXaxisRange(-0.1, 0.1);
  stack -> SetXaxisTitle("EB #Delta#eta_{in}");
  stack -> Draw(histoNames, "sameAreaStack", lumi, step, 20., true);
  
  histoNames.at(0) = "ele_sigmaIetaIeta_EB";
  stack -> SetXaxisRange(-0.1, 0.1);
  stack -> SetXaxisTitle("EB #sigma_{I#etaI#eta}");
  stack -> Draw(histoNames, "sameAreaStack", lumi, step, 2., true);
  
  histoNames.at(0) = "ele_dxy_EB";
  stack -> SetXaxisRange(-0.1, 0.1);
  stack -> SetXaxisTitle("EB d_{xy}");
  stack -> Draw(histoNames, "sameAreaStack", lumi, step, 20., true);

  histoNames.at(0) = "ele_dz_EB";
  stack -> SetXaxisRange(-0.1, 0.1);
  stack -> SetXaxisTitle("EB d_{z}");
  stack -> Draw(histoNames, "sameAreaStack", lumi, step, 2., false);
  
  
  
  histoNames.at(0) = "ele_tkIso_EE";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("EE #Sigma pt_{tracks}");
  stack -> Draw(histoNames, "sameAreaStack", lumi, step, 20., true);
  
  histoNames.at(0) = "ele_emIso_EE";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("EE #Sigma et_{em}");
  stack -> Draw(histoNames, "sameAreaStack", lumi, step, 20., true);
  
  histoNames.at(0) = "ele_hadIso_EE";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("EE #Sigma et_{had}");
  stack -> Draw(histoNames, "sameAreaStack", lumi, step, 20., true);
  
  histoNames.at(0) = "ele_fbrem_EE";
  stack -> SetXaxisRange(0., 1.);
  stack -> SetXaxisTitle("EE f_{brem}");
  stack -> Draw(histoNames, "sameAreaStack", lumi, step, 20., true);
  
  histoNames.at(0) = "ele_HoverE_EE";
  stack -> SetXaxisRange(0., 1.);
  stack -> SetXaxisTitle("EE H/E");
  stack -> Draw(histoNames, "sameAreaStack", lumi, step, 10., true);
  
  histoNames.at(0) = "ele_DphiIn_EE";
  stack -> SetXaxisRange(-0.1, 0.1);
  stack -> SetXaxisTitle("EE #Delta#phi_{in}");
  stack -> Draw(histoNames, "sameAreaStack", lumi, step, 20., true);
  
  histoNames.at(0) = "ele_DetaIn_EE";
  stack -> SetXaxisRange(-0.1, 0.1);
  stack -> SetXaxisTitle("EE #Delta#eta_{in}");
  stack -> Draw(histoNames, "sameAreaStack", lumi, step, 20., true);
  
  histoNames.at(0) = "ele_sigmaIetaIeta_EE";
  stack -> SetXaxisRange(-0.1, 0.1);
  stack -> SetXaxisTitle("EE #sigma_{I#etaI#eta}");
  stack -> Draw(histoNames, "sameAreaStack", lumi, step, 2., true);
  
  histoNames.at(0) = "ele_dxy_EE";
  stack -> SetXaxisRange(-0.1, 0.1);
  stack -> SetXaxisTitle("EE d_{xy}");
  stack -> Draw(histoNames, "sameAreaStack", lumi, step, 20., true);

  histoNames.at(0) = "ele_dz_EE";
  stack -> SetXaxisRange(-0.1, 0.1);
  stack -> SetXaxisTitle("EE d_{z}");
  stack -> Draw(histoNames, "sameAreaStack", lumi, step, 2., false);
////  
////  
////  
////  histoNames.at(0) = "mu_tkIso";
////  stack -> SetXaxisRange(0., 0.1);
////  stack -> SetXaxisTitle("#Sigma pt_{tracks}/pt_{mu}");
////  stack -> DrawData(histoName, step, 1., true);
////  
////  histoNames.at(0) = "mu_emIso";
////  stack -> SetXaxisRange(0., 0.1);
////  stack -> SetXaxisTitle("#Sigma et_{em}/et_{mu}");
////  stack -> DrawData(histoName, step, 1., true);  
////  
////  histoNames.at(0) = "mu_hadIso";
////  stack -> SetXaxisRange(0., 0.1);
////  stack -> SetXaxisTitle("#Sigma et_{had}/et_{mu}");
////  stack -> DrawData(histoName, step, 1., true);
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
  //---------------------------------------------  
  // Met
  //---------------------------------------------
  
  histoNames.at(0) = "met_et";
  stack -> SetXaxisRange(0., 100.);
  stack -> SetYaxisRange(0.01, 100.);
  stack -> SetXaxisTitle("et_{met}");
  stack -> Draw(histoNames, "sameAreaStack", lumi, step, 5, true);
  
  histoNames.at(0) = "lepMet_mt";
  stack -> SetXaxisRange(0., 150.);
  stack -> SetYaxisRange(0.01, 100.);
  stack -> SetXaxisTitle("mt_{lep+met}");
  stack -> Draw(histoNames, "sameAreaStack", lumi, step, 5., true);

  histoNames.at(0) = "lepMet_Dphi";
  stack -> SetXaxisRange(0., 3.5);
  stack -> SetYaxisRange(0.01, 500.);
  stack -> SetXaxisTitle("#Delta#phi_{lep+met}");
  stack -> Draw(histoNames, "sameAreaStack", lumi, step, 5., true);
  
  histoNames.at(0) = "lepMetW_mt";
  stack -> SetXaxisRange(0., 400.);
  stack -> SetYaxisRange(0.001, 100.);
  stack -> SetXaxisTitle("mt_{lep+met+W}");
  stack -> Draw(histoNames, "sameAreaStack", lumi, step, 10., true);

//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  //---------------------------------------------  
//  // Jets
//  //---------------------------------------------
//  
//  histoNames.at(0) = "jet_n";
//  stack -> SetXaxisRange(0., 10.);
//  stack -> SetXaxisTitle("n_{jet}");
//  stack -> DrawData(histoName, step, 1., true);
//  
//  histoNames.at(0) = "jet_et";
//  stack -> SetXaxisRange(0., 100.);
//  stack -> SetXaxisTitle("et_{jet}");
//  stack -> DrawData(histoName, step, 2., true);
//  
//  histoNames.at(0) = "jet_phi";
//  stack -> SetXaxisRange(0., 3.5);
//  stack -> SetXaxisTitle("#phi_{jet}");
//  stack -> Draw(histoNames, "sameAreaStack", lumi,step, 4., false);
//  
//  histoNames.at(0) = "jet_eta";
//  stack -> SetXaxisRange(-5., 5.);
//  stack -> SetXaxisTitle("#eta_{jet}");
//  stack -> Draw(histoNames, "sameAreaStack", lumi,step, 4., false);
//  
//  histoNames.at(0) = "jet_absEta";
//  stack -> SetXaxisRange(0., 5.);
//  stack -> SetXaxisTitle("|#eta_{jet}|");
//  stack -> Draw(histoNames, "sameAreaStack", lumi,step, 4., false);
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
  
  //---------------------------------------------  
  // tag Jets
  //---------------------------------------------
  
  histoNames.at(0) = "tagJJ_m";
  stack -> SetXaxisRange(0., 2000.);
  stack -> SetYaxisRange(0.1, 100.);
  stack -> SetXaxisTitle("m_{tagJJ}");
  stack -> Draw(histoNames, "sameAreaStack", lumi, step, 50., true);
  
  histoNames.at(0) = "tagJJ_Dphi";
  stack -> SetXaxisRange(0., 3.);
  stack -> SetXaxisTitle("#Delta#phi_{tagJJ}");
  stack -> Draw(histoNames, "sameAreaStack", lumi,step, 10, false);
  
  histoNames.at(0) = "tagJJ_Deta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#Delta#eta_{tagJJ}");
  stack -> Draw(histoNames, "sameAreaStack", lumi,step, 10., false);
  
  histoNames.at(0) = "tagJJ_DR";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#DeltaR_{tagJJ}");
  stack -> Draw(histoNames, "sameAreaStack", lumi,step, 10., false);

  histoNames.at(0) = "tagJJ_etaProd";
  stack -> SetXaxisRange(-25., 25.);
  stack -> SetXaxisTitle("(#eta_1 x #eta_2)_{tagJJ}");
  stack -> Draw(histoNames, "sameAreaStack", lumi,step, 10., false);  
  
  
  
  histoNames2.at(0) = "tagJJ_min_e";
  histoNames2.at(1) = "tagJJ_max_e";
  stack -> SetXaxisRange(0., 1000.);
  stack -> SetXaxisTitle("e_{tagJ}");
  stack -> Draw(histoNames2, "sameAreaStack", lumi,step, 40., true);
  
  histoNames2.at(0) = "tagJJ_min_et";
  histoNames2.at(1) = "tagJJ_max_et";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("et_{tagJ}");
  stack -> Draw(histoNames2, "sameAreaStack", lumi,step, 4., true);
  
  histoNames2.at(0) = "tagJJ_min_eta";
  histoNames2.at(1) = "tagJJ_max_eta";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta_{tagJ}");
  stack -> Draw(histoNames2, "sameAreaStack", lumi,step, 10., false);
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // W Jets
  //---------------------------------------------
  
  histoNames.at(0) = "WJJ_m";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("m_{Wjj}");
  stack -> Draw(histoNames, "sameAreaStack", lumi,step, 5., false);
  
  histoNames.at(0) = "WJJ_Dphi";
  stack -> SetXaxisRange(-3., 3.);
  stack -> SetXaxisTitle("#Delta#phi_{Wjj}");
  stack -> Draw(histoNames, "sameAreaStack", lumi,step, 5., false);
  
  histoNames.at(0) = "WJJ_Deta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#Delta#eta_{Wjj}");
  stack -> Draw(histoNames, "sameAreaStack", lumi,step, 5., false);
  
  histoNames.at(0) = "WJJ_DR";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#DeltaR_{Wjj}");
  stack -> Draw(histoNames, "sameAreaStack", lumi,step, 5., false);
  
  histoNames.at(0) = "WJJ_et";
  stack -> SetXaxisRange(0., 100.);
  stack -> SetXaxisTitle("et_{WJJ}");
  stack -> Draw(histoNames, "sameAreaStack", lumi,step, 5., false);
  
  histoNames.at(0) = "WJJ_phi";
  stack -> SetXaxisRange(-3.5, 3.5);
  stack -> SetXaxisTitle("#phi_{WJJ}");
  stack -> Draw(histoNames, "sameAreaStack", lumi,step, 5., false);
  
  histoNames.at(0) = "WJJ_eta";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta_{WJJ}");
  stack -> Draw(histoNames, "sameAreaStack", lumi,step, 5., false);

  histoNames.at(0) = "WJJ_zepp";
  stack -> SetXaxisRange(0., 1.);
  stack -> SetXaxisTitle("z_{WJJ}");
  stack -> Draw(histoNames, "sameAreaStack", lumi,step, 10., false);  
  
  
  
  histoNames2.at(0) = "WJJ_min_e";
  histoNames2.at(1) = "WJJ_max_e";
  stack -> SetXaxisRange(0., 500.);
  stack -> SetXaxisTitle("e_{WJ}");
  stack -> Draw(histoNames2, "sameAreaStack", lumi,step, 10., true);
  
  histoNames2.at(0) = "WJJ_min_et";
  histoNames2.at(1) = "WJJ_max_et";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("et_{WJ}");
  stack -> Draw(histoNames2, "sameAreaStack", lumi,step, 4., true);
  
  histoNames2.at(0) = "WJJ_min_eta";
  histoNames2.at(1) = "WJJ_max_eta";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta_{WJ}");
  stack -> Draw(histoNames2, "sameAreaStack", lumi,step, 10., false);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // tag-W Jets
  //---------------------------------------------

  histoNames.at(0) = "tagWJ_bTag";
  stack -> SetXaxisRange(-10., 10.);
  stack -> SetYaxisRange(0.1, 200.);
  stack -> SetXaxisTitle("bTag_{tag-WJ}");
  stack -> Draw(histoNames, "sameAreaStack", lumi,step, 2., true);
  
  histoNames.at(0) = "tagWJ_emEnergyFraction";
  stack -> SetXaxisRange(0., 1.);
  stack -> SetXaxisTitle("emEnergyFraction_{tag-WJ}");
  stack -> Draw(histoNames, "sameAreaStack", lumi,step, 10., false);
  
  histoNames.at(0) = "tagWJ_etaEtaMoment";
  stack -> SetXaxisRange(0., 0.1);
  stack -> SetXaxisTitle("#eta#eta_{tag-WJ}");
  stack -> Draw(histoNames, "sameAreaStack", lumi,step, 20., false);
  
  histoNames.at(0) = "tagWJ_phiPhiMoment";
  stack -> SetXaxisRange(0., 0.1);
  stack -> SetXaxisTitle("#phi#phi_{tag-WJ}");
  stack -> Draw(histoNames, "sameAreaStack", lumi,step, 20., false);
  
  histoNames.at(0) = "tagWJ_etaPhiMoment";
  stack -> SetXaxisRange(-0.05, 0.05);
  stack -> SetXaxisTitle("#eta#phi_{tag-WJ}");
  stack -> Draw(histoNames, "sameAreaStack", lumi,step, 5., false);
  
  histoNames.at(0) = "tagWJ_fHPD";
  stack -> SetXaxisRange(0., 1.);
  stack -> SetXaxisTitle("fHPD_{tag-WJ}");
  stack -> Draw(histoNames, "sameAreaStack", lumi,step, 10., true);
  
  histoNames.at(0) = "tagWJ_fRBX";
  stack -> SetXaxisRange(0., 1.);
  stack -> SetXaxisTitle("fRBX_{tag-WJ}");
  stack -> Draw(histoNames, "sameAreaStack", lumi,step, 10., true);
  
  histoNames.at(0) = "tagWJ_n90Hits";
  stack -> SetXaxisRange(0., 100.);
  stack -> SetXaxisTitle("n90Hits_{tag-WJ}");
  stack -> Draw(histoNames, "sameAreaStack", lumi,step, 2., false);
}
