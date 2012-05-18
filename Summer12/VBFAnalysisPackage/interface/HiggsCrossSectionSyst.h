#ifndef HiggsCrossSectionSyst_h
#define HiggsCrossSectionSyst_h

#include <string>
#include <cmath>



double HiggsCrossSectionSyst(const float& mH, const std::string& productionMode, const std::string& updown);

double HiggsQCDScaleSyst(const float& mH, const std::string& productionMode, const std::string& updown);

double HiggsPDFSyst(const float& mH, const std::string& productionMode, const std::string& updown);

double HiggsHighMassSyst(const float& mH, const std::string& productionMode, const std::string& updown);

#endif
