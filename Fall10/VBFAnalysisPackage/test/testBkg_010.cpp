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

// #include "PURescaleFactor.C"
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

  float LUMI = 2145. ; //PG to have output in /fb
  LUMI *= 0.5 ; //PG to have output in /fb with half the stats

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

  hColl Em4_EvenHigher ("Em4_EvenHigher", 70, 100., 800.) ;
  hColl Em4_upper ("Em4_upper", 70, 100., 800.) ;
  hColl Em4_signal ("Em4_signal", 70, 100., 800.) ;
  hColl Em4_lower ("Em4_lower", 70, 100., 800.) ;
  hColl Em4_sideband ("Em4_sideband", 70, 100., 800.) ;

  TH1F * Em4_EvenHigher_DATA = new TH1F ("Em4_EvenHigher_DATA", "Em4_EvenHigher_DATA", 70, 100., 800.) ;
  TH1F * Em4_upper_DATA = new TH1F ("Em4_upper_DATA", "Em4_upper_DATA", 70, 100., 800.) ;
  TH1F * Em4_signal_DATA = new TH1F ("Em4_signal_DATA", "Em4_signal_DATA", 70, 100., 800.) ;
  TH1F * Em4_lower_DATA = new TH1F ("Em4_lower_DATA", "Em4_lower_DATA", 70, 100., 800.) ;
  TH1F * Em4_sideband_DATA = new TH1F ("Em4_sideband_DATA", "Em4_sideband_DATA", 70, 100., 800.) ;

  hColl Om4_EvenHigher ("Om4_EvenHigher", 70, 100., 800.) ;
  hColl Om4_upper ("Om4_upper", 70, 100., 800.) ;
  hColl Om4_signal ("Om4_signal", 70, 100., 800.) ;
  hColl Om4_lower ("Om4_lower", 70, 100., 800.) ;
  hColl Om4_sideband ("Om4_sideband", 70, 100., 800.) ;

  TH1F * Om4_EvenHigher_DATA = new TH1F ("Om4_EvenHigher_DATA", "Om4_EvenHigher_DATA", 70, 100., 800.) ;
  TH1F * Om4_upper_DATA = new TH1F ("Om4_upper_DATA", "Om4_upper_DATA", 70, 100., 800.) ;
  TH1F * Om4_signal_DATA = new TH1F ("Om4_signal_DATA", "Om4_signal_DATA", 70, 100., 800.) ;
  TH1F * Om4_lower_DATA = new TH1F ("Om4_lower_DATA", "Om4_lower_DATA", 70, 100., 800.) ;
  TH1F * Om4_sideband_DATA = new TH1F ("Om4_sideband_DATA", "Om4_sideband_DATA", 70, 100., 800.) ;

  //PG the cuts
  TCut generalCut = "" ;
  generalCut = generalCut && "1 == 1" ;
  
  TCut lower = "WJJ_m > 50 && WJJ_m < 65" ;
  TCut signal = "WJJ_m > 65 && WJJ_m < 95" ;
  TCut upper = "WJJ_m > 95 && WJJ_m < 130" ;
  TCut EvenHigher = "WJJ_m > 130 && WJJ_m < 400" ;
  TCut isEvenEvent = "eventNaiveId % 2 == 0" ;

  string PUweight = "(%s) * 1./totEvents * crossSection * %f * PURescaleFactor((PUit_n+PUoot_n)/3.)" ;

  //PG loop over samples
  int index = 0 ;
  for (map<string, TChain *>::iterator iColl = collections.begin () ;
       iColl != collections.end () ; 
       ++iColl)
    {

      if (iColl->first.find ("ggH") != string::npos) continue ;
      if (iColl->first.find ("qqH") != string::npos) continue ;
//      if (iColl->first == "ggH") continue ;

      TCut evenCutLower = generalCut && isEvenEvent && lower ;
      TCut evenCutLowerExtended = Form (PUweight.c_str (), evenCutLower.GetTitle (), LUMI) ;    
      TCut evenCutSignal = generalCut && isEvenEvent && signal ;
      TCut evenCutSignalExtended = Form (PUweight.c_str (), evenCutSignal.GetTitle (), LUMI) ;    
      TCut evenCutUpper = generalCut && isEvenEvent && upper ;
      TCut evenCutUpperExtended = Form (PUweight.c_str (), evenCutUpper.GetTitle (), LUMI) ;    
      TCut evenCutEvenHigher = generalCut && isEvenEvent && EvenHigher ;
      TCut evenCutEvenHigherExtended = Form (PUweight.c_str (), evenCutEvenHigher.GetTitle (), LUMI) ;    
      TCut evenCutSideband = generalCut && isEvenEvent && (EvenHigher || lower) ;
      TCut evenCutSidebandExtended = Form (PUweight.c_str (), evenCutSideband.GetTitle (), LUMI) ;    

      TCut oddCutLower = generalCut && !isEvenEvent && lower ;
      TCut oddCutLowerExtended = Form (PUweight.c_str (), oddCutLower.GetTitle (), LUMI) ;    
      TCut oddCutSignal = generalCut && !isEvenEvent && signal ;
      TCut oddCutSignalExtended = Form (PUweight.c_str (), oddCutSignal.GetTitle (), LUMI) ;    
      TCut oddCutUpper = generalCut && !isEvenEvent && upper ;
      TCut oddCutUpperExtended = Form (PUweight.c_str (), oddCutUpper.GetTitle (), LUMI) ;    
      TCut oddCutEvenHigher = generalCut && !isEvenEvent && EvenHigher ;
      TCut oddCutEvenHigherExtended = Form (PUweight.c_str (), oddCutEvenHigher.GetTitle (), LUMI) ;    
      TCut oddCutSideband = generalCut && !isEvenEvent && (EvenHigher || lower) ;
      TCut oddCutSidebandExtended = Form (PUweight.c_str (), oddCutSideband.GetTitle (), LUMI) ;    

      if (iColl->first == "DATA")
        {
          iColl->second->Draw ("lepNuW_m >> Em4_lower_DATA", evenCutLower) ;
          iColl->second->Draw ("lepNuW_m >> Em4_signal_DATA", evenCutSignal) ;
          iColl->second->Draw ("lepNuW_m >> Em4_upper_DATA", evenCutUpper) ;
          iColl->second->Draw ("lepNuW_m >> Em4_EvenHigher_DATA", evenCutEvenHigher) ;
          iColl->second->Draw ("lepNuW_m >> Em4_sideband_DATA", evenCutSideband) ;
        
          iColl->second->Draw ("lepNuW_m >> Om4_lower_DATA", oddCutLower) ;
          iColl->second->Draw ("lepNuW_m >> Om4_signal_DATA", oddCutSignal) ;
          iColl->second->Draw ("lepNuW_m >> Om4_upper_DATA", oddCutUpper) ;
          iColl->second->Draw ("lepNuW_m >> Om4_EvenHigher_DATA", oddCutEvenHigher) ;
          iColl->second->Draw ("lepNuW_m >> Om4_sideband_DATA", oddCutSideband) ;
        
          continue ;
        }  
      cout << " reading " << iColl->first << endl ;
      
      TH1F * h_Em4_lower = Em4_lower.addSample (iColl->first.c_str ()) ;
      TH1F * h_Em4_signal = Em4_signal.addSample (iColl->first.c_str ()) ;
      TH1F * h_Em4_upper = Em4_upper.addSample (iColl->first.c_str ()) ;
      TH1F * h_Em4_EvenHigher = Em4_EvenHigher.addSample (iColl->first.c_str ()) ;
      TH1F * h_Em4_sideband = Em4_sideband.addSample (iColl->first.c_str ()) ;

      TH1F * h_Om4_lower = Om4_lower.addSample (iColl->first.c_str ()) ;
      TH1F * h_Om4_signal = Om4_signal.addSample (iColl->first.c_str ()) ;
      TH1F * h_Om4_upper = Om4_upper.addSample (iColl->first.c_str ()) ;
      TH1F * h_Om4_EvenHigher = Om4_EvenHigher.addSample (iColl->first.c_str ()) ;
      TH1F * h_Om4_sideband = Om4_sideband.addSample (iColl->first.c_str ()) ;

      TChain * chain = iColl->second ;
      VBFAnalysisVariables vars ;
      SetVBFPreselectionTreeBranches (vars, chain) ;

      iColl->second->Draw (TString ("lepNuW_m >> ") + h_Em4_lower->GetName (), evenCutLowerExtended) ;
      iColl->second->Draw (TString ("lepNuW_m >> ") + h_Em4_signal->GetName (), evenCutSignalExtended) ;
      iColl->second->Draw (TString ("lepNuW_m >> ") + h_Em4_upper->GetName (), evenCutUpperExtended) ;
      iColl->second->Draw (TString ("lepNuW_m >> ") + h_Em4_EvenHigher->GetName (), evenCutEvenHigherExtended) ;
      iColl->second->Draw (TString ("lepNuW_m >> ") + h_Em4_sideband->GetName (), evenCutSidebandExtended) ;
      
      iColl->second->Draw (TString ("lepNuW_m >> ") + h_Om4_lower->GetName (), oddCutLowerExtended) ;
      iColl->second->Draw (TString ("lepNuW_m >> ") + h_Om4_signal->GetName (), oddCutSignalExtended) ;
      iColl->second->Draw (TString ("lepNuW_m >> ") + h_Om4_upper->GetName (), oddCutUpperExtended) ;
      iColl->second->Draw (TString ("lepNuW_m >> ") + h_Om4_EvenHigher->GetName (), oddCutEvenHigherExtended) ;
      iColl->second->Draw (TString ("lepNuW_m >> ") + h_Om4_sideband->GetName (), oddCutSidebandExtended) ;
      
      ++index ;   
    } //PG loop over samples

  // define out file names
