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

#include <cstdlib>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>

#include "RooKeysPdf.h"
#include "RooHistPdf.h"
#include "RooDataHist.h"
#include "RooRealVar.h"

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
  if (argc != 2)
    {
      std::cerr << ">>>>> VBFAnalysis::usage: " << argv[0] << " configFileName" << std::endl ;
      return 1 ;
    }
  
  RooRealVar x ("x", "higgs mass", 200., 1000.) ;
  int xBinning = 160 ; //PG 5 GeV binning
  //int xBinning = 80 ; //PG 10 GeV binning
  
  float LUMI = 2145. ; //PG to have output in 1/fb
  x.setBins (xBinning) ;

  string inputFileList (argv[1]) ;
  cout << "samples " << inputFileList << endl ;

  map<string, TChain *> collections ;
  string treeName = "ntu_14" ;
  ReadFile (collections, inputFileList, treeName) ;
  
  TH1::SetDefaultSumw2 (kTRUE) ;

  map<string, hColl *> signalz ;
  hColl signal_250 ("signal_250", 80, 200., 1000.) ; signalz["250"] = &signal_250 ;
  hColl signal_300 ("signal_300", 80, 200., 1000.) ; signalz["300"] = &signal_300 ;
  hColl signal_350 ("signal_350", 80, 200., 1000.) ; signalz["350"] = &signal_350 ;
  hColl signal_400 ("signal_400", 80, 200., 1000.) ; signalz["400"] = &signal_400 ;
  hColl signal_450 ("signal_450", 80, 200., 1000.) ; signalz["450"] = &signal_450 ;
  hColl signal_500 ("signal_500", 80, 200., 1000.) ; signalz["500"] = &signal_500 ;
  hColl signal_550 ("signal_550", 80, 200., 1000.) ; signalz["550"] = &signal_550 ;
  hColl signal_600 ("signal_600", 80, 200., 1000.) ; signalz["600"] = &signal_600 ;

  //PG loop over samples
  for (map<string, TChain *>::iterator iColl = collections.begin () ;
       iColl != collections.end () ; 
       ++iColl)
    {
      TCut cutSignal = "WJJ_m > 65  && WJJ_m < 95" ;
//      TCut cutSignalExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PURescaleFactor((PUit_n+PUoot_n)/3.)", cutSignal.GetTitle (), LUMI) ;    
      TCut cutSignalExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PURescaleFactor(PUit_n)", cutSignal.GetTitle (), LUMI) ;    

      //PG search for signals
      if (iColl->first.find ("ggH") == string::npos && 
          iColl->first.find ("qqH") == string::npos) continue ;

      cout << " reading " << iColl->first << endl ;
      string mass = iColl->first ;
      mass.erase (0, 3) ;
      cout << "MASS : " << mass << endl ;

      TH1F * histo = signalz[mass]->addSample (iColl->first.c_str ()) ;
      iColl->second->Draw (TString ("lepNuW_m_KF >> ") + histo->GetName (), cutSignalExtended) ;

    } //PG loop over samples
  cout << "filling done " << endl ;    

  TFile* outputRootFile = new TFile ("signalShapes.root", "RECREATE") ;
  outputRootFile->cd () ;
  for (map<string, hColl*>::const_iterator iMap = signalz.begin () ; 
       iMap != signalz.end () ; 
       ++iMap) 
    {
      cout << "saving " << iMap->first << endl ;
//      iMap->second->save (outputRootFile) ;
      THStack stack = iMap->second->makeStack () ;
      TH1F * total = (TH1F *) stack.GetStack ()->Last () ;
      cout << "   TH1F integral : " << total->Integral () << endl ;

      stringstream nameRdh ;
      nameRdh << "RDH_" << iMap->first ;
//      RooRealVar x ("x", "higgs mass", 200., 1000.) ;
      RooDataHist bdata (nameRdh.str ().c_str (), nameRdh.str ().c_str (), RooArgList (x), total) ;

      stringstream namePdf ;
      namePdf << "PDF_" << iMap->first ;
      RooHistPdf pdf (namePdf.str ().c_str (), namePdf.str ().c_str (), RooArgList (x), bdata, 8) ;
      pdf.Write () ;

      stringstream name ; 
      name << "sig_" << iMap->first ;
      total->Write (name.str ().c_str ()) ;
    }
  outputRootFile->Close () ;
  delete outputRootFile ;
  
  return 0 ;
}

/*

#include  <./test/plotUtils.C>



*/

