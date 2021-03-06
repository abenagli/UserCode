#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "HiggsMassWindows.h"
#include "HiggsMassFits.h"
#include "RooFitFunctions.h"
#include "HiggsCrossSectionSyst.h"
#include "FitBiasSyst.h"
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
#include "RooExtendPdf.h"
#include "RooWorkspace.h"
#include "RooGaussian.h"

using namespace std;


float GetXFitMIN1(const float& mH, const int& step = -1, const std::string& additionalCuts = "none");
float GetXFitMAX2(const float& mH);






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
  int useTurnOn   = gConfigParser -> readIntOption("Options::useTurnOn");
  int blockTurnOn = gConfigParser -> readIntOption("Options::blockTurnOn");
  int blockParams = gConfigParser -> readIntOption("Options::blockParams");
  if( useTurnOn == 0 ) blockTurnOn = 1;
  
  int sigSyst = gConfigParser -> readIntOption("Options::sigSyst");
  int bkgSyst = gConfigParser -> readIntOption("Options::bkgSyst");
  
  float xWidth = gConfigParser -> readFloatOption("Options::xWidth");
  char xWidthChar[50];
  sprintf(xWidthChar,"%d",int(xWidth));
  
  int step = gConfigParser -> readIntOption("Options::step");
  char stepChar[50];
  sprintf(stepChar,"%d",step);  
  
  std::string additionalCuts = gConfigParser -> readStringOption("Options::additionalCuts");
  
  std::string flavour = gConfigParser -> readStringOption("Options::flavour");
  std::string charge  = gConfigParser -> readStringOption("Options::charge");
  if( charge != "pm" ) charge = "_" + charge;
  else                 charge.clear();
  
  float sigStrength = gConfigParser -> readFloatOption("Options::sigStrength");
  
  int nToys = gConfigParser -> readIntOption("Options::nToys");  
  std::string toyParent = gConfigParser -> readStringOption("Options::toyParent");
    
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
    std::cout << "\n\n>>> mass: " << massString << std::endl;
    
    
    // define outfile
    std::stringstream ss;
    
    if( analysisMethod != "sidebands" )
      ss << outputRootFilePath << "/" << outputRootFileName << "_" << analysisMethod << "_" << fitMethod << "_" << mass << "_" << flavour << charge << ".root";
    else
      ss << outputRootFilePath << "/" << outputRootFileName << "_" << analysisMethod << "_" << mass << "_" << flavour << charge << ".root";
    
    TFile* outFile = new TFile((ss.str()).c_str(),"RECREATE");
    
    
    TFile* outFile_toy;
    if( nToys > 0 )
    {
      ss.str(std::string());
      
      if( analysisMethod != "sidebands" )
        ss << outputRootFilePath << "/" << outputRootFileName << "_" << analysisMethod << "_" << fitMethod << "_" << mass << "_" << flavour << charge << "_" << toyParent << "_toy.root";
      else
        ss << outputRootFilePath << "/" << outputRootFileName << "_" << analysisMethod << "_" << mass << "_" << flavour << charge << "_" << toyParent << "_toy.root";
      
      outFile_toy = new TFile((ss.str()).c_str(),"RECREATE");
    }  

    
    
    // define infile
    std::stringstream fileName;
    TFile* inFile;
    TFile* inFile2;
        
    float xMin;
    float xMax;
    int nBins;
    int binMin;
    int binMax;
    
    TH1F* data = NULL;
    TH1F* data_obs = NULL;
    double n_data_obs;
     
    TH1F* hint = NULL;
    TH1F* bkg = NULL;
    TH1F* bkg_fitErrUp = NULL;
    TH1F* bkg_fitErrDown = NULL;
    TH1F* alpha = NULL;
    double n_bkg;
    double n_bkg_err;
    
    TH1F* histo = NULL;
    TH1F* histo_fit = NULL;
    std::map<std::string,TH1F*> H;
    std::map<std::string,TH1F*> H_fit;
    std::map<std::string,RooDataHist*> dh_H;
    std::map<std::string,RooAbsPdf*> pdf_H;
    std::map<std::string,double> n_H;
    
    RooWorkspace* workspace = NULL;
    
    RooRealVar* x = NULL;
    RooRealVar* rooXMin = NULL;
    RooRealVar* rooXMax = NULL;
    RooRealVar* rooXWidth = NULL;
    RooRealVar* rooN_data_obs = NULL;
    std::map<std::string,RooRealVar*> rooN_H;
    
    RooGenericPdf*  pdf_bkg_nonExt = NULL;
    RooGenericPdf*  pdf_bkg_nonExt_clone = NULL;
    RooExtendPdf* pdf_bkg = NULL;
    RooRealVar* norm = NULL;
    int nPars;
    std::vector<RooRealVar*> pars;
    std::vector<std::string> parNames;
    
    RooDataHist* dh_data_obs = NULL;
    
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
    
    for(int toyIt = 0; toyIt < nToys+1; ++toyIt)
    {
      std::stringstream toyLabel;
      if( toyIt > 0 )
      {
        toyLabel << "_toy" << toyIt;
        std::cout << ">>>>>> toy " << toyIt << "\r" << std::flush;
      }
      
      
      //------------------------------------------------------------------------
      
      
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
        histoName << "H" << mass << "/h_data" << mass << "_" << flavour << charge;
        data = (TH1F*)( inFile->Get(histoName.str().c_str()) );
        
        histoName.str(std::string());
        histoName << "H" << mass << "/h_dataFit" << mass << "_" << flavour << charge << "_" << fitMethod;      
        hint = (TH1F*)( inFile->Get(histoName.str().c_str()) );
        
        xMin = GetLepNuWMMIN(mass);
        xMax = GetLepNuWMMAX(mass);
      }
      
      
      //------------------------------------------------------------------------
      
      
      if( (analysisMethod == "fitNoHoles") || (analysisMethod == "fakeNoHoles") ) 
      {
        std::stringstream histoName;
        
        histoName.str(std::string());
        histoName << "H" << mass << "/h_data" << mass << "_" << flavour << charge;
        data = (TH1F*)( inFile->Get(histoName.str().c_str()) );
        
        xMin = GetXFitMIN1(mass,step,additionalCuts);
        xMax = GetXFitMAX2(mass);
        
        
        if( toyIt == 0 )
          workspace = new RooWorkspace("workspace");
        else
          workspace = new RooWorkspace(("workspace_"+toyParent+toyLabel.str()).c_str());
        
        
        if( toyIt == 0 )
        {
          x = new RooRealVar("x","lepNuW_m_KF",0.,1000.);
          x -> setRange(xMin,xMax);
          
          rooXMin   = new RooRealVar("rooXMin",  "",xMin,xMin,xMin);
          rooXMax   = new RooRealVar("rooXMax",  "",xMax,xMax,xMax);
          rooXWidth = new RooRealVar("rooXWidth","",xWidth,xWidth,xWidth);
        }
          
        workspace -> import(*x);
        workspace -> import(*rooXMin);
        workspace -> import(*rooXMax);
        workspace -> import(*rooXWidth);
        
        if( toyIt == 0 )
        {
          nPars = DefineRooFitFunction(x,&pdf_bkg_nonExt,pars,parNames,fitMethod,useTurnOn,blockTurnOn,blockParams,mass,step,flavour,charge,additionalCuts);
          
          for(int parIt = 0; parIt < nPars; ++parIt)
            workspace -> import(*pars[parIt]);
          
          norm = new RooRealVar(("CMS_HWWlvjj_"+flavour+charge+"_norm").c_str(), "",n_data_obs,0.,1.E+08);
          pdf_bkg = new RooExtendPdf("bkg","",*pdf_bkg_nonExt,*norm);
          //pdf_bkg_nonExt_clone = (RooGenericPdf*) pdf_bkg_nonExt -> clone("bkg");
        }
        
        workspace -> import(*norm);
        workspace -> import(*pdf_bkg);
//        workspace -> import(*pdf_bkg_nonExt_clone);
      }
      
      
      //------------------------------------------------------------------------
      
      
      if( analysisMethod == "sidebands" )
      {
        data  = (TH1F*)(inFile->Get("signalRegion"));    
        hint  = (TH1F*)(inFile->Get("extrapolated_bkg"));
        alpha = (TH1F*)(inFile->Get("num_fit_error"));
        
        xMin = alpha -> GetBinLowEdge(1);
        xMax = alpha -> GetBinLowEdge(alpha->GetNbinsX()) + xWidth;
      }
      
      
      
      //------------------------------------------------------------------------
      
      
      nBins = int( (xMax - xMin)/xWidth );
      xMax = xMin + nBins*xWidth;
      binMin = 1;
      binMax = nBins;
      
      if( toyIt == 0 )
      {
        std::cout << ">>>>>>>>> nBins:  " << nBins  << std::endl;
        std::cout << ">>>>>>>>> xMin:   " << xMin   << std::endl;
        std::cout << ">>>>>>>>> xMax:   " << xMax   << std::endl;
        std::cout << ">>>>>>>>> xWidth: " << xWidth << std::endl;
      }
      
      
      
      
      
      //--------------------------
      // data and background shape
      
      // data
      if( (analysisMethod == "fit") || (analysisMethod == "sidebands") )
        data_obs = new TH1F("data_obs","",nBins,xMin,xMax);
      if( (analysisMethod == "fitNoHoles") || (analysisMethod == "fakeNoHoles") )
        data_obs = new TH1F("data_obs_dummy","",nBins,xMin,xMax);
      data_obs -> Sumw2();
      
      if( toyIt == 0 )
      {
        for(int bin = 1; bin <= data->GetNbinsX(); ++bin)
        {
          float binCenter  = data -> GetBinCenter(bin);
          float binContent = data -> GetBinContent(bin);
          if( (binCenter >= xMin) && (binCenter < xMax) )
            for(int entry = 0; entry < binContent; ++entry)
              data_obs -> Fill(binCenter);
        }
        n_data_obs = data_obs -> Integral(binMin,binMax);
      }
      
      else
      {
        std::stringstream funcName;
        funcName.str(std::string());
        funcName << "H" << mass << "/fitFunc_" << mass << "_" << flavour << charge << "_" << toyParent;
        
        TF1* data_fit = (TF1*)( inFile->Get(funcName.str().c_str()) );
        data_fit -> SetName("data_fit");
        
        data_obs -> Reset();
        data_obs -> FillRandom("data_fit",int(n_data_obs));
      }
      
      
      //------------------------------------------------------------------------
      
      
      if( (analysisMethod == "fit") || (analysisMethod == "sidebands") )
      {
        // background
        bkg            = new TH1F("bkg",           "",nBins,xMin,xMax);
        bkg_fitErrUp   = new TH1F(("bkg_CMS_HWWlvjj_"+flavour+charge+"_bkgSystUp").c_str(),  "",nBins,xMin,xMax);
        bkg_fitErrDown = new TH1F(("bkg_CMS_HWWlvjj_"+flavour+charge+"_bkgSystDown").c_str(),"",nBins,xMin,xMax);
        
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
        
        if( toyIt == 0 )
          outFile -> cd();
        else
          outFile_toy -> cd();
        
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
      //labels.push_back("qqH");
      
      std::vector<std::string> labels_syst;
      labels_syst.push_back("JESUp");
      labels_syst.push_back("JESDown");
      labels_syst.push_back("PUUp");
      labels_syst.push_back("PUDown");
      //labels_syst.push_back("LepEffUp");
      //labels_syst.push_back("LepEffDown");
      //labels_syst.push_back("lepScaleUp");
      //labels_syst.push_back("lepScaleDown");
      //if( flavour == "e" )
      //{
      //  labels_syst.push_back("MtEffUp");
      //  labels_syst.push_back("MtEffDown");
      //}
      
      
      for(unsigned int labelIt = 0; labelIt < 1 + 1*labels_syst.size(); ++labelIt)
      {

        std::string label;
        if(labelIt < 1) label = labels.at(labelIt);
        else            label = labels.at(0) + "_CMS_HWWlvjj_" + labels_syst.at((labelIt-1)/1);

        if( toyIt == 0 )
        {
          fileName.str(std::string());
          if(labelIt < 1) fileName << inputDir << "/countSignalEvents.root";
          else            fileName << inputDir << "/countSignalEvents_" + labels_syst.at((labelIt-1)) + ".root";
          inFile2 = TFile::Open(fileName.str().c_str(),"READ");
          //std::cout << ">>> opened file " << fileName.str() << std::endl; 
          //std::cout << ">>>>>> label: " << label << std::endl;
          
    
          histo = (TH1F*)( inFile2->Get(("H"+massString+"/h_ggH"+massString+"_"+flavour+charge).c_str()) );
          //if( labelIt%2 == 1 ) histo = (TH1F*)( inFile2->Get(("H"+massString+"/h_qqH"+massString+"_"+flavour+charge).c_str()) );
          
          histo_fit = (TH1F*)( inFile2->Get(("H"+massString+"/h_ggH"+massString+"_"+flavour+"_fit").c_str()) );
          //if( labelIt%2 == 1 ) histo_fit = (TH1F*)( inFile2->Get(("H"+massString+"/h_qqH"+massString+"_"+flavour+"_fit").c_str()) );
          
          TF1* fitFunc = histo_fit->GetFunction("fitFunc_scb");
          
          
          if( toyIt == 0 )
            outFile -> cd();
          else
            outFile_toy -> cd();
          
          
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
        }
        
        
        if( (analysisMethod == "fit") || (analysisMethod == "sidebands") )
        {
          H_fit[label] -> Write();         
        }

        if( (analysisMethod == "fitNoHoles") || (analysisMethod == "fakeNoHoles") )
        {
          if( toyIt == 0 )
          {
            TF1* fitFunc = histo_fit->GetFunction("fitFunc_scb");
            
            
            // use smooth histograms for the signal shapes
            dh_H[label] = new RooDataHist(("dh_"+label).c_str(),"",RooArgList(*x),H_fit[label]);
            pdf_H[label] = new RooHistPdf(label.c_str(),"",RooArgList(*x),*dh_H[label],2);
            workspace -> import(*pdf_H[label]);
                        
            /*
            // use a parametric function for the signal shapes
            double scb_muDouble     = 175.;
            double scb_kTDouble     = GetHiggsMassTurnOnWidth(mass);
            double scb_meanDouble   = fitFunc -> GetParameter(3);
            double scb_sigmaDouble  = fitFunc -> GetParameter(4);
            double scb_alphaDouble  = fitFunc -> GetParameter(5);
            double scb_nDouble      = fitFunc -> GetParameter(6);
            double scb_alpha2Double = fitFunc -> GetParameter(7);
            double scb_n2Double     = fitFunc -> GetParameter(8);
            
            scb_mu[label]     = new RooRealVar(("CMS_HWWlvjj_"+flavour+charge+"_scb_mu_"+label).c_str(),    "",scb_muDouble,scb_muDouble,scb_muDouble);
            scb_kT[label]     = new RooRealVar(("CMS_HWWlvjj_"+flavour+charge+"_scb_kT_"+label).c_str(),    "",scb_kTDouble,scb_kTDouble,scb_kTDouble);
            scb_mean[label]   = new RooRealVar(("CMS_HWWlvjj_"+flavour+charge+"_scb_mean_"+label).c_str(),  "",scb_meanDouble,scb_meanDouble,scb_meanDouble);
            scb_sigma[label]  = new RooRealVar(("CMS_HWWlvjj_"+flavour+charge+"_scb_sigma_"+label).c_str(), "",scb_sigmaDouble,scb_sigmaDouble,scb_sigmaDouble);
            scb_alpha[label]  = new RooRealVar(("CMS_HWWlvjj_"+flavour+charge+"_scb_alpha_"+label).c_str(), "",scb_alphaDouble,scb_alphaDouble,scb_alphaDouble);
            scb_n[label]      = new RooRealVar(("CMS_HWWlvjj_"+flavour+charge+"_scb_n_"+label).c_str(),     "",scb_nDouble,scb_nDouble,scb_nDouble);
            scb_alpha2[label] = new RooRealVar(("CMS_HWWlvjj_"+flavour+charge+"_scb_alpha2_"+label).c_str(),"",scb_alpha2Double,scb_alpha2Double,scb_alpha2Double);
            scb_n2[label]     = new RooRealVar(("CMS_HWWlvjj_"+flavour+charge+"_scb_n2_"+label).c_str(),    "",scb_n2Double,scb_n2Double,scb_n2Double);
            
            pdf_scb[label] = new RooAttenuatedCrystalBallLowHigh(label.c_str(),"",*x,
                                                                                  *(scb_mu[label]),
                                                                                  *(scb_kT[label]),
                                                                                  *(scb_mean[label]),
                                                                                  *(scb_sigma[label]),
                                                                                  *(scb_alpha[label]),
                                                                                  *(scb_n[label]),
                                                                                  *(scb_alpha2[label]),
                                                                                  *(scb_n2[label]));
            
            rooN_H[label] = new RooRealVar(("rooN_"+label).c_str(),"",n_H[label],n_H[label],n_H[label]);
            */
          }
          
          /*
          workspace -> import(*(scb_mu[label]));
          workspace -> import(*(scb_kT[label]));
          workspace -> import(*(scb_mean[label]));
          workspace -> import(*(scb_sigma[label]));
          workspace -> import(*(scb_alpha[label]));
          workspace -> import(*(scb_n[label]));
          workspace -> import(*(scb_alpha2[label]));
          workspace -> import(*(scb_n2[label]));
          workspace -> import(*(pdf_scb[label]));
          
          workspace -> import(*rooN_H[label]);
          */
        }
        
      }
      

      
      if( (analysisMethod == "fitNoHoles") || (analysisMethod == "fakeNoHoles") )
      {
        if( toyIt == 0 )
          outFile -> cd();
        else
          outFile_toy -> cd();
        
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
      //labels_sa_num.push_back("LepEffUp");
      //labels_sa_num.push_back("LepEffDown");
      //if( flavour == "e" )
      //{
      //  labels_sa_num.push_back("MtEffUp");
      //  labels_sa_num.push_back("MtEffDown");
      //}
      //labels_sa_num.push_back("lepScaleUp");
      //labels_sa_num.push_back("lepScaleDown");
      
      std::vector<std::string> labels_sa_num_names;
      labels_sa_num_names.push_back("CMS_HWWlvjj_PU      ");
      //if( flavour == "e" )
      //{
      //  labels_sa_num_names.push_back("CMS_HWWlvjj_e_lepEff");
      //  labels_sa_num_names.push_back("CMS_HWWlvjj_e_mtEff ");
      //  labels_sa_num_names.push_back("CMS_HWWlvjj_e_lepScale");
      //}
      //else
      //{
      //  labels_sa_num_names.push_back("CMS_HWWlvjj_mu_lepEff");
      //  labels_sa_num_names.push_back("CMS_HWWlvjj_mu_lepScale");
      //}
      
      std::vector<std::string> labels_sa_shape;
      labels_sa_shape.push_back("JESUp");
      labels_sa_shape.push_back("JESDown");
      std::vector<std::string> labels_sa_shape_names;
      labels_sa_shape_names.push_back("CMS_HWWlvjj_JES    ");
      
      std::stringstream name;
      
      
      if( ( (analysisMethod == "sidebands") || (analysisMethod == "fitNoHoles") || (analysisMethod == "fakeNoHoles") ) &&
          ( toyIt < 2 ) )
      {
        std::stringstream ss3;
        
        if( toyIt == 0 )
        {
          if( analysisMethod == "sidebands" )
            ss3 << outputRootFilePath << "/" << "datacard" << "_" << analysisMethod << "_"<< "shapeanalysis" << "_" << mass << "_" << flavour << charge << ".txt";
          else
            ss3 << outputRootFilePath << "/" << "datacard" << "_" << analysisMethod << "_" << fitMethod << "_"<< "shapeanalysis" << "_" << mass << "_" << flavour << charge << ".txt";
        }
        else
        {
          if( analysisMethod == "sidebands" )
            ss3 << outputRootFilePath << "/" << "datacard" << "_" << analysisMethod << "_"<< "shapeanalysis" << "_" << mass << "_" << flavour << charge << "_" << toyParent << "_toy.txt";
          else
            ss3 << outputRootFilePath << "/" << "datacard" << "_" << analysisMethod << "_" << fitMethod << "_"<< "shapeanalysis" << "_" << mass << "_" << flavour << charge << "_" << toyParent << "_toy.txt";
        }

        
        std::ofstream datacard_sa(ss3.str().c_str(),std::ios::out);
        
        datacard_sa << std::fixed;
        datacard_sa << std::left;
        
        datacard_sa << "********* H > WW > lvjj analysis for mH = " << mass << " GeV/c^2 *********" << std::endl;
        datacard_sa << "********* parametric shape analysis *********" << std::endl;
        
        datacard_sa << "-----------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
        datacard_sa << "imax 1   # number of channels" << std::endl;
        datacard_sa << "jmax 1   # number of processes - 1" << std::endl;
        datacard_sa << "kmax *   # number of nuisance parameters (sources of systematic uncertainties)" << std::endl;
        
        datacard_sa << "-----------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
        if( analysisMethod == "sidebands" )
        {
          datacard_sa << "shapes *          " << setw(4) << std::string(flavour+charge) << "   "
                      << "shapes_" << analysisMethod << "_" << mass << "_" << flavour << charge << ".root   "
                      << "$PROCESS $PROCESS_$SYSTEMATIC" << std::endl;
          datacard_sa << "shapes data_obs   " << setw(4) << std::string(flavour+charge) << "   "
                      << "shapes_" << analysisMethod << "_" << mass << "_" << flavour << charge << ".root   "
                      << "data_obs" << std::endl;
        }
        else
        {
          if( toyIt == 0 )
          {
            datacard_sa << "shapes *          " << setw(4) << std::string(flavour+charge) << "   "
                        << "shapes_" << analysisMethod << "_" << fitMethod << "_" << mass << "_" << flavour << charge << ".root   "
                        << "workspace:$PROCESS workspace:$PROCESS_$SYSTEMATIC" << std::endl;
            datacard_sa << "shapes data_obs   " << setw(4) << std::string(flavour+charge) << "   "
                        << "shapes_" << analysisMethod << "_" << fitMethod << "_" << mass << "_" << flavour << charge << ".root   "
                        << "workspace:data_obs" << std::endl;
          }
          else
          {
            datacard_sa << "shapes *          " << setw(4) << std::string(flavour+charge) << "   "
                        << "shapes_" << analysisMethod << "_" << fitMethod << "_" << mass << "_" << flavour << charge << "_" << toyParent << "_toy.root   "
                        << "workspace_TOYPARENT_toyTOYIT:$PROCESS workspace_TOYPARENT_toyTOYIT:$PROCESS_$SYSTEMATIC" << std::endl;
            datacard_sa << "shapes data_obs   " << setw(4) << std::string(flavour+charge) << "   "
                        << "shapes_" << analysisMethod << "_" << fitMethod << "_" << mass << "_" << flavour << charge << "_" << toyParent << "_toy.root   "
                        << "workspace_TOYPARENT_toyTOYIT:data_obs" << std::endl;
          }
        }
        datacard_sa << "-----------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
        datacard_sa << setw(33) << "bin" << "   "
                    << setw(8) << std::string(flavour+charge) << std::endl;
        datacard_sa << setw(33) << "observation" << "   "
                    << std::setprecision(0) << setw(8) << n_data_obs << std::endl;    
        
        datacard_sa << "-----------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
        datacard_sa << setw(33) << "bin" << "   "
                    << setw(8) << std::string(flavour+charge) << "   "
                    << setw(8) << std::string(flavour+charge) << std::endl;
        
        datacard_sa << setw(33) << "process" << "   "
                    << setw(8) << "ggH" << "   "
          //          << setw(8) << "qqH" << "   "
                    << setw(8) << "bkg" << std::endl;
        
        datacard_sa << setw(33) << "process" << "   "
                    << setw(8) << "-2" << "   "
          //          << setw(8) << "-1" << "   "
                    << setw(8) << "1" << std::endl;
        
        if( analysisMethod == "sidebands")
        {
          datacard_sa << setw(33) << "rate" << "   "
                      << std::setprecision(2) << setw(8) << n_H["ggH"] << "   "
          //            << std::setprecision(2) << setw(8) << n_H["qqH"] << "   "
                      << std::setprecision(2) << setw(8) << n_bkg << std::endl;
        }
        else
        {
          datacard_sa << setw(33) << "rate" << "   "
                      << std::setprecision(2) << setw(8) << n_H["ggH"] << "   "
          //            << std::setprecision(2) << setw(8) << n_H["qqH"] << "   "
                      << std::setprecision(2) << setw(8) << n_data_obs << std::endl;
        }
        
        datacard_sa  << "-----------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
        
        
        datacard_sa << std::setprecision(3);
        
        if( sigSyst == 1 )
        {
          datacard_sa << setw(25) << "lumi"     << "   " << setw(5) << "lnN" << "   "
                       << setw(8) << "1.022" << "   "
          //            << setw(8) << "1.022" << "   "
                      << setw(8) << "-" << std::endl;
          
          datacard_sa << setw(25) << "QCDscale_ggH" << "   " << setw(5) << "lnN" << "   "
                      << setw(8) << 1.+0.5*(HiggsQCDScaleSyst(mass,"gg","up")+HiggsQCDScaleSyst(mass,"gg","down")) << "   "
          //            << setw(8) << "-" << "   " 
                      << setw(8) << "-" << std::endl;
          
          datacard_sa << setw(25) << "QCDscale_qqH" << "   " << setw(5) << "lnN" << "   "
                      << setw(8) << "-" << "   "
          //            << setw(8) << 1.+0.5*(HiggsQCDScaleSyst(mass,"qq","up")+HiggsQCDScaleSyst(mass,"qq","down")) << "   "
                      << setw(8) << "-" << std::endl;
          
          datacard_sa << setw(25) << "pdf_gg" << "   " << setw(5) << "lnN" << "   "
                      << setw(8) << 1.+0.5*(HiggsPDFSyst(mass,"gg","up")+HiggsPDFSyst(mass,"gg","down")) << "   "
          //            << setw(8) << "-" << "   "
                      << setw(8) << "-" << std::endl;
          
          datacard_sa << setw(25) << "pdf_qqbar" << "   " << setw(5) << "lnN" << "   "
                      << setw(8) << "-" << "   "
          //            << setw(8) << 1.+0.5*(HiggsPDFSyst(mass,"qq","up")+HiggsPDFSyst(mass,"qq","down")) << "   "
                      << setw(8) << "-" << std::endl;
          
          datacard_sa << setw(25) << "theoryUncXS_HighMH" << "   " << setw(5) << "lnN" << "   "
                      << setw(8) << 1.+0.5*(HiggsHighMassSyst(mass,"gg","up")+HiggsHighMassSyst(mass,"gg","down")) << "   "
          //            << setw(8) << "-" << "   "
                      << setw(8) << "-" << std::endl;
          
          for(unsigned int labelIt = 0; labelIt < labels_sa_num_names.size(); ++labelIt)
          {
            double ggH_errUp   = fabs(n_H["ggH_CMS_HWWlvjj_"+labels_sa_num.at(2*labelIt)]   - n_H["ggH"]);
            //double qqH_errUp   = fabs(n_H["qqH_CMS_HWWlvjj_"+labels_sa_num.at(2*labelIt)]   - n_H["qqH"]);
            double ggH_errDown = fabs(n_H["ggH_CMS_HWWlvjj_"+labels_sa_num.at(2*labelIt+1)] - n_H["ggH"]);
            //double qqH_errDown = fabs(n_H["qqH_CMS_HWWlvjj_"+labels_sa_num.at(2*labelIt+1)] - n_H["qqH"]);
            
            double ggH_errAve  = 0.5*(ggH_errUp + ggH_errDown);
            //double qqH_errAve  = 0.5*(qqH_errUp + qqH_errDown);

            datacard_sa << setw(25) << labels_sa_num_names.at(labelIt) << "   " << setw(5) << "lnN" << "   "
                        << setw(8) << 1. + ggH_errAve/n_H["ggH"] << "   "
            //            << setw(8) << 1. + qqH_errAve/n_H["qqH"] << "   "
                        << setw(8) << "-" << std::endl;
          }
          for(unsigned int labelIt = 0; labelIt < labels_sa_shape_names.size(); ++labelIt)
          {
            datacard_sa << setw(25) << labels_sa_shape_names.at(labelIt) << "   " << setw(5) << "shape" << "   "
                        << setw(8) << "1." << "   "
            //            << setw(8) << "1." << "   "
                        << setw(8) << "-" << std::endl;
          }
          
	  name.str(std::string()); name << "CMS_HWWlvjj_" << flavour << "_lepHLT";
          if( flavour == "e" )
            datacard_sa << setw(25) << name.str() << "   " << setw(5) << "lnN" << "   "
                        << setw(8) << "1.003" << "   "
            //            << setw(8) << "1.003" << "   "
                        << setw(8) << "-" << std::endl;
          if( flavour == "mu" )
            datacard_sa << setw(25) << name.str() << "   " << setw(5) << "lnN" << "   " << setw(8)
                        << "1.010" << "   "
            //            << setw(8) << "1.010" << "   "
                        << setw(8) << "-" << std::endl;
          
          name.str(std::string()); name << "CMS_HWWlvjj_" << flavour << "_mTHLT";
          if( flavour == "e" )
            datacard_sa << setw(25) << name.str() << "   " << setw(5) << "lnN" << "   "
                        << setw(8) << "1.014" << "   "
            //            << setw(8) << "1.014" << "   "
                        << setw(8) << "-" << std::endl;
          
          name.str(std::string()); name << "CMS_HWWlvjj_" << flavour << "_lep";
          if( flavour == "e" )
            datacard_sa << setw(25) << name.str() << "   " << setw(5) << "lnN" << "   "
                        << setw(8) << "1.022" << "   "
            //            << setw(8) << "1.022" << "   "
                        << setw(8) << "-" << std::endl;
          if( flavour == "mu" )
            datacard_sa << setw(25) << name.str() << "   " << setw(5) << "lnN" << "   "
                        << setw(8) << "1.008" << "   "
            //            << setw(8) << "1.008" << "   "
                        << setw(8) << "-" << std::endl;
          
          name.str(std::string()); name << "CMS_HWWlvjj_bTag";
          datacard_sa << setw(25) << name.str() << "   " << setw(5) << "lnN" << "   "
                      << setw(8) << "1.010" << "   "
            //          << setw(8) << "1.010" << "   "
                      << setw(8) << "-" << std::endl;
        }
        
        
        
        if( (analysisMethod == "fitNoHoles") && (bkgSyst == 1) )
        {
          name.str(std::string()); name << "CMS_HWWlvjj_" << flavour << charge << "_bias";
          datacard_sa << setw(25) << name.str() << "   " << setw(5) << "lnN" << "   "
                      << setw(8) << 1.+FitBiasSyst(mass,flavour) << "   "
          //            << setw(8) << 1.+FitBiasSyst(mass,flavour) << "   "
                      << setw(8) << "-" << std::endl;
        }
        
        if( (analysisMethod == "sidebands") && (bkgSyst == 1) )
        {
          name.str(std::string()); name << "CMS_HWWlvjj_" << flavour << charge << "_bkgSyst";
          datacard_sa << setw(25) << name.str() << "   " << setw(5) << "shape" << "   "
                      << setw(8) << "-" << "   "
          //            << setw(8) << "-" << "   "
                      << setw(8) << "1." << std::endl;
        }
        
        
        
        if( analysisMethod == "fitNoHoles" )
        {
        /*
          float err;
          if ( flavour == "e" ) err = 1.3;
          else err = 1.2;
          name.str(std::string()); name << "CMS_HWWlvjj_" << flavour << charge << "_bkgNorm";
          datacard_sa << setw(25) << name.str() << "   " << setw(5) << "lnU"<< "   "
                      << setw(8) << "-" << "   "
                      << setw(8) << "-" << "   "
                      << setw(8) << err << std::endl;*/
          
          datacard_sa << "-----------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
          
          int parItMin = 0;
          if( blockTurnOn == 1 ) parItMin = 2;
          if( blockParams == 1 ) parItMin = nPars;
          
          int parItMax = nPars;
          if( (blockTurnOn == 0) && (blockParams == 1) ) parItMax = 2; 
          
          datacard_sa << setw(25) << norm->GetName() << "   flatParam   " << norm->getMin() << " " << norm->getMax() << std::endl;
          
          for(int parIt = parItMin; parIt < parItMax; ++parIt) 
            datacard_sa << setw(25) << parNames[parIt] << "   flatParam   " << pars[parIt]->getMin() << " " << pars[parIt]->getMax() << std::endl;
        }
      }
      
      
      if( data_obs != NULL )    delete data_obs;
      if( dh_data_obs != NULL ) delete dh_data_obs;
      
      inFile  -> Close();
      inFile2 -> Close();
    }
    
    outFile -> Close();
    if( nToys > 0 )
      outFile_toy -> Close();
  }
  
  
  
  return 1;
}






