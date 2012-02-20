#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "HiggsMassWindows.h"
#include "HiggsMassFits.h"
#include "HiggsCrossSectionSyst.h"
#include "RooAttenuatedCrystalBallLowHigh.h"

#include <iostream>
#include <iomanip>
#include <sstream>

#include "TH1F.h"
#include "TF1.h"

#include "RooMsgService.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooAbsPdf.h"
#include "RooHistPdf.h"
#include "RooGenericPdf.h"
#include "RooWorkspace.h"
#include "RooGaussian.h"

using namespace std;


float GetXFitMIN1(const float& mH, const int& step = -1, const std::string& additionalCuts = "none");
float GetXFitMAX2(const float& mH);

void GetAttenuatedExponentialTurnOnParameters(float& mu, float& kT, const float& mH, const int& step, const std::string& flavour, const std::string& additionalCuts);
void GetAttenuatedExponentialParameters(float& L1, const float& mH, const int& step, const std::string& flavour, const std::string& additionalCuts);

void GetAttenuatedDoubleExponentialTurnOnParameters(float& mu, float& kT, const float& mH, const int& step, const std::string& additionalCuts);
void GetAttenuatedDoubleExponentialParameters(float& L1, float& L2, float& N, const float& mH, const int& step, const std::string& additionalCuts);






