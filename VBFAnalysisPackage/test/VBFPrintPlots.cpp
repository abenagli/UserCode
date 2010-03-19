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
    std::cerr << ">>>>> VBFPrintPlots::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
  
  std::string inputDir = gConfigParser -> readStringOption("Input::inputDir");
  std::string jetAlgorithm  = gConfigParser -> readStringOption("Input::jetAlgorithm");

  std::string outputDir = gConfigParser -> readStringOption("Output::outputDir");
  
  //float lumi = gConfigParser -> readFloatOption("Options::lumi");
  
  
  
  
  
  // draw plots
  drawTStack* stack = new drawTStack(inputDir, "listOfSamplesAndCrossSections.txt", "VBFHiggsToWWTolnujjAnalysis", jetAlgorithm, outputDir);
  
  std::string histoName;
  int step = 2;
  
  
  
  // tag jets
  histoName = "tagJJ_Deta";
  stack -> SetXaxisRange(0., 10.);
  stack -> Draw(histoName, step, 5, true);

  histoName = "tagJJ_Dphi";
  stack -> SetXaxisRange(0., 3.);
  stack -> Draw(histoName, step, 5, true);  

  histoName = "tagJJ_max_et";
  stack -> SetXaxisRange(0., 500.);
  stack -> Draw(histoName, step, 5, true);  

  histoName = "tagJJ_min_et";
  stack -> SetXaxisRange(0., 250.);
  stack -> Draw(histoName, step, 5, true);

  histoName = "tagJJ_et";
  stack -> SetXaxisRange(0., 500.);
  stack -> Draw(histoName, step, 5, true);

  histoName = "tagJJ_m";
  stack -> SetXaxisRange(0., 3000.);
  stack -> Draw(histoName, step, 50, true);

  histoName = "tagJJ_bTag";
  stack -> SetXaxisRange(-50., 50.);
  stack -> Draw(histoName, step, 10, true);
  
  //c1 = drawPlot(inputDir,"h_7_W_tag_Dphi",
  //             5, 1., 
  //             0., 3.15,
  //             0.001, 1., true,
  //             "W_tag - Dphi", "event fraction");
  //c1 -> Print((outputDir+"/W_tag_Dphi.png").c_str(), "png");
  //delete c1;
  //Close(inRootFiles);

  
  
  
  
  
  // W jets
  histoName = "WJJ_Deta";
  stack -> SetXaxisRange(0., 5.);
  stack -> Draw(histoName, step, 5, true);
  
  histoName = "WJJ_Dphi";
  stack -> SetXaxisRange(0., 3.);
  stack -> Draw(histoName, step, 5, true);

  histoName = "WJJ_max_et";
  stack -> SetXaxisRange(0., 300.);
  stack -> Draw(histoName, step, 5, true);  
  
  histoName = "WJJ_min_et";
  stack -> SetXaxisRange(0., 150.);
  stack -> Draw(histoName, step, 5, true);
  
  histoName = "WJJ_et";
  stack -> SetXaxisRange(0., 300.);
  stack -> Draw(histoName, step, 5, true);

  histoName = "WJJ_m";
  stack -> SetXaxisRange(0., 200.);
  stack -> Draw(histoName, step, 5, false);
  
  histoName = "WJJ_zepp";
  stack -> SetXaxisRange(-1., 1.);
  stack -> Draw(histoName, step, 50, true);
  
  histoName = "WJJ_max_zepp";
  stack -> SetXaxisRange(-1., 1.);
  stack -> Draw(histoName, step, 50, true);
  
  histoName = "WJJ_min_zepp";
  stack -> SetXaxisRange(-1., 1.);
  stack -> Draw(histoName, step, 50, true);  
  
  histoName = "WJJ_bTag";
  stack -> SetXaxisRange(-50., 50.);
  stack -> Draw(histoName, step, 10, true);
  
  
  
  
  
  // other jets
  histoName = "otherJ_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> Draw(histoName, step, 1, true);
  
  histoName = "otherJ_et";
  stack -> SetXaxisRange(0., 250.);
  stack -> Draw(histoName, step, 5, true);  
  
  //c1 = drawPlot(inputDir,"h_7_otherJet_zepp",
  //              10, 1., 
  //              -1., 1.,
  //              0.0001, 1., true,
  //              "otherJet - zepp", "event fraction");
  //c1 -> Print((outputDir+"/otherJet_zepp.png").c_str(), "png");
  //delete c1;
  //Close(inRootFiles);
  
  
  
  
  
  
  // central jets
  histoName = "centralJ_n";
  stack -> SetXaxisRange(0., 10.);
  stack -> Draw(histoName, step, 1, true);
  
  histoName = "centralJ_et";
  stack -> SetXaxisRange(0., 250.);
  stack -> Draw(histoName, step, 5, true);  
  
  //c1 = drawPlot(inputDir,"h_7_otherJet_zepp",
  //              10, 1., 
  //              -1., 1.,
  //              0.0001, 1., true,
  //              "otherJet - zepp", "event fraction");
  //c1 -> Print((outputDir+"/otherJet_zepp.png").c_str(), "png");
  //delete c1;
  //Close(inRootFiles);
  
  
  
  
  
  
  // lepton
  histoName = "lep_pt";
  stack -> SetXaxisRange(0., 250.);
  stack -> Draw(histoName, step, 5, true);
  
  histoName = "lep_zepp";
  stack -> SetXaxisRange(-1., 1.);
  stack -> Draw(histoName, step, 50, true);  
  
  histoName = "lep_lipSig";
  stack -> SetXaxisRange(-50., 50.);
  stack -> Draw(histoName, step, 50, true);
  
  histoName = "lep_tipSig";
  stack -> SetXaxisRange(0., 50.);
  stack -> Draw(histoName, step, 50, true);
  
  histoName = "lep_3DipSig";
  stack -> SetXaxisRange(0., 50.);
  stack -> Draw(histoName, step, 50, true);
  
  
  
  
  
  
  // met
  histoName = "met_et";
  stack -> SetXaxisRange(0., 500.);
  stack -> Draw(histoName, step, 10, true);
  
  
  
  
  
  
  // lepton + met
  histoName = "lepMet_mt";
  stack -> SetXaxisRange(0., 500.);
  stack -> Draw(histoName, step, 10, true);
  
  histoName = "lepMetW_DR";
  stack -> SetXaxisRange(0., 10.);
  stack -> Draw(histoName, step, 10, true);
  
  histoName = "lepMetW_Deta";
  stack -> SetXaxisRange(0., 10.);
  stack -> Draw(histoName, step, 10, true);
  
  histoName = "lepMetW_Dphi";
  stack -> SetXaxisRange(0., 3.);
  stack -> Draw(histoName, step, 10, true);
  
  
  
  
  
  
  // lepton + W + met
  histoName = "lepMetW_mt";
  stack -> SetXaxisRange(0., 500.);
  stack -> Draw(histoName, step, 10, true);
  
  histoName = "lepMetW_pt";
  stack -> SetXaxisRange(0., 500.);
  stack -> Draw(histoName, step, 10, true);
  
  //c1 = drawPlot(inputDir,"h_7_lep_tag_Dphi",
  //              5, 1., 
  //              0., 3.15,
  //              0.01, 0.1, true,
  //              "lep_tag - Dphi", "event fraction");
  //c1 -> Print((outputDir+"/lep_tag_Dphi.png").c_str(), "png");
  //delete c1;
  //Close(inRootFiles);

  //c1 = drawPlot(inputDir,"h_7_met_tag_Dphi",
  //              5, 1., 
  //              0., 3.15,
  //              0.01, 0.2, true,
  //              "met_tag - Dphi", "event fraction");
  //c1 -> Print((outputDir+"/met_tag_Dphi.png").c_str(), "png");
  //delete c1;
  //Close(inRootFiles);

  //c1 = drawPlot(inputDir,"h_7_lepMetW_tag_Dphi",
  //              2, 1., 
  //              0., 3.15,
  //              0.01, 0.2, true,
  //              "lepMetW_tag - Dphi", "event fraction");
  //c1 -> Print((outputDir+"/lepMetW_tag_Dphi.png").c_str(), "png");
  //delete c1;
  //Close(inRootFiles);
}
