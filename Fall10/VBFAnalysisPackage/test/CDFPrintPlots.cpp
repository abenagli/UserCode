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
    std::cerr << ">>>>> CDFPrintPlots_Collisions7TeV::usage: " << argv[0] << " configFileName" << std::endl ;
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
  std::string fullHtmlName  = inputDir + "/plots/" + stepDir + "_" + jetType + "_" + method + ".html";
  
  
  
  
  
  
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
  stack -> SetXaxisRange(-0.30, 0.20);
  stack -> SetXaxisTitle("mva");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, PURescale);
  
  
  
  
  
  
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
  
  
  
  
  
  
  
  
  
  
  
  
  std::cout << std::endl;
}





int RunCommand(const char *strCommand)
{
  std::cout << ">>>CDFPrintPlots::command " << std::string(strCommand) << std::endl;
  
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