int main(int argc, char** argv)
{
  RooMsgService::instance().deleteStream(0);
  RooMsgService::instance().deleteStream(1);
  
  
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> VBFAnalysis_makeShapes::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
  
  
  //[Input]
  std::string inputDir       = gConfigParser -> readStringOption("Input::inputDir");
  std::string fitMethod      = gConfigParser -> readStringOption("Input::fitMethod");
  std::string analysisMethod = gConfigParser -> readStringOption("Input::analysisMethod");
  
  //[Output]
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  
  //[Options]
  float xWidth = gConfigParser -> readFloatOption("Options::xWidth");
  char xWidthChar[50];
  sprintf(xWidthChar,"%d",int(xWidth));
  
  int step = gConfigParser -> readIntOption("Options::step");
  char stepChar[50];
  sprintf(stepChar,"%d",step);  
  
  std::string additionalCuts = gConfigParser -> readStringOption("Options::additionalCuts");
  
  std::string flavour = gConfigParser -> readStringOption("Options::flavour");
  
  float sigStrength = gConfigParser -> readFloatOption("Options::sigStrength");
  
  
  if( additionalCuts == "none" )
  {  
    inputDir += "/countSignalEvents/binWidth" + std::string(xWidthChar) + "/step" + std::string(stepChar) + "/";
    outputRootFilePath += "/combine_signal/binWidth" + std::string(xWidthChar) + "/step" + std::string(stepChar) + "/";
  }
  else
  {
    inputDir += "/countSignalEvents/binWidth" + std::string(xWidthChar) + "/step" + std::string(stepChar) + "_" + additionalCuts + "/";
    outputRootFilePath += "/combine_signal/binWidth" + std::string(xWidthChar) + "/step" + std::string(stepChar) + "_" + additionalCuts + "/";
  }

  
  
  std::vector<int> masses = GetMasses();
  unsigned int nMasses = masses.size();
  
  for(unsigned int iMass = 0; iMass < nMasses; ++iMass)
  {
    int mass = masses[iMass];
    char massChar[50];
    sprintf(massChar,"%d",mass);
    std::string massString(massChar);
    std::cout << "\n>>> mass: " << massString << std::endl;
    
    
    // define outfile
    std::stringstream ss;
    if( analysisMethod != "sidebands" )
      ss << outputRootFilePath << "/" << outputRootFileName << "_" << analysisMethod << "_" << fitMethod << "_" << mass << "_" << flavour << ".root";
    else
      ss << outputRootFilePath << "/" << outputRootFileName << "_" << analysisMethod << "_" << mass << "_" << flavour << ".root";
    TFile* outFile = new TFile((ss.str()).c_str(),"RECREATE");
    
    
    // define infile
    std::stringstream fileName;
    TFile* inFile;
    
    float xMin;
    float xMax;
    int nBins;
    int binMin;
    int binMax;
    
    TH1F* data;
    TH1F* data_obs;
    double n_data_obs;
     
    TH1F* hint;
    TH1F* bkg;
    TH1F* bkg_fitErrUp;
    TH1F* bkg_fitErrDown;
    TH1F* alpha;
    double n_bkg;
    double n_bkg_err;
    
    TH1F* histo;
    TH1F* histo_fit;
    std::map<std::string,TH1F*> H;
    std::map<std::string,TH1F*> H_fit;
    std::map<std::string,RooDataHist*> dh_H;
    std::map<std::string,RooAbsPdf*> pdf_H;
    std::map<std::string,double> n_H;
    
    RooWorkspace* workspace;
    
    RooRealVar* x;
    RooRealVar* rooXMin;
    RooRealVar* rooXMax;
    RooRealVar* rooXWidth;
    RooRealVar* rooN_data_obs;
    std::map<std::string,RooRealVar*> rooN_H;
    
    RooGenericPdf* pdf_bkg;
    RooRealVar* mu;
    RooRealVar* kT;
    RooRealVar* N;
    RooRealVar* L1;
    RooRealVar* L2;
    
    float tempL1 = -1.;
    float tempL2 = -1.;
    float tempN  = -1.;
    
    RooDataHist* dh_data_obs;
    
    std::map<std::string,RooAttenuatedCrystalBallLowHigh*> pdf_scb;
    std::map<std::string,RooRealVar*> scb_mu;
    std::map<std::string,RooRealVar*> scb_kT;
    std::map<std::string,RooRealVar*> scb_mean;
    std::map<std::string,RooRealVar*> scb_sigma;
    std::map<std::string,RooRealVar*> scb_alpha;
    std::map<std::string,RooRealVar*> scb_n;
    std::map<std::string,RooRealVar*> scb_alpha2;
    std::map<std::string,RooRealVar*> scb_n2;
    
    
    
    
    
    
    //------------------
    // data & bkgshapes
    
    fileName.str(std::string());
    
    if( (analysisMethod == "fit") || (analysisMethod == "fitNoHoles") ||
	(analysisMethod == "fake") || (analysisMethod == "fakeNoHoles") )
      fileName << inputDir << "/countSignalEvents.root";
    
    if( analysisMethod == "sidebands" )
      fileName << inputDir << "/output_017.root";
    
     inFile = TFile::Open(fileName.str().c_str(),"READ");
    //std::cout << ">>> opened file " << fileName.str() << std::endl; 
    
    inFile -> cd();
    
    
    
    
    
    
    //------------------------------------------------------------------------
    
    if( analysisMethod == "fit" )
    {
      std::stringstream histoName;
      
      histoName.str(std::string());
      histoName << "H" << mass << "/h_data" << mass << "_" << flavour;
      data = (TH1F*)( inFile->Get(histoName.str().c_str()) );
      
      histoName.str(std::string());
      histoName << "H" << mass << "/h_dataFit" << mass << "_" << flavour << "_" << fitMethod;      
      hint = (TH1F*)( inFile->Get(histoName.str().c_str()) );
      
      xMin = GetLepNuWMMIN(mass);
      xMax = GetLepNuWMMAX(mass);
    }
    
    
    //------------------------------------------------------------------------
    
    
    if( (analysisMethod == "fitNoHoles") || (analysisMethod == "fakeNoHoles") ) 
    {
      std::stringstream histoName;
      
      histoName.str(std::string());
      histoName << "H" << mass << "/h_data" << mass << "_" << flavour;
      data = (TH1F*)( inFile->Get(histoName.str().c_str()) );
      
      xMin = GetXFitMIN1(mass,step,additionalCuts);
      xMax = GetXFitMAX2(mass);
      
      
      workspace = new RooWorkspace("workspace");
      
      x = new RooRealVar("x","lepNuW_m_KF",0.,1000.);
      x -> setRange(xMin,xMax);
      
      rooXMin   = new RooRealVar("rooXMin",  "",xMin,xMin,xMin);
      rooXMax   = new RooRealVar("rooXMax",  "",xMax,xMax,xMax);
      rooXWidth = new RooRealVar("rooXWidth","",xWidth,xWidth,xWidth);
      
      workspace -> import(*x);
      workspace -> import(*rooXMin);
      workspace -> import(*rooXMax);
      workspace -> import(*rooXWidth);
      
      
      
      float tempMu = -1.; 
      float tempkT = -1.;
      
      if( fitMethod == "exponential" )
      {
        L1 = new RooRealVar((flavour+"_L1").c_str(),"",0.011,0.,0.1);
        pdf_bkg = new RooGenericPdf("bkg","","exp(-1*@1*@0)",RooArgSet(*x,*L1));
        
        workspace -> import(*L1);
        workspace -> import(*pdf_bkg);
      }
      
      if( fitMethod == "attenuatedExponential" )
      {
        GetAttenuatedExponentialTurnOnParameters(tempMu,tempkT,mass,step,flavour,additionalCuts);
        GetAttenuatedExponentialParameters(tempL1,mass,step,flavour,additionalCuts);
        
        mu = new RooRealVar(("CMS_HWWlvjj_"+flavour+"_mu").c_str(),"",tempMu,tempMu,tempMu);
        kT = new RooRealVar(("CMS_HWWlvjj_"+flavour+"_kT").c_str(),"",tempkT,tempkT,tempkT);
        L1 = new RooRealVar(("CMS_HWWlvjj_"+flavour+"_L1").c_str(),"",0.012,0.,0.1);
        pdf_bkg = new RooGenericPdf("bkg","","1./(exp(-1.*(@0-@1)/@2)+1.) * exp(-1*@3*@0)",RooArgSet(*x,*mu,*kT,*L1));
        
        workspace -> import(*mu);
        workspace -> import(*kT);
        workspace -> import(*L1);
        workspace -> import(*pdf_bkg);
      }
      
      if( fitMethod == "doubleExponential" )
      {
        N  = new RooRealVar(("CMS_HWWlvjj_"+flavour+"_N").c_str(), "", 0.711,0.,10.0);
        L1 = new RooRealVar(("CMS_HWWlvjj_"+flavour+"_L1").c_str(),"",0.015,0.,0.1);
        L2 = new RooRealVar(("CMS_HWWlvjj_"+flavour+"_L2").c_str(),"",0.011,0.,0.1);
        pdf_bkg = new RooGenericPdf("bkg","","(exp(-1*@2*@0) + @1*exp(-1*@3*@0))",RooArgSet(*x,*N,*L1,*L2));
        
        workspace -> import(*N);
        workspace -> import(*L1);
        workspace -> import(*L2);
        workspace -> import(*pdf_bkg);
      }
      
      if( fitMethod == "attenuatedDoubleExponential" )
      {
        GetAttenuatedDoubleExponentialTurnOnParameters(tempMu,tempkT,mass,step,additionalCuts);
        GetAttenuatedDoubleExponentialParameters(tempL1,tempL2,tempN,mass,step,additionalCuts);
        
        mu = new RooRealVar(("CMS_HWWlvjj_"+flavour+"_mu").c_str(),"",tempMu,tempMu,tempMu);
        kT = new RooRealVar(("CMS_HWWlvjj_"+flavour+"_kT").c_str(),"",tempkT,tempkT,tempkT);
        N  = new RooRealVar(("CMS_HWWlvjj_"+flavour+"_N").c_str(), "",tempN,0.,1000000.0);
        L1 = new RooRealVar(("CMS_HWWlvjj_"+flavour+"_L1").c_str(),"",tempL1,0.,0.1);
        L2 = new RooRealVar(("CMS_HWWlvjj_"+flavour+"_L2").c_str(),"",tempL2,0.,0.1);
        pdf_bkg = new RooGenericPdf("bkg","","1./(exp(-1.*(@0-@1)/@2)+1.) * (exp(-1*@4*@0) + @3*exp(-1*@5*@0))",RooArgSet(*x,*mu,*kT,*N,*L1,*L2));
        
        workspace -> import(*mu);
        workspace -> import(*kT);
        workspace -> import(*N);
        workspace -> import(*L1);
        workspace -> import(*L2);
        workspace -> import(*pdf_bkg);
      }
    }
    
    
    //------------------------------------------------------------------------
    
    
    if( analysisMethod == "sidebands" )
    {
      data  = (TH1F*)(inFile->Get("signalRegion"));    
      hint  = (TH1F*)(inFile->Get("extrapolated_bkg"));
      alpha = (TH1F*)(inFile->Get("h_correctionBand"));
      
      xMin = alpha -> GetBinLowEdge(1);
      xMax = alpha -> GetBinLowEdge(hint->GetNbinsX()) + xWidth;
    }
    
    
    
    //------------------------------------------------------------------------
    
    
    nBins = int( (xMax - xMin)/xWidth );
    xMax = xMin + nBins*xWidth;
    binMin = 1;
    binMax = nBins;
    
    std::cout << ">>>>>> nBins: " << nBins << "   bin width: " << xWidth << std::endl;
    std::cout << ">>>>>> xMin: " << xMin << std::endl;
    std::cout << ">>>>>> xMax: " << xMax << std::endl;
    
    
    
    
    
    
    //--------------------------
    // data and background shape
    
    
    // data
    if( (analysisMethod == "fit") || (analysisMethod == "sidebands") )
      data_obs = new TH1F("data_obs","",nBins,xMin,xMax);
    if( (analysisMethod == "fitNoHoles") || (analysisMethod == "fakeNoHoles") )
      data_obs = new TH1F("data_obs_dummy","",nBins,xMin,xMax);
    data_obs -> Sumw2();
    
    for(int bin = 1; bin <= data->GetNbinsX(); ++bin)
    {
      float binCenter  = data -> GetBinCenter(bin);
      float binContent = data -> GetBinContent(bin);
      if( (binCenter >= xMin) && (binCenter < xMax) )
        for(int entry = 0; entry < binContent; ++entry)
          data_obs -> Fill(binCenter);
    }
    
    n_data_obs = data_obs -> Integral(binMin,binMax);
    
    
    //------------------------------------------------------------------------
    
    
    if( (analysisMethod == "fit") || (analysisMethod == "sidebands") )
    {
      // background
      bkg            = new TH1F("bkg",           "",nBins,xMin,xMax);
      bkg_fitErrUp   = new TH1F(("CMS_HWWlvjj_"+flavour+"_bkgSystUp").c_str(),  "",nBins,xMin,xMax);
      bkg_fitErrDown = new TH1F(("CMS_HWWlvjj_"+flavour+"_bkgSystDown").c_str(),"",nBins,xMin,xMax);
      
      for(int bin = 1; bin <= hint->GetNbinsX(); ++bin)
      {
        float binCenter  = hint -> GetBinCenter(bin);
        float binContent = hint -> GetBinContent(bin);
        float binError   = hint -> GetBinError(bin);
        if( (binCenter >= xMin) && (binCenter < xMax) )
        {
          int localBin = bkg -> Fill(binCenter,binContent);
          bkg -> SetBinError(localBin,binError);
          bkg_fitErrUp   -> Fill(binCenter,binContent+binError);
          bkg_fitErrDown -> Fill(binCenter,binContent-binError);
        }
      }
      
      outFile -> cd();
      
      data_obs       -> Write();
      bkg            -> Write();
      bkg_fitErrUp   -> Write();
      bkg_fitErrDown -> Write();
      
      n_bkg = bkg -> Integral(binMin,binMax);
      n_bkg_err = bkg_fitErrUp -> Integral(binMin,binMax) - n_bkg;
    }
    

    //------------------------------------------------------------------------
    
        
    if( (analysisMethod == "fitNoHoles") || (analysisMethod == "fakeNoHoles") )
    {
      // data
      dh_data_obs = new RooDataHist("data_obs","",RooArgList(*x),data_obs);
      workspace -> import(*dh_data_obs);
      
      n_data_obs = data_obs -> Integral(binMin,binMax);
      
      rooN_data_obs = new RooRealVar("rooN_data_obs","",n_data_obs,n_data_obs,n_data_obs);
      workspace -> import(*rooN_data_obs);
    }
    
    
    
    
    
    
    //--------------
    // signal shapes
    
    std::vector<std::string> labels;
    labels.push_back("ggH");
    labels.push_back("qqH");
    
    std::vector<std::string> labels_syst;
    labels_syst.push_back("JESUp");
    labels_syst.push_back("JESDown");
    labels_syst.push_back("PUUp");
    labels_syst.push_back("PUDown");
    
    
    for(unsigned int labelIt = 0; labelIt < 2 + 2*labels_syst.size(); ++labelIt)
    {
      std::string label;
      if(labelIt < 2) label = labels.at(labelIt);
      else            label = labels.at(labelIt%2) + "_CMS_HWWlvjj_" + labels_syst.at((labelIt-2)/2);
      
      fileName.str(std::string());
      if(labelIt < 2) fileName << inputDir << "/countSignalEvents.root";
      else            fileName << inputDir << "/countSignalEvents_" + labels_syst.at((labelIt-2)/2) + ".root";
      inFile = TFile::Open(fileName.str().c_str(),"READ");
      //std::cout << ">>> opened file " << fileName.str() << std::endl; 
      //std::cout << ">>>>>> label: " << label << std::endl;
      

      if( labelIt%2 == 0 ) histo = (TH1F*)( inFile->Get(("H"+massString+"/h_ggH"+massString+"_"+flavour).c_str()) );
      if( labelIt%2 == 1 ) histo = (TH1F*)( inFile->Get(("H"+massString+"/h_qqH"+massString+"_"+flavour).c_str()) );
      
      if( labelIt%2 == 0 ) histo_fit = (TH1F*)( inFile->Get(("H"+massString+"/h_ggH"+massString+"_"+flavour+"_fit").c_str()) );
      if( labelIt%2 == 1 ) histo_fit = (TH1F*)( inFile->Get(("H"+massString+"/h_qqH"+massString+"_"+flavour+"_fit").c_str()) );
      
      TF1* fitFunc = histo_fit->GetFunction("fitFunc_scb");
      
      
      outFile -> cd();
      
      
      // binned histogram
      H[label] = new TH1F((label+"_dummy").c_str(),"",nBins,xMin,xMax);
      
      for(int bin = 1; bin <= histo->GetNbinsX(); ++bin)
      {
        float binCenter  = histo -> GetBinCenter(bin);
        float binContent = histo -> GetBinContent(bin);
        float binError   = histo -> GetBinError(bin);
        if( (binCenter >= xMin) && (binCenter < xMax) )
        {
          int localBin = H[label] -> Fill(binCenter,sigStrength*binContent);
          H[label] -> SetBinError(localBin,binError);
        }
      }
      
      n_H[label] = H[label] -> Integral(binMin,binMax);
      
      
      // smooth histogram
      if( (analysisMethod == "fit") || (analysisMethod == "sidebands") )
        H_fit[label] = (TH1F*)( H[label]->Clone(label.c_str()) );
      if( (analysisMethod == "fitNoHoles") || (analysisMethod == "fakeNoHoles") )
        H_fit[label] = (TH1F*)( H[label]->Clone((label+"_dummy").c_str()) );
      H_fit[label] -> Reset();
      for(int bin = 1; bin  <= H_fit[label]->GetNbinsX(); ++bin)
      {
        float binLowEdge = H_fit[label] -> GetBinLowEdge(bin);
        float binWidth   = H_fit[label] -> GetBinWidth(bin);
        H_fit[label] -> SetBinContent(bin,fitFunc->Integral(binLowEdge,binLowEdge+binWidth));
      }
      H_fit[label] -> Scale(n_H[label]/H_fit[label]->Integral());
      
      
      if( (analysisMethod == "fit") || (analysisMethod == "sidebands") )
      {
        H_fit[label] -> Write();         
      }
      
      if( (analysisMethod == "fitNoHoles") || (analysisMethod == "fakeNoHoles") )
      {
        // use smooth histograms for the signal shapes
        /*  
        dh_H[label] = new RooDataHist(("dh_"+label).c_str(),"",RooArgList(*x),H_fit[label]);
        pdf_H[label] = new RooHistPdf(label.c_str(),"",RooArgList(*x),*dh_H[label],2);
        workspace -> import(*pdf_H[label]);
        */
        
        // use a parametric function for the signal shapes
        double scb_muDouble     = 175.;
        double scb_kTDouble     = GetHiggsMassTurnOnWidth(mass);
        double scb_meanDouble   = fitFunc -> GetParameter(3);
        double scb_sigmaDouble  = fitFunc -> GetParameter(4);
        double scb_alphaDouble  = fitFunc -> GetParameter(5);
        double scb_nDouble      = fitFunc -> GetParameter(6);
        double scb_alpha2Double = fitFunc -> GetParameter(7);
        double scb_n2Double     = fitFunc -> GetParameter(8);
        
        scb_mu[label]     = new RooRealVar(("CMS_HWWlvjj_"+flavour+"_scb_mu_"+label).c_str(),    "",scb_muDouble,scb_muDouble,scb_muDouble);
        scb_kT[label]     = new RooRealVar(("CMS_HWWlvjj_"+flavour+"_scb_kT_"+label).c_str(),    "",scb_kTDouble,scb_kTDouble,scb_kTDouble);
        scb_mean[label]   = new RooRealVar(("CMS_HWWlvjj_"+flavour+"_scb_mean_"+label).c_str(),  "",scb_meanDouble,scb_meanDouble,scb_meanDouble);
        scb_sigma[label]  = new RooRealVar(("CMS_HWWlvjj_"+flavour+"_scb_sigma_"+label).c_str(), "",scb_sigmaDouble,scb_sigmaDouble,scb_sigmaDouble);
        scb_alpha[label]  = new RooRealVar(("CMS_HWWlvjj_"+flavour+"_scb_alpha_"+label).c_str(), "",scb_alphaDouble,scb_alphaDouble,scb_alphaDouble);
        scb_n[label]      = new RooRealVar(("CMS_HWWlvjj_"+flavour+"_scb_n_"+label).c_str(),     "",scb_nDouble,scb_nDouble,scb_nDouble);
        scb_alpha2[label] = new RooRealVar(("CMS_HWWlvjj_"+flavour+"_scb_alpha2_"+label).c_str(),"",scb_alpha2Double,scb_alpha2Double,scb_alpha2Double);
        scb_n2[label]     = new RooRealVar(("CMS_HWWlvjj_"+flavour+"_scb_n2_"+label).c_str(),    "",scb_n2Double,scb_n2Double,scb_n2Double);
        
        pdf_scb[label] = new RooAttenuatedCrystalBallLowHigh(label.c_str(),"",
							     *x,
							     *(scb_mu[label]),
							     *(scb_kT[label]),
							     *(scb_mean[label]),
							     *(scb_sigma[label]),
							     *(scb_alpha[label]),
							     *(scb_n[label]),
							     *(scb_alpha2[label]),
							     *(scb_n2[label]));
        
        workspace -> import(*(scb_mu[label]));
        workspace -> import(*(scb_kT[label]));
        workspace -> import(*(scb_mean[label]));
        workspace -> import(*(scb_sigma[label]));
        workspace -> import(*(scb_alpha[label]));
        workspace -> import(*(scb_n[label]));
        workspace -> import(*(scb_alpha2[label]));
        workspace -> import(*(scb_n2[label]));
        workspace -> import(*(pdf_scb[label]));
        
        rooN_H[label] = new RooRealVar(("rooN_"+label).c_str(),"",n_H[label],n_H[label],n_H[label]);
        workspace -> import(*rooN_H[label]);
      }
      
      inFile -> Close();
    }
    
    
    
    if( (analysisMethod == "fitNoHoles") || (analysisMethod == "fakeNoHoles") )
    {
      outFile -> cd();
      workspace -> Write();
    }      
    
    
    
    
    
    
    //------------------------------
    // make datacards - bin counting
    
    std::vector<std::string> labels_bc_num;
    labels_bc_num.push_back("JESUp");
    labels_bc_num.push_back("JESDown");
    labels_bc_num.push_back("PUUp");
    labels_bc_num.push_back("PUDown");
    std::vector<std::string> labels_bc_num_names;
    labels_bc_num_names.push_back("CMS_HWWlvjj_JES    ");
    labels_bc_num_names.push_back("CMS_HWWlvjj_PU     ");
    
    
    //--------------------------------
    // make datacards - shape analysis
    
    std::vector<std::string> labels_sa_num;
    labels_sa_num.push_back("PUUp");
    labels_sa_num.push_back("PUDown");
    std::vector<std::string> labels_sa_num_names;
    labels_sa_num_names.push_back("CMS_HWWlvjj_PU     ");
    
    std::vector<std::string> labels_sa_shape;
    labels_sa_shape.push_back("JESUp");
    labels_sa_shape.push_back("JESDown");
    std::vector<std::string> labels_sa_shape_names;
    labels_sa_shape_names.push_back("CMS_HWWlvjj_JES    ");
    
    std::stringstream name;
    
    
    
    if( (analysisMethod == "fit") || (analysisMethod == "sidebands") )
    {
      /*
      std::stringstream ss2;
      if( analysisMethod != "sidebands" )
        ss2 << outputRootFilePath << "/" << "datacard" << "_" << analysisMethod << "_" << fitMethod << "_" << "bincounting" << "_" << mass << "_" << flavour << ".txt";
      else
      ss2 << outputRootFilePath << "/" << "datacard" << "_" << analysisMethod << "_" << "bincounting" << "_" << mass << "_" << flavour << ".txt";
      std::ofstream datacard_bc(ss2.str().c_str(),std::ios::out);
      
      datacard_bc << std::fixed;
      datacard_bc << std::left;
      datacard_bc << "********* H > WW > lvjj analysis for mH = " << mass << " GeV/c^2 *********" << std::endl;
      datacard_bc << "********* counting experiment *********" << std::endl;
      datacard_bc << "-----------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
      datacard_bc << "imax 1   # number of channels" << std::endl;
      datacard_bc << "jmax 2   # number of processes - 1" << std::endl;
      datacard_bc << "kmax " << 4 + labels_bc_num_names.size() << "   # number of nuisance parameters (sources of systematic uncertainties)" << std::endl;
      datacard_bc << "-----------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
      datacard_bc << "bin                           " << setw(7) << flavour << std::endl;
      datacard_bc << "observation                   " << std::setprecision(0) << setw(7) << n_data_obs << std::endl;    
      datacard_bc  << "-----------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
      datacard_bc << "bin                           "
                  << setw(7) << flavour << "    "
                  << setw(7) << flavour << "    "
                  << setw(7) << flavour << std::endl;
      datacard_bc << "process                           ggH        qqH        bkg" << std::endl;
      datacard_bc << "process                            -1         -2          1" << std::endl;
      datacard_bc << "rate             " << std::setprecision(2)
                  << setw(8) << n_H["ggH"] << "   "
                  << setw(8) << n_H["qqH"] << "   "
                  << setw(8) << n_bkg
                  << std::endl;
      datacard_bc << std::endl;
      datacard_bc  << "-----------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
      datacard_bc << "fitErr                 lnN          -          -   " << std::setprecision(3) << setw(8) << 1.+n_bkg_err/n_bkg << std::endl;
      datacard_bc << "lumi                   lnN      1.045      1.045          -" << std::endl;
      datacard_bc << "other                  lnN      1.060      1.060          -" << std::endl;
      datacard_bc << "QCDscale_ggH           lnN   "
                  << std::setprecision(3) << setw(8) << 1.+0.5*(HiggsQCDScaleSyst(mass,"gg","up")+HiggsQCDScaleSyst(mass,"gg","down")) << "   "
                  << "       -" << "   " 
                  << "       -" << std::endl;
      datacard_bc << "QCDscale_qqH           lnN   "
                  << "       -" << "   "
                  << std::setprecision(3) << setw(8) << 1.+0.5*(HiggsQCDScaleSyst(mass,"qq","up")+HiggsQCDScaleSyst(mass,"qq","down")) << "   "
                  << "       -" << std::endl;
      datacard_bc << "pdf_gg                 lnN   "
                  << std::setprecision(3) << setw(8) << 1.+0.5*(HiggsPDFSyst(mass,"gg","up")+HiggsPDFSyst(mass,"gg","down")) << "   "
                  << "       -" << "   "
                  << "       -" << std::endl;
      datacard_bc << "pdf_qqbar              lnN   "
                  << "       -" << "   "
                  << std::setprecision(3) << setw(8) << 1.+0.5*(HiggsPDFSyst(mass,"qq","up")+HiggsPDFSyst(mass,"qq","down")) << "   "
                  << "       -" << std::endl;
      datacard_bc << "theoryUncXS_HighMH     lnN   "
                  << std::setprecision(3) << setw(8) << 1.+0.5*(HiggsHighMassSyst(mass,"gg","up")+HiggsHighMassSyst(mass,"gg","down")) << "   "
                  << "       -" << "   "
                  << "       -" << std::endl;
      for(unsigned int labelIt = 0; labelIt < labels_bc_num_names.size(); ++labelIt)
      {
        double ggH_errUp   = fabs(n_H["ggH_CMS_HWWlvjj_"+labels_bc_num.at(2*labelIt)]   - n_H["ggH"]);
        double qqH_errUp   = fabs(n_H["qqH_CMS_HWWlvjj_"+labels_bc_num.at(2*labelIt)]   - n_H["qqH"]);
        double ggH_errDown = fabs(n_H["ggH_CMS_HWWlvjj_"+labels_bc_num.at(2*labelIt+1)] - n_H["ggH"]);
        double qqH_errDown = fabs(n_H["qqH_CMS_HWWlvjj_"+labels_bc_num.at(2*labelIt+1)] - n_H["qqH"]);
        
        double ggH_errAve  = 0.5*(ggH_errUp + ggH_errDown);
        double qqH_errAve  = 0.5*(qqH_errUp + qqH_errDown);
        
        datacard_bc << labels_bc_num_names.at(labelIt) << "    lnN   "
                    << std::setprecision(3) << setw(8) << 1. + ggH_errAve/n_H["ggH"] << "   "
                    << std::setprecision(3) << setw(8) << 1. + qqH_errAve/n_H["qqH"] << "   "
                    << "       -" << std::endl;
      }
      */
    }      
    
    
    
    if( (analysisMethod == "sidebands") || (analysisMethod == "fitNoHoles") || (analysisMethod == "fakeNoHoles") )
    {
      std::stringstream ss3;
      if( analysisMethod == "sidebands" )
        ss3 << outputRootFilePath << "/" << "datacard" << "_" << analysisMethod << "_"<< "shapeanalysis" << "_" << mass << "_" << flavour << ".txt";
      else
        ss3 << outputRootFilePath << "/" << "datacard" << "_" << analysisMethod << "_" << fitMethod << "_"<< "shapeanalysis" << "_" << mass << "_" << flavour << ".txt";      
      std::ofstream datacard_sa(ss3.str().c_str(),std::ios::out);
      
      datacard_sa << std::fixed;
      datacard_sa << std::left;
      
      datacard_sa << "********* H > WW > lvjj analysis for mH = " << mass << " GeV/c^2 *********" << std::endl;
      datacard_sa << "********* parametric shape analysis *********" << std::endl;
      
      datacard_sa << "-----------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
      datacard_sa << "imax 1   # number of channels" << std::endl;
      datacard_sa << "jmax 2   # number of processes - 1" << std::endl;
      datacard_sa << "kmax *   # number of nuisance parameters (sources of systematic uncertainties)" << std::endl;
      
      datacard_sa << "-----------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
      if( analysisMethod == "sidebands" )
      {
        datacard_sa << "shapes *          " << setw(4) << flavour << "   "
	  	    << "shapes_" << analysisMethod << "_" << mass << "_" << flavour << ".root   "
                    << "workspace:$PROCESS workspace:$PROCESS_$SYSTEMATIC" << std::endl;
        datacard_sa << "shapes data_obs   " << setw(4) << flavour << "   "
                    << "shapes_" << analysisMethod << "_" << mass << "_" << flavour << ".root   "
                    << "workspace:data_obs" << std::endl;
      }
      else
      {
        datacard_sa << "shapes *          " << setw(4) << flavour << "   "
	  	    << "shapes_" << analysisMethod << "_" << fitMethod << "_" << mass << "_" << flavour << ".root   "
                    << "workspace:$PROCESS workspace:$PROCESS_$SYSTEMATIC" << std::endl;
        datacard_sa << "shapes data_obs   " << setw(4) << flavour << "   "
                    << "shapes_" << analysisMethod << "_" << fitMethod << "_" << mass << "_" << flavour << ".root   "
                    << "workspace:data_obs" << std::endl;
      }
      datacard_sa << "-----------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
      datacard_sa << setw(33) << "bin" << "   "
                  << setw(8) << flavour << std::endl;
      datacard_sa << setw(33) << "observation" << "   "
                  << std::setprecision(0) << setw(8) << n_data_obs << std::endl;    
      
      datacard_sa << "-----------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
      datacard_sa << setw(33) << "bin" << "   "
                  << setw(8) << flavour << "   "
                  << setw(8) << flavour << "   "
                  << setw(8) << flavour << std::endl;
      
      datacard_sa << setw(33) << "process" << "   "
                  << setw(8) << "ggH" << "   "
                  << setw(8) << "qqH" << "   "
                  << setw(8) << "bkg" << std::endl;
      
      datacard_sa << setw(33) << "process" << "   "
                  << setw(8) << "-2" << "   "
                  << setw(8) << "-1" << "   "
                  << setw(8) << "1" << std::endl;
      
      if( analysisMethod == "sidebands")
      {
        datacard_sa << setw(33) << "rate" << "   "
                    << std::setprecision(2) << setw(8) << n_H["ggH"] << "   "
                    << std::setprecision(2) << setw(8) << n_H["qqH"] << "   "
                    << std::setprecision(2) << setw(8) << n_bkg << std::endl;
      }
      else
      {
        datacard_sa << setw(33) << "rate" << "   "
                    << std::setprecision(2) << setw(8) << n_H["ggH"] << "   "
                    << std::setprecision(2) << setw(8) << n_H["qqH"] << "   "
                    << std::setprecision(2) << setw(8) << n_data_obs << std::endl;
      }
      
      datacard_sa  << "-----------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
      datacard_sa << setw(25) << "lumi"     << "   " << setw(5) << "lnN" << "   " << setw(8) << "1.045" << "   " << setw(8) << "1.045" << "   " << setw(8) << "-" << std::endl;
      
      datacard_sa << setw(25) << "QCDscale_ggH" << "   " << setw(5) << "lnN" << "   "
                  << std::setprecision(3) << setw(8) << 1.+0.5*(HiggsQCDScaleSyst(mass,"gg","up")+HiggsQCDScaleSyst(mass,"gg","down")) << "   "
                  << setw(8) << "-" << "   " 
                  << setw(8) << "-" << std::endl;
      
      datacard_sa << setw(25) << "QCDscale_qqH" << "   " << setw(5) << "lnN" << "   "
                  << setw(8) << "-" << "   "
                  << std::setprecision(3) << setw(8) << 1.+0.5*(HiggsQCDScaleSyst(mass,"qq","up")+HiggsQCDScaleSyst(mass,"qq","down")) << "   "
                  << setw(8) << "-" << std::endl;
      
      datacard_sa << setw(25) << "pdf_gg" << "   " << setw(5) << "lnN" << "   "
                  << std::setprecision(3) << setw(8) << 1.+0.5*(HiggsPDFSyst(mass,"gg","up")+HiggsPDFSyst(mass,"gg","down")) << "   "
                  << setw(8) << "-" << "   "
                  << setw(8) << "-" << std::endl;
      
      datacard_sa << setw(25) << "pdf_qqbar" << "   " << setw(5) << "lnN" << "   "
                  << setw(8) << "-" << "   "
                  << std::setprecision(3) << setw(8) << 1.+0.5*(HiggsPDFSyst(mass,"qq","up")+HiggsPDFSyst(mass,"qq","down")) << "   "
                  << setw(8) << "-" << std::endl;
      
      datacard_sa << setw(25) << "theoryUncXS_HighMH" << "   " << setw(5) << "lnN" << "   "
                  << std::setprecision(3) << setw(8) << 1.+0.5*(HiggsHighMassSyst(mass,"gg","up")+HiggsHighMassSyst(mass,"gg","down")) << "   "
                  << setw(8) << "-" << "   "
                  << setw(8) << "-" << std::endl;
      
      for(unsigned int labelIt = 0; labelIt < labels_sa_num_names.size(); ++labelIt)
      {
        double ggH_errUp   = fabs(n_H["ggH_CMS_HWWlvjj_"+labels_sa_num.at(2*labelIt)]   - n_H["ggH"]);
        double qqH_errUp   = fabs(n_H["qqH_CMS_HWWlvjj_"+labels_sa_num.at(2*labelIt)]   - n_H["qqH"]);
        double ggH_errDown = fabs(n_H["ggH_CMS_HWWlvjj_"+labels_sa_num.at(2*labelIt+1)] - n_H["ggH"]);
        double qqH_errDown = fabs(n_H["qqH_CMS_HWWlvjj_"+labels_sa_num.at(2*labelIt+1)] - n_H["qqH"]);
        
        double ggH_errAve  = 0.5*(ggH_errUp + ggH_errDown);
        double qqH_errAve  = 0.5*(qqH_errUp + qqH_errDown);
        
        datacard_sa << setw(25) << labels_sa_num_names.at(labelIt) << "   " << setw(5) << "lnN" << "   "
                    << std::setprecision(3) << setw(8) << 1. + ggH_errAve/n_H["ggH"] << "   "
                    << std::setprecision(3) << setw(8) << 1. + qqH_errAve/n_H["qqH"] << "   "
                    << setw(8) << "-" << std::endl;
      }
      for(unsigned int labelIt = 0; labelIt < labels_sa_shape_names.size(); ++labelIt)
      {
        datacard_sa << setw(25) << labels_sa_shape_names.at(labelIt) << "   " << setw(5) << "shape" << "   "
                    << setw(8) << "1." << "   "
                    << setw(8) << "1." << "   "
                    << setw(8) << "-" << std::endl;
      }
      
      name.str(std::string()); name << "CMS_HWWlvjj_" << flavour << "_other";
      datacard_sa << setw(25) << name.str() << "   " << setw(5) << "lnN" << "   " << setw(8) << "1.060" << "   " << setw(8) << "1.060" << "   " << setw(8) << "-" << std::endl;
      
      if( analysisMethod == "sidebands" )
      {
        name.str(std::string()); name << "CMS_HWWlvjj_" << flavour << "_bkgSyst";
        datacard_sa << setw(25) << name.str() << "   " << setw(5) << "shape" << "   " << setw(8) << "-" << "   " << setw(8) << "-" << "   " << setw(8) << "1." << std::endl;
      }
      else
      {
        name.str(std::string()); name << "CMS_HWWlvjj_" << flavour << "_bkgNorm";
        datacard_sa << setw(25) << name.str() << "   " << setw(5) << "lnU" << "   " << setw(8) << "-" << "   " << setw(8) << "-" << "   " << setw(8) << "1.500" << std::endl;
      }
      
      if( analysisMethod != "sidebands" )
      {
        datacard_sa << "-----------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
        if( (fitMethod == "exponential") || (fitMethod == "attenuatedExponential") )
        {
          name.str(std::string()); name << "CMS_HWWlvjj_" << flavour << "_L1";
          datacard_sa << setw(15) << name.str() << "   param   " << std::setprecision(3) << setw(12) << tempL1 << "   1.   [0.,0.1]"  << std::endl; 
        }
        if( (fitMethod == "doubleExponential") || (fitMethod == "attenuatedDoubleExponential") )
        {
          name.str(std::string()); name << "CMS_HWWlvjj_" << flavour << "_N";
          datacard_sa << setw(15) << name.str() << "   param   " << std::setprecision(3) << setw(12) << tempN  << "   1.   [0.,1000000.]" << std::endl; 
          name.str(std::string()); name << "CMS_HWWlvjj_" << flavour << "_L1";
          datacard_sa << setw(15) << name.str() << "   param   " << std::setprecision(3) << setw(12) << tempL1 << "   1.   [0.,0.1]"      << std::endl; 
          name.str(std::string()); name << "CMS_HWWlvjj_" << flavour << "_L2";
          datacard_sa << setw(15) << name.str() << "   param   " << std::setprecision(3) << setw(12) << tempL2 << "   1.   [0.,0.1]"      << std::endl; 
        }
      }
    }
    
    outFile -> Close();
  }
  
  
  
  return 1;
}






