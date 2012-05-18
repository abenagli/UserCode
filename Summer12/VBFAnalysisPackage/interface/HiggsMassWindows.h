#ifndef HiggsMassWindows_h
#define HiggsMassWindows_h

#include <string>
#include <vector>

#include "TH1F.h"
#include "TF1.h"
#include "TVirtualFitter.h"



std::vector<int> GetMasses();

float GetHiggsWidth(const float& mH);
float GetHiggsMassFitMIN(const float& mH);
float GetHiggsMassFitMAX(const float& mH);

float GetLepNuWMMIN(const float& mH);
float GetLepNuWMMAX(const float& mH);

#endif
