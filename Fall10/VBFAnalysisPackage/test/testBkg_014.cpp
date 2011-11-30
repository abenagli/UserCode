/*
testBkg_014.exe cfg/2011-10-11-listaFile.txt  


*/


#include "VBFAnalysisVariables.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "kalanand.h"
#include "hFactory.h"
#include "h2Factory.h"
#include "stdHisto.h"
#include "plotUtils.h"

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

  float LUMI = 2145. ; //PG to have output in 1/fb

  string mass = argv[2] ;

  string inputFileList (argv[1]) ;
  cout << "samples " << inputFileList << endl ;

  map<string, TChain *> collections ;
  string treeName = "ntu_14" ;
  ReadFile (collections, inputFileList, treeName) ;
  
  TH1::SetDefaultSumw2 (kTRUE) ;
  int colors[11] = {kBlue+2, kRed, 10, kCyan+2, kOrange+7, kGray, kMagenta+1, kGreen+2, kOrange, kViolet+2, kRed+3} ;


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

  //PG calculated with testBkg_013.cpp
  Float_t xAxis [86] = {0, 165, 170, 175, 180, 185, 190, 195, 200, 205, 210, 215, 220, 225, 230, 235, 240, 245, 250, 255, 260, 265, 270, 275, 280, 285, 290, 295, 300, 305, 310, 315, 320, 325, 330, 335, 340, 345, 350, 355, 360, 365, 370, 375, 380, 385, 390, 395, 400, 405, 410, 415, 420, 425, 430, 435, 440, 445, 450, 455, 460, 465, 470, 475, 480, 485, 490, 500, 505, 515, 520, 525, 535, 545, 555, 570, 585, 595, 615, 630, 645, 665, 700, 765, 870, 1000} ;
  int nBins = 85 ; //PG xAxis elements minus one
  double m4_min = 0. ;
  double m4_max = 1000. ;
  //PG the cuts
  TCut generalCut = "" ;
//  generalCut = generalCut && "WJJ_pt > 40" ; //PG pt cut on hadronic W
//  generalCut = generalCut && "lepMetW_mt > 40" ; //PG mt cut on leptonic W
//  generalCut = generalCut && "lep_flavour == 13" ; //PG only muons
//  generalCut = generalCut && "lep_flavour == 13" ; //PG only electrons
  std::string outputRootFullFileName = "testBkg_004_S" + mass + ".root" ;
