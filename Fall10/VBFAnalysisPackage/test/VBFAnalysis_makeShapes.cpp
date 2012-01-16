#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "HiggsMassWindows.h"
#include "HiggsCrossSectionSyst.h"
#include "Functions.h"

#include <iostream>
#include <iomanip>
#include <sstream>

#include "TH1F.h"
#include "TF1.h"

#include "RooMsgService.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooGenericPdf.h"
#include "RooWorkspace.h"
#include "RooGaussian.h"





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
  std::string baseDir = gConfigParser -> readStringOption("Input::baseDir");
  std::string fitMethod      = gConfigParser -> readStringOption("Input::fitMethod");
  std::string analysisMethod = gConfigParser -> readStringOption("Input::analysisMethod");
  
  //[Output]
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  
  //[Options]
  float sigStrength = gConfigParser -> readFloatOption("Options::sigStrength");
  
  
  
  std::vector<int> masses = GetMasses();
  unsigned int nMasses = masses.size();
  
  for(unsigned int iMass = 0; iMass < nMasses; ++iMass)
  {
    int mass = masses[iMass];
    char massChar[50];
    sprintf(massChar,"%d",mass);
    std::cout << ">>> mass: " << mass << std::endl;
    float lepNuWMMIN = GetLepNuWMMIN(mass);
    float lepNuWMMAX = GetLepNuWMMAX(mass);    
    
    
    // define outfile
    std::stringstream ss;
    if( (analysisMethod == "fit") || (analysisMethod == "fitNoHoles") )
      ss << outputRootFilePath << "/" << outputRootFileName << "_" << analysisMethod << "_" << fitMethod << "_" << mass << ".root";
    else
      ss << outputRootFilePath << "/" << outputRootFileName << "_" << analysisMethod << "_" << fitMethod << "_" << mass << ".root";
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
    TH1F* histo_toFit;
    std::map<std::string,TH1F*> H;
    std::map<std::string,TH1F*> H_fit;
    std::map<std::string,RooDataHist*> dh_H;
    std::map<std::string,RooHistPdf*> pdf_H;
    std::map<std::string,double> n_H;
    std::map<std::string,double> nWindow_H;
    
    RooWorkspace* workspace;
    RooRealVar* x;
    RooRealVar* mu;
    RooRealVar* kT;
    RooRealVar* N;
    RooRealVar* L1;
    RooRealVar* L2;
    RooGenericPdf* pdf_bkg;
    RooDataHist* dh_data_obs; 
    
    
    
    
    
    
    //------------------
    // data & bkgshapes
    
    fileName.str(std::string());
    if( (analysisMethod == "fit") || (analysisMethod == "fitNoHoles") ||
	(analysisMethod == "fake") || (analysisMethod == "fakeNoHoles") )
      fileName << baseDir << "/fitHiggsMassBinned_" << fitMethod << "_PFlow_H" << mass << ".root";
    if( analysisMethod == "sidebands" )
      fileName << baseDir << "/output_017.root";
    inFile = TFile::Open(fileName.str().c_str(),"READ");
    //std::cout << ">>> opened file " << fileName.str() << std::endl; 
    
    inFile -> cd();
    
    
    if( analysisMethod == "fit" )
    {
      data = (TH1F*)( inFile->Get("data/h_data_lepNuW_m") );
      hint = (TH1F*)( inFile->Get("data/hint") );
      
      xWidth = data -> GetBinWidth(1);
      if( (fitMethod == "doubleExponential") ||
          (fitMethod == "attenuatedExponential") ||
          (fitMethod == "attenuatedDoubleExponential") )
      {
        xMin = GetLepNuWMMIN(mass);
        xMax = GetLepNuWMMAX(mass);
      }
      if( (fitMethod == "doubleExponentialNoHoles") ||
          (fitMethod == "attenuatedExponentialNoHoles") ||
          (fitMethod == "attenuatedDoubleExponentialNoHoles") )
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
    
    if( (analysisMethod == "fitNoHoles") || (analysisMethod == "fakeNoHoles") ) 
    {
      if( analysisMethod == "fitNoHoles" )
        data = (TH1F*)( inFile->Get("data/h_data_lepNuW_m") );
      if( analysisMethod == "fakeNoHoles" )
        data = (TH1F*)( inFile->Get("fake/h_fake_lepNuW_m") );
      
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
        N  = new RooRealVar("N", "N", 0.711,0.,10.0);
        L1 = new RooRealVar("L1","L1",0.015,0.,0.1);
        L2 = new RooRealVar("L2","L2",0.011,0.,0.1);
        pdf_bkg = new RooGenericPdf("bkg","","(exp(-1*@2*@0) + @1*exp(-1*@3*@0))",RooArgSet(*x,*N,*L1,*L2));
        
        workspace -> import(*N);
        workspace -> import(*L1);
        workspace -> import(*L2);
        workspace -> import(*pdf_bkg);
      }
      if( fitMethod == "attenuatedExponential" )
      {
        mu = new RooRealVar("mu","mu",130.,0.,500.); 
        kT = new RooRealVar("kT","kT",20.,0.,100.); 
        L1 = new RooRealVar("L1","L1",0.012,0.,0.1);
        pdf_bkg = new RooGenericPdf("bkg","","1./(exp(-1.*(@0-@1)/@2)+1.) * exp(-1*@3*@0)",RooArgSet(*x,*mu,*kT,*L1));
        
        workspace -> import(*mu);
        workspace -> import(*kT);
        workspace -> import(*L1);
        workspace -> import(*pdf_bkg);
      }
      if( fitMethod == "attenuatedDoubleExponential" )
      {
        mu = new RooRealVar("mu","mu",190.,190.,190.); 
        kT = new RooRealVar("kT","kT", 55., 55.,55.); 
        N  = new RooRealVar("N", "N", 0.059,0.,1.0);
        L1 = new RooRealVar("L1","L1",0.023,0.,0.1);
        L2 = new RooRealVar("L2","L2",0.011,0.,0.1);
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
      xMin = GetXFitMIN1(mass,fitMethod);
      xMax = GetXFitMAX2(mass,fitMethod);
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
    
    
    if( (analysisMethod == "fitNoHoles") || (analysisMethod == "fakeNoHoles") )
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
    
    std::vector<std::string> labels;
    labels.push_back("ggH");
    labels.push_back("qqH");
    std::vector<std::string> labels_syst;
    labels_syst.push_back("JESUp");
    labels_syst.push_back("JESDown");
    labels_syst.push_back("PUUp");
    labels_syst.push_back("PUDown");
    
    std::vector<std::string> labels2;
    labels2.push_back("PU     ");
    labels2.push_back("JES    ");

    for(unsigned int labelIt = 0; labelIt < 2 + 2*labels_syst.size(); ++labelIt)
    {
      std::string label;
      if(labelIt < 2) label = labels.at(labelIt);
      else            label = labels.at(labelIt%2) + "_" + labels_syst.at((labelIt-2)/2);
      
      fileName.str(std::string());
      if(labelIt < 2) fileName << baseDir << "/countSignalEvents_PFlow.root";
      else            fileName << baseDir << "/countSignalEvents_" + labels_syst.at((labelIt-2)/2) + "_PFlow.root";
      inFile = TFile::Open(fileName.str().c_str(),"READ");
      //std::cout << ">>> opened file " << fileName.str() << std::endl; 
      std::cout << ">>>>>> label: " << label << std::endl;
      
      
      if( labelIt%2 == 0 ) histo = (TH1F*)( inFile->Get(("h_ggH"+std::string(massChar)).c_str()) );
      if( labelIt%2 == 1 ) histo = (TH1F*)( inFile->Get(("h_qqH"+std::string(massChar)).c_str()) );
      
      outFile -> cd();
      histo_toFit = (TH1F*)( histo->Clone((label+"_toFit").c_str()) );
      histo_toFit -> Reset();
      H[label] = new TH1F((label+"_dummy").c_str(),"",nBins,xMin,xMax);
      
      for(int bin = 1; bin <= histo->GetNbinsX(); ++bin)
      {
        float binCenter  = histo -> GetBinCenter(bin);
        float binContent = histo -> GetBinContent(bin);
        float binError   = histo -> GetBinError(bin);
        if( binContent/histo->Integral() > 0.0005 )
        {
          int localBin = histo_toFit -> Fill(binCenter,sigStrength*binContent);
          histo_toFit -> SetBinError(localBin,binError);
        }
      }
      
      for(int bin = 1; bin <= histo->GetNbinsX(); ++bin)
      {
        float binCenter  = histo -> GetBinCenter(bin);
        float binContent = histo -> GetBinContent(bin);
        float binError   = histo -> GetBinError(bin);
        if( (binCenter >= xMin) && (binCenter < xMax) && (binContent/histo->Integral() > 0.0005) )
        {
          int localBin = H[label] -> Fill(binCenter,sigStrength*binContent);
          H[label] -> SetBinError(localBin,binError);
        }
      }
      
      n_H[label]       = H[label] -> Integral(binMin,binMax);
      nWindow_H[label] = H[label] -> Integral(binMinWindow,binMaxWindow);      
      
      
      
      TF1* fitFunc;
      FitHiggsMass(&fitFunc,"fitFunc",xMin,xMax,histo_toFit,mass,"crystalBallLowHigh");
      histo_toFit -> Write();
      
      
      if( (analysisMethod == "fit") || (analysisMethod == "sidebands") )
        H_fit[label] = (TH1F*)( H[label]->Clone(label.c_str()) );
      if( (analysisMethod == "fitNoHoles") || (analysisMethod == "fakeNoHoles") )
        H_fit[label] = (TH1F*)( H[label]->Clone((label+"_fit").c_str()) );
      H_fit[label] -> Reset();
      H_fit[label] -> FillRandom(fitFunc->GetName(),1000000);
      H_fit[label] -> Scale(n_H[label]/H_fit[label]->Integral());
      
      
      if( (analysisMethod == "fit") || (analysisMethod == "sidebands") )
      {
        H_fit[label] -> Write();      
      }
      if( (analysisMethod == "fitNoHoles") || (analysisMethod == "fakeNoHoles") )
      {
        dh_H[label] = new RooDataHist(("dh_"+label).c_str(),"",RooArgList(*x),H_fit[label]);
        pdf_H[label] = new RooHistPdf(label.c_str(),"",RooArgList(*x),*dh_H[label],2);
        workspace -> import(*pdf_H[label]);
      }
      
      //float effMean = mass + fitFunc->GetParameter(3);
      //float effSigma = sqrt(GetHiggsWidth(mass)*GetHiggsWidth(mass) + fitFunc->GetParameter(4)*fitFunc->GetParameter(4));
      //RooRealVar* mean    = new RooRealVar("mean","mean",effMean,effMean,effMean);
      //RooRealVar* sigma = new RooRealVar("sigma","sigma",effSigma,effSigma,effSigma);
      //RooGaussian* gaussian = new RooGaussian("ggH","",*x,*mean,*sigma);
      //
      //workspace -> import(*mean);
      //workspace -> import(*sigma);
      //workspace -> import(*gaussian);
      
      delete fitFunc;
      inFile -> Close();
    }
    
    
    
    
    
    
    if( (analysisMethod == "fitNoHoles") || (analysisMethod == "fakeNoHoles") )
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
        ss2 << outputRootFilePath << "/" << "datacard" << "_" << analysisMethod << "_" << fitMethod << "_" << "bincounting" << "." << mass << ".txt";
      std::ofstream datacard_bc(ss2.str().c_str(),std::ios::out);
      
      datacard_bc << std::fixed;
      datacard_bc << "# H > WW > lvjj analysis for mH = " << mass << " GeV/c^2" << std::endl;
      datacard_bc << "# counting experiment" << std::endl;
      datacard_bc << std::endl;
      datacard_bc << "imax 1   # number of channels" << std::endl;
      datacard_bc << "jmax 2   # number of processes - 1" << std::endl;
      datacard_bc << "kmax " << 4 + labels2.size() << "   # number of nuisance parameters (sources of systematic uncertainties)" << std::endl;
      datacard_bc << std::endl;
      datacard_bc << "bin           " << std::setprecision(0) << std::setw(5) << 1 << std::endl;
      datacard_bc << "observation   " << std::setprecision(0) << std::setw(5) << nWindow_data_obs << std::endl;
      datacard_bc << std::endl;
      datacard_bc << "bin                     1          1          1" << std::endl;
      datacard_bc << "process               ggH        qqH        bkg" << std::endl;
      datacard_bc << "process                -1         -2          1" << std::endl;
      datacard_bc << "rate             " << std::setprecision(2)
                  << std::setw(8) << nWindow_H["ggH"] << "   "
                  << std::setw(8) << nWindow_H["qqH"] << "   "
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
      for(unsigned int labelIt = 0; labelIt < labels2.size(); ++labelIt)
      {
        double ggH_errUp   = fabs(nWindow_H["ggH_"+labels_syst.at(2*labelIt)]   - nWindow_H["ggH"]);
        double qqH_errUp   = fabs(nWindow_H["qqH_"+labels_syst.at(2*labelIt)]   - nWindow_H["qqH"]);
        double ggH_errDown = fabs(nWindow_H["ggH_"+labels_syst.at(2*labelIt+1)] - nWindow_H["ggH"]);
        double qqH_errDown = fabs(nWindow_H["qqH_"+labels_syst.at(2*labelIt+1)] - nWindow_H["qqH"]);
        
        double ggH_errAve  = 0.5*(ggH_errUp + ggH_errDown);
        double qqH_errAve  = 0.5*(qqH_errUp + qqH_errDown);
        
        datacard_bc << labels2.at(labelIt) << "    lnN   "
                    << std::setprecision(3) << std::setw(8) << 1. + ggH_errAve/nWindow_H["ggH"] << "   "
                    << std::setprecision(3) << std::setw(8) << 1. + qqH_errAve/nWindow_H["qqH"] << "   "
                   << "       -" << std::endl;
      } 
      
      
      
      std::stringstream ss3;
      if( analysisMethod == "fit" )
        ss3 << outputRootFilePath << "/" << "datacard" << "_" << analysisMethod << "_" << fitMethod << "_" << "shapeanalysis" << "." << mass << ".txt";
      else
        ss3 << outputRootFilePath << "/" << "datacard" << "_" << analysisMethod << "_" << fitMethod << "_" << "shapeanalysis" << "." << mass << ".txt";
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
                  << std::setw(8) << n_H["ggH"] << "   "
                  << std::setw(8) << n_H["qqH"] << "   "
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
    
    
    if( (analysisMethod == "fitNoHoles") || (analysisMethod == "fakeNoHoles") )
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
                  << std::setw(8) << n_H["ggH"] << "   "
                  << std::setw(8) << n_H["qqH"] << "   "
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
      //for(unsigned int labelIt = 0; labelIt < labels.size(); ++labelIt)
      //{
      //  std::string label = labels.at(labelIt);
      //  
      //  datacard_sa << labels2.at(labelIt) << "  shape          1          1          - " << std::endl;
      //}
      datacard_sa << std::endl;
      if( fitMethod == "doubleExponential" )
      {
        datacard_sa << "N    param    0.711     1.   [0.,10.]"   << std::endl; 
        datacard_sa << "L1   param    0.015     1.   [0.,0.1]"  << std::endl; 
        datacard_sa << "L2   param    0.011     1.   [0.,0.1]"  << std::endl; 
      }
      if( fitMethod == "attenuatedExponential" )
      {
        datacard_sa << "mu   param     130.   100.   [0.,500.]" << std::endl; 
        datacard_sa << "kT   param      20.   100.   [0.,100.]" << std::endl; 
        datacard_sa << "L1   param    0.012     1.   [0.,0.1]"  << std::endl; 
      }
      if( fitMethod == "attenuatedDoubleExponential" )
      {
        //datacard_sa << "mu   param     190.     0.   [190.,190.]" << std::endl; 
        //datacard_sa << "kT   param      55.     0.   [55.,55.]" << std::endl; 
        datacard_sa << "N    param    0.059     1.   [0.,1.]"   << std::endl; 
        datacard_sa << "L1   param    0.023     1.   [0.,0.1]"  << std::endl; 
        datacard_sa << "L2   param    0.011     1.   [0.,0.1]"  << std::endl; 
      }
    }
    
    
    outFile -> Close();
  }
  
  
  
  return 1;
}
