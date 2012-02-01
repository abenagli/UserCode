/*
testPdf_001.exe cfg/pdf-listaFile.txt  


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


double getMinNonZero (TH1F * histo)
{
  for (int i = 1 ; i <= histo->GetNbinsX () ; ++i)
    {
      if (histo->GetBinContent (i) > 0)
        return histo->GetBinCenter (i) ;
    }
  return -1. ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


double getMaxNonZero (TH1F * histo)
{
  for (int i = histo->GetNbinsX () ; i > 0  ; --i)
    {
      if (histo->GetBinContent (i) > 0)
        return histo->GetBinCenter (i) ;
    }
  return -1. ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void saveShapes (map<string, vector<TH1F *> > shapes, TFile & f) 
{
  f.cd () ;
  for (map<string, vector<TH1F *> >::iterator iShapes = shapes.begin () ;
       iShapes != shapes.end () ;
       ++iShapes)
    {
      for (unsigned int iHisto = 0 ; iHisto < iShapes->second.size () ; ++iHisto)
        iShapes->second.at (iHisto)->Write () ;
    }
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

//   float LUMI = 2145. ; //PG to have output in 1/fb
  float LUMI = 4236.79 ; //PG to have output in 1/fb
  
  string inputFileList (argv[1]) ;
  cout << "samples " << inputFileList << endl ;

  //PG this is dummy, just to have the map filled coherently with the rest

  map<string, TChain *> collections ;
  string treeName = "ntu_14" ;
  ReadFile (collections, inputFileList, treeName) ;


  TFile f_tot ("testPDF_002_raw.root") ;
  TFile f_sel ("testPDF_001_raw.root") ;

  vector<TH1F *> results ;
 
  //PG loop over signal samples
  for (map<string, TChain *>::iterator iColl = collections.begin () ;
       iColl != collections.end () ; 
       ++iColl)
    {
      TH1F * eff_CT10  = new TH1F ("eff_CT10", "eff_CT10", 10000, 0, 1) ;
      TH1F * eff_MSTW  = new TH1F ("eff_MSTW", "eff_MSTW", 10000, 0, 1) ;
      TH1F * eff_NNPDF = new TH1F  ("eff_NNPDF", "eff_NNPDF", 10000, 0, 1) ;

      //PG loop over MSTW weights
      for (int i = 0 ; i < 41 ; ++i)
        {
          stringstream name ;
          name << iColl->first << "_MSTW_" << i ;
          TH1F * num = (TH1F *) f_sel.Get (name.str ().c_str ()) ;
          stringstream name_sel ;
          name_sel << name.str () << "_sel" ;
          num->SetName (name_sel.str ().c_str ()) ;          
          TH1F * den = (TH1F *) f_tot.Get (name.str ().c_str ()) ;
          double eff = num->Integral () / den->Integral () ;
          eff_MSTW->Fill (eff) ;
        } //PG loop over MSTW weights

      //PG loop over NNPDF weights
      for (int i = 0 ; i < 101 ; ++i)
        {
          stringstream name ;
          name << iColl->first << "_NNPDF_" << i ;
          TH1F * num = (TH1F *) f_sel.Get (name.str ().c_str ()) ;
          stringstream name_sel ;
          name_sel << name.str () << "_sel" ;
          num->SetName (name_sel.str ().c_str ()) ;          
          TH1F * den = (TH1F *) f_tot.Get (name.str ().c_str ()) ;
          double eff = num->Integral () / den->Integral () ;
          eff_NNPDF->Fill (eff) ;
        } //PG loop over NNPDF weights

      //PG loop over CT10 weights
      for (int i = 0 ; i < 53 ; ++i)
        { 
          stringstream name ;
          name << iColl->first << "_CT10_" << i ;
          TH1F * num = (TH1F *) f_sel.Get (name.str ().c_str ()) ;
          stringstream name_sel ;
          name_sel << name.str () << "_sel" ;
          num->SetName (name_sel.str ().c_str ()) ;          
          TH1F * den = (TH1F *) f_tot.Get (name.str ().c_str ()) ;
          double eff = num->Integral () / den->Integral () ;
          eff_CT10->Fill (eff) ;
        } //PG loop over CT10 weights

      //PG cloning

      TH1F * eff_int = (TH1F *) eff_CT10->Clone ("eff_int") ;
      eff_int->Add (eff_NNPDF) ;
      eff_int->Add (eff_MSTW) ;

      stringstream name ;
      name << iColl->first << "_effs" ;
      eff_int->SetName (name.str ().c_str ()) ;
      results.push_back (eff_int) ;

      delete eff_CT10  ;
      delete eff_MSTW  ;
      delete eff_NNPDF ;


    } //PG loop over signal samples


  TFile output ("testPdf_003.root", "recreate") ;
  output.cd () ;
  cout << "sample : minEff - maxEff : halfWidth\n" ;
  for (int i = 0 ; i < results.size () ; ++i)
    {
      cout << results.at (i)->GetName () << " : " 
           << getMinNonZero (results.at (i)) << " - " << getMaxNonZero (results.at (i))
           << " : " << (getMaxNonZero (results.at (i)) - getMinNonZero (results.at (i))) / (getMaxNonZero (results.at (i)) + getMinNonZero (results.at (i))) 
           << endl ;
      results.at (i)->Write () ;
    }
  output.Close () ;

  return 0 ;
}

