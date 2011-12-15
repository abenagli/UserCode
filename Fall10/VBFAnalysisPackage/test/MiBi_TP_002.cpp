#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"

#include <iomanip>
#include <string>
#include <algorithm>
#include <functional>
#include <vector>
#include <map>

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "Math/Vector4D.h"
#include "Math/Vector3D.h"
#include "Math/Point3D.h"

using namespace std ;


double l_deltaPhi (double phi1, double phi2)
{ 
  double deltaphi = fabs (phi1 - phi2) ;
  if (deltaphi > 6.283185308) deltaphi -= 6.283185308 ;
  if (deltaphi > 3.141592654) deltaphi = 6.283185308 - deltaphi ;
  return deltaphi ;
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

  string inputFileList = argv[1] ;
  string treeName = "MiBiCommonNTOneLeptonTwoJetsPFlow/SimpleNtuple" ;
  TChain* chain = new TChain (treeName.c_str ()) ;
  if (!FillChain (*chain, inputFileList.c_str ())) return 1 ;
  treeReader reader ( (TTree*) (chain), false) ;

  TH1F h_DR_1 ("h_DR_1", "h_DR_1", 100, 0., 0.5) ;
  TH1F h_DR_2 ("h_DR_2", "h_DR_2", 100, 0., 0.5) ;
  vector<TH1F *> vh_DR ;
  vh_DR.push_back (&h_DR_1) ;
  vh_DR.push_back (&h_DR_2) ;

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

      map<double, pair<int, int> > matched ;
 
      for (int iEle = 0 ; iEle < electrons->size () ; ++iEle)
        {
          for (unsigned int iSC = 0 ; iSC < SCPosition->size () ; ++iSC)
            {
              double DR = l_deltaR (electrons->at (iEle).eta (), electrons->at (iEle).phi (), 
                                    SCPosition->at (iSC).eta (), SCPosition->at (iSC).phi ()) ;
              matched[DR] = pair<int, int> (iEle, iSC) ;
            }
        }


      vector<int> assignEle ;
      vector<int> assignSC ;

      //PG prune the map
      for (map<double, pair<int, int> >::iterator iMap = matched.begin () ;
           iMap != matched.end () ; ++iMap)
        {
          if (find (assignEle.begin () , assignEle.end () , iMap->second.first) 
                   != assignEle.end ()) continue ;
          if (find (assignSC.begin () , assignSC.end () , iMap->second.second) 
                   != assignSC.end ()) continue ;
          vh_DR.at (iMap->second.first)->Fill (iMap->first) ;
          assignSC.push_back (iMap->second.second) ;
          assignEle.push_back (iMap->second.first) ;
          if (assignEle.size () == electrons->size ()) break ;
        }


    } // LOOP OVER THE EVENTS

  TFile outFile ("MiBi_TP_002.root", "recreate") ;
  outFile.cd () ;
  h_DR_1.Write () ;
  h_DR_2.Write () ;
  outFile.Close () ;
  
  return 0 ;
}  
