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



struct btag_sort: public std::binary_function<pair<double, ROOT::Math::XYZTVector>, pair<double, ROOT::Math::XYZTVector>, bool>
{
  bool operator() (pair<double, ROOT::Math::XYZTVector> x, pair<double, ROOT::Math::XYZTVector> y)
    {
      return x.first < y.first ;
    }
} ;



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


struct pt_sort: public std::binary_function<pair<double, ROOT::Math::XYZTVector>, pair<double, ROOT::Math::XYZTVector>, bool>
{
  bool operator() (pair<double, ROOT::Math::XYZTVector> x, pair<double, ROOT::Math::XYZTVector> y)
    {
      return x.second.pt () < y.second.pt () ;
    }
} ;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


struct aeta_sort: public std::binary_function<pair<double, ROOT::Math::XYZTVector>, pair<double, ROOT::Math::XYZTVector>, bool>
{
  bool operator() (pair<double, ROOT::Math::XYZTVector> x, pair<double, ROOT::Math::XYZTVector> y)
    {
      return fabs (x.second.eta ()) < fabs (y.second.eta ()) ;
    }
} ;


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

  vector<TH1F *> v_M4_sideDo ;
  vector<TH1F *> v_M4_signal ;
  vector<TH1F *> v_M4_sideUp ;

  //PG questo serve?
  TChain * chain = collections.begin ()->second ;
  VBFAnalysisVariables vars ;
  SetVBFPreselectionTreeBranches (vars, chain) ;

  //PG loop over samples
  int index = 0 ;
  for (map<string, TChain *>::iterator iColl = collections.begin () ;
       iColl != collections.end () ; 
       ++iColl)
    {

      TH1F * M4_sideDo = new TH1F ((iColl->first + ("_M4_sideDo")).c_str (), iColl->first.c_str (), 50, 100, 600) ;
      v_M4_sideDo.push_back (M4_sideDo) ;
      M4_sideDo->SetStats (0) ;
      M4_sideDo->SetFillColor (colors[index]) ;

      TH1F * M4_signal = new TH1F ((iColl->first + ("_M4_signal")).c_str (), iColl->first.c_str (), 50, 100, 600) ;
      v_M4_signal.push_back (M4_signal) ;
      M4_signal->SetStats (0) ;
      M4_signal->SetFillColor (colors[index]) ;

      TH1F * M4_sideUp = new TH1F ((iColl->first + ("_M4_sideUp")).c_str (), iColl->first.c_str (), 50, 100, 600) ;
      v_M4_sideUp.push_back (M4_sideUp) ;
      M4_sideUp->SetStats (0) ;
      M4_sideUp->SetFillColor (colors[index]) ;

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

          ROOT::Math::XYZTVector Higgs = vars.WJJ + vars.lepNu ;
          vars.WJJ_m = vars.WJJ.M () ;
          if (vars.WJJ_m < 50.) continue ;
          else if (vars.WJJ_m < 65.)  
            {  
              // riempi sideDo
              M4_sideDo->Fill (Higgs.M (), weight) ;
            }
          else if (vars.WJJ_m < 95.)  
            {
              // riempi signal
              M4_signal->Fill (Higgs.M (), weight) ;
            } 
          else if (vars.WJJ_m < 130.) 
            {
              // riempi sideUp
              M4_sideUp->Fill (Higgs.M (), weight) ;

            }
          else continue ;

//           if (vars.lep_flavour == 11) // ele
//           if (vars.lep_flavour == 13) // mu
        } // LOOP OVER THE EVENTS
      ++index ;   
    } //PG loop over samples

  // define out file names
  std::string outputRootFullFileName = "testBkg_001.root" ;
  TFile* outputRootFile = new TFile (outputRootFullFileName.c_str (), "RECREATE") ;
  outputRootFile->cd () ;
    
  // save histograms
  THStack stack_M4_sideDo ("stack_M4_sideDo ","stack") ;
  for (unsigned int i = 0 ; i < v_M4_sideDo.size () ; ++i) 
    {
      v_M4_sideDo.at (i)->Write () ;
      stack_M4_sideDo.Add (v_M4_sideDo.at (i)) ;
    }  
  stack_M4_sideDo.Write () ;

  THStack stack_M4_signal ("stack_M4_signal ","stack") ;
  for (unsigned int i = 0 ; i < v_M4_signal.size () ; ++i) 
    {
      v_M4_signal.at (i)->Write () ;
      stack_M4_signal.Add (v_M4_signal.at (i)) ;
    }  
  stack_M4_signal.Write () ;

  THStack stack_M4_sideUp ("stack_M4_sideUp ","stack") ;
  for (unsigned int i = 0 ; i < v_M4_sideUp.size () ; ++i) 
    {
      v_M4_sideUp.at (i)->Write () ;
      stack_M4_sideUp.Add (v_M4_sideUp.at (i)) ;
    }  
  stack_M4_sideUp.Write () ;

  outputRootFile->Close () ;
  delete outputRootFile ;
  
  
  
  return 0 ;
}

