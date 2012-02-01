#ifndef HiggsMassFits_h
#define HiggsMassFits_h

#include "Functions.h"
#include "HiggsMassWindows.h"

#include <string>

#include "TH1F.h"
#include "TF1.h"
#include "TVirtualFitter.h"

#include "RooArgList.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooWorkspace.h"

using namespace RooFit;



void FitHiggsMass(TF1** fitFunc, const std::string& funcName, const float& xMin, const float& xMax,
                  TH1F* h, const float& mH, const std::string& fitMethod = "crystallBallLowHigh");

float GetHiggsMassTurnOnWidth(const float& mH);

#endif