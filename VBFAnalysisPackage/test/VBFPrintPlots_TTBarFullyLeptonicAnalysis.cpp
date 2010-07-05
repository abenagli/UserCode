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
  drawTStack* stack = new drawTStack(inputDir, "listOfSamplesAndCrossSections.txt", "TTBarFullyLeptonicAnalysis", outputDir);
  
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
  //stack -> Draw(histoNames, "sameAreaNoStack", lumi,step, 4., false);
  
  //histoNames.at(0) = "ele_eta";
  //stack -> SetXaxisRange(-3., 3.);
  //stack -> SetXaxisTitle("#eta_{ele}");
  //stack -> Draw(histoNames, "sameAreaNoStack", lumi,step, 4., false);
  
  //histoNames.at(0) = "ele_absEta";
  //stack -> SetXaxisRange(0., 3.);
  //stack -> SetXaxisTitle("|#eta_{ele}|");
  //stack -> Draw(histoNames, "sameAreaNoStack", lumi,step, 4., false);
  
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
  //stack -> Draw(histoNames, "sameAreaNoStack", lumi,step, 4., false);
  
  //histoNames.at(0) = "mu_eta";
  //stack -> SetXaxisRange(-3., 3.);
  //stack -> SetXaxisTitle("#eta_{mu}");
  //stack -> Draw(histoNames, "sameAreaNoStack", lumi,step, 4., false);
  
  //histoNames.at(0) = "mu_absEta";
  //stack -> SetXaxisRange(0., 3.);
  //stack -> SetXaxisTitle("|#eta_{mu}|");
  //stack -> Draw(histoNames, "sameAreaNoStack", lumi,step, 4., false);
  
  //histoNames.at(0) = "muMet_mt";
  //stack -> SetXaxisRange(0., 200.);
  //stack -> SetXaxisTitle("mt_{mu+met}");
  //stack -> DrawData(histoName, step, 4., true);
  
  
  
  
  
  
  //histoNames.at(0) = "lep_n";
  //stack -> SetXaxisRange(0., 10.);
  //stack -> SetXaxisTitle("n_{lep}");
  //stack -> DrawData(histoName, step, 1., true);
  
  histoNames2.at(0) = "leplep_min_pt";
  histoNames2.at(1) = "leplep_max_pt";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("pt_{lep}");
  stack -> Draw(histoNames2, "sameAreaNoStack", lumi, step, 2., false);

  histoNames.at(0) = "leplep_min_pt";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("min pt_{lep}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 2., false);
  
  histoNames.at(0) = "leplep_max_pt";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("max pt_{lep}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 2., false);
  
        
  histoNames2.at(0) = "leplep_min_eta";
  histoNames2.at(1) = "leplep_max_eta";
  stack -> SetXaxisRange(-3., 3.);
  stack -> SetXaxisTitle("#eta_{lep}");
  stack -> Draw(histoNames2, "sameAreaNoStack", lumi,step, 2., false);
  
  histoNames.at(0) = "leplep_m";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("m_{lep-lep}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 2., true);
//  
//  histoNames.at(0) = "lep_absEta";
//  stack -> SetXaxisRange(0., 3.);
//  stack -> SetXaxisTitle("|#eta_{lep}|");
//  stack -> Draw(histoNames, "sameAreaNoStack", lumi,step, 4., false);
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
  stack -> SetXaxisRange(0., 0.15);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("EB #Sigma pt_{tracks}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., true);
  
  histoNames.at(0) = "ele_emIso_EB";
  stack -> SetXaxisRange(0., 2.);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("EB #Sigma et_{em}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., true);
  
  histoNames.at(0) = "ele_hadIso_EB";
  stack -> SetXaxisRange(0., 0.15);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("EB #Sigma et_{had}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., true);
  
  histoNames.at(0) = "ele_fbrem_EB";
  stack -> SetXaxisRange(0., 1.);
  //stack -> SetYaxisRange(0.0001, 1.);
  stack -> SetXaxisTitle("EB f_{brem}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., true);
  
  histoNames.at(0) = "ele_HoverE_EB";
  stack -> SetXaxisRange(0., 1.);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("EB H/E");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 10., true);
  
  histoNames.at(0) = "ele_DphiIn_EB";
  stack -> SetXaxisRange(-0.1, 0.1);
  //stack -> SetYaxisRange(0.0001, 1.);
  stack -> SetXaxisTitle("EB #Delta#phi_{in}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 10., true);
  
  histoNames.at(0) = "ele_DetaIn_EB";
  stack -> SetXaxisRange(-0.1, 0.1);
  //stack -> SetYaxisRange(0.0001, 1.);
  stack -> SetXaxisTitle("EB #Delta#eta_{in}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 10., true);
  
  histoNames.at(0) = "ele_sigmaIetaIeta_EB";
  stack -> SetXaxisRange(0., 0.03);
  stack -> SetXaxisTitle("EB #sigma_{I#etaI#eta}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 2., true);
  
  histoNames.at(0) = "ele_dxy_EB";
  stack -> SetXaxisRange(-0.1, 0.1);
  //stack -> SetYaxisRange(0.0001, 1.);
  stack -> SetXaxisTitle("EB d_{xy}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 10., true);

  histoNames.at(0) = "ele_dz_EB";
  stack -> SetXaxisRange(-0.1, 0.1);
  //stack -> SetYaxisRange(0.0001, 1.);
  stack -> SetXaxisTitle("EB d_{z}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 10., true);
  
  
  
  histoNames.at(0) = "ele_tkIso_EE";
  stack -> SetXaxisRange(0., 0.15);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("EE #Sigma pt_{tracks}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., true);
  
  histoNames.at(0) = "ele_emIso_EE";
  stack -> SetXaxisRange(0., 0.15);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("EE #Sigma et_{em}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 1., true);
  
  histoNames.at(0) = "ele_hadIso_EE";
  stack -> SetXaxisRange(0., 0.15);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("EE #Sigma et_{had}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., true);
  
  histoNames.at(0) = "ele_fbrem_EE";
  stack -> SetXaxisRange(0., 1.);
  //stack -> SetYaxisRange(0.0001, 1.);
  stack -> SetXaxisTitle("EE f_{brem}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., true);
  
  histoNames.at(0) = "ele_HoverE_EE";
  stack -> SetXaxisRange(0., 1.);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("EE H/E");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 10., true);
  
  histoNames.at(0) = "ele_DphiIn_EE";
  stack -> SetXaxisRange(-0.1, 0.1);
  //stack -> SetYaxisRange(0.0001, 1.);
  stack -> SetXaxisTitle("EE #Delta#phi_{in}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 10., true);
  
  histoNames.at(0) = "ele_DetaIn_EE";
  stack -> SetXaxisRange(-0.1, 0.1);
  //stack -> SetYaxisRange(0.0001, 1.);
  stack -> SetXaxisTitle("EE #Delta#eta_{in}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 10., true);
  
  histoNames.at(0) = "ele_sigmaIetaIeta_EE";
  stack -> SetXaxisRange(0., 0.03);
  stack -> SetXaxisTitle("EE #sigma_{I#etaI#eta}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 2., true);
  
  histoNames.at(0) = "ele_dxy_EE";
  stack -> SetXaxisRange(-0.1, 0.1);
  //stack -> SetYaxisRange(0.0001, 1.);
  stack -> SetXaxisTitle("EE d_{xy}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 10., true);

  histoNames.at(0) = "ele_dz_EE";
  stack -> SetXaxisRange(-0.1, 0.1);
  //stack -> SetYaxisRange(0.0001, 1.);
  stack -> SetXaxisTitle("EE d_{z}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 10., true);
  
  
  
  
  
  
  histoNames.at(0) = "mu_tkIso";
  stack -> SetXaxisRange(0., 0.15);
  //stack -> SetYaxisRange(0.0001, 1.);
  stack -> SetXaxisTitle("#Sigma pt_{tracks}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., true);
  
  histoNames.at(0) = "mu_emIso";
  stack -> SetXaxisRange(0., 0.15);
  //stack -> SetYaxisRange(0.0001, 1.);
  stack -> SetXaxisTitle("#Sigma et_{em}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., true);
  
  histoNames.at(0) = "mu_hadIso";
  stack -> SetXaxisRange(0., 0.15);
  //stack -> SetYaxisRange(0.0001, 1.);
  stack -> SetXaxisTitle("#Sigma et_{had}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., true);
  
  histoNames.at(0) = "mu_normalizedChi2";
  stack -> SetXaxisRange(0., 10.);
  //stack -> SetYaxisRange(0.0001, 1.);
  stack -> SetXaxisTitle("#chi^{2}/d.o.f.");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 10., true);
  
  histoNames.at(0) = "mu_numberOfValidTrackerHits";
  stack -> SetXaxisRange(0., 30.);
  stack -> SetXaxisTitle("n. tracker hits");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 1., false);

  histoNames.at(0) = "mu_numberOfValidMuonHits";
  stack -> SetXaxisRange(0., 60.);
  stack -> SetXaxisTitle("n. tracker hits");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 1., false);
  
  histoNames.at(0) = "mu_3DipSig";
  stack -> SetXaxisRange(-10., 10.);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("IP_{3D}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., true);

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
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("et_{met}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 2, true);
  
//  histoNames.at(0) = "lepMetW_mt";
//  stack -> SetXaxisRange(0., 400.);
//  //stack -> SetYaxisRange(0.000001, 100.);
//  stack -> SetXaxisTitle("mt_{lep+met+W}");
//  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 4., true);
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
  // Jets
  //---------------------------------------------
  
  histoNames.at(0) = "jets_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("n_{jet}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 1., true);
  
  histoNames.at(0) = "jets_et";
  stack -> SetXaxisRange(0., 100.);
  stack -> SetXaxisTitle("et_{jet}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 2., true);
  
  histoNames.at(0) = "jets_phi";
  stack -> SetXaxisRange(0., 3.5);
  stack -> SetXaxisTitle("#phi_{jet}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi,step, 4., false);
  
  histoNames.at(0) = "jets_eta";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta_{jet}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi,step, 4., false);
  
  histoNames.at(0) = "jets_absEta";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("|#eta_{jet}|");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi,step, 4., false);
  
  histoNames.at(0) = "jet_bTag1";
  stack -> SetXaxisRange(-5., 10.);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("bTag1_{jet}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi,step, 2., true);

  histoNames.at(0) = "jet_bTag2";
  stack -> SetXaxisRange(-5., 10.);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("bTag2_{jet}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi,step, 2., true);

  histoNames.at(0) = "jet_bTag3";
  stack -> SetXaxisRange(-5., 10.);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("bTag3_{jet}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi,step, 2., true);

  histoNames.at(0) = "jet_bTag4";
  stack -> SetXaxisRange(-5., 10.);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("bTag4_{jet}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi,step, 2., true);
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
  // tag Jets
  //---------------------------------------------
  
  histoNames.at(0) = "tagJJ_m";
  stack -> SetXaxisRange(0., 2000.);
  stack -> SetXaxisTitle("m_{tagJJ}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 40., true);
  
  histoNames.at(0) = "tagJJ_Dphi";
  stack -> SetXaxisRange(0., 3.);
  stack -> SetXaxisTitle("#Delta#phi_{tagJJ}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi,step, 10, false);
  
  histoNames.at(0) = "tagJJ_Deta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#Delta#eta_{tagJJ}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi,step, 10., false);
  
  histoNames.at(0) = "tagJJ_DR";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#DeltaR_{tagJJ}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi,step, 10., false);

  
  
  
  histoNames2.at(0) = "tagJJ_min_e";
  histoNames2.at(1) = "tagJJ_max_e";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("e_{tagJ}");
  stack -> Draw(histoNames2, "sameAreaNoStack", lumi,step, 10., false);
  
  histoNames2.at(0) = "tagJJ_min_et";
  histoNames2.at(1) = "tagJJ_max_et";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("et_{tagJ}");
  stack -> Draw(histoNames2, "sameAreaNoStack", lumi,step, 4., false);
  
  histoNames2.at(0) = "tagJJ_min_eta";
  histoNames2.at(1) = "tagJJ_max_eta";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta_{tagJ}");
  stack -> Draw(histoNames2, "sameAreaNoStack", lumi,step, 5., false);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // 3rd Jet
  //---------------------------------------------
  
  histoNames.at(0) = "3rdJ_eta";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta_{3rd jet}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., true);
  
  histoNames.at(0) = "3rdJ_et";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("et_{3rd jet}");
  stack -> Draw(histoNames, "sameAreaNoStack", lumi, step, 5., true);
}