/*

#include  <./test/plotUtils.C>

THStack * stack_M4_sideDo = (THStack *) _file0->Get ("stack_M4_sideDo") ;
TH1F * totShape_sideDo = (TH1F *) stack_M4_sideDo->GetStack ()->Last () ;
THStack * stack_M4_signal = (THStack *) _file0->Get ("stack_M4_signal") ;
TH1F * totShape_signal = (TH1F *) stack_M4_signal->GetStack ()->Last () ;
THStack * stack_M4_sideUp = (THStack *) _file0->Get ("stack_M4_sideUp") ;
TH1F * totShape_sideUp = (TH1F *) stack_M4_sideUp->GetStack ()->Last () ;

totShape_sideDo->SetLineWidth (2) ;
totShape_signal->SetLineWidth (2) ;
totShape_sideUp->SetLineWidth (2) ;

totShape_sideDo->SetFillColor (0) ;
totShape_signal->SetFillColor (0) ;
totShape_sideUp->SetFillColor (0) ;

totShape_sideDo->SetTitle (0) ;
totShape_signal->SetTitle (0) ;
totShape_sideUp->SetTitle (0) ;

totShape_sideDo->SetLineColor (kBlue+2) ;
totShape_signal->SetLineColor (kBlack) ;
totShape_sideUp->SetLineColor (kRed) ;

TCanvas c1 ;

totShape_signal->DrawNormalized ("hist") ;
totShape_sideDo->DrawNormalized ("histsame") ;
totShape_sideUp->DrawNormalized ("histsame") ;

TLegend leg_shapes (0.5, 0.6, 0.9, 0.9, NULL, "brNDC") ;
leg_shapes.SetBorderSize (0) ;
leg_shapes.SetTextFont (42) ;
leg_shapes.SetTextSize (0.04) ;
leg_shapes.SetLineColor (1) ;
leg_shapes.SetLineStyle (1) ;
leg_shapes.SetLineWidth (1) ;
leg_shapes.SetFillColor (0) ;
leg_shapes.SetFillStyle (0) ;
leg_shapes.AddEntry (totShape_signal,"signal region","lp") ;
leg_shapes.AddEntry (totShape_sideDo,"low mjj","lp") ;
leg_shapes.AddEntry (totShape_sideUp,"high mjj","lp") ;
leg_shapes.Draw () ;

TCanvas c2 ;
c2.Divide (1, 3) ;

c2.cd (1) ;
stack_M4_sideDo->Draw ("hist") ;
TLegend * leg_M4_sideDo = makeLegendTitle (*stack_M4_sideDo)
leg_M4_sideDo->Draw () ;
totShape_sideDo->Draw ("same") ;

c2.cd (2) ;
stack_M4_signal->Draw ("hist") ;
TLegend * leg_M4_signal = makeLegendTitle (*stack_M4_signal)
leg_M4_signal->Draw () ;
totShape_signal->Draw ("same") ;

c2.cd (3) ;
stack_M4_sideUp->Draw ("hist") ;
TLegend * leg_M4_sideUp = makeLegendTitle (*stack_M4_sideUp)
leg_M4_sideUp->Draw () ;
totShape_sideUp->Draw ("same") ;




*/

