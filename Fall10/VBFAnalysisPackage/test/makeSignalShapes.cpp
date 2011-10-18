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
  if (argc != 2)
    {
      std::cerr << ">>>>> VBFAnalysis::usage: " << argv[0] << " configFileName" << std::endl ;
      return 1 ;
    }

  float LUMI = 2145. ; //PG to have output in 1/fb

  string inputFileList (argv[1]) ;
  cout << "samples " << inputFileList << endl ;

  map<string, TChain *> collections ;
  string treeName = "ntu_14" ;
  ReadFile (collections, inputFileList, treeName) ;
  
  TH1::SetDefaultSumw2 (kTRUE) ;

  map<int, hColl *> signalz ;
  hColl signal_250 ("signal_250", 100, 0., 1000.) ; signalz[250] = &signal_250 ;
  hColl signal_300 ("signal_300", 100, 0., 1000.) ; signalz[300] = &signal_300 ;
  hColl signal_350 ("signal_350", 100, 0., 1000.) ; signalz[350] = &signal_350 ;
  hColl signal_400 ("signal_400", 100, 0., 1000.) ; signalz[400] = &signal_400 ;
  hColl signal_450 ("signal_450", 100, 0., 1000.) ; signalz[450] = &signal_450 ;
  hColl signal_500 ("signal_500", 100, 0., 1000.) ; signalz[500] = &signal_500 ;
  hColl signal_550 ("signal_550", 100, 0., 1000.) ; signalz[550] = &signal_550 ;
  hColl signal_600 ("signal_600", 100, 0., 1000.) ; signalz[600] = &signal_600 ;

  //PG loop over samples
  for (map<string, TChain *>::iterator iColl = collections.begin () ;
       iColl != collections.end () ; 
       ++iColl)
    {
      TCut cutSignal = "WJJ_m > 65  && WJJ_m < 95" ;
      TCut cutSignalExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PURescaleFactor((PUit_n+PUoot_n)/3.)", cutSignal.GetTitle (), LUMI) ;    

      //PG search for signals
      if (iColl->first.find ("ggH") == string::npos && 
          iColl->first.find ("qqH") == string::npos) continue ;

      cout << " reading " << iColl->first << endl ;
      for (map<int, hColl*>::const_iterator iMap = signalz.begin () ; 
           iMap != signalz.end () ; 
           ++iMap)
        {  
          stringstream mass ; 
          mass << iMap->first ;
          if (iColl->first.find (mass.str ()) != string::npos)
            {
              cout << "filling " << mass.str () << endl ;
              TH1F * histo = iMap->second->addSample (iColl->first.c_str ()) ;
              iColl->second->Draw (TString ("lepNuW_m_KF >> ") + histo->GetName (), cutSignalExtended) ;
            }
        }
    } //PG loop over samples
  cout << "filling done " << endl ;    

  TFile* outputRootFile = new TFile ("signalShapes.root", "RECREATE") ;
  outputRootFile->cd () ;
  for (map<int, hColl*>::const_iterator iMap = signalz.begin () ; 
       iMap != signalz.end () ; 
       ++iMap) 
    {
      cout << "saving " << iMap->first << endl ;
//      iMap->second->save (outputRootFile) ;
      THStack stack = iMap->second->makeStack () ;
      TH1F * total = (TH1F *) stack.GetStack ()->Last () ;

      stringstream nameRdh ;
      nameRdh << "RDH_" << iMap->first ;
      RooRealVar m ("m", "higgs mass", 0., 1000.) ;
      RooDataHist bdata (nameRdh.str ().c_str (), nameRdh.str ().c_str (), RooArgList (m), total) ;
      stringstream namePdf ;
      namePdf << "PDF_" << iMap->first ;
      RooHistPdf pdf (namePdf.str ().c_str (), namePdf.str ().c_str (), RooArgList (m), bdata, 8) ;
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

