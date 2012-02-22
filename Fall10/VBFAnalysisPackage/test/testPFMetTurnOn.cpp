/* Usage example
bin/testPFMetTurnOn.exe cfg/testPFJetMetTurnOnFileList.txt /gwteraz/users/benaglia/data/EfficiencyCorrections/ plots/
*/


#include "VBFAnalysisVariables.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "analysisUtils.h"
#include "PUUtils.h"
#include "readJSONFile.h"
#include "HelicityLikelihoodDiscriminant.h"
#include "QGLikelihoodCalculator.h"
#include "VBFKinematicFit.h"

#include <iomanip>

#include "TH1F.h"
#include "TF1.h"
#include "TF2.h"
#include "TProfile.h"
#include "TObject.h"
#include "TRandom3.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TLegend.h"
#include "Math/Vector4D.h"

int ReadFile (map<string, TChain *> & output, string inputList, string treeName)
{
  std::ifstream inFile (inputList.c_str ()) ;
  std::string buffer ; 
  while (!inFile.eof ())
    {
      getline (inFile, buffer) ;
      if (buffer != "") 
        { ///---> save from empty line at the end!
          if (buffer.at(0) != '#')
            {
              std::string dummyName ;
              std::stringstream line ( buffer );       
              line >> dummyName ; 
              std::cout << dummyName << " ";
              std::string dummyLocation ;
              line >> dummyLocation ; 
              std::cout << dummyLocation << "\n";

              // Open tree
              if (output.find (dummyName) == output.end ())
                {
                  TChain* chain = new TChain (treeName.c_str ()) ;
                  chain->Add (dummyLocation.c_str ()) ;
                  output[dummyName] = chain ;
                }
              else 
                output[dummyName]->Add (dummyLocation.c_str ()) ; 
            } 
        }
    }
  return 0 ;
}

TH1F* ratioHisto(TH1F* h_num, TH1F* h_den, const float& yMin, const float& yMax)
{
  TH1F* h_ratio = (TH1F*)( h_num->Clone() );
  h_ratio -> Divide(h_den);
  
  h_ratio -> GetYaxis() -> SetRangeUser(yMin,yMax);
  h_ratio -> GetXaxis() -> SetLabelSize(0.09);
  h_ratio -> GetYaxis() -> SetLabelSize(0.09);
  h_ratio -> GetXaxis() -> SetLabelFont(42);
  h_ratio -> GetYaxis() -> SetLabelFont(42);
  h_ratio -> GetYaxis() -> SetTitleSize(0.15);
  h_ratio -> GetYaxis() -> SetTitleFont(42);
  h_ratio -> GetYaxis() -> SetTitleOffset(0.42);
  h_ratio -> GetYaxis() -> SetNdivisions(206);
  h_ratio -> GetXaxis() -> SetTitle("");
  h_ratio -> GetYaxis() -> SetTitle("ratio");
  
  return h_ratio;
}

