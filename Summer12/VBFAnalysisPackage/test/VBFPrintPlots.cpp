#include "setTDRStyle.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "HiggsMassWindows.h"
#include "plotUtils.h"
#include "../obj/mydict.h"

#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"

#include <vector>
#include <iostream>
#include <string>
#include <sstream>
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
  int higgsMass = gConfigParser -> readIntOption("Input::higgsMass");
    
  std::string outputDir = gConfigParser -> readStringOption("Output::outputDir");
    
  std::string imgFormat = gConfigParser -> readStringOption("Options::imgFormat");
  int step = gConfigParser -> readIntOption("Options::step");  
  float lumiEle = gConfigParser -> readFloatOption("Options::lumiEle");
  float lumiMu = gConfigParser -> readFloatOption("Options::lumiMu");
  bool PURescale = gConfigParser -> readBoolOption("Options::PURescale");
  bool weightEvent = gConfigParser -> readBoolOption("Options::weightEvent");
  bool stackSig = gConfigParser -> readBoolOption("Options::stackSig");
  bool higgsMassCut = gConfigParser -> readBoolOption("Options::higgsMassCut");
  bool leptonWeight = gConfigParser -> readBoolOption("Options::leptonWeight");
  bool metWeight = gConfigParser -> readBoolOption("Options::metWeight");
  std::string method = gConfigParser -> readStringOption("Options::method");
  std::string jetType = gConfigParser -> readStringOption("Options::jetType");
  
  char stepDir[50];
  sprintf(stepDir, "step%d", step);
  char stepNumber[50];
  sprintf(stepNumber, "%d", step);
  std::string fullOutputDir = inputDir + "/plots/" + outputDir + "/" + method + "_" + stepDir + "/";
  std::string fullHtmlName  = inputDir + "/plots/" + outputDir + "/" + method + "_" + stepDir + ".html";
  
  
  
  
  
  
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
  stack -> SetYLegend(0.30, 0.92);
  
  float lumi = lumiEle*0.5 + lumiMu*0.5;
  stack -> DrawEvents("events", lumi, step, true, PURescale, stackSig);
  stack -> DrawEvents("eventsScaled", lumi, step, true, PURescale, stackSig);
  stack -> DrawEvents("eventsScaledStack", lumi, step, true, PURescale, stackSig);
  stack -> SetYaxisRange(0.0000001, 1.1);
  stack -> DrawEvents("efficiencies", lumi, step, true, PURescale, stackSig);
  stack -> SetYaxisRange(0.01, 1.1);
  stack -> DrawEvents("efficienciesRelative", lumi, step, true, PURescale, stackSig);
  
  
  
  
  std::vector<std::string> variableNames;
  variableNames.push_back("");
  std::vector<std::string> variableNames2;
  variableNames2.push_back("");
  variableNames2.push_back("");
  
  std::vector<std::string>* cuts = new std::vector<std::string>;
  std::vector<std::string>* cuts2 = new std::vector<std::string>;
  cuts  -> push_back("");
  cuts2 -> push_back("");  
  cuts2 -> push_back("");
  
  std::stringstream ss;
  ss << " * ( lepNuW_m_KF >= " << GetLepNuWMMIN(higgsMass) << " && lepNuW_m_KF < " << GetLepNuWMMAX(higgsMass) << ") ";
  
  std::string generalCut = "(1 == 1)";
  if( higgsMassCut == true )  generalCut += ss.str();
  if( leptonWeight == false ) generalCut += " * 1./leptonWeight ";
  if( metWeight    == false ) generalCut += " * 1./metWeight ";
  stack -> SetGeneralCut(generalCut);
  
  std::string histoName;
  
  
  
  //---------------------------------------------  
  // Angles
  //---------------------------------------------
  
  
  //---------------------------------------------  
  // electron
  //---------------------------------------------
  
  variableNames.at(0) = "PV_n";  
  histoName    = "ele_PV_n";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(-0.5, 49.5);
  stack -> SetXaxisTitle("N_{PV}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 50, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep_pt";
  histoName = "ele_pt";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("p_{T}^{electron}");
  stack -> SetUnit("GeV/c");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 20, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep.eta()";
  histoName = "ele_eta";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(-2.5, 2.5);
  stack -> SetXaxisTitle("#eta_{electron}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 25, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep.phi()";
  histoName = "ele_phi";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(-3.14159, 3.14159);
  stack -> SetXaxisTitle("#phi_{electron}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 18, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "abs(lep_dB)/lep_edB";
  histoName = "ele_3DipSig";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("3DIP_{electron}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 50, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep_combIso/lep_pt";
  histoName = "ele_relPfIso";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 0.3);
  stack -> SetXaxisTitle("relPfIso");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 50, PURescale, weightEvent, stackSig, cuts);

  variableNames.at(0) = "lep_combIso";
  histoName = "ele_pfIso";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("pfIso");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 50, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep_tkIso/lep.pt()";
  histoName = "ele_tkRelIso";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 0.2);
  stack -> SetXaxisTitle("#Sigma p_{T}^{trk} / p_{T}^{electron}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 20, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep_emIso/lep.pt()";
  histoName = "ele_emRelIso";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 0.2);
  stack -> SetXaxisTitle("#Sigma E_{T}^{em} / p_{T}^{electron}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 20, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep_hadIso/lep.pt()";
  histoName = "ele_hadRelIso";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 0.2);
  stack -> SetXaxisTitle("#Sigma E_{T}^{had} / p_{T}^{electron}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 20, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "(lep_tkIso+max(0.,lep_emIso)+lep_hadIso)/lep.pt()";
  histoName = "ele_combRelIso";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 0.3);
  stack -> SetXaxisTitle("#Sigma (p_{T}^{trk}+max(0.,E_{T}*{em})+E_{T}^{had}) / p_{T}^{electron}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 30, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "(lep_tkIso+max(0.,lep_emIso)+lep_hadIso-rhoForIsolation*3.14159*0.3*0.3)/lep.pt()";
  histoName = "ele_combRelIso_PUCorr";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(-0.3, 0.15);
  stack -> SetXaxisTitle("#Sigma (p_{T}^{trk}+max(0.,E_{T}^{em})+E_{T}^{had} - #rho_{PU}#pi#DeltaR^{2} ) / p_{T}^{electron}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 45, PURescale, weightEvent, stackSig, cuts);
  
  
  
  
  
  
  variableNames.at(0) = "met.Et()";
  histoName = "ele_met_et";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("ME_{T}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 30, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "met.phi()";  
  histoName = "ele_met_phi";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(-3.14159, 3.14159);
  stack -> SetXaxisTitle("#phi_{ME_{T}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 18, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lepMet_mt";
  histoName = "ele_lepMet_mt";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 150.);
  stack -> SetXaxisTitle("m_{T}^{lepton+ME_{T}}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 15, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lepMet_Dphi";
  histoName = "ele_lepMet_Dphi";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 3.14159);
  stack -> SetXaxisTitle("#Delta#phi_{lepton-ME_{T}}");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 18, PURescale, weightEvent, stackSig, cuts);
  
  
  
  
  
  
  variableNames2.at(0) = "WJ1.pt()";
  variableNames2.at(1) = "WJ2.pt()";
  histoName = "ele_WJ1_pt+WJ2_pt";
  cuts2 -> at(0) = "( lep_flavour == 11 )";
  cuts2 -> at(1) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("p_{T}^{jet_{1},jet_{2}}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames2, histoName, method, lumiEle, step, 30, PURescale, weightEvent, stackSig, cuts2);
  
  variableNames.at(0) = "max(WJ1.pt(), WJ2.pt())";
  histoName = "ele_WJ1_pt";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("max p_{T}^{jet_{1},jet_{2}}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 30, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "min(WJ1.pt(), WJ2.pt())";
  histoName = "ele_WJ2_pt";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 150.);
  stack -> SetXaxisTitle("min p_{T}^{jet_{1},jet_{2}}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 15, PURescale, weightEvent, stackSig, cuts);
  
  
    
  variableNames2.at(0) = "WJ1.eta()";
  variableNames2.at(1) = "WJ1.eta()";
  histoName = "ele_WJ1_eta+WJ2_eta";
  cuts2 -> at(0) = "( lep_flavour == 11 )";
  cuts2 -> at(1) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta_{jet_{1},jet_{2}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames2, histoName, method, lumiEle, step, 25, PURescale, weightEvent, stackSig, cuts2);
  
  variableNames.at(0) = "max(abs(WJ1.eta()), abs(WJ2.eta()))";
  histoName = "ele_WJ1_absEta";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 2.5);
  stack -> SetXaxisTitle("max |#eta|_{jet_{1},jet_{2}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 12, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "min(abs(WJ1.eta()), abs(WJ2.eta()))";
  histoName = "ele_WJ2_absEta";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 2.5);
  stack -> SetXaxisTitle("min |#eta|_{jet_{1},jet_{2}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 12, PURescale, weightEvent, stackSig, cuts);
  
  
  
  
  
  
  variableNames.at(0) = "WJJ_m";
  histoName = "ele_WJJ_m";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("m_{jj}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 30, PURescale, weightEvent, stackSig, cuts);

  variableNames.at(0) = "WJJ_m";
  histoName = "ele_WJJ_m_blind";
  cuts -> at(0) = "( lep_flavour == 11 ) && ( WJJ_m < 65 || WJJ_m > 95 )";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("m_{jj}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 30, PURescale, weightEvent, stackSig, cuts);

  variableNames.at(0) = "WJJ_pt";
  histoName = "ele_WJJ_pt";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("p_{T}^{jj}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 30, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "WJJ_eta";
  histoName = "ele_WJJ_eta";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta^{jj}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 25, PURescale, weightEvent, stackSig, cuts);

  variableNames.at(0) = "WJJ_Deta";
  histoName = "ele_WJJ_Deta";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("#Delta#eta_{jj}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 25, PURescale, weightEvent, stackSig, cuts);

  variableNames.at(0) = "WJJ_Dphi";
  histoName = "ele_WJJ_Dphi";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 3.14159);
  stack -> SetXaxisTitle("#Delta#phi_{jj}");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 18, PURescale, weightEvent, stackSig, cuts);
  
  
  
  
  
  
  
  variableNames.at(0) = "lepNuW_eta";
  histoName = "ele_lepNuW_eta";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta_{l#nujj}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 25, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lepNuW_m";
  histoName = "ele_lepNuW_m";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(100., 800);
  stack -> SetXaxisTitle("m_{l#nujj}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 35, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lepNuW_m_KF";
  histoName = "ele_lepNuW_m_KF";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(100., 800);
  stack -> SetXaxisTitle("m_{l#nujj} after KF");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 35, PURescale, weightEvent, stackSig, cuts);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // muon
  //---------------------------------------------
  variableNames.at(0) = "PV_n";  
  histoName    = "mu_PV_n";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(-0.5, 49.5);
  stack -> SetXaxisTitle("N_{PV}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 50, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep_pt";
  histoName = "mu_pt";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("p_{T}^{muon}");
  stack -> SetUnit("GeV/c");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 20, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep.eta()";
  histoName = "mu_eta";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(-2.5, 2.5);
  stack -> SetXaxisTitle("#eta_{muon}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 25, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep.phi()";
  histoName = "mu_phi";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(-3.14159, 3.14159);
  stack -> SetXaxisTitle("#phi_{muon}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 18, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "abs(lep_dB)/lep_edB";
  histoName = "mu_3DipSig";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("3DIP_{muon}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 50, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep_combIso/lep_pt";
  histoName = "mu_relPfIso";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 0.3);
  stack -> SetXaxisTitle("relPfIso");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 50, PURescale, weightEvent, stackSig, cuts);

  variableNames.at(0) = "lep_combIso";
  histoName = "mu_pfIso";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("pfIso");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 50, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep_tkIso/lep.pt()";
  histoName = "mu_tkRelIso";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 0.2);
  stack -> SetXaxisTitle("#Sigma p_{T}^{trk} / p_{T}^{muon}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 20, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep_emIso/lep.pt()";
  histoName = "mu_emRelIso";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 0.2);
  stack -> SetXaxisTitle("#Sigma E_{T}^{em} / p_{T}^{muon}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 20, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep_hadIso/lep.pt()";
  histoName = "mu_hadRelIso";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 0.2);
  stack -> SetXaxisTitle("#Sigma E_{T}^{had} / p_{T}^{muon}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 20, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "(lep_tkIso+max(0.,lep_emIso)+lep_hadIso)/lep.pt()";
  histoName = "mu_combRelIso";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 0.3);
  stack -> SetXaxisTitle("#Sigma (p_{T}^{trk}+max(0.,E_{T}*{em})+E_{T}^{had}) / p_{T}^{muon}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 30, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "(lep_tkIso+max(0.,lep_emIso)+lep_hadIso-rhoForIsolation*3.14159*0.3*0.3)/lep.pt()";
  histoName = "mu_combRelIso_PUCorr";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(-0.3, 0.15);
  stack -> SetXaxisTitle("#Sigma (p_{T}^{trk}+max(0.,E_{T}^{em})+E_{T}^{had} - #rho_{PU}#pi#DeltaR^{2} ) / p_{T}^{muon}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 45, PURescale, weightEvent, stackSig, cuts);
  
  
  
  
  
  
  variableNames.at(0) = "met.Et()";
  histoName = "mu_met_et";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("ME_{T}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 30, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "met.phi()";  
  histoName = "mu_met_phi";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(-3.14159, 3.14159);
  stack -> SetXaxisTitle("#phi_{ME_{T}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 18, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lepMet_mt";
  histoName = "mu_lepMet_mt";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 150.);
  stack -> SetXaxisTitle("m_{T}^{lepton+ME_{T}}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 15, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lepMet_Dphi";
  histoName = "mu_lepMet_Dphi";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 3.14159);
  stack -> SetXaxisTitle("#Delta#phi_{lepton-ME_{T}}");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 18, PURescale, weightEvent, stackSig, cuts);
  
  
  
  
  
  
  variableNames2.at(0) = "WJ1.pt()";
  variableNames2.at(1) = "WJ2.pt()";
  histoName = "mu_WJ1_pt+WJ2_pt";
  cuts2 -> at(0) = "( lep_flavour == 13 )";
  cuts2 -> at(1) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("p_{T}^{jet_{1},jet_{2}}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames2, histoName, method, lumiEle, step, 30, PURescale, weightEvent, stackSig, cuts2);
  
  variableNames.at(0) = "max(WJ1.pt(), WJ2.pt())";
  histoName = "mu_WJ1_pt";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("max p_{T}^{jet_{1},jet_{2}}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 30, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "min(WJ1.pt(), WJ2.pt())";
  histoName = "mu_WJ2_pt";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 150.);
  stack -> SetXaxisTitle("min p_{T}^{jet_{1},jet_{2}}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 15, PURescale, weightEvent, stackSig, cuts);
  
  
    
  variableNames2.at(0) = "WJ1.eta()";
  variableNames2.at(1) = "WJ1.eta()";
  histoName = "mu_WJ1_eta+WJ2_eta";
  cuts2 -> at(0) = "( lep_flavour == 13 )";
  cuts2 -> at(1) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta_{jet_{1},jet_{2}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames2, histoName, method, lumiEle, step, 25, PURescale, weightEvent, stackSig, cuts2);
  
  variableNames.at(0) = "max(abs(WJ1.eta()), abs(WJ2.eta()))";
  histoName = "mu_WJ1_absEta";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 2.5);
  stack -> SetXaxisTitle("max |#eta|_{jet_{1},jet_{2}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 12, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "min(abs(WJ1.eta()), abs(WJ2.eta()))";
  histoName = "mu_WJ2_absEta";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 2.5);
  stack -> SetXaxisTitle("min |#eta|_{jet_{1},jet_{2}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 12, PURescale, weightEvent, stackSig, cuts);
  
  
  
  
  
  
  variableNames.at(0) = "WJJ_m";
  histoName = "mu_WJJ_m";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("m_{jj}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 30, PURescale, weightEvent, stackSig, cuts);

  variableNames.at(0) = "WJJ_m";
  histoName = "mu_WJJ_m_blind";
  cuts -> at(0) = "( lep_flavour == 13 ) && ( WJJ_m < 65 || WJJ_m > 95 )";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("m_{jj}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 30, PURescale, weightEvent, stackSig, cuts);

  variableNames.at(0) = "WJJ_pt";
  histoName = "mu_WJJ_pt";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("p_{T}^{jj}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 30, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "WJJ_eta";
  histoName = "mu_WJJ_eta";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta^{jj}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 25, PURescale, weightEvent, stackSig, cuts);

  variableNames.at(0) = "WJJ_Deta";
  histoName = "mu_WJJ_Deta";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("#Delta#eta_{jj}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 25, PURescale, weightEvent, stackSig, cuts);

  variableNames.at(0) = "WJJ_Dphi";
  histoName = "mu_WJJ_Dphi";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 3.14159);
  stack -> SetXaxisTitle("#Delta#phi_{jj}");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 18, PURescale, weightEvent, stackSig, cuts);
  
  
  
  
  
  
  
  variableNames.at(0) = "lepNuW_eta";
  histoName = "mu_lepNuW_eta";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta_{l#nujj}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 25, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lepNuW_m";
  histoName = "mu_lepNuW_m";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(100., 800);
  stack -> SetXaxisTitle("m_{l#nujj}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 35, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lepNuW_m_KF";
  histoName = "mu_lepNuW_m_KF";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(100., 800);
  stack -> SetXaxisTitle("m_{l#nujj} after KF");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumiEle, step, 35, PURescale, weightEvent, stackSig, cuts);
  
  
  
  
  
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
