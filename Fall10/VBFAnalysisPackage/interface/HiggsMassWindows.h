#ifndef HiggsMassWindows_h
#define HiggsMassWindows_h

#include <string>
#include <vector>



float GetBinWidth();

std::vector<int> GetMasses();

float GetHiggsWidth(const float& mH);

float GetLepNuWMMIN(const float& mH);
float GetLepNuWMMAX(const float& mH);

float GetXFitMIN1(const float& mH, const std::string& fitMethod = "");
float GetXFitMAX1(const float& mH, const std::string& fitMethod = "");

float GetXFitMIN2(const float& mH, const std::string& fitMethod = "");
float GetXFitMAX2(const float& mH, const std::string& fitMethod = "");

#endif
