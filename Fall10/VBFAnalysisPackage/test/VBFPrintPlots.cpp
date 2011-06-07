#include "setTDRStyle.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "plotUtils.h"
#include "../obj/mydict.h"

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
  
  
  std::string imgFormat = gConfigParser -> readStringOption("Options::imgFormat");
  int step = gConfigParser -> readIntOption("Options::step");  
  float lumi = gConfigParser -> readFloatOption("Options::lumi");
  bool PURescale = gConfigParser -> readBoolOption("Options::PURescale");
  std::string method = gConfigParser -> readStringOption("Options::method");
  std::string jetType = gConfigParser -> readStringOption("Options::jetType");
  
  char stepDir[50];
  sprintf(stepDir, "plots_%d", step);
  char stepNumber[50];
  sprintf(stepNumber, "%d", step);
  std::string fullOutputDir = inputDir + "/plots/" + stepDir + "_" + jetType + "_" + method + "/";
  std::string fullHtmlName = inputDir + "/plots/" + stepDir + "_" + jetType + "_" + method + ".html";
  
  
  
  
  
  
  // make directory
  int iStatus = 0;
	
  std::string command;

  command = "mkdir " + fullOutputDir;
  iStatus = RunCommand(command.c_str());  

  //command = "rm " + fullOutputDir + "/*";
  //iStatus = RunCommand(command.c_str());
  
  
  
  
  
  
  
  // make html
  iStatus = 0;

  if( method == "sameAreaNoStack" )
  {
    command = "cat " + baseDir + "/cfg/plots.html | sed -e s%STEP%" + stepNumber + "%g | sed -e s%NORM%area%g | sed -e s%FORMAT%"+imgFormat+"%g | sed -e s%DIR%" + stepDir + "_" + jetType + "_" + method + "%g > " + fullHtmlName;
    iStatus = RunCommand(command.c_str());
  }
  if( method == "eventsScaled" )
  {
    command = "cat " + baseDir + "/cfg/plots.html | sed -e s%STEP%" + stepNumber + "%g | sed -e s%NORM%lumi%g | sed -e s%FORMAT%"+imgFormat+"%g | sed -e s%DIR%" + stepDir + "_" + jetType + "_" + method + "%g > " + fullHtmlName;
    iStatus = RunCommand(command.c_str());
  }  
  
  
  
  
  
  // draw plots
  drawTStack* stack = new drawTStack(inputDir, samplesListFile, inputFileName, fullOutputDir, imgFormat);
  
  stack -> SetXLegend(0.76, 0.99);
  stack -> SetYLegend(0.60, 0.85);

  stack -> DrawEvents("events", lumi, step, true);
  stack -> DrawEvents("eventsScaled", lumi, step, true);
  stack -> DrawEvents("eventsScaledStack", lumi, step, true);
  stack -> DrawEvents("efficiencies", lumi, step, true);
  stack -> SetYaxisRange(0.01, 1.1);
  stack -> DrawEvents("efficienciesRelative", lumi, step, true);
  //stack -> DrawEvents("significance", lumi, step, false);
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
  
  
  
  variableNames.at(0) = "lepNuW_cphi";
  histoName = "lepNuW_cphi";
  stack -> SetXaxisRange(-1., 1.);
  stack -> SetXaxisTitle("cos#phi(W_{1}-W_{2} decay planes}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale);
  
  variableNames.at(0) = "lepNuZ_cphi";
  histoName = "lepNuZ_cphi";
  stack -> SetXaxisRange(-1., 1.);
  stack -> SetXaxisTitle("cos#phi_{1}(W_{1} production-decay planes)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale);
  
  variableNames.at(0) = "lep_ctheta";
  histoName = "lep_ctheta";
  stack -> SetXaxisRange(-1., 1.);
  stack -> SetXaxisTitle("cos#theta_{1}(lepton-Higgs)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale);
  
  variableNames.at(0) = "WJ1_ctheta";
  histoName = "WJ1_ctheta";
  stack -> SetXaxisRange(-1., 1.);
  stack -> SetXaxisTitle("cos#theta_{2}(WJet_{1}-Higgs)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale);
  
  variableNames.at(0) = "lepNu_ctheta";
  histoName = "lepNu_ctheta";
  stack -> SetXaxisRange(-1., 1.);
  stack -> SetXaxisTitle("cos#theta*(W_{1}-W_{2})");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale);
  
  
  
  
  
  
  //---------------------------------------------  
  // MVA
  //---------------------------------------------
  
  variableNames.at(0) = "mva";  
  histoName = "mva";
  stack -> SetXaxisRange(-0.8, 0.4);
  stack -> SetXaxisTitle("mva");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 24, PURescale);
  
  
  
  
  
  
  //---------------------------------------------  
  // PV
  //---------------------------------------------
  
  variableNames.at(0) = "PV_n";  
  histoName    = "PV_n";
  stack -> SetXaxisRange(-0.5, 19.5);
  stack -> SetXaxisTitle("n(PV)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 20, PURescale);
    
  variableNames.at(0) = "PV_d0";  
  histoName    = "PV_d0";
  stack -> SetXaxisRange(0., 0.5);
  stack -> SetXaxisTitle("d0(PV)");
  stack -> SetUnit("cm");
  stack -> Draw(variableNames, histoName, method, lumi, step, 250, PURescale);
  
  variableNames.at(0) = "PV_z";
  histoName    = "PV_z";
  stack -> SetXaxisRange(-30., 30.);
  stack -> SetXaxisTitle("z(PV)");
  stack -> SetUnit("cm");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale);
  
  variableNames.at(0) = "PV_nTracks";
  histoName    = "PV_nTracks";
  stack -> SetXaxisRange(0., 150);
  stack -> SetXaxisTitle("nTracks(PV)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale);
  
  variableNames.at(0) = "PV_ndof";
  histoName    = "PV_ndof";
  stack -> SetXaxisRange(0., 250);
  stack -> SetXaxisTitle("ndof(PV)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale);
  
  variableNames.at(0) = "PV_normalizedChi2";
  histoName    = "PV_normalizedChi2";
  stack -> SetXaxisRange(0., 2.);
  stack -> SetXaxisTitle("#chi^{2}/ndof(PV)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // lepton
  //---------------------------------------------
  
  variableNames.at(0) = "lep.pt()";
  histoName = "lep_pt";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("pt(lepton)");
  stack -> SetUnit("GeV/c");
  stack -> Draw(variableNames, histoName, method, lumi, step, 60, PURescale);
  
  variableNames.at(0) = "lep.phi()";
  histoName = "lep_phi";
  stack -> SetXaxisRange(-3.14159, 3.14159);
  stack -> SetXaxisTitle("#phi(lepton)");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi,step, 36, PURescale);
  
  variableNames.at(0) = "lep.eta()";
  histoName = "lep_eta";
  stack -> SetXaxisRange(-2.5, 2.5);
  stack -> SetXaxisTitle("#eta(lepton)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi,step, 50, PURescale);
  
  variableNames.at(0) = "abs(lep.eta())";
  histoName = "lep_absEta";
  stack -> SetXaxisRange(0., 2.5);
  stack -> SetXaxisTitle("|#eta|(lepton)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi,step, 25, PURescale);
  
  variableNames.at(0) = "abs(lep_zepp)";
  histoName = "lep_zepp";
  cuts -> at(0) = "( tagJ1.Et() > 0. && tagJ2.Et() > 0. )";
  stack -> SetXaxisRange(0., 1.);
  stack -> SetXaxisTitle("zepp(lepton)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi,step, 20, PURescale, cuts);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // Met
  //---------------------------------------------
  
  variableNames.at(0) = "met.Et()";  
  histoName = "met_et";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("met");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi, step, 60, PURescale);
  
  
  
  variableNames.at(0) = "lepMet_mt";
  histoName = "lepMet_mt";
  stack -> SetXaxisRange(0., 150.);
  stack -> SetXaxisTitle("mt(lepton+met)");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale);
  
  variableNames.at(0) = "lepMet_Dphi";
  histoName = "lepMet_Dphi";
  stack -> SetXaxisRange(0., 3.14159);
  stack -> SetXaxisTitle("#Delta#phi(lepton-met)");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi, step, 36, PURescale);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // electron
  //---------------------------------------------
  
  variableNames.at(0) = "lep_pt";
  histoName = "ele_pt";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("pt(electron)");
  stack -> SetUnit("GeV/c");
  stack -> Draw(variableNames, histoName, method, lumi, step, 60, PURescale, cuts);
  
  variableNames.at(0) = "lep.eta()";
  histoName = "ele_eta";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(-2.5, 2.5);
  stack -> SetXaxisTitle("#eta(electron)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi,step, 50, PURescale, cuts);
  
  variableNames.at(0) = "lep.phi()";
  histoName = "ele_phi";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(-2.5, 2.5);
  stack -> SetXaxisTitle("#eta(electron)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi,step, 50, PURescale, cuts);
  
  /*
  variableNames.at(0) = "lep_sigmaIetaIeta";
  histoName = "ele_sigmaIetaIeta_EB";
  cuts -> at(0) = "( lep_isEB == 1 && lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 0.030);
  stack -> SetXaxisTitle("EB #sigma_{I#etaI#eta}(electron)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, cuts);
  
  variableNames.at(0) = "lep_sigmaIetaIeta";
  histoName = "ele_sigmaIetaIeta_EE";
  cuts -> at(0) = "( lep_isEB == 0 && lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 0.030);
  stack -> SetXaxisTitle("EE #sigma_{I#etaI#eta}(electron)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, cuts);
  
  
  
  variableNames.at(0) = "lep_DphiIn";
  histoName = "ele_DphiIn_EB";
  cuts -> at(0) = "( lep_isEB == 1 && lep_flavour == 11 )";
  stack -> SetXaxisRange(-0.040, 0.040);
  stack -> SetXaxisTitle("EB #Delta#phi_{in}(electron)");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, cuts);
  
  variableNames.at(0) = "lep_DphiIn";
  histoName = "ele_DphiIn_EE";
  cuts -> at(0) = "( lep_isEB == 0 && lep_flavour == 11 )";
  stack -> SetXaxisRange(-0.040, 0.040);
  stack -> SetXaxisTitle("EE #Delta#phi_{in}(electron)");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, cuts);
  
  
  
  variableNames.at(0) = "lep_DetaIn";
  histoName = "ele_DetaIn_EB";
  cuts -> at(0) = "( lep_isEB == 1 && lep_flavour == 11 )";
  stack -> SetXaxisRange(-0.010, 0.010);
  stack -> SetXaxisTitle("EB #Delta#eta_{in}(electron)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, cuts);
  
  variableNames.at(0) = "lep_DetaIn";
  histoName = "ele_DetaIn_EE";
  cuts -> at(0) = "( lep_isEB == 0 && lep_flavour == 11 )";
  stack -> SetXaxisRange(-0.010, 0.010);
  stack -> SetXaxisTitle("EE #Delta#eta_{in}(electron)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, cuts);
  
  
  
  variableNames.at(0) = "lep_HOverE";
  histoName = "ele_HOverE_EB";
  cuts -> at(0) = "( lep_isEB == 1 && lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 0.050);
  stack -> SetXaxisTitle("EB H/E(electron)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, cuts);
  
  variableNames.at(0) = "lep_HOverE";
  histoName = "ele_HOverE_EE";
  cuts -> at(0) = "( lep_isEB == 0 && lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 0.050);
  stack -> SetXaxisTitle("EE H/E(electron)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, cuts);
  
  
  
  
  
  
  variableNames.at(0) = "lep_dxy_PV";
  histoName = "ele_dxy_EB";
  cuts -> at(0) = "( lep_isEB == 1 && lep_flavour == 11 )";
  stack -> SetXaxisRange(-0.03, 0.03);
  stack -> SetXaxisTitle("EB d_{xy}(electron)");
  stack -> SetUnit("cm");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, cuts);
  
  variableNames.at(0) = "lep_dxy_PV";
  histoName = "ele_dxy_EE";
  cuts -> at(0) = "( lep_isEB == 0 && lep_flavour == 11 )";
  stack -> SetXaxisRange(-0.03, 0.03);
  stack -> SetXaxisTitle("EE d_{xy}(electron)");
  stack -> SetUnit("cm");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, cuts);
  
  
  
  variableNames.at(0) = "lep_dz_PV";
  histoName = "ele_dz_EB";
  cuts -> at(0) = "( lep_isEB == 1 && lep_flavour == 11 )";
  stack -> SetXaxisRange(-0.03, 0.03);
  stack -> SetXaxisTitle("EB d_{z}(electron)");
  stack -> SetUnit("cm");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, cuts);
  
  variableNames.at(0) = "lep_dz_PV";
  histoName = "ele_dz_EE";
  cuts -> at(0) = "( lep_isEB == 0 && lep_flavour == 11 )";
  stack -> SetXaxisRange(-0.03, 0.03);
  stack -> SetXaxisTitle("EE d_{z}(electron)");
  stack -> SetUnit("cm");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, cuts);
  
  
  
  variableNames.at(0) = "abs(lep_dxy_PV)/lep_edxy_PV";
  histoName = "ele_3DipSig_EB";
  cuts -> at(0) = "( lep_isEB == 1 && lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("EB 3D ip(electron)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, cuts);
  
  variableNames.at(0) = "abs(lep_dxy_PV)/lep_edxy_PV";
  histoName = "ele_3DipSig_EE";
  cuts -> at(0) = "( lep_isEB == 0 && lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("EE 3D ip(electron)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, cuts);
  
  
  
  
  
  
  variableNames.at(0) = "lep_tkIso/lep.pt()";
  histoName = "ele_tkRelIso_EB";
  cuts -> at(0) = "( lep_isEB == 1 && lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 0.2);
  stack -> SetXaxisTitle("EB #Sigma pt_{tracks} / pt_{electron}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, cuts);
  
  variableNames.at(0) = "lep_tkIso/lep.pt()";
  histoName = "ele_tkRelIso_EE";
  cuts -> at(0) = "( lep_isEB == 0 && lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 0.2);
  stack -> SetXaxisTitle("EE #Sigma pt_{tracks} / pt_{electron}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, cuts);
  
  
  
  variableNames.at(0) = "lep_emIso/lep.pt()";
  histoName = "ele_emRelIso_EB";
  cuts -> at(0) = "( lep_isEB == 1 && lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 0.2);
  stack -> SetXaxisTitle("EB #Sigma et_{ECAL} / pt_{electron}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, cuts);
  
  variableNames.at(0) = "lep_emIso/lep.pt()";
  histoName = "ele_emRelIso_EE";
  cuts -> at(0) = "( lep_isEB == 0 && lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 0.2);
  stack -> SetXaxisTitle("EE #Sigma et_{ECAL} / pt_{electron}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, cuts);
  
  
  
  variableNames.at(0) = "lep_hadIso/lep.pt()";
  histoName = "ele_hadRelIso_EB";
  cuts -> at(0) = "( lep_isEB == 1 && lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 0.2);
  stack -> SetXaxisTitle("EB #Sigma et_{HCAL} / pt_{electron}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, cuts);
  
  variableNames.at(0) = "lep_hadIso/lep.pt()";
  histoName = "ele_hadRelIso_EE";
  cuts -> at(0) = "( lep_isEB == 0 && lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 0.2);
  stack -> SetXaxisTitle("EE #Sigma et_{HCAL} / pt_{electron}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, cuts);
  
  
  
  variableNames.at(0) = "(lep_tkIso+lep_emIso+lep_hadIso)/lep.pt()";
  histoName = "ele_combRelIso_EB";
  cuts -> at(0) = "( lep_isEB == 1 && lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 0.4);
  stack -> SetXaxisTitle("EB #Sigma (pt_{trakcs}+et_{ECAL}+et_{HCAL}) / pt_{electron}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 80, PURescale, cuts);
  
  variableNames.at(0) = "(lep_tkIso+lep_emIso+lep_hadIso)/lep.pt()";
  histoName = "ele_combRelIso_EE";
  cuts -> at(0) = "( lep_isEB == 0 && lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 0.4);
  stack -> SetXaxisTitle("EE #Sigma (pt_{trakcs}+et_{ECAL}+et_{HCAL}) / pt_{electron}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 80, PURescale, cuts);
  */
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // muon
  //---------------------------------------------
  
  variableNames.at(0) = "lep_pt";
  histoName = "mu_pt";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("pt(muon)");
  stack -> SetUnit("GeV/c");
  stack -> Draw(variableNames, histoName, method, lumi, step, 60, PURescale, cuts);
  
  variableNames.at(0) = "lep.eta()";
  histoName = "mu_eta";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(-2.5, 2.5);
  stack -> SetXaxisTitle("#eta(muon)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi,step, 50, PURescale, cuts);
  
  variableNames.at(0) = "lep.phi()";
  histoName = "mu_phi";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(-2.5, 2.5);
  stack -> SetXaxisTitle("#phi(muon)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi,step, 50, PURescale, cuts);
  
  /*
  variableNames.at(0) = "lep_dxy_PV";
  histoName = "mu_dxy";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(-0.03, 0.03);
  stack -> SetXaxisTitle("d_{xy}(muon)");
  stack -> SetUnit("cm");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, cuts);
  
  variableNames.at(0) = "lep_dz_PV";
  histoName = "mu_dz";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(-0.03, 0.03);
  stack -> SetXaxisTitle("d_{z}(muon)");
  stack -> SetUnit("cm");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, cuts);
  
  variableNames.at(0) = "abs(lep_dxy_PV)/lep_edxy_PV";
  histoName = "mu_3DipSig";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("3D ip(muon)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, PURescale, cuts);
  
  variableNames.at(0) = "lep_normalizedChi2";
  histoName = "mu_normalizedChi2";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#chi^{2}/ndof(muon)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, cuts);
  
  variableNames.at(0) = "lep_numberOfValidTrackerHits";
  histoName = "mu_numberOfValidTrackerHits";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 40.);
  stack -> SetXaxisTitle("nHits_{tracker}(muon)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 20, PURescale, cuts);
  
  variableNames.at(0) = "lep_numberOfValidMuonHits";  
  histoName = "mu_numberOfValidMuonHits";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 60.);
  stack -> SetXaxisTitle("nHits(muon)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, cuts);
  
  
  
  
  
  
  variableNames.at(0) = "lep_tkIso/lep.pt()";
  histoName = "mu_tkRelIso";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 2.);
  stack -> SetXaxisTitle("#Sigma pt_{tracks} / pt_{muon}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, cuts);
   
  variableNames.at(0) = "lep_emIso/lep.pt()";
  histoName = "mu_emRelIso";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 0.2);
  stack -> SetXaxisTitle("#Sigma et_{ECAL} / pt_{muon}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, cuts);
  
  variableNames.at(0) = "lep_hadIso/lep.pt()";
  histoName = "mu_hadRelIso";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 0.2);
  stack -> SetXaxisTitle("#Sigma et_{HCAL} / pt_{muon}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, cuts);
  
  variableNames.at(0) = "(lep_tkIso+lep_emIso+lep_hadIso)/lep.pt()";
  histoName = "mu_combRelIso";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 0.4);
  stack -> SetXaxisTitle("#Sigma (pt_{tracks}+et_{ECAL}+et_{HCAL}) / pt_{muon}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 80, PURescale, cuts);
  */
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // Jets
  //---------------------------------------------
  
  /*variableNames.at(0) = "deltaR(lep.eta(),lep.phi(),leadingJ.eta(),leadingJ.phi())";
  histoName = "leadingJLep_DR";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("DR(leading jet-lepton)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale);
  
  variableNames.at(0) = "leadingJ.Et()";
  histoName = "leadingJ_et";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("et(leading jet)");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi, step, 20, PURescale);
  
  variableNames.at(0) = "leadingJ.eta()";
  histoName = "leadingJ_eta";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta(leading jet)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi,step, 50, PURescale);
  
  variableNames.at(0) = "leadingJ_bTag";
  histoName = "leadingJ_bTag";
  stack -> SetXaxisRange(-5., 10.);
  stack -> SetXaxisTitle("bTag(leading jet)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi,step, 60, PURescale);
  
  variableNames.at(0) = "leadingJ_chargedMultiplicity";
  histoName = "leadingJ_chargedMultiplicity";
  stack -> SetXaxisRange(0., 20.);
  stack -> SetXaxisTitle("charged multiplicity(leading jet)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi,step, 10, PURescale);
  */
  
  
  
  variableNames.at(0) = "thirdJ.Et()";
  histoName = "thirdJ_et";
  cuts -> at(0) = "thirdJ.Et() > 0.";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("et(third jet)");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, cuts);
  
  variableNames.at(0) = "thirdJ.eta()";
  histoName = "thirdJ_eta";
  cuts -> at(0) = "thirdJ.Et() > 0.";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta(third jet)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi,step, 50, PURescale, cuts);
  
  variableNames.at(0) = "thirdJ_WJJDeta";
  histoName = "thirdJ_WJJDeta";
  cuts -> at(0) = "thirdJ.Et() > 0.";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("#Delta#eta(third jet-WJet_{1})");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi,step, 50, PURescale, cuts);
  
  variableNames.at(0) = "thirdJ_tagJJDeta";
  histoName = "thirdJ_tagJJDeta";
  cuts -> at(0) = "thirdJ.Et() > 0.";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("#Delta#eta(third jet-tagJet_{1})");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi,step, 50, PURescale, cuts);
  
  variableNames.at(0) = "thirdJ_bTag";
  histoName = "thirdJ_bTag";
  cuts -> at(0) = "thirdJ.Et() > 0.";
  stack -> SetXaxisRange(-5., 10.);
  stack -> SetXaxisTitle("bTag(third jet)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi,step, 60, PURescale, cuts);
  
  
  
  
  
  
  variableNames.at(0) = "nJets";
  histoName = "jets_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("n(jets)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 10, PURescale);

  variableNames.at(0) = "nJets_cnt";
  histoName = "jets_cnt_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("n(central jets)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 10, PURescale);

  variableNames.at(0) = "nJets_fwd";
  histoName = "jets_fwd_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("n(forward jets)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 10, PURescale);
  
  variableNames.at(0) = "nJets_btw_et20";
  histoName = "jets_btw";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("n(between jets)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 10, PURescale);
  
  variableNames.at(0) = "jets_bTag1";
  histoName = "jets_bTag1";
  stack -> SetXaxisRange(-5., 10.);
  stack -> SetXaxisTitle("1^{st} bTag");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi,step, 60, PURescale);
  
  variableNames.at(0) = "jets_bTag2";
  histoName = "jets_bTag2";
  stack -> SetXaxisRange(-5., 10.);
  stack -> SetXaxisTitle("2^{nd} bTag2");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi,step, 60, PURescale);  
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------
  // W-Jets
  //---------------------------------------------
  
  variableNames.at(0) = "WJJ_m";
  histoName = "WJJ_m";
  stack -> SetXaxisRange(0., 400.);
  stack -> SetXaxisTitle("m(WJet_{1}+WJet_{2})");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale);
  
  variableNames.at(0) = "WJJ_et";
  histoName = "WJJ_et";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("et(WJet_{1}+WJet_{2})");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale);  
  
  variableNames.at(0) = "WJJ_Deta";
  histoName = "WJJ_Deta";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("#Delta#eta(WJet_{1}-WJet_{2})");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale);

  variableNames.at(0) = "WJJ_Dphi";
  histoName = "WJJ_Dphi";
  stack -> SetXaxisRange(0., 3.14159);
  stack -> SetXaxisTitle("#Delta#phi(WJet_{1}-WJet_{2})");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi, step, 36, PURescale);
  
  variableNames.at(0) = "WJJ_DR";
  histoName = "WJJ_DR";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("#DeltaR(WJet_{1}-WJet_{2})");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale);  
  
  variableNames.at(0) = "min(WJ1_lep_Dphi,WJ2_lep_Dphi)";
  histoName = "WJ1_lep_Dphi";
  stack -> SetXaxisRange(0., 3.14159);
  stack -> SetXaxisTitle("min #Delta#phi(WJet-lepton)");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi, step, 36, PURescale);
  
  variableNames.at(0) = "min(WJ1_met_Dphi,WJ2_met_Dphi)";
  histoName = "WJ1_met_Dphi";
  stack -> SetXaxisRange(0., 3.14159);
  stack -> SetXaxisTitle("min #Delta#phi(WJet-met)");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi, step, 36, PURescale);
  
  variableNames.at(0) = "abs(WJJ_zepp)";
  histoName = "WJJ_zepp";
  stack -> SetXaxisRange(0., 1.);
  stack -> SetXaxisTitle("zepp(WJet_{1}+WJet_{2})");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 20, PURescale);
  
  
  
  variableNames2.at(0) = "WJ1.Et()";
  variableNames2.at(1) = "WJ2.Et()";
  histoName = "WJJ_et_min+WJJ_et_max";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("et(WJets)");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames2, histoName, method, lumi, step, 30, PURescale);
  
  variableNames.at(0) = "max(WJ1.Et(), WJ2.Et())";
  histoName = "WJJ_et_max";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("max et(WJets)");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale);
  
  variableNames.at(0) = "min(WJ1.Et(), WJ2.Et())";
  histoName = "WJJ_et_min";
  stack -> SetXaxisRange(0., 150.);
  stack -> SetXaxisTitle("min et(WJets)");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale);
  
  
    
  variableNames2.at(0) = "WJ1.eta()";
  variableNames2.at(1) = "WJ1.eta()";
  histoName = "WJJ_eta_min+WJJ_eta_max";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta(WJets)");
  stack -> SetUnit("");
  stack -> Draw(variableNames2, histoName, method, lumi, step, 50, PURescale);
  
  variableNames.at(0) = "max(abs(WJ1.eta()), abs(WJ2.eta()))";
  histoName = "WJJ_absEta_max";
  stack -> SetXaxisRange(0., 2.5);
  stack -> SetXaxisTitle("max #eta(WJets)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 25, PURescale);
  
  variableNames.at(0) = "min(abs(WJ1.eta()), abs(WJ2.eta()))";
  histoName = "WJJ_absEta_min";
  stack -> SetXaxisRange(0., 2.5);
  stack -> SetXaxisTitle("min eta(WJets)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 25, PURescale);  
  
  
  
  variableNames2.at(0) = "abs(WJ1_zepp)";
  variableNames2.at(1) = "abs(WJ2_zepp)";
  histoName = "WJJ_zepp_min+WJJ_zepp_max";
  cuts2 -> at(0) = "( tagJ1.Et() > 0. && tagJ2.Et() > 0. )";
  cuts2 -> at(1) = "( tagJ1.Et() > 0. && tagJ2.Et() > 0. )";
  stack -> SetXaxisRange(0, 1.);
  stack -> SetXaxisTitle("zepp(WJets)");
  stack -> SetUnit("");
  stack -> Draw(variableNames2, histoName, method, lumi, step, 20, PURescale, cuts2);
  
  variableNames.at(0) = "max(abs(WJ1_zepp), abs(WJ2_zepp))";
  histoName = "WJJ_zepp_max";
  cuts -> at(0) = "( tagJ1.Et() > 0. && tagJ2.Et() > 0. )";
  stack -> SetXaxisRange(0, 1.);
  stack -> SetXaxisTitle("max zepp(WJets)");
  stack -> SetUnit(""); 
  stack -> Draw(variableNames, histoName, method, lumi, step, 20, PURescale, cuts);
  
  variableNames.at(0) = "min(abs(WJ1_zepp), abs(WJ2_zepp))";
  histoName = "WJJ_zepp_min";
  cuts -> at(0) = "( tagJ1.Et() > 0. && tagJ2.Et() > 0. )";
  stack -> SetXaxisRange(0, 1.);
  stack -> SetXaxisTitle("min zepp(WJets)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 20, PURescale, cuts);
  
  
  
  variableNames.at(0) = "WJ1_bTag+WJ2_bTag";
  histoName = "WJ1_bTag+WJ2_bTag";
  stack -> SetXaxisRange(-5., 10.);
  stack -> SetXaxisTitle("bTag(WJet_{1})+bTag(WJet_{2})");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 60, PURescale);
  
  variableNames.at(0) = "-1.*lep_charge*(WJ1_charge+WJ2_charge)";
  histoName = "WJJ_charge";
  stack -> SetXaxisRange(-3.5, 3.5);
  stack -> SetXaxisTitle("charge(WJets)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 7, PURescale);
  
  variableNames.at(0) = "WJ1_chargedMultiplicity+WJ2_chargedMultiplicity";
  histoName = "WJJ_chargedMultiplicity";
  stack -> SetXaxisRange(0., 40.);
  stack -> SetXaxisTitle("charged multiplicity(WJets)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 20, PURescale);
  
  
  
  //variableNames2.at(0) = "WJ1_dzAvg-PV_z";
  //variableNames2.at(1) = "WJ2_dzAvg-PV_z";
  //histoName = "WJJ_dzAvg_max+WJJ_dzAvg_min";
  //cuts2->at(0) = "WJ1.Et() > 0. && WJ2.Et() > 0. && WJ1_dzAvg > -99. && WJ2_dzAvg > -99.";
  //cuts2->at(1) = "WJ1.Et() > 0. && WJ2.Et() > 0. && WJ1_dzAvg > -99. && WJ2_dzAvg > -99.";
  //stack -> SetXaxisRange(-10., 10.);
  //stack -> SetXaxisTitle("dz(WJets)");
  //stack -> SetUnit("");
  //stack -> Draw(variableNames2, histoName, method, lumi, step, 100, false, cuts2);
  
  //variableNames.at(0) = "max(abs(WJ1_dzAvg-PV_z),abs(WJ2_dzAvg-PV_z))";
  //histoName = "WJJ_dzAvg_max";
  //cuts -> at(0) = "WJ1.Et() > 0. && WJ2.Et() > 0. && WJ1_dzAvg > -99. && WJ2_dzAvg > -99.";
  //stack -> SetXaxisRange(0., 10.);
  //stack -> SetXaxisTitle("max dz(WJets)");
  //stack -> SetUnit("");
  //stack -> Draw(variableNames, histoName, method, lumi, step, 50, false, cuts);
  
  //variableNames.at(0) = "min(abs(WJ1_dzAvg-PV_z),abs(WJ2_dzAvg-PV_z))";
  //histoName = "WJJ_dzAvg_min";
  //cuts -> at(0) = "WJ1.Et() > 0. && WJ2.Et() > 0. && WJ1_dzAvg > -99. && WJ2_dzAvg > -99.";
  //stack -> SetXaxisRange(0., 10.);
  //stack -> SetXaxisTitle("min dz(WJets)");
  //stack -> SetUnit("");
  //stack -> Draw(variableNames, histoName, method, lumi, step, 50, false, cuts);
  
  
  
  
  
  
  variableNames.at(0) = "lepWJJ_pt1";
  histoName = "lepWJJ_pt1";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("1^{st} pt(lepton,WJets)");
  stack -> SetUnit("GeV/c");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale);
  
  variableNames.at(0) = "lepWJJ_pt2";
  histoName = "lepWJJ_pt2";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("2^{nd} pt(lepton,WJets)");
  stack -> SetUnit("GeV/c");
  stack -> Draw(variableNames, histoName, method, lumi, step, 20, PURescale);
  
  variableNames.at(0) = "lepWJJ_pt3";
  histoName = "lepWJJ_pt3";
  stack -> SetXaxisRange(0., 100.);
  stack -> SetXaxisTitle("3^{rd} pt(lepton,WJets)");
  stack -> SetUnit("GeV/c");
  stack -> Draw(variableNames, histoName, method, lumi, step, 20, PURescale);
  
  
  
  
  
  
  
  
  
  
  

  //---------------------------------------------
  // Higgs
  //---------------------------------------------
    
  variableNames.at(0) = "lepNuW_m";
  histoName = "lepNuW_m";
  stack -> SetXaxisRange(0., 800);
  stack -> SetXaxisTitle("m(lepton+neutrino+WJets)");
  stack -> SetUnit("GeV/c2");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale);
  
  variableNames.at(0) = "abs(lepNuW_zepp)";
  histoName = "lepNuW_zepp";
  stack -> SetXaxisRange(0., 1.);
  stack -> SetXaxisTitle("zepp(lepton+neutrino+WJets)");
  stack -> SetUnit("GeV/c2");
  stack -> Draw(variableNames, histoName, method, lumi, step, 20, PURescale);
  
  variableNames.at(0) = "lepMetW_pt/lepNuW_m";
  histoName = "lepMetW_jacobian";
  stack -> SetXaxisRange(0., 2.);
  stack -> SetXaxisTitle("pt(lepton+met+WJets)/m(lepton+neutrino+WJets)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, PURescale);
  
  
  
  variableNames.at(0) = "lepMetW_pt";
  histoName = "lepMetW_pt";
  stack -> SetXaxisRange(0., 200);
  stack -> SetXaxisTitle("pt(lepton+met+WJets)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale);
  
  variableNames.at(0) = "lepMetW_mt";
  histoName = "lepMetW_mt";
  stack -> SetXaxisRange(100., 800.);
  stack -> SetXaxisTitle("mt(lepton+met+WJets)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 35, PURescale);
  
  variableNames.at(0) = "lepMetW_Dphi";
  histoName = "lepMetW_Dphi";
  stack -> SetXaxisRange(0., 3.14159);
  stack -> SetXaxisTitle("#Delta#phi(lepton+met-WJets)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 36, PURescale);
  
  
  

  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------
  // tag-Jets
  //---------------------------------------------
  
  variableNames.at(0) = "tagJJ_m";
  histoName = "tagJJ_m";
  cuts -> at(0) = "( tagJ1.Et() > 0. && tagJ2.Et() > 0. )";
  stack -> SetXaxisRange(0., 2500.);
  stack -> SetXaxisTitle("m(tagJet_{1}+tagJet_{2})");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, cuts);
  
  variableNames.at(0) = "tagJJ_Deta";
  histoName = "tagJJ_Deta";
  cuts -> at(0) = "( tagJ1.Et() > 0. && tagJ2.Et() > 0. )";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#Delta#eta(tagJet_{1}-tagJet_{2})");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, cuts);  
  
  variableNames.at(0) = "tagJJ_Dphi";
  histoName = "tagJJ_Dphi";
  cuts -> at(0) = "( tagJ1.Et() > 0. && tagJ2.Et() > 0. )";
  stack -> SetXaxisRange(0., 3.14159);
  stack -> SetXaxisTitle("#Delta#phi(tagJet_{1}-tagJet_{2})");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 36, PURescale, cuts);  
  
  variableNames.at(0) = "tagJ1.eta()*tagJ2.eta()";
  histoName = "tagJJ_etaProd";
  cuts -> at(0) = "( tagJ1.Et() > 0. && tagJ2.Et() > 0. )";
  stack -> SetXaxisRange(-25., 0.);
  stack -> SetXaxisTitle("#eta(tagJet_{1})#eta(tagJet_{2})");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, cuts);  
  
  
  
  variableNames2.at(0) = "tagJ1.eta()";
  variableNames2.at(1) = "tagJ2.eta()";
  histoName = "tagJJ_eta_min+tagJJ_eta_max";
  cuts2->at(0) = "( tagJ1.Et() > 0. && tagJ2.Et() > 0. )";
  cuts2->at(1) = "( tagJ1.Et() > 0. && tagJ2.Et() > 0. )";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta(tagJets)");
  stack -> SetUnit("");
  stack -> Draw(variableNames2, histoName, method, lumi, step, 50, PURescale, cuts2);
  
  variableNames.at(0) = "max(abs(tagJ1.eta()),abs(tagJ2.eta()))";
  histoName = "tagJJ_absEta_max";
  cuts -> at(0) = "( tagJ1.Et() > 0. && tagJ2.Et() > 0. )";
  stack -> SetXaxisRange(0, 6);
  stack -> SetXaxisTitle("max #eta(tagJets)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, cuts);
  
  variableNames.at(0) = "min(abs(tagJ1.eta()),abs(tagJ2.eta()))";
  histoName = "tagJJ_absEta_min";
  cuts -> at(0) = "( tagJ1.Et() > 0. && tagJ2.Et() > 0. )";
  stack -> SetXaxisRange(0, 6);
  stack -> SetXaxisTitle("min #eta(tagJets)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, cuts);
  
  
  
  variableNames.at(0) = "tagJ1.Et()";
  variableNames.at(0) = "tagJ2.Et()";
  histoName = "tagJJ_et_min+tagJJ_et_max";
  cuts -> at(0) = "( tagJ1.Et() > 0. && tagJ2.Et() > 0. )";
  stack -> SetXaxisRange(0, 300);
  stack -> SetXaxisTitle("et(tagJets)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 60, PURescale, cuts);
  
  variableNames.at(0) = "max(abs(tagJ1.Et()),abs(tagJ2.Et()))";
  histoName = "tagJJ_et_max";
  cuts -> at(0) = "( tagJ1.Et() > 0. && tagJ2.Et() > 0. )";
  stack -> SetXaxisRange(0, 300);
  stack -> SetXaxisTitle("max et(tagJets)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 60, PURescale, cuts);
  
  variableNames.at(0) = "min(abs(tagJ1.Et()),abs(tagJ2.Et()))";
  histoName = "tagJJ_et_min";
  cuts -> at(0) = "( tagJ1.Et() > 0. && tagJ2.Et() > 0. )";
  stack -> SetXaxisRange(0, 150);
  stack -> SetXaxisTitle("min et(tagJets)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, cuts);
  
  
  
  variableNames.at(0) = "tagJ1_chargedMultiplicity+tagJ2_chargedMultiplicity";
  histoName = "tagJJ_chargedMultiplicity";
  cuts -> at(0) = "( tagJ1.Et() > 0. && tagJ2.Et() > 0. )";
  stack -> SetXaxisRange(0., 40.);
  stack -> SetXaxisTitle("charged multiplicity(tagJets)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 20, PURescale, cuts);
  
  
  
  //variableNames2.at(0) = "tagJ1_dzAvg-PV_z";
  //variableNames2.at(1) = "tagJ2_dzAvg-PV_z";
  //histoName = "tagJJ_dzAvg_max+tagJJ_dzAvg_min";
  //cuts2->at(0) = "tagJ1.Et() > 0. && tagJ2.Et() > 0. && tagJ1_dzAvg > -99. && tagJ2_dzAvg > -99.";
  //cuts2->at(1) = "tagJ1.Et() > 0. && tagJ2.Et() > 0. && tagJ1_dzAvg > -99. && tagJ2_dzAvg > -99.";
  //stack -> SetXaxisRange(-10., 10.);
  //stack -> SetXaxisTitle("dz(tagJets)");
  //stack -> SetUnit("");
  //stack -> Draw(variableNames2, histoName, method, lumi, step, 100, false, cuts2);
  
  //variableNames.at(0) = "max(abs(tagJ1_dzAvg-PV_z),abs(tagJ2_dzAvg-PV_z))";
  //histoName = "tagJJ_dzAvg_max";
  //cuts -> at(0) = "tagJ1.Et() > 0. && tagJ2.Et() > 0. && tagJ1_dzAvg > -99. && tagJ2_dzAvg > -99.";
  //stack -> SetXaxisRange(0., 10.);
  //stack -> SetXaxisTitle("max dz(tagJets)");
  //stack -> SetUnit("");
  //stack -> Draw(variableNames, histoName, method, lumi, step, 50, false, cuts);
  
  //variableNames.at(0) = "min(abs(tagJ1_dzAvg-PV_z),abs(tagJ2_dzAvg-PV_z))";
  //histoName = "tagJJ_dzAvg_min";
  //cuts -> at(0) = "tagJ1.Et() > 0. && tagJ2.Et() > 0. && tagJ1_dzAvg > -99. && tagJ2_dzAvg > -99.";
  //stack -> SetXaxisRange(0., 10.);
  //stack -> SetXaxisTitle("min dz(tagJets)");
  //stack -> SetUnit("");
  //stack -> Draw(variableNames, histoName, method, lumi, step, 50, false, cuts);  
  
  
  
  
  
  
//  histoName = "jets_etaEtaMoment";
//  stack -> SetXaxisRange(0., 0.1);
//  stack -> SetXaxisTitle("#eta#eta_{jets}");
//  stack -> SetUnit("");
//  stack -> Draw(variableNames, histoName, method, lumi,step, 50., PURescale, cuts);
//  
//  histoName = "jets_cnt_etaEtaMoment";
//  stack -> SetXaxisRange(0., 0.1);
//  stack -> SetXaxisTitle("#eta#eta_{central jets}");
//  stack -> SetUnit("");
//  stack -> Draw(variableNames, histoName, method, lumi,step, 50., PURescale, cuts);
//  
//  histoName = "jets_fwd_etaEtaMoment";
//  stack -> SetXaxisRange(0., 0.1);
//  stack -> SetXaxisTitle("#eta#eta_{forward jets}");
//  stack -> SetUnit("");
//  stack -> Draw(variableNames, histoName, method, lumi,step, 50., PURescale, cuts);
//  
//  
//  
//  histoName = "jets_phiPhiMoment";
//  stack -> SetXaxisRange(0., 0.1);
//  stack -> SetXaxisTitle("#phi#phi_{jets}");
//  stack -> SetUnit("");
//  stack -> Draw(variableNames, histoName, method, lumi,step, 50., PURescale, cuts);
//  
//  histoName = "jets_cnt_phiPhiMoment";
//  stack -> SetXaxisRange(0., 0.1);
//  stack -> SetXaxisTitle("#phi#phi_{central jets}");
//  stack -> SetUnit("");
//  stack -> Draw(variableNames, histoName, method, lumi,step, 50., PURescale, cuts);
//  
//  histoName = "jets_fwd_phiPhiMoment";
//  stack -> SetXaxisRange(0., 0.1);
//  stack -> SetXaxisTitle("#phi#phi_{forward jets}");
//  stack -> SetUnit("");
//  stack -> Draw(variableNames, histoName, method, lumi,step, 50., PURescale, cuts);
//  
//  
//  
//  histoName = "jets_etaPhiMoment";
//  stack -> SetXaxisRange(-0.05, 0.05);
//  stack -> SetXaxisTitle("#eta#phi_{jets}");
//  stack -> SetUnit("");
//  stack -> Draw(variableNames, histoName, method, lumi,step, 10., PURescale, cuts);
//  
//  histoName = "jets_cnt_etaPhiMoment";
//  stack -> SetXaxisRange(-0.05, 0.05);
//  stack -> SetXaxisTitle("#eta#phi_{central jets}");
//  stack -> SetUnit("");
//  stack -> Draw(variableNames, histoName, method, lumi,step, 10., PURescale, cuts);
//  
//  histoName = "jets_fwd_etaPhiMoment";
//  stack -> SetXaxisRange(-0.05, 0.05);
//  stack -> SetXaxisTitle("#eta#phi_{forward jets}");
//  stack -> SetUnit("");
//  stack -> Draw(variableNames, histoName, method, lumi,step, 10., PURescale, cuts);
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
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., PURescale, cuts);
//    
//    histoName = "jets_cnt_emEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("emEnergyFraction_{central jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., PURescale, cuts);
//    
//    histoName = "jets_fwd_emEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("emEnergyFraction_{forward jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., PURescale, cuts);
//    
//    
//    
//    histoName = "jets_fHPD";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("fHPD_{jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., PURescale, cuts);
//    
//    histoName = "jets_cnt_fHPD";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("fHPD_{central jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., PURescale, cuts);
//    
//    histoName = "jets_fwd_fHPD";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("fHPD_{forward jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., PURescale, cuts);
//    
//    
//    
//    histoName = "jets_fRBX";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("fRBX_{jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., PURescale, cuts);
//    
//    histoName = "jets_cnt_fRBX";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("fRBX_{central jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., PURescale, cuts);
//    
//    histoName = "jets_fwd_fRBX";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("fRBX_{forward jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., PURescale, cuts);
//    
//    
//    
//    histoName = "jets_n90Hits";
//    stack -> SetXaxisRange(0., 100.);
//    stack -> SetXaxisTitle("n90Hits_{jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 10., PURescale, cuts);
//    
//    histoName = "jets_cnt_n90Hits";
//    stack -> SetXaxisRange(0., 100.);
//    stack -> SetXaxisTitle("n90Hits_{central jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 10., PURescale, cuts);
//    
//    histoName = "jets_fwd_n90Hits";
//    stack -> SetXaxisRange(0., 100.);
//    stack -> SetXaxisTitle("n90Hits_{forward jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 10., PURescale, cuts);
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
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., PURescale, cuts);
//  
//    histoName = "jets_cnt_chargedHadronEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("chargedHadronEnergyFraction_{central jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., PURescale, cuts);
//    
//    histoName = "jets_fwd_chargedHadronEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("chargedHadronEnergyFraction_{forward jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., PURescale, cuts);
//    
//    
//    
//    histoName = "jets_neutralHadronEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("neutralHadronEnergyFraction_{jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., PURescale, cuts);  
//  
//    histoName = "jets_cnt_neutralHadronEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("neutralHadronEnergyFraction_{central jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., PURescale, cuts);
//    
//    histoName = "jets_fwd_neutralHadronEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("neutralHadronEnergyFraction_{forward jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., PURescale, cuts);  
//    
//    
//    
//    histoName = "jets_chargedEmEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("chargedEmEnergyFraction_{jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., PURescale, cuts);  
//    
//    histoName = "jets_cnt_chargedEmEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("chargedEmEnergyFraction_{central jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., PURescale, cuts);
//    
//    histoName = "jets_fwd_chargedEmEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("chargedEmEnergyFraction_{forward jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., PURescale, cuts);
//    
//    
//    
//    histoName = "jets_neutralEmEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("neutralEmEnergyFraction_{jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., PURescale, cuts);  
//    
//    histoName = "jets_cnt_neutralEmEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("neutralEmEnergyFraction_{central jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., PURescale, cuts);
//    
//    histoName = "jets_fwd_neutralEmEnergyFraction";
//    stack -> SetXaxisRange(0., 1.);
//    stack -> SetXaxisTitle("neutralEmEnergyFraction_{forward jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 50., PURescale, cuts);
//    
//    
//    
//    histoName = "jets_chargedMultiplicity";
//    stack -> SetXaxisRange(0., 40.);
//    stack -> SetXaxisTitle("chargedMultiplicity_{jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 1., PURescale, cuts);  
//    
//    histoName = "jets_cnt_chargedMultiplicity";
//    stack -> SetXaxisRange(0., 40.);
//    stack -> SetXaxisTitle("chargedMultiplicity_{central jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 1., PURescale, cuts);
//    
//    histoName = "jets_fwd_chargedMultiplicity";
//    stack -> SetXaxisRange(0., 40.);
//    stack -> SetXaxisTitle("chargedMultiplicity_{forward jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 1., PURescale, cuts);
//    
//    
//    
//    histoName = "jets_neutralMultiplicity";
//    stack -> SetXaxisRange(0., 40.);
//    stack -> SetXaxisTitle("neutralMultiplicity_{jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 1., PURescale, cuts);  
//    
//    histoName = "jets_cnt_neutralMultiplicity";
//    stack -> SetXaxisRange(0., 40.);
//    stack -> SetXaxisTitle("neutralMultiplicity_{central jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 1., PURescale, cuts);
//    
//    histoName = "jets_fwd_neutralMultiplicity";
//    stack -> SetXaxisRange(0., 40.);
//    stack -> SetXaxisTitle("neutralMultiplicity_{forward jets}");
//    stack -> SetUnit("");
//    stack -> Draw(variableNames, histoName, method, lumi,step, 1., PURescale, cuts);
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
  std::cout << ">>>VBFPrintPlots::command " << std::string(strCommand) << std::endl;
  
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