//  std::string outputRootFullFileName = "testBkg_004_cuts_S" + mass + ".root" ;
//  std::string outputRootFullFileName = "testBkg_004_noKF_S" + mass + ".root" ;
//  std::string outputRootFullFileName = "testBkg_004_5GeV_S" + mass + ".root" ;
//  std::string outputRootFullFileName = "testBkg_004_20GeV_S" + mass + ".root" ;
//  std::string outputRootFullFileName = "testBkg_004_noKF.root" ;
//  TString m4_VAR = "lepNuW_m" ;
  TString m4_VAR = "lepNuW_m_KF" ;

  hColl m4_EvenHigher ("m4_EvenHigher", nBins, xAxis) ;
  hColl m4_upper      ("m4_upper",      nBins, xAxis) ;
  hColl m4_upper_c    ("m4_upper_c",    nBins, xAxis) ; // close
  hColl m4_upper_a    ("m4_upper_a",    nBins, xAxis) ; // away
  hColl m4_signal     ("m4_signal",     nBins, xAxis) ;
  hColl m4_lower      ("m4_lower",      nBins, xAxis) ;
  hColl m4_lower_c    ("m4_lower_c",    nBins, xAxis) ; // close
  hColl m4_lower_a    ("m4_lower_a",    nBins, xAxis) ; // away
  hColl m4_sideband   ("m4_sideband",   nBins, xAxis) ;

  TH1F * m4_EvenHigher_DATA = new TH1F ("m4_EvenHigher_DATA", "m4_EvenHigher_DATA", nBins, xAxis) ;
  TH1F * m4_upper_DATA      = new TH1F ("m4_upper_DATA",      "m4_upper_DATA",      nBins, xAxis) ;
  TH1F * m4_upper_c_DATA    = new TH1F ("m4_upper_c_DATA",    "m4_upper_c_DATA",    nBins, xAxis) ;
  TH1F * m4_upper_a_DATA    = new TH1F ("m4_upper_a_DATA",    "m4_upper_a_DATA",    nBins, xAxis) ;
  TH1F * m4_signal_DATA     = new TH1F ("m4_signal_DATA",     "m4_signal_DATA",     nBins, xAxis) ;
  TH1F * m4_lower_DATA      = new TH1F ("m4_lower_DATA",      "m4_lower_DATA",      nBins, xAxis) ;
  TH1F * m4_lower_c_DATA    = new TH1F ("m4_lower_c_DATA",    "m4_lower_c_DATA",    nBins, xAxis) ;
  TH1F * m4_lower_a_DATA    = new TH1F ("m4_lower_a_DATA",    "m4_lower_a_DATA",    nBins, xAxis) ;
  TH1F * m4_sideband_DATA   = new TH1F ("m4_sideband_DATA",   "m4_sideband_DATA",   nBins, xAxis) ;

  hColl m4_EvenHigher_SIG ("m4_EvenHigher_SIG", nBins, xAxis) ;
  hColl m4_upper_SIG      ("m4_upper_SIG",      nBins, xAxis) ;
  hColl m4_upper_c_SIG    ("m4_upper_c_SIG",    nBins, xAxis) ;
  hColl m4_upper_a_SIG    ("m4_upper_a_SIG",    nBins, xAxis) ;
  hColl m4_signal_SIG     ("m4_signal_SIG",     nBins, xAxis) ;
  hColl m4_lower_SIG      ("m4_lower_SIG",      nBins, xAxis) ;
  hColl m4_lower_c_SIG    ("m4_lower_c_SIG",    nBins, xAxis) ;
  hColl m4_lower_a_SIG    ("m4_lower_a_SIG",    nBins, xAxis) ;
  hColl m4_sideband_SIG   ("m4_sideband_SIG",   nBins, xAxis) ;

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
      TCut cutLowerExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PURescaleFactor(PUit_n)", cutLower.GetTitle (), LUMI) ;    

      TCut cutLower_c = generalCut && lower_c ;
      TCut cutLower_cExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PURescaleFactor(PUit_n)", cutLower_c.GetTitle (), LUMI) ;    

      TCut cutLower_a = generalCut && lower_a ;
      TCut cutLower_aExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PURescaleFactor(PUit_n)", cutLower_a.GetTitle (), LUMI) ;    

      TCut cutSignal = generalCut && signal ;
      TCut cutSignalExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PURescaleFactor(PUit_n)", cutSignal.GetTitle (), LUMI) ;    

      TCut cutUpper = generalCut && upper ;
      TCut cutUpperExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PURescaleFactor(PUit_n)", cutUpper.GetTitle (), LUMI) ;    

      TCut cutUpper_c = generalCut && upper_c ;
      TCut cutUpper_cExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PURescaleFactor(PUit_n)", cutUpper_c.GetTitle (), LUMI) ;    

      TCut cutUpper_a = generalCut && upper_a ;
      TCut cutUpper_aExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PURescaleFactor(PUit_n)", cutUpper_a.GetTitle (), LUMI) ;    

      TCut cutEvenHigher = generalCut && EvenHigher ;
      TCut cutEvenHigherExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PURescaleFactor(PUit_n)", cutEvenHigher.GetTitle (), LUMI) ;    

      TCut cutSideband = generalCut && (upper || lower) ;
      TCut cutSidebandExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PURescaleFactor(PUit_n)", cutSideband.GetTitle (), LUMI) ;    

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
      
      TH1F * h_m4_lower = m4_lower.addSample (iColl->first.c_str ()) ;
      TH1F * h_m4_lower_c = m4_lower_c.addSample (iColl->first.c_str ()) ;
      TH1F * h_m4_lower_a = m4_lower_a.addSample (iColl->first.c_str ()) ;
      TH1F * h_m4_signal = m4_signal.addSample (iColl->first.c_str ()) ;
      TH1F * h_m4_upper = m4_upper.addSample (iColl->first.c_str ()) ;
      TH1F * h_m4_upper_c = m4_upper_c.addSample (iColl->first.c_str ()) ;
      TH1F * h_m4_upper_a = m4_upper_a.addSample (iColl->first.c_str ()) ;
      TH1F * h_m4_EvenHigher = m4_EvenHigher.addSample (iColl->first.c_str ()) ;
      TH1F * h_m4_sideband = m4_sideband.addSample (iColl->first.c_str ()) ;

      TChain * chain = iColl->second ;
      VBFAnalysisVariables vars ;
      SetVBFPreselectionTreeBranches (vars, chain) ;

      iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_lower->GetName (), cutLowerExtended) ;
      iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_lower_c->GetName (), cutLower_cExtended) ;
      iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_lower_a->GetName (), cutLower_aExtended) ;
      iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_signal->GetName (), cutSignalExtended) ;
      iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_upper->GetName (), cutUpperExtended) ;
      iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_upper_c->GetName (), cutUpper_cExtended) ;
      iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_upper_a->GetName (), cutUpper_aExtended) ;
      iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_EvenHigher->GetName (), cutEvenHigherExtended) ;
      iColl->second->Draw (m4_VAR + TString (" >> ") + h_m4_sideband->GetName (), cutSidebandExtended) ;
      
      ++index ;   
    } //PG loop over samples


  m4_EvenHigher.normToBinWidths () ;
  m4_upper     .normToBinWidths () ;
  m4_upper_c   .normToBinWidths () ;
  m4_upper_a   .normToBinWidths () ;
  m4_signal    .normToBinWidths () ;
  m4_lower     .normToBinWidths () ;
  m4_lower_c   .normToBinWidths () ;
  m4_lower_a   .normToBinWidths () ;
  m4_sideband  .normToBinWidths () ;

  normalizeToBinWidths (m4_EvenHigher_DATA) ;
  normalizeToBinWidths (m4_upper_DATA     ) ;
  normalizeToBinWidths (m4_upper_c_DATA   ) ;
  normalizeToBinWidths (m4_upper_a_DATA   ) ;
  normalizeToBinWidths (m4_signal_DATA    ) ;
  normalizeToBinWidths (m4_lower_DATA     ) ;
  normalizeToBinWidths (m4_lower_c_DATA   ) ;
  normalizeToBinWidths (m4_lower_a_DATA   ) ;
  normalizeToBinWidths (m4_sideband_DATA  ) ;

  m4_EvenHigher_SIG.normToBinWidths () ;
  m4_upper_SIG     .normToBinWidths () ;
  m4_upper_c_SIG   .normToBinWidths () ;
  m4_upper_a_SIG   .normToBinWidths () ;
  m4_signal_SIG    .normToBinWidths () ;
  m4_lower_SIG     .normToBinWidths () ;
  m4_lower_c_SIG   .normToBinWidths () ;
  m4_lower_a_SIG   .normToBinWidths () ;
  m4_sideband_SIG  .normToBinWidths () ;


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

