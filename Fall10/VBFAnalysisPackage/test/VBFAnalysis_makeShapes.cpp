#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "HiggsMassWindows.h"

#include <iostream>
#include <sstream>

#include "TH1F.h"
#include "TF1.h"






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
  
  //[Output]
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  
  
  
  int nMasses = 8;
  int* masses = new int[nMasses];
  masses[0] = 250;
  masses[1] = 300;
  masses[2] = 350;
  masses[3] = 400;
  masses[4] = 450;
  masses[5] = 500;
  masses[6] = 550;
  masses[7] = 600;
  
  
  
  for(int iMass = 0; iMass < nMasses; ++iMass)
  {
    int mass = masses[iMass];
    std::cout << ">>> mass: " << mass << std::endl;
    
    
    // define outfile
    std::stringstream ss;
    ss << outputRootFilePath << "/" << outputRootFileName << "_" << mass << ".root";
    TFile* outFile = new TFile((ss.str()).c_str(),"RECREATE");
    
    
    // define infile
    std::stringstream fileName;
    TFile* inFile;
    
    TH1F* data;
    TH1F* data_obs;
    TH1F* hint;
    TH1F* bkg;
    TH1F* bkg_fitErrUp;
    TH1F* bkg_fitErrDown;

    TH1F* histo;
    TH1F* histo_gg;
    TH1F* histo_qq;
    
    std::stringstream histoName_gg;
    histoName_gg << "ggH" << mass;
    std::stringstream histoName_qq;
    histoName_qq << "qqH" << mass;
    
    
    
    // data shapes
    fileName.str(std::string());
    fileName << baseDir << "/fitHiggsMassBinned_doubleExponential_PFlow_H" << mass << ".root";
    inFile = TFile::Open(fileName.str().c_str(),"READ");
    std::cout << ">>> opened file " << fileName.str() << std::endl; 
    
    inFile -> cd();
    data = (TH1F*)(inFile->Get("data/h_lepNuW_m"));
    
    float xWidth = data -> GetBinWidth(1);
    float xMin = GetLepNuWMMIN(mass);
    float xMax = GetLepNuWMMAX(mass);
    int nBins = int( (xMax - xMin)/xWidth );
    
    data_obs = new TH1F("data_obs","",nBins,xMin,xMax);
    data_obs -> Sumw2();
    
    
    hint = (TH1F*)(inFile->Get("data/hint"));
    bkg            = new TH1F("bkg",           "",nBins,xMin,xMax);
    bkg_fitErrUp   = new TH1F("bkg_fitErrUp",  "",nBins,xMin,xMax);
    bkg_fitErrDown = new TH1F("bkg_fitErrDown","",nBins,xMin,xMax);
    
    for(int bin = 1; bin <= data->GetNbinsX(); ++bin)
    {
      float binCenter  = data -> GetBinCenter(bin);
      float binContent = data -> GetBinContent(bin);
      if( (binCenter >= xMin) && (binCenter < xMax) )
        for(int entry = 0; entry < binContent; ++entry)
          data_obs -> Fill(binCenter);
    }
    
    for(int bin = 1; bin <= hint->GetNbinsX(); ++bin)
    {
      float binCenter  = hint -> GetBinCenter(bin);
      float binContent = hint -> GetBinContent(bin);
      float binError   = hint -> GetBinError(bin);
      if( (binCenter >= xMin) && (binCenter < xMax) )
      {
        bkg            -> Fill(binCenter,binContent);
        bkg_fitErrUp   -> Fill(binCenter,binContent+binError);
        bkg_fitErrDown -> Fill(binCenter,binContent-binError);
      }
    }
    
    outFile -> cd();
    data_obs       -> Write();
    bkg            -> Write();
    bkg_fitErrUp   -> Write();
    bkg_fitErrDown -> Write();
    
    
    
    
    
    
    // standard signal shapes
    fileName.str(std::string());
    fileName << baseDir << "/countSignalEvents_PFlow.root";
    inFile = TFile::Open(fileName.str().c_str(),"READ");
    std::cout << ">>> opened file " << fileName.str() << std::endl; 
    
    inFile -> cd();
    histo_gg = (TH1F*)(inFile->Get(histoName_gg.str().c_str()));
    outFile -> cd();
    histo = new TH1F("ggH","",nBins,xMin,xMax);
    //histo -> Sumw2();
    for(int bin = 1; bin <= histo_gg->GetNbinsX(); ++bin)
    {
      float binCenter  = histo_gg -> GetBinCenter(bin);
      float binContent = histo_gg -> GetBinContent(bin);
      if( (binCenter >= xMin) && (binCenter < xMax) )
      {
        histo -> Fill(binCenter,binContent);
      }
    }
    histo -> Write();
    
    inFile -> cd();
    histo_qq = (TH1F*)(inFile->Get(histoName_qq.str().c_str()));
    outFile -> cd();
    histo = new TH1F("qqH","",nBins,xMin,xMax);
    //histo -> Sumw2();
    for(int bin = 1; bin <= histo_qq->GetNbinsX(); ++bin)
    {
      float binCenter  = histo_qq -> GetBinCenter(bin);
      float binContent = histo_qq -> GetBinContent(bin);
      if( (binCenter >= xMin) && (binCenter < xMax) )
      {
        histo -> Fill(binCenter,binContent);
      }
    }
    histo -> Write();
    
    
    
    // systematic signal shapes
    std::vector<std::string> labels;
    labels.push_back("PUUp");
    labels.push_back("PUDown");
    labels.push_back("JESUp");
    labels.push_back("JESDown");
    
    for(unsigned int labelIt = 0; labelIt < labels.size(); ++labelIt)
    {
      std::string label = labels.at(labelIt);
      
      fileName.str(std::string());
      fileName << baseDir << "/countSignalEvents_" + label + "_PFlow.root";
      inFile = TFile::Open(fileName.str().c_str(),"READ");
      std::cout << ">>> opened file " << fileName.str() << std::endl;
      
      inFile -> cd();
      histo_gg = (TH1F*)(inFile->Get(histoName_gg.str().c_str()));
      outFile -> cd();
      histo = new TH1F(("ggH_"+label).c_str(),"",nBins,xMin,xMax);
      //histo -> Sumw2();
      for(int bin = 1; bin <= histo_gg->GetNbinsX(); ++bin)
      {
        float binCenter  = histo_gg -> GetBinCenter(bin);
        float binContent = histo_gg -> GetBinContent(bin);
        if( (binCenter >= xMin) && (binCenter < xMax) )
        {
          histo -> Fill(binCenter,binContent);
        }
      }
      histo -> Write();
      
      inFile -> cd();
      histo_qq = (TH1F*)(inFile->Get(histoName_qq.str().c_str()));
      outFile -> cd();
      histo = new TH1F(("qqH_"+label).c_str(),"",nBins,xMin,xMax);
      //histo -> Sumw2();  
      for(int bin = 1; bin <= histo_qq->GetNbinsX(); ++bin)
      {
        float binCenter  = histo_qq -> GetBinCenter(bin);
        float binContent = histo_qq -> GetBinContent(bin);
        if( (binCenter >= xMin) && (binCenter < xMax) )
        {
          histo -> Fill(binCenter,binContent);
        }
      }      
      histo -> Write();
    }
    
    
    
    outFile -> Close();
  }
  
  
  return 1;
}