float GetXFitMIN1(const float& mH, const int& step, const std::string& additionalCuts)
{
  if( (step <= 13) && (additionalCuts == "none") )
  {
    if     ( mH == 200. ) return 180.;
    if     ( mH == 250. ) return 180.;
    else if( mH == 300. ) return 180.;
    else if( mH == 350. ) return 180.;
    else if( mH == 400. ) return 180.;
    else if( mH == 450. ) return 180.;
    else if( mH == 500. ) return 180.;
    else if( mH == 550. ) return 180.;
    else if( mH == 600. ) return 180.;
    else return 1.;
  }
  
  if( (step <= 13) && (additionalCuts == "Dphi") )
  {
    if     ( mH == 200. ) return 200.;
    if     ( mH == 250. ) return 200.;
    else if( mH == 300. ) return 200.;
    else if( mH == 350. ) return 200.;
    else if( mH == 400. ) return 220.;
    else if( mH == 450. ) return 220.;
    else if( mH == 500. ) return 220.;
    else if( mH == 550. ) return 240.;
    else if( mH == 600. ) return 240.;
    else return 1.;
  }
  
  else if( (step > 13) && (additionalCuts == "none") )
  {
    if     ( mH == 200. ) return 200.;
    if     ( mH == 250. ) return 200.;
    else if( mH == 300. ) return 200.;
    else if( mH == 350. ) return 200.;
    else if( mH == 400. ) return 200.;
    else if( mH == 450. ) return 200.;
    else if( mH == 500. ) return 200.;
    else if( mH == 550. ) return 200.;
    else if( mH == 600. ) return 200.;
    else return 1.;
  }
  
  else if( (step > 13) && (additionalCuts == "Dphi") )
  {
    if     ( mH == 200. ) return 200.;
    if     ( mH == 250. ) return 200.;
    else if( mH == 300. ) return 200.;
    else if( mH == 350. ) return 200.;
    else if( mH == 400. ) return 220.;
    else if( mH == 450. ) return 220.;
    else if( mH == 500. ) return 220.;
    else if( mH == 550. ) return 240.;
    else if( mH == 600. ) return 240.;
    else return 1.;
  }
  
  else return -1.;
}



