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

  map<string, TChain *> collections ;
  string treeName = "ntu_14" ;
  ReadFile (collections, inputFileList, treeName) ;
  
  TH1::SetDefaultSumw2 (kTRUE) ;
  int colors[11] = {kBlue+2, kRed, 10, kCyan+2, kOrange+7, kGray, kMagenta+1, kGreen+2, kOrange, kViolet+2, kRed+3} ;

  int nBins = 200 ;
  double m4_min = 0. ;
  double m4_max = 1000. ;

  //PG the cuts
  TCut generalCut = "" ;
  generalCut = generalCut && "WJJ_m > 65  && WJJ_m < 95" ;
//   TCut generalCut = "helicityLikelihood > 0.5" ;
//  generalCut = generalCut && "WJJ_pt > 40" ; //PG pt cut on hadronic W
//  generalCut = generalCut && "lepMetW_mt > 40" ; //PG mt cut on leptonic W
//   generalCut = generalCut && "lep_flavour == 13" ; //PG only muons
//   generalCut = generalCut && "lep_flavour == 11" ; //PG only electrons
  
  TString m4_VAR = "lepNuW_m_KF" ;

  map<string, vector<TH1F *> > shapes ;

  //PG loop over signal samples
  for (map<string, TChain *>::iterator iColl = collections.begin () ;
       iColl != collections.end () ; 
       ++iColl)
    {
      vector<TH1F *> l_shapes ;
      shapes[iColl->first] = l_shapes ;
      //PG loop over weights
      for (int i = 0 ; i < 53 ; ++i)
        { 
          stringstream name ;
          name << iColl->first << "_" << i ;
          TH1F * dummy = new TH1F (name.str ().c_str (), name.str ().c_str (), nBins, m4_min, m4_max) ;
          stringstream form ; 
          form << "(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight * PDF_weights[" << i << "]" ;
          TCut cut = Form (form.str ().c_str (), generalCut.GetTitle (), LUMI) ;    

          iColl->second->Draw (m4_VAR + TString (" >> ") + dummy->GetName (), cut) ;
          
          
          shapes[iColl->first].push_back (dummy) ;
        } //PG loop over weights
    } //PG loop over signal samples

  //PG saving the raw histograms
  //PG -------------------------

  TFile * outputRootFile = new TFile ("testPDF_001_raw.root", "RECREATE") ;
  outputRootFile->cd () ;
  for (map<string, vector<TH1F *> >::iterator iShapes = shapes.begin () ; 
       iShapes != shapes.end () ;
       ++iShapes)
    {
      for (int iHisto = 0 ; iHisto < iShapes->second.size () ; ++iHisto)
        iShapes->second.at (iHisto)->Write () ;
    }   
  outputRootFile->Close () ;

  //PG calculating the shapes
  //PG ----------------------

  map<string, TH1F *> PDFUp_shapes ;
  map<string, TH1F *> PDFDown_shapes ;

  //PG loop over samples
  for (map<string, vector<TH1F *> >::iterator iShapes = shapes.begin () ; 
       iShapes != shapes.end () ;
       ++iShapes)
    {
      cout << "------> calculating " << iShapes->first << endl ;
      int nEigen = iShapes->second.size () / 2 ;

      stringstream nameUp ;
      nameUp << iShapes->first << "_PDFUp" ;
      TH1F * dummyUp = new TH1F (nameUp.str ().c_str (), nameUp.str ().c_str (), nBins, m4_min, m4_max) ;
      PDFUp_shapes[iShapes->first] = dummyUp ;
      stringstream nameDown ;
      nameDown << iShapes->first << "_PDFDown" ;
      TH1F * dummyDown = new TH1F (nameDown.str ().c_str (), nameDown.str ().c_str (), nBins, m4_min, m4_max) ;
      PDFDown_shapes[iShapes->first] = dummyDown ;

      //PG loop over eigenvectors
      for (int iEigen = 0 ; iEigen < nEigen ; ++iEigen)
        {
          cout << "-----------> eigenvalue: " << iEigen << endl ;
          //PG loop over histo bins
          for (int iBin = 1 ; iBin <= iShapes->second.at (0)->GetNbinsX () ; ++iBin)
            {
              float val_0 = iShapes->second.at (0)->GetBinContent (iBin) ;
              float val_p = iShapes->second.at (2*iEigen + 1)->GetBinContent (iBin) ;
              float val_m = iShapes->second.at (2*iEigen + 2)->GetBinContent (iBin) ;
//              cout << val_m  << " " << val_0 << " " << val_p << endl ; 
              double delta_p = max (val_p - val_0, val_m - val_0) ; 
//              cout << delta_p << " " << (val_p - val_0)  << " " <<  (val_m - val_0) << endl ; 
              delta_p = max (delta_p, 0.) ;
              dummyUp->SetBinContent (iBin, dummyUp->GetBinContent (iBin) + delta_p * delta_p) ;
              double delta_m = max (val_0 - val_p, val_0 - val_m) ; 
              delta_m = max (delta_m, 0.) ;
              dummyDown->SetBinContent (iBin, dummyDown->GetBinContent (iBin) + delta_m * delta_m) ;
            } //PG loop over histo bins
        } //PG loop over eigenvectors

      for (int iBin = 1 ; iBin <= iShapes->second.at (0)->GetNbinsX () ; ++iBin)
        {
          dummyUp->SetBinContent (iBin, iShapes->second.at (0)->GetBinContent (iBin) + sqrt (dummyUp->GetBinContent (iBin))) ;
          dummyDown->SetBinContent (iBin, iShapes->second.at (0)->GetBinContent (iBin) - sqrt (dummyDown->GetBinContent (iBin))) ;
        }
    } //PG loop over samples

  //PG save up, down central shapes for inspection
  //PG -------------------------------------------
  
  outputRootFile = new TFile ("testPDF_001_insp.root", "RECREATE") ;
  outputRootFile->cd () ;
  
  for (map<string,TH1F * >::iterator iShapes = PDFUp_shapes.begin () ; 
       iShapes != PDFUp_shapes.end () ;
       ++iShapes) iShapes->second->Write () ;

  for (map<string,TH1F * >::iterator iShapes = PDFDown_shapes.begin () ; 
       iShapes != PDFDown_shapes.end () ;
       ++iShapes) iShapes->second->Write () ;

  for (map<string, vector<TH1F *> >::iterator iShapes = shapes.begin () ; 
       iShapes != shapes.end () ;
       ++iShapes) iShapes->second.at (0)->Write () ;

  outputRootFile->Close () ;
  
  //PG save in the format for the analysis
  //PG -----------------------------------
  
  outputRootFile = new TFile ("countSignalEvents_PDFUp_PFlow.root", "RECREATE") ;
  outputRootFile->cd () ;
  
  for (map<string,TH1F * >::iterator iShapes = PDFUp_shapes.begin () ; 
       iShapes != PDFUp_shapes.end () ;
       ++iShapes) iShapes->second->Write (iShapes->first.c_str ()) ;

  outputRootFile->Close () ;

  outputRootFile = new TFile ("countSignalEvents_PDFDown_PFlow.root", "RECREATE") ;
  outputRootFile->cd () ;
  
  for (map<string,TH1F * >::iterator iShapes = PDFDown_shapes.begin () ; 
       iShapes != PDFDown_shapes.end () ;
       ++iShapes) iShapes->second->Write (iShapes->first.c_str ()) ;

  outputRootFile->Close () ;

  return 0 ;
}

