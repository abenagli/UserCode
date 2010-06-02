#include "readReducedTree.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"

#include "TGraph.h"
#include "TF1.h"

#include <iostream>
#include <vector>






int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> VBFIsoSignificancePlot::usage: " << argv[0] << " configFileName" << std::endl;
    return 1;
  }
  
  
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
  
  std::string inputFileName = gConfigParser -> readStringOption("Input::inputFileName");
  
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");
  
  
  
  // Create outFile
  std::string outputRootFullFileName = outputRootFilePath+outputRootFileName+".root";
  TFile* outFile = new TFile(outputRootFullFileName.c_str(), "RECREATE");
  outFile -> cd();
  
  
  
  
  
  
  //****************
  // CUT-BASED
  //****************
  
  // define set of cuts
  std::vector<cuts*> myCuts;
  unsigned int cutIt = 0;
  for(float tagJJ_DetaMIN = 2.; tagJJ_DetaMIN <= 7.; tagJJ_DetaMIN+=0.25)
  for(float tagJJ_mMIN = 200.; tagJJ_mMIN <= 1000.; tagJJ_mMIN+=25.)
  for(float lepMetW_DphiMAX = 0.1; lepMetW_DphiMAX <= 3.14; lepMetW_DphiMAX+=0.1)
  {
    myCuts.push_back(new cuts());
    initializeCuts(*(myCuts.at(cutIt)));
    
    (myCuts.at(cutIt))->tagJJ_DetaMIN = tagJJ_DetaMIN;
    (myCuts.at(cutIt))->tagJJ_mMIN = tagJJ_mMIN;
    (myCuts.at(cutIt))->lepMetW_DphiMAX = lepMetW_DphiMAX;
    
    ++cutIt;
  }
  
  
  // run the analysis
  std::vector<float> nEventsScaled_sig(myCuts.size());
  std::vector<float> nEventsScaled_bkg(myCuts.size());
  
  readReducedTree(inputFileName.c_str(), false, &nEventsScaled_sig, &nEventsScaled_bkg, &myCuts);
  
  
  // create isoSig plot
  TGraph* graph = new TGraph();  
  
  for(unsigned int cutIt = 0; cutIt < myCuts.size(); ++cutIt)
    graph -> SetPoint(cutIt, nEventsScaled_sig.at(cutIt), nEventsScaled_bkg.at(cutIt));
  
  graph -> Write("graph");
  
  
  
  
  
  
  ////****************
  //// TMVA-BASED
  ////****************
  //
  //// define set of cuts
  //std::vector<cuts*> myCuts_TMVA;
  //cutIt = 0;
  //for(float mvaValueMIN = 0.; mvaValueMIN <= 0.4; mvaValueMIN+=0.01)
  //{
  //  myCuts_TMVA.push_back(new cuts());
  //  initializeCuts(*(myCuts_TMVA.at(cutIt)));
  //  
  //  (myCuts_TMVA.at(cutIt))->mvaValueMIN = mvaValueMIN;
  //  
  //  ++cutIt;
  //}
  //
  //
  //// run the analysis
  //std::vector<float> nEventsScaled_sig_TMVA(myCuts.size());
  //std::vector<float> nEventsScaled_bkg_TMVA(myCuts.size());
  //
  //readReducedTree(inputFileName.c_str(), true, &nEventsScaled_sig_TMVA, &nEventsScaled_bkg_TMVA, &myCuts_TMVA);
  //
  //
  //// create isoSig plot
  //TGraph* graph_TMVA = new TGraph();
  //
  //for(unsigned int cutIt = 0; cutIt < myCuts.size(); ++cutIt)
  //  graph_TMVA -> SetPoint(cutIt, nEventsScaled_sig_TMVA.at(cutIt), nEventsScaled_bkg_TMVA.at(cutIt));
  //
  //graph_TMVA -> Write("graph_TMVA"); 
  
  
  
  
  
  
  TF1* f01sig = new TF1("f01sig", "100*x*x", 0., 1000.);
  f01sig->SetLineStyle(2);
  f01sig->SetLineColor(kBlack);
  f01sig -> Write();
  
  TF1* f05sig = new TF1("f05sig", "4*x*x", 0., 1000.);
  f05sig->SetLineStyle(2);
  f05sig->SetLineColor(kBlack);
  f05sig -> Write();
  
  TF1* f1sig = new TF1("f1sig", "x*x", 0., 1000.);
  f1sig->SetLineStyle(2);
  f1sig->SetLineWidth(3);
  f1sig->SetLineColor(kBlack);
  f1sig -> Write();
  
  TF1* f2sig = new TF1("f2sig", "x*x/4.", 0., 1000.);
  f2sig->SetLineStyle(2);
  f2sig->SetLineColor(kBlack);
  f2sig -> Write();
  
  TF1* f3sig = new TF1("f3sig", "x*x/9.", 0., 1000.);
  f3sig->SetLineStyle(2);
  f3sig->SetLineColor(kBlack);
  f3sig -> Write();
  
  
  
  TGraph* graph_axis = new TGraph();
  graph_axis -> SetPoint(0, 0.001, 0.001);
  graph_axis -> SetPoint(1, 100., 1000.);
  graph_axis -> Write("graph_axis");
}