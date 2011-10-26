/*
testBkg_008.exe cfg/2011-09-19-listaFile.txt

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
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>


#include "TH1.h"
#include "TGraph.h"
#include "TProfile.h"
#include "TObject.h"
#include "TRandom3.h"
#include "Math/Vector4D.h"

#include "TMVA/Reader.h"
#include "hColl.h"

// #include "PURescaleFactor.C"
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

  vector<TH1F *> comparisons ;
  vector<double> widths ;
  vector<double> stats_signal ;
  vector<double> stats_sideband ;
  vector<double> stats_ratio ;

  //PG loop over different widths
  for (double Mjj_width = 10. ; Mjj_width < 25. ; Mjj_width += 1.)
    {
      TH1::SetDefaultSumw2 (kTRUE) ;
      int colors[11] = {kBlue+2, kRed, 10, kCyan+2, kOrange+7, kGray, kMagenta+1, kGreen+2, kOrange, kViolet+2, kRed+3} ;
    
      hColl m4_coll ("m4", 70, 100., 800.) ;
      hColl m4_sideband_coll ("m4_sideband", 70, 100., 800.) ;
      hColl m4_signal_coll ("m4_signal", 70, 100., 800.) ;
    
      TCut generalCut = "" ;
      generalCut = generalCut && "1 == 1" ;
    
      double min = 80. - Mjj_width ;
      double max = 80. + Mjj_width ;
      stringstream s_SignalRegion ; 
      s_SignalRegion << "(WJJ_m > " << min << " && WJJ_m < " << max << ")" ;
      TCut SignalRegion = (s_SignalRegion.str ().c_str ()) ;
      stringstream s_SidebandRegion ;
//      s_SidebandRegion << "(WJJ_m > 35. && WJJ_m < " << min << ") || (WJJ_m < 300. && WJJ_m > " << max << ")" ;
      s_SidebandRegion << "(WJJ_m > 50. && WJJ_m < " << min << ") || (WJJ_m < 130. && WJJ_m > " << max << ")" ;
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
              continue ;
            }
          else
            {
              TH1F * h_m4 = m4_coll.addSample (iColl->first.c_str ()) ;
              iColl->second->Draw (TString ("lepNuW_m_KF >> ") + h_m4->GetName (), cutExtended) ;
    
              TH1F * h_m4_Sideband = m4_sideband_coll.addSample (iColl->first.c_str ()) ;
              iColl->second->Draw (TString ("lepNuW_m_KF >> ") + h_m4_Sideband->GetName (), cutSidebandExtended) ;
    
              TH1F * h_m4_Signal = m4_signal_coll.addSample (iColl->first.c_str ()) ;
              iColl->second->Draw (TString ("lepNuW_m_KF >> ") + h_m4_Signal->GetName (), cutSignalExtended) ;    
            }
          
          ++index ;   
        } //PG loop over samples
    
      widths.push_back (Mjj_width) ;

      stringstream s_SidebandName ;
      s_SidebandName << "tot_sideband_" << Mjj_width ;
      THStack SidebandStack = m4_sideband_coll.makeStack () ;
      TH1F * tot_Sideband = (TH1F *) SidebandStack.GetStack ()->Last ()->Clone (s_SidebandName.str ().c_str ()) ;
      stats_sideband.push_back (tot_Sideband->Integral ()) ;
      tot_Sideband->Scale (1. / tot_Sideband->Integral ()) ;

      stringstream s_SignalName ;
      s_SignalName << "tot_Signal_" << Mjj_width ;
      THStack SignalStack = m4_signal_coll.makeStack () ;
      TH1F * tot_Signal = (TH1F *) SignalStack.GetStack ()->Last ()->Clone (s_SignalName.str ().c_str ()) ;
      stats_signal.push_back (tot_Signal->Integral ()) ;
      tot_Signal->Scale (1. / tot_Signal->Integral ()) ;
      
      stats_ratio.push_back (stats_signal.back () / stats_sideband.back ()) ;
      stringstream s_RatioName ;
      s_RatioName << "sig_o_side_" << Mjj_width ;
      TH1F * sig_o_side = (TH1F *) tot_Signal->Clone (s_RatioName.str ().c_str ()) ;
      sig_o_side->Divide (tot_Sideband) ;
      comparisons.push_back (sig_o_side) ;      

    }//PG loop over different widths

  TGraph g_stats_signal (widths.size (), &widths.at (0), &stats_signal.at (0)) ;
  TGraph g_stats_sideband (widths.size (), &widths.at (0), &stats_sideband.at (0)) ;
  TGraph g_stats_ratio (widths.size (), &widths.at (0), &stats_ratio.at (0)) ;
  
  // define out file names
  string outputRootFileName = "testBkg_008.root" ;
  TFile* outputRootFile = new TFile (outputRootFileName.c_str (), "RECREATE") ;
  outputRootFile->cd () ;

  for (unsigned int i = 0 ; i < comparisons.size () ; ++i) 
    {
      comparisons.at (i)->Write () ;
    }
  g_stats_signal.Write ("g_stats_signal") ;
  g_stats_sideband.Write ("g_stats_sideband") ;
  g_stats_ratio.Write ("g_stats_ratio") ;

  outputRootFile->Close () ;
  delete outputRootFile ;
  
  return 0 ;
}

/*

#include  <./test/plotUtils.C>



*/