float GetXFitMAX2(const float& mH)
{
  if     ( mH == 200. ) return 800.;
  else if( mH == 250. ) return 800.;
  else if( mH == 300. ) return 800.;
  else if( mH == 350. ) return 800.;
  else if( mH == 400. ) return 800.;
  else if( mH == 450. ) return 800.;
  else if( mH == 500. ) return 800.;
  else if( mH == 550. ) return 800.;
  else if( mH == 600. ) return 800.;
  else return -1.;
}






void GetAttenuatedExponentialTurnOnParameters(float& mu, float& kT, const float& mH, const int& step, const std::string& flavour, const std::string& additionalCuts)
{
  if( (step <= 13) && (flavour == "e") && (additionalCuts == "none") )
  {
    mu = 135.;
    kT = 20.;
  }
  if( (step <= 13) && (flavour == "mu") && (additionalCuts == "none") )
  {
     mu = 100.;
     kT = 1.;
  }
}

void GetAttenuatedExponentialParameters(float& L1, const float& mH, const int& step, const std::string& flavour, const std::string& additionalCuts)
{
  if( (step <= 13) && (flavour == "e") && (additionalCuts == "none") )
  {
    L1 = 0.012;
  }
  if( (step <= 13) && (flavour == "mu") && (additionalCuts == "none") )
  {
    L1 = 0.011;
  }
}






