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
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>



int RunCommand(const char *strCommand);





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

  std::string baseDir         =  gConfigParser -> readStringOption("Input::baseDir");  
  std::string inputDir        = gConfigParser -> readStringOption("Input::inputDir");
  std::string samplesListFile = gConfigParser -> readStringOption("Input::samplesListFile");
  std::string inputFileName   = gConfigParser -> readStringOption("Input::inputFileName");
  
  std::string outputDir = gConfigParser -> readStringOption("Output::outputDir");

  int step = gConfigParser -> readIntOption("Options::step");  
  float lumi = gConfigParser -> readFloatOption("Options::lumi");
  std::string method = gConfigParser -> readStringOption("Options::method");
  std::string jetType = gConfigParser -> readStringOption("Options::jetType");
  
  char stepDir[50];
	sprintf(stepDir, "plots_%d", step);
  char stepNumber[50];
	sprintf(stepNumber, "%d", step);
	std::string fullOutputDir = outputDir + "/" + stepDir + "_" + jetType + "_" + method + "/";
	std::string fullHtmlName = outputDir + "/" + stepDir + "_" + jetType + "_" + method + ".html";
  
  
  
  
  
  
  // make directory
  int iStatus = 0;
	
  std::string command = "rm -rf " + fullOutputDir + "/*" ;
	iStatus = RunCommand(command.c_str());

	command = "mkdir " + fullOutputDir;
	iStatus = RunCommand(command.c_str());  
  
  
  
  
  
  
  // make html
  iStatus = 0;
	
	if( method == "sameAreaStack" )
    command = "cat " + baseDir + "/cfg/plots.html | sed -e s%STEP%" + stepNumber + "%g | sed -e s%NORM%area%g | sed -e s%DIR%" + stepDir + "_" + jetType + "_" + method + "%g > " + fullHtmlName;
	if( method == "eventsScaled" )
    command = "cat " + baseDir + "/cfg/plots.html | sed -e s%STEP%" + stepNumber + "%g | sed -e s%NORM%lumi%g | sed -e s%DIR%" + stepDir + "_" + jetType + "_" + method + "%g > " + fullHtmlName;
	
	iStatus = RunCommand(command.c_str());

  
  
  
  
  
  // draw plots
  drawTStack* stack = new drawTStack(inputDir, samplesListFile, inputFileName, fullOutputDir);
  
  stack -> DrawEvents("events", lumi, step, true);
  stack -> DrawEvents("eventsScaled", lumi, step, true);
  stack -> DrawEvents("eventsScaledStack", lumi, step, true);
  stack -> DrawEvents("efficiencies", lumi, step, true);
  stack -> DrawEvents("efficienciesRelative", lumi, step, true);
  //stack -> DrawEventRatio_nJets("int", lumi, step, false);
  //stack -> DrawEventRatio_nJets("0jets", lumi, step, false);
  //stack -> DrawEventRatio_nJets("1jets", lumi, step, false);
  //stack -> DrawEventRatio_nJets("2jets", lumi, step, false);
    
  
  
  
  
  std::string histoName;
  std::vector<std::string> histoNames;
  histoNames.push_back("");
  std::vector<std::string> histoNames2;
  histoNames2.push_back("");
  histoNames2.push_back("");
  
  
  
  
  
  
  //---------------------------------------------  
  // PV
  //---------------------------------------------

  histoNames.at(0) = "PV_d0";
  stack -> SetXaxisRange(0., 0.2);
  stack -> SetXaxisTitle("d0_{PV}");
  stack -> SetUnit("cm");
  stack -> Draw(histoNames, method, lumi, step, 1., false);

  histoNames.at(0) = "PV_z";
  stack -> SetXaxisRange(-30., 30.);
  stack -> SetXaxisTitle("z_{PV}");
  stack -> SetUnit("cm");
  stack -> Draw(histoNames, method, lumi, step, 20., false);
  
  histoNames.at(0) = "PV_nTracks";
  stack -> SetXaxisRange(0., 100);
  stack -> SetXaxisTitle("nTracks_{PV}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 2., false);  

  histoNames.at(0) = "PV_ndof";
  stack -> SetXaxisRange(0., 200);
  stack -> SetXaxisTitle("ndof_{PV}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 2., false);    

  histoNames.at(0) = "PV_normalizedChi2";
  stack -> SetXaxisRange(0., 2.);
  stack -> SetXaxisTitle("#chi^{2}/ndof_{PV}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 5., false);      
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // lepton
  //---------------------------------------------

  //histoNames.at(0) = "lep_n";
  //stack -> SetXaxisRange(0., 10.);
  //stack -> SetXaxisTitle("n_{lep}");
  //stack -> DrawData(histoName, step, 1., true);
  
  histoNames.at(0) = "lep_pt";
  stack -> SetXaxisRange(15., 200.);
  stack -> SetXaxisTitle("pt_{lep}");
  stack -> SetUnit("GeV/c");
  stack -> Draw(histoNames, method, lumi, step, 2., false);

  histoNames.at(0) = "lep_p_pt";
  stack -> SetXaxisRange(15., 200.);
  stack -> SetXaxisTitle("pt_{lep} - e^{+}");
  stack -> SetUnit("GeV/c");
  stack -> Draw(histoNames, method, lumi, step, 2., false);

  histoNames.at(0) = "lep_m_pt";
  stack -> SetXaxisRange(15., 200.);
  stack -> SetXaxisTitle("pt_{lep} - e^{-}");
  stack -> SetUnit("GeV/c");
  stack -> Draw(histoNames, method, lumi, step, 2., false);  
  
  
  
  histoNames.at(0) = "lep_phi";
  stack -> SetXaxisRange(-3.5, 3.5);
  stack -> SetXaxisTitle("#phi_{lep}");
  stack -> SetUnit("rad");
  stack -> Draw(histoNames, method, lumi,step, 8., false);

  histoNames.at(0) = "lep_p_phi";
  stack -> SetXaxisRange(-3.5, 3.5);
  stack -> SetXaxisTitle("#phi_{lep} - e^{+}");
  stack -> SetUnit("rad");
  stack -> Draw(histoNames, method, lumi,step, 8., false);
  
  histoNames.at(0) = "lep_m_phi";
  stack -> SetXaxisRange(-3.5, 3.5);
  stack -> SetXaxisTitle("#phi_{lep} - e^{-}");
  stack -> SetUnit("rad");
  stack -> Draw(histoNames, method, lumi,step, 8., false);
  
  
  
  histoNames.at(0) = "lep_eta";
  stack -> SetXaxisRange(-3., 3.);
  stack -> SetXaxisTitle("#eta_{lep}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi,step, 4., false);
  
  histoNames.at(0) = "lep_p_eta";
  stack -> SetXaxisRange(-3., 3.);
  stack -> SetXaxisTitle("#eta_{lep} - e^{+}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi,step, 4., false);
  
  histoNames.at(0) = "lep_m_eta";
  stack -> SetXaxisRange(-3., 3.);
  stack -> SetXaxisTitle("#eta_{lep} - e^{-}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi,step, 4., false);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // Met
  //---------------------------------------------
  
  histoNames.at(0) = "met_et";
  stack -> SetXaxisRange(15., 80.);
  stack -> SetXaxisTitle("et_{met}");
  stack -> SetUnit("GeV");
  stack -> Draw(histoNames, method, lumi, step, 2, false);

  histoNames.at(0) = "met_p_et";
  stack -> SetXaxisRange(15., 80.);
  stack -> SetXaxisTitle("et_{met} - e^{+}");
  stack -> SetUnit("GeV");
  stack -> Draw(histoNames, method, lumi, step, 2, false);
  
  histoNames.at(0) = "met_m_et";
  stack -> SetXaxisRange(15., 80.);
  stack -> SetXaxisTitle("et_{met} - e^{-}");
  stack -> SetUnit("GeV");
  stack -> Draw(histoNames, method, lumi, step, 2, false);  
  
  
  
  histoNames.at(0) = "lepMet_mt";
  stack -> SetXaxisRange(25., 150.);
  stack -> SetXaxisTitle("mt_{lep+met}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(histoNames, method, lumi, step, 4., false);
  
  histoNames.at(0) = "lepMet_mt_p";
  stack -> SetXaxisRange(25., 150.);
  stack -> SetXaxisTitle("mt_{lep+met} - e^{+}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(histoNames, method, lumi, step, 4., false);
  
  histoNames.at(0) = "lepMet_mt_m";
  stack -> SetXaxisRange(25., 150.);
  stack -> SetXaxisTitle("mt_{lep+met} - e^{-}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(histoNames, method, lumi, step, 4., false);
  
  
  
  histoNames.at(0) = "lepMet_Dphi";
  stack -> SetXaxisRange(0., 3.15);
  stack -> SetXaxisTitle("#Delta#phi_{lep+met}");
  stack -> SetUnit("rad");
  stack -> Draw(histoNames, method, lumi, step, 2., false);

  histoNames.at(0) = "lepMet_p_Dphi";
  stack -> SetXaxisRange(0., 3.15);
  stack -> SetXaxisTitle("#Delta#phi_{lep+met} - e^{+}");
  stack -> SetUnit("rad");
  stack -> Draw(histoNames, method, lumi, step, 2., false);
  
  histoNames.at(0) = "lepMet_m_Dphi";
  stack -> SetXaxisRange(0., 3.15);
  stack -> SetXaxisTitle("#Delta#phi_{lep+met} - e^{-}");
  stack -> SetUnit("rad");
  stack -> Draw(histoNames, method, lumi, step, 2., false);  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // electron
  //---------------------------------------------
  
  histoNames.at(0) = "ele_DphiIn_EB";
  stack -> SetXaxisRange(-0.05, 0.05);
  //stack -> SetYaxisRange(0.0001, 1.);
  stack -> SetXaxisTitle("EB #Delta#phi_{in}");
  stack -> SetUnit("rad");
  stack -> Draw(histoNames, method, lumi, step, 2., false);
  
  histoNames.at(0) = "ele_DphiIn_EE";
  stack -> SetXaxisRange(-0.05, 0.05);
  //stack -> SetYaxisRange(0.0001, 1.);
  stack -> SetXaxisTitle("EE #Delta#phi_{in}");
  stack -> SetUnit("rad");
  stack -> Draw(histoNames, method, lumi, step, 2., false);
  
  
  
  histoNames.at(0) = "ele_DetaIn_EB";
  stack -> SetXaxisRange(-0.02, 0.02);
  //stack -> SetYaxisRange(0.0001, 1.);
  stack -> SetXaxisTitle("EB #Delta#eta_{in}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 4., false);
  
  histoNames.at(0) = "ele_DetaIn_EE";
  stack -> SetXaxisRange(-0.02, 0.02);
  //stack -> SetYaxisRange(0.0001, 1.);
  stack -> SetXaxisTitle("EE #Delta#eta_{in}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 4., false);
  
  
  
  histoNames.at(0) = "ele_sigmaIetaIeta_EB";
  stack -> SetXaxisRange(0., 0.03);
  stack -> SetXaxisTitle("EB #sigma_{I#etaI#eta}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 5., false);
  
  histoNames.at(0) = "ele_sigmaIetaIeta_EE";
  stack -> SetXaxisRange(0.01, 0.07);
  stack -> SetXaxisTitle("EE #sigma_{I#etaI#eta}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 10., false);
  
  
  
  histoNames.at(0) = "ele_HoverE_EB";
  stack -> SetXaxisRange(0., 0.20);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("EB H/E");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 5., true);
  
  histoNames.at(0) = "ele_HoverE_EE";
  stack -> SetXaxisRange(0., 0.20);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("EE H/E");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 5., true);
  
  
  
  histoNames.at(0) = "ele_dxy_EB";
  stack -> SetXaxisRange(-0.1, 0.1);
  //stack -> SetYaxisRange(0.0001, 1.);
  stack -> SetXaxisTitle("EB d_{xy}");
  stack -> SetUnit("cm");
  stack -> Draw(histoNames, method, lumi, step, 2., false);
  
  histoNames.at(0) = "ele_dxy_EE";
  stack -> SetXaxisRange(-0.2, 0.2);
  //f(0.0001, 1.);
  stack -> SetXaxisTitle("EE d_{xy}");
  stack -> SetUnit("cm");
  stack -> Draw(histoNames, method, lumi, step, 2., false);
  
  
  
  histoNames.at(0) = "ele_dz_EB";
  stack -> SetXaxisRange(-0.1, 0.1);
  //stack -> SetYaxisRange(0.0001, 1.);
  stack -> SetXaxisTitle("EB d_{z}");
  stack -> SetUnit("cm");
  stack -> Draw(histoNames, method, lumi, step, 2., false);
  
  histoNames.at(0) = "ele_dz_EE";
  stack -> SetXaxisRange(-0.2, 0.2);
  //stack -> SetYaxisRange(0.0001, 1.);
  stack -> SetXaxisTitle("EE d_{z}");
  stack -> SetUnit("cm");
  stack -> Draw(histoNames, method, lumi, step, 2., false);
  
  
  
  histoNames.at(0) = "ele_fbrem_EB";
  stack -> SetXaxisRange(0., 1.);
  //stack -> SetYaxisRange(0.0001, 1.);
  stack -> SetXaxisTitle("EB f_{brem}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 40., true);
  
  histoNames.at(0) = "ele_fbrem_EE";
  stack -> SetXaxisRange(0., 1.);
  //stack -> SetYaxisRange(0.0001, 1.);
  stack -> SetXaxisTitle("EE f_{brem}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 40., true);
  
  
  
  histoNames.at(0) = "ele_eOverP_EB";
  stack -> SetXaxisRange(0., 4.);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("EB E/p");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 5., false);
  
  histoNames.at(0) = "ele_eOverP_EE";
  stack -> SetXaxisRange(0., 4.);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("EE E/p");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 5., false);
  
  
  
  histoNames.at(0) = "ele_3DipSig_EB";
  stack -> SetXaxisRange(-10., 10.);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("EB 3D ip");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 4., true);

  histoNames.at(0) = "ele_3DipSig_EE";
  stack -> SetXaxisRange(-10., 10.);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("EE 3D ip");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 4., true);  
  
  
  
  
  
  
  histoNames.at(0) = "ele_tkIso_EB";
  stack -> SetXaxisRange(0., 0.25);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("EB #Sigma pt_{tracks} / pt_{ele}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 10., true);
  
  histoNames.at(0) = "ele_tkIso_EE";
  stack -> SetXaxisRange(0., 0.25);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("EE #Sigma pt_{tracks} / pt_{ele}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 10., true);



  histoNames.at(0) = "ele_emIso_EB";
  stack -> SetXaxisRange(0., 0.25);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("EB #Sigma et_{ECAL} / pt_{ele}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 10., true);
  
  histoNames.at(0) = "ele_emIso_EE";
  stack -> SetXaxisRange(0., 0.25);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("EE #Sigma et_{ECAL} / pt_{ele}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 10., true);
  
  
  
  histoNames.at(0) = "ele_hadIso_EB";
  stack -> SetXaxisRange(0., 0.25);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("EB #Sigma et_{HCAL} / pt_{ele}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 10., true);
  
  histoNames.at(0) = "ele_hadIso_EE";
  stack -> SetXaxisRange(0., 0.25);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("EE #Sigma et_{HCAL} / pt_{ele}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 10., true);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // muon
  //---------------------------------------------
  
  histoNames.at(0) = "mu_tkIso";
  stack -> SetXaxisRange(0., 0.25);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("#Sigma pt_{tracks} / pt_{mu}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 10., true);

  histoNames.at(0) = "mu_emIso";
  stack -> SetXaxisRange(0., 0.25);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("#Sigma et_{ECAL} / pt_{mu}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 10., true);  

  histoNames.at(0) = "mu_hadIso";
  stack -> SetXaxisRange(0., 0.25);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("#Sigma et_{HCAL} / pt_{mu}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 10., true);  
  
  
  
  histoNames.at(0) = "mu_dxy";
  stack -> SetXaxisRange(-0.02, 0.02);
  //stack -> SetYaxisRange(0.0001, 1.);
  stack -> SetXaxisTitle("d_{xy}");
  stack -> SetUnit("cm");
  stack -> Draw(histoNames, method, lumi, step, 1., true);

  histoNames.at(0) = "mu_dz";
  stack -> SetXaxisRange(-0.1, 0.1);
  //stack -> SetYaxisRange(0.0001, 1.);
  stack -> SetXaxisTitle("d_{z}");
  stack -> SetUnit("cm");
  stack -> Draw(histoNames, method, lumi, step, 1., true);  
  
  histoNames.at(0) = "mu_3DipSig";
  stack -> SetXaxisRange(-10., 10.);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("3D ip");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 5., true);
  
  
  
  histoNames.at(0) = "mu_normalizedChi2";
  stack -> SetXaxisRange(0., 10.);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("#chi^{2}/ndof");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 10., true);  

  histoNames.at(0) = "mu_numberOfValidTrackerHits";
  stack -> SetXaxisRange(0., 40.);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("nHits_{tracker}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 1., false);  

  histoNames.at(0) = "mu_numberOfValidMuonHits";
  stack -> SetXaxisRange(0., 60.);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("nHits_{muon}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 1., false);    
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // Jets
  //---------------------------------------------

  histoNames.at(0) = "leadingJetLep_DR";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("DR_{leading jet-lep}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 100., false);
  
  histoNames.at(0) = "leadingJet_et";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("et_{leading jet}");
  stack -> SetUnit("GeV");
  stack -> Draw(histoNames, method, lumi, step, 5., false);
  
  histoNames.at(0) = "leadingJet_eta";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta_{leading jet}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi,step, 5., false);
  
  
  
  
  
  histoNames.at(0) = "jetLep_DR";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("DR_{jet-lep}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 100., true);
  
  
  
  histoNames.at(0) = "jets_n";
  stack -> SetXaxisRange(0., 7.);
  //stack -> SetYaxisRange(0.01, 20000.);
  stack -> SetXaxisTitle("n_{jets}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 1., true);

  histoNames.at(0) = "jets_cnt_n";
  stack -> SetXaxisRange(0., 7.);
  //stack -> SetYaxisRange(0.1, 5000.);
  stack -> SetXaxisTitle("n_{central jets}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 1., true);
  
  histoNames.at(0) = "jets_fwd_n";
  stack -> SetXaxisRange(0., 7.);
  //stack -> SetYaxisRange(0.1, 5000.);
  stack -> SetXaxisTitle("n_{forward jets}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 1., true);
  
  
    
  histoNames.at(0) = "jets_et";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("et_{jets}");
  stack -> SetUnit("GeV");
  stack -> Draw(histoNames, method, lumi, step, 10., false);
  
  histoNames.at(0) = "jets_cnt_et";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("et_{central jets}");
  stack -> SetUnit("GeV");
  stack -> Draw(histoNames, method, lumi, step, 10., false);
  
  histoNames.at(0) = "jets_fwd_et";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("et_{forward jets}");
  stack -> SetUnit("GeV");
  stack -> Draw(histoNames, method, lumi, step, 10., false);
  
  
  
  histoNames.at(0) = "jets_phi";
  stack -> SetXaxisRange(-3.5, 3.5);
  stack -> SetXaxisTitle("#phi_{jets}");
  stack -> SetUnit("rad");
  stack -> Draw(histoNames, method, lumi,step, 10., false);
  
  histoNames.at(0) = "jets_cnt_phi";
  stack -> SetXaxisRange(-3.5, 3.5);
  stack -> SetXaxisTitle("#phi_{central jets}");
  stack -> SetUnit("rad");
  stack -> Draw(histoNames, method, lumi,step, 10., false);
  
  histoNames.at(0) = "jets_fwd_phi";
  stack -> SetXaxisRange(-3.5, 3.5);
  stack -> SetXaxisTitle("#phi_{forward jets}");
  stack -> SetUnit("rad");
  stack -> Draw(histoNames, method, lumi,step, 10., false);
  
  
  
  histoNames.at(0) = "jets_eta";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta_{jets}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi,step, 10., false);

  histoNames.at(0) = "jets_cnt_eta";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta_{central jets}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi,step, 10., false);
  
  histoNames.at(0) = "jets_fwd_eta";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta_{forward jets}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi,step, 10., false);  
  
  
  
  histoNames.at(0) = "jets_bTag";
  stack -> SetXaxisRange(-5., 10.);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("bTag_{jets}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi,step, 5., true);
  
  histoNames.at(0) = "jets_cnt_bTag";
  stack -> SetXaxisRange(-5., 10.);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("bTag_{central jets}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi,step, 5., true);
  
  histoNames.at(0) = "jets_fwd_bTag";
  stack -> SetXaxisRange(-5., 10.);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("bTag_{forward jets}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi,step, 5., true);
  
  histoNames.at(0) = "jets_bTag1";
  stack -> SetXaxisRange(-5., 10.);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("bTag1_{jets}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi,step, 4., true);

  histoNames.at(0) = "jets_bTag2";
  stack -> SetXaxisRange(-5., 10.);
  //stack -> SetYaxisRange(0.00001, 1.);
  stack -> SetXaxisTitle("bTag2_{jets}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi,step, 4., true);  
  
  
  
  
  
  
  histoNames.at(0) = "jets_etaEtaMoment";
  stack -> SetXaxisRange(0., 0.1);
  stack -> SetXaxisTitle("#eta#eta_{jets}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi,step, 50., false);
  
  histoNames.at(0) = "jets_cnt_etaEtaMoment";
  stack -> SetXaxisRange(0., 0.1);
  stack -> SetXaxisTitle("#eta#eta_{central jets}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi,step, 50., false);
  
  histoNames.at(0) = "jets_fwd_etaEtaMoment";
  stack -> SetXaxisRange(0., 0.1);
  stack -> SetXaxisTitle("#eta#eta_{forward jets}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi,step, 50., false);
  
  
  
  histoNames.at(0) = "jets_phiPhiMoment";
  stack -> SetXaxisRange(0., 0.1);
  stack -> SetXaxisTitle("#phi#phi_{jets}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi,step, 50., false);
  
  histoNames.at(0) = "jets_cnt_phiPhiMoment";
  stack -> SetXaxisRange(0., 0.1);
  stack -> SetXaxisTitle("#phi#phi_{central jets}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi,step, 50., false);
  
  histoNames.at(0) = "jets_fwd_phiPhiMoment";
  stack -> SetXaxisRange(0., 0.1);
  stack -> SetXaxisTitle("#phi#phi_{forward jets}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi,step, 50., false);
  
  
  
  histoNames.at(0) = "jets_etaPhiMoment";
  stack -> SetXaxisRange(-0.05, 0.05);
  stack -> SetXaxisTitle("#eta#phi_{jets}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi,step, 10., false);
  
  histoNames.at(0) = "jets_cnt_etaPhiMoment";
  stack -> SetXaxisRange(-0.05, 0.05);
  stack -> SetXaxisTitle("#eta#phi_{central jets}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi,step, 10., false);
  
  histoNames.at(0) = "jets_fwd_etaPhiMoment";
  stack -> SetXaxisRange(-0.05, 0.05);
  stack -> SetXaxisTitle("#eta#phi_{forward jets}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi,step, 10., false);
    
  
  
  
  
  
  if(jetType == "Calo")
  {
    histoNames.at(0) = "jets_emEnergyFraction";
    stack -> SetXaxisRange(0., 1.);
    stack -> SetXaxisTitle("emEnergyFraction_{jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 50., false);
    
    histoNames.at(0) = "jets_cnt_emEnergyFraction";
    stack -> SetXaxisRange(0., 1.);
    stack -> SetXaxisTitle("emEnergyFraction_{central jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 50., false);
    
    histoNames.at(0) = "jets_fwd_emEnergyFraction";
    stack -> SetXaxisRange(0., 1.);
    stack -> SetXaxisTitle("emEnergyFraction_{forward jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 50., false);
    
    
    
    histoNames.at(0) = "jets_fHPD";
    stack -> SetXaxisRange(0., 1.);
    stack -> SetXaxisTitle("fHPD_{jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 50., false);
    
    histoNames.at(0) = "jets_cnt_fHPD";
    stack -> SetXaxisRange(0., 1.);
    stack -> SetXaxisTitle("fHPD_{central jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 50., false);
    
    histoNames.at(0) = "jets_fwd_fHPD";
    stack -> SetXaxisRange(0., 1.);
    stack -> SetXaxisTitle("fHPD_{forward jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 50., false);
    
    
    
    histoNames.at(0) = "jets_fRBX";
    stack -> SetXaxisRange(0., 1.);
    stack -> SetXaxisTitle("fRBX_{jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 50., false);
    
    histoNames.at(0) = "jets_cnt_fRBX";
    stack -> SetXaxisRange(0., 1.);
    stack -> SetXaxisTitle("fRBX_{central jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 50., false);
    
    histoNames.at(0) = "jets_fwd_fRBX";
    stack -> SetXaxisRange(0., 1.);
    stack -> SetXaxisTitle("fRBX_{forward jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 50., false);
    
    
    
    histoNames.at(0) = "jets_n90Hits";
    stack -> SetXaxisRange(0., 100.);
    stack -> SetXaxisTitle("n90Hits_{jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 10., false);
    
    histoNames.at(0) = "jets_cnt_n90Hits";
    stack -> SetXaxisRange(0., 100.);
    stack -> SetXaxisTitle("n90Hits_{central jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 10., false);
    
    histoNames.at(0) = "jets_fwd_n90Hits";
    stack -> SetXaxisRange(0., 100.);
    stack -> SetXaxisTitle("n90Hits_{forward jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 10., false);
  }
  
  
  
  
  if( jetType == "PF")
  {
    histoNames.at(0) = "jets_chargedHadronEnergyFraction";
    stack -> SetXaxisRange(0., 1.);
    stack -> SetXaxisTitle("chargedHadronEnergyFraction_{jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 50., false);
  
    histoNames.at(0) = "jets_cnt_chargedHadronEnergyFraction";
    stack -> SetXaxisRange(0., 1.);
    stack -> SetXaxisTitle("chargedHadronEnergyFraction_{central jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 50., false);
    
    histoNames.at(0) = "jets_fwd_chargedHadronEnergyFraction";
    stack -> SetXaxisRange(0., 1.);
    stack -> SetXaxisTitle("chargedHadronEnergyFraction_{forward jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 50., false);
    
    
    
    histoNames.at(0) = "jets_neutralHadronEnergyFraction";
    stack -> SetXaxisRange(0., 1.);
    stack -> SetXaxisTitle("neutralHadronEnergyFraction_{jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 50., false);  
  
    histoNames.at(0) = "jets_cnt_neutralHadronEnergyFraction";
    stack -> SetXaxisRange(0., 1.);
    stack -> SetXaxisTitle("neutralHadronEnergyFraction_{central jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 50., false);
    
    histoNames.at(0) = "jets_fwd_neutralHadronEnergyFraction";
    stack -> SetXaxisRange(0., 1.);
    stack -> SetXaxisTitle("neutralHadronEnergyFraction_{forward jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 50., false);  
    
    
    
    histoNames.at(0) = "jets_chargedEmEnergyFraction";
    stack -> SetXaxisRange(0., 1.);
    stack -> SetXaxisTitle("chargedEmEnergyFraction_{jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 50., false);  
    
    histoNames.at(0) = "jets_cnt_chargedEmEnergyFraction";
    stack -> SetXaxisRange(0., 1.);
    stack -> SetXaxisTitle("chargedEmEnergyFraction_{central jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 50., false);
    
    histoNames.at(0) = "jets_fwd_chargedEmEnergyFraction";
    stack -> SetXaxisRange(0., 1.);
    stack -> SetXaxisTitle("chargedEmEnergyFraction_{forward jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 50., false);
    
    
    
    histoNames.at(0) = "jets_neutralEmEnergyFraction";
    stack -> SetXaxisRange(0., 1.);
    stack -> SetXaxisTitle("neutralEmEnergyFraction_{jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 50., false);  
    
    histoNames.at(0) = "jets_cnt_neutralEmEnergyFraction";
    stack -> SetXaxisRange(0., 1.);
    stack -> SetXaxisTitle("neutralEmEnergyFraction_{central jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 50., false);
    
    histoNames.at(0) = "jets_fwd_neutralEmEnergyFraction";
    stack -> SetXaxisRange(0., 1.);
    stack -> SetXaxisTitle("neutralEmEnergyFraction_{forward jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 50., false);
    
    
    
    histoNames.at(0) = "jets_chargedMultiplicity";
    stack -> SetXaxisRange(0., 40.);
    stack -> SetXaxisTitle("chargedMultiplicity_{jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 1., false);  
    
    histoNames.at(0) = "jets_cnt_chargedMultiplicity";
    stack -> SetXaxisRange(0., 40.);
    stack -> SetXaxisTitle("chargedMultiplicity_{central jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 1., false);
    
    histoNames.at(0) = "jets_fwd_chargedMultiplicity";
    stack -> SetXaxisRange(0., 40.);
    stack -> SetXaxisTitle("chargedMultiplicity_{forward jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 1., false);
    
    
    
    histoNames.at(0) = "jets_neutralMultiplicity";
    stack -> SetXaxisRange(0., 40.);
    stack -> SetXaxisTitle("neutralMultiplicity_{jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 1., false);  
    
    histoNames.at(0) = "jets_cnt_neutralMultiplicity";
    stack -> SetXaxisRange(0., 40.);
    stack -> SetXaxisTitle("neutralMultiplicity_{central jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 1., false);
    
    histoNames.at(0) = "jets_fwd_neutralMultiplicity";
    stack -> SetXaxisRange(0., 40.);
    stack -> SetXaxisTitle("neutralMultiplicity_{forward jets}");
    stack -> SetUnit("");
    stack -> Draw(histoNames, method, lumi,step, 1., false);
  }
  
  
  
  
  
  
  
  
  
  
  
  
  histoNames.at(0) = "jetsjets_m";
  stack -> SetXaxisRange(0., 500.);
  //stack -> SetYaxisRange(0.02, 20.);
  stack -> SetXaxisTitle("m_{jj}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(histoNames, method, lumi, step, 20., true);

  histoNames.at(0) = "jetsjets_et";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("et_{jj}");
  stack -> SetUnit("GeV");
  stack -> Draw(histoNames, method, lumi, step, 20., false);  
  
  histoNames.at(0) = "jetsjets_Deta";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#Delta#eta_{jj}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 10., true);
  
  
  
  histoNames.at(0) = "WJJ_m";
  stack -> SetXaxisRange(0., 200.);
  //stack -> SetYaxisRange(0.02, 20.);
  stack -> SetXaxisTitle("m_{Wjj}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(histoNames, method, lumi, step, 5., false);

  histoNames.at(0) = "WJJ_et";
  stack -> SetXaxisRange(0., 500.);
  stack -> SetXaxisTitle("et_{Wjj}");
  stack -> SetUnit("GeV");
  stack -> Draw(histoNames, method, lumi, step, 10., false);  

  histoNames.at(0) = "WJJ_max_et";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("et_{Wjj} max");
  stack -> SetUnit("GeV");
  stack -> Draw(histoNames, method, lumi, step, 5., false);

  histoNames.at(0) = "WJJ_min_et";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("et_{Wjj} min");
  stack -> SetUnit("GeV");
  stack -> Draw(histoNames, method, lumi, step, 5., false);
    
  histoNames.at(0) = "WJJ_Deta";
  stack -> SetXaxisRange(0., 2.);
  //stack -> SetYaxisRange(0.002, 50.);
  stack -> SetXaxisTitle("#Delta#eta_{Wjj}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 5., false);

  histoNames.at(0) = "WJJ_etRatio";
  stack -> SetXaxisRange(0., 1.);
  //stack -> SetYaxisRange(0.002, 50.);
  stack -> SetXaxisTitle("etRatio_{Wjj}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 20., false);  

  histoNames.at(0) = "WJJ_et1OverEt2";
  stack -> SetXaxisRange(-5., 5.);
  //stack -> SetYaxisRange(0.002, 50.);
  stack -> SetXaxisTitle("et1/et2_{Wjj}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 10., false);
  
  histoNames.at(0) = "WJJ_chargeSum";
  stack -> SetXaxisRange(-5., 5.);
  //stack -> SetYaxisRange(0.002, 50.);
  stack -> SetXaxisTitle("chargeSum_{Wjj}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 10., false);
  
  
  
  
  
  
  histoNames.at(0) = "lepWJJ_pt1";
  stack -> SetXaxisRange(0., 300.);
  //stack -> SetYaxisRange(0.002, 50.);
  stack -> SetXaxisTitle("pt1_{lepWjj}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 5., false);
  
  histoNames.at(0) = "lepWJJ_pt2";
  stack -> SetXaxisRange(0., 200.);
  //stack -> SetYaxisRange(0.002, 50.);
  stack -> SetXaxisTitle("pt2_{lepWjj}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 5., false);
  
  histoNames.at(0) = "lepWJJ_pt3";
  stack -> SetXaxisRange(0., 100.);
  //stack -> SetYaxisRange(0.002, 50.);
  stack -> SetXaxisTitle("pt3_{lepWjj}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 5., false);  
  
  
  
  
  histoNames.at(0) = "lepMetW_mt";
  stack -> SetXaxisRange(0., 750.);
  //stack -> SetYaxisRange(0.002, 50.);
  stack -> SetXaxisTitle("mt_{lepMet-W}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 5., false);
  
  histoNames.at(0) = "lepMetW_Deta";
  stack -> SetXaxisRange(0., 3.15);
  //stack -> SetYaxisRange(0.002, 50.);
  stack -> SetXaxisTitle("#Delta#eta_{lepMet-W}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 5., false);
  
  histoNames.at(0) = "lepMetW_Dphi";
  stack -> SetXaxisRange(0., 3.15);
  //stack -> SetYaxisRange(0.002, 50.);
  stack -> SetXaxisTitle("#Delta#phi_{lepMet-W}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 5., false);  
  
  
  
  
  
  histoNames.at(0) = "tagJJ_m";
  stack -> SetXaxisRange(0., 2000.);
  //stack -> SetYaxisRange(0.02, 20.);
  stack -> SetXaxisTitle("m_{tagJJ}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(histoNames, method, lumi, step, 20., true);

  histoNames.at(0) = "tagJJ_et";
  stack -> SetXaxisRange(0., 500.);
  stack -> SetXaxisTitle("et_{tagJJ}");
  stack -> SetUnit("GeV");
  stack -> Draw(histoNames, method, lumi, step, 20., false);  
  
  histoNames.at(0) = "tagJJ_Deta";
  stack -> SetXaxisRange(2., 10.);
  //stack -> SetYaxisRange(0.002, 50.);
  stack -> SetXaxisTitle("#Delta#eta_{tagJJ}");
  stack -> SetUnit("");
  stack -> Draw(histoNames, method, lumi, step, 5., false);
  
  histoNames.at(0) = "tagJJ_max_et";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("et_{tagJJ} max");
  stack -> SetUnit("GeV");
  stack -> Draw(histoNames, method, lumi, step, 5., false);

  histoNames.at(0) = "tagJJ_min_et";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("et_{tagJJ} min");
  stack -> SetUnit("GeV");
  stack -> Draw(histoNames, method, lumi, step, 5., false);
}





int RunCommand(const char *strCommand)
{
	int iForkId, iStatus;
	iForkId = vfork();
	if (iForkId == 0)	// This is the child 
	{
		iStatus = execl("/bin/sh","sh","-c", strCommand, (char*) NULL);
		exit(iStatus);	// We must exit here, 
				// or we will have multiple
				// mainlines running...  
	}
	else if (iForkId > 0)	// Parent, no error
	{
		iStatus = 0;
	}
	else	// Parent, with error (iForkId == -1)
	{
		iStatus = -1;
	}
	return(iStatus);
} 

