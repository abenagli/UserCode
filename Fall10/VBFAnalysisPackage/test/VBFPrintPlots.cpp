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
  float lumi = gConfigParser -> readFloatOption("Options::lumi");
  bool PURescale = gConfigParser -> readBoolOption("Options::PURescale");
  bool weightEvent = gConfigParser -> readBoolOption("Options::weightEvent");
  bool stackSig = gConfigParser -> readBoolOption("Options::stackSig");
  bool higgsMassCut = gConfigParser -> readBoolOption("Options::higgsMassCut");
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
  stack -> SetYLegend(0.50, 0.90);
  
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
  ss << " ( lepNuW_m_KF >= " << GetLepNuWMMIN(higgsMass) << " && lepNuW_m_KF < " << GetLepNuWMMAX(higgsMass) << ") ";
  
  std::string generalCut;
  if( higgsMassCut == true ) generalCut = ss.str();
  else                       generalCut = "(1 == 1)";
  stack -> SetGeneralCut(generalCut);
  
  std::string histoName;
  
  
  
  
  
  
  
  
  
    
  
  
  //---------------------------------------------  
  // Angles
  //---------------------------------------------
  
  variableNames.at(0) = "lepNuW_cphi";
  histoName = "angles_cphi";
  stack -> SetXaxisRange(-1., 1.);
  stack -> SetXaxisTitle("cos#phi_{W_{1}-W_{2} decay planes}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "lepNuZ_cphi";
  histoName = "angles_cphistar";
  stack -> SetXaxisRange(-1., 1.);
  stack -> SetXaxisTitle("cos#phi*_{W_{1} production-decay planes}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "lep_ctheta";
  histoName = "angles_ctheta1";
  stack -> SetXaxisRange(-1., 1.);
  stack -> SetXaxisTitle("cos#theta_{1}^{lepton-Higgs}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "WJ1_ctheta";
  histoName = "angles_ctheta2";
  stack -> SetXaxisRange(-1., 1.);
  stack -> SetXaxisTitle("cos#theta_{2}^{jet_{1}-Higgs}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "lepNu_ctheta";
  histoName = "angles_cthetastar";
  stack -> SetXaxisRange(-1., 1.);
  stack -> SetXaxisTitle("cos#theta*_{W_{1}-W_{2}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "helicityLikelihood";
  histoName = "angles_helicityLikelihood";
  stack -> SetXaxisRange(0., 1.);
  stack -> SetXaxisTitle("helicity likelihood discriminant");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // MVA & KF
  //---------------------------------------------
  //
  //variableNames.at(0) = "mva";  
  //histoName = "mva";
  //stack -> SetXaxisRange(-0.8, 0.4);
  //stack -> SetXaxisTitle("mva");
  //stack -> SetUnit("");
  //stack -> Draw(variableNames, histoName, method, lumi, step, 24, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "chi2_KF/ndf_KF";
  histoName = "chi2_KF";
  stack -> SetXaxisRange(0., 100.);
  stack -> SetXaxisTitle("(#chi^{2}/N_{dof}^{KF}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // PV & rho
  //---------------------------------------------
  
  variableNames.at(0) = "PV_n";  
  histoName    = "PV_n";
  stack -> SetXaxisRange(-0.5, 34.5);
  stack -> SetXaxisTitle("N_{PV}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 35, PURescale, weightEvent, stackSig);
    
  variableNames.at(0) = "PV_d0";  
  histoName    = "PV_d0";
  stack -> SetXaxisRange(0., 0.5);
  stack -> SetXaxisTitle("d_{0}^{PV}");
  stack -> SetUnit("cm");
  stack -> Draw(variableNames, histoName, method, lumi, step, 250, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "PV_z";
  histoName    = "PV_z";
  stack -> SetXaxisRange(-30., 30.);
  stack -> SetXaxisTitle("z^{PV}");
  stack -> SetUnit("cm");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "PV_nTracks";
  histoName    = "PV_nTracks";
  stack -> SetXaxisRange(0., 150);
  stack -> SetXaxisTitle("N_{tracks}^{PV}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig);
  
  //variableNames.at(0) = "PV_ndof";
  //histoName    = "PV_ndof";
  //stack -> SetXaxisRange(0., 250);
  //stack -> SetXaxisTitle("N_{ndof}^{PV}");
  //stack -> SetUnit("");
  //stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig);
  
  //variableNames.at(0) = "PV_normalizedChi2";
  //histoName    = "PV_normalizedChi2";
  //stack -> SetXaxisRange(0., 2.);
  //stack -> SetXaxisTitle("#chi^{2}/N_{dof}^{PV}");
  //stack -> SetUnit("");
  //stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig);
  
  
  
  variableNames.at(0) = "rhoForJets";
  histoName    = "PU_rhoForJets";
  stack -> SetXaxisRange(0., 30.);
  stack -> SetXaxisTitle("#rho_{PU}^{0 < |#eta| <5.0}");
  stack -> SetUnit("GeV/#Delta#eta#Delta#phi");
  stack -> Draw(variableNames, histoName, method, lumi, step, 60, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "rhoForIsolation";
  histoName    = "PU_rhoForIsolation";
  stack -> SetXaxisRange(0., 30.);
  stack -> SetXaxisTitle("#rho_{PU}^{0 < |#eta| < 2.5}");
  stack -> SetUnit("GeV/#Delta#eta#Delta#phi");
  stack -> Draw(variableNames, histoName, method, lumi, step, 60, PURescale, weightEvent, stackSig);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // lepton
  //---------------------------------------------
  
  variableNames.at(0) = "lep.pt()";
  histoName = "lep_pt";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("p_{T}^{lepton}");
  stack -> SetUnit("GeV/c");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "lep.phi()";
  histoName = "lep_phi";
  stack -> SetXaxisRange(-3.14159, 3.14159);
  stack -> SetXaxisTitle("#phi_{lepton}");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi, step, 36, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "lep.eta()";
  histoName = "lep_eta";
  stack -> SetXaxisRange(-2.5, 2.5);
  stack -> SetXaxisTitle("#eta_{lepton}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "abs(lep.eta())";
  histoName = "lep_absEta";
  stack -> SetXaxisRange(0., 2.5);
  stack -> SetXaxisTitle("|#eta|_{lepton}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 25, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "abs(lep_dB)/lep_edB";
  histoName = "lep_3DipSig";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("3DIP_{lepton}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig);
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // Met
  //---------------------------------------------
  
  variableNames.at(0) = "met.Et()";  
  histoName = "met_et";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("ME_{T}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi, step, 60, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "met.phi()";  
  histoName = "met_phi";
  stack -> SetXaxisRange(-3.14159, 3.14159);
  stack -> SetXaxisTitle("#phi_{ME_{T}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 36, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "lepMet_pt";
  histoName = "lepMet_pt";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("p_{T}^{lepton+ME_{T}}");
  stack -> SetUnit("GeV/c");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "lepNu_m";
  histoName = "lepNu_m";
  stack -> SetXaxisRange(0., 150.);
  stack -> SetXaxisTitle("m_{lepton+neutrino}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "lepMet_mt";
  histoName = "lepMet_mt";
  stack -> SetXaxisRange(0., 150.);
  stack -> SetXaxisTitle("m_{T}^{lepton+ME_{T}}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "lepMet_Dphi";
  histoName = "lepMet_Dphi";
  stack -> SetXaxisRange(0., 3.14159);
  stack -> SetXaxisTitle("#Delta#phi_{lepton-ME_{T}}");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi, step, 36, PURescale, weightEvent, stackSig);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // electron
  //---------------------------------------------
  
  variableNames.at(0) = "lep_pt";
  histoName = "ele_pt";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("p_{T}^{electron}");
  stack -> SetUnit("GeV/c");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep.eta()";
  histoName = "ele_eta";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(-2.5, 2.5);
  stack -> SetXaxisTitle("#eta_{electron}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep.phi()";
  histoName = "ele_phi";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(-3.14159, 3.14159);
  stack -> SetXaxisTitle("#phi_{electron}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 36, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "abs(lep_dB)/lep_edB";
  histoName = "ele_3DipSig";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("3DIP_{electron}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig, cuts);
  
  /*
  variableNames.at(0) = "lep_sigmaIetaIeta";
  histoName = "ele_sigmaIetaIeta_EB";
  cuts -> at(0) = "( lep_isEB == 1 && lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 0.030);
  stack -> SetXaxisTitle("EB #sigma_{I#etaI#eta}(electron)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep_sigmaIetaIeta";
  histoName = "ele_sigmaIetaIeta_EE";
  cuts -> at(0) = "( lep_isEB == 0 && lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 0.030);
  stack -> SetXaxisTitle("EE #sigma_{I#etaI#eta}(electron)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig, cuts);
  
  
  
  variableNames.at(0) = "lep_DphiIn";
  histoName = "ele_DphiIn_EB";
  cuts -> at(0) = "( lep_isEB == 1 && lep_flavour == 11 )";
  stack -> SetXaxisRange(-0.040, 0.040);
  stack -> SetXaxisTitle("EB #Delta#phi_{in}(electron)");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep_DphiIn";
  histoName = "ele_DphiIn_EE";
  cuts -> at(0) = "( lep_isEB == 0 && lep_flavour == 11 )";
  stack -> SetXaxisRange(-0.040, 0.040);
  stack -> SetXaxisTitle("EE #Delta#phi_{in}(electron)");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, weightEvent, stackSig, cuts);
  
  
  
  variableNames.at(0) = "lep_DetaIn";
  histoName = "ele_DetaIn_EB";
  cuts -> at(0) = "( lep_isEB == 1 && lep_flavour == 11 )";
  stack -> SetXaxisRange(-0.010, 0.010);
  stack -> SetXaxisTitle("EB #Delta#eta_{in}(electron)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep_DetaIn";
  histoName = "ele_DetaIn_EE";
  cuts -> at(0) = "( lep_isEB == 0 && lep_flavour == 11 )";
  stack -> SetXaxisRange(-0.010, 0.010);
  stack -> SetXaxisTitle("EE #Delta#eta_{in}(electron)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig, cuts);
  
  
  
  variableNames.at(0) = "lep_HOverE";
  histoName = "ele_HOverE_EB";
  cuts -> at(0) = "( lep_isEB == 1 && lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 0.050);
  stack -> SetXaxisTitle("EB H/E(electron)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep_HOverE";
  histoName = "ele_HOverE_EE";
  cuts -> at(0) = "( lep_isEB == 0 && lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 0.050);
  stack -> SetXaxisTitle("EE H/E(electron)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig, cuts);
  
  
  
  
  
  
  variableNames.at(0) = "lep_dB";
  histoName = "ele_dB_EB";
  cuts -> at(0) = "( lep_isEB == 1 && lep_flavour == 11 )";
  stack -> SetXaxisRange(-0.03, 0.03);
  stack -> SetXaxisTitle("EB d_{xy}(electron)");
  stack -> SetUnit("cm");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep_dB";
  histoName = "ele_dB_EE";
  cuts -> at(0) = "( lep_isEB == 0 && lep_flavour == 11 )";
  stack -> SetXaxisRange(-0.03, 0.03);
  stack -> SetXaxisTitle("EE d_{xy}(electron)");
  stack -> SetUnit("cm");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig, cuts);
  
  
  
  variableNames.at(0) = "lep_dz_PV";
  histoName = "ele_dz_EB";
  cuts -> at(0) = "( lep_isEB == 1 && lep_flavour == 11 )";
  stack -> SetXaxisRange(-0.03, 0.03);
  stack -> SetXaxisTitle("EB d_{z}(electron)");
  stack -> SetUnit("cm");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep_dz_PV";
  histoName = "ele_dz_EE";
  cuts -> at(0) = "( lep_isEB == 0 && lep_flavour == 11 )";
  stack -> SetXaxisRange(-0.03, 0.03);
  stack -> SetXaxisTitle("EE d_{z}(electron)");
  stack -> SetUnit("cm");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig, cuts);
  */
  
  
  
  
  
  variableNames.at(0) = "lep_tkIso/lep.pt()";
  histoName = "ele_tkRelIso";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 0.2);
  stack -> SetXaxisTitle("#Sigma p_{T}^{trk} / p_{T}^{electron}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep_emIso/lep.pt()";
  histoName = "ele_emRelIso";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 0.2);
  stack -> SetXaxisTitle("#Sigma E_{T}^{em} / p_{T}^{electron}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep_hadIso/lep.pt()";
  histoName = "ele_hadRelIso";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 0.2);
  stack -> SetXaxisTitle("#Sigma E_{T}^{had} / p_{T}^{electron}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "(lep_tkIso+max(0.,lep_emIso)+lep_hadIso)/lep.pt()";
  histoName = "ele_combRelIso";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 0.3);
  stack -> SetXaxisTitle("#Sigma (p_{T}^{trk}+max(0.,E_{T}*{em})+E_{T}^{had}) / p_{T}^{electron}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "(lep_tkIso+max(0.,lep_emIso)+lep_hadIso-rhoForIsolation*3.14159*0.3*0.3)/lep.pt()";
  histoName = "ele_combRelIso_PUCorr";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(-0.3, 0.15);
  stack -> SetXaxisTitle("#Sigma (p_{T}^{trk}+max(0.,E_{T}^{em})+E_{T}^{had} - #rho_{PU}#pi#DeltaR^{2} ) / p_{T}^{electron}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 45, PURescale, weightEvent, stackSig, cuts);
  
  
  
  
  
  
  variableNames.at(0) = "met.Et()";
  histoName = "ele_met_et";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("ME_{T}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi, step, 60, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "met.phi()";  
  histoName = "ele_met_phi";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(-3.14159, 3.14159);
  stack -> SetXaxisTitle("#phi_{ME_{T}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 36, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lepMet_mt";
  histoName = "ele_lepMet_mt";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 150.);
  stack -> SetXaxisTitle("m_{T}^{lepton+ME_{T}}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig, cuts);
  
  
  
  
  
  
  variableNames2.at(0) = "WJ1.pt()";
  variableNames2.at(1) = "WJ2.pt()";
  histoName = "ele_WJ1_pt+WJ2_pt";
  cuts2 -> at(0) = "( lep_flavour == 11 )";
  cuts2 -> at(1) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("p_{T}^{jet_{1},jet_{2}}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames2, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig, cuts2);
  
  variableNames.at(0) = "max(WJ1.pt(), WJ2.pt())";
  histoName = "ele_WJ1_pt";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("max p_{T}^{jet_{1},jet_{2}}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "min(WJ1.pt(), WJ2.pt())";
  histoName = "ele_WJ2_pt";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 150.);
  stack -> SetXaxisTitle("min p_{T}^{jet_{1},jet_{2}}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig, cuts);
  
  
    
  variableNames2.at(0) = "WJ1.eta()";
  variableNames2.at(1) = "WJ1.eta()";
  histoName = "ele_WJ1_eta+WJ2_eta";
  cuts2 -> at(0) = "( lep_flavour == 11 )";
  cuts2 -> at(1) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta_{jet_{1},jet_{2}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames2, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig, cuts2);
  
  variableNames.at(0) = "max(abs(WJ1.eta()), abs(WJ2.eta()))";
  histoName = "ele_WJ1_absEta";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 2.5);
  stack -> SetXaxisTitle("max |#eta|_{jet_{1},jet_{2}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 25, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "min(abs(WJ1.eta()), abs(WJ2.eta()))";
  histoName = "ele_WJ2_absEta";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 2.5);
  stack -> SetXaxisTitle("min |#eta|_{jet_{1},jet_{2}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 25, PURescale, weightEvent, stackSig, cuts);
  
  
  
  
  
  
  variableNames.at(0) = "WJJ_m";
  histoName = "ele_WJJ_m";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("m_{jj}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumi, step, 60, PURescale, weightEvent, stackSig, cuts);

  variableNames.at(0) = "WJJ_pt";
  histoName = "ele_WJJ_pt";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("p_{T}^{jj}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "WJJ_eta";
  histoName = "ele_WJJ_eta";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta^{jj}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig, cuts);

  variableNames.at(0) = "WJJ_Deta";
  histoName = "ele_WJJ_Deta";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("#Delta#eta_{jj}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig, cuts);

  variableNames.at(0) = "WJJ_Dphi";
  histoName = "ele_WJJ_Dphi";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(0., 3.14159);
  stack -> SetXaxisTitle("#Delta#phi_{jj}");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi, step, 36, PURescale, weightEvent, stackSig, cuts);
  
  
  
  
  
  
  
  variableNames.at(0) = "lepNuW_eta";
  histoName = "ele_lepNuW_eta";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta_{l#nujj}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lepNuW_m";
  histoName = "ele_lepNuW_m";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(100., 800);
  stack -> SetXaxisTitle("m_{l#nujj}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumi, step, 70, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lepNuW_m_KF";
  histoName = "ele_lepNuW_m_KF";
  cuts -> at(0) = "( lep_flavour == 11 )";
  stack -> SetXaxisRange(100., 800);
  stack -> SetXaxisTitle("m_{l#nujj} after KF");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumi, step, 70, PURescale, weightEvent, stackSig, cuts);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // muon
  //---------------------------------------------
  
  variableNames.at(0) = "lep_pt";
  histoName = "mu_pt";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("p_{T}^{muon}");
  stack -> SetUnit("GeV/c");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep.eta()";
  histoName = "mu_eta";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(-2.5, 2.5);
  stack -> SetXaxisTitle("#eta_{muon}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep.phi()";
  histoName = "mu_phi";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(-3.14159, 3.14159);
  stack -> SetXaxisTitle("#phi_{muon}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 36, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "abs(lep_dB)/lep_edB";
  histoName = "mu_3DipSig";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("3DIP_{muon}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig, cuts);
  
  /*
  variableNames.at(0) = "lep_dB";
  histoName = "mu_dB";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(-0.03, 0.03);
  stack -> SetXaxisTitle("d_{xy}(muon)");
  stack -> SetUnit("cm");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep_dz_PV";
  histoName = "mu_dz";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(-0.03, 0.03);
  stack -> SetXaxisTitle("d_{z}(muon)");
  stack -> SetUnit("cm");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "abs(lep_dB)/lep_edB";
  histoName = "mu_3DipSig";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("3D ip(muon)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 100, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep_normalizedChi2";
  histoName = "mu_normalizedChi2";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("#chi^{2}/ndof(muon)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep_numberOfValidTrackerHits";
  histoName = "mu_numberOfValidTrackerHits";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 40.);
  stack -> SetXaxisTitle("nHits_{tracker}(muon)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 20, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep_numberOfValidMuonHits";  
  histoName = "mu_numberOfValidMuonHits";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 60.);
  stack -> SetXaxisTitle("nHits(muon)");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig, cuts);
  */
  
  
  
  
  
  variableNames.at(0) = "lep_tkIso";
  histoName = "mu_tkIso";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 4.);
  stack -> SetXaxisTitle("#Sigma p_{T}^{trk}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, weightEvent, stackSig, cuts);
   
  variableNames.at(0) = "lep_emIso";
  histoName = "mu_emIso";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 4.);
  stack -> SetXaxisTitle("#Sigma E_{T}^{em}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep_hadIso";
  histoName = "mu_hadIso";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 4.);
  stack -> SetXaxisTitle("#Sigma E_{T}^{had}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, weightEvent, stackSig, cuts);
  
  
  
  variableNames.at(0) = "lep_tkIso/lep.pt()";
  histoName = "mu_tkRelIso";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 0.2);
  stack -> SetXaxisTitle("#Sigma p_{T}^{trk} / p_{T}^{muon}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, weightEvent, stackSig, cuts);
   
  variableNames.at(0) = "lep_emIso/lep.pt()";
  histoName = "mu_emRelIso";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 0.2);
  stack -> SetXaxisTitle("#Sigma E_{T}^{em} / p_{T}^{muon}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lep_hadIso/lep.pt()";
  histoName = "mu_hadRelIso";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 0.2);
  stack -> SetXaxisTitle("#Sigma E_{T}^{had} / p_{T}^{muon}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "(lep_tkIso+lep_emIso+lep_hadIso)/lep.pt()";
  histoName = "mu_combRelIso";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 0.3);
  stack -> SetXaxisTitle("#Sigma (p_{T}^{trk}+E_{T}^{em}+E_{T}^{had}) / p_{T}^{muon}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "(lep_tkIso+lep_emIso+lep_hadIso-rhoForIsolation*3.14159*0.3*0.3)/lep.pt()";
  histoName = "mu_combRelIso_PUCorr";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(-0.3, 0.15);
  stack -> SetXaxisTitle("#Sigma (p_{T}^{trk}+E_{T}^{em}+E_{T}^{had}-#rho_{PU}#pi#DeltaR^{2}) / p_{T}^{muon}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 45, PURescale, weightEvent, stackSig, cuts);
  
  
  
  
  
  
  variableNames.at(0) = "met.Et()";
  histoName = "mu_met_et";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("ME_{T}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi, step, 60, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "met.phi()";  
  histoName = "mu_met_phi";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(-3.14159, 3.14159);
  stack -> SetXaxisTitle("#phi_{ME_{T}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 36, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lepMet_mt";
  histoName = "mu_lepMet_mt";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 150.);
  stack -> SetXaxisTitle("m_{T}^{lepton+ME_{T}}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig, cuts);
  
  
  
  
  
  
  variableNames2.at(0) = "WJ1.pt()";
  variableNames2.at(1) = "WJ2.pt()";
  histoName = "mu_WJ1_pt+WJ2_pt";
  cuts2 -> at(0) = "( lep_flavour == 13 )";
  cuts2 -> at(1) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("p_{T}^{jet_{1},jet_{2}}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames2, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig, cuts2);
  
  variableNames.at(0) = "max(WJ1.pt(), WJ2.pt())";
  histoName = "mu_WJ1_pt";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("max p_{T}^{jet_{1},jet_{2}}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "min(WJ1.pt(), WJ2.pt())";
  histoName = "mu_WJ2_pt";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 150.);
  stack -> SetXaxisTitle("min p_{T}^{jet_{1},jet_{2}}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig, cuts);
  
  
    
  variableNames2.at(0) = "WJ1.eta()";
  variableNames2.at(1) = "WJ1.eta()";
  histoName = "mu_WJ1_eta+WJ2_eta";
  cuts2 -> at(0) = "( lep_flavour == 13 )";
  cuts2 -> at(1) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta_{jet_{1},jet_{2}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames2, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig, cuts2);
  
  variableNames.at(0) = "max(abs(WJ1.eta()), abs(WJ2.eta()))";
  histoName = "mu_WJ1_absEta";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 2.5);
  stack -> SetXaxisTitle("max |#eta|_{jet_{1},jet_{2}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 25, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "min(abs(WJ1.eta()), abs(WJ2.eta()))";
  histoName = "mu_WJ2_absEta";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 2.5);
  stack -> SetXaxisTitle("min |#eta|_{jet_{1},jet_{2}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 25, PURescale, weightEvent, stackSig, cuts);  
  
  
  
  
  
  
  variableNames.at(0) = "WJJ_m";
  histoName = "mu_WJJ_m";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("m_{jj}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumi, step, 60, PURescale, weightEvent, stackSig, cuts);

  variableNames.at(0) = "WJJ_pt";
  histoName = "mu_WJJ_pt";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("p_{T}^{jj}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "WJJ_eta";
  histoName = "mu_WJJ_eta";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta^{jj}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig, cuts);

  variableNames.at(0) = "WJJ_Deta";
  histoName = "mu_WJJ_Deta";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("#Delta#eta_{jj}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig, cuts);

  variableNames.at(0) = "WJJ_Dphi";
  histoName = "mu_WJJ_Dphi";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(0., 3.14159);
  stack -> SetXaxisTitle("#Delta#phi_{jj}");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi, step, 36, PURescale, weightEvent, stackSig, cuts); 
  
  
  
  
  
  
  variableNames.at(0) = "lepNuW_eta";
  histoName = "mu_lepNuW_eta";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta_{l#nujj}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lepNuW_m";
  histoName = "mu_lepNuW_m";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(100., 800);
  stack -> SetXaxisTitle("m_{l#nujj}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumi, step, 70, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "lepNuW_m_KF";
  histoName = "mu_lepNuW_m_KF";
  cuts -> at(0) = "( lep_flavour == 13 )";
  stack -> SetXaxisRange(100., 800);
  stack -> SetXaxisTitle("m_{l#nujj} after KF");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumi, step, 70, PURescale, weightEvent, stackSig, cuts);  
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------  
  // Jets
  //---------------------------------------------
  
  variableNames.at(0) = "nJets";
  histoName = "jets_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("N_{jets}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 10, PURescale, weightEvent, stackSig);

  variableNames.at(0) = "nJets_cnt";
  histoName = "jets_cnt_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("N_{central jets}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 10, PURescale, weightEvent, stackSig);

  variableNames.at(0) = "nJets_fwd";
  histoName = "jets_fwd_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> SetXaxisTitle("N_{forward jets}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 10, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "max(WJ1_bTag,WJ2_bTag)";
  histoName = "WJets_bTag1_2j";
  cuts -> at(0) = "nJets_cnt_pt30 == 2";
  stack -> SetXaxisRange(-5., 10.);
  stack -> SetXaxisTitle("max bTag_{jet_{1},jet_{2}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 60, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "min(WJ1_bTag,WJ2_bTag)";
  histoName = "WJets_bTag2_2j";
  cuts -> at(0) = "nJets_cnt_pt30 == 2";
  stack -> SetXaxisRange(-5., 10.);
  stack -> SetXaxisTitle("min bTag_{jet_{1},jet_{2}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 60, PURescale, weightEvent, stackSig, cuts);  
  
  variableNames.at(0) = "max(WJ1_bTag,WJ2_bTag)";
  histoName = "WJets_bTag1_3j";
  cuts -> at(0) = "nJets_cnt_pt30 == 3";
  stack -> SetXaxisRange(-5., 10.);
  stack -> SetXaxisTitle("max bTag_{jet_{1},jet_{2}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 60, PURescale, weightEvent, stackSig, cuts);
  
  variableNames.at(0) = "min(WJ1_bTag,WJ2_bTag)";
  histoName = "WJets_bTag2_3j";
  cuts -> at(0) = "nJets_cnt_pt30 == 3";
  stack -> SetXaxisRange(-5., 10.);
  stack -> SetXaxisTitle("min bTag_{jet_{1},jet_{2}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 60, PURescale, weightEvent, stackSig, cuts);  
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------
  // W-Jets
  //---------------------------------------------
  
  variableNames.at(0) = "WJJ_m";
  histoName = "WJJ_m";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("m_{jj}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumi, step, 60, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "WJJ_pt";
  histoName = "WJJ_pt";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("p_{T}^{jj}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig);  
  
  variableNames.at(0) = "WJJ_eta";
  histoName = "WJJ_eta";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta^{jj}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig);  
  
  variableNames.at(0) = "WJJ_Deta";
  histoName = "WJJ_Deta";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("#Delta#eta_{jj}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig);

  variableNames.at(0) = "WJJ_Dphi";
  histoName = "WJJ_Dphi";
  stack -> SetXaxisRange(0., 3.14159);
  stack -> SetXaxisTitle("#Delta#phi_{jj}");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi, step, 36, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "WJJ_DR";
  histoName = "WJJ_DR";
  stack -> SetXaxisRange(0., 5.);
  stack -> SetXaxisTitle("#DeltaR^{jj}");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig);  
  
  variableNames.at(0) = "WJ1_lep_Dphi";
  histoName = "WJ1_lep_Dphi";
  stack -> SetXaxisRange(0., 3.14159);
  stack -> SetXaxisTitle("#Delta#phi_{lepton-jet_{1}}");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi, step, 36, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "WJ1_met_Dphi";
  histoName = "WJ1_met_Dphi";
  stack -> SetXaxisRange(0., 3.14159);
  stack -> SetXaxisTitle("#Delta#phi_{ME_{T}-jet_{1}}");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi, step, 36, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "WJ2_lep_Dphi";
  histoName = "WJ2_lep_Dphi";
  stack -> SetXaxisRange(0., 3.14159);
  stack -> SetXaxisTitle("#Delta#phi_{lepton-jet_{2}}");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi, step, 36, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "WJ2_met_Dphi";
  histoName = "WJ2_met_Dphi";
  stack -> SetXaxisRange(0., 3.14159);
  stack -> SetXaxisTitle("#Delta#phi_{ME_{T}-jet_{2}}");
  stack -> SetUnit("rad");
  stack -> Draw(variableNames, histoName, method, lumi, step, 36, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "min(WJ1.pt(),WJ2.pt())/WJJ_m";
  histoName = "WJ2_pt_over_WJJ_m";
  stack -> SetXaxisRange(0., 2.);
  stack -> SetXaxisTitle("p_{T}^{jet_{2}}/m_{jj}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig);
  
  
  
  variableNames2.at(0) = "WJ1.pt()";
  variableNames2.at(1) = "WJ2.pt()";
  histoName = "WJ1_pt+WJ2_pt";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("p_{T}^{jet_{1},jet_{2}}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames2, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "max(WJ1.pt(), WJ2.pt())";
  histoName = "WJ1_pt";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("max p_{T}^{jet_{1},jet_{2}}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "min(WJ1.pt(), WJ2.pt())";
  histoName = "WJ2_pt";
  stack -> SetXaxisRange(0., 150.);
  stack -> SetXaxisTitle("min p_{T}^{jet_{1},jet_{2}}");
  stack -> SetUnit("GeV");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig);
  
  
    
  variableNames2.at(0) = "WJ1.eta()";
  variableNames2.at(1) = "WJ1.eta()";
  histoName = "WJ1_eta+WJ2_eta";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta_{jet_{1},jet_{2}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames2, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "max(abs(WJ1.eta()), abs(WJ2.eta()))";
  histoName = "WJ1_absEta";
  stack -> SetXaxisRange(0., 2.5);
  stack -> SetXaxisTitle("max |#eta|_{jet_{1},jet_{2}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 25, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "min(abs(WJ1.eta()), abs(WJ2.eta()))";
  histoName = "WJ2_absEta";
  stack -> SetXaxisRange(0., 2.5);
  stack -> SetXaxisTitle("min |#eta|_{jet_{1},jet_{2}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 25, PURescale, weightEvent, stackSig);  
  
  
  
  variableNames.at(0) = "WJ1_bTag+WJ2_bTag";
  histoName = "WJ1_bTag+WJ2_bTag";
  stack -> SetXaxisRange(-5., 10.);
  stack -> SetXaxisTitle("bTag^{jet_{1}}+bTag^{jet_{2}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 60, PURescale, weightEvent, stackSig);
  
  
  
  variableNames.at(0) = "WJ1_QGLikelihood";
  histoName = "WJ1_QGLikelihood";
  stack -> SetXaxisRange(0., 1.);
  stack -> SetXaxisTitle("quark-gluon likelihood_{jet_{1}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig);
    
  variableNames.at(0) = "WJ2_QGLikelihood";
  histoName = "WJ2_QGLikelihood";
  stack -> SetXaxisRange(0., 1.);
  stack -> SetXaxisTitle("quark-gluon likelihood_{jet_{2}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig);
    
  variableNames.at(0) = "WJ1_QGLikelihood * WJ2_QGLikelihood";
  histoName = "WJJ_QGLikelihood";
  stack -> SetXaxisRange(0., 1.);
  stack -> SetXaxisTitle("quark-gluon likelihood_{jet_{1}} * likelihood_{jet_{2}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig);    
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------
  // Ordered pt
  //---------------------------------------------
  
  variableNames.at(0) = "lepWJJ_pt1";
  histoName = "lepWJJ_pt1";
  stack -> SetXaxisRange(0., 300.);
  stack -> SetXaxisTitle("1^{st} p_{T}(lepton,jet_{1},jet_{2})");
  stack -> SetUnit("GeV/c");
  stack -> Draw(variableNames, histoName, method, lumi, step, 30, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "lepWJJ_pt2";
  histoName = "lepWJJ_pt2";
  stack -> SetXaxisRange(0., 200.);
  stack -> SetXaxisTitle("2^{nd} p_{T}(lepton,jet_{1},jet_{2})");
  stack -> SetUnit("GeV/c");
  stack -> Draw(variableNames, histoName, method, lumi, step, 20, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "lepWJJ_pt3";
  histoName = "lepWJJ_pt3";
  stack -> SetXaxisRange(0., 100.);
  stack -> SetXaxisTitle("3^{rd} p_{T}(lepton,jet_{1},jet_{2})");
  stack -> SetUnit("GeV/c");
  stack -> Draw(variableNames, histoName, method, lumi, step, 20, PURescale, weightEvent, stackSig);
  
  
  
  
  
  
  
  
  
  
  
  
  //---------------------------------------------
  // Higgs
  //---------------------------------------------
    
  variableNames.at(0) = "lepNuW_eta";
  histoName = "lepNuW_eta";
  stack -> SetXaxisRange(-5., 5.);
  stack -> SetXaxisTitle("#eta_{l#nujj}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 50, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "lepNuW_m";
  histoName = "lepNuW_m";
  stack -> SetXaxisRange(100., 800);
  stack -> SetXaxisTitle("m_{l#nujj}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumi, step, 70, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "lepNuW_m_KF";
  histoName = "lepNuW_m_KF";
  stack -> SetXaxisRange(100., 800);
  stack -> SetXaxisTitle("m_{l#nujj} after KF");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumi, step, 70, PURescale, weightEvent, stackSig);
  
  //variableNames.at(0) = "lepMetW_pt/lepNuW_m";
  //histoName = "lepMetW_jacobian";
  //stack -> SetXaxisRange(0., 1.);
  //stack -> SetXaxisTitle("p_{T}^{lepton+ME_{T}+jet_{1}+jet_{2})/m_{l#nujj}");
  //stack -> SetUnit("");
  //stack -> Draw(variableNames, histoName, method, lumi, step, 25, PURescale, weightEvent, stackSig);
  
  //variableNames.at(0) = "lepW_pt";
  //histoName = "lepW_pt";
  //stack -> SetXaxisRange(0., 200);
  //stack -> SetXaxisTitle("p_{T}^{lepton+jet_{1}+jet_{2})");
  //stack -> SetUnit("GeV/c");
  //stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "lepMetW_pt";
  histoName = "lepMetW_pt";
  stack -> SetXaxisRange(0., 200);
  stack -> SetXaxisTitle("p_{T}^{lepton+ME_{T}+jet_{1}+jet_{2}}");
  stack -> SetUnit("GeV/c");
  stack -> Draw(variableNames, histoName, method, lumi, step, 40, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "lepMetW_mt";
  histoName = "lepMetW_mt";
  stack -> SetXaxisRange(100., 800.);
  stack -> SetXaxisTitle("m_{T}^{lepton+ME_{T}+jet_{1}+jet_{2}}");
  stack -> SetUnit("GeV/c^{2}");
  stack -> Draw(variableNames, histoName, method, lumi, step, 35, PURescale, weightEvent, stackSig);
  
  variableNames.at(0) = "lepMetW_Dphi";
  histoName = "lepMetW_Dphi";
  stack -> SetXaxisRange(0., 3.14159);
  stack -> SetXaxisTitle("#Delta#phi_{lepton+ME_{T}-jet_{1}+jet_{2}}");
  stack -> SetUnit("");
  stack -> Draw(variableNames, histoName, method, lumi, step, 36, PURescale, weightEvent, stackSig);
  
  
  
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
