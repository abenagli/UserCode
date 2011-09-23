/*
testBkg_004.exe cfg/2011-07-09-listaFile.txt

NB Kinematic Fit applied (_KF in the vars name)

*/


#include "VBFAnalysisVariables.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "kalanand.h"
#include "hFactory.h"
#include "h2Factory.h"
#include "stdHisto.h"

#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>


#include "TH1.h"
#include "TProfile.h"
#include "TObject.h"
#include "TRandom3.h"
#include "Math/Vector4D.h"
#include "TMVA/Reader.h"

#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooArgSet.h"

#include "hColl.h"

#include "PURescaleFactor.C"
#pragma link C++ function PURescaleFactor (const int&);

using namespace std ;


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


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


// void
// fillDS (RooDataSet * output, TTree * input, double LUMI)
// {
// 
// }


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int main (int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there
  if (argc != 2)
    {
      std::cerr << ">>>>> VBFAnalysis::usage: " << argv[0] << " configFileName" << std::endl ;
      return 1 ;
    }

  float LUMI = 2145. ; //PG to have output in 1/fb

  string inputFileList (argv[1]) ;
  cout << "samples " << inputFileList << endl ;

  map<string, TChain *> collections ;
  string treeName = "ntu_18" ;
  ReadFile (collections, inputFileList, treeName) ;
  
  TH1::SetDefaultSumw2 (kTRUE) ;
  int colors[11] = {kBlue+2, kRed, 10, kCyan+2, kOrange+7, kGray, kMagenta+1, kGreen+2, kOrange, kViolet+2, kRed+3} ;

  vector<RooDataSet*> simulation_DS ;
  RooDataSet * data_DS ;

  RooRealVar WJJ_m ("WJJ_m", "had W inv mass", 0., 500.) ;
  RooRealVar lepNuW_m_KF ("lepNuW_m_KF", "4-body inv mass", 0., 1000.) ;
  RooRealVar weight ("weight", "event weight", 0., 1000000000000.) ;

//  RooRealVar totEvents ("totEvents", "total number of events", 0., 1000000000.) ;
//  RooRealVar crossSection ("crossSection", "process cross-section", 0., 1000000000.) ;
//  RooRealVar PUit_n ("PUit_n", "in time pileup", 0., 100.) ;
//  RooRealVar PUoot_n ("PUoot_n", "out of time pileup", 0., 100.) ;
//  TString expression = "1./totEvents * crossSection * " ;
//  expression += LUMI ;
//  expression += " * PURescaleFactor((PUit_n+PUoot_n)/3.)" ; 
//  RooFormulaVar weight ("weight", expression, RooArgList (totEvents, crossSection, PUit_n, PUoot_n)) ;


//  TH1F * m4_upper_DATA = new TH1F ("m4_upper_DATA", "m4_upper_DATA", 70, 100., 800.) ;
//  TH1F * m4_signal_DATA = new TH1F ("m4_signal_DATA", "m4_signal_DATA", 70, 100., 800.) ;
//  TH1F * m4_lower_DATA = new TH1F ("m4_lower_DATA", "m4_lower_DATA", 70, 100., 800.) ;
//  TH1F * m4_sideband_DATA = new TH1F ("m4_sideband_DATA", "m4_sideband_DATA", 70, 100., 800.) ;

  //PG loop over samples
  int index = 0 ;
  for (map<string, TChain *>::iterator iColl = collections.begin () ;
       iColl != collections.end () ; 
       ++iColl)
    {

      if (iColl->first.find ("ggH") != string::npos) continue ;
      if (iColl->first.find ("qqH") != string::npos) continue ;
//      if (iColl->first == "ggH") continue ;

      if (iColl->first == "DATA")
        {
          RooDataSet * rds = new RooDataSet ("data", "data", iColl->second, RooArgSet (WJJ_m, lepNuW_m_KF)) ; 
          VBFAnalysisVariables vars ;
          SetVBFPreselectionTreeBranches (vars, iColl->second) ;
          for (int i = 0 ; i < iColl->second->GetEntries () ; ++i)
            {
              iColl->second->GetEntry (i) ;
              WJJ_m = vars.WJJ_m ;   
              lepNuW_m_KF = vars.lepNuW_m_KF ;
              rds->add (RooArgSet (WJJ_m, lepNuW_m_KF)) ;            
            }
          data_DS = rds ;
          continue ;
        }  
      cout << " reading " << iColl->first << endl ;

      TString name = "mc_" ;
      name += iColl->first ;
      RooDataSet * rds = new RooDataSet (name, name, iColl->second, RooArgSet (WJJ_m, lepNuW_m_KF)) ; 

      VBFAnalysisVariables vars ;
      SetVBFPreselectionTreeBranches (vars, iColl->second) ;
      for (int i = 0 ; i < iColl->second->GetEntries () ; ++i)
        {
          iColl->second->GetEntry (i) ;
          WJJ_m = vars.WJJ_m ;   
          lepNuW_m_KF = vars.lepNuW_m_KF ;
          double weight = 1./vars.totEvents * vars.crossSection *  LUMI * PURescaleFactor ((vars.PUit_n + vars.PUoot_n)/3.) ; 
          rds->add (RooArgSet (WJJ_m, lepNuW_m_KF), weight) ;            
        }
      simulation_DS.push_back (rds) ;
      ++index ;   
    } //PG loop over samples

  // define out file names
  std::string outputRootFullFileName = "testBkg_009.root" ;
  TFile* outputRootFile = new TFile (outputRootFullFileName.c_str (), "RECREATE") ;
  outputRootFile->cd () ;
  for (int i = 0 ; i < simulation_DS.size () ; ++i) simulation_DS.at (i)->Write () ;
  data_DS->Write () ;
  outputRootFile->Close () ;
  delete outputRootFile ;
  
  return 0 ;
}

/*

#include  <./test/plotUtils.C>



*/

