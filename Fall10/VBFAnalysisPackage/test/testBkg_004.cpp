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


int main (int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there
  if (argc != 3)
    {
      std::cerr << ">>>>> VBFAnalysis::usage: " << argv[0] << " configFileName" << std::endl ;
      return 1 ;
    }

  float LUMI = 2145. ; //PG to have output in 1/fb

  string mass = argv[2] ;

  string inputFileList (argv[1]) ;
  cout << "samples " << inputFileList << endl ;

  map<string, TChain *> collections ;
  string treeName = "ntu_18" ;
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
//      |   |                    |
//      |   |     upper          |     
//      |   |                    |
//      |   +--------------------+
//      |   |                    |
//      |   |    signal          |     
//      |   |                    |
//      |   +--------------------+
//      |   |                    |
//      |   |    lower           |     
//      |   |                    |
//      |   +--------------------+
//      +-------------------------------  m4

  int nBins = 140 ;
  double m4_min = 100. ;
  double m4_max = 800. ;
  //PG the cuts
  TCut generalCut = "" ;
  generalCut = generalCut && "1 == 1" ;
//  generalCut = generalCut && "lep_flavour == 13" ; //PG only muons
  generalCut = generalCut && "lep_flavour == 13" ; //PG only electrons
  std::string outputRootFullFileName = "testBkg_004_5GeV_S" + mass + ".root" ;
//  std::string outputRootFullFileName = "testBkg_004_noKF.root" ;


  hColl m4_EvenHigher ("m4_EvenHigher", nBins, m4_min, m4_max) ;
  hColl m4_upper      ("m4_upper",      nBins, m4_min, m4_max) ;
  hColl m4_signal     ("m4_signal",     nBins, m4_min, m4_max) ;
  hColl m4_lower      ("m4_lower",      nBins, m4_min, m4_max) ;
  hColl m4_sideband   ("m4_sideband",   nBins, m4_min, m4_max) ;

  TH1F * m4_EvenHigher_DATA = new TH1F ("m4_EvenHigher_DATA", "m4_EvenHigher_DATA", nBins, m4_min, m4_max) ;
  TH1F * m4_upper_DATA      = new TH1F ("m4_upper_DATA",      "m4_upper_DATA",      nBins, m4_min, m4_max) ;
  TH1F * m4_signal_DATA     = new TH1F ("m4_signal_DATA",     "m4_signal_DATA",     nBins, m4_min, m4_max) ;
  TH1F * m4_lower_DATA      = new TH1F ("m4_lower_DATA",      "m4_lower_DATA",      nBins, m4_min, m4_max) ;
  TH1F * m4_sideband_DATA   = new TH1F ("m4_sideband_DATA",   "m4_sideband_DATA",   nBins, m4_min, m4_max) ;

  hColl m4_EvenHigher_SIG ("m4_EvenHigher_SIG", nBins, m4_min, m4_max) ;
  hColl m4_upper_SIG      ("m4_upper_SIG",      nBins, m4_min, m4_max) ;
  hColl m4_signal_SIG     ("m4_signal_SIG",     nBins, m4_min, m4_max) ;
  hColl m4_lower_SIG      ("m4_lower_SIG",      nBins, m4_min, m4_max) ;
  hColl m4_sideband_SIG   ("m4_sideband_SIG",   nBins, m4_min, m4_max) ;

  TCut lower      = "WJJ_m > 50  && WJJ_m < 65" ;
  TCut signal     = "WJJ_m > 65  && WJJ_m < 95" ;
  TCut upper      = "WJJ_m > 95  && WJJ_m < 130" ;
  TCut EvenHigher = "WJJ_m > 130 && WJJ_m < 400" ;

  //PG loop over samples
  int index = 0 ;
  for (map<string, TChain *>::iterator iColl = collections.begin () ;
       iColl != collections.end () ; 
       ++iColl)
    {

      TCut cutLower = generalCut && lower ;
      TCut cutLowerExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PURescaleFactor((PUit_n+PUoot_n)/3.)", cutLower.GetTitle (), LUMI) ;    

      TCut cutSignal = generalCut && signal ;
      TCut cutSignalExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PURescaleFactor((PUit_n+PUoot_n)/3.)", cutSignal.GetTitle (), LUMI) ;    

      TCut cutUpper = generalCut && upper ;
      TCut cutUpperExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PURescaleFactor((PUit_n+PUoot_n)/3.)", cutUpper.GetTitle (), LUMI) ;    

      TCut cutEvenHigher = generalCut && EvenHigher ;
      TCut cutEvenHigherExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PURescaleFactor((PUit_n+PUoot_n)/3.)", cutEvenHigher.GetTitle (), LUMI) ;    

      TCut cutSideband = generalCut && (upper || lower) ;
      TCut cutSidebandExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PURescaleFactor((PUit_n+PUoot_n)/3.)", cutSideband.GetTitle (), LUMI) ;    

      cout << " reading " << iColl->first << endl ;
      if (iColl->first.find ("ggH") != string::npos || iColl->first.find ("qqH") != string::npos) 
        {
          if (iColl->first.find (mass.c_str ()) == string::npos) 
            {
              cout << "...skipping" << endl ;
              continue ; 
            } 
                
          TH1F * h_m4_lower_SIG = m4_lower_SIG.addSample (iColl->first.c_str ()) ;
          TH1F * h_m4_signal_SIG = m4_signal_SIG.addSample (iColl->first.c_str ()) ;
          TH1F * h_m4_upper_SIG = m4_upper_SIG.addSample (iColl->first.c_str ()) ;
          TH1F * h_m4_EvenHigher_SIG = m4_EvenHigher_SIG.addSample (iColl->first.c_str ()) ;
          TH1F * h_m4_sideband_SIG = m4_sideband_SIG.addSample (iColl->first.c_str ()) ;
    
          TChain * chain = iColl->second ;
          VBFAnalysisVariables vars ;
          SetVBFPreselectionTreeBranches (vars, chain) ;
    
          iColl->second->Draw (TString ("lepNuW_m_KF >> ") + h_m4_lower_SIG->GetName (), cutLowerExtended) ;
          iColl->second->Draw (TString ("lepNuW_m_KF >> ") + h_m4_signal_SIG->GetName (), cutSignalExtended) ;
          iColl->second->Draw (TString ("lepNuW_m_KF >> ") + h_m4_upper_SIG->GetName (), cutUpperExtended) ;
          iColl->second->Draw (TString ("lepNuW_m_KF >> ") + h_m4_EvenHigher_SIG->GetName (), cutEvenHigherExtended) ;
          iColl->second->Draw (TString ("lepNuW_m_KF >> ") + h_m4_sideband_SIG->GetName (), cutSidebandExtended) ;
          
          continue ;
        }

      if (iColl->first == "DATA")
        {
          iColl->second->Draw ("lepNuW_m_KF >> m4_lower_DATA", cutLower) ;
          iColl->second->Draw ("lepNuW_m_KF >> m4_signal_DATA", cutSignal) ;
          iColl->second->Draw ("lepNuW_m_KF >> m4_upper_DATA", cutUpper) ;
          iColl->second->Draw ("lepNuW_m_KF >> m4_EvenHigher_DATA", cutEvenHigher) ;
          iColl->second->Draw ("lepNuW_m_KF >> m4_sideband_DATA", cutSideband) ;
        
          continue ;
        }  
      
      TH1F * h_m4_lower = m4_lower.addSample (iColl->first.c_str ()) ;
      TH1F * h_m4_signal = m4_signal.addSample (iColl->first.c_str ()) ;
      TH1F * h_m4_upper = m4_upper.addSample (iColl->first.c_str ()) ;
      TH1F * h_m4_EvenHigher = m4_EvenHigher.addSample (iColl->first.c_str ()) ;
      TH1F * h_m4_sideband = m4_sideband.addSample (iColl->first.c_str ()) ;

      TChain * chain = iColl->second ;
      VBFAnalysisVariables vars ;
      SetVBFPreselectionTreeBranches (vars, chain) ;

      iColl->second->Draw (TString ("lepNuW_m_KF >> ") + h_m4_lower->GetName (), cutLowerExtended) ;
      iColl->second->Draw (TString ("lepNuW_m_KF >> ") + h_m4_signal->GetName (), cutSignalExtended) ;
      iColl->second->Draw (TString ("lepNuW_m_KF >> ") + h_m4_upper->GetName (), cutUpperExtended) ;
      iColl->second->Draw (TString ("lepNuW_m_KF >> ") + h_m4_EvenHigher->GetName (), cutEvenHigherExtended) ;
      iColl->second->Draw (TString ("lepNuW_m_KF >> ") + h_m4_sideband->GetName (), cutSidebandExtended) ;
      
      ++index ;   
    } //PG loop over samples

  TFile* outputRootFile = new TFile (outputRootFullFileName.c_str (), "RECREATE") ;
  outputRootFile->cd () ;

  m4_upper.save (outputRootFile) ;
  m4_EvenHigher.save (outputRootFile) ;
  m4_signal.save (outputRootFile) ;
  m4_lower.save (outputRootFile) ;
  m4_sideband.save (outputRootFile) ;

  m4_upper_DATA->Write () ;   
  m4_EvenHigher_DATA->Write () ;   
  m4_signal_DATA->Write () ;  
  m4_lower_DATA->Write () ;   
  m4_sideband_DATA->Write () ;

  m4_upper_SIG.save (outputRootFile) ;   
  m4_EvenHigher_SIG.save (outputRootFile) ;   
  m4_signal_SIG.save (outputRootFile) ;  
  m4_lower_SIG.save (outputRootFile) ;   
  m4_sideband_SIG.save (outputRootFile) ;

  outputRootFile->Close () ;
  delete outputRootFile ;
  
  return 0 ;
}

/*

#include  <./test/plotUtils.C>



*/

