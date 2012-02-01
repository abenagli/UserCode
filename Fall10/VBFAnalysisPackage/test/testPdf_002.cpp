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

  map<string, TChain *> collections ;
  string treeName = "ntu" ;
  ReadFile (collections, inputFileList, treeName) ;
  
  TH1::SetDefaultSumw2 (kTRUE) ;
  int colors[11] = {kBlue+2, kRed, 10, kCyan+2, kOrange+7, kGray, kMagenta+1, kGreen+2, kOrange, kViolet+2, kRed+3} ;

  int nBins = 200 ;
  double m4_min = 0. ;
  double m4_max = 1000. ;

  //PG the cuts
  TCut generalCut = "1==1" ;
//  generalCut = generalCut && "WJJ_m > 65  && WJJ_m < 95" ;
//  generalCut = generalCut && "WJJ_Deta < 1.5" ;
//  generalCut = generalCut && "abs(lep_eta) < 1.5" ;

//   TCut generalCut = "helicityLikelihood > 0.5" ;
//  generalCut = generalCut && "WJJ_pt > 40" ; //PG pt cut on hadronic W
//  generalCut = generalCut && "lepMetW_mt > 40" ; //PG mt cut on leptonic W
//   generalCut = generalCut && "lep_flavour == 13" ; //PG only muons
//   generalCut = generalCut && "lep_flavour == 11" ; //PG only electrons
  
  TString m4_VAR = "lepNuW_m_KF" ;

  map<string, vector<TH1F *> > CT10_shapes ;
  map<string, vector<TH1F *> > NNPDF_shapes ;
  map<string, vector<TH1F *> > MSTW_shapes ;

  //PG loop over signal samples
  for (map<string, TChain *>::iterator iColl = collections.begin () ;
       iColl != collections.end () ; 
       ++iColl)
    {
      cout << "reading " << iColl->first << endl ;
      vector<TH1F *> l_shapes ;
      CT10_shapes[iColl->first] = l_shapes ;
      NNPDF_shapes[iColl->first] = l_shapes ;
      MSTW_shapes[iColl->first] = l_shapes ;

      cout << "   beginning MSTW" << endl ;
      //PG loop over MSTW weights
      for (int i = 0 ; i < 41 ; ++i)
        {
          stringstream name ;
          name << iColl->first << "_MSTW_" << i ;
          TH1F * dummy = new TH1F (name.str ().c_str (), name.str ().c_str (), nBins, m4_min, m4_max) ;
          stringstream form ;
          form << "(%s) * 1./totEvents * crossSection * %f * eventWeight * PDF_weights_MSTW2008nlo68cl[" << i << "]" ;
          TCut cut = Form (form.str ().c_str (), generalCut.GetTitle (), LUMI) ;

          iColl->second->Draw (m4_VAR + TString (" >> ") + dummy->GetName (), cut) ;

          MSTW_shapes[iColl->first].push_back (dummy) ;
        } //PG loop over MSTW weights

      cout << "   beginning NNPDF" << endl ;
      //PG loop over NNPDF weights
      for (int i = 0 ; i < 101 ; ++i)
        {
          stringstream name ;
          name << iColl->first << "_NNPDF_" << i ;
          TH1F * dummy = new TH1F (name.str ().c_str (), name.str ().c_str (), nBins, m4_min, m4_max) ;
          stringstream form ;
          form << "(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight * PDF_weights_NNPDF20[" << i << "]" ;
          TCut cut = Form (form.str ().c_str (), generalCut.GetTitle (), LUMI) ;

          iColl->second->Draw (m4_VAR + TString (" >> ") + dummy->GetName (), cut) ;

          NNPDF_shapes[iColl->first].push_back (dummy) ;
        } //PG loop over NNPDF weights

      cout << "   beginning CT10" << endl ;
      //PG loop over CT10 weights
      for (int i = 0 ; i < 53 ; ++i)
        { 
          stringstream name ;
          name << iColl->first << "_CT10_" << i ;
          TH1F * dummy = new TH1F (name.str ().c_str (), name.str ().c_str (), nBins, m4_min, m4_max) ;
          stringstream form ; 
          form << "(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight * PDF_weights_CT10[" << i << "]" ;
          TCut cut = Form (form.str ().c_str (), generalCut.GetTitle (), LUMI) ;    

          iColl->second->Draw (m4_VAR + TString (" >> ") + dummy->GetName (), cut) ;
          
          CT10_shapes[iColl->first].push_back (dummy) ;
        } //PG loop over CT10 weights
    } //PG loop over signal samples

  //PG saving the raw histograms
  //PG -------------------------

  TFile * outputRootFile = new TFile ("testPDF_002_raw.root", "RECREATE") ;
  outputRootFile->cd () ;
  saveShapes (CT10_shapes, *outputRootFile) ;
  saveShapes (NNPDF_shapes, *outputRootFile) ;
  saveShapes (MSTW_shapes, *outputRootFile) ;
  outputRootFile->Close () ;

  //PG calculating the CT10 shapes and errors
  //PG --------------------------------------

  map<string, TH1F *> PDFUp_CT10_shapes ;
  map<string, TH1F *> PDFDown_CT10_shapes ;

  map<string, pair<double, double> > CT10_band ;
  map<string, pair<double, double> > CT10_relBand ;

  //PG loop over samples
  for (map<string, vector<TH1F *> >::iterator iShapes = CT10_shapes.begin () ; 
       iShapes != CT10_shapes.end () ;
       ++iShapes)
    {
      cout << "------> calculating CT10 " << iShapes->first << endl ;
      int nEigen = iShapes->second.size () / 2 ;

      stringstream nameUp ;
      nameUp << iShapes->first << "_PDFUp" ;
      TH1F * dummyUp = new TH1F (nameUp.str ().c_str (), nameUp.str ().c_str (), nBins, m4_min, m4_max) ;
      PDFUp_CT10_shapes[iShapes->first] = dummyUp ;
      stringstream nameDown ;
      nameDown << iShapes->first << "_PDFDown" ;
      TH1F * dummyDown = new TH1F (nameDown.str ().c_str (), nameDown.str ().c_str (), nBins, m4_min, m4_max) ;
      PDFDown_CT10_shapes[iShapes->first] = dummyDown ;

      double errUp = 0. ;
      double errDown = 0. ;

      double intCentre = iShapes->second.at (0)->Integral () ;

      //PG loop over eigenvectors
      for (int iEigen = 0 ; iEigen < nEigen ; ++iEigen)
        {
          cout << "-----------> eigenvalue: " << iEigen << endl ;
          double intPlus = iShapes->second.at (2*iEigen + 1)->Integral () ;
          double intMinus = iShapes->second.at (2*iEigen + 2)->Integral () ;
          //PG ctrl chi e' piu' alto
          double unc_p = max (intPlus - intCentre, intMinus - intCentre) ; 
          unc_p = max (unc_p, 0.) ;
          double unc_m = max (intCentre - intPlus, intCentre - intMinus) ; 
          unc_m = max (unc_m, 0.) ;
          errUp += unc_p * unc_p ;
          errDown += unc_m * unc_m ;

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

      CT10_band[iShapes->first] = pair<double, double> (intCentre + sqrt (errUp), intCentre - sqrt (errDown)) ;
      CT10_relBand[iShapes->first] = pair<double, double> (sqrt (errUp) / intCentre, sqrt (errDown) / intCentre) ;
      for (int iBin = 1 ; iBin <= iShapes->second.at (0)->GetNbinsX () ; ++iBin)
        {
          dummyUp->SetBinContent (iBin, iShapes->second.at (0)->GetBinContent (iBin) + sqrt (dummyUp->GetBinContent (iBin))) ;
          dummyDown->SetBinContent (iBin, iShapes->second.at (0)->GetBinContent (iBin) - sqrt (dummyDown->GetBinContent (iBin))) ;
        }
    } //PG loop over samples


  //PG calculating the NNPDF shapes and errors
  //PG ---------------------------------------

  map<string, pair<double, double> > NNPDF_band ;
  map<string, pair<double, double> > NNPDF_relBand ;

  //PG loop over samples
  for (map<string, vector<TH1F *> >::iterator iShapes = NNPDF_shapes.begin () ; 
       iShapes != NNPDF_shapes.end () ;
       ++iShapes)
    {
      cout << "------> calculating NNPDF " << iShapes->first << endl ;
      int nEigen = iShapes->second.size () / 2 ;

      double errUp = 0. ;
      double errDown = 0. ;

      double intCentre = iShapes->second.at (0)->Integral () ;
      //PG loop over eigenvectors
      for (int iEigen = 0 ; iEigen < nEigen ; ++iEigen)
        {
          cout << "-----------> eigenvalue: " << iEigen << endl ;
          double intPlus = iShapes->second.at (2*iEigen + 1)->Integral () ;
          double intMinus = iShapes->second.at (2*iEigen + 2)->Integral () ;
          //PG ctrl chi e' piu' alto
          double unc_p = intPlus - intCentre ; 
          double unc_m = intCentre - intMinus ; 
          errUp += unc_p * unc_p ;
          errDown += unc_m * unc_m ;
        } //PG loop over eigenvectors
      errUp /= (double) (nEigen - 1) ;
      errDown /= (double) (nEigen - 1) ;
      NNPDF_band[iShapes->first] = pair<double, double> (intCentre + sqrt (errUp), intCentre - sqrt (errDown)) ;
      NNPDF_relBand[iShapes->first] = pair<double, double> (sqrt (errUp) / intCentre, sqrt (errDown) / intCentre) ;
    } //PG loop over samples


  //PG calculating the MSTW shapes and errors
  //PG ---------------------------------------

  map<string, pair<double, double> > MSTW_band ;
  map<string, pair<double, double> > MSTW_relBand ;

  //PG loop over samples
  for (map<string, vector<TH1F *> >::iterator iShapes = MSTW_shapes.begin () ; 
       iShapes != MSTW_shapes.end () ;
       ++iShapes)
    {
      cout << "------> calculating MSTW " << iShapes->first << endl ;
      int nEigen = iShapes->second.size () / 2 ;

      double errUp = 0. ;
      double errDown = 0. ;

      double intCentre = iShapes->second.at (0)->Integral () ;
      //PG loop over eigenvectors
      for (int iEigen = 0 ; iEigen < nEigen ; ++iEigen)
        {
          cout << "-----------> eigenvalue: " << iEigen << endl ;
          double intPlus = iShapes->second.at (2*iEigen + 1)->Integral () ;
          double intMinus = iShapes->second.at (2*iEigen + 2)->Integral () ;
          //PG ctrl chi e' piu' alto
          double unc_p = max (intPlus - intCentre, intMinus - intCentre) ; 
          unc_p = max (unc_p, 0.) ;
          double unc_m = max (intCentre - intPlus, intCentre - intMinus) ; 
          unc_m = max (unc_m, 0.) ;
          errUp += unc_p * unc_p ;
          errDown += unc_m * unc_m ;
        } //PG loop over eigenvectors
      MSTW_band[iShapes->first] = pair<double, double> (intCentre + sqrt (errUp), intCentre - sqrt (errDown)) ;
      MSTW_relBand[iShapes->first] = pair<double, double> (sqrt (errUp) / intCentre, sqrt (errDown) / intCentre) ;
    } //PG loop over samples

  //PG printout errors and global envelope
  //PG -------------------------------------------------

  cout << " mini | maxi | rel half width | min CT10 - max CT10 | min NNPDF - max NNPDF | min MSTW = max MSTW \n" ;
  cout << "---------------------------------------------------------------------\n" ;
  //PG loop over samples
  for (map<string, pair<double, double> >::iterator iMap = CT10_band.begin () ; 
       iMap != CT10_band.end () ; ++iMap)
    {
      cout << iMap->first ;
      
      //PG the minimum of the envelope
      double mini = (iMap->second).second ;
      mini = std::min (mini, NNPDF_band[iMap->first].second) ;
      mini = std::min (mini, MSTW_band[iMap->first].second) ;
    
      cout << " | " << mini ;
    
      //PG the maximum of the envelope
      double maxi = (iMap->second).first ;
      maxi = std::max (maxi, NNPDF_band[iMap->first].first) ;
      maxi = std::max (maxi, MSTW_band[iMap->first].first) ;

      cout << " | " << maxi ;

      //PG the relative half width of the envelope
      cout <<  " e : " << (maxi-mini) / (maxi+mini) ;

      //PG the three bands
      cout << " @CT10 " << CT10_band[iMap->first].first << " - " << CT10_band[iMap->first].second ;
      cout << " @NNPDF " << NNPDF_band[iMap->first].first << " - " << NNPDF_band[iMap->first].second ;
      cout << " @MSTW " << MSTW_band[iMap->first].first << " - " << MSTW_band[iMap->first].second ;
          
      cout << endl ;    
    } //PG loop over samples

  //PG printout the relative errors envelope
  //PG -------------------------------------------------

  cout << "\nRELATIVE BAND ONLY\n\n" ;

  cout << " mini | maxi | rel half width \n" ;
  cout << "------------------------------\n" ;

  for (map<string, pair<double, double> >::iterator iMap = CT10_relBand.begin () ; 
       iMap != CT10_relBand.end () ; ++iMap)
    {
      cout << iMap->first ;
      //PG the minimum of the envelope
      double mini = (iMap->second).second ;
      mini = std::min (mini, NNPDF_relBand[iMap->first].second) ;
      mini = std::min (mini, MSTW_relBand[iMap->first].second) ;
    
      cout << " | " << mini ;
    
      //PG the maximum of the envelope
      double maxi = (iMap->second).first ;
      maxi = std::max (maxi, NNPDF_relBand[iMap->first].first) ;
      maxi = std::max (maxi, MSTW_relBand[iMap->first].first) ;

      cout << " | " << maxi ;

      cout << " | " << (mini + maxi) / (maxi - mini + 2) << endl ;
      /* 
      la semilarghezza dello scarto, relativa:
      [(N + Nf+) - (N - Nf-)] / [(N + Nf+) + (N - Nf-)] = (f+ + f-) / (f+ - f- + 2)
      */
    }  


  //PG renormalize up and down shapes to the central one
  //PG -------------------------------------------------
  
  for (map<string,TH1F * >::iterator iShapes = PDFUp_CT10_shapes.begin () ; 
       iShapes != PDFUp_CT10_shapes.end () ;
       ++iShapes) 
    {   
      iShapes->second->Scale (CT10_shapes[iShapes->first].at (0)->Integral () / iShapes->second->Integral ()) ;
    }
  for (map<string,TH1F * >::iterator iShapes = PDFDown_CT10_shapes.begin () ; 
       iShapes != PDFDown_CT10_shapes.end () ;
       ++iShapes) 
    {   
      iShapes->second->Scale (CT10_shapes[iShapes->first].at (0)->Integral () / iShapes->second->Integral ()) ;
    }

  //PG save up, down central shapes for inspection
  //PG -------------------------------------------
  
  outputRootFile = new TFile ("testPDF_002_insp.root", "RECREATE") ;
  outputRootFile->cd () ;
  
  for (map<string,TH1F * >::iterator iShapes = PDFUp_CT10_shapes.begin () ; 
       iShapes != PDFUp_CT10_shapes.end () ;
       ++iShapes) iShapes->second->Write () ;

  for (map<string,TH1F * >::iterator iShapes = PDFDown_CT10_shapes.begin () ; 
       iShapes != PDFDown_CT10_shapes.end () ;
       ++iShapes) iShapes->second->Write () ;

  for (map<string, vector<TH1F *> >::iterator iShapes = CT10_shapes.begin () ; 
       iShapes != CT10_shapes.end () ;
       ++iShapes) iShapes->second.at (0)->Write () ;

  outputRootFile->Close () ;
  
  //PG save in the format for the analysis
  //PG -----------------------------------
  
  outputRootFile = new TFile ("countSignalEvents_002_PDFUp_PFlow.root", "RECREATE") ;
  outputRootFile->cd () ;
  
  for (map<string,TH1F * >::iterator iShapes = PDFUp_CT10_shapes.begin () ; 
       iShapes != PDFUp_CT10_shapes.end () ;
       ++iShapes) iShapes->second->Write (iShapes->first.c_str ()) ;

  outputRootFile->Close () ;

  outputRootFile = new TFile ("countSignalEvents_002_PDFDown_PFlow.root", "RECREATE") ;
  outputRootFile->cd () ;
  
  for (map<string,TH1F * >::iterator iShapes = PDFDown_CT10_shapes.begin () ; 
       iShapes != PDFDown_CT10_shapes.end () ;
       ++iShapes) iShapes->second->Write (iShapes->first.c_str ()) ;

  outputRootFile->Close () ;

  return 0 ;
}

