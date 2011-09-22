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


#include "TF1.h"
#include "TH1.h"
#include "TProfile.h"
#include "TObject.h"
#include "TRandom3.h"
#include "Math/Vector4D.h"

#include "TMVA/Reader.h"
#include "hColl.h"

using namespace std ;



TH1F * histo_WW ;

double shape_WW (double* x, double* par)
  {
    double xx = par[1] * (x[0] - par[2]);
    
    double xMin = histo_WW -> GetBinCenter(1);
    double xMax = histo_WW -> GetBinCenter(histo_WW -> GetNbinsX());

    if( (xx < xMin) || (xx >= xMax) )  return 1.e-10;
    
    else
    {  
      int bin = histo_WW -> FindBin(xx);
      int bin1 = 0;
      int bin2 = 0;
      
      if(xx >= histo_WW -> GetBinCenter(bin))
      {
        bin1 = bin;
        bin2 = bin+1;
      }
      
      else
      {
        bin1 = bin-1;
        bin2 = bin;
      }
      
      double x1 = histo_WW -> GetBinCenter(bin1);
      double y1 = histo_WW -> GetBinContent(bin1);
      double x2 = histo_WW -> GetBinCenter(bin2);
      double y2 = histo_WW -> GetBinContent(bin2);
      double m = 1. * (y2 - y1) / (x2 - x1);
      
      if( (y1 + m * (xx - x1)) < 1.e-10)
        return 1.e-10;
      
      return par[0] * par[1] * (y1 + m * (xx - x1));
    }
    
    return 1.e-10;  
  }
  

// ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ ------


TH1F * histo_Wj ;

double shape_Wj (double* x, double* par)
  {
    double xx = par[1] * (x[0] - par[2]);
    
    double xMin = histo_Wj -> GetBinCenter(1);
    double xMax = histo_Wj -> GetBinCenter(histo_Wj -> GetNbinsX());

    if( (xx < xMin) || (xx >= xMax) )  return 1.e-10;
    
    else
    {  
      int bin = histo_Wj -> FindBin(xx);
      int bin1 = 0;
      int bin2 = 0;
      
      if(xx >= histo_Wj -> GetBinCenter(bin))
      {
        bin1 = bin;
        bin2 = bin+1;
      }
      
      else
      {
        bin1 = bin-1;
        bin2 = bin;
      }
      
      double x1 = histo_Wj -> GetBinCenter(bin1);
      double y1 = histo_Wj -> GetBinContent(bin1);
      double x2 = histo_Wj -> GetBinCenter(bin2);
      double y2 = histo_Wj -> GetBinContent(bin2);
      double m = 1. * (y2 - y1) / (x2 - x1);
      
      if( (y1 + m * (xx - x1)) < 1.e-10)
        return 1.e-10;
      
      return par[0] * par[1] * (y1 + m * (xx - x1));
    }
    
    return 1.e-10;  
  }
  

// ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ ------


double shape_Wx (double* x, double* par)
{
  return shape_WW (x,par) + shape_Wj (x,&par[3]) ;
}
  

// ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ ------ ------


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
  string treeName = "ntu_15" ;
  ReadFile (collections, inputFileList, treeName) ;
  
  TH1::SetDefaultSumw2 (kTRUE) ;
  int colors[11] = {kBlue+2, kRed, 10, kCyan+2, kOrange+7, kGray, kMagenta+1, kGreen+2, kOrange, kViolet+2, kRed+3} ;

  hColl m2 ("m2", 25, 30., 130.) ;

  TH1F * m2_DATA = new TH1F ("m2_DATA", "m2_DATA", 25, 30., 130.) ;

  //PG the cuts
  TCut generalCut = "" ;
  generalCut = generalCut && "1 == 1" ;
  
  TCut chi2 = "chi2_KF < 15" ;

  //PG loop over samples
  int index = 0 ;
  for (map<string, TChain *>::iterator iColl = collections.begin () ;
       iColl != collections.end () ; 
       ++iColl)
    {

      if (iColl->first.find ("ggH") != string::npos) continue ;
      if (iColl->first.find ("qqH") != string::npos) continue ;
//      if (iColl->first == "ggH") continue ;

//      TCut cutLower = generalCut && lower ;
//      TCut cutLowerExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PURescaleFactor((PUit_n+PUoot_n)/3.)", cutLower.GetTitle (), LUMI) ;    

     TCut generalCutExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PURescaleFactor((PUit_n+PUoot_n)/3.)", generalCut.GetTitle (), LUMI) ;    

     TCut cut = chi2 && generalCut ;
     TCut cutExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PURescaleFactor((PUit_n+PUoot_n)/3.)", cut.GetTitle (), LUMI) ;    
      
     if (iColl->first == "DATA")
        {
          iColl->second->Draw ("WJJ_m >> m2_DATA", cut) ;
          continue ;
        }  
      cout << " reading " << iColl->first << endl ;
      
      TH1F * h_m2 = m2.addSample (iColl->first.c_str ()) ;

      TChain * chain = iColl->second ;
      VBFAnalysisVariables vars ;
      SetVBFPreselectionTreeBranches (vars, chain) ;

      // rivedi come era fatto con amax
      iColl->second->Draw (TString ("WJJ_m >> ") + h_m2->GetName (), cutExtended) ;
      
      ++index ;   
    } //PG loop over samples

  int k = 0 ;
  TH1F * m2_DY   = m2.collection.at (k++) ;
  TH1F * m2_QCD  = m2.collection.at (k++) ;
  TH1F * m2_TT   = m2.collection.at (k++) ;
  TH1F * m2_WW   = m2.collection.at (k++) ;
  TH1F * m2_WZ   = m2.collection.at (k++) ;
  TH1F * m2_Wjet = m2.collection.at (k++) ;
  TH1F * m2_ZZ   = m2.collection.at (k++) ;
  TH1F * m2_top  = m2.collection.at (k++) ;

  THStack stack_bkg = m2.makeStack () ;

  TH1F * m2_bkg = (TH1F *) stack_bkg.GetStack ()->Last () ;

  TH1F * total_bkg = (TH1F *) m2_bkg->Clone ("total_bkg") ;
  total_bkg->Add (m2_DY, -1) ;  
  total_bkg->Add (m2_QCD, -1) ;  
  total_bkg->Add (m2_TT, -1) ;  
  total_bkg->Add (m2_ZZ, -1) ;  
  total_bkg->Add (m2_top, -1) ;  

  TH1F * m2_DATA_toFit = (TH1F *) m2_DATA->Clone ("m2_DATA_toFit") ;  
  //TH1F * m2_DATA_toFit = (TH1F *) m2_bkg->Clone ("m2_DATA_toFit") ;  
  m2_DATA_toFit->Add (m2_DY, -1) ;  
  m2_DATA_toFit->Add (m2_QCD, -1) ;  
  m2_DATA_toFit->Add (m2_TT, -1) ;  
  m2_DATA_toFit->Add (m2_ZZ, -1) ;  
  m2_DATA_toFit->Add (m2_top, -1) ;  
  
  TH1F* m2_VV = (TH1F * ) m2_WW -> Clone("m2_VV") ;
  m2_VV -> Add (m2_WZ, 1.) ;
  histo_WW = m2_VV ;
  histo_Wj = m2_Wjet ;
  
  TF1 fitFunc_WW ("fitFunc_WW", shape_WW, 30., 130. , 3) ;
  fitFunc_WW.SetParameter (0, 1.) ;
  fitFunc_WW.FixParameter (1, 1.) ;
  fitFunc_WW.FixParameter (2, 0.) ;
  fitFunc_WW.SetLineColor(kOrange);
  TCanvas c0_WW ;
  m2_WW -> Draw () ;
  fitFunc_WW.Draw ("same") ;
  c0_WW.Print ("functioning_WW.pdf", "pdf") ;  
  
  TF1 fitFunc_Wjet ("fitFunc_Wjet", shape_Wj, 30., 130. , 3) ;
  fitFunc_Wjet.SetParameter (0, 1.) ;
  fitFunc_Wjet.FixParameter (1, 1.) ;
  fitFunc_Wjet.FixParameter (2, 0.) ;
  fitFunc_Wjet.SetLineColor(kBlue);
  TCanvas c0_Wjet ;
  m2_Wjet -> Draw () ;
  fitFunc_Wjet.Draw ("same") ;
  c0_Wjet.Print ("functioning_Wjet.pdf", "pdf") ;  
  

  
  TF1 fitFunc ("fitFunc", shape_Wx, 30., 130. , 6) ;
  fitFunc.SetParameter (0, 1.) ;
  fitFunc.FixParameter (1, 1.) ;
  fitFunc.FixParameter (2, 0.) ;
  fitFunc.SetParameter (3, 1.) ;
  fitFunc.FixParameter (4, 1.) ;
  fitFunc.FixParameter (5, 0.) ;
  
  TCanvas c0 ;
  total_bkg -> Draw () ;
  fitFunc.Draw ("same") ;
  fitFunc_WW.Draw ("same") ;
  fitFunc_Wjet.Draw ("same") ;
  c0.Print ("functioning.pdf", "pdf") ;
  
  m2_DATA_toFit->Fit ("fitFunc", "+") ;

  TCanvas c1 ;
  m2_DATA_toFit->Draw () ;
  total_bkg->SetFillColor (kOrange) ;
  total_bkg->Draw ("histsame") ;
  m2_DATA_toFit->Draw ("same") ;
  c1.Print ("fitting.pdf", "pdf") ;

  TH1F * fitted_WW = (TH1F *) histo_WW->Clone ("fitted_WW") ; 
  fitted_WW->Scale (fitFunc.GetParameter (0)) ;
  TH1F * fitted_Wj = (TH1F *) histo_Wj->Clone ("fitted_Wj") ;
  fitted_Wj->Scale (fitFunc.GetParameter (3)) ;
  
  TH1F * m2_DATA_toCheck = (TH1F *) m2_DATA->Clone ("m2_DATA_toCheck") ;
  //TH1F * m2_DATA_toCheck = (TH1F *) m2_bkg->Clone ("m2_DATA_toCheck") ;
  m2_DATA_toCheck->Add (m2_DY, -1) ;  
  m2_DATA_toCheck->Add (m2_QCD, -1) ;  
  m2_DATA_toCheck->Add (m2_TT, -1) ;  
  m2_DATA_toCheck->Add (m2_ZZ, -1) ;  
  m2_DATA_toCheck->Add (m2_top, -1) ;  
  m2_DATA_toCheck->Add (fitted_Wj, -1) ; 
  
  m2_DATA_toCheck->Draw () ;
  m2_WW->Draw ("samehist") ;
  m2_DATA_toCheck->Draw ("same") ;
  c1.Print ("testing.pdf", "pdf") ;
  
  stack_bkg.Draw ("hist") ;
  c1.Print ("stacking.pdf", "pdf") ;
  
  
  // define out file names
  std::string outputRootFullFileName = "testWW_001.root" ;
  TFile* outputRootFile = new TFile (outputRootFullFileName.c_str (), "RECREATE") ;
  outputRootFile->cd () ;

  m2.save (outputRootFile) ;
  m2_DATA->Write () ;   

  outputRootFile->Close () ;
  delete outputRootFile ;
  
  
  
  return 0 ;
}

/*

#include  <./test/plotUtils.C>



*/

