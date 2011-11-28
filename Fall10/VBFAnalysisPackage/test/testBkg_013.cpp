/*
testBkg_013.exe cfg/2011-10-11-listaFile.txt  


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
#include <sstream>


#include "TH1.h"
#include "TProfile.h"
#include "TObject.h"
#include "TRandom3.h"
#include "Math/Vector4D.h"

#include "TMVA/Reader.h"
#include "hColl.h"

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


int main (int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there
  if (argc != 3)
    {
      std::cerr << ">>>>> VBFAnalysis::usage: " << argv[0] << " configFileName eventsNum" << std::endl ;
      return 1 ;
    }

  int minEvents = atoi (argv[2]) ;

  float LUMI = 2145. ; //PG to have output in 1/fb

  string inputFileList (argv[1]) ;
  cout << "samples " << inputFileList << endl ;

  map<string, TChain *> collections ;
  string treeName = "ntu_14" ;
  ReadFile (collections, inputFileList, treeName) ;
  
  TH1::SetDefaultSumw2 (kTRUE) ;
  int colors[11] = {kBlue+2, kRed, 10, kCyan+2, kOrange+7, kGray, kMagenta+1, kGreen+2, kOrange, kViolet+2, kRed+3} ;

// crossSection
// totEvents

//  m2  |        
//      |   +--------------------+
//      |   |                    |
//      |   |     EvenHigher     |     
//      |   |                    |
//      |   +--------------------+
//      |   |                 a  |
//      |   |     upper      --- |     
//      |   |                 c  |
//      |   +--------------------+
//      |   |                    |
//      |   |    signal          |     
//      |   |                    |
//      |   +--------------------+
//      |   |                 c  |
//      |   |    lower       --- |     
//      |   |                 a  |
//      |   +--------------------+
//      +-------------------------------  m4

  int nBins = 200 ;
  double m4_min = 0. ;
  double m4_max = 1000. ;
  double step = (m4_max - m4_min) / nBins ;
  //PG the cuts
  TCut generalCut = "" ;
//  generalCut = generalCut && "WJJ_pt > 40" ; //PG pt cut on hadronic W
//  generalCut = generalCut && "lepMetW_mt > 40" ; //PG mt cut on leptonic W
//  generalCut = generalCut && "lep_flavour == 13" ; //PG only muons
//  generalCut = generalCut && "lep_flavour == 13" ; //PG only electrons
  std::string outputRootFullFileName = "testBkg_013.root" ;
  TString m4_VAR = "lepNuW_m_KF" ;

  TH1F * m4_EvenHigher_DATA = new TH1F ("m4_EvenHigher_DATA", "m4_EvenHigher_DATA", nBins, m4_min, m4_max) ;
  TH1F * m4_upper_DATA      = new TH1F ("m4_upper_DATA",      "m4_upper_DATA",      nBins, m4_min, m4_max) ;
  TH1F * m4_upper_c_DATA    = new TH1F ("m4_upper_c_DATA",    "m4_upper_c_DATA",    nBins, m4_min, m4_max) ;
  TH1F * m4_upper_a_DATA    = new TH1F ("m4_upper_a_DATA",    "m4_upper_a_DATA",    nBins, m4_min, m4_max) ;
  TH1F * m4_signal_DATA     = new TH1F ("m4_signal_DATA",     "m4_signal_DATA",     nBins, m4_min, m4_max) ;
  TH1F * m4_lower_DATA      = new TH1F ("m4_lower_DATA",      "m4_lower_DATA",      nBins, m4_min, m4_max) ;
  TH1F * m4_lower_c_DATA    = new TH1F ("m4_lower_c_DATA",    "m4_lower_c_DATA",    nBins, m4_min, m4_max) ;
  TH1F * m4_lower_a_DATA    = new TH1F ("m4_lower_a_DATA",    "m4_lower_a_DATA",    nBins, m4_min, m4_max) ;
  TH1F * m4_sideband_DATA   = new TH1F ("m4_sideband_DATA",   "m4_sideband_DATA",   nBins, m4_min, m4_max) ;

  TCut lower      = "WJJ_m > 50  && WJJ_m < 65" ;
  TCut lower_c    = "WJJ_m > 57  && WJJ_m < 65" ;
  TCut lower_a    = "WJJ_m > 50  && WJJ_m < 57" ;
  TCut signal     = "WJJ_m > 65  && WJJ_m < 95" ;
  TCut upper      = "WJJ_m > 95  && WJJ_m < 130" ;
  TCut upper_c    = "WJJ_m > 95  && WJJ_m < 107" ;
  TCut upper_a    = "WJJ_m > 107 && WJJ_m < 130" ;
  TCut EvenHigher = "WJJ_m > 130 && WJJ_m < 400" ;

  //PG loop over samples
  int index = 0 ;
  for (map<string, TChain *>::iterator iColl = collections.begin () ;
       iColl != collections.end () ; 
       ++iColl)
    {
      TCut cutLower = generalCut && lower ;
      TCut cutLower_c = generalCut && lower_c ;
      TCut cutLower_a = generalCut && lower_a ;
      TCut cutSignal = generalCut && signal ;
      TCut cutUpper = generalCut && upper ;
      TCut cutUpper_c = generalCut && upper_c ;
      TCut cutUpper_a = generalCut && upper_a ;
      TCut cutEvenHigher = generalCut && EvenHigher ;
      TCut cutSideband = generalCut && (upper || lower) ;
      
      if (iColl->first == "DATA")
        {
          iColl->second->Draw (m4_VAR + TString (" >> m4_lower_DATA"), cutLower) ;
          iColl->second->Draw (m4_VAR + TString (" >> m4_lower_a_DATA"), cutLower_a) ;
          iColl->second->Draw (m4_VAR + TString (" >> m4_lower_c_DATA"), cutLower_c) ;
          iColl->second->Draw (m4_VAR + TString (" >> m4_signal_DATA"), cutSignal) ;
          iColl->second->Draw (m4_VAR + TString (" >> m4_upper_DATA"), cutUpper) ;
          iColl->second->Draw (m4_VAR + TString (" >> m4_upper_a_DATA"), cutUpper_a) ;
          iColl->second->Draw (m4_VAR + TString (" >> m4_upper_c_DATA"), cutUpper_c) ;
          iColl->second->Draw (m4_VAR + TString (" >> m4_EvenHigher_DATA"), cutEvenHigher) ;
          iColl->second->Draw (m4_VAR + TString (" >> m4_sideband_DATA"), cutSideband) ;
        
          continue ;
        }   
      ++index ;   
    } //PG loop over samples

  vector<double> xbins ;
  int enough = true ;
  int eventsInBin = 0 ;
  //PG loop on the bins of the histogram
  for (int iBin = 1 ; iBin < m4_signal_DATA->GetNbinsX () ; ++iBin)
    {
      if (enough) xbins.push_back (m4_signal_DATA->GetBinLowEdge (iBin)) ;
      eventsInBin += m4_signal_DATA->GetBinContent (iBin) ;
      if (eventsInBin < minEvents)
        {
          enough = false ;
        }
      else
        {
          enough = true ;
          eventsInBin = 0 ;
        }        
    } //PG loop on the bins of the histogram


  cout << "Float_t xAxis [" << xbins.size () << "] = {" ;
  for (int i = 0 ; i < xbins.size () ; ++i) cout << xbins.at (i) << ", " ; 
  cout << m4_max << "} ;\n" ; 
   	
  return 0 ;
}
