/*
testBkg_017.exe cfg/testBkg_017-fileList.txt  

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
  string treeName = "ntu_13" ; //PG all the preselections + deta_jj
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
//   generalCut = generalCut && "lep_flavour == 13"; //PG only muons
//   generalCut = generalCut && "lep_flavour == 11"; //PG only electrons
//  generalCut = generalCut && "(helicityLikelihood > 0.5)" ; //PG helicity likelihood di HZZ
//  generalCut = generalCut && "WJJ_pt > 40" ; //PG pt cut on hadronic W
//  generalCut = generalCut && "lepMetW_mt > 40" ; //PG mt cut on leptonic W
//  generalCut = generalCut && "lep_flavour == 13" && "nJets_cnt_pt30 == 2" ; //PG only muons 2 jet
//  generalCut = generalCut && "lep_flavour == 13" && "nJets_cnt_pt30 != 2" ; //PG only muons 3 jet
//  generalCut = generalCut && "lep_flavour == 11" && "nJets_cnt_pt30 == 2" ; //PG only ele 2 jet
//  generalCut = generalCut && "lep_flavour == 11" && "nJets_cnt_pt30 != 2" ; //PG only ele 3 jet
  
  std::string outputRootFullFileName = "testBkg_017_S" + mass + "_2011AB.root" ;
//  std::string outputRootFullFileName = "testBkg_017_S" + mass + "_LI_2011AB.root" ;  //PG helicity likelihood di HZZ
//   std::string outputRootFullFileName = "testBkg_017_mu_S" + mass + "_2011AB_10GeVbin.root" ;
//   std::string outputRootFullFileName = "testBkg_017_el_S" + mass + "_2011AB_10GeVbin.root" ;
//   std::string outputRootFullFileName = "testBkg_004_el_S" + mass + "_2011AB.root" ;
  

  TString m4_VAR = "lepNuW_m_KF" ;

  hColl m4_EvenHigher ("m4_EvenHigher", nBins, m4_min, m4_max) ;
  hColl m4_upper      ("m4_upper",      nBins, m4_min, m4_max) ;
  hColl m4_upper_c    ("m4_upper_c",    nBins, m4_min, m4_max) ; // close
  hColl m4_upper_a    ("m4_upper_a",    nBins, m4_min, m4_max) ; // away
  hColl m4_signal     ("m4_signal",     nBins, m4_min, m4_max) ;
  hColl m4_lower      ("m4_lower",      nBins, m4_min, m4_max) ;
  hColl m4_lower_c    ("m4_lower_c",    nBins, m4_min, m4_max) ; // close
  hColl m4_lower_a    ("m4_lower_a",    nBins, m4_min, m4_max) ; // away
  hColl m4_sideband   ("m4_sideband",   nBins, m4_min, m4_max) ;

  TH1F * m4_EvenHigher_DATA = new TH1F ("m4_EvenHigher_DATA", "m4_EvenHigher_DATA", nBins, m4_min, m4_max) ;
  TH1F * m4_upper_DATA      = new TH1F ("m4_upper_DATA",      "m4_upper_DATA",      nBins, m4_min, m4_max) ;
  TH1F * m4_upper_c_DATA    = new TH1F ("m4_upper_c_DATA",    "m4_upper_c_DATA",    nBins, m4_min, m4_max) ;
  TH1F * m4_upper_a_DATA    = new TH1F ("m4_upper_a_DATA",    "m4_upper_a_DATA",    nBins, m4_min, m4_max) ;
  TH1F * m4_signal_DATA     = new TH1F ("m4_signal_DATA",     "m4_signal_DATA",     nBins, m4_min, m4_max) ;
  TH1F * m4_lower_DATA      = new TH1F ("m4_lower_DATA",      "m4_lower_DATA",      nBins, m4_min, m4_max) ;
  TH1F * m4_lower_c_DATA    = new TH1F ("m4_lower_c_DATA",    "m4_lower_c_DATA",    nBins, m4_min, m4_max) ;
  TH1F * m4_lower_a_DATA    = new TH1F ("m4_lower_a_DATA",    "m4_lower_a_DATA",    nBins, m4_min, m4_max) ;
  TH1F * m4_sideband_DATA   = new TH1F ("m4_sideband_DATA",   "m4_sideband_DATA",   nBins, m4_min, m4_max) ;

  hColl m4_EvenHigher_SIG ("m4_EvenHigher_SIG", nBins, m4_min, m4_max) ;
  hColl m4_upper_SIG      ("m4_upper_SIG",      nBins, m4_min, m4_max) ;
  hColl m4_upper_c_SIG    ("m4_upper_c_SIG",    nBins, m4_min, m4_max) ;
  hColl m4_upper_a_SIG    ("m4_upper_a_SIG",    nBins, m4_min, m4_max) ;
  hColl m4_signal_SIG     ("m4_signal_SIG",     nBins, m4_min, m4_max) ;
  hColl m4_lower_SIG      ("m4_lower_SIG",      nBins, m4_min, m4_max) ;
  hColl m4_lower_c_SIG    ("m4_lower_c_SIG",    nBins, m4_min, m4_max) ;
  hColl m4_lower_a_SIG    ("m4_lower_a_SIG",    nBins, m4_min, m4_max) ;
  hColl m4_sideband_SIG   ("m4_sideband_SIG",   nBins, m4_min, m4_max) ;

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

      TCut cutLower = generalCut && lower ;
      TCut cutLowerExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight", cutLower.GetTitle (), LUMI) ;    

      TCut cutLower_c = generalCut && lower_c ;
      TCut cutLower_cExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight", cutLower_c.GetTitle (), LUMI) ;    

      TCut cutLower_a = generalCut && lower_a ;
      TCut cutLower_aExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight", cutLower_a.GetTitle (), LUMI) ;    

      TCut cutSignal = generalCut && signal ;
      TCut cutSignalExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight", cutSignal.GetTitle (), LUMI) ;    

      TCut cutUpper = generalCut && upper ;
      TCut cutUpperExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight", cutUpper.GetTitle (), LUMI) ;    

      TCut cutUpper_c = generalCut && upper_c ;
      TCut cutUpper_cExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight", cutUpper_c.GetTitle (), LUMI) ;    

      TCut cutUpper_a = generalCut && upper_a ;
      TCut cutUpper_aExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight", cutUpper_a.GetTitle (), LUMI) ;    

      TCut cutEvenHigher = generalCut && EvenHigher ;
      TCut cutEvenHigherExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight", cutEvenHigher.GetTitle (), LUMI) ;    

      TCut cutSideband = generalCut && (upper || lower) ;
      TCut cutSidebandExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight", cutSideband.GetTitle (), LUMI) ;    

      cout << " reading " << iColl->first << endl ;
      if (iColl->first.find ("ggH") != string::npos || iColl->first.find ("qqH") != string::npos) 
        {
          if (iColl->first.find (mass.c_str ()) == string::npos) 
            {
              cout << "...skipping" << endl ;
              continue ; 
            } 
                
          TH1F * h_m4_lower_SIG = m4_lower_SIG.addSample (iColl->first.c_str ()) ;
          TH1F * h_m4_lower_c_SIG = m4_lower_c_SIG.addSample (iColl->first.c_str ()) ;
          TH1F * h_m4_lower_a_SIG = m4_lower_a_SIG.addSample (iColl->first.c_str ()) ;
          TH1F * h_m4_signal_SIG = m4_signal_SIG.addSample (iColl->first.c_str ()) ;
          TH1F * h_m4_upper_SIG = m4_upper_SIG.addSample (iColl->first.c_str ()) ;
          TH1F * h_m4_upper_c_SIG = m4_upper_c_SIG.addSample (iColl->first.c_str ()) ;
          TH1F * h_m4_upper_a_SIG = m4_upper_a_SIG.addSample (iColl->first.c_str ()) ;
          TH1F * h_m4_EvenHigher_SIG = m4_EvenHigher_SIG.addSample (iColl->first.c_str ()) ;
          TH1F * h_m4_sideband_SIG = m4_sideband_SIG.addSample (iColl->first.c_str ()) ;
    
          TChain * chain = iColl->second ;
          VBFAnalysisVariables vars ;
          SetVBFPreselectionTreeBranches (vars, chain) ;
    
          iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_lower_SIG->GetName (), cutLowerExtended) ;
          iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_lower_c_SIG->GetName (), cutLower_cExtended) ;
          iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_lower_a_SIG->GetName (), cutLower_aExtended) ;
          iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_signal_SIG->GetName (), cutSignalExtended) ;
          iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_upper_SIG->GetName (), cutUpperExtended) ;
          iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_upper_c_SIG->GetName (), cutUpper_cExtended) ;
          iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_upper_a_SIG->GetName (), cutUpper_aExtended) ;
          iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_EvenHigher_SIG->GetName (), cutEvenHigherExtended) ;
          iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_sideband_SIG->GetName (), cutSidebandExtended) ;
          
          continue ;
        }
      
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

      cout << "   defining histos" << endl ;
      
      TH1F * h_m4_lower = m4_lower.addSample (iColl->first.c_str ()) ;
      TH1F * h_m4_lower_c = m4_lower_c.addSample (iColl->first.c_str ()) ;
      TH1F * h_m4_lower_a = m4_lower_a.addSample (iColl->first.c_str ()) ;
      TH1F * h_m4_signal = m4_signal.addSample (iColl->first.c_str ()) ;
      TH1F * h_m4_upper = m4_upper.addSample (iColl->first.c_str ()) ;
      TH1F * h_m4_upper_c = m4_upper_c.addSample (iColl->first.c_str ()) ;
      TH1F * h_m4_upper_a = m4_upper_a.addSample (iColl->first.c_str ()) ;
      TH1F * h_m4_EvenHigher = m4_EvenHigher.addSample (iColl->first.c_str ()) ;
      TH1F * h_m4_sideband = m4_sideband.addSample (iColl->first.c_str ()) ;

      cout << "   getting chain" << endl ;

      TChain * chain = iColl->second ;
      VBFAnalysisVariables vars ;
//      SetVBFPreselectionTreeBranches (vars, chain) ;

      cout << "   filling histos" << endl ;

      int IND = 0 ;
      cout << "      index " << IND++ << endl ; iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_lower->GetName (), cutLowerExtended) ;
      cout << "      index " << IND++ << endl ; iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_lower_c->GetName (), cutLower_cExtended) ;
      cout << "      index " << IND++ << endl ; iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_lower_a->GetName (), cutLower_aExtended) ;
      cout << "      index " << IND++ << endl ; iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_signal->GetName (), cutSignalExtended) ;
      cout << "      index " << IND++ << endl ; iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_upper->GetName (), cutUpperExtended) ;
      cout << "      index " << IND++ << endl ; iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_upper_c->GetName (), cutUpper_cExtended) ;
      cout << "      index " << IND++ << endl ; iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_upper_a->GetName (), cutUpper_aExtended) ;
      cout << "      index " << IND++ << endl ; iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_EvenHigher->GetName (), cutEvenHigherExtended) ;
      cout << "      index " << IND++ << endl ; iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_sideband->GetName (), cutSidebandExtended) ;
      
      ++index ;   
    } //PG loop over samples



  //PG get the Wjets histograms from collections
  TH1F * m4_upper_c_WJets = m4_upper_c.findHisto ("m4_upper_c_WJets") ;
  TH1F * m4_upper_a_WJets = m4_upper_a.findHisto ("m4_upper_a_WJets") ;
  TH1F * m4_signal_WJets = m4_signal.findHisto ("m4_signal_WJets") ;

  //FC fix the empty bins in the Wjets sample
  setErrorForEmptyBins_v2 (m4_upper_c_WJets);
  setErrorForEmptyBins_v2 (m4_upper_a_WJets);
  setErrorForEmptyBins_v2 (m4_signal_WJets);
  
  //FC get the signal and sideband histograms from collections
  THStack m4_signalStack   = m4_signal.makeStack () ;
  THStack m4_sidebandStack = m4_sideband.makeStack () ;
  
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
  
  
  
  hColl m4_signal_reBinned     ("m4_signal_reBinned",     xbins.size() - 1, m4_min, m4_max) ;
  hColl m4_sideband_reBinned   ("m4_sideband_reBinned",   xbins.size() - 1, m4_min, m4_max) ;
  
  cout << "\n" ;
  cout << "   loop over bkg samples for filling the rebinned histos" << endl ;
  
  //PG loop over samples
  //Add eventWeight for the pT weighting of the Higgs in the gg samples
  index = 0 ;
  for (map<string, TChain *>::iterator iColl = collections.begin () ;
       iColl != collections.end () ; 
       ++iColl)
    {    

      TCut cutSignal = generalCut && signal ;
      TCut cutSignalExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight", cutSignal.GetTitle (), LUMI) ;      

      TCut cutSideband = generalCut && (upper || lower) ;
      TCut cutSidebandExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight", cutSideband.GetTitle (), LUMI) ;    

      cout << " reading " << iColl->first << endl ;
      if (iColl->first.find ("ggH") != string::npos || iColl->first.find ("qqH") != string::npos) 
          
          continue ;
      
      if (iColl->first == "DATA")

          continue ;

      
      TH1F * h_m4_signal_reBinned = m4_signal_reBinned.addSample (iColl->first.c_str ()) ;
      TH1F * h_m4_sideband_reBinned = m4_sideband_reBinned.addSample (iColl->first.c_str ()) ;
      h_m4_signal_reBinned   -> GetXaxis() -> Set(xbins.size() - 1, &xbins[0]) ;
      h_m4_sideband_reBinned -> GetXaxis() -> Set(xbins.size() - 1, &xbins[0]) ;
      
      cout << "   getting chain" << endl ;

      TChain * chain = iColl->second ;
      VBFAnalysisVariables vars ;

      cout << "   filling histos" << endl ;

      int IND = 0 ;
      cout << "      index " << IND++ << endl ; iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_signal_reBinned->GetName (), cutSignalExtended);
      cout << "      index " << IND++ << endl ; iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_sideband_reBinned->GetName (), cutSidebandExtended) ;
      
      ++index ;   
    } //PG loop over samples
  
  
  cout << "   filling DONE" << endl ;
    
  
  TFile* outputRootFile = new TFile (outputRootFullFileName.c_str (), "RECREATE") ;
  outputRootFile->cd () ;

  m4_upper.save (outputRootFile) ;
  m4_upper_c.save (outputRootFile) ;
  m4_upper_a.save (outputRootFile) ;
  m4_EvenHigher.save (outputRootFile) ;
  m4_signal.save (outputRootFile) ;
  m4_lower.save (outputRootFile) ;
  m4_lower_c.save (outputRootFile) ;
  m4_lower_a.save (outputRootFile) ;
  m4_sideband.save (outputRootFile) ;

  m4_signal_reBinned.save (outputRootFile) ;
  m4_sideband_reBinned.save (outputRootFile) ;
  
  m4_upper_DATA->Write () ;   
  m4_upper_c_DATA->Write () ;   
  m4_upper_a_DATA->Write () ;   
  m4_EvenHigher_DATA->Write () ;   
  m4_signal_DATA->Write () ;  
  m4_lower_DATA->Write () ;   
  m4_lower_c_DATA->Write () ;   
  m4_lower_a_DATA->Write () ;   
  m4_sideband_DATA->Write () ;

  m4_upper_SIG.save (outputRootFile) ;   
  m4_upper_c_SIG.save (outputRootFile) ;   
  m4_upper_a_SIG.save (outputRootFile) ;   
  m4_EvenHigher_SIG.save (outputRootFile) ;   
  m4_signal_SIG.save (outputRootFile) ;  
  m4_lower_SIG.save (outputRootFile) ;   
  m4_lower_c_SIG.save (outputRootFile) ;   
  m4_lower_a_SIG.save (outputRootFile) ;   
  m4_sideband_SIG.save (outputRootFile) ;

  outputRootFile->Close () ;
  delete outputRootFile ;
  
  return 0 ;
}

/*

#include  <./test/plotUtils.C>



*/