void GetAttenuatedDoubleExponentialTurnOnParameters(float& mu, float& kT, const float& mH, const int& step, const std::string& additionalCuts)
{
  if( (step <= 13) && (additionalCuts == "none") )
  {
    if     ( mH == 200. ) { mu = 190.; kT = 55.; }
    else if( mH == 250. ) { mu = 190.; kT = 55.; }
    else if( mH == 300. ) { mu = 190.; kT = 55.; }
    else if( mH == 350. ) { mu = 190.; kT = 55.; }
    else if( mH == 400. ) { mu = 190.; kT = 55.; }
    else if( mH == 450. ) { mu = 190.; kT = 55.; }
    else if( mH == 500. ) { mu = 190.; kT = 55.; }
    else if( mH == 550. ) { mu = 190.; kT = 55.; }
    else if( mH == 600. ) { mu = 190.; kT = 55.; }
  }
  
  if( (step <= 13) && (additionalCuts == "Dphi") )
  {
    if     ( mH == 200. ) { mu = 203.; kT = 22.; }
    else if( mH == 250. ) { mu = 203.; kT = 22.; }
    else if( mH == 300. ) { mu = 203.; kT = 22.; }
    else if( mH == 350. ) { mu = 203.; kT = 22.; }
    else if( mH == 400. ) { mu = 240.; kT = 18.; }
    else if( mH == 450. ) { mu = 240.; kT = 18.; }
    else if( mH == 500. ) { mu = 240.; kT = 18.; }
    else if( mH == 550. ) { mu = 257.; kT = 19.; }
    else if( mH == 600. ) { mu = 257.; kT = 19.; }
  }
  
  if( (step > 13) && (additionalCuts == "none") )
  {
    if     ( mH == 200. ) { mu = 246.; kT = 21.; }
    else if( mH == 250. ) { mu = 246.; kT = 21.; }
    else if( mH == 300. ) { mu = 246.; kT = 21.; }
    else if( mH == 350. ) { mu = 246.; kT = 21.; }
    else if( mH == 400. ) { mu = 246.; kT = 21.; }
    else if( mH == 450. ) { mu = 246.; kT = 21.; }
    else if( mH == 500. ) { mu = 246.; kT = 21.; }
    else if( mH == 550. ) { mu = 246.; kT = 21.; }
    else if( mH == 600. ) { mu = 246.; kT = 21.; }
  }
  
  if( (step > 13) && (additionalCuts == "Dphi") )
  {
    if     ( mH == 200. ) { mu = 205.; kT = 20.; }
    else if( mH == 250. ) { mu = 205.; kT = 20.; }
    else if( mH == 300. ) { mu = 205.; kT = 20.; }
    else if( mH == 350. ) { mu = 205.; kT = 20.; }
    else if( mH == 400. ) { mu = 242.; kT = 18.; }
    else if( mH == 450. ) { mu = 242.; kT = 18.; }
    else if( mH == 500. ) { mu = 242.; kT = 18.; }
    else if( mH == 550. ) { mu = 259.; kT = 20.; }
    else if( mH == 600. ) { mu = 259.; kT = 20.; }
  }

}

