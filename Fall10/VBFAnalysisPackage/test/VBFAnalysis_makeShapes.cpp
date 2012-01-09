#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "HiggsMassWindows.h"
#include "HiggsCrossSectionSyst.h"

#include <iostream>
#include <iomanip>
#include <sstream>

#include "TH1F.h"
#include "TF1.h"

#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooGenericPdf.h"
#include "RooWorkspace.h"





int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> VBFAnalysis_makeShapes::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
  
  
  //[Input]
  std::string baseDir = gConfigParser -> readStringOption("Input::baseDir");
  std::string fitMethod      = gConfigParser -> readStringOption("Input::fitMethod");
  std::string analysisMethod = gConfigParser -> readStringOption("Input::analysisMethod");
  
  //[Output]
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  
  
  
  std::vector<int> masses = GetMasses();
  unsigned int nMasses = masses.size();
  
  for(unsigned int iMass = 0; iMass < nMasses; ++iMass)
  {
    int mass = masses[iMass];
    std::cout << ">>> mass: " << mass << std::endl;
    float lepNuWMMIN = GetLepNuWMMIN(mass);
    float lepNuWMMAX = GetLepNuWMMAX(mass);    
    
    
    // define outfile
    std::stringstream ss;
    if( (analysisMethod == "fit") || (analysisMethod == "fitNoHoles") )
      ss << outputRootFilePath << "/" << outputRootFileName << "_" << analysisMethod << "_" << fitMethod << "_" << mass << ".root";
    else
      ss << outputRootFilePath << "/" << outputRootFileName << "_" << analysisMethod << "_" << mass << ".root";
    TFile* outFile = new TFile((ss.str()).c_str(),"RECREATE");
    
    
    // define infile
    std::stringstream fileName;
    TFile* inFile;
    
    float xWidth;
    float xMin;
    float xMax;
    int nBins;
    int binMin;
    int binMax;
    int binMinWindow;
    int binMaxWindow;
    
    TH1F* data;
    TH1F* data_obs;
    double n_data_obs;
    double nWindow_data_obs;
    
    TH1F* hint;
    TH1F* bkg;
    TH1F* bkg_fitErrUp;
    TH1F* bkg_fitErrDown;
    double n_bkg;
    double nWindow_bkg;
    double nWindow_bkg_err;
        
    TH1F* histo;
    TH1F* ggH;
    TH1F* qqH;
    RooDataHist* dh_ggH;
    RooDataHist* dh_qqH;
    RooHistPdf* pdf_ggH;
    RooHistPdf* pdf_qqH;
    std::map<std::string,TH1F*> ggH_syst;
    std::map<std::string,TH1F*> qqH_syst;
    std::map<std::string,RooDataHist*> dh_ggH_syst;
    std::map<std::string,RooDataHist*> dh_qqH_syst;
    std::map<std::string,RooHistPdf*> pdf_ggH_syst;
    std::map<std::string,RooHistPdf*> pdf_qqH_syst;
    double n_ggH;
    double n_qqH;
    double nWindow_ggH;
    double nWindow_qqH;
    std::map<std::string,double> n_ggH_syst;
    std::map<std::string,double> n_qqH_syst;
    std::map<std::string,double> nWindow_ggH_syst;
    std::map<std::string,double> nWindow_qqH_syst;
    
    RooWorkspace* workspace;
    RooRealVar* x;
    RooRealVar* mu;
    RooRealVar* kT;
    RooRealVar* N;
    RooRealVar* L1;
    RooRealVar* L2;
    RooGenericPdf* pdf_bkg;
    RooDataHist* dh_data_obs; 
    
    std::stringstream histoName_gg;
    histoName_gg << "h_ggH" << mass;
    std::stringstream histoName_qq;
    histoName_qq << "h_qqH" << mass;
    
    
    
    //------------------
    // data & bkgshapes
    
    fileName.str(std::string());
    if( (analysisMethod == "fit") || (analysisMethod == "fitNoHoles") )
      fileName << baseDir << "/fitHiggsMassBinned_" << fitMethod << "_PFlow_H" << mass << ".root";
    if( analysisMethod == "sidebands" )
      fileName << baseDir << "/output_015.root";
    inFile = TFile::Open(fileName.str().c_str(),"READ");
    std::cout << ">>> opened file " << fileName.str() << std::endl; 
    
    inFile -> cd();
    
    
    if( analysisMethod == "fit" )
    {
      data = (TH1F*)( inFile->Get("data/h_lepNuW_m") );
      hint = (TH1F*)( inFile->Get("data/hint") );
      
      xWidth = data -> GetBinWidth(1);
      if( (fitMethod == "doubleExponential") || (fitMethod == "attenuatedDoubleExponential") )
      {
        xMin = GetLepNuWMMIN(mass);
        xMax = GetLepNuWMMAX(mass);
      }
      if( (fitMethod == "doubleExponentialNoHoles") || (fitMethod == "attenuatedDoubleExponentialNoHoles") )
      {
        xMin = GetXFitMIN1(mass,fitMethod);
        xMax = GetXFitMAX2(mass,fitMethod);
      }
      nBins = int( (xMax - xMin)/xWidth );
      binMin = 1;
      binMax = nBins;
      binMinWindow = 1;
      binMaxWindow = nBins;
    }
    
    if( analysisMethod == "fitNoHoles" ) 
    {
      data = (TH1F*)( inFile->Get("data/h_lepNuW_m") );
      
      xWidth = data -> GetBinWidth(1);
      xMin = GetXFitMIN1(mass,fitMethod);
      xMax = GetXFitMAX2(mass,fitMethod);
      nBins = int( (xMax - xMin)/xWidth );
      binMin = 1;
      binMax = nBins;
      binMinWindow = 1;
      binMaxWindow = nBins;
      
      workspace = new RooWorkspace("workspace");
      
      x = new RooRealVar("x","lepNuW_m_KF",0.,1000.);
      x -> setRange(xMin,xMax);
      workspace -> import(*x);
      
      if( fitMethod == "doubleExponential" )
      {
        N  = new RooRealVar("N", "N", 0.007,0.,1.0);
        L1 = new RooRealVar("L1","L1",0.015,0.,0.1);
        L2 = new RooRealVar("L2","L2",0.005,0.,0.1);
        pdf_bkg = new RooGenericPdf("bkg","","(exp(-1*@2*@0) + @1*exp(-1*@3*@0))",RooArgSet(*x,*N,*L1,*L2));
        
        workspace -> import(*N);
        workspace -> import(*L1);
        workspace -> import(*L2);
        workspace -> import(*pdf_bkg);
      }
      if( fitMethod == "attenuatedDoubleExponential" )
      {
        mu = new RooRealVar("mu","mu",160.,0.,500.); 
        kT = new RooRealVar("kT","kT", 20.,0.,100.); 
        N  = new RooRealVar("N", "N", 0.007,0.,1.0);
        L1 = new RooRealVar("L1","L1",0.015,0.,0.1);
        L2 = new RooRealVar("L2","L2",0.005,0.,0.1);
        pdf_bkg = new RooGenericPdf("bkg","","1./(exp(-1.*(@0-@1)/@2)+1.) * (exp(-1*@4*@0) + @3*exp(-1*@5*@0))",RooArgSet(*x,*mu,*kT,*N,*L1,*L2));
        
        workspace -> import(*mu);
        workspace -> import(*kT);
        workspace -> import(*N);
        workspace -> import(*L1);
        workspace -> import(*L2);
        workspace -> import(*pdf_bkg);
      }
    }
    
    if( analysisMethod == "sidebands" )
    {
      data = (TH1F*)(inFile->Get("signalRegion"));    
      hint = (TH1F*)(inFile->Get("extrapolated_bkg"));
      
      xWidth = data -> GetBinWidth(1);
      xMin = GetXFitMIN1(mass);
      xMax = GetXFitMAX2(mass);
      nBins = int( (xMax - xMin)/xWidth );
      binMin = 1;
      binMax = nBins;
      binMinWindow = -1;
      binMaxWindow = -1;
      for(int localBin = 1; localBin <= nBins; ++localBin)
      {
        float localBinCenter = xMin + 0.5*xWidth + (localBin-1)*xWidth;
	if( (localBinCenter >= lepNuWMMIN) && (binMinWindow == -1) ) binMinWindow = localBin;
	if( (localBinCenter >= lepNuWMMIN) && (localBinCenter < lepNuWMMAX) ) binMaxWindow = localBin;
      }
    }
    
    
    
    if( (analysisMethod == "fit") || (analysisMethod == "sidebands") )
    {
      // data
      data_obs = new TH1F("data_obs","",nBins,xMin,xMax);
      data_obs -> Sumw2();
      
      for(int bin = 1; bin <= data->GetNbinsX(); ++bin)
      {
        float binCenter  = data -> GetBinCenter(bin);
        float binContent = data -> GetBinContent(bin);
        if( (binCenter >= xMin) && (binCenter < xMax) )
          for(int entry = 0; entry < binContent; ++entry)
            data_obs -> Fill(binCenter);
      }
      
      // background
      bkg            = new TH1F("bkg",           "",nBins,xMin,xMax);
      bkg_fitErrUp   = new TH1F("bkg_fitErrUp",  "",nBins,xMin,xMax);
      bkg_fitErrDown = new TH1F("bkg_fitErrDown","",nBins,xMin,xMax);
      
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
      
      n_data_obs = data_obs -> Integral(binMin,binMax);
      nWindow_data_obs = data_obs -> Integral(binMinWindow,binMaxWindow);
      
      n_bkg = bkg -> Integral(binMin,binMax);
      nWindow_bkg = bkg -> Integral(binMinWindow,binMaxWindow);
      nWindow_bkg_err = bkg_fitErrUp -> Integral(binMinWindow,binMaxWindow) - nWindow_bkg;
    }    
    
    
    if( analysisMethod == "fitNoHoles" )
    {
      // data
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
      
      dh_data_obs = new RooDataHist("data_obs","",RooArgList(*x),data_obs);
      workspace -> import(*dh_data_obs);
      
      n_data_obs = data_obs -> Integral(binMin,binMax);
      nWindow_data_obs = data_obs -> Integral(binMinWindow,binMaxWindow);
    }
    
    
    
    
    
    
    //--------------
    // signal shapes
    
    fileName.str(std::string());
    fileName << baseDir << "/countSignalEvents_PFlow.root";
    inFile = TFile::Open(fileName.str().c_str(),"READ");
    std::cout << ">>> opened file " << fileName.str() << std::endl; 
    
    
    // gg Higgs
    inFile -> cd();
    histo = (TH1F*)(inFile->Get(histoName_gg.str().c_str()));
    outFile -> cd();
    
    if( (analysisMethod == "fit") || (analysisMethod == "sidebands") )
      ggH = new TH1F("ggH","",nBins,xMin,xMax);
    if( analysisMethod == "fitNoHoles" )
      ggH = new TH1F("ggH_dummy","",nBins,xMin,xMax);
    
    for(int bin = 1; bin <= histo->GetNbinsX(); ++bin)
    {
      float binCenter  = histo -> GetBinCenter(bin);
      float binContent = histo -> GetBinContent(bin);
      if( (binCenter >= xMin) && (binCenter < xMax) )
      {
        ggH -> Fill(binCenter,binContent);
      }
    }
    
    if( (analysisMethod == "fit") || (analysisMethod == "sidebands") )
      ggH -> Write();
    if( analysisMethod == "fitNoHoles" )
    {
      dh_ggH = new RooDataHist("dh_ggH","",RooArgList(*x),ggH);
      pdf_ggH = new RooHistPdf("ggH","",RooArgList(*x),*dh_ggH,2);
      workspace -> import(*pdf_ggH);
    }
    
    n_ggH = ggH -> Integral(binMin,binMax);
    nWindow_ggH = ggH -> Integral(binMinWindow,binMaxWindow);
    
    
    // qq Higgs    
    inFile -> cd();
    histo = (TH1F*)(inFile->Get(histoName_qq.str().c_str()));
    outFile -> cd();
    if( (analysisMethod == "fit") || (analysisMethod == "sidebands") )
      qqH = new TH1F("qqH","",nBins,xMin,xMax);
    if( analysisMethod == "fitNoHoles" )
      qqH = new TH1F("qqH_dummy","",nBins,xMin,xMax);
    
    for(int bin = 1; bin <= histo->GetNbinsX(); ++bin)
    {
      float binCenter  = histo -> GetBinCenter(bin);
      float binContent = histo -> GetBinContent(bin);
      if( (binCenter >= xMin) && (binCenter < xMax) )
      {
        qqH -> Fill(binCenter,binContent);
      }
    }

    if( (analysisMethod == "fit") || (analysisMethod == "sidebands") )
      qqH -> Write();
    if( analysisMethod == "fitNoHoles" )
    {
      dh_qqH = new RooDataHist("dh_qqH","",RooArgList(*x),qqH);
      pdf_qqH = new RooHistPdf("qqH","",RooArgList(*x),*dh_qqH,2);
      workspace -> import(*pdf_qqH);
    }    
    
    n_qqH = qqH -> Integral(binMin,binMax);
    nWindow_qqH = qqH -> Integral(binMinWindow,binMaxWindow);
    
    
    
    
    
    
    //-------------------------
    // systematic signal shapes
    
    std::vector<std::string> labels;
    labels.push_back("PU");
    labels.push_back("JES");
    std::vector<std::string> labels2;
    labels2.push_back("PU     ");
    labels2.push_back("JES    ");
    
    for(unsigned int labelIt = 0; labelIt < labels.size(); ++labelIt)
    {
      std::string label = labels.at(labelIt)+"Up";
      
      fileName.str(std::string());
      fileName << baseDir << "/countSignalEvents_" + label + "_PFlow.root";
      inFile = TFile::Open(fileName.str().c_str(),"READ");
      std::cout << ">>> opened file " << fileName.str() << std::endl;
      
      
      // gg Higgs
      inFile -> cd();
      histo = (TH1F*)(inFile->Get(histoName_gg.str().c_str()));
      outFile -> cd();
      if( (analysisMethod == "fit") || (analysisMethod == "sidebands") )
        ggH_syst[label] = new TH1F(("ggH_"+label).c_str(),"",nBins,xMin,xMax);
      if( analysisMethod == "fitNoHoles" )
        ggH_syst[label] = new TH1F(("ggH_"+label+"_dummy").c_str(),"",nBins,xMin,xMax);
      
      for(int bin = 1; bin <= histo->GetNbinsX(); ++bin)
      {
        float binCenter  = histo -> GetBinCenter(bin);
        float binContent = histo -> GetBinContent(bin);
        if( (binCenter >= xMin) && (binCenter < xMax) )
        {
          ggH_syst[label] -> Fill(binCenter,binContent);
        }
      }
      
      if( (analysisMethod == "fit") || (analysisMethod == "sidebands") )
        ggH_syst[label] -> Write();
      if( analysisMethod == "fitNoHoles" )
      {
        dh_ggH_syst[label] = new RooDataHist(("dh_ggH_"+label).c_str(),"",RooArgList(*x),ggH_syst[label]);
        pdf_ggH_syst[label] = new RooHistPdf(("ggH_"+label).c_str(),"",RooArgList(*x),*dh_ggH_syst[label],2);
        workspace -> import(*pdf_ggH_syst[label]);
      }

      n_ggH_syst[label] = ggH_syst[label] -> Integral(binMin,binMax);
      nWindow_ggH_syst[label] = ggH_syst[label] -> Integral(binMinWindow,binMaxWindow);
      
      
      // qq Higgs
      inFile -> cd();
      histo = (TH1F*)(inFile->Get(histoName_qq.str().c_str()));
      outFile -> cd();
      if( (analysisMethod == "fit") || (analysisMethod == "sidebands") )
        qqH_syst[label] = new TH1F(("qqH_"+label).c_str(),"",nBins,xMin,xMax);
      if( analysisMethod == "fitNoHoles" )
        qqH_syst[label] = new TH1F(("qqH_"+label+"_dummy").c_str(),"",nBins,xMin,xMax);
            
      for(int bin = 1; bin <= histo->GetNbinsX(); ++bin)
      {
        float binCenter  = histo -> GetBinCenter(bin);
        float binContent = histo -> GetBinContent(bin);
        if( (binCenter >= xMin) && (binCenter < xMax) )
        {
          qqH_syst[label] -> Fill(binCenter,binContent);
        }
      }
      if( (analysisMethod == "fit") || (analysisMethod == "sidebands") )      
        qqH_syst[label] -> Write();
      
      if( analysisMethod == "fitNoHoles" )
      {
        dh_qqH_syst[label] = new RooDataHist(("dh_qqH_"+label).c_str(),"",RooArgList(*x),qqH_syst[label]);
        pdf_qqH_syst[label] = new RooHistPdf(("qqH_"+label).c_str(),"",RooArgList(*x),*dh_qqH_syst[label],2);
        workspace -> import(*pdf_qqH_syst[label]);
      }
      
      n_qqH_syst[label] = qqH_syst[label] -> Integral(binMin,binMax);
      nWindow_qqH_syst[label] = qqH_syst[label] -> Integral(binMinWindow,binMaxWindow);
      
      
      
      
      
      
      label = labels.at(labelIt)+"Down";
      
      fileName.str(std::string());
      fileName << baseDir << "/countSignalEvents_" + label + "_PFlow.root";
      inFile = TFile::Open(fileName.str().c_str(),"READ");
      std::cout << ">>> opened file " << fileName.str() << std::endl;
      
      
      // gg Higgs
      inFile -> cd();
      histo = (TH1F*)(inFile->Get(histoName_gg.str().c_str()));
      outFile -> cd();
      if( (analysisMethod == "fit") || (analysisMethod == "sidebands") )
        ggH_syst[label] = new TH1F(("ggH_"+label).c_str(),"",nBins,xMin,xMax);
      if( analysisMethod == "fitNoHoles" )      
        ggH_syst[label] = new TH1F(("ggH_"+label+"_dummy").c_str(),"",nBins,xMin,xMax);
      
      for(int bin = 1; bin <= histo->GetNbinsX(); ++bin)
      {
        float binCenter  = histo -> GetBinCenter(bin);
        float binContent = histo -> GetBinContent(bin);
        if( (binCenter >= xMin) && (binCenter < xMax) )
        {
          ggH_syst[label] -> Fill(binCenter,binContent);
        }
      }
      
      if( (analysisMethod == "fit") || (analysisMethod == "sidebands") )
        ggH_syst[label] -> Write();
      if( analysisMethod == "fitNoHoles" )
      {
        dh_ggH_syst[label] = new RooDataHist(("dh_ggH_"+label).c_str(),"",RooArgList(*x),ggH_syst[label]);
        pdf_ggH_syst[label] = new RooHistPdf(("ggH_"+label).c_str(),"",RooArgList(*x),*dh_ggH_syst[label],2);
        workspace -> import(*pdf_ggH_syst[label]);
      }
      
      n_ggH_syst[label] = ggH_syst[label] -> Integral(binMin,binMax);
      nWindow_ggH_syst[label] = ggH_syst[label] -> Integral(binMinWindow,binMaxWindow);
          
      
      // qq Higgs
      inFile -> cd();
      histo = (TH1F*)(inFile->Get(histoName_qq.str().c_str()));
      outFile -> cd();
      if( (analysisMethod == "fit") || (analysisMethod == "sidebands") )
        qqH_syst[label] = new TH1F(("qqH_"+label).c_str(),"",nBins,xMin,xMax);
      if( analysisMethod == "fitNoHoles" )
        qqH_syst[label] = new TH1F(("qqH_"+label+"_dummy").c_str(),"",nBins,xMin,xMax);
            
      for(int bin = 1; bin <= histo->GetNbinsX(); ++bin)
      {
        float binCenter  = histo -> GetBinCenter(bin);
        float binContent = histo -> GetBinContent(bin);
        if( (binCenter >= xMin) && (binCenter < xMax) )
        {
          qqH_syst[label] -> Fill(binCenter,binContent);
        }
      }
      
      if( (analysisMethod == "fit") || (analysisMethod == "sidebands") )      
        qqH_syst[label] -> Write();
      if( analysisMethod == "fitNoHoles" )
      {
        dh_qqH_syst[label] = new RooDataHist(("dh_qqH_"+label).c_str(),"",RooArgList(*x),qqH_syst[label]);
        pdf_qqH_syst[label] = new RooHistPdf(("qqH_"+label).c_str(),"",RooArgList(*x),*dh_qqH_syst[label],2);
        workspace -> import(*pdf_qqH_syst[label]);
      }
      
      n_qqH_syst[label] = qqH_syst[label] -> Integral(binMin,binMax);
      nWindow_qqH_syst[label] = qqH_syst[label] -> Integral(binMinWindow,binMaxWindow);
    }
    
    
    
    
    
    
    if( analysisMethod == "fitNoHoles" )
    {
      outFile -> cd();
      workspace -> Write();
    }      
    
    
    
    
    
    
    //--------------
    // make datacard
    
    if( (analysisMethod == "fit") || (analysisMethod == "sidebands") )
    {
      std::stringstream ss2;
      if( analysisMethod == "fit" )
        ss2 << outputRootFilePath << "/" << "datacard" << "_" << analysisMethod << "_" << fitMethod << "_" << "bincounting" << "." << mass << ".txt";
      else
        ss2 << outputRootFilePath << "/" << "datacard" << "_" << analysisMethod << "_" << "bincounting" << "." << mass << ".txt";
      std::ofstream datacard_bc(ss2.str().c_str(),std::ios::out);
      
      datacard_bc << std::fixed;
      datacard_bc << "# H > WW > lvjj analysis for mH = " << mass << " GeV/c^2" << std::endl;
      datacard_bc << "# counting experiment" << std::endl;
      datacard_bc << std::endl;
      datacard_bc << "imax 1   # number of channels" << std::endl;
      datacard_bc << "jmax 2   # number of processes - 1" << std::endl;
      datacard_bc << "kmax " << 4 + labels.size() << "   # number of nuisance parameters (sources of systematic uncertainties)" << std::endl;
      datacard_bc << std::endl;
      datacard_bc << "bin           " << std::setprecision(0) << std::setw(5) << 1 << std::endl;
      datacard_bc << "observation   " << std::setprecision(0) << std::setw(5) << nWindow_data_obs << std::endl;
      datacard_bc << std::endl;
      datacard_bc << "bin                     1          1          1" << std::endl;
      datacard_bc << "process               ggH        qqH        bkg" << std::endl;
      datacard_bc << "process                -1         -2          1" << std::endl;
      datacard_bc << "rate             " << std::setprecision(2)
                  << std::setw(8) << nWindow_ggH << "   "
                  << std::setw(8) << nWindow_qqH << "   "
                  << std::setw(8) << nWindow_bkg
                  << std::endl;
      datacard_bc << std::endl;
      datacard_bc << "fitErr     lnN          -          -   " << std::setprecision(3) << std::setw(8) << 1.+nWindow_bkg_err/nWindow_bkg << std::endl;
      datacard_bc << "lumi       lnN      1.045      1.045          -" << std::endl;
      datacard_bc << "other      lnN      1.060      1.060          -" << std::endl;
      datacard_bc << "xsec       lnN   "
                  << std::setprecision(3) << std::setw(8) << 1.+0.5*(HiggsCrossSectionSyst(mass,"gg","up")+HiggsCrossSectionSyst(mass,"gg","down")) << "   "
                  << std::setprecision(3) << std::setw(8) << 1.+0.5*(HiggsCrossSectionSyst(mass,"qq","up")+HiggsCrossSectionSyst(mass,"qq","down")) << "   "
                  << "       -" << std::endl;
      for(unsigned int labelIt = 0; labelIt < labels.size(); ++labelIt)
      {
        std::string label = labels.at(labelIt);
        double ggH_errUp   = fabs(nWindow_ggH_syst[label+"Up"]  - nWindow_ggH);
        double ggH_errDown = fabs(nWindow_ggH_syst[label+"Down"]- nWindow_ggH);
        double ggH_errAve  = 0.5*(ggH_errUp + ggH_errDown);
        double qqH_errUp   = fabs(nWindow_qqH_syst[label+"Up"]  - nWindow_qqH);
        double qqH_errDown = fabs(nWindow_qqH_syst[label+"Down"]- nWindow_qqH);
        double qqH_errAve  = 0.5*(qqH_errUp + qqH_errDown);
        
        datacard_bc << labels2.at(labelIt) << "    lnN   "
                    << std::setprecision(3) << std::setw(8) << 1. + ggH_errAve/nWindow_ggH << "   "
                    << std::setprecision(3) << std::setw(8) << 1. + qqH_errAve/nWindow_qqH << "   "
                   << "       -" << std::endl;
      } 
      
      
      
      std::stringstream ss3;
      if( analysisMethod == "fit" )
        ss3 << outputRootFilePath << "/" << "datacard" << "_" << analysisMethod << "_" << fitMethod << "_" << "shapeanalysis" << "." << mass << ".txt";
      else
        ss3 << outputRootFilePath << "/" << "datacard" << "_" << analysisMethod << "_" << "shapeanalysis" << "." << mass << ".txt";
      std::ofstream datacard_sa(ss3.str().c_str(),std::ios::out);
      
      datacard_sa << std::fixed;
      datacard_sa << "# H > WW > lvjj analysis for mH = " << mass << " GeV/c^2" << std::endl;
      datacard_sa << "# shape analysis" << std::endl;
      datacard_sa << std::endl;
      datacard_sa << "imax 1   # number of channels" << std::endl;
      datacard_sa << "jmax 2   # number of processes - 1" << std::endl;
      datacard_sa << "kmax " << 4 + labels.size() << "   # number of nuisance parameters (sources of systematic uncertainties)" << std::endl;
      datacard_sa << std::endl;
      datacard_sa << "---------------" << std::endl;
      datacard_sa << "shapes * * shapes_" << analysisMethod << "_" << fitMethod << "_" << mass << ".root $PROCESS $PROCESS_$SYSTEMATIC" << std::endl;
      datacard_sa << "---------------" << std::endl;
      datacard_sa << std::endl;
      datacard_sa << "bin           " << std::setprecision(0) << std::setw(5) << 1 << std::endl;
      datacard_sa << "observation   " << std::setprecision(0) << std::setw(5) << n_data_obs << std::endl;    
      datacard_sa << std::endl;
      datacard_sa << "bin                     1          1          1" << std::endl;
      datacard_sa << "process               ggH        qqH        bkg" << std::endl;
      datacard_sa << "process                -1         -2          1" << std::endl;
      datacard_sa << "rate             " << std::setprecision(2)
                  << std::setw(8) << n_ggH << "   "
                  << std::setw(8) << n_qqH << "   "
                  << std::setw(8) << n_bkg
                  << std::endl;
      datacard_sa << std::endl;
      datacard_sa << "fitErr   shape          -          -          1" << std::endl;
      datacard_sa << "lumi       lnN      1.045      1.045          -" << std::endl;
      datacard_sa << "other      lnN      1.060      1.060          -" << std::endl;
      datacard_sa << "xsec       lnN   "
                  << std::setprecision(3) << std::setw(8) << 1.+0.5*(HiggsCrossSectionSyst(mass,"gg","up")+HiggsCrossSectionSyst(mass,"gg","down")) << "   "
                  << std::setprecision(3) << std::setw(8) << 1.+0.5*(HiggsCrossSectionSyst(mass,"qq","up")+HiggsCrossSectionSyst(mass,"qq","down")) << "   "
                  << "       -" << std::endl;
      for(unsigned int labelIt = 0; labelIt < labels.size(); ++labelIt)
      {
        std::string label = labels.at(labelIt);
        
        datacard_sa << labels2.at(labelIt) << "  shape          1          1          - " << std::endl;
      }
    } 
    
    
    if( analysisMethod == "fitNoHoles" )
    {
      std::stringstream ss3;
      ss3 << outputRootFilePath << "/" << "datacard" << "_" << analysisMethod << "_" << fitMethod << "_"<< "shapeanalysis" << "." << mass << ".txt";
      std::ofstream datacard_sa(ss3.str().c_str(),std::ios::out);
      
      datacard_sa << std::fixed;
      datacard_sa << "# H > WW > lvjj analysis for mH = " << mass << " GeV/c^2" << std::endl;
      datacard_sa << "# parametric shape analysis" << std::endl;
      datacard_sa << std::endl;
      datacard_sa << "imax 1   # number of channels" << std::endl;
      datacard_sa << "jmax 2   # number of processes - 1" << std::endl;
      datacard_sa << "kmax *   # number of nuisance parameters (sources of systematic uncertainties)" << std::endl;
      datacard_sa << std::endl;
      datacard_sa << "---------------" << std::endl;
      datacard_sa << "shapes * * shapes_" << analysisMethod << "_" << fitMethod << "_" << mass << ".root workspace:$PROCESS workspace:$PROCESS_$SYSTEMATIC" << std::endl;
      datacard_sa << "---------------" << std::endl;
      datacard_sa << std::endl;
      datacard_sa << "bin           " << std::setprecision(0) << std::setw(5) << 1 << std::endl;
      datacard_sa << "observation   " << std::setprecision(0) << std::setw(5) << n_data_obs << std::endl;    
      datacard_sa << std::endl;
      datacard_sa << "bin                     1          1          1" << std::endl;
      datacard_sa << "process               ggH        qqH        bkg" << std::endl;
      datacard_sa << "process                -1         -2          1" << std::endl;
      datacard_sa << "rate             " << std::setprecision(2)
                  << std::setw(8) << n_ggH << "   "
                  << std::setw(8) << n_qqH << "   "
                  << std::setw(8) << n_data_obs
                  << std::endl;
      datacard_sa << std::endl;
      datacard_sa << "norm       lnU          -          -      1.500" << std::endl;
      datacard_sa << "lumi       lnN      1.045      1.045          -" << std::endl;
      datacard_sa << "other      lnN      1.060      1.060          -" << std::endl;
      datacard_sa << "xsec       lnN   "
                  << std::setprecision(3) << std::setw(8) << 1.+0.5*(HiggsCrossSectionSyst(mass,"gg","up")+HiggsCrossSectionSyst(mass,"gg","down")) << "   "
                  << std::setprecision(3) << std::setw(8) << 1.+0.5*(HiggsCrossSectionSyst(mass,"qq","up")+HiggsCrossSectionSyst(mass,"qq","down")) << "   "
                  << "       -" << std::endl;
      for(unsigned int labelIt = 0; labelIt < labels.size(); ++labelIt)
      {
        std::string label = labels.at(labelIt);
        
        datacard_sa << labels2.at(labelIt) << "  shape          1          1          - " << std::endl;
      }
      datacard_sa << std::endl;
      if( fitMethod == "doubleExponential" )
      {
        datacard_sa << "N    param    0.007     1.   [0.,1.]"   << std::endl; 
        datacard_sa << "L1   param    0.015     1.   [0.,0.1]"  << std::endl; 
        datacard_sa << "L2   param    0.005     1.   [0.,0.1]"  << std::endl; 
      }
      if( fitMethod == "attenuatedDoubleExponential" )
      {
        datacard_sa << "mu   param     160.   100.   [0.,500.]" << std::endl; 
        datacard_sa << "kT   param      20.   100.   [0.,100.]" << std::endl; 
        datacard_sa << "N    param    0.007     1.   [0.,1.]"   << std::endl; 
        datacard_sa << "L1   param    0.015     1.   [0.,0.1]"  << std::endl; 
        datacard_sa << "L2   param    0.005     1.   [0.,0.1]"  << std::endl; 
      }
    }
    
    
    outFile -> Close();
  }
  
  
  
  return 1;
}
