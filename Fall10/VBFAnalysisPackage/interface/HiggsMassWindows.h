#ifndef HiggsMassWindows_h
#define HiggsMassWindows_h

#include "Functions.h"

#include <string>
#include <vector>

#include "TH1F.h"
#include "TF1.h"
#include "TVirtualFitter.h"



float GetBinWidth();

std::vector<int> GetMasses();

float GetHiggsWidth(const float& mH);
float GetHiggsMassFitMIN(const float& mH);
float GetHiggsMassFitMAX(const float& mH);

void FitHiggsMass(TF1** fitFunc, const std::string& funcName, const float& xMin, const float& xMax,
                  TH1F* h, const float& mH, const std::string& fitMethod = "crystallBallLowHigh");

float GetLepNuWMMIN(const float& mH);
float GetLepNuWMMAX(const float& mH);

float GetXFitMIN1(const float& mH, const std::string& fitMethod = "");
float GetXFitMAX1(const float& mH, const std::string& fitMethod = "");

float GetXFitMIN2(const float& mH, const std::string& fitMethod = "");
float GetXFitMAX2(const float& mH, const std::string& fitMethod = "");

#endif
