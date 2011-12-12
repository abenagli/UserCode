#ifndef PUUtils_h
#define PUUtils_h

#include "histoFunc.h"

#include <iostream>
#include <iomanip>

#include "TH1F.h"
#include "TF1.h"



/** get the PU scale factor  */
double PURescaleFactor(TH1F* distrPU_DATA, TH1F* distrPU_MC,
                       const double& nPU_MC, const int& PUScale);

#endif
