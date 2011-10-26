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
#include "Functions.h"

#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>


#include "TF1.h"
#include "TH1.h"
#include "TProfile.h"
#include "TObject.h"
#include "TRandom3.h"
#include "Math/Vector4D.h"

#include "TMVA/Reader.h"
#include "hColl.h"

// #include "PURescaleFactor.C"
//#pragma link C++ function PURescaleFactor (const int&);

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
  string treeName = "ntu_18" ;
  ReadFile (collections, inputFileList, treeName) ;
  
  TH1::SetDefaultSumw2 (kTRUE) ;

  //PG histos in the signal region
  
  hColl m4_Signal ("m4_Signal", 70, 100., 800.) ;
  TCut cutSignal = "WJJ_m > 65  && WJJ_m < 95" ;
  TCut cutSignalExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PURescaleFactor((PUit_n+PUoot_n)/3.)", cutSignal.GetTitle (), LUMI) ;    

  //PG fill the signal histos
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  //PG loop over samples
  for (map<string, TChain *>::iterator iColl = collections.begin () ;
       iColl != collections.end () ; 
       ++iColl)
    {
      if (iColl->first.find ("ggH") != string::npos || iColl->first.find ("qqH") != string::npos) continue ;
      if (iColl->first == "DATA") continue ;

      TH1F * h_m4_Signal = m4_Signal.addSample (iColl->first.c_str ()) ;
      iColl->second->Draw (TString ("lepNuW_m_KF >> ") + h_m4_Signal->GetName (), cutSignalExtended) ;
    } //PG loop over samples

  THStack SignalStack = m4_Signal.makeStack () ;
  TH1F * SignalHisto = (TH1F *) SignalStack.GetStack ()->Last () ;

  TF1 * SignalFitFunc = new TF1 ("SignalFitFunc", attenuatedCB, 0., 1000., 7) ;
  //PG                        N                             , gaus m, gaus s, joint, exp, fermi E, kT
