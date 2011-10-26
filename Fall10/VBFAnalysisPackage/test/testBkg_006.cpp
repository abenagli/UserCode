/*
testBkg_004.exe cfg/2011-07-09-listaFile.txt

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

  float LUMI = 1000. ; //PG to have output in 1/fb

  string inputFileList (argv[1]) ;
  cout << "samples " << inputFileList << endl ;

  map<string, TChain *> collections ;
  std::string treeName = "ntu_18" ;
  ReadFile (collections, inputFileList, treeName) ;

  double btag_cut = 5. ;
  
  TH1::SetDefaultSumw2 (kTRUE) ;
  int colors[11] = {kBlue+2, kRed, 10, kCyan+2, kOrange+7, kGray, kMagenta+1, kGreen+2, kOrange, kViolet+2, kRed+3} ;

  hColl m4_coll ("m4", 50, 100., 600.) ;

  //PG DEFINE THE SELECTIONS
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
  TCut generalCut = "" ;
  generalCut = generalCut && "1 == 1" ;
  
  TCut baseCutEle = "lep_flavour == 11" ;
  baseCutEle = baseCutEle && "lep.Pt() > 30" ;
  baseCutEle = baseCutEle && "TMath::Abs(lep_eta) < 2.5" ;
  baseCutEle = baseCutEle && "met.Et() > 25" ;
  baseCutEle = baseCutEle && "nJets_et30==2" ;

  TCut baseCutMu = "lep_flavour == 13" ;
  baseCutEle = baseCutEle && "lep.Pt() > 25" ;
  baseCutEle = baseCutEle && "TMath::Abs(lep_eta) < 2.1" ;
  baseCutEle = baseCutEle && "met.Et() > 25" ;
  baseCutEle = baseCutEle && "nJets_et30==2" ;

  generalCut = generalCut && (baseCutEle || baseCutMu) ;

  TCut bVeto = "jets_bTag1<3 && jets_bTag2 < 3" ; //FIXME fix the discriminant
  generalCut = generalCut && bVeto ;
                           
  TCut analysisCut ;
  analysisCut = analysisCut && "lepMet_mt > 40" ;                             
  analysisCut = analysisCut && "lepMet_Dphi > 0. && lepMet_Dphi < 3.15" ;                             
  analysisCut = analysisCut && "WJJ_m > 65 && WJJ_m < 105" ;                             
  analysisCut = analysisCut && "WJJ_Deta < 2" ;                             
  analysisCut = analysisCut && "WJJ_Dphi > 0. && WJJ_Dphi < 3.15" ;                             
  analysisCut = analysisCut && "lepWJJ_pt1 > 30" ;                             
  analysisCut = analysisCut && "lepWJJ_pt1 > 20" ;                             
  analysisCut = analysisCut && "lepWJJ_pt1 > 20" ;                             
  analysisCut = analysisCut && "lepNuW_m > 200 && lepNuW_m < 300" ;                             
  
  //FIXME c'e' il kine fit applicato?
  
  //PG loop over samples
  int index = 0 ;
  for (map<string, TChain *>::iterator iColl = collections.begin () ;
       iColl != collections.end () ; 
       ++iColl)
    {

      TH1F * h_m4 = m4_coll.addSample (iColl->first.c_str ()) ;

      TChain * chain = iColl->second ;
      VBFAnalysisVariables vars ;
      SetVBFPreselectionTreeBranches (vars, chain) ;

      // rivedi come era fatto con amax
      TCut cut = generalCut ;
      TCut cutExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PURescaleFactor(PUit_n)", cut.GetTitle (), LUMI) ;    

      iColl->second->Draw (TString ("lepNuW_m >> ") + h_m4->GetName (), cutExtended) ;
      
      cout << iColl->first.c_str () 
           << "\t" << h_m4->GetEntries () 
           << "\t" << h_m4->Integral ()
           << "\n" ;
           
      ++index ;   
    } //PG loop over samples

  // define out file names
  std::string outputRootFullFileName = "testBkg_006.root" ;
  TFile* outputRootFile = new TFile (outputRootFullFileName.c_str (), "RECREATE") ;
  outputRootFile->cd () ;

  m4_coll.save (outputRootFile) ;

  outputRootFile->Close () ;
  delete outputRootFile ;
  
  return 0 ;
}

/*

#include  <./test/plotUtils.C>



*/

