/*
testRooFit_004.exe cfg/2011-10-18-listaFile.txt
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
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooGenericPdf.h"
#include "RooWorkspace.h"

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
  if (argc != 2)
    {
      std::cerr << ">>>>> VBFAnalysis::usage: " << argv[0] << " configFileName" << std::endl ;
      return 1 ;
    }
  
  
  // lepNuW_m_KF from signal (mJJ in [65,95] GeV) or sideband (mJJ in [55,65] U [95,115] GeV) region
  string mode = "useSignalRegion" ;
  //string mode = "useSidebandRegion" ;
  
  cout << "Usage mode: " << mode << endl;
  
  
  RooRealVar x ("x", "higgs mass", 200., 800.) ;
  int xBinning = 120 ; // 5 GeV binning
  
  float LUMI = 4236.79 ;
  x.setBins (xBinning) ;

  string inputFileList (argv[1]) ;
  cout << "samples " << inputFileList << endl ;

  map<string, TChain *> collections ;
  string treeName = "ntu_14" ;
  ReadFile (collections, inputFileList, treeName) ;
  
  TH1::SetDefaultSumw2 (kTRUE) ;
  int colors[11] = {kBlue+2, kRed, 10, kCyan+2, kOrange+7, kGray, kMagenta+1, kGreen+2, kOrange, kViolet+2, kRed+3} ;

  RooDataSet * data_DS ;
  map<string, RooDataSet *> sig_DS ;

  TH1::SetDefaultSumw2 (kTRUE) ;

  map<string, hColl *> signalz ;
  hColl signal_250 ("signal_250", 160, 200., 1000.) ; signalz["250"] = &signal_250 ;
  hColl signal_300 ("signal_300", 160, 200., 1000.) ; signalz["300"] = &signal_300 ;
  hColl signal_350 ("signal_350", 160, 200., 1000.) ; signalz["350"] = &signal_350 ;
  hColl signal_400 ("signal_400", 160, 200., 1000.) ; signalz["400"] = &signal_400 ;
  hColl signal_450 ("signal_450", 160, 200., 1000.) ; signalz["450"] = &signal_450 ;
  hColl signal_500 ("signal_500", 160, 200., 1000.) ; signalz["500"] = &signal_500 ;
  hColl signal_550 ("signal_550", 160, 200., 1000.) ; signalz["550"] = &signal_550 ;
  hColl signal_600 ("signal_600", 160, 200., 1000.) ; signalz["600"] = &signal_600 ;

  TCut cutSignal   = "WJJ_m > 65 && WJJ_m < 95 && lepNuW_m_KF >= 200 && lepNuW_m_KF <= 800 " ;
  TCut cutSideband = "((WJJ_m > 55 && WJJ_m < 65) || (WJJ_m > 95 && WJJ_m < 115)) && lepNuW_m_KF >= 200 && lepNuW_m_KF <= 800 " ;
  TCut cutExtended ;
  
  if (mode == "useSignalRegion")
   cutExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight", cutSignal.GetTitle (), LUMI) ;    

  if (mode == "useSidebandRegion")
   cutExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight", cutSideband.GetTitle (), LUMI) ;    
   
   
  //PG loop over samples
  int index = 0 ;
  for (map<string, TChain *>::iterator iColl = collections.begin () ;
       iColl != collections.end () ; 
       ++iColl)
    {
      //PG in case of higgs signal
      if (iColl->first.find ("ggH") != string::npos
          || iColl->first.find ("qqH") != string::npos) 
        {
          cout << " reading collection " << iColl->first << endl ;
          string mass = iColl->first ;
          mass.erase (0, 3) ;
          cout << "MASS : " << mass << endl ;
          
          //PG fill the histograms
          //PG --- --- --- --- --- --- --- --- --- --- ---
                  
          TH1F * histo = signalz[mass]->addSample (iColl->first.c_str ()) ;
          iColl->second->Draw (TString ("lepNuW_m_KF >> ") + histo->GetName (), cutExtended) ;


          //PG fill the RooDataSets
          //PG --- --- --- --- --- --- --- --- --- --- ---

          RooDataSet * sig ;
          if (sig_DS.find (mass) == sig_DS.end ()) 
            {
              string name = "Higgs_" + mass ;
              sig = new RooDataSet (name.c_str (), name.c_str (), RooArgSet (x)) ; 
              cout << "creating " << name << " RooDataSet" << endl ;
              sig_DS[mass] = sig ;
            }
          else 
            {
              sig = sig_DS[mass] ;
              cout << "found " << sig->GetName () << " RooDataSet" << endl ;
            }

          float WJJ_m ;
          iColl->second->SetBranchAddress ("WJJ_m", &WJJ_m) ;
          float lepNuW_m_KF ;
          iColl->second->SetBranchAddress ("lepNuW_m_KF", &lepNuW_m_KF) ;
          float crossSection ;
          iColl->second->SetBranchAddress ("crossSection", &crossSection) ;
          int totEvents ;
          iColl->second->SetBranchAddress ("totEvents", &totEvents) ;
          float PUWeight ;
          iColl->second->SetBranchAddress ("PUWeight", &PUWeight) ;
          float eventWeight ;
          iColl->second->SetBranchAddress ("eventWeight", &eventWeight) ;
          

          for (int i = 0 ; i < iColl->second->GetEntries () ; ++i)
            {
              iColl->second->GetEntry (i) ;
              
              if (mode == "useSignalRegion") {
                if (WJJ_m < 65 || WJJ_m > 95) continue ;
                if (lepNuW_m_KF < 200 || lepNuW_m_KF > 800) continue ;
              }
              
              if (mode == "useSidebandRegion") {
                if (WJJ_m < 55 || (WJJ_m > 65 && WJJ_m < 95) || WJJ_m > 115) continue ;
                if (lepNuW_m_KF < 200 || lepNuW_m_KF > 800) continue ;
              }
              
              x = lepNuW_m_KF ;
              double weight = (1./totEvents) * crossSection * LUMI * PUWeight * eventWeight ; 
              sig->add (RooArgSet (x), weight) ;
            }

        } //PG in case of higgs signal

      //PG in case of DATA
      if (iColl->first.find ("DATA") != string::npos) 
        {
          cout << " reading " << iColl->first << endl ;
          RooDataSet * rds = new RooDataSet ("data_unbinned", "data_unbinned", RooArgSet (x)) ; 
          float WJJ_m ;
          iColl->second->SetBranchAddress ("WJJ_m", &WJJ_m) ;
          float lepNuW_m_KF ;
          iColl->second->SetBranchAddress ("lepNuW_m_KF", &lepNuW_m_KF) ;
    
          for (int i = 0 ; i < iColl->second->GetEntries () ; ++i)
            {
              iColl->second->GetEntry (i) ;

              if (mode == "useSignalRegion") {
                if (WJJ_m < 65 || WJJ_m > 95) continue ;
                if (lepNuW_m_KF < 200 || lepNuW_m_KF > 800) continue ;
              }
              
              if (mode == "useSidebandRegion") {
                if (WJJ_m < 55 || (WJJ_m > 65 && WJJ_m < 95) || WJJ_m > 115) continue ;
                if (lepNuW_m_KF < 200 || lepNuW_m_KF > 800) continue ;
              }
              
              x = lepNuW_m_KF ;
              rds->add (RooArgSet (x)) ;            
            }
          data_DS = rds ;
        } //PG in case of DATA
      
      
      //Free the memory used  
      iColl->second->Delete("");

    } //PG loop over samples
  
 

  RooRealVar N ("N", "rel amplitude second exp", 0., 1.) ;
  RooRealVar L1 ("L1", "rel attenuation first exp",  0., 1.) ;
  RooRealVar L2 ("L2", "rel attenuation first exp",  0., 1.) ;
  RooRealVar mu ("mu", "mu",  0., 1.) ;
  RooRealVar kT ("kT", "kT",  0., 1.) ;
  
  RooGenericPdf RFdoubleExp ("RFdoubleExp", "double exponential",
                             //"N1 * exp (-1 * L1 * x) + N2 * exp (-1 * L2 * x)",
                             "exp (-1 * @1 * @0) + @2 * exp (-1 * @3 * @0)",
                             RooArgSet (x, L1, N, L2)) ;
  
  RooGenericPdf RFAttenuatedDoubleExp ("RFAttenuatedDoubleExp", "double exponential with turn-on",
                             "1. / ( exp(-1.*(@0 - @1)/ @2 ) + 1.) * exp (-1 * @3 * @0) + @4 * exp (-1 * @5 * @0)",
                             RooArgSet (x, mu, kT, L1, N, L2)) ;
                             
                             
  RooWorkspace w ("w") ;
  w.import (x) ;
  w.import (N) ;
  w.import (L1) ;
  w.import (L2) ;
  w.import (mu) ;
  w.import (kT) ;
  w.import (RFdoubleExp) ;
  w.import (RFAttenuatedDoubleExp) ;
  //w.import (*data_DS) ;
  RooDataHist * data_obs = new RooDataHist ("data_obs", "data_obs", *data_DS->get (), *data_DS) ;
  w.import (*data_obs) ;
  
  //PG loop on signal histo collections
  for (map<string, hColl*>::const_iterator iMap = signalz.begin () ; 
       iMap != signalz.end () ; 
       ++iMap) 
    {
      THStack stack = iMap->second->makeStack () ;
      TH1F * total = (TH1F *) stack.GetStack ()->Last () ;
      cout << "   TH1F integral : " << total->Integral () << endl ;
      string name = "RDH_" + iMap->first ;


      // *** ROOT v5.33 ***
      //RooDataHist bdata (name.c_str (), name.c_str (), RooArgList (x), total) ;
      //name = "PDF_" + iMap->first ;
      //RooHistPdf pdf (name.c_str (), name.c_str (), RooArgList (x), bdata, 8) ;
      //w.import (pdf) ; 


      // *** ROOT v5.27 ***
      RooDataHist * bdata = new RooDataHist (name.c_str (), name.c_str (), RooArgList (x), total) ;
      name = "PDF_" + iMap->first ;
      RooHistPdf * pdf = new RooHistPdf (name.c_str (), name.c_str (), RooArgList (x), *bdata, 8) ;
      w.import (*pdf) ;

      
      for (int iHist = 0 ; iHist < iMap->second->collection.size () ; ++iHist)
        {
          name = "RDH_" + string (iMap->second->collection.at (iHist)->GetName ()) ;


          // *** ROOT v5.33 ***
          //RooDataHist bdata_single (name.c_str (), name.c_str (), RooArgList (x), iMap->second->collection.at (iHist)) ;
          //name = "PDF_" + string (iMap->second->collection.at (iHist)->GetName ()) ;
          //RooHistPdf pdf_single (name.c_str (), name.c_str (), RooArgList (x), bdata_single, 8) ;
          //w.import (pdf_single) ;
  

          // *** ROOT v5.27 ***
          RooDataHist * bdata_single = new RooDataHist (name.c_str (), name.c_str (), RooArgList (x), iMap->second->collection.at (iHist)) ;
          name = "PDF_" + string (iMap->second->collection.at (iHist)->GetName ()) ;
          RooHistPdf * pdf_single = new RooHistPdf (name.c_str (), name.c_str (), RooArgList (x), *bdata_single, 8) ;
          w.import (*pdf_single) ;
          
        }
    } //PG loop on signal histo collections


/*
  //PG in case I want to use the RooDataSet of signal,
  //PG which ny now does not seem to work, as it returns weird number of events
  for (map<string, RooDataSet *>::iterator iMap = sig_DS.begin () ;
       iMap != sig_DS.end () ;
       ++iMap)
    {      
      cout << "expected rate for mH = " << iMap->first << " : " << iMap->second->sumEntries () << endl ;
      string name = "RDH_" + iMap->first ;
      RooDataHist sig_RDH (name.c_str () ,name.c_str (), RooArgList (x), *(iMap->second)) ;
      name = "PDF_" + iMap->first ;
      RooHistPdf sig_PDF (name.c_str (), name.c_str (), RooArgList (x), sig_RDH, 8) ;
      w.import (sig_PDF) ;    
    } 
*/

  // define out file names
  string outputRootFullFileName ;
  if (mode == "useSignalRegion")   outputRootFullFileName = "limitsWS_signalWJJ.root" ;
  if (mode == "useSidebandRegion") outputRootFullFileName = "limitsWS_sidebandWJJ.root" ;
  
  TFile* outputRootFile = new TFile (outputRootFullFileName.c_str (), "RECREATE") ;
  outputRootFile->cd () ;
  
  w.Write () ;
  w.Print () ;
//  data_DS->Write () ;
  outputRootFile->Close () ;
  delete outputRootFile ;
 
  return 0 ;
}

/*

#include  <./test/plotUtils.C>



*/

