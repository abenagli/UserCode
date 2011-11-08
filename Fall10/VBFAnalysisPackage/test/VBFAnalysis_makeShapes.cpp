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
    data_obs = (TH1F*)(data->Clone("data_obs"));
    data_obs -> Reset();
    
    hint = (TH1F*)(inFile->Get("data/hint"));
    bkg            = (TH1F*)(hint->Clone("bkg"));
    bkg_fitErrUp   = (TH1F*)(hint->Clone("bkg_fitErrUp"));
    bkg_fitErrDown = (TH1F*)(hint->Clone("bkg_fitErrDown"));
    bkg            -> Reset();
    bkg_fitErrUp   -> Reset();
    bkg_fitErrDown -> Reset();
    
    for(int bin = 1; bin <= data->GetNbinsX(); ++bin)
    {
      float binCenter  = data -> GetBinCenter(bin);
      float binContent = data -> GetBinContent(bin);
      if( (binCenter >= GetLepNuWMMIN(mass)) && (binCenter < GetLepNuWMMAX(mass)) )
        data_obs -> SetBinContent(bin,binContent);
    }
    
    for(int bin = 1; bin <= hint->GetNbinsX(); ++bin)
    {
      float binCenter  = hint -> GetBinCenter(bin);
      float binContent = hint -> GetBinContent(bin);
      float binError   = hint -> GetBinError(bin);
      if( (binCenter >= GetLepNuWMMIN(mass)) && (binCenter < GetLepNuWMMAX(mass)) )
      {
        bkg            -> SetBinContent(bin,binContent);
        bkg_fitErrUp   -> SetBinContent(bin,binContent+binError);
        bkg_fitErrDown -> SetBinContent(bin,binContent-binError);
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
    histo_gg -> SetName("ggH");
    histo_gg -> Write();
    
    inFile -> cd();
    histo_qq = (TH1F*)(inFile->Get(histoName_qq.str().c_str()));
    outFile -> cd();
    histo_qq -> SetName("qqH");
    histo_qq -> Write();
    
    
    
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
      histo_gg -> SetName(("ggH_"+label).c_str());
      histo_gg -> Write();
      
      inFile -> cd();
      histo_qq = (TH1F*)(inFile->Get(histoName_qq.str().c_str()));
      outFile -> cd();
      histo_qq -> SetName(("qqH_"+label).c_str());
      histo_qq -> Write();
    }
    
    
    
    outFile -> Close();
  }
  
  
  return 1;
}
