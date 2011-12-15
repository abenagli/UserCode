#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"

#include <iomanip>
#include <string>
#include <algorithm>
#include <functional>

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "Math/Vector4D.h"
#include "Math/Vector3D.h"
#include "Math/Point3D.h"

using namespace std ;


double l_deltaPhi (double phi1, double phi2)
{ 
  double deltaphi = fabs(phi1 - phi2);
  if (deltaphi > 6.283185308) deltaphi -= 6.283185308;
  if (deltaphi > 3.141592654) deltaphi = 6.283185308 - deltaphi;
  return deltaphi;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


double
l_deltaR (double eta1, double phi1, double eta2, double phi2)
{
  double deltaEta = (eta1 - eta2) * (eta1 - eta2) ;
  double deltaPhi = l_deltaPhi (phi1, phi2) ;
  deltaPhi *= deltaPhi ;
  return sqrt (deltaEta + deltaPhi) ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


int 
countPartAboveThreshold (const std::vector<ROOT::Math::XYZTVector> * part, double ptMin, double etaMax = 5.)
{
  int counter = 0 ;
  for (unsigned int iPart = 0 ; iPart < part->size () ; ++iPart)
    {
      if (part->at (iPart).eta () > etaMax) continue ;
      if (part->at (iPart).pt () < ptMin) continue ;
      ++ counter ;
    } //PG loop over part  

  return counter ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


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

  TH1F TP_Zshape ("TP_Zshape", "TP_Zshape", 100, 0, 200) ;
  TH1F TT_Zshape ("TT_Zshape", "TT_Zshape", 100, 0, 200) ;
  TH1F EB_probe_pt ("EB_probe_pt", "EB_probe_pt", 20, 0, 200) ;
  TH1F EB_pass_pt  ("EB_pass_pt",  "EB_pass_pt",  20, 0, 200) ;

  int nomatch = 0 ; //PG a second electron is found, but it does not match the probe
  int match = 0 ;   //PG a second electron is found, and it does match the probe

  // LOOP OVER THE EVENTS
  for (int entry = 0 ; entry < chain->GetEntries () ; ++entry)
    {
      reader.GetEntry (entry) ;
      if ((entry % 1000) == 0) cout << "event " << entry << endl ;   

      //PG get the relevant variables from the ntuple
      //PG ------------------------------------------
      
      std::vector<ROOT::Math::XYZPoint>   * SCPosition = reader.Get3PV   ("SCPosition") ;
      std::vector<float>                  * SCEnergy   = reader.GetFloat ("SCEnergy") ;
      std::vector<int>                    * isEB       = reader.GetInt   ("isEB") ;

      std::vector<ROOT::Math::XYZTVector> * electrons  = reader.Get4V    ("electrons") ;
      std::vector<ROOT::Math::XYZTVector> * muons      = reader.Get4V    ("muons") ;

      //PG apply some trivial selections
      //PG -----------------------------

      if (countPartAboveThreshold (muons, 10) > 1) continue ;          //PG no muons in the event
      int nInterestingEle = countPartAboveThreshold (electrons, 20) ;
      if (nInterestingEle < 1) continue ;                              //PG at least one selected electron
      if (nInterestingEle > 2) continue ;                              //PG at most two selected electrons

      //PG choose the tag electron
      //PG -----------------------

      if (electrons->size () > 1) 
        {
          if (electrons->at (0).pt () < electrons->at (1).pt ()) swap (electrons->at (0), electrons->at (1)) ;
        }
//      cout << "electron momentum: " << sqrt (electrons->at (0).Vect ().Mag2 ()) << endl ;
//      cout << "electron energy: " << electrons->at (0).E () << endl ;


      //PG find the probe supercluster
      //PG ---------------------------

      int probeIndex = 0 ;
      int deltaM = 1000. ;
      ROOT::Math::XYZTVector SC4Vector (0., 0., 0., 0.) ;
      //PG loop over superclusters
      for (unsigned int iSC = 0 ; iSC < isEB->size () ; ++iSC)
        {
          if (isEB->at (iSC) == 0) continue ; //PG only in EB
          double scaleFactor = SCEnergy->at (iSC) / SCPosition->at (iSC).R () ;
          ROOT::Math::XYZTVector d_SC4Vector (scaleFactor * SCPosition->at (iSC).x (),
                                              scaleFactor * SCPosition->at (iSC).y (),
                                              scaleFactor * SCPosition->at (iSC).z (),
                                              SCEnergy->at (iSC)) ;

          if (d_SC4Vector.Et () < 20) continue ; //PG minimum pt

          if (l_deltaR (electrons->at (0).eta (), electrons->at (0).phi (), 
                        d_SC4Vector.eta (), d_SC4Vector.phi ()) < 0.15) continue ; //PG not the SC of the tag

          ROOT::Math::XYZTVector Z_cand = d_SC4Vector + electrons->at (0) ;                       
          if (fabs (Z_cand.M () - 91.186) < deltaM)
            {
              swap (d_SC4Vector, SC4Vector) ;
              probeIndex = iSC ;
              deltaM = fabs (Z_cand.M () - 91.186) ;
            }
        } //PG loop over superclusters

      if (SC4Vector.R () == 0) continue ; //PG no probes found

      ROOT::Math::XYZTVector Z_cand = SC4Vector + electrons->at (0) ;
      TP_Zshape.Fill (Z_cand.M ()) ;
      EB_probe_pt.Fill (SC4Vector.pt ()) ;

      //PG search for the tag, once the probe is found
      //PG -------------------------------------------

      if (electrons->size () > 1) //PG probe found 
        {
          //PG should I check is not closer to the tag
          //PG than the minimum I put between the tag and the probe?
          if (electrons->at (1).eta () > 1.5) continue ; //PG only in EB
          if (electrons->at (1).pt () < 20) continue ;   //PG min pT on electrons

          ROOT::Math::XYZTVector Z_hyp = electrons->at (1) + electrons->at (0) ;
          TT_Zshape.Fill (Z_hyp.M ()) ;

          //PG check that the SC matches the electron
          if (l_deltaR (electrons->at (1).eta (), electrons->at (1).phi (), 
                        SCPosition->at (probeIndex).eta (), SCPosition->at (probeIndex).phi ()) > 0.1)
            { 
              // cerr << "the second lepton does not match the probe" 
              //           << " " << electrons->at (1).eta () << " " << electrons->at (1).phi () << " - " << 
              //          SCPosition->at (probeIndex).eta () << " " << SCPosition->at (probeIndex).phi ()
              //      << endl ;
              ++nomatch ;
              continue ;
            }
          ++match ;
          EB_pass_pt.Fill (electrons->at (1).pt ()) ;
        } //PG probe found

    } // LOOP OVER THE EVENTS

  cout << "nomatch " << nomatch << endl ;
  cout << "match " << match << endl ;

  TFile outFile ("MiBi_TP_001.root", "recreate") ;
  outFile.cd () ;
  TP_Zshape.Write () ;
  TT_Zshape.Write () ;
  EB_probe_pt.Write () ;
  EB_pass_pt.Write () ;
  outFile.Close () ;
  
  return 0 ;
}  
