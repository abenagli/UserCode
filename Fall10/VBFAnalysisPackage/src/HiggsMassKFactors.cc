#include "HiggsMassKFactors.h"



double HiggsMassKFactors(const float& mcH_m, const float& mH, const int& runningHiggsWidth)
{
  /*
  c     INPUT
  c     mh : Higgs boson mass (used in the POWHEG BOX generation)
  c     gh : Higgs boson width (used in the POWHEG BOX generation)
  c     mt : top quark mass
  c     BWflag : 0    if the sample to reweight was produced with fixed Higgs width
  c              1    if the sample to reweight was produced with running Higgs
  c                   width (this is the default in the POWHEG BOX)
  c     m : virtuality of the produced Higgs boson resonance
  c     OUTPUT
  c     w : the reweighting factor
  */
  
  double m = (double)(mcH_m);
  double mh = (double)(mH);
  double mt = 172.5;
  int BWflag = runningHiggsWidth;
  double w = -1.;
  
  double gh;
  if( mh == 200. ) gh = 1.43;
  if( mh == 250. ) gh = 4.04;
  if( mh == 300. ) gh = 8.43;
  if( mh == 350. ) gh = 15.2;
  if( mh == 400. ) gh = 29.2;
  if( mh == 450. ) gh = 46.9;
  if( mh == 500. ) gh = 68.0;
  if( mh == 550. ) gh = 93.1;
  if( mh == 600. ) gh = 123.;
  
  pwhg_cphto_reweight_(&mh, &gh, &mt, &BWflag, &m, &w);
  
  //std::cout << "w = " << w << std::endl;
  
  return w;
}