//  SignalFitFunc->SetParameters (signalRegionMC->Integral () / 5, 200.  , 20.   , 0.1  , 10 , 200    , 10) ;
  SignalFitFunc->SetParameter (0, SignalHisto->Integral () / 5.) ;
  SignalFitFunc->SetParameter (1, 200.) ;
  SignalFitFunc->SetParameter (2, 20.) ;
  SignalFitFunc->SetParameter (3, 0.1) ;
  SignalFitFunc->SetParameter (4, 10.) ;
  SignalFitFunc->SetParameter (5, 200) ;
  SignalFitFunc->SetParameter (6, 10) ;
  SignalFitFunc->SetLineWidth (1) ;
  SignalFitFunc->SetLineColor (kBlue+2) ;
  SignalFitFunc->SetNpx (10000) ;
  SignalHisto->Fit (SignalFitFunc, "Q", "", 160., 800.) ;
  TFitResultPtr fitResultPtr = SignalHisto->Fit (SignalFitFunc, "LQ", "", 160., 800.) ;
  int fitStatus = (int)(fitResultPtr) ;
  //PG FIXME make a drawing to check

  TRandom3 r ;
  int nToys = 500 ;
  TH1F * dummyNum = (TH1F *) SignalHisto->Clone ("dummyNum") ;
  TH1F * dummyDen = (TH1F *) SignalHisto->Clone ("dummyDen") ;
  double intSignal = SignalHisto->Integral () ;
  TH1F alfas ("alfas", "", 100, 0, 10) ;
  TProfile * correctionBand = new TProfile ("correctionBand", "", 70, 100, 800, 0, 10) ; //PG memory leak
  
  double minMin = 30 ;
  double minMax = 120 ;
  double maxMax = 400 ;
  double step = 10. ;
  TH2F summary ("summary", "", 
                (step+minMax-minMin)/step, minMin - 0.5 * step, minMax + 0.5 * step,
                (step+maxMax-minMin)/step, minMin - 0.5 * step, maxMax + 0.5 * step) ;
  //PG loop on the side-bands definition, i.e. min e max
  for (double bandMin = minMin ; bandMin < minMax ; bandMin += step)
    {
      for (double bandMax = bandMin + step ; bandMax < maxMax ; bandMax += step)
        {
          //PG avoid useless job
          if (bandMin > 65 && bandMin < 95) continue ;
          if (bandMax > 65 && bandMax < 95) continue ;
        
          //PG the cut for the sideband
          stringstream sideband_cut ;
          sideband_cut << "!(WJJ_m > 65 && WJJ_m < 95)" 
                       << " && " << "(WJJ_m > " << bandMin
                       << " && " << "WJJ_m < " << bandMax << ")" ;
          cout << sideband_cut.str () << endl ; 
          TCut cutSideband = sideband_cut.str ().c_str () ;
          TCut cutSidebandExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PURescaleFactor((PUit_n+PUoot_n)/3.)", cutSideband.GetTitle (), LUMI) ;    

          //PG the collection for the sideband
          hColl m4_Sideband ("m4_Sideband",   70, 100., 800.) ;

          //PG loop over samples and fill the histos
          for (map<string, TChain *>::iterator iColl = collections.begin () ;
               iColl != collections.end () ; 
               ++iColl)
            {
              if (iColl->first.find ("ggH") != string::npos || iColl->first.find ("qqH") != string::npos) continue ;
              if (iColl->first == "DATA") continue ;
 
              TH1F * h_m4_Sideband = m4_Sideband.addSample (iColl->first.c_str ()) ;
              iColl->second->Draw (TString ("lepNuW_m_KF >> ") + h_m4_Sideband->GetName (), cutSidebandExtended) ;

            } //PG loop over samples

         THStack SidebandStack = m4_Sideband.makeStack () ;
         TH1F * SidebandHisto = (TH1F *) SidebandStack.GetStack ()->Last () ;
       
         TF1 * SidebandFitFunc = new TF1 ("SidebandFitFunc", attenuatedCB, 0., 1000., 7) ;
         SidebandFitFunc->SetParameter (0, SidebandHisto->Integral () / 5.) ;
         SidebandFitFunc->SetParameter (1, 200.) ;
         SidebandFitFunc->SetParameter (2, 20.) ;
         SidebandFitFunc->SetParameter (3, 0.1) ;
         SidebandFitFunc->SetParameter (4, 10.) ;
         SidebandFitFunc->SetParameter (5, 200) ;
         SidebandFitFunc->SetParameter (6, 10) ;
         SidebandFitFunc->SetLineWidth (1) ;
         SidebandFitFunc->SetLineColor (kBlue+2) ;
         SidebandFitFunc->SetNpx (10000) ;
         SidebandHisto->Fit (SidebandFitFunc, "Q", "", 160., 800.) ;
         int fitStatus = 1 ;
         int loops = 0 ; 
         while (fitStatus != 0 && loops < 30)
           {
             TFitResultPtr fitResultPtr = SidebandHisto->Fit (SidebandFitFunc, "LQ", "", 160., 800.) ;
             fitStatus = (int)(fitResultPtr) ;
             ++loops ;
           }
         cout << "`--> " << fitStatus << " @ " << loops << "\n" ;
         if (fitStatus != 0) 
           {
             cout << " `--> SKIPPING\n" ;
             continue ;
           }
         double intSideband = SidebandHisto->Integral () ;

//         TH2F * correctionPlane = new TH2F ("correctionPlane", "", 70, 100, 800, 200, 0, 3) ;
         correctionBand->Reset () ;
         for (int iToy = 0 ; iToy < nToys ; ++iToy)
           {
             if (iToy %(nToys/5) == 0) cout << "toy number " << iToy << "\n" ;
        
             int nNum = r.Poisson (intSignal) ;
             dummyNum->Reset () ;
             dummyNum->FillRandom ("SignalFitFunc", nNum) ;
        
             int nDen = r.Poisson (intSideband) ;
             dummyDen->Reset () ;
             dummyDen->FillRandom ("SidebandFitFunc", nDen) ;
        
             dummyNum->Divide (dummyDen) ;
             for (int iBin = 1 ; iBin <= dummyNum->GetNbinsX () ; ++iBin)
               {
                 correctionBand->Fill (dummyNum->GetBinCenter (iBin), dummyNum->GetBinContent (iBin)) ;
               }
           }
        
         alfas.Reset () ; 
         for (int iBin = 1 ; iBin <= correctionBand->GetNbinsX () ; ++iBin)
           {
             alfas.Fill (correctionBand->GetBinContent (iBin)) ;
           } 

         int binx = 1 + (bandMin - (minMin - 0.5 * step)) / step ;
         int biny = 1 + (bandMax - (minMin - 0.5 * step)) / step ;
         cout << "   ==> " << binx << "," << biny << " " << alfas.GetRMS () << endl ;
         summary.SetBinContent (binx, biny, alfas.GetRMS ()) ;
        }
    } //PG loop on the side-bands definition, i.e. min e max
  
  TFile outputRootFile ("testBkg_012.root", "RECREATE") ;
  outputRootFile.cd () ;
  summary.Write () ;
  outputRootFile.Close () ;
  
  return 0 ;
}

