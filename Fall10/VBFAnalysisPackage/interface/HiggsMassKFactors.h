#ifndef HiggsMassKFactors_h
#define HiggsMassKFactors_h

#include<iostream>



extern"C" {
  void pwhg_cphto_reweight_(double *mh, double *gh, double *mt, int *BWflag, double *m, double *w);
}

double HiggsMassKFactors(const float& m, const float& mh, const int& runningHiggsWidth);

#endif
