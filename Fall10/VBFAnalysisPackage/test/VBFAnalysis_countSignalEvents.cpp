#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "PUUtils.h"
#include "HiggsMassWindows.h"
#include "HiggsMassFits.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <map>

#include "TH1F.h"



void RegularizeHistogram(TH1F* h);

float GetCut_lepMet_Dphi(const float& mH);
float GetCut_WJJ_Dphi(const float& mH);






int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> VBFAnalysis_countSignalEvents::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
  
  
  //[Input]
  std::string inputDir = gConfigParser -> readStringOption("Input::inputDir");
  float lumi = gConfigParser -> readFloatOption("Input::lumi");
  
  std::string inputFileName = gConfigParser -> readStringOption("Input::inputFileName");
  std::vector<std::string> inputSigDirs  = gConfigParser -> readStringListOption("Input::inputSigDirs");
  std::vector<std::string> inputDataDirs = gConfigParser -> readStringListOption("Input::inputDataDirs");
  
  unsigned int nSigTrees  = inputSigDirs.size();
  unsigned int nDataTrees = inputDataDirs.size();
  
  std::vector<std::string> inputTotDirs;
  for(unsigned int i = 0; i < nSigTrees; ++ i)  inputTotDirs.push_back(inputSigDirs.at(i));
  for(unsigned int i = 0; i < nDataTrees; ++ i) inputTotDirs.push_back(inputDataDirs.at(i));
  
  
  //[Output]
  std::string outputRootFilePath  = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName  = gConfigParser -> readStringOption("Output::outputRootFileName");
  std::string outputRootFileLabel = gConfigParser -> readStringOption("Output::outputRootFileLabel");

  
  //[Options]
  int onData = gConfigParser -> readIntOption("Options::onData");
  unsigned int nTotTrees  = nSigTrees;
  if( onData == 1 ) nTotTrees += nDataTrees;
  
  float xWidth = gConfigParser -> readFloatOption("Options::xWidth");
  char xWidthChar[50];
  sprintf(xWidthChar,"%d",int(xWidth));
  int step = gConfigParser -> readIntOption("Options::step");
  char stepChar[50];
  sprintf(stepChar,"%d",step);
  
  std::string additionalCuts = gConfigParser -> readStringOption("Options::additionalCuts");
  
  int PUScale = gConfigParser -> readIntOption("Options::PUScale");
  std::string pileupFileName = gConfigParser -> readStringOption("Options::pileupFileName");
  
  float xMin = 0.;
  float xMax = 1000.;
  int nBins = int((xMax-xMin)/xWidth);
  xMax = xMin + xWidth*nBins;
  
  
  
  // Define the output file
  if( additionalCuts == "none" )
    outputRootFilePath += "/countSignalEvents/binWidth" + std::string(xWidthChar) + "/step" + std::string(stepChar) + "/";
  else
    outputRootFilePath += "/countSignalEvents/binWidth" + std::string(xWidthChar) + "/step" + std::string(stepChar) + "_" + additionalCuts + "/";
  
  std::string outputRootFullFileName;
  if( outputRootFileLabel == "none" )
    outputRootFullFileName = outputRootFilePath + outputRootFileName + ".root";
  else
    outputRootFullFileName = outputRootFilePath + outputRootFileName + "_" + outputRootFileLabel + ".root";
  TFile* outFile = new TFile(outputRootFullFileName.c_str(), "RECREATE");
  
  
  
  // get the data pileup distribution
  TFile* inFile_pileup = TFile::Open(pileupFileName.c_str(),"READ");
  TH1F* distrPU_DATA;
  if     ( PUScale ==  1 ) distrPU_DATA = (TH1F*)( inFile_pileup->Get("pileupUp") );
  else if( PUScale == -1 ) distrPU_DATA = (TH1F*)( inFile_pileup->Get("pileupDown") );
  else                     distrPU_DATA = (TH1F*)( inFile_pileup->Get("pileup") );
  distrPU_DATA -> Scale(1./distrPU_DATA->Integral());
  
  
  
  
  
  
  //------------------
  // Define histograms
  
  std::vector<int> masses = GetMasses();
  unsigned int nMasses = masses.size();
  
  std::vector<std::string> flavours;
  flavours.push_back("emu");
  flavours.push_back("e");
  flavours.push_back("mu");
  unsigned int nFlavours = flavours.size();
  
  std::stringstream label;
  
  
  
  std::map<std::string,TH1F*> h_data;
  std::map<std::string,TH1F*> h_qqH;
  std::map<std::string,TH1F*> h_ggH;
  
  for(unsigned int massIt = 0; massIt < nMasses; ++massIt)
    for(unsigned int flavourIt = 0; flavourIt < nFlavours; ++flavourIt)
    {
      int mass = masses.at(massIt);
      
      // data histograms
      if( onData == 1 )
      {
        label.str(std::string());
        label << "h_data" << mass << "_" << flavours.at(flavourIt);
        h_data[label.str()] = new TH1F(label.str().c_str(),"",nBins,xMin,xMax);
        h_data[label.str()] -> Sumw2();
      }
      
      // signal histograms
      label.str(std::string());
      label << "h_ggH" << mass << "_" << flavours.at(flavourIt);
      h_ggH[label.str()] = new TH1F(label.str().c_str(),"",nBins,xMin,xMax);
      h_ggH[label.str()] -> Sumw2();
      
      label.str(std::string());
      label << "h_qqH" << mass << "_" << flavours.at(flavourIt);
      h_qqH[label.str()] = new TH1F(label.str().c_str(),"",nBins,xMin,xMax);
      h_qqH[label.str()] -> Sumw2();
      
      // signal histograms - fit
      label.str(std::string());
      label << "h_ggH" << mass << "_" << flavours.at(flavourIt) << "_fit";
      h_ggH[label.str()] = new TH1F(label.str().c_str(),"",nBins,xMin,xMax);
      h_ggH[label.str()] -> Sumw2();
      
      label.str(std::string());
      label << "h_qqH" << mass << "_" << flavours.at(flavourIt) << "_fit";
      h_qqH[label.str()] = new TH1F(label.str().c_str(),"",nBins,xMin,xMax);
      h_qqH[label.str()] -> Sumw2();  
    } 
  
  
  
  
  
  
  //----------------
  // Fill Histograms
  
  
  // Define tree variables
  float mH;
  int dataFlag;
  int totEvents;
  int PUtrue_n;
  float eventWeight;
  float crossSection;
  float WJJ_m;
  float lepNuW_m_KF;
  float lepMet_Dphi;
  float WJJ_Dphi;
  float WJ1_ctheta;
  float lepNu_ctheta;
  int lep_flavour;
  
  
  // loop on the events
  for(unsigned int i = 0; i < nTotTrees; ++i)
  {
    // open root file
    std::string inputFullFileName;
    if( i < nSigTrees )
    {
      inputFullFileName = inputDir + "/" + inputSigDirs.at(i)  + "/" + inputFileName + ".root";
      std::cout << ">>>>> VBFAnalysis_countSignalEvents::signal tree in " << inputSigDirs.at(i) << " opened" << std::endl;    
    }
    else
    {
      inputFullFileName = inputDir + "/" + inputDataDirs.at(i-nSigTrees) + "/" + inputFileName + ".root";
      std::cout << ">>>>> VBFAnalysis_countSignalEvents::data tree in " << inputDataDirs.at(i-nSigTrees) << " opened" << std::endl;
    }
    TFile* inputFile = TFile::Open(inputFullFileName.c_str());
    
    
    // get the tree at nth step
    TTree* tree = NULL;
    char treeName[50];
    sprintf(treeName, "ntu_%d", step);
    inputFile -> GetObject(treeName, tree);
    if ( tree -> GetEntries() == 0 ) continue; 
    
    
    // get the mc pileup distribution
    TH1F* distrPU_MC;
    if( i < nSigTrees )
    {
      distrPU_MC = (TH1F*)( inputFile->Get("nPUtrue") );
      distrPU_MC -> Scale(1./distrPU_MC->Integral());
    }  
    
    
    // set tree branches
    tree -> SetBranchAddress("mH",             &mH);
    tree -> SetBranchAddress("dataFlag",       &dataFlag);
    tree -> SetBranchAddress("eventWeight",    &eventWeight);
    tree -> SetBranchAddress("PUtrue_n",       &PUtrue_n);
    tree -> SetBranchAddress("totEvents",      &totEvents);
    tree -> SetBranchAddress("crossSection",   &crossSection);
    tree -> SetBranchAddress("WJJ_m",          &WJJ_m);
    tree -> SetBranchAddress("lepMet_Dphi",    &lepMet_Dphi);
    tree -> SetBranchAddress("WJJ_Dphi",       &WJJ_Dphi);
    tree -> SetBranchAddress("WJ1_ctheta",     &WJ1_ctheta);
    tree -> SetBranchAddress("lepNu_ctheta",   &lepNu_ctheta);
    tree -> SetBranchAddress("lepNuW_m_KF",    &lepNuW_m_KF);
    tree -> SetBranchAddress("lep_flavour",    &lep_flavour);
    
    
    // fill counters and histograms
    for(int entry = 0; entry < tree->GetEntries(); ++entry)
    {
      tree -> GetEntry(entry);
      
      double weight;
      if( i < nSigTrees ) weight = lumi * 1000 * 1. / totEvents * crossSection * PURescaleFactor(distrPU_DATA,distrPU_MC,PUtrue_n) * eventWeight;
      else                weight = 1.;
      
      
      
      // non mass-dependent cuts
      if( (WJJ_m < 65.) || (WJJ_m >= 100.) ) continue;
      //if( WJ1_ctheta > 0.6 ) continue;
      //if( fabs(lepNu_ctheta) > 0.65 ) continue;
      
      
      
      //-------
      // signal
      
      if( i < nSigTrees )
      {
        // count events
        int mass = int(int(mH)%1000);
        
        
        // mass-dependent cuts
        //if( lepMet_Dphi > GetCut_lepMet_Dphi(mass) ) continue;
        //if( WJJ_Dphi > GetCut_WJJ_Dphi(mass) ) continue;
        
        
        // ggH
        if( mH < 1000 )
        {
          for(unsigned int flavourIt = 0; flavourIt < nFlavours; ++flavourIt)
          {
            label.str(std::string());
            label << "h_ggH" << mass << "_" << flavours.at(flavourIt);
            
            if( (flavours.at(flavourIt) == "emu") ||
                ( (flavours.at(flavourIt) == "e")  && (lep_flavour == 11) ) ||
                ( (flavours.at(flavourIt) == "mu") && (lep_flavour == 13) ) )
            {
              h_ggH[label.str()]        -> Fill(lepNuW_m_KF,weight);
              h_ggH[label.str()+"_fit"] -> Fill(lepNuW_m_KF,weight);
            }
          }
        }
        
        // ggH
        else
        {
          for(unsigned int flavourIt = 0; flavourIt < nFlavours; ++flavourIt)
          {
            label.str(std::string());
            label << "h_qqH" << mass << "_" << flavours.at(flavourIt);
            
            if( (flavours.at(flavourIt) == "emu") ||
                ( (flavours.at(flavourIt) == "e")  && (lep_flavour == 11) ) ||
                ( (flavours.at(flavourIt) == "mu") && (lep_flavour == 13) ) )
            {
              h_qqH[label.str()]        -> Fill(lepNuW_m_KF,weight);
              h_qqH[label.str()+"_fit"] -> Fill(lepNuW_m_KF,weight);
	    }
	  }
        }
        
      }
      
      
      //-----
      // data
      
      else
      {
        for(unsigned int iMass = 0; iMass < nMasses; ++iMass)
        {
          int mass = masses.at(iMass);
          
          
          // mass-dependent cuts
          //if( lepMet_Dphi > GetCut_lepMet_Dphi(mass) ) continue;
          //if( WJJ_Dphi > GetCut_WJJ_Dphi(mass) ) continue;
          
          for(unsigned int flavourIt = 0; flavourIt < nFlavours; ++flavourIt)
          {
            label.str(std::string());
            label << "h_data" << mass << "_" << flavours.at(flavourIt);
            
            if( (flavours.at(flavourIt) == "emu") ||
                ( (flavours.at(flavourIt) == "e")  && (lep_flavour == 11) ) ||
                ( (flavours.at(flavourIt) == "mu") && (lep_flavour == 13) ) )
            {
              h_data[label.str()] -> Fill(lepNuW_m_KF,weight);
            }
          }          
        }
      }
      
    }
    
    inputFile -> Close(); 
  }
  
  
  
  
  
  
  //----------------
  // save histograms
  outFile -> cd();
  
  for(unsigned int massIt = 0; massIt < nMasses; ++massIt)
  {
    int mass = masses.at(massIt);
    char massChar[50];
    sprintf(massChar,"H%d",mass);
    
    outFile -> mkdir(massChar);
  }
  
  
  for(unsigned int massIt = 0; massIt < nMasses; ++massIt)
    for(unsigned int flavourIt = 0; flavourIt < nFlavours; ++flavourIt)
    {
      int mass = masses.at(massIt);
      char massChar[50];
      sprintf(massChar,"H%d",mass);
      
      outFile -> cd(massChar);
      
      // data histograms
      if( onData == 1 )
      {
        label.str(std::string());
        label << "h_data" << masses.at(massIt) << "_" << flavours.at(flavourIt);
        h_data[label.str()] -> Write();
      }
      
      // signal histograms
      label.str(std::string());
      label << "h_ggH" << mass << "_" << flavours.at(flavourIt);
      h_ggH[label.str()] -> Write();
      
      label.str(std::string());
      label << "h_qqH" << mass << "_" << flavours.at(flavourIt);
      h_qqH[label.str()] -> Write();
      
      // signal histograms - fit
      TF1* fitFunc_scb;
      
      label.str(std::string());
      label << "h_ggH" << mass << "_" << flavours.at(flavourIt) << "_fit";
      RegularizeHistogram(h_ggH[label.str()]);
      FitHiggsMass(&fitFunc_scb,"fitFunc_scb",0.,1000.,h_ggH[label.str()],mass,"crystalBallLowHigh");
      h_ggH[label.str()] -> Write();
      
      label.str(std::string());
      label << "h_qqH" << mass << "_" << flavours.at(flavourIt) << "_fit";
      RegularizeHistogram(h_qqH[label.str()]);
      FitHiggsMass(&fitFunc_scb,"fitFunc_scb",0.,1000.,h_qqH[label.str()],mass,"crystalBallLowHigh");
      h_qqH[label.str()] -> Write();
      
      outFile -> cd(massChar);
    }
  
  
  
  return 0;
}