void GetAttenuatedDoubleExponentialParameters(float& L1, float& L2, float& N, const float& mH, const int& step, const std::string& additionalCuts)
{
  if( (step <= 13) && (additionalCuts == "none") )
  {
    if     ( mH == 200. ) { L1 = 0.016; L2 = 0.012; N = 43.6; }
    else if( mH == 250. ) { L1 = 0.016; L2 = 0.012; N = 43.6; }
    else if( mH == 300. ) { L1 = 0.016; L2 = 0.012; N = 43.6; }
    else if( mH == 350. ) { L1 = 0.016; L2 = 0.012; N = 43.6; }
    else if( mH == 400. ) { L1 = 0.016; L2 = 0.012; N = 43.6; }
    else if( mH == 450. ) { L1 = 0.016; L2 = 0.012; N = 43.6; }
    else if( mH == 500. ) { L1 = 0.016; L2 = 0.012; N = 43.6; }
    else if( mH == 550. ) { L1 = 0.016; L2 = 0.012; N = 43.6; }
    else if( mH == 600. ) { L1 = 0.016; L2 = 0.012; N = 43.6; }
  }
  
  if( (step <= 13) && (additionalCuts == "Dphi") )
  {
    if     ( mH == 200. ) { L1 = 0.013; L2 = 0.060; N = 1.5e-05; }
    else if( mH == 250. ) { L1 = 0.013; L2 = 0.060; N = 1.5e-05; }
    else if( mH == 300. ) { L1 = 0.013; L2 = 0.060; N = 1.5e-05; }
    else if( mH == 350. ) { L1 = 0.013; L2 = 0.060; N = 1.5e-05; }
    else if( mH == 400. ) { L1 = 0.013; L2 = 0.060; N = 7.58e-07; }
    else if( mH == 450. ) { L1 = 0.013; L2 = 0.060; N = 7.58e-07; }
    else if( mH == 500. ) { L1 = 0.013; L2 = 0.060; N = 7.58e-07; }
    else if( mH == 550. ) { L1 = 0.013; L2 = 0.057; N = 519; }
    else if( mH == 600. ) { L1 = 0.013; L2 = 0.057; N = 519; }
  }
  
  if( (step > 13) && (additionalCuts == "none") )
  {
    if     ( mH == 200. ) { L1 = 0.013; L2 = 0.057; N = 38900; }
    else if( mH == 250. ) { L1 = 0.013; L2 = 0.057; N = 38900; }
    else if( mH == 300. ) { L1 = 0.013; L2 = 0.057; N = 38900; }
    else if( mH == 350. ) { L1 = 0.013; L2 = 0.057; N = 38900; }
    else if( mH == 400. ) { L1 = 0.013; L2 = 0.057; N = 38900; }
    else if( mH == 450. ) { L1 = 0.013; L2 = 0.057; N = 38900; }
    else if( mH == 500. ) { L1 = 0.013; L2 = 0.057; N = 38900; }
    else if( mH == 550. ) { L1 = 0.013; L2 = 0.057; N = 38900; }
    else if( mH == 600. ) { L1 = 0.013; L2 = 0.057; N = 38900; }
  }
  
  if( (step > 13) && (additionalCuts == "Dphi") )
  {
    if     ( mH == 200. ) { L1 = 0.010; L2 = 0.013; N = 146; }
    else if( mH == 250. ) { L1 = 0.010; L2 = 0.013; N = 146; }
    else if( mH == 300. ) { L1 = 0.010; L2 = 0.013; N = 146; }
    else if( mH == 350. ) { L1 = 0.010; L2 = 0.013; N = 146; }
    else if( mH == 400. ) { L1 = 0.011; L2 = 0.013; N = 150.; }
    else if( mH == 450. ) { L1 = 0.011; L2 = 0.013; N = 150.; }
    else if( mH == 500. ) { L1 = 0.011; L2 = 0.013; N = 150.; }
    else if( mH == 550. ) { L1 = 0.013; L2 = 0.013; N = 170.; }
    else if( mH == 600. ) { L1 = 0.013; L2 = 0.013; N = 170.; }
  }
}
