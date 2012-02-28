/*
testBkg_017.exe cfg/testBkg_017-fileList.txt 400
Optimized cuts on WJJ_m for signal/sideband regions
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
    std::cerr << ">>>>> VBFAnalysis::usage: " << argv[0] << " configFileName higgsmass" << std::endl ;
    return 1 ;
  }
  
  float LUMI = 4680 ; //PG to have output in 1/fb
  
  string mass = argv[2] ;
  
  string inputFileList (argv[1]) ;
  cout << "samples " << inputFileList << endl ;
  
  map<string, TChain *> collections ;
  string treeName = "ntu_14" ; //PG all the preselections + deta_jj
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
  
  int nBins = 100 ;
  double m4_min = 0. ;
  double m4_max = 1000. ;
  
  //PG the cuts
  TCut generalCut = "" ;
  //  generalCut = generalCut && "(helicityLikelihood > 0.5)" ; //PG helicity likelihood di HZZ
  //  generalCut = generalCut && "WJJ_pt > 40" ; //PG pt cut on hadronic W
  //  generalCut = generalCut && "lepMetW_mt > 40" ; //PG mt cut on leptonic W
  // generalCut = generalCut && "nJets_cnt_pt30 == 2" ; //only 2 jets
  // generalCut = generalCut && "nJets_cnt_pt30 != 2" ; //only 3 jets
  
  std::string outputRootFullFileName = "testBkg_017_S" + mass + "_2011AB.root" ;  
  
  TString m4_VAR = "lepNuW_m_KF" ;
  
  //Make everything for 3 flavour combinations
  std::vector<TCut> flavourCut;
  flavourCut.push_back("lep_flavour == 11"); //e
  flavourCut.push_back("lep_flavour == 13"); //mu
  flavourCut.push_back(""); //emu
  std::vector<TString> flavourName;
  flavourName.push_back("e"); //e
  flavourName.push_back("mu"); //mu
  flavourName.push_back("emu"); //emu
  std::vector<std::string> stringFlavourName;
  stringFlavourName.push_back("e"); //e
  stringFlavourName.push_back("mu"); //mu
  stringFlavourName.push_back("emu"); //emu
  const int nFlavourCombinations = 3;
  
  hColl* m4_EvenHigher[nFlavourCombinations] ;
  hColl* m4_upper     [nFlavourCombinations] ;
  hColl* m4_upper_c   [nFlavourCombinations] ; // close
  hColl* m4_upper_a   [nFlavourCombinations] ; // away
  hColl* m4_signal    [nFlavourCombinations] ;
  hColl* m4_lower     [nFlavourCombinations] ;
  hColl* m4_lower_c   [nFlavourCombinations] ; // close
  hColl* m4_lower_a   [nFlavourCombinations] ; // away
  hColl* m4_sideband  [nFlavourCombinations] ;
  
  TH1F * m4_EvenHigher_DATA[nFlavourCombinations] ;
  TH1F * m4_upper_DATA     [nFlavourCombinations] ;
  TH1F * m4_upper_c_DATA   [nFlavourCombinations] ;
  TH1F * m4_upper_a_DATA   [nFlavourCombinations] ;
  TH1F * m4_signal_DATA    [nFlavourCombinations] ;
  TH1F * m4_lower_DATA     [nFlavourCombinations] ;
  TH1F * m4_lower_c_DATA   [nFlavourCombinations] ;
  TH1F * m4_lower_a_DATA   [nFlavourCombinations] ;
  TH1F * m4_sideband_DATA  [nFlavourCombinations] ;
  
  hColl* m4_EvenHigher_SIG[nFlavourCombinations] ;
  hColl* m4_upper_SIG     [nFlavourCombinations] ;
  hColl* m4_upper_c_SIG   [nFlavourCombinations] ;
  hColl* m4_upper_a_SIG   [nFlavourCombinations] ;
  hColl* m4_signal_SIG    [nFlavourCombinations] ;
  hColl* m4_lower_SIG     [nFlavourCombinations] ;
  hColl* m4_lower_c_SIG   [nFlavourCombinations] ;
  hColl* m4_lower_a_SIG   [nFlavourCombinations] ;
  hColl* m4_sideband_SIG  [nFlavourCombinations] ;
  
  for ( int iFlav = 0; iFlav < nFlavourCombinations; iFlav++ )  {
    
    m4_EvenHigher[iFlav] = new  hColl(stringFlavourName[iFlav] + "_m4_EvenHigher", nBins, m4_min, m4_max) ;
    m4_upper[iFlav]      = new  hColl(stringFlavourName[iFlav] + "_m4_upper",      nBins, m4_min, m4_max) ;
    m4_upper_c[iFlav]    = new  hColl(stringFlavourName[iFlav] + "_m4_upper_c",    nBins, m4_min, m4_max) ; // close
    m4_upper_a[iFlav]    = new  hColl(stringFlavourName[iFlav] + "_m4_upper_a",    nBins, m4_min, m4_max) ; // away
    m4_signal[iFlav]     = new  hColl(stringFlavourName[iFlav] + "_m4_signal",     nBins, m4_min, m4_max) ;
    m4_lower[iFlav]      = new  hColl(stringFlavourName[iFlav] + "_m4_lower",      nBins, m4_min, m4_max) ;
    m4_lower_c[iFlav]    = new  hColl(stringFlavourName[iFlav] + "_m4_lower_c",    nBins, m4_min, m4_max) ; // close
    m4_lower_a[iFlav]    = new  hColl(stringFlavourName[iFlav] + "_m4_lower_a",    nBins, m4_min, m4_max) ; // away
    m4_sideband[iFlav]   = new  hColl(stringFlavourName[iFlav] + "_m4_sideband",   nBins, m4_min, m4_max) ;
    
    m4_EvenHigher_DATA[iFlav] = new TH1F ( flavourName[iFlav] + "_m4_EvenHigher_DATA", flavourName[iFlav] + "_m4_EvenHigher_DATA_", nBins, m4_min, m4_max) ;
    m4_upper_DATA[iFlav]      = new TH1F ( flavourName[iFlav] + "_m4_upper_DATA",      flavourName[iFlav] + "_m4_upper_DATA_",      nBins, m4_min, m4_max) ;
    m4_upper_c_DATA[iFlav]    = new TH1F ( flavourName[iFlav] + "_m4_upper_c_DATA",    flavourName[iFlav] + "_m4_upper_c_DATA_",    nBins, m4_min, m4_max) ;
    m4_upper_a_DATA[iFlav]    = new TH1F ( flavourName[iFlav] + "_m4_upper_a_DATA",    flavourName[iFlav] + "_m4_upper_a_DATA_",    nBins, m4_min, m4_max) ;
    m4_signal_DATA[iFlav]     = new TH1F ( flavourName[iFlav] + "_m4_signal_DATA",     flavourName[iFlav] + "_m4_signal_DATA_",     nBins, m4_min, m4_max) ;
    m4_lower_DATA[iFlav]      = new TH1F ( flavourName[iFlav] + "_m4_lower_DATA",      flavourName[iFlav] + "_m4_lower_DATA_",      nBins, m4_min, m4_max) ;
    m4_lower_c_DATA[iFlav]    = new TH1F ( flavourName[iFlav] + "_m4_lower_c_DATA",    flavourName[iFlav] + "_m4_lower_c_DATA_",    nBins, m4_min, m4_max) ;
    m4_lower_a_DATA[iFlav]    = new TH1F ( flavourName[iFlav] + "_m4_lower_a_DATA",    flavourName[iFlav] + "_m4_lower_a_DATA_",    nBins, m4_min, m4_max) ;
    m4_sideband_DATA[iFlav]   = new TH1F ( flavourName[iFlav] + "_m4_sideband_DATA",   flavourName[iFlav] + "_m4_sideband_DATA_",   nBins, m4_min, m4_max) ;
    
    m4_EvenHigher_SIG[iFlav] = new hColl(stringFlavourName[iFlav] + "_m4_EvenHigher_SIG", nBins, m4_min, m4_max) ;
    m4_upper_SIG[iFlav]      = new hColl(stringFlavourName[iFlav] + "_m4_upper_SIG",      nBins, m4_min, m4_max) ;
    m4_upper_c_SIG[iFlav]    = new hColl(stringFlavourName[iFlav] + "_m4_upper_c_SIG",    nBins, m4_min, m4_max) ;
    m4_upper_a_SIG[iFlav]    = new hColl(stringFlavourName[iFlav] + "_m4_upper_a_SIG",    nBins, m4_min, m4_max) ;
    m4_signal_SIG[iFlav]     = new hColl(stringFlavourName[iFlav] + "_m4_signal_SIG",     nBins, m4_min, m4_max) ;
    m4_lower_SIG[iFlav]      = new hColl(stringFlavourName[iFlav] + "_m4_lower_SIG",      nBins, m4_min, m4_max) ;
    m4_lower_c_SIG[iFlav]    = new hColl(stringFlavourName[iFlav] + "_m4_lower_c_SIG",    nBins, m4_min, m4_max) ;
    m4_lower_a_SIG[iFlav]    = new hColl(stringFlavourName[iFlav] + "_m4_lower_a_SIG",    nBins, m4_min, m4_max) ;
    m4_sideband_SIG[iFlav]   = new hColl(stringFlavourName[iFlav] + "_m4_sideband_SIG",   nBins, m4_min, m4_max) ;
    
  }
  
  TCut lower      = "WJJ_m > 55  && WJJ_m < 65" ;
  TCut lower_c    = "WJJ_m > 60  && WJJ_m < 65" ;
  TCut lower_a    = "WJJ_m > 55  && WJJ_m < 60" ;
  TCut signal     = "WJJ_m > 65  && WJJ_m < 95" ;
  TCut upper      = "WJJ_m > 95  && WJJ_m < 115" ;
  TCut upper_c    = "WJJ_m > 95  && WJJ_m < 105" ;
  TCut upper_a    = "WJJ_m > 105 && WJJ_m < 115" ;
  TCut EvenHigher = "WJJ_m > 115 && WJJ_m < 400" ;
  
  //PG loop over samples
  //Add eventWeight for the pT weighting of the Higgs in the gg samples
  int index = 0 ;
  for (map<string, TChain *>::iterator iColl = collections.begin () ;
  iColl != collections.end () ; 
  ++iColl)
  {
    TCut cutLower[nFlavourCombinations];
    TCut cutLowerExtended[nFlavourCombinations];
    
    TCut cutLower_c[nFlavourCombinations];
    TCut cutLower_cExtended[nFlavourCombinations];
    
    TCut cutLower_a[nFlavourCombinations];
    TCut cutLower_aExtended[nFlavourCombinations];
    
    TCut cutSignal[nFlavourCombinations];
    TCut cutSignalExtended[nFlavourCombinations];
    
    TCut cutUpper[nFlavourCombinations];
    TCut cutUpperExtended[nFlavourCombinations];
    
    TCut cutUpper_c[nFlavourCombinations];
    TCut cutUpper_cExtended[nFlavourCombinations];
    
    TCut cutUpper_a[nFlavourCombinations];
    TCut cutUpper_aExtended[nFlavourCombinations];
    
    TCut cutEvenHigher[nFlavourCombinations];
    TCut cutEvenHigherExtended[nFlavourCombinations];
    
    TCut cutSideband[nFlavourCombinations];
    TCut cutSidebandExtended[nFlavourCombinations];
    
    for ( int iFlav = 0; iFlav < nFlavourCombinations; iFlav++ )  {
      
      cutLower[iFlav] = generalCut && lower && flavourCut[iFlav] ;
      cutLowerExtended[iFlav] = Form ("(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight", cutLower[iFlav].GetTitle (), LUMI) ;    
      
      cutLower_c[iFlav] = generalCut && lower_c && flavourCut[iFlav] ;
      cutLower_cExtended[iFlav] = Form ("(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight", cutLower_c[iFlav].GetTitle (), LUMI) ;    
      
      cutLower_a[iFlav] = generalCut && lower_a && flavourCut[iFlav] ;
      cutLower_aExtended[iFlav] = Form ("(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight", cutLower_a[iFlav].GetTitle (), LUMI) ;    
      
      cutSignal[iFlav] = generalCut && signal && flavourCut[iFlav] ;
      cutSignalExtended[iFlav] = Form ("(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight", cutSignal[iFlav].GetTitle (), LUMI) ;    
      
      cutUpper[iFlav] = generalCut && upper && flavourCut[iFlav] ;
      cutUpperExtended[iFlav] = Form ("(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight", cutUpper[iFlav].GetTitle (), LUMI) ;    
      
      cutUpper_c[iFlav] = generalCut && upper_c && flavourCut[iFlav] ;
      cutUpper_cExtended[iFlav] = Form ("(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight", cutUpper_c[iFlav].GetTitle (), LUMI) ;    
      
      cutUpper_a[iFlav] = generalCut && upper_a && flavourCut[iFlav] ;
      cutUpper_aExtended[iFlav] = Form ("(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight", cutUpper_a[iFlav].GetTitle (), LUMI) ;    
      
      cutEvenHigher[iFlav] = generalCut && EvenHigher && flavourCut[iFlav] ;
      cutEvenHigherExtended[iFlav] = Form ("(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight", cutEvenHigher[iFlav].GetTitle (), LUMI) ;    
      
      cutSideband[iFlav] = generalCut && (upper || lower) && flavourCut[iFlav] ;
      cutSidebandExtended[iFlav] = Form ("(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight", cutSideband[iFlav].GetTitle (), LUMI) ;    
      
    }
    
    cout << " reading " << iColl->first << endl ;
    if (iColl->first.find ("ggH") != string::npos || iColl->first.find ("qqH") != string::npos) 
    {
      if (iColl->first.find (mass.c_str ()) == string::npos) 
      {
	cout << "...skipping" << endl ;
	continue ; 
      } 
      
      TH1F * h_m4_lower_SIG[nFlavourCombinations];
      TH1F * h_m4_lower_c_SIG[nFlavourCombinations];
      TH1F * h_m4_lower_a_SIG[nFlavourCombinations];
      TH1F * h_m4_signal_SIG[nFlavourCombinations];
      TH1F * h_m4_upper_SIG[nFlavourCombinations];
      TH1F * h_m4_upper_c_SIG[nFlavourCombinations];
      TH1F * h_m4_upper_a_SIG[nFlavourCombinations];
      TH1F * h_m4_EvenHigher_SIG[nFlavourCombinations];
      TH1F * h_m4_sideband_SIG[nFlavourCombinations];
      
      for ( int iFlav = 0; iFlav < nFlavourCombinations; iFlav++ )  {
	
	h_m4_lower_SIG[iFlav] = m4_lower_SIG[iFlav] -> addSample (iColl->first.c_str ()) ;
	h_m4_lower_c_SIG[iFlav] = m4_lower_c_SIG[iFlav] -> addSample (iColl->first.c_str ()) ;
	h_m4_lower_a_SIG[iFlav] = m4_lower_a_SIG[iFlav] -> addSample (iColl->first.c_str ()) ;
	h_m4_signal_SIG[iFlav] = m4_signal_SIG[iFlav] -> addSample (iColl->first.c_str ()) ;
	h_m4_upper_SIG[iFlav] = m4_upper_SIG[iFlav] -> addSample (iColl->first.c_str ()) ;
	h_m4_upper_c_SIG[iFlav] = m4_upper_c_SIG[iFlav] -> addSample (iColl->first.c_str ()) ;
	h_m4_upper_a_SIG[iFlav] = m4_upper_a_SIG[iFlav] -> addSample (iColl->first.c_str ()) ;
	h_m4_EvenHigher_SIG[iFlav] = m4_EvenHigher_SIG[iFlav] -> addSample (iColl->first.c_str ()) ;
	h_m4_sideband_SIG[iFlav] = m4_sideband_SIG[iFlav] -> addSample (iColl->first.c_str ()) ;
	
      }
      
      TChain * chain = iColl->second ;
      VBFAnalysisVariables vars ;
      SetVBFPreselectionTreeBranches (vars, chain) ;
      
      for ( int iFlav = 0; iFlav < nFlavourCombinations; iFlav++ )  {
	
	iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_lower_SIG[iFlav]->GetName (), cutLowerExtended[iFlav] ) ;
	iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_lower_c_SIG[iFlav]->GetName (), cutLower_cExtended[iFlav] ) ;
	iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_lower_a_SIG[iFlav]->GetName (), cutLower_aExtended[iFlav] ) ;
	iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_signal_SIG[iFlav]->GetName (), cutSignalExtended[iFlav] ) ;
	iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_upper_SIG[iFlav]->GetName (), cutUpperExtended[iFlav] ) ;
	iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_upper_c_SIG[iFlav]->GetName (), cutUpper_cExtended[iFlav] ) ;
	iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_upper_a_SIG[iFlav]->GetName (), cutUpper_aExtended[iFlav] ) ;
	iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_EvenHigher_SIG[iFlav]->GetName (), cutEvenHigherExtended[iFlav] ) ;
	iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_sideband_SIG[iFlav]->GetName (), cutSidebandExtended[iFlav] ) ;
	
      }
      
      continue ;
    }
    
    if (iColl->first == "DATA")
    {
      for ( int iFlav = 0; iFlav < nFlavourCombinations; iFlav++ )  {
	
	iColl->second->Draw (m4_VAR + TString (" >> ") + m4_lower_DATA[iFlav]->GetName (), cutLower[iFlav] ) ;
	iColl->second->Draw (m4_VAR + TString (" >> ") + m4_lower_a_DATA[iFlav]->GetName (), cutLower_a[iFlav] ) ; 
	iColl->second->Draw (m4_VAR + TString (" >> ") + m4_lower_c_DATA[iFlav]->GetName (), cutLower_c[iFlav] ) ;
	iColl->second->Draw (m4_VAR + TString (" >> ") + m4_signal_DATA[iFlav]->GetName (), cutSignal[iFlav] ) ;
	iColl->second->Draw (m4_VAR + TString (" >> ") + m4_upper_DATA[iFlav]->GetName (), cutUpper[iFlav] ) ;
	iColl->second->Draw (m4_VAR + TString (" >> ") + m4_upper_a_DATA[iFlav]->GetName (), cutUpper_a[iFlav] ) ;
	iColl->second->Draw (m4_VAR + TString (" >> ") + m4_upper_c_DATA[iFlav]->GetName (), cutUpper_c[iFlav] ) ;
	iColl->second->Draw (m4_VAR + TString (" >>  ") + m4_EvenHigher_DATA[iFlav]->GetName (), cutEvenHigher[iFlav] ) ;
	iColl->second->Draw (m4_VAR + TString (" >>  ") + m4_sideband_DATA[iFlav]->GetName (), cutSideband[iFlav] ) ;
	
      }
      
      continue ;
    }  
    
    cout << "   defining histos" << endl ;
    
    TH1F * h_m4_lower[nFlavourCombinations];
    TH1F * h_m4_lower_c[nFlavourCombinations];
    TH1F * h_m4_lower_a[nFlavourCombinations];
    TH1F * h_m4_signal[nFlavourCombinations];
    TH1F * h_m4_upper[nFlavourCombinations];
    TH1F * h_m4_upper_c[nFlavourCombinations];
    TH1F * h_m4_upper_a[nFlavourCombinations];
    TH1F * h_m4_EvenHigher[nFlavourCombinations];
    TH1F * h_m4_sideband[nFlavourCombinations];
    
    for ( int iFlav = 0; iFlav < nFlavourCombinations; iFlav++ )  {
      
      h_m4_lower[iFlav] = m4_lower[iFlav] -> addSample (iColl->first.c_str ()) ;
      h_m4_lower_c[iFlav] = m4_lower_c[iFlav] -> addSample (iColl->first.c_str ()) ;
      h_m4_lower_a[iFlav] = m4_lower_a[iFlav] -> addSample (iColl->first.c_str ()) ;
      h_m4_signal[iFlav] = m4_signal[iFlav] -> addSample (iColl->first.c_str ()) ;
      h_m4_upper[iFlav] = m4_upper[iFlav] -> addSample (iColl->first.c_str ()) ;
      h_m4_upper_c[iFlav] = m4_upper_c[iFlav] -> addSample (iColl->first.c_str ()) ;
      h_m4_upper_a[iFlav] = m4_upper_a[iFlav] -> addSample (iColl->first.c_str ()) ;
      h_m4_EvenHigher[iFlav] = m4_EvenHigher[iFlav] -> addSample (iColl->first.c_str ()) ;
      h_m4_sideband[iFlav] = m4_sideband[iFlav] -> addSample (iColl->first.c_str ()) ;
      
    }
    
    cout << "   getting chain" << endl ;
    
    TChain * chain = iColl->second ;
    VBFAnalysisVariables vars ;
    //      SetVBFPreselectionTreeBranches (vars, chain) ;
    
    cout << "   filling histos" << endl ;
    
    for ( int iFlav = 0; iFlav < nFlavourCombinations; iFlav++ )  {
      
      int IND = 0 ;
      cout << "      index " << IND++ << endl ; iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_lower[iFlav]->GetName (), cutLowerExtended[iFlav] ) ;
      cout << "      index " << IND++ << endl ; iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_lower_c[iFlav]->GetName (), cutLower_cExtended[iFlav] ) ;
      cout << "      index " << IND++ << endl ; iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_lower_a[iFlav]->GetName (), cutLower_aExtended[iFlav] ) ;
      cout << "      index " << IND++ << endl ; iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_signal[iFlav]->GetName (), cutSignalExtended[iFlav] ) ;
      cout << "      index " << IND++ << endl ; iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_upper[iFlav]->GetName (), cutUpperExtended[iFlav] ) ;
      cout << "      index " << IND++ << endl ; iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_upper_c[iFlav]->GetName (), cutUpper_cExtended[iFlav] ) ;
      cout << "      index " << IND++ << endl ; iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_upper_a[iFlav]->GetName (), cutUpper_aExtended[iFlav] ) ;
      cout << "      index " << IND++ << endl ; iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_EvenHigher[iFlav]->GetName (), cutEvenHigherExtended[iFlav] ) ;
      cout << "      index " << IND++ << endl ; iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_sideband[iFlav]->GetName (), cutSidebandExtended[iFlav] ) ;
      
    }
    
    ++index ;   
  } //PG loop over samples
  
  //PG get the Wjets histograms from collections
  TH1F * m4_upper_c_WJets[nFlavourCombinations];
  TH1F * m4_upper_a_WJets[nFlavourCombinations];
  TH1F * m4_signal_WJets[nFlavourCombinations];
  
  for ( int iFlav = 0; iFlav < nFlavourCombinations; iFlav++ )  {
    
    m4_upper_c_WJets[iFlav] = m4_upper_c[iFlav] -> findHisto ( stringFlavourName[iFlav] + "_m4_upper_c_WJets" ) ;
    m4_upper_a_WJets[iFlav] = m4_upper_a[iFlav] -> findHisto (stringFlavourName[iFlav] + "_m4_upper_a_WJets" ) ;
    m4_signal_WJets[iFlav] = m4_signal[iFlav] -> findHisto (stringFlavourName[iFlav] + "_m4_signal_WJets" ) ;
    
    //FC fix the empty bins in the Wjets sample
    setErrorForEmptyBins_v2 (m4_upper_c_WJets[iFlav]);
    setErrorForEmptyBins_v2 (m4_upper_a_WJets[iFlav]);
    setErrorForEmptyBins_v2 (m4_signal_WJets[iFlav]);
    
  }
 
  hColl* m4_signal_reBinned[nFlavourCombinations];
  hColl* m4_sideband_reBinned[nFlavourCombinations];
  TH1F * h_m4_signal_reBinned[nFlavourCombinations];
  TH1F * h_m4_sideband_reBinned[nFlavourCombinations];
  
  for ( int iFlav = 0; iFlav < nFlavourCombinations; iFlav++ )  {
    
    //FC get the signal and sideband histograms from collections
    THStack m4_signalStack   = m4_signal[iFlav] -> makeStack () ;
    THStack m4_sidebandStack = m4_sideband[iFlav] -> makeStack () ;
    
    TH1F * m4_signal_Bkg   = (TH1F*) m4_signalStack.GetStack() -> Last() ;
    TH1F * m4_sideband_Bkg = (TH1F*) m4_sidebandStack.GetStack() -> Last() ;
    
    vector<double> xbins ;
    int enough = true ;
    int minEvents = 20 ;
    int eventsInBin_signal = 0 ;
    int eventsInBin_sideband = 0 ;
    
    //PG loop on the bins of the histogram
    for (int iBin = 1 ; iBin < m4_signal_Bkg->GetNbinsX () ; ++iBin)
    {
      if (enough) xbins.push_back (m4_signal_Bkg->GetBinLowEdge (iBin)) ;
      
      eventsInBin_signal += m4_signal_Bkg->GetBinContent (iBin) ;
      eventsInBin_sideband += m4_sideband_Bkg->GetBinContent (iBin) ;
      if (eventsInBin_signal < minEvents ||
	eventsInBin_sideband < minEvents )
      {
	enough = false ;
      }
      else
      {
	enough = true ;
	eventsInBin_signal = 0 ;
	eventsInBin_sideband = 0 ;
      }        
    } //PG loop on the bins of the histogram
    
    xbins.push_back (m4_max) ;
    
    cout << "Float_t xAxis [" << xbins.size () << "] = {" ;
    for (int i = 0 ; i < xbins.size () - 1; ++i) cout << xbins.at (i) << ", " ; 
    cout << m4_max << "} ;\n" ; 
    
    
    
    m4_signal_reBinned[iFlav] = new hColl( stringFlavourName[iFlav] + "_m4_signal_reBinned",     xbins.size() - 1, m4_min, m4_max) ;
    m4_sideband_reBinned[iFlav] = new hColl( stringFlavourName[iFlav] + "_m4_sideband_reBinned",   xbins.size() - 1, m4_min, m4_max) ;
    
    cout << "\n" ;
    cout << "   loop over bkg samples for filling the rebinned histos" << endl ;
    
    //PG loop over samples
    //Add eventWeight for the pT weighting of the Higgs in the gg samples
    index = 0 ;
    for (map<string, TChain *>::iterator iColl = collections.begin () ;
    iColl != collections.end () ; 
    ++iColl)
    {    
      
      TCut cutSignal = generalCut && signal && flavourCut[iFlav] ;
      TCut cutSignalExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight", cutSignal.GetTitle (), LUMI) ;      
      
      TCut cutSideband = generalCut && (upper || lower)  && flavourCut[iFlav] ;
      TCut cutSidebandExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight", cutSideband.GetTitle (), LUMI) ;    
      
      cout << " reading " << iColl->first << endl ;
      if (iColl->first.find ("ggH") != string::npos || iColl->first.find ("qqH") != string::npos) 
	
	continue ;
      
      if (iColl->first == "DATA")
	
	continue ;
      
      
      h_m4_signal_reBinned[iFlav] = m4_signal_reBinned[iFlav] -> addSample (iColl->first.c_str ()) ;
      h_m4_sideband_reBinned[iFlav] = m4_sideband_reBinned[iFlav] -> addSample (iColl->first.c_str ()) ;
      h_m4_signal_reBinned[iFlav]   -> GetXaxis() -> Set(xbins.size() - 1, &xbins[0]) ;
      h_m4_sideband_reBinned[iFlav] -> GetXaxis() -> Set(xbins.size() - 1, &xbins[0]) ;
      
      cout << "   getting chain" << endl ;
      
      TChain * chain = iColl->second ;
      VBFAnalysisVariables vars ;
      
      cout << "   filling histos" << endl ;
      
      int IND = 0 ;
      cout << "      index " << IND++ << endl ; iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_signal_reBinned[iFlav]->GetName (), cutSignalExtended);
      cout << "      index " << IND++ << endl ; iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_sideband_reBinned[iFlav]->GetName (), cutSidebandExtended) ;
      
      ++index ;   
    } //PG loop over samples
    
  } //loop on flavours
  
  
  TFile* outputRootFile = new TFile (outputRootFullFileName.c_str (), "RECREATE") ;
  outputRootFile->cd () ;
  
  for ( int iFlav = 0; iFlav < nFlavourCombinations; iFlav++ )  {
    
    m4_upper[iFlav] -> save (outputRootFile) ;
    m4_upper_c[iFlav] -> save (outputRootFile) ;
    m4_upper_a[iFlav] -> save (outputRootFile) ;
    m4_EvenHigher[iFlav] -> save (outputRootFile) ;
    m4_signal[iFlav] -> save (outputRootFile) ;
    m4_lower[iFlav] -> save (outputRootFile) ;
    m4_lower_c[iFlav] -> save (outputRootFile) ;
    m4_lower_a[iFlav] -> save (outputRootFile) ;
    m4_sideband[iFlav] -> save (outputRootFile) ;
    
    m4_signal_reBinned[iFlav] -> save (outputRootFile) ;
    m4_sideband_reBinned[iFlav] -> save (outputRootFile) ;
    
    m4_upper_DATA[iFlav]->Write () ;   
    m4_upper_c_DATA[iFlav]->Write () ;   
    m4_upper_a_DATA[iFlav]->Write () ;   
    m4_EvenHigher_DATA[iFlav]->Write () ;   
    m4_signal_DATA[iFlav]->Write () ;  
    m4_lower_DATA[iFlav]->Write () ;   
    m4_lower_c_DATA[iFlav]->Write () ;   
    m4_lower_a_DATA[iFlav]->Write () ;   
    m4_sideband_DATA[iFlav]->Write () ;
    
    m4_upper_SIG[iFlav] -> save (outputRootFile) ;   
    m4_upper_c_SIG[iFlav] -> save (outputRootFile) ;   
    m4_upper_a_SIG[iFlav] -> save (outputRootFile) ;   
    m4_EvenHigher_SIG[iFlav] -> save (outputRootFile) ;   
    m4_signal_SIG[iFlav] -> save (outputRootFile) ;  
    m4_lower_SIG[iFlav] -> save (outputRootFile) ;   
    m4_lower_c_SIG[iFlav] -> save (outputRootFile) ;   
    m4_lower_a_SIG[iFlav] -> save (outputRootFile) ;   
    m4_sideband_SIG[iFlav] -> save (outputRootFile) ;
    
  }
  
  outputRootFile->Close () ;
  delete outputRootFile ;
  
  return 0 ;
}
