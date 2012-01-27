#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "PUUtils.h"
#include "HiggsMassWindows.h"
#include "HiggsMassFits.h"

#include <iostream>
#include <iomanip>
#include <fstream>

#include "TH1F.h"
#include "TGraph.h"



void RegularizeHistogram(TH1F* h);



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
  std::vector<std::string> inputSigDirs = gConfigParser -> readStringListOption("Input::inputSigDirs");
  
  unsigned int nSigTrees = inputSigDirs.size();
  unsigned int nTotTrees = nSigTrees;
  
  std::vector<std::string> inputTotDirs;
  for(unsigned int i = 0; i < nSigTrees; ++ i) inputTotDirs.push_back(inputSigDirs.at(i));
  
  
  //[Output]
  std::string outputRootFilePath  = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName  = gConfigParser -> readStringOption("Output::outputRootFileName");
  std::string outputRootFileLabel = gConfigParser -> readStringOption("Output::outputRootFileLabel");

  
  //[Options]
  float xWidth = gConfigParser -> readFloatOption("Options::xWidth");
  char xWidthChar[50];
  sprintf(xWidthChar,"%d",int(xWidth));
  int step = gConfigParser -> readIntOption("Options::step");
  char stepChar[50];
  sprintf(stepChar,"%d",step);
  int PUScale = gConfigParser -> readIntOption("Options::PUScale");
  std::string pileupFileName = gConfigParser -> readStringOption("Options::pileupFileName");
  
  float xMin = 0.;
  float xMax = 1000.;
  int nBins = int((xMax-xMin)/xWidth);
  xMax = xMin + xWidth*nBins;
  
  
  // Define the output file
  outputRootFilePath += "/combine/binWidth" + std::string(xWidthChar) + "/step" + std::string(stepChar) + "/";
  std::string outputRootFullFileName;
  if( outputRootFileLabel == "none" )
    outputRootFullFileName = outputRootFilePath + outputRootFileName + ".root";
  else
    outputRootFullFileName = outputRootFilePath + outputRootFileName + "_" + outputRootFileLabel + ".root";
  TFile* outFile = new TFile(outputRootFullFileName.c_str(), "RECREATE");
  
  
  // Define tree variables
  float mH;
  int totEvents;
  int PUtrue_n;
  float eventWeight;
  float crossSection;
  float lepNuW_m_KF;
  int lep_flavour;
  int nJets_cnt_pt30;
  
  
  // get the data pileup distribution
  TFile* inFile_pileup = TFile::Open(pileupFileName.c_str(),"READ");
  TH1F* distrPU_DATA;
  if     ( PUScale ==  1 ) distrPU_DATA = (TH1F*)( inFile_pileup->Get("pileupUp") );
  else if( PUScale == -1 ) distrPU_DATA = (TH1F*)( inFile_pileup->Get("pileupDown") );
  else                     distrPU_DATA = (TH1F*)( inFile_pileup->Get("pileup") );
  distrPU_DATA -> Scale(1./distrPU_DATA->Integral());
  
  
  // Define the output graph
  std::vector<int> masses = GetMasses();
  unsigned int nMasses = masses.size();
  
  
  
  // Define counters
  std::map<int,float> S;
  std::map<int,float> ggS;
  std::map<int,float> qqS;
  
  // Define histograms
  std::map<int,TH1F*> h_S_emu;
  
  std::map<int,TH1F*> h_ggS_emu;
  std::map<int,TH1F*> h_qqS_emu;
  std::map<int,TH1F*> h_ggS_e;
  std::map<int,TH1F*> h_qqS_e;
  std::map<int,TH1F*> h_ggS_mu;
  std::map<int,TH1F*> h_qqS_mu;
  
  //std::map<int,TH1F*> h_ggS_emu_2j;
  //std::map<int,TH1F*> h_qqS_emu_2j;
  //std::map<int,TH1F*> h_ggS_emu_3j;
  //std::map<int,TH1F*> h_qqS_emu_3j;
  
  //std::map<int,TH1F*> h_ggS_e_2j;
  //std::map<int,TH1F*> h_qqS_e_2j;
  //std::map<int,TH1F*> h_ggS_mu_2j;
  //std::map<int,TH1F*> h_qqS_mu_2j;
  //std::map<int,TH1F*> h_ggS_e_3j;
  //std::map<int,TH1F*> h_qqS_e_3j;
  //std::map<int,TH1F*> h_ggS_mu_3j;
  //std::map<int,TH1F*> h_qqS_mu_3j;
  
  // Define histograms to fit
  std::map<int,TH1F*> h_S_toFit_emu;
  
  std::map<int,TH1F*> h_ggS_toFit_emu;
  std::map<int,TH1F*> h_qqS_toFit_emu;
  std::map<int,TH1F*> h_ggS_toFit_e;
  std::map<int,TH1F*> h_qqS_toFit_e;
  std::map<int,TH1F*> h_ggS_toFit_mu;
  std::map<int,TH1F*> h_qqS_toFit_mu;
  
  //std::map<int,TH1F*> h_ggS_emu_2j_toFit;
  //std::map<int,TH1F*> h_qqS_emu_2j_toFit;
  //std::map<int,TH1F*> h_ggS_emu_3j_toFit;
  //std::map<int,TH1F*> h_qqS_emu_3j_toFit;
  
  //std::map<int,TH1F*> h_ggS_e_2j_toFit;
  //std::map<int,TH1F*> h_qqS_e_2j_toFit;
  //std::map<int,TH1F*> h_ggS_mu_2j_toFit;
  //std::map<int,TH1F*> h_qqS_mu_2j_toFit;
  //std::map<int,TH1F*> h_ggS_e_3j_toFit;
  //std::map<int,TH1F*> h_qqS_e_3j_toFit;
  //std::map<int,TH1F*> h_ggS_mu_3j_toFit;
  //std::map<int,TH1F*> h_qqS_mu_3j_toFit;
  
  for(unsigned int iMass = 0; iMass < nMasses; ++iMass)
  {
    int mass = masses[iMass];
    char histoName[50];
    
    
    // build histograms
    sprintf(histoName,"h_H%d_emu",mass);
    h_S_emu[mass] = new TH1F(histoName,"",nBins,xMin,xMax);
    h_S_emu[mass] -> Sumw2();
     
    
    sprintf(histoName,"h_ggH%d_emu",mass);
    h_ggS_emu[mass] = new TH1F(histoName,"",nBins,xMin,xMax);
    h_ggS_emu[mass] -> Sumw2();
    
    sprintf(histoName,"h_qqH%d_emu",mass);
    h_qqS_emu[mass] = new TH1F(histoName,"",nBins,xMin,xMax);
    h_qqS_emu[mass] -> Sumw2();
    
    sprintf(histoName,"h_ggH%d_e",mass);
    h_ggS_e[mass] = new TH1F(histoName,"",nBins,xMin,xMax);
    h_ggS_e[mass] -> Sumw2();
    
    sprintf(histoName,"h_qqH%d_e",mass);
    h_qqS_e[mass] = new TH1F(histoName,"",nBins,xMin,xMax);
    h_qqS_e[mass] -> Sumw2();
    
    sprintf(histoName,"h_ggH%d_mu",mass);
    h_ggS_mu[mass] = new TH1F(histoName,"",nBins,xMin,xMax);
    h_ggS_mu[mass] -> Sumw2();
    
    sprintf(histoName,"h_qqH%d_mu",mass);
    h_qqS_mu[mass] = new TH1F(histoName,"",nBins,xMin,xMax);
    h_qqS_mu[mass] -> Sumw2();
    
    
    /*
    sprintf(histoName,"h_ggH%d_emu_2j",mass);
    h_ggS_emu_2j[mass] = new TH1F(histoName,"",nBins,xMin,xMax);
    h_ggS_emu_2j[mass] -> Sumw2();
    
    sprintf(histoName,"h_qqH%d_emu_2j",mass);
    h_qqS_emu_2j[mass] = new TH1F(histoName,"",nBins,xMin,xMax);
    h_qqS_emu_2j[mass] -> Sumw2();
    
    sprintf(histoName,"h_ggH%d_emu_3j",mass);
    h_ggS_emu_3j[mass] = new TH1F(histoName,"",nBins,xMin,xMax);
    h_ggS_emu_3j[mass] -> Sumw2();
    
    sprintf(histoName,"h_qqH%d_emu_3j",mass);
    h_qqS_emu_3j[mass] = new TH1F(histoName,"",nBins,xMin,xMax);
    h_qqS_emu_3j[mass] -> Sumw2();
    
    
    sprintf(histoName,"h_ggH%d_e_2j",mass);
    h_ggS_e_2j[mass] = new TH1F(histoName,"",nBins,xMin,xMax);
    h_ggS_e_2j[mass] -> Sumw2();
    
    sprintf(histoName,"h_qqH%d_e_2j",mass);
    h_qqS_e_2j[mass] = new TH1F(histoName,"",nBins,xMin,xMax);
    h_qqS_e_2j[mass] -> Sumw2();
    
    sprintf(histoName,"h_ggH%d_e_3j",mass);
    h_ggS_e_3j[mass] = new TH1F(histoName,"",nBins,xMin,xMax);
    h_ggS_e_3j[mass] -> Sumw2();
    
    sprintf(histoName,"h_qqH%d_e_3j",mass);
    h_qqS_e_3j[mass] = new TH1F(histoName,"",nBins,xMin,xMax);
    h_qqS_e_3j[mass] -> Sumw2();
     
    sprintf(histoName,"h_ggH%d_mu_2j",mass);
    h_ggS_mu_2j[mass] = new TH1F(histoName,"",nBins,xMin,xMax);
    h_ggS_mu_2j[mass] -> Sumw2();
    
    sprintf(histoName,"h_qqH%d_mu_2j",mass);
    h_qqS_mu_2j[mass] = new TH1F(histoName,"",nBins,xMin,xMax);
    h_qqS_mu_2j[mass] -> Sumw2();
    
    sprintf(histoName,"h_ggH%d_mu_3j",mass);
    h_ggS_mu_3j[mass] = new TH1F(histoName,"",nBins,xMin,xMax);
    h_ggS_mu_3j[mass] -> Sumw2();
    
    sprintf(histoName,"h_qqH%d_mu_3j",mass);
    h_qqS_mu_3j[mass] = new TH1F(histoName,"",nBins,xMin,xMax);
    h_qqS_mu_3j[mass] -> Sumw2();
    */
    
    
    
    // build histograms to fit
    sprintf(histoName,"h_H%d_toFit_emu",mass);
    h_S_toFit_emu[mass] = new TH1F(histoName,"",200,0.,1000.);
    h_S_toFit_emu[mass] -> Sumw2();
    
    
    sprintf(histoName,"h_ggH%d_toFit_emu",mass);
    h_ggS_toFit_emu[mass] = new TH1F(histoName,"",200,0.,1000.);
    h_ggS_toFit_emu[mass] -> Sumw2();
    
    sprintf(histoName,"h_qqH%d_toFit_emu",mass);
    h_qqS_toFit_emu[mass] = new TH1F(histoName,"",200,0.,1000.);
    h_qqS_toFit_emu[mass] -> Sumw2();
    
    sprintf(histoName,"h_ggH%d_toFit_e",mass);
    h_ggS_toFit_e[mass] = new TH1F(histoName,"",200,0.,1000.);
    h_ggS_toFit_e[mass] -> Sumw2();
    
    sprintf(histoName,"h_qqH%d_toFit_e",mass);
    h_qqS_toFit_e[mass] = new TH1F(histoName,"",200,0.,1000.);
    h_qqS_toFit_e[mass] -> Sumw2();
    
    sprintf(histoName,"h_ggH%d_toFit_mu",mass);
    h_ggS_toFit_mu[mass] = new TH1F(histoName,"",200,0.,1000.);
    h_ggS_toFit_mu[mass] -> Sumw2();
    
    sprintf(histoName,"h_qqH%d_toFit_mu",mass);
    h_qqS_toFit_mu[mass] = new TH1F(histoName,"",200,0.,1000.);
    h_qqS_toFit_mu[mass] -> Sumw2();
    
    
    /*
    sprintf(histoName,"h_ggH%d_toFit_emu_2j",mass);
    h_ggS_emu_2j_toFit[mass] = new TH1F(histoName,"",200,0.,1000.);
    h_ggS_emu_2j_toFit[mass] -> Sumw2();
    
    sprintf(histoName,"h_qqH%d_toFit_emu_2j",mass);
    h_qqS_emu_2j_toFit[mass] = new TH1F(histoName,"",200,0.,1000.);
    h_qqS_emu_2j_toFit[mass] -> Sumw2();
    
    sprintf(histoName,"h_ggH%d_toFit_emu_3j",mass);
    h_ggS_emu_3j_toFit[mass] = new TH1F(histoName,"",200,0.,1000.);
    h_ggS_emu_3j_toFit[mass] -> Sumw2();
    
    sprintf(histoName,"h_qqH%d_toFit_emu_3j",mass);
    h_qqS_emu_3j_toFit[mass] = new TH1F(histoName,"",200,0.,1000.);
    h_qqS_emu_3j_toFit[mass] -> Sumw2();
    
    
    sprintf(histoName,"h_ggH%d_toFit_e_2j",mass);
    h_ggS_e_2j_toFit[mass] = new TH1F(histoName,"",200,0.,1000.);
    h_ggS_e_2j_toFit[mass] -> Sumw2();
    
    sprintf(histoName,"h_qqH%d_toFit_e_2j",mass);
    h_qqS_e_2j_toFit[mass] = new TH1F(histoName,"",200,0.,1000.);
    h_qqS_e_2j_toFit[mass] -> Sumw2();
    
    sprintf(histoName,"h_ggH%d_toFit_e_3j",mass);
    h_ggS_e_3j_toFit[mass] = new TH1F(histoName,"",200,0.,1000.);
    h_ggS_e_3j_toFit[mass] -> Sumw2();
    
    sprintf(histoName,"h_qqH%d_toFit_e_3j",mass);
    h_qqS_e_3j_toFit[mass] = new TH1F(histoName,"",200,0.,1000.);
    h_qqS_e_3j_toFit[mass] -> Sumw2();
    
    sprintf(histoName,"h_ggH%d_toFit_mu_2j",mass);
    h_ggS_mu_2j_toFit[mass] = new TH1F(histoName,"",200,0.,1000.);
    h_ggS_mu_2j_toFit[mass] -> Sumw2();
    
    sprintf(histoName,"h_qqH%d_toFit_mu_2j",mass);
    h_qqS_mu_2j_toFit[mass] = new TH1F(histoName,"",200,0.,1000.);
    h_qqS_mu_2j_toFit[mass] -> Sumw2();
    
    sprintf(histoName,"h_ggH%d_toFit_mu_3j",mass);
    h_ggS_mu_3j_toFit[mass] = new TH1F(histoName,"",200,0.,1000.);
    h_ggS_mu_3j_toFit[mass] -> Sumw2();
    
    sprintf(histoName,"h_qqH%d_toFit_mu_3j",mass);
    h_qqS_mu_3j_toFit[mass] = new TH1F(histoName,"",200,0.,1000.);
    h_qqS_mu_3j_toFit[mass] -> Sumw2();
    */
  }
  
  
  
  // count S events
  for(unsigned int i = 0; i < nTotTrees; ++i)
  {
    // open root file
    std::string inputFullFileName;
    if(i < nSigTrees) inputFullFileName = inputDir + "/" + inputSigDirs.at(i)   + "/" + inputFileName + ".root";
    TFile* inputFile = TFile::Open(inputFullFileName.c_str());
    std::cout << ">>>>> VBFAnalysis_countSignalEvents::signal tree in " << inputSigDirs.at(i) << " opened" << std::endl;    
    
    
    
    // get the tree at nth step
    TTree* tree = NULL;
    char treeName[50];
    sprintf(treeName, "ntu_%d", step);
    inputFile -> GetObject(treeName, tree);
    if ( tree -> GetEntries() == 0 ) continue; 
    
    
    
    // get the mc pileup distribution
    TH1F* distrPU_MC = (TH1F*)( inputFile->Get("nPUtrue") );
    distrPU_MC -> Scale(1./distrPU_MC->Integral());
    
    
    
    // set tree branches
    tree -> SetBranchAddress("mH",             &mH);
    tree -> SetBranchAddress("eventWeight",    &eventWeight);
    tree -> SetBranchAddress("PUtrue_n",       &PUtrue_n);
    tree -> SetBranchAddress("totEvents",      &totEvents);
    tree -> SetBranchAddress("crossSection",   &crossSection);
    tree -> SetBranchAddress("lepNuW_m_KF",    &lepNuW_m_KF);
    tree -> SetBranchAddress("lep_flavour",    &lep_flavour);
    tree -> SetBranchAddress("nJets_cnt_pt30", &nJets_cnt_pt30);
    
    
    // fill counters and histograms
    for(int entry = 0; entry < tree->GetEntries(); ++entry)
    {
      tree -> GetEntry(entry);
      double weight = lumi * 1000 * 1. / totEvents * crossSection * PURescaleFactor(distrPU_DATA,distrPU_MC,PUtrue_n) * eventWeight;
      
      int mass = int(int(mH)%1000);
      
      // count events
      if( (lepNuW_m_KF >= GetLepNuWMMIN(mass)) && (lepNuW_m_KF < GetLepNuWMMAX(mass)) )
      {
        S[mass] += weight;
        if( mH < 1000 ) ggS[mass] += weight;
        else            qqS[mass] += weight;
      }
      
      
      // fill histograms
      h_S_emu[mass]       -> Fill(lepNuW_m_KF,weight);
      h_S_toFit_emu[mass] -> Fill(lepNuW_m_KF,weight);
      
      if( mH < 1000 ) h_ggS_emu[mass] -> Fill(lepNuW_m_KF,weight);
      else            h_qqS_emu[mass] -> Fill(lepNuW_m_KF,weight);
      if( mH < 1000 ) h_ggS_toFit_emu[mass] -> Fill(lepNuW_m_KF,weight);
      else            h_qqS_toFit_emu[mass] -> Fill(lepNuW_m_KF,weight);
      
      // electrons
      if( lep_flavour == 11 )
      {
        if( mH < 1000 ) h_ggS_e[mass] -> Fill(lepNuW_m_KF,weight);
        else            h_qqS_e[mass] -> Fill(lepNuW_m_KF,weight);
        if( mH < 1000 ) h_ggS_toFit_e[mass] -> Fill(lepNuW_m_KF,weight);
        else            h_qqS_toFit_e[mass] -> Fill(lepNuW_m_KF,weight);
        
        /*
        if( nJets_cnt_pt30 == 2 )
        {
          if( mH < 1000 ) h_ggS_e_2j[mass] -> Fill(lepNuW_m_KF,weight);
          else            h_qqS_e_2j[mass] -> Fill(lepNuW_m_KF,weight);        
          if( mH < 1000 ) h_ggS_e_2j_toFit[mass] -> Fill(lepNuW_m_KF,weight);
          else            h_qqS_e_2j_toFit[mass] -> Fill(lepNuW_m_KF,weight);        
        }
        
        if( nJets_cnt_pt30 == 3 )
        {
          if( mH < 1000 ) h_ggS_e_3j[mass] -> Fill(lepNuW_m_KF,weight);
          else            h_qqS_e_3j[mass] -> Fill(lepNuW_m_KF,weight);        
          if( mH < 1000 ) h_ggS_e_3j_toFit[mass] -> Fill(lepNuW_m_KF,weight);
          else            h_qqS_e_3j_toFit[mass] -> Fill(lepNuW_m_KF,weight);        
        }
        */
      }
      
      // muons
      if( lep_flavour == 13 )
      {
        if( mH < 1000 ) h_ggS_mu[mass] -> Fill(lepNuW_m_KF,weight);
        else            h_qqS_mu[mass] -> Fill(lepNuW_m_KF,weight);
        if( mH < 1000 ) h_ggS_toFit_mu[mass] -> Fill(lepNuW_m_KF,weight);
        else            h_qqS_toFit_mu[mass] -> Fill(lepNuW_m_KF,weight);
        
        /*
        if( nJets_cnt_pt30 == 2 )
        {
          if( mH < 1000 ) h_ggS_mu_2j[mass] -> Fill(lepNuW_m_KF,weight);
          else            h_qqS_mu_2j[mass] -> Fill(lepNuW_m_KF,weight);        
          if( mH < 1000 ) h_ggS_mu_2j_toFit[mass] -> Fill(lepNuW_m_KF,weight);
          else            h_qqS_mu_2j_toFit[mass] -> Fill(lepNuW_m_KF,weight);        
        }
        
        if( nJets_cnt_pt30 == 3 )
        {
          if( mH < 1000 ) h_ggS_mu_3j[mass] -> Fill(lepNuW_m_KF,weight);
          else            h_qqS_mu_3j[mass] -> Fill(lepNuW_m_KF,weight);        
          if( mH < 1000 ) h_ggS_mu_3j_toFit[mass] -> Fill(lepNuW_m_KF,weight);
          else            h_qqS_mu_3j_toFit[mass] -> Fill(lepNuW_m_KF,weight);        
        }
        */
      }
      
      
      /*
      // 2 jets
      if( nJets_cnt_pt30 == 2 )
      {
        if( mH < 1000 ) h_ggS_emu_2j[mass] -> Fill(lepNuW_m_KF,weight);
        else            h_qqS_emu_2j[mass] -> Fill(lepNuW_m_KF,weight);
        if( mH < 1000 ) h_ggS_emu_2j_toFit[mass] -> Fill(lepNuW_m_KF,weight);
        else            h_qqS_emu_2j_toFit[mass] -> Fill(lepNuW_m_KF,weight);
      }
      
      // 3 jets
      if( nJets_cnt_pt30 == 3 )
      {
        if( mH < 1000 ) h_ggS_emu_3j[mass] -> Fill(lepNuW_m_KF,weight);
        else            h_qqS_emu_3j[mass] -> Fill(lepNuW_m_KF,weight);
        if( mH < 1000 ) h_ggS_emu_3j_toFit[mass] -> Fill(lepNuW_m_KF,weight);
        else            h_qqS_emu_3j_toFit[mass] -> Fill(lepNuW_m_KF,weight);
      }
      */
    }

  inputFile -> Close(); 

  }
  
  
  
  // gluon-gluon fusion
  std::cout << ">>>>>> RESULTS FOR ggH <<<<<<" << std::endl;
  for(unsigned int iMass = 0; iMass < nMasses; ++iMass)
  {
    int mass = masses[iMass];
    
    std::cout << ">>> Higgs mass: " << mass
              << "      ggS: "      << std::fixed << std::setprecision(3) << std::setw(7) << ggS[mass]
              << std::endl;
  }
  
  // vector boson fusion
  std::cout << ">>>>>> RESULTS FOR qqH <<<<<<" << std::endl;
  for(unsigned int iMass = 0; iMass < nMasses; ++iMass)
  {
    int mass = masses[iMass];
    
    std::cout << ">>> Higgs mass: " << mass
              << "      qqS: "      << std::fixed << std::setprecision(3) << std::setw(7) << qqS[mass]
              << std::endl;
  }
  
  // gluon-gluon fusion + vector boson fusion
  std::cout << ">>>>>> RESULTS FOR ggH+qqH <<<<<<" << std::endl;
  for(unsigned int iMass = 0; iMass < nMasses; ++iMass)
  {
    int mass = masses[iMass];
    
    std::cout << ">>> Higgs mass: " << mass
              << "        S: "      << std::fixed << std::setprecision(3) << std::setw(7) << S[mass]
              << std::endl;
  }
  
  
  
  // save histograms
  outFile -> cd();
  outFile -> mkdir("noFit");
  
  outFile -> cd();
  outFile -> mkdir("toFit");
  
  for(unsigned int iMass = 0; iMass < nMasses; ++iMass)
  {
    outFile -> cd("noFit");

    int mass = masses[iMass];

    h_S_emu[mass]   -> Write();
    
    h_ggS_emu[mass] -> Write();
    h_qqS_emu[mass] -> Write();
    h_ggS_e[mass] -> Write();
    h_qqS_e[mass] -> Write();
    h_ggS_mu[mass] -> Write();
    h_qqS_mu[mass] -> Write();
    
    //h_ggS_emu_2j[mass] -> Write();
    //h_qqS_emu_2j[mass] -> Write();
    //h_ggS_emu_3j[mass] -> Write();
    //h_qqS_emu_3j[mass] -> Write();
    
    //h_ggS_e_2j[mass] -> Write();
    //h_qqS_e_2j[mass] -> Write();
    //h_ggS_e_3j[mass] -> Write();
    //h_qqS_e_3j[mass] -> Write();
    //h_ggS_mu_2j[mass] -> Write();
    //h_qqS_mu_2j[mass] -> Write();
    //h_ggS_mu_3j[mass] -> Write();
    //h_qqS_mu_3j[mass] -> Write();
    
    outFile -> cd();
    
    
    
    outFile -> cd("toFit");
    
    TF1* fitFunc_scb;
    //TF1* fitFunc_sdgcc;
    
    std::cout << ">>> VBFAnalysis_countSignalEvents::fitting mass " << mass << std::endl;
    
    RegularizeHistogram(h_S_toFit_emu[mass]);
    FitHiggsMass(&fitFunc_scb,"fitFunc_scb",0.,1000.,h_S_toFit_emu[mass],mass,"crystalBallLowHigh");
    h_S_toFit_emu[mass] -> Write();
    
    
    RegularizeHistogram(h_ggS_toFit_emu[mass]);
    FitHiggsMass(&fitFunc_scb,"fitFunc_scb",0.,1000.,h_ggS_toFit_emu[mass],mass,"crystalBallLowHigh");
    h_ggS_toFit_emu[mass] -> Write();
    
    RegularizeHistogram(h_qqS_toFit_emu[mass]);
    FitHiggsMass(&fitFunc_scb,"fitFunc_scb",0.,1000.,h_qqS_toFit_emu[mass],mass,"crystalBallLowHigh");
    h_qqS_toFit_emu[mass] -> Write();
    
    RegularizeHistogram(h_ggS_toFit_e[mass]);
    FitHiggsMass(&fitFunc_scb,"fitFunc_scb",0.,1000.,h_ggS_toFit_e[mass],mass,"crystalBallLowHigh");
    h_ggS_toFit_e[mass] -> Write();
    
    RegularizeHistogram(h_qqS_toFit_e[mass]);
    FitHiggsMass(&fitFunc_scb,"fitFunc_scb",0.,1000.,h_qqS_toFit_e[mass],mass,"crystalBallLowHigh");
    h_qqS_toFit_e[mass] -> Write();
    
    RegularizeHistogram(h_ggS_toFit_mu[mass]);
    FitHiggsMass(&fitFunc_scb,"fitFunc_scb",0.,1000.,h_ggS_toFit_mu[mass],mass,"crystalBallLowHigh");
    h_ggS_toFit_mu[mass] -> Write();
    
    RegularizeHistogram(h_qqS_toFit_mu[mass]);
    FitHiggsMass(&fitFunc_scb,"fitFunc_scb",0.,1000.,h_qqS_toFit_mu[mass],mass,"crystalBallLowHigh");
    h_qqS_toFit_mu[mass] -> Write();
    
    
    //RegularizeHistogram(h_ggS_emu_2j_toFit[mass]);
    //FitHiggsMass(&fitFunc_scb,"fitFunc_scb",0.,1000.,h_ggS_emu_2j_toFit[mass],mass,"crystalBallLowHigh");
    //h_ggS_emu_2j_toFit[mass] -> Write();
    
    //RegularizeHistogram(h_qqS_emu_2j_toFit[mass]);
    //FitHiggsMass(&fitFunc_scb,"fitFunc_scb",0.,1000.,h_qqS_emu_2j_toFit[mass],mass,"crystalBallLowHigh");
    //h_qqS_emu_2j_toFit[mass] -> Write();
    
    //RegularizeHistogram(h_ggS_emu_3j_toFit[mass]);
    //FitHiggsMass(&fitFunc_scb,"fitFunc_scb",0.,1000.,h_ggS_emu_3j_toFit[mass],mass,"crystalBallLowHigh");
    //h_ggS_emu_3j_toFit[mass] -> Write();
    
    //RegularizeHistogram(h_qqS_emu_3j_toFit[mass]);
    //FitHiggsMass(&fitFunc_scb,"fitFunc_scb",0.,1000.,h_qqS_emu_3j_toFit[mass],mass,"crystalBallLowHigh");
    //h_qqS_emu_3j_toFit[mass] -> Write();
    
    
    //RegularizeHistogram(h_ggS_e_2j_toFit[mass]);
    //FitHiggsMass(&fitFunc_scb,"fitFunc_scb",0.,1000.,h_ggS_e_2j_toFit[mass],mass,"crystalBallLowHigh");    
    //h_ggS_e_2j_toFit[mass] -> Write();
    
    //RegularizeHistogram(h_qqS_e_2j_toFit[mass]);
    //FitHiggsMass(&fitFunc_scb,"fitFunc_scb",0.,1000.,h_qqS_e_2j_toFit[mass],mass,"crystalBallLowHigh");    
    //h_qqS_e_2j_toFit[mass] -> Write();
    
    //RegularizeHistogram(h_ggS_e_3j_toFit[mass]);
    //FitHiggsMass(&fitFunc_scb,"fitFunc_scb",0.,1000.,h_ggS_e_3j_toFit[mass],mass,"crystalBallLowHigh");    
    //h_ggS_e_3j_toFit[mass] -> Write();
    
    //RegularizeHistogram(h_qqS_e_3j_toFit[mass]);
    //FitHiggsMass(&fitFunc_scb,"fitFunc_scb",0.,1000.,h_qqS_e_3j_toFit[mass],mass,"crystalBallLowHigh");    
    //h_qqS_e_3j_toFit[mass] -> Write();
    
    //RegularizeHistogram(h_ggS_mu_2j_toFit[mass]);
    //FitHiggsMass(&fitFunc_scb,"fitFunc_scb",0.,1000.,h_ggS_mu_2j_toFit[mass],mass,"crystalBallLowHigh");    
    //h_ggS_mu_2j_toFit[mass] -> Write();
    
    //RegularizeHistogram(h_qqS_mu_2j_toFit[mass]);
    //FitHiggsMass(&fitFunc_scb,"fitFunc_scb",0.,1000.,h_qqS_mu_2j_toFit[mass],mass,"crystalBallLowHigh");    
    //h_qqS_mu_2j_toFit[mass] -> Write();
    
    //RegularizeHistogram(h_ggS_mu_3j_toFit[mass]);
    //FitHiggsMass(&fitFunc_scb,"fitFunc_scb",0.,1000.,h_ggS_mu_3j_toFit[mass],mass,"crystalBallLowHigh");    
    //h_ggS_mu_3j_toFit[mass] -> Write();
    
    //RegularizeHistogram(h_qqS_mu_3j_toFit[mass]);
    //FitHiggsMass(&fitFunc_scb,"fitFunc_scb",0.,1000.,h_qqS_mu_3j_toFit[mass],mass,"crystalBallLowHigh");    
    //h_qqS_mu_3j_toFit[mass] -> Write();
    
    outFile -> cd();
  }
  
  outFile -> Close();
 
  
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
