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

  std::string baseDir =  gConfigParser -> readStringOption("Input::baseDir");  
  std::string inputDir = gConfigParser -> readStringOption("Input::inputDir");
  std::string samplesListFile = gConfigParser -> readStringOption("Input::samplesListFile");
  std::string inputFileName = gConfigParser -> readStringOption("Input::inputFileName");
  
  
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
  
  stack -> SetXLegend(0.76, 0.99);
  stack -> SetYLegend(0.60, 0.85);
  
  stack -> DrawEvents("events", lumi, step, true);
  stack -> DrawEvents("eventsScaled", lumi, step, true);
  stack -> DrawEvents("eventsScaledStack", lumi, step, true);
  stack -> DrawEvents("efficiencies", lumi, step, true);
  stack -> DrawEvents("efficienciesRelative", lumi, step, true);
  //stack -> DrawEventRatio_nJets("int", lumi, step, false);
  //stack -> DrawEventRatio_nJets("0jets", lumi, step, false);
  //stack -> DrawEventRatio_nJets("1jets", lumi, step, false);
  //stack -> DrawEventRatio_nJets("2jets", lumi, step, false);
    
  
  
  

  std::vector<std::string> variableNames;
  variableNames.push_back("");
  std::vector<std::string> variableNames2;
  variableNames2.push_back("");
  variableNames2.push_back("");
  
  std::vector<std::string>* cuts = new std::vector<std::string>;
  cuts->push_back("");
  std::vector<std::string>* cuts2 = new std::vector<std::string>;
  cuts2->push_back("");  
  cuts2->push_back("");
  
  std::string histoName;
  
  
  
  
  
  //---------------------------------------------  
  // MVA
  //---------------------------------------------
  
  //histoName = "mva";
  //stack -> SetXaxisRange(0., 1.);
  //stack -> SetXaxisTitle("mva");
  //stack -> SetUnit("");
  //<stack -> Draw(variableNames, histoName, method, lumi, step, 2., true);
  
  
  
  
  
  
  //---------------------------------------------  
  // PV
  //---------------------------------------------

  variableNames.at(0) = "PV_d0";  
  histoName    = "PV_d0";
  stack -> SetXaxisRange(0., 0.2);
  stack -> SetXaxisTitle("d0_{PV}");
  stack -> SetUnit("cm");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false);
  
  variableNames.at(0) = "PV_z";
  histoName    = "PV_z";
  stack -> SetXaxisRange(-30., 30.);
  stack -> SetXaxisTitle("z_{PV}");
  stack -> SetUnit("cm");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false);
  
  variableNames.at(0) = "PV_nTracks";
  histoName    = "PV_nTracks";
  cuts->at(0) = "WJ1.Et() > 0. && WJ2.Et() > 0. && tagJ1.Et() > 0. && tagJ2.Et() > 0.";
  stack -> SetXaxisRange(0., 150);
  stack -> SetXaxisTitle("nTracks_{PV}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, false, cuts);
  
  variableNames.at(0) = "PV_ndof";
  histoName    = "PV_ndof";
  stack -> SetXaxisRange(0., 250);
  stack -> SetXaxisTitle("ndof_{PV}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 125, false);
  
  variableNames.at(0) = "PV_normalizedChi2";
  histoName    = "PV_normalizedChi2";
  stack -> SetXaxisRange(0., 2.);
  stack -> SetXaxisTitle("#chi^{2}/ndof_{PV}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // lepton
  //---------------------------------------------
  
  variableNames.at(0) = "lep.pt()";
  histoName    = "lep_pt";
  stack -> SetXaxisRange(0., 120.);
  stack -> SetXaxisTitle("pt_{lep}");
  stack -> SetUnit("GeV/c");
  stack -> Draw(variableNames, histoName, method, lumi, step, 120, false);
  
  histoName = "plus_lep_pt";
  cuts->at(0) = "lep_charge > 0";
  stack -> SetXaxisRange(0., 120.);
  stack -> SetXaxisTitle("pt_{lep} - e^{+}");
  stack -> SetUnit("GeV/c");
  stack -> Draw(variableNames, histoName, method, lumi, step, 120, false, cuts);
  
  histoName = "minus_lep_pt";
  cuts->at(0) = "lep_charge < 0";
  stack -> SetXaxisRange(0., 120.);
  stack -> SetXaxisTitle("pt_{lep} - e^{-}");
  stack -> SetUnit("GeV/c");
  stack -> Draw(variableNames, histoName, method, lumi, step, 120, false, cuts);
  
  
  
  variableNames.at(0) = "lep.phi()";
  histoName = "lep_phi";
  stack -> SetXaxisRange(-3.15, 3.15);
  stack -> SetXaxisTitle("#phi_{lep}");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi,step, 50, false);

  histoName = "plus_lep_phi";
  cuts->at(0) = "lep_charge > 0";
  stack -> SetXaxisRange(-3.15, 3.15);
  stack -> SetXaxisTitle("#phi_{lep} - e^{+}");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi,step, 50, false, cuts);
  
  histoName = "minus_lep_phi";
  cuts->at(0) = "lep_charge < 0";
  stack -> SetXaxisRange(-3.15, 3.15);
  stack -> SetXaxisTitle("#phi_{lep} - e^{-}");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi,step, 50, false, cuts);
  
  

  variableNames.at(0) = "lep.eta()";  
  histoName = "lep_eta";
  stack -> SetXaxisRange(-2.5, 2.5);
  stack -> SetXaxisTitle("#eta_{lep}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi,step, 50, false);
  
  histoName = "plus_lep_eta";
  cuts->at(0) = "lep_charge > 0";
  stack -> SetXaxisRange(-2.5, 2.5);
  stack -> SetXaxisTitle("#eta_{lep} - e^{+}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi,step, 50, false, cuts);
  
  histoName = "minus_lep_eta";
  cuts->at(0) = "lep_charge < 0";
  stack -> SetXaxisRange(-2.5, 2.5);
  stack -> SetXaxisTitle("#eta_{lep} - e^{-}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi,step, 50, false, cuts);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // Met
  //---------------------------------------------
  
  variableNames.at(0) = "met.Et()";  
  histoName = "met_et";
  stack -> SetXaxisRange(0., 120.);
  stack -> SetXaxisTitle("et_{met}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi, step, 120, false);

  histoName = "plus_met_et";
  cuts->at(0) = "lep_charge > 0";  
  stack -> SetXaxisRange(0., 120.);
  stack -> SetXaxisTitle("et_{met} - e^{+}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi, step, 120, false, cuts);
  
  histoName = "minus_met_et";
  cuts->at(0) = "lep_charge < 0";
  stack -> SetXaxisRange(0., 120.);
  stack -> SetXaxisTitle("et_{met} - e^{-}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi, step, 120, false, cuts);  
  
  
  
  variableNames.at(0) = "lepMet_mt";  
  histoName = "lepMet_mt";
  stack -> SetXaxisRange(0., 150.);
  stack -> SetXaxisTitle("mt_{lep+met}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumi, step, 150, false);
  
  histoName = "plus_lepMet_mt";
  cuts->at(0) = "lep_charge > 0";
  stack -> SetXaxisRange(0., 150.);
  stack -> SetXaxisTitle("mt_{lep+met} - e^{+}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumi, step, 150, false, cuts);
  
  histoName = "minus_lepMet_mt";
  cuts->at(0) = "lep_charge < 0";
  stack -> SetXaxisRange(2., 150.);
  stack -> SetXaxisTitle("mt_{lep+met} - e^{-}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumi, step, 150, false, cuts);
  
  
  
  variableNames.at(0) = "lepMet_Dphi";
  histoName = "lepMet_Dphi";
  stack -> SetXaxisRange(0., 3.15);
  stack -> SetXaxisTitle("#Delta#phi_{lep+met}");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false);
  
  histoName = "plus_lepMet_Dphi";
  cuts->at(0) = "lep_charge > 0";
  stack -> SetXaxisRange(0., 3.15);
  stack -> SetXaxisTitle("#Delta#phi_{lep+met} - e^{+}");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  histoName = "minus_lepMet_Dphi";
  cuts->at(0) = "lep_charge < 0";  
  stack -> SetXaxisRange(0., 3.15);
  stack -> SetXaxisTitle("#Delta#phi_{lep+met} - e^{-}");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // electron
  //---------------------------------------------
  
  variableNames.at(0) = "lep_sigmaIetaIeta";
  histoName = "ele_sigmaIetaIeta_EB";
  cuts->at(0) = "lep_isEB == 1 && lep_flavour == 11";
  stack -> SetXaxisRange(0., 0.030);
  stack -> SetXaxisTitle("EB #sigma_{I#etaI#eta}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 150, false, cuts);
  
  histoName = "ele_sigmaIetaIeta_EE";
  cuts->at(0) = "lep_isEB != 1 && lep_flavour == 11";
  stack -> SetXaxisRange(0., 0.030);
  stack -> SetXaxisTitle("EE #sigma_{I#etaI#eta}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 150, false, cuts);
  
  

  variableNames.at(0) = "lep_DphiIn";
  histoName = "ele_DphiIn_EB";
  cuts->at(0) = "lep_isEB == 1 && lep_flavour == 11";
  stack -> SetXaxisRange(-0.040, 0.040);
  stack -> SetXaxisTitle("EB #Delta#phi_{in}");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  histoName = "ele_DphiIn_EE";
  cuts->at(0) = "lep_isEB != 1 && lep_flavour == 11";
  stack -> SetXaxisRange(-0.040, 0.040);
  stack -> SetXaxisTitle("EE #Delta#phi_{in}");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  
  
  variableNames.at(0) = "lep_DetaIn";
  histoName = "ele_DetaIn_EB";
  cuts->at(0) = "lep_isEB == 1 && lep_flavour == 11";
  stack -> SetXaxisRange(-0.010, 0.010);
  stack -> SetXaxisTitle("EB #Delta#eta_{in}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  histoName = "ele_DetaIn_EE";
  cuts->at(0) = "lep_isEB != 1 && lep_flavour == 11";
  stack -> SetXaxisRange(-0.010, 0.010);
  stack -> SetXaxisTitle("EE #Delta#eta_{in}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  
  
  variableNames.at(0) = "lep_HOverE";
  histoName = "ele_HOverE_EB";
  cuts->at(0) = "lep_isEB == 1 && lep_flavour == 11";
  stack -> SetXaxisRange(0., 0.050);
  stack -> SetXaxisTitle("EB H/E");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  histoName = "ele_HOverE_EE";
  cuts->at(0) = "lep_isEB != 1 && lep_flavour == 11";
  stack -> SetXaxisRange(0., 0.050);
  stack -> SetXaxisTitle("EE H/E");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  
  
  
  
  
  variableNames.at(0) = "lep_dxy";
  histoName = "ele_dxy_EB";
  cuts->at(0) = "lep_isEB == 1 && lep_flavour == 11";
  stack -> SetXaxisRange(-0.03, 0.03);
  stack -> SetXaxisTitle("EB d_{xy}");
  stack -> SetUnit("cm");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  histoName = "ele_dxy_EE";
  cuts->at(0) = "lep_isEB != 1 && lep_flavour == 11";
  stack -> SetXaxisRange(-0.03, 0.03);
  stack -> SetXaxisTitle("EE d_{xy}");
  stack -> SetUnit("cm");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  
  
  variableNames.at(0) = "lep_dz";
  histoName = "ele_dz_EB";
  cuts->at(0) = "lep_isEB == 1 && lep_flavour == 11";
  stack -> SetXaxisRange(-0.03, 0.03);
  stack -> SetXaxisTitle("EB d_{z}");
  stack -> SetUnit("cm");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  histoName = "ele_dz_EE";
  cuts->at(0) = "lep_isEB != 1 && lep_flavour == 11";
  stack -> SetXaxisRange(-0.03, 0.03);
  stack -> SetXaxisTitle("EE d_{z}");
  stack -> SetUnit("cm");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  
  variableNames.at(0) = "abs(lep_3DipSig)";
  histoName = "ele_3DipSig_EB";
  cuts->at(0) = "lep_isEB == 1 && lep_flavour == 11";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("EB 3D ip");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  histoName = "ele_3DipSig_EE";
  cuts->at(0) = "lep_isEB != 1 && lep_flavour == 11";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("EE 3D ip");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  
  
  variableNames.at(0) = "lep_fbrem";
  cuts->at(0) = "lep_isEB == 1 && lep_flavour == 11";
  histoName = "ele_fbrem_EB";
  stack -> SetXaxisRange(-0.1, 1.);
  stack -> SetXaxisTitle("EB f_{brem}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  histoName = "ele_fbrem_EE";
  cuts->at(0) = "lep_isEB != 1 && lep_flavour == 11";
  stack -> SetXaxisRange(-0.1, 1.);
  stack -> SetXaxisTitle("EE f_{brem}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  

  variableNames.at(0) = "lep_EOverP";
  histoName = "ele_EOverP_EB";
  cuts->at(0) = "lep_isEB == 1 && lep_flavour == 11";
  stack -> SetXaxisRange(0., 3.);
  stack -> SetXaxisTitle("EB E/p");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  histoName = "ele_EOverP_EE";
  cuts->at(0) = "lep_isEB != 1 && lep_flavour == 11";
  stack -> SetXaxisRange(0., 3.);
  stack -> SetXaxisTitle("EE E/p");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  
  
  
  
  
  
  variableNames.at(0) = "lep_tkIso/lep.pt()";
  histoName = "ele_tkRelIso_EB";
  cuts->at(0) = "lep_isEB == 1 && lep_flavour == 11";
  stack -> SetXaxisRange(0., 0.5);
  stack -> SetXaxisTitle("EB #Sigma pt_{tracks} / pt_{ele}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  histoName = "ele_tkRelIso_EE";
  cuts->at(0) = "lep_isEB != 1 && lep_flavour == 11";  
  stack -> SetXaxisRange(0., 0.5);
  stack -> SetXaxisTitle("EE #Sigma pt_{tracks} / pt_{ele}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  
  
  variableNames.at(0) = "lep_emIso/lep.pt()";
  histoName = "ele_emRelIso_EB";
  cuts->at(0) = "lep_isEB == 1 && lep_flavour == 11";
  stack -> SetXaxisRange(0., 0.5);
  stack -> SetXaxisTitle("EB #Sigma et_{ECAL} / pt_{ele}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  histoName = "ele_emRelIso_EE";
  cuts->at(0) = "lep_isEB != 1 && lep_flavour == 11";
  stack -> SetXaxisRange(0., 0.5);
  stack -> SetXaxisTitle("EE #Sigma et_{ECAL} / pt_{ele}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  

  variableNames.at(0) = "lep_hadIso/lep.pt()";
  histoName = "ele_hadRelIso_EB";
  cuts->at(0) = "lep_isEB == 1 && lep_flavour == 11";
  stack -> SetXaxisRange(0., 0.5);
  stack -> SetXaxisTitle("EB #Sigma et_{HCAL} / pt_{ele}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  histoName = "ele_hadRelIso_EE";
  cuts->at(0) = "lep_isEB != 1 && lep_flavour == 11";
  stack -> SetXaxisRange(0., 0.5);
  stack -> SetXaxisTitle("EE #Sigma et_{HCAL} / pt_{ele}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  
  
  variableNames.at(0) = "(lep_tkIso+lep_emIso+lep_hadIso)/lep.pt()";
  histoName = "ele_combRelIso_EB";
  cuts->at(0) = "lep_isEB == 1 && lep_flavour == 11";
  stack -> SetXaxisRange(0., 1.0);
  //stack -> SetYaxisRange(20., 2000);
  stack -> SetXaxisTitle("EB #Sigma (pt_{trakcs}+et_{ECAL}+et_{HCAL}) / pt_{ele}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  histoName = "ele_combRelIso_EE";
  cuts->at(0) = "lep_isEB != 1 && lep_flavour == 11";
  stack -> SetXaxisRange(0., 0.5);
  stack -> SetXaxisTitle("EE #Sigma (pt_{tracks}+et_{ECAL}+et_{HCAL}) / pt_{ele}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // muon
  //---------------------------------------------

  variableNames.at(0) = "lep_tkIso/lep.pt()";
  histoName = "mu_tkRelIso";
  cuts->at(0) = "lep_flavour == 13";
  stack -> SetXaxisRange(0., 1.);
  stack -> SetXaxisTitle("#Sigma pt_{tracks} / pt_{mu}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);

  variableNames.at(0) = "lep_emIso/lep.pt()";
  histoName = "mu_emRelIso";
  cuts->at(0) = "lep_flavour == 13";
  stack -> SetXaxisRange(0., 1.);
  stack -> SetXaxisTitle("#Sigma et_{ECAL} / pt_{mu}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);

  variableNames.at(0) = "lep_hadIso/lep.pt()";
  histoName = "mu_hadRelIso";
  cuts->at(0) = "lep_flavour == 13";
  stack -> SetXaxisRange(0., 1.);
  stack -> SetXaxisTitle("#Sigma et_{HCAL} / pt_{mu}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  variableNames.at(0) = "(lep_tkIso+lep_emIso+lep_hadIso)/lep.pt()";
  histoName = "mu_combRelIso";
  cuts->at(0) = "lep_flavour == 13";
  stack -> SetXaxisRange(0., 1.);
  //stack -> SetYaxisRange(20., 2000);
  stack -> SetXaxisTitle("#Sigma (pt_{tracks}+et_{ECAL}+et_{HCAL}) / pt_{mu}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
  
  

//  
//  
//  
//  histoName = "mu_dxy";
//  stack -> SetXaxisRange(-0.03, 0.03);
//  //stack -> SetYaxisRange(0.0001, 1.);
//  stack -> SetXaxisTitle("d_{xy}");
//  stack -> SetUnit("cm");
//  stack -> Draw(variableNames, histoName, method, lumi, step, 1., true);
//
//  histoName = "mu_dz";
//  stack -> SetXaxisRange(-0.03, 0.03);
//  //stack -> SetYaxisRange(0.0001, 1.);
//  stack -> SetXaxisTitle("d_{z}");
//  stack -> SetUnit("cm");
//  stack -> Draw(variableNames, histoName, method, lumi, step, 1., true);  
//  
  variableNames.at(0) = "abs(lep_3DipSig)";
  histoName = "mu_3DipSig";
  cuts->at(0) = "lep_flavour == 13";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("3D ip");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
//  histoName = "mu_3DipSig";
//  stack -> SetXaxisRange(-5., 5.);
//  //stack -> SetYaxisRange(0.00001, 1.);
//  stack -> SetXaxisTitle("3D ip");
//  stack -> SetUnit("");
//  stack -> Draw(variableNames, histoName, method, lumi, step, 2., true);
//  
//  
//  
//  histoName = "mu_normalizedChi2";
//  stack -> SetXaxisRange(0., 10.);
//  //stack -> SetYaxisRange(0.00001, 1.);
//  stack -> SetXaxisTitle("#chi^{2}/ndof");
//  stack -> SetUnit("");
//  stack -> Draw(variableNames, histoName, method, lumi, step, 10., true);  
//
//  histoName = "mu_numberOfValidTrackerHits";
//  stack -> SetXaxisRange(0., 40.);
//  //stack -> SetYaxisRange(0.00001, 1.);
//  stack -> SetXaxisTitle("nHits_{tracker}");
//  stack -> SetUnit("");
//  stack -> Draw(variableNames, histoName, method, lumi, step, 1., false);  
//
//  histoName = "mu_numberOfValidMuonHits";
//  stack -> SetXaxisRange(0., 60.);
//  //stack -> SetYaxisRange(0.00001, 1.);
//  stack -> SetXaxisTitle("nHits_{muon}");
//  stack -> SetUnit("");
//  stack -> Draw(variableNames, histoName, method, lumi, step, 1., false);    
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // Jets
  //---------------------------------------------

  variableNames.at(0) = "sqrt( (leadingJ.eta()-lep.eta())*(leadingJ.eta()-lep.eta()) + (leadingJ.phi()-lep.phi())*(leadingJ.phi()-lep.phi()) )";
  histoName = "leadingJLep_DR";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("DR_{leading jet-lep}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false);
  
  variableNames.at(0) = "leadingJ.Et()";
  histoName = "leadingJ_et";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("et_{leading jet}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false);
  
  variableNames.at(0) = "leadingJ.eta()";
  histoName = "leadingJ_eta";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta_{leading jet}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi,step, 100, false);
  
  variableNames.at(0) = "leadingJ_bTag";
  histoName = "leadingJ_bTag";
  stack -> SetXaxisRange(-5., 10.);
  stack -> SetXaxisTitle("bTag_{leading jet}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi,step, 75, false);
  
  
  
  variableNames.at(0) = "nJets";
  histoName = "jets_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("n_{jets}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 10, false);

  variableNames.at(0) = "nJets_cnt";
  histoName = "jets_cnt_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("n_{central jets}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 10, false);

  variableNames.at(0) = "nJets_fwd";
  histoName = "jets_fwd_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("n_{forward jets}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 10, false);

  variableNames.at(0) = "nJets_btw";
  histoName = "jets_btw_n";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("n_{jets between}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 5, false);
  
  

  variableNames.at(0) = "jets_bTag1";
  histoName = "jets_bTag1";
  stack -> SetXaxisRange(-5., 10.);
  stack -> SetXaxisTitle("bTag1_{jets}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi,step, 150, false);
  
  variableNames.at(0) = "jets_bTag2";
  histoName = "jets_bTag2";
  stack -> SetXaxisRange(-5., 10.);
  stack -> SetXaxisTitle("bTag2_{jets}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi,step, 150, false);  
  
  
  
  variableNames.at(0) = "HT";
  histoName = "HT";
  stack -> SetXaxisRange(0., 600.);
  stack -> SetXaxisTitle("HT");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi,step, 200, false);
  
  variableNames.at(0) = "HT_cnt";
  histoName = "HT_cnt";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("HT_cnt");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi,step, 100, false);
  
  variableNames.at(0) = "HT_fwd";
  histoName = "HT_fwd";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("HT_fwd");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi,step, 100, false);
  
  
  
  
  
  
  
  
  
  
  
  variableNames.at(0) = "WJ12.mass()";
  histoName = "WJJ_m";
  cuts->at(0) = "WJ1.Et() > 0. && WJ2.Et() > 0. && tagJ1.Et() > 0 && tagJ2.Et() > 0";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("m_{Wjj}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumi, step, 75, false, cuts);

  variableNames.at(0) = "WJ12.Et()";
  histoName = "WJJ_et";
  cuts->at(0) = "WJ1.Et() > 0. && WJ2.Et() > 0.";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("et_{Wjj}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi, step, 150, false, cuts);  
  
  variableNames.at(0) = "abs(WJ1.eta() - WJ2.eta())";
  histoName = "WJJ_Deta";
  cuts->at(0) = "WJ1.Et() > 0. && WJ2.Et() > 0.";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("#Delta#eta_{Wjj}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, false, cuts);

  variableNames2.at(0) = "WJ1.phi() - WJ2.phi()";
  variableNames2.at(1) = "2*3.14159 - (WJ1.phi() - WJ2.phi())";
  histoName = "WJJ_Dphi";
  cuts2->at(0) = "WJ1.Et() > 0. && WJ2.Et() > 0. && (WJ1.phi() - WJ2.phi()) < 3.14159";  
  cuts2->at(1) = "WJ1.Et() > 0. && WJ2.Et() > 0. && (WJ1.phi() - WJ2.phi()) > 3.14159";
  stack -> SetXaxisRange(0., 3.15);
  stack -> SetXaxisTitle("#Delta#phi_{Wjj}");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames2, histoName, method, lumi, step, 100, false, cuts2);

  variableNames.at(0) = "sqrt( (WJ1.eta() - WJ2.eta())*(WJ1.eta() - WJ2.eta()) + (WJ1.phi() - WJ2.phi())*(WJ1.phi() - WJ2.phi()) )";
  histoName = "WJJ_DR";
  cuts->at(0) = "WJ1.Et() > 0. && WJ2.Et() > 0.";  
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("#DeltaR_{Wjj}");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);  
  
  
  
  variableNames.at(0) = "max(WJ1.Et(), WJ2.Et())";
  histoName = "WJJ_max_et";
  cuts->at(0) = "WJ1.Et() > 0. && WJ2.Et() > 0.";  
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("et_{Wjj} max");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi, step, 150, false, cuts);
  
  variableNames.at(0) = "min(WJ1.Et(), WJ2.Et())";
  histoName = "WJJ_min_et";
  cuts->at(0) = "WJ1.Et() > 0. && WJ2.Et() > 0.";
  stack -> SetXaxisRange(0., 150.);
  stack -> SetXaxisTitle("et_{Wjj} min");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi, step, 75, false, cuts);
    
  variableNames2.at(0) = "WJ1.eta()";
  variableNames2.at(1) = "WJ1.eta()";
  histoName = "WJJ_min_eta + WJJ_max_eta";
  cuts2->at(0) = "WJ1.Et() > 0. && WJ2.Et() > 0.";
  cuts2->at(1) = "WJ1.Et() > 0. && WJ2.Et() > 0.";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("eta_{Wjj}");
  stack -> SetUnit("");
  stack -> Draw(variableNames2, histoName, method, lumi, step, 100, false, cuts2);
  
  variableNames.at(0) = "max(abs(WJ1.eta()), abs(WJ2.eta()))";
  histoName = "WJJ_max_absEta";
  cuts->at(0) = "WJ1.Et() > 0. && WJ2.Et() > 0.";  
  stack -> SetXaxisRange(0., 2.5);
  stack -> SetXaxisTitle("eta_{Wjj} max");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 25, false, cuts);
  
  variableNames.at(0) = "min(abs(WJ1.eta()), abs(WJ2.eta()))";
  histoName = "WJJ_min_absEta";
  cuts->at(0) = "WJ1.Et() > 0. && WJ2.Et() > 0.";  
  stack -> SetXaxisRange(0., 2.5);
  stack -> SetXaxisTitle("eta_{Wjj} min");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 25, false, cuts);  
//  histoName = "WJJ_etRatio";
//  stack -> SetXaxisRange(0., 1.);
//  //stack -> SetYaxisRange(0.002, 50.);
//  stack -> SetXaxisTitle("etRatio_{Wjj}");
//  stack -> SetUnit("");
//  stack -> Draw(variableNames, histoName, method, lumi, step, 20, false);  
//
//  histoName = "WJJ_et1OverEt2";
//  stack -> SetXaxisRange(-5., 5.);
//  //stack -> SetYaxisRange(0.002, 50.);
//  stack -> SetXaxisTitle("et1/et2_{Wjj}");
//  stack -> SetUnit("");
//  stack -> Draw(variableNames, histoName, method, lumi, step, 20, false);
//  
//  histoName = "WJJ_chargeSum";
//  stack -> SetXaxisRange(-5., 5.);
//  //stack -> SetYaxisRange(0.002, 50.);
//  stack -> SetXaxisTitle("chargeSum_{Wjj}");
//  stack -> SetUnit("");
//  stack -> Draw(variableNames, histoName, method, lumi, step, 20, false);
  variableNames.at(0) = "WJ1_bTag+WJ2_bTag";
  histoName = "WJ1_bTag+WJ2_bTag";
  cuts->at(0) = "WJ1.Et() > 0. && WJ2.Et() > 0.";
  stack -> SetXaxisRange(-5., 10.);
  stack -> SetXaxisTitle("bTag_{WJJ}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 150, false, cuts);
  
  
  
  
  
  
  variableNames.at(0) = "lepWJJ_pt1";
  histoName = "lepWJJ_pt1";
  cuts->at(0) = "WJ1.Et() > 0. && WJ2.Et() > 0.";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("pt1_{lepWJJ}");
  stack -> SetUnit("GeV/c");
  stack -> Draw(variableNames, histoName, method, lumi, step, 150, false, cuts);

  variableNames.at(0) = "lepWJJ_pt2";  
  histoName = "lepWJJ_pt2";
  cuts->at(0) = "WJ1.Et() > 0. && WJ2.Et() > 0.";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("pt2_{lepWJJ}");
  stack -> SetUnit("GeV/c");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);

  variableNames.at(0) = "lepWJJ_pt3";
  histoName = "lepWJJ_pt3";
  cuts->at(0) = "WJ1.Et() > 0. && WJ2.Et() > 0.";
  stack -> SetXaxisRange(0., 100.);
  stack -> SetXaxisTitle("pt3_{lepWJJ}");
  stack -> SetUnit("GeV/c");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, false, cuts);
  
  
  
  
  
  
  variableNames.at(0) = "lepNuW_m";
  histoName = "lepNuW_m";
  cuts->at(0) = "WJ1.Et() > 0. && WJ2.Et() > 0."; 
  stack -> SetXaxisRange(100., 600);
  stack -> SetXaxisTitle("m_{lepNu-W}");
  stack -> SetUnit("GeV/c2");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, false, cuts);
//  
//  histoName = "lepMetW_pt";
//  stack -> SetXaxisRange(0., 300);
//  //stack -> SetYaxisRange(0.002, 50.);
//  stack -> SetXaxisTitle("pt_{lepMetW}");
//  stack -> SetUnit("GeV/c");
//  stack -> Draw(variableNames, histoName, method, lumi, step, 20, false);
//  
//  histoName = "lepMetW_mt";
//  stack -> SetXaxisRange(0., 700);
//  //stack -> SetYaxisRange(0.002, 50.);
//  stack -> SetXaxisTitle("mt_{lepMetW}");
//  stack -> SetUnit("GeV/c2");
//  stack -> Draw(variableNames, histoName, method, lumi, step, 20, false);
//  
//  histoName = "lepMetW_Deta";
//  stack -> SetXaxisRange(0., 5.);
//  //stack -> SetYaxisRange(0.002, 50.);
//  stack -> SetXaxisTitle("#Delta#eta_{lepMet-W}");
//  stack -> SetUnit("");
//  stack -> Draw(variableNames, histoName, method, lumi, step, 4, false);
//  
  variableNames.at(0) = "lepMetW_Dphi";
  histoName = "lepMetW_Dphi";
  cuts->at(0) = "WJ1.Et() > 0. && WJ2.Et() > 0. && tagJ1.Et() > 0. && tagJ2.Et() > 0.";  
  stack -> SetXaxisRange(0., 3.15);
  stack -> SetXaxisTitle("#Delta#phi_{lepMet-W}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, false, cuts);
//  
//  
//  
//  
//  
  variableNames.at(0) = "sqrt( (tagJ1.E()+tagJ2.E())*(tagJ1.E()+tagJ2.E()) - (tagJ1.Px()+tagJ2.Px())*(tagJ1.Px()+tagJ2.Px()) - (tagJ1.Py()+tagJ2.Py())*(tagJ1.Py()+tagJ2.Py()) - (tagJ1.Pz()+tagJ2.Pz())*(tagJ1.Pz()+tagJ2.Pz()) )";
  histoName = "tagJJ_m";
  cuts->at(0) = "tagJ1.Et() > 0. && tagJ2.Et() > 0.";
  stack -> SetXaxisRange(0., 2500.);
  stack -> SetYaxisRange(0.0003, 0.3);
  stack -> SetXaxisTitle("m_{tagJJ}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, false, cuts);
  
  //histoName = "tagJJ_et";
  //stack -> SetXaxisRange(0., 1000.);
  //stack -> SetXaxisTitle("et_{tagJJ}");
  //stack -> SetUnit("GeV");
  //stack -> Draw(variableNames, histoName, method, lumi, step, 20, false);  
  //
  
  variableNames2.at(0) = "tagJ1.eta()";
  variableNames2.at(1) = "tagJ1.eta()";
  histoName = "tagJJ_min_eta + tagJJ_max_eta";
  cuts2->at(0) = "tagJ1.Et() > 0. && tagJ2.Et() > 0.";
  cuts2->at(1) = "tagJ1.Et() > 0. && tagJ2.Et() > 0.";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("eta_{tagJj}");
  stack -> SetUnit("");
  stack -> Draw(variableNames2, histoName, method, lumi, step, 25, false, cuts2);
  
  //histoName = "tagJJ_Deta";
  //stack -> SetXaxisRange(0., 10.);
  ////stack -> SetYaxisRange(0.002, 50.);
  //stack -> SetXaxisTitle("#Delta#eta_{tagJJ}");
  //stack -> SetUnit("");
  //stack -> Draw(variableNames, histoName, method, lumi, step, 10, false);
  //
  //histoName = "tagJLep_Deta";
  //stack -> SetXaxisRange(0., 10.);
  ////stack -> SetYaxisRange(0.002, 50.);
  //stack -> SetXaxisTitle("#Delta#eta_{tagJ-lep}");
  //stack -> SetUnit("");
  //stack -> Draw(variableNames, histoName, method, lumi, step, 2, false);
  
  variableNames.at(0) = "abs(tagJ1.eta() - tagJ2.eta())";
  histoName = "tagJJ_Deta";
  cuts->at(0) = "tagJ1.Et() > 0. && tagJ2.Et() > 0.";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#Delta#eta_{tagJ-WJJ}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 25, false, cuts);  
  
  variableNames.at(0) = "tagJ1.eta()*tagJ2.eta()";
  histoName = "tagJJ_etaProd";
  cuts->at(0) = "tagJ1.Et() > 0. && tagJ2.Et() > 0.";
  stack -> SetXaxisRange(-25., 0.);
  stack -> SetXaxisTitle("#eta_{1}#eta_{2}^{tagJ}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, false, cuts);  
  
  variableNames.at(0) = "tagJ1.eta()*tagJ2.eta()/abs(tagJ1.eta()*tagJ2.eta())";
  histoName = "tagJJ_etaProdNorm";
  cuts->at(0) = "tagJ1.Et() > 0. && tagJ2.Et() > 0.";
  stack -> SetXaxisRange(-2, 2.);
  stack -> SetXaxisTitle("#etaProd_{tagJ-WJJ}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 4, false, cuts);
  
  variableNames.at(0) = "max(abs(tagJ1.eta()),abs(tagJ2.eta()))";
  histoName = "tagJJ_absEtaMax";
  cuts->at(0) = "tagJ1.Et() > 0. && tagJ2.Et() > 0.";
  stack -> SetXaxisRange(0, 10);
  stack -> SetXaxisTitle("max #eta_{tagJJ}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, false, cuts);
  
  variableNames.at(0) = "min(abs(tagJ1.eta()),abs(tagJ2.eta()))";
  histoName = "tagJJ_absEtaMin";
  cuts->at(0) = "tagJ1.Et() > 0. && tagJ2.Et() > 0.";
  stack -> SetXaxisRange(0, 10);
  stack -> SetXaxisTitle("min #eta_{tagJJ}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, false, cuts);
//  
//  
//  
//  histoName = "tagJJ_max_et";
//  stack -> SetXaxisRange(0., 300.);
//  stack -> SetXaxisTitle("et_{tagJJ} max");
//  stack -> SetUnit("GeV");
//  stack -> Draw(variableNames, histoName, method, lumi, step, 10, false);
//
//  histoName = "tagJJ_min_et";
//  stack -> SetXaxisRange(0., 200.);
//  stack -> SetXaxisTitle("et_{tagJJ} min");
//  stack -> SetUnit("GeV");
//  stack -> Draw(variableNames, histoName, method, lumi, step, 10, false);
//  
//  histoNames2.at(0) = "tagJJ_min_eta";
//  histoNames2.at(1) = "tagJJ_max_eta";
//  stack -> SetXaxisRange(-5., 5.);
//  stack -> SetXaxisTitle("eta_{tagJJ}");
//  stack -> SetUnit("");
//  stack -> Draw(histoName2, method, lumi, step, 10, false);
//  
//  
//  
//  
//  
//  
  
  
  
  //  histoName = "jets_etaEtaMoment";
//  stack -> SetXaxisRange(0., 0.1);
//  stack -> SetXaxisTitle("#eta#eta_{jets}");
//  stack -> SetUnit("");
//  stack -> Draw(variableNames, histoName, method, lumi,step, 50., false);
//  
//  histoName = "jets_cnt_etaEtaMoment";
//  stack -> SetXaxisRange(0., 0.1);
//  stack -> SetXaxisTitle("#eta#eta_{central jets}");
//  stack -> SetUnit("");
//  stack -> Draw(variableNames, histoName, method, lumi,step, 50., false);
//  
//  histoName = "jets_fwd_etaEtaMoment";
//  stack -> SetXaxisRange(0., 0.1);
//  stack -> SetXaxisTitle("#eta#eta_{forward jets}");
//  stack -> SetUnit("");
//  stack -> Draw(variableNames, histoName, method, lumi,step, 50., false);
//  
//  
//  
//  histoName = "jets_phiPhiMoment";
//  stack -> SetXaxisRange(0., 0.1);
//  stack -> SetXaxisTitle("#phi#phi_{jets}");
//  stack -> SetUnit("");
//  stack -> Draw(variableNames, histoName, method, lumi,step, 50., false);
//  
//  histoName = "jets_cnt_phiPhiMoment";
//  stack -> SetXaxisRange(0., 0.1);
//  stack -> SetXaxisTitle("#phi#phi_{central jets}");
//  stack -> SetUnit("");
//  stack -> Draw(variableNames, histoName, method, lumi,step, 50., false);
//  
//  histoName = "jets_fwd_phiPhiMoment";
//  stack -> SetXaxisRange(0., 0.1);
//  stack -> SetXaxisTitle("#phi#phi_{forward jets}");
//  stack -> SetUnit("");
//  stack -> Draw(variableNames, histoName, method, lumi,step, 50., false);
//  
//  
//  
//  histoName = "jets_etaPhiMoment";
//  stack -> SetXaxisRange(-0.05, 0.05);
//  stack -> SetXaxisTitle("#eta#phi_{jets}");
//  stack -> SetUnit("");
//  stack -> Draw(variableNames, histoName, method, lumi,step, 10., false);
//  
//  histoName = "jets_cnt_etaPhiMoment";
//  stack -> SetXaxisRange(-0.05, 0.05);
//  stack -> SetXaxisTitle("#eta#phi_{central jets}");
//  stack -> SetUnit("");
//  stack -> Draw(variableNames, histoName, method, lumi,step, 10., false);
//  
//  histoName = "jets_fwd_etaPhiMoment";
//  stack -> SetXaxisRange(-0.05, 0.05);
//  stack -> SetXaxisTitle("#eta#phi_{forward jets}");
//  stack -> SetUnit("");
//  stack -> Draw(variableNames, histoName, method, lumi,step, 10., false);
//    
//  
//  
//  
//  
//  
//  if(jetType == "Calo")
//  {
//    histoName = "jets_emEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("emEnergyFraction_{jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., false);
//    
//    histoName = "jets_cnt_emEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("emEnergyFraction_{central jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., false);
//    
//    histoName = "jets_fwd_emEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("emEnergyFraction_{forward jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., false);
//    
//    
//    
//    histoName = "jets_fHPD";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("fHPD_{jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., false);
//    
//    histoName = "jets_cnt_fHPD";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("fHPD_{central jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., false);
//    
//    histoName = "jets_fwd_fHPD";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("fHPD_{forward jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., false);
//    
//    
//    
//    histoName = "jets_fRBX";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("fRBX_{jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., false);
//    
//    histoName = "jets_cnt_fRBX";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("fRBX_{central jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., false);
//    
//    histoName = "jets_fwd_fRBX";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("fRBX_{forward jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., false);
//    
//    
//    
//    histoName = "jets_n90Hits";
//    stack -> SetXaxisRange(0., 100.);
//    stack -> SetXaxisTitle("n90Hits_{jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 10., false);
//    
//    histoName = "jets_cnt_n90Hits";
//    stack -> SetXaxisRange(0., 100.);
//    stack -> SetXaxisTitle("n90Hits_{central jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 10., false);
//    
//    histoName = "jets_fwd_n90Hits";
//    stack -> SetXaxisRange(0., 100.);
//    stack -> SetXaxisTitle("n90Hits_{forward jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 10., false);
//  }
//  
//  
//  
//  
//  if( jetType == "PF")
//  {
//    histoName = "jets_chargedHadronEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("chargedHadronEnergyFraction_{jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., false);
//  
//    histoName = "jets_cnt_chargedHadronEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("chargedHadronEnergyFraction_{central jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., false);
//    
//    histoName = "jets_fwd_chargedHadronEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("chargedHadronEnergyFraction_{forward jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., false);
//    
//    
//    
//    histoName = "jets_neutralHadronEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("neutralHadronEnergyFraction_{jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., false);  
//  
//    histoName = "jets_cnt_neutralHadronEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("neutralHadronEnergyFraction_{central jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., false);
//    
//    histoName = "jets_fwd_neutralHadronEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("neutralHadronEnergyFraction_{forward jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., false);  
//    
//    
//    
//    histoName = "jets_chargedEmEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("chargedEmEnergyFraction_{jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., false);  
//    
//    histoName = "jets_cnt_chargedEmEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("chargedEmEnergyFraction_{central jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., false);
//    
//    histoName = "jets_fwd_chargedEmEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("chargedEmEnergyFraction_{forward jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., false);
//    
//    
//    
//    histoName = "jets_neutralEmEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("neutralEmEnergyFraction_{jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., false);  
//    
//    histoName = "jets_cnt_neutralEmEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("neutralEmEnergyFraction_{central jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., false);
//    
//    histoName = "jets_fwd_neutralEmEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("neutralEmEnergyFraction_{forward jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., false);
//    
//    
//    
//    histoName = "jets_chargedMultiplicity";
//    stack -> SetXaxisRange(0., 40.);
//    stack -> SetXaxisTitle("chargedMultiplicity_{jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 1., false);  
//    
//    histoName = "jets_cnt_chargedMultiplicity";
//    stack -> SetXaxisRange(0., 40.);
//    stack -> SetXaxisTitle("chargedMultiplicity_{central jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 1., false);
//    
//    histoName = "jets_fwd_chargedMultiplicity";
//    stack -> SetXaxisRange(0., 40.);
//    stack -> SetXaxisTitle("chargedMultiplicity_{forward jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 1., false);
//    
//    
//    
//    histoName = "jets_neutralMultiplicity";
//    stack -> SetXaxisRange(0., 40.);
//    stack -> SetXaxisTitle("neutralMultiplicity_{jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 1., false);  
//    
//    histoName = "jets_cnt_neutralMultiplicity";
//    stack -> SetXaxisRange(0., 40.);
//    stack -> SetXaxisTitle("neutralMultiplicity_{central jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 1., false);
//    
//    histoName = "jets_fwd_neutralMultiplicity";
//    stack -> SetXaxisRange(0., 40.);
//    stack -> SetXaxisTitle("neutralMultiplicity_{forward jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 1., false);
//  }
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
  std::cout << std::endl;
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

