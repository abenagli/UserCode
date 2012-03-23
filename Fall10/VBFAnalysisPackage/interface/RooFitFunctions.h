#include <iostream>

#include "HiggsMassFits.h"

#include "RooRealVar.h"
#include "RooGenericPdf.h"



int DefineRooFitFunction(RooRealVar* x, RooGenericPdf** pdf_bkg, std::vector<RooRealVar*>& pars, std::vector<std::string>& parNames,
                         const std::string& fitMethod, const int& useTurnOn, const int& blockTurnOn, const int& blockParams,
                         const float& mH, const int& step, const std::string& flavour, const std::string& additionalCuts);