void drawTriplet(TH1F* h_data, TH1F* h_mc, TH1F* h_mc_corr, TString varName, TString saveName)
{
  // Draw the nice histo
  TCanvas* c1 = new TCanvas("c1","c1",600,600);
  c1 -> cd();
  TPad* p1 = new TPad("p1","p1",0., 0.25, 1., 1.);
  TPad* p2 = new TPad("p2","p2",0., 0., 1., 0.25);
  p1 -> Draw();
  p2 -> Draw();

  p1 -> cd();
  p1 -> SetGridx();
  p1 -> SetGridy();

  TLegend* leg = new TLegend(0.8,0.8,0.95,0.95);
  leg -> SetFillColor(kWhite);
  h_data -> GetXaxis() -> SetTitle(varName);
  h_data -> GetYaxis() -> SetTitle("events");
  h_data -> Draw("e");
  leg -> AddEntry(h_data,"data","pe");
  h_mc -> SetLineColor(kBlue);
  h_mc -> Draw("hist,same");
  leg -> AddEntry(h_mc,"MC","l");
  h_mc_corr -> SetLineColor(kRed);
  h_mc_corr -> Draw("hist,same");
  leg -> AddEntry(h_mc_corr,"MC corr","l");
  leg -> Draw();
  
  p2 -> cd();
  p2 -> SetGridx();
  p2 -> SetGridy();

  TH1F* h_ratio_mc        = ratioHisto(h_data,h_mc,0.60,1.40);
  TH1F* h_ratio_mc_corr   = ratioHisto(h_data,h_mc,0.60,1.40);
  h_ratio_mc      -> SetLineColor(kBlue);
  h_ratio_mc_corr -> SetLineColor(kRed);
  h_ratio_mc      -> Draw("hist");
  h_ratio_mc_corr -> Draw("same");

  c1 -> Print(saveName,"pdf");
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int main(int argc, char** argv)
{
  //white bkg
  gROOT->ProcessLine(".x ~/public/setTDRStyle.C");
  //no stat box
  gStyle->SetOptStat(0);

  
  //Check if all nedeed arguments to parse are there
  if (argc != 4)
    {
      std::cerr << ">>>>> usage: " << argv[0] << " inputFileList inputCorrectionFolder outputFolder" << std::endl ;
      return 1 ;
    }
  TH1::SetDefaultSumw2 (kTRUE) ;

  // Parse the config file
  string inputFileList (argv[1]) ;
  cout << "samples " << inputFileList << endl ;
  string inputCorrectionFolder = argv[2] ;
  string outFolder = argv[3] ;

  map<string, TChain *> collections ;
  string treeName = "ntu_13" ; //PG all the preselections + deta_jj
  ReadFile (collections, inputFileList, treeName) ;
  
  std::vector<std::string> runMin;
  runMin.push_back("165088");
  runMin.push_back("175832");
  runMin.push_back("176461");
  runMin.push_back("178420");
  std::vector<std::string> runMax;
  runMax.push_back("173692");
  runMax.push_back("176309");
  runMax.push_back("178380");
  runMax.push_back("180252");
  std::vector<std::string> MetCorrectionNames;
  MetCorrectionNames.push_back("metEffHLT1");
  MetCorrectionNames.push_back("metEffHLT2");
  MetCorrectionNames.push_back("metEffHLT3");
  MetCorrectionNames.push_back("metEffHLT4");
  std::vector<std::string> lumiPeriods;
  lumiPeriods.push_back("1956");
  lumiPeriods.push_back("337.5");
  lumiPeriods.push_back("1359");
  lumiPeriods.push_back("816");
  const int nMetPeriods = 4;

  // Cut on data
  TString dataCut = "lep_flavour == 11";
  // Cut on MC
//  TString mcCut = "(1./totEvents * crossSection * PUWeight * eventWeight/metWeight) * (lep_flavour == 11)";
  TString mcCut = "(1./totEvents * crossSection * PUWeight * eventWeight) * (lep_flavour == 11)";

  TString thisCut;

  int nBinsMet = 40 ;
  float minMet = 0. ;
  float maxMet = 200. ;

  int nBinsJet = 40 ;
  float minJet = 0. ;
  float maxJet = 200. ;

  TCanvas* can = new TCanvas();
  
  TH1F* h_data_met[nMetPeriods];
  TH1F* h_mc_met[nMetPeriods];
  TH1F* h_mc_corr_met[nMetPeriods];
  for ( int i = 0; i < nMetPeriods; i++ ) {
    h_data_met[i] = new TH1F(("h_data_met_"+MetCorrectionNames[i]).c_str(), ("h_data_met"+MetCorrectionNames[i]).c_str(), nBinsMet, minMet, maxMet);
    h_mc_met[i] = new TH1F(("h_mc_met_"+MetCorrectionNames[i]).c_str(), ("h_mc_met"+MetCorrectionNames[i]).c_str(), nBinsMet, minMet, maxMet);
    h_mc_corr_met[i] = new TH1F(("h_mc_corr_met_"+MetCorrectionNames[i]).c_str(), ("h_mc_corr_met"+MetCorrectionNames[i]).c_str(), nBinsMet, minMet, maxMet);
  }
  TH1F* h_data_jet1 = new TH1F("h_data_jet1", "h_data_jet1", nBinsJet*2, minJet, maxJet*2);
  TH1F* h_mc_jet1 = new TH1F("h_mc_jet1", "h_mc_jet1", nBinsJet*2, minJet, maxJet*2);
  TH1F* h_mc_corr_jet1 = new TH1F("h_mc_corr_jet1", "h_mc_corr_jet1", nBinsJet*2, minJet, maxJet*2);
  TH1F* h_data_jet2 = new TH1F("h_jet2", "h_jet2", nBinsJet, minJet, maxJet);
  TH1F* h_mc_jet2 = new TH1F("h_mc_jet2", "h_mc_jet2", nBinsJet, minJet, maxJet);
  TH1F* h_mc_corr_jet2 = new TH1F("h_mc_corr_jet2", "h_mc_corr_jet2", nBinsJet, minJet, maxJet);
  
  //Loop over samples
  int index = 0 ;
  for (map<string, TChain *>::iterator iColl = collections.begin () ;
       iColl != collections.end () ; 
       ++iColl) {

    
    TChain * chain = iColl->second ;
    
    if (iColl->first == "DATA") {
    
      for ( int iPeriod = 0; iPeriod < nMetPeriods; iPeriod++ ) {
        thisCut = " ( runId <= " + runMax[iPeriod] + " && runId >= " + runMin[iPeriod] + " )*" + dataCut;
        iColl->second->Draw ("met_et" + TString (" >>+ ") + h_data_met[iPeriod]->GetName (), thisCut);
      }
      thisCut = " ( runId <= " + runMax[nMetPeriods-1] + " && runId >= " + runMin[nMetPeriods-1] + " )*" + dataCut;
      iColl->second->Draw ("WJ1_pt" + TString (" >>+ ") + h_data_jet1->GetName (), thisCut);
      iColl->second->Draw ("WJ2_pt" + TString (" >>+ ") + h_data_jet2->GetName (), thisCut);
    
    }
    else {
    
      for ( int iPeriod = 0; iPeriod < nMetPeriods; iPeriod++ ) {
        thisCut = lumiPeriods[iPeriod] + "*" + mcCut + "/metWeight";
        iColl->second->Draw ("met_et" + TString (" >>+ ") + h_mc_met[iPeriod]->GetName (), thisCut);
 
      }
      thisCut = lumiPeriods[nMetPeriods-1] + "*" + mcCut + "/jetWeight";
      iColl->second->Draw ("WJ1_pt" + TString (" >>+ ") + h_mc_jet1->GetName (), thisCut);
      iColl->second->Draw ("WJ2_pt" + TString (" >>+ ") + h_mc_jet2->GetName (), thisCut);

      for ( int iPeriod = 0; iPeriod < nMetPeriods; iPeriod++ ) {
        thisCut = lumiPeriods[iPeriod] + "*" + mcCut;
        iColl->second->Draw ("met_et" + TString (" >>+ ") + h_mc_corr_met[iPeriod]->GetName (), thisCut);

      }

      thisCut = lumiPeriods[nMetPeriods-1] + "*" + mcCut;
      iColl->second->Draw ("WJ1_pt" + TString (" >>+ ") + h_mc_corr_jet1->GetName (), thisCut);
      iColl->second->Draw ("WJ2_pt" + TString (" >>+ ") + h_mc_corr_jet2->GetName (), thisCut);
 
    }

  }//End loop over samples
  
  // Print histos
  for ( int i = 0; i < nMetPeriods; i++ ) {
    drawTriplet(h_data_met[i], h_mc_met[i], h_mc_corr_met[i], "MET", (TString) outFolder + MetCorrectionNames[i] + ".pdf");
  }

  drawTriplet(h_data_jet1, h_mc_jet1, h_mc_corr_jet1, "WJ1_pt", (TString) outFolder + "jet1.pdf");

  drawTriplet(h_data_jet2, h_mc_jet2, h_mc_corr_jet2, "WJ2_pt", (TString) outFolder + "jet2.pdf");
    
}

