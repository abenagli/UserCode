/*
testBkg_003.exe cfg/2011-07-09-listaFile.txt

NB qui non c'e' PU reweighting!

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
  if (argc != 2)
    {
      std::cerr << ">>>>> VBFAnalysis::usage: " << argv[0] << " configFileName" << std::endl ;
      return 1 ;
    }

  string inputFileList (argv[1]) ;
  cout << "samples " << inputFileList << endl ;

  map<string, TChain *> collections ;
  std::string treeName = "ntu_13" ;
  ReadFile (collections, inputFileList, treeName) ;

  double btag_cut = 5. ;
  
  TH1::SetDefaultSumw2 (kTRUE) ;
  int colors[11] = {kBlue+2, kRed, 10, kCyan+2, kOrange+7, kGray, kMagenta+1, kGreen+2, kOrange, kViolet+2, kRed+3} ;

//  m2  |        
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

  hColl m4_upper ("m4_upper", 50, 100., 600.) ;
  hColl m4_signal ("m4_signal", 50, 100., 600.) ;
  hColl m4_lower ("m4_lower", 50, 100., 600.) ;

  //PG loop over samples
  int index = 0 ;
  for (map<string, TChain *>::iterator iColl = collections.begin () ;
       iColl != collections.end () ; 
       ++iColl)
    {
      TH1F * h_m4_lower = m4_lower.addSample (iColl->first.c_str ()) ;
      TH1F * h_m4_signal = m4_signal.addSample (iColl->first.c_str ()) ;
      TH1F * h_m4_upper = m4_upper.addSample (iColl->first.c_str ()) ;

      TChain * chain = iColl->second ;
      VBFAnalysisVariables vars ;
      SetVBFPreselectionTreeBranches (vars, chain) ;

      // LOOP OVER THE EVENTS
      for (int entry = 0 ; entry < chain->GetEntries () ; ++entry)
        {
          chain->GetEntry (entry) ;
          double weight = vars.crossSection / vars.totEvents ;
           
          //*******************
          // SET SOME VARIABLES
          vars.lep = * (vars.p_lep) ;
  
          double isoVar = (vars.lep_tkIso + vars.lep_emIso + vars.lep_hadIso - vars.rhoForIsolation * 3.1415 * 0.3 * 0.3) / vars.lep.Pt () ;
          if ((vars.lep_flavour == 11 && isoVar > 0.1) ||
              (vars.lep_flavour == 13 && (isoVar > 0.05 || isoVar * vars.lep.Pt () > 1))) continue ;
          
          vars.met = * (vars.p_met) ;
          if (vars.met.Pt () < 30) continue ;

          vars.nu = * (vars.p_nu) ;
          vars.lepNu = * (vars.p_lep) + * (vars.p_nu) ;
          vars.leadingJ = * (vars.p_leadingJ) ;
          vars.WJ1 = * (vars.p_WJ1) ;
          vars.WJ2 = * (vars.p_WJ2) ;
          vars.WJJ = * (vars.p_WJ1) + * (vars.p_WJ2) ;
          vars.tagJ1 = * (vars.p_tagJ1) ;
          vars.tagJ2 = * (vars.p_tagJ2) ;

          vars.WJJ_m = vars.WJJ.M () ;
          ROOT::Math::XYZTVector Higgs = vars.WJJ + vars.lepNu ;
          vars.mH = Higgs.M () ;

          if (vars.WJJ_m < 40.) continue ;
          else if (vars.WJJ_m > 40. && vars.WJJ_m < 65.)
            {
              h_m4_lower->Fill (vars.mH, weight) ;                
            }
          else if (vars.WJJ_m > 65. && vars.WJJ_m < 95.)
            {
              h_m4_signal->Fill (vars.mH, weight) ;                
            }
          else if (vars.WJJ_m > 95. && vars.WJJ_m < 135.)
            {
              h_m4_upper->Fill (vars.mH, weight) ;                
            }
          else continue ;  
        } // LOOP OVER THE EVENTS
      ++index ;   
    } //PG loop over samples

  // define out file names
  std::string outputRootFullFileName = "testBkg_003.root" ;
  TFile* outputRootFile = new TFile (outputRootFullFileName.c_str (), "RECREATE") ;
  outputRootFile->cd () ;

  m4_upper.save (outputRootFile) ;
  m4_signal.save (outputRootFile) ;
  m4_lower.save (outputRootFile) ;

  outputRootFile->Close () ;
  delete outputRootFile ;
  
  
  
  return 0 ;
}

/*

#include  <./test/plotUtils.C>



*/