void RegularizeHistogram(TH1F* h)
{
  TH1F* h_clone = (TH1F*)( h->Clone("temp") );
  
  h -> Reset();
  for(int bin = 1; bin <= h->GetNbinsX(); ++bin)
  {
    float binContent = h_clone -> GetBinContent(bin);
    float binError   = h_clone -> GetBinError(bin);
    if( binContent/h_clone->Integral() > 0.0005 )
    {
      h -> SetBinContent(bin,binContent);
      h -> SetBinError(bin,binError);
    }
  }
  
  return;
}





float GetCut_lepMet_Dphi(const float& mH)
{
  if     ( mH == 200. ) return 2.5;
  else if( mH == 250. ) return 2.5;
  else if( mH == 300. ) return 2.5;
  else if( mH == 350. ) return 2.5;
  else if( mH == 400. ) return 2.;
  else if( mH == 450. ) return 2.;
  else if( mH == 500. ) return 2.;
  else if( mH == 550. ) return 1.5;
  else if( mH == 600. ) return 1.5;
  else return -1;
}

float GetCut_WJJ_Dphi(const float& mH)
{
  if     ( mH == 200. ) return 2.;
  else if( mH == 250. ) return 2.;
  else if( mH == 300. ) return 2.;
  else if( mH == 350. ) return 2.;
  else if( mH == 400. ) return 1.5;
  else if( mH == 450. ) return 1.5;
  else if( mH == 500. ) return 1.5;
  else if( mH == 550. ) return 1.5;
  else if( mH == 600. ) return 1.5;
  else return -1;
}