//  std::string outputRootFullFileName = "testBkg_004.root" ;
  std::string outputRootFullFileName = "testBkg_010.root" ;
  TFile* outputRootFile = new TFile (outputRootFullFileName.c_str (), "RECREATE") ;
  outputRootFile->cd () ;

  Em4_upper.save (outputRootFile) ;
  Em4_EvenHigher.save (outputRootFile) ;
  Em4_signal.save (outputRootFile) ;
  Em4_lower.save (outputRootFile) ;
  Em4_sideband.save (outputRootFile) ;

  Em4_upper_DATA->Write () ;   
  Em4_EvenHigher_DATA->Write () ;   
  Em4_signal_DATA->Write () ;  
  Em4_lower_DATA->Write () ;   
  Em4_sideband_DATA->Write () ;

  Om4_upper.save (outputRootFile) ;
  Om4_EvenHigher.save (outputRootFile) ;
  Om4_signal.save (outputRootFile) ;
  Om4_lower.save (outputRootFile) ;
  Om4_sideband.save (outputRootFile) ;

  Om4_upper_DATA->Write () ;   
  Om4_EvenHigher_DATA->Write () ;   
  Om4_signal_DATA->Write () ;  
  Om4_lower_DATA->Write () ;   
  Om4_sideband_DATA->Write () ;

  outputRootFile->Close () ;
  delete outputRootFile ;
  
  
  
  return 0 ;
}

/*

#include  <./test/plotUtils.C>



*/

