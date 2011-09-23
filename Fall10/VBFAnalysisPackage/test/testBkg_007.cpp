/*
testBkg_007.exe cfg/2011-09-19-listaFile.txt

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
#include <sstream>
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
  ifstream inFile (inputList.c_str ()) ;
  string buffer ; 
  while (!inFile.eof ())
    {
      getline (inFile, buffer) ;
      if (buffer != "") 
        { ///---> save from empty line at the end!
          if (buffer.at(0) != '#')
            {
              string dummyName ;
              stringstream line ( buffer );       
              line >> dummyName ; 
              cout << "reading " << dummyName << "... ";
              string dummyLocation ;
              line >> dummyLocation ; 
              cout << dummyLocation << "\n";

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
  cout << "samples read\n" ; 
  return 0 ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int main (int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there
  if (argc != 2)
    {
      cerr << ">>>>> VBFAnalysis::usage: " << argv[0] << " configFileName" << endl ;
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

  hColl m4_coll ("m4", 70, 100., 800.) ;
  TH1F * m4_DATA = new TH1F ("m4_DATA", "m4_DATA", 70, 100., 800.) ;

  hColl m4_sideband_coll ("m4_sideband", 70, 100., 800.) ;
  TH1F * m4_sideband_DATA = new TH1F ("m4_sideband_DATA", "m4_sideband_DATA", 70, 100., 800.) ;

  hColl m4_signal_coll ("m4_signal", 70, 100., 800.) ;
  TH1F * m4_signal_DATA = new TH1F ("m4_signal_DATA", "m4_signal_DATA", 70, 100., 800.) ;

  hColl m2_coll ("m2", 100, 0., 300.) ;
  TH1F * m2_DATA = new TH1F ("m2_DATA", "m2_DATA", 100, 0., 300.) ;

  hColl m2_sideband_coll ("m2_sideband",  100, 0., 300.) ;
  TH1F * m2_sideband_DATA = new TH1F ("m2_sideband_DATA", "m2_sideband_DATA",  100, 0., 300.) ;

  hColl m2_signal_coll ("m2_signal",  100, 0., 300.) ;
  TH1F * m2_signal_DATA = new TH1F ("m2_signal_DATA", "m2_signal_DATA",  100, 0., 300.) ;

  TCut generalCut = "" ;
  generalCut = generalCut && "1 == 1" ;

  double Mjj_width = 15. ; // GeV
  double min = 80. - Mjj_width ;
  double max = 80. + Mjj_width ;
  stringstream s_SignalRegion ; 
  s_SignalRegion << "(WJJ_m > " << min << " && WJJ_m < " << max << ")" ;
  TCut SignalRegion = (s_SignalRegion.str ().c_str ()) ;
  stringstream s_SidebandRegion ;
  s_SidebandRegion << "(WJJ_m > 35. && WJJ_m < " << min << ") || (WJJ_m < 300. && WJJ_m > " << max << ")" ;
  TCut SidebandRegion = (s_SidebandRegion.str ().c_str ()) ;

  //FIXME c'e' il kine fit applicato?
  //PG loop over samples
  int index = 0 ;
  for (map<string, TChain *>::iterator iColl = collections.begin () ;
       iColl != collections.end () ; 
       ++iColl)
    {
      TChain * chain = iColl->second ;
      VBFAnalysisVariables vars ;
      SetVBFPreselectionTreeBranches (vars, chain) ;

      TCut cut = generalCut ;
      TCut cutExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PURescaleFactor((PUit_n+PUoot_n)/3.)", cut.GetTitle (), LUMI) ;

      TCut cutSideband = generalCut && SidebandRegion ;
      TCut cutSidebandExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PURescaleFactor((PUit_n+PUoot_n)/3.)", cutSideband.GetTitle (), LUMI) ;

      TCut cutSignal = generalCut && SignalRegion ;
      TCut cutSignalExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PURescaleFactor((PUit_n+PUoot_n)/3.)", cutSignal.GetTitle (), LUMI) ;

      if (iColl->first == "DATA") 
        {
          iColl->second->Draw (TString ("lepNuW_m >> m4_DATA"), cut) ;
          iColl->second->Draw (TString ("lepNuW_m >> m4_sideband_DATA"), cutSideband) ;
          iColl->second->Draw (TString ("lepNuW_m >> m4_signal_DATA"), cutSignal) ;
          iColl->second->Draw (TString ("WJJ_m >> m2_DATA"), cut) ;
          iColl->second->Draw (TString ("WJJ_m >> m2_sideband_DATA"), cutSideband) ;
          iColl->second->Draw (TString ("WJJ_m >> m2_signal_DATA"), cutSignal) ;
        }
      else
        {
          TH1F * h_m4 = m4_coll.addSample (iColl->first.c_str ()) ;
          iColl->second->Draw (TString ("lepNuW_m >> ") + h_m4->GetName (), cutExtended) ;

          TH1F * h_m4_sideband = m4_sideband_coll.addSample (iColl->first.c_str ()) ;
          iColl->second->Draw (TString ("lepNuW_m >> ") + h_m4_sideband->GetName (), cutSidebandExtended) ;

          TH1F * h_m4_signal = m4_signal_coll.addSample (iColl->first.c_str ()) ;
          iColl->second->Draw (TString ("lepNuW_m >> ") + h_m4_signal->GetName (), cutSignalExtended) ;

          TH1F * h_m2 = m2_coll.addSample (iColl->first.c_str ()) ;
          iColl->second->Draw (TString ("WJJ_m >> ") + h_m2->GetName (), cutExtended) ;

          TH1F * h_m2_sideband = m2_sideband_coll.addSample (iColl->first.c_str ()) ;
          iColl->second->Draw (TString ("WJJ_m >> ") + h_m2_sideband->GetName (), cutSidebandExtended) ;

          TH1F * h_m2_signal = m2_signal_coll.addSample (iColl->first.c_str ()) ;
          iColl->second->Draw (TString ("WJJ_m >> ") + h_m2_signal->GetName (), cutSignalExtended) ;

          cout << iColl->first.c_str () 
               << "\t" << h_m4->GetEntries () 
               << "\t" << h_m4->Integral ()
               << "\n" ;
               
        }
      
      ++index ;   
    } //PG loop over samples

  // define out file names
  string outputRootFileName = "testBkg_007.root" ;
  TFile* outputRootFile = new TFile (outputRootFileName.c_str (), "RECREATE") ;
  outputRootFile->cd () ;

  m4_DATA->Write () ;
  m4_coll.save (outputRootFile) ;

  m4_sideband_DATA->Write () ;
  m4_sideband_coll.save (outputRootFile) ;

  m4_signal_DATA->Write () ;
  m4_signal_coll.save (outputRootFile) ;

  m2_DATA->Write () ;
  m2_coll.save (outputRootFile) ;

  m2_sideband_DATA->Write () ;
  m2_sideband_coll.save (outputRootFile) ;

  m2_signal_DATA->Write () ;
  m2_signal_coll.save (outputRootFile) ;

  outputRootFile->Close () ;
  delete outputRootFile ;
  
  return 0 ;
}

/*

#include  <./test/plotUtils.C>



*/