float GetXFitMIN1(const float& mH, const int& step, const std::string& additionalCuts)
{
  if( (step <= 13) && ( (additionalCuts == "none") || (additionalCuts == "angles") ) )
  {
    /*
    if     ( mH == 250. ) return 185.;
    else if( mH == 300. ) return 205.;
    else if( mH == 350. ) return 220.;
    else if( mH == 400. ) return 230.;
    else if( mH == 450. ) return 240.;
    else if( mH == 500. ) return 240.;
    else if( mH == 550. ) return 240.;
    else if( mH == 600. ) return 235.;
    else return 1.;*/
    
    if     ( mH == 200. ) return 230.;
    if     ( mH == 250. ) return 230.;
    else if( mH == 300. ) return 230.;
    else if( mH == 350. ) return 230.;
    else if( mH == 400. ) return 230.;
    else if( mH == 450. ) return 230.;
    else if( mH == 500. ) return 230.;
    else if( mH == 550. ) return 230.;
    else if( mH == 600. ) return 230.;
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
  
  else return -1.;
}



float GetXFitMAX2(const float& mH)
{
  /*
  if     ( mH == 250. ) return 495.;
  else if( mH == 300. ) return 490.;
  else if( mH == 350. ) return 515.;
  else if( mH == 400. ) return 605.;
  else if( mH == 450. ) return 680.;
  else if( mH == 500. ) return 725.;
  else if( mH == 550. ) return 760.;
  else if( mH == 600. ) return 785.;
  else return -1.;
  */
  
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
