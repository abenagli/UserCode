#ifndef HiggsMassWindows_h
#define HiggsMassWindows_h

#include <string>



float GetLepNuWMMIN(const float& mH);
float GetLepNuWMMAX(const float& mH);

float GetXFitMIN1(const float& mH, const std::string& method = "");
float GetXFitMAX1(const float& mH, const std::string& method = "");

float GetXFitMIN2(const float& mH, const std::string& method = "");
float GetXFitMAX2(const float& mH, const std::string& method = "");

#endif
