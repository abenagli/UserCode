#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"

#include <iomanip>
#include <string>

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"

using namespace std ;

int main(int argc, char** argv)
{
//  //Check if all nedeed arguments to parse are there
//  if(argc != 2)
//  {
//    std::cerr << ">>>>> VBFPreselection::usage: " << argv[0] << " configFileName" << std::endl ;
//    return 1;
//  }
//
//  // Parse the config file
//  parseConfigFile(argv[1]);
//  

  string inputFileList = argv[1] ;
  string treeName = "MiBiCommonNTOneLeptonTwoJetsPFlow/SimpleNtuple" ;
  TChain* chain = new TChain (treeName.c_str ()) ;
  if (!FillChain (*chain, inputFileList.c_str ())) return 1 ;
  treeReader reader ( (TTree*) (chain), false) ;

  TH1F h_EB_SCnum ("h_EB_SCnum", "h_EB_SCnum", 20, 0, 20) ;
  TH1F h_EE_SCnum ("h_EE_SCnum", "h_EE_SCnum", 20, 0, 20) ;
  
  TH1F h_Elenum ("h_Elenum", "h_Elenum", 10, 0, 0) ;
  TH1F h_Elenum_EB ("h_Elenum_EB", "h_Elenum_EB", 10, 0, 0) ;


  TH2F h_Ne_vs_Nu ("h_Ne_vs_Nu", "h_Ne_vs_Nu", 10, 0, 10, 10, 0, 10) ;
  h_Ne_vs_Nu.GetXaxis ()->SetTitle ("muons number") ;
  h_Ne_vs_Nu.GetYaxis ()->SetTitle ("electrons number") ;

  TH2F h_NSC_vs_Ne ("h_NSC_vs_Ne", "h_NSC_vs_Ne", 10, 0, 10, 10, 0, 10) ;
  h_NSC_vs_Ne.GetXaxis ()->SetTitle ("electrons number") ;
  h_NSC_vs_Ne.GetYaxis ()->SetTitle ("superclusters number") ;

  TH2F h_NSC_vs_Ne_EB ("h_NSC_vs_Ne_EB", "h_NSC_vs_Ne_EB", 10, 0, 10, 10, 0, 10) ;
  h_NSC_vs_Ne_EB.GetXaxis ()->SetTitle ("electrons number") ;
  h_NSC_vs_Ne_EB.GetYaxis ()->SetTitle ("superclusters number") ;

  // LOOP OVER THE EVENTS
  for (int entry = 0 ; entry < chain->GetEntries () ; ++entry)
    {
      reader.GetEntry (entry) ;
      if ((entry % 1000) == 0) cout << "event " << entry << endl ;   
      reader.GetInt ("isEB") ;

      std::vector<ROOT::Math::XYZPoint>   * SCPosition = reader.Get3PV   ("SCPosition") ;
      std::vector<float>                  * SCEnergy   = reader.GetFloat ("SCEnergy") ;
      std::vector<int>                    * isEB       = reader.GetInt   ("isEB") ;

      std::vector<ROOT::Math::XYZTVector> * electrons  = reader.Get4V    ("electrons") ;
      std::vector<ROOT::Math::XYZTVector> * muons      = reader.Get4V    ("muons") ;

      h_Ne_vs_Nu.Fill (muons->size (), electrons->size ()) ;
      h_NSC_vs_Ne.Fill (electrons->size (), SCPosition->size ()) ;
      h_Elenum.Fill (electrons->size ()) ;

      int EB_SCnum = 0 ;
      int EE_SCnum = 0 ;
      //PG loop over superclusters
      for (unsigned int iSC = 0 ; iSC < isEB->size () ; ++iSC)
        {
          if (isEB->at (iSC) == 1) ++EB_SCnum ;
          else ++EE_SCnum ;
        } //PG loop over superclusters
        
      int EB_Elenum = 0 ;
      //PG loop over electrons  
      for (unsigned int iEle = 0 ; iEle < electrons->size () ; ++iEle)
        {
          if (electrons->at (iEle).eta () > 1.479) continue ;
          ++ EB_Elenum ;
        } //PG loop over electrons  
      h_NSC_vs_Ne_EB.Fill (EB_Elenum, EB_SCnum) ;
      h_Elenum_EB.Fill (EB_Elenum) ;
        
      h_EB_SCnum.Fill (EB_SCnum) ;
      h_EE_SCnum.Fill (EE_SCnum) ;
    } // LOOP OVER THE EVENTS

  TFile outFile ("MiBi_countSC.root", "recreate") ;
  outFile.cd () ;
  h_EB_SCnum.Write () ;
  h_EE_SCnum.Write () ;
  h_Ne_vs_Nu.Write () ;
  h_NSC_vs_Ne_EB.Write () ;
  h_NSC_vs_Ne.Write () ;
  h_Elenum.Write () ;
  h_Elenum_EB.Write () ;
  outFile.Close () ;
  
  return 0 ;
}  
