#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "HiggsMassWindows.h"

#include <iostream>
#include <vector>
#include <map>

#include "TH1F.h"
#include "TGraph.h"
#include "TGraph2D.h"






int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> VBFAnalysis_optimizeHiggsMassWindow::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
  
  
  //[Input]
  std::string baseDir      = gConfigParser -> readStringOption("Input::baseDir");
  std::string jetAlgorithm = gConfigParser -> readStringOption("Input::jetAlgorithm");
  std::string jetType      = gConfigParser -> readStringOption("Input::jetType");
  float lumi = gConfigParser -> readFloatOption("Input::lumi");
  
  std::string inputFileName = gConfigParser -> readStringOption("Input::inputFileName");
  std::vector<std::string> inputSigDirs = gConfigParser -> readStringListOption("Input::inputSigDirs");
  std::vector<std::string> inputBkgDirs = gConfigParser -> readStringListOption("Input::inputBkgDirs");
  
  unsigned int nSigTrees = inputSigDirs.size();
  unsigned int nBkgTrees = inputBkgDirs.size();
  unsigned int nTotTrees = nSigTrees + nBkgTrees;
  
  std::vector<std::string> inputTotDirs;
  for(unsigned int i = 0; i < nSigTrees; ++ i) inputTotDirs.push_back(inputSigDirs.at(i));
  for(unsigned int i = 0; i < nBkgTrees; ++ i) inputTotDirs.push_back(inputBkgDirs.at(i));
  
  
  //[Output]
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  
  
  //[Options]
  int step = gConfigParser -> readIntOption("Options::step");
  
  
  
  // Define tree variables
  float mH;
  int totEvents;
  float eventWeight;
  float PUWeight;
  float crossSection;
  float lepNuW_m;
  float lepNuW_m_KF;
  
  
  
  // Define the output file
  std::string outputRootFullFileName = outputRootFilePath + "/" + outputRootFileName + "_" + jetAlgorithm + ".root";
  TFile* outFile = new TFile(outputRootFullFileName.c_str(), "RECREATE");
  
  // Define the output graph
  std::vector<int> masses = GetMasses();
  unsigned int nMasses = masses.size();
  
  std::map<int,TGraph*> g_significance;
  std::map<int,TGraph*> g_significance_KF;
  //std::map<int,TGraph2D*> g_asymmSignificance;
  //std::map<int,TGraph2D*> g_asymmSignificance_KF;
  for(unsigned int iMass = 0; iMass < nMasses; ++iMass)
  {
    int mass = masses[iMass];
    
    g_significance[mass] = new TGraph();
    g_significance_KF[mass] = new TGraph();
    //g_asymmSignificance[mass] = new TGraph2D();
    //g_asymmSignificance_KF[mass] = new TGraph2D();   
  }
  
  
  
  
  
  
  // Define counters
  // symmetric windows
  int nPoints = 100;
  std::map<int,std::vector<std::pair<float,float> > > S;
  std::map<int,std::vector<std::pair<float,float> > > B;
  std::map<int,std::vector<std::pair<float,float> > > S_KF;
  std::map<int,std::vector<std::pair<float,float> > > B_KF;
  
  for(unsigned int iMass = 0; iMass < nMasses; ++iMass)
  {
    int mass = masses[iMass];
    
    for(int point = 0; point < nPoints; ++point)
    {
      float window = 0. + point*250./nPoints;
      
      std::pair<float,float> dummy(window,0.);
      
      S[mass].push_back( dummy );
      B[mass].push_back( dummy );
      S_KF[mass].push_back( dummy );
      B_KF[mass].push_back( dummy );
    }
  }
  
  
  /*
  // Define counters
  // asymmetric windows
  int nPointsLow = 25;
  int nPointsHig = 25;
  std::map<int,std::vector<std::pair<std::pair<float,float>,float> > > asymmS;
  std::map<int,std::vector<std::pair<std::pair<float,float>,float> > > asymmB;
  std::map<int,std::vector<std::pair<std::pair<float,float>,float> > > asymmS_KF;
  std::map<int,std::vector<std::pair<std::pair<float,float>,float> > > asymmB_KF;
  
  for(unsigned int iMass = 0; iMass < nMasses; ++iMass)
  {
    int mass = masses[iMass];
    
    for(int pointLow = 0; pointLow < nPointsLow; ++pointLow)
      for(int pointHig = 0; pointHig < nPointsHig; ++pointHig)
      {
        float windowLow = 0. + pointLow*250./nPointsLow;
        float windowHig = 0. + pointHig*250./nPointsHig;
        
        std::pair<float,float> dummy(windowLow,windowHig);
        std::pair<std::pair<float,float>,float> dummy2(dummy,0.);
        
        asymmS[mass].push_back( dummy2 );
        asymmB[mass].push_back( dummy2 );
        asymmS_KF[mass].push_back( dummy2 );
        asymmB_KF[mass].push_back( dummy2 );
      }
  }
  */
  
  
  
  
  
  
  // count S and B events
  for(unsigned int i = 0; i < nTotTrees; ++i)
  {
    // open root file
    std::string inputFullFileName;
    if(i < nSigTrees) inputFullFileName = baseDir + "/" + inputSigDirs.at(i)   + "/" + inputFileName + ".root";
    else              inputFullFileName = baseDir + "/" + inputBkgDirs.at(i-nSigTrees) + "/" + inputFileName + ".root";
    TFile* inputFile = TFile::Open(inputFullFileName.c_str());
    
    if( i < nSigTrees)
    {
      std::cout << ">>> VBFAnalysis_optimizeHiggsMassWindow::signal tree in " << inputSigDirs.at(i) << " opened" << std::endl;    
    }
    else
    {
      std::cout << ">>> VBFAnalysis_optimizeHiggsMassWindow::background tree in " << inputBkgDirs.at(i-nSigTrees) << " opened" << std::endl;
    }
    
    
    
    // get the tree at nth step
    TTree* tree = NULL;
    char treeName[50];
    sprintf(treeName, "ntu_%d", step);
    inputFile -> GetObject(treeName, tree);
    if ( tree -> GetEntries() == 0 ) continue; 
    
    
    // set tree branches
    tree -> SetBranchAddress("mH",           &mH);
    tree -> SetBranchAddress("totEvents",    &totEvents);
    tree -> SetBranchAddress("eventWeight",  &eventWeight);
    tree -> SetBranchAddress("PUWeight",     &PUWeight);
    tree -> SetBranchAddress("crossSection", &crossSection);
    tree -> SetBranchAddress("lepNuW_m",     &lepNuW_m);
    tree -> SetBranchAddress("lepNuW_m_KF",  &lepNuW_m_KF);
    
    for(int entry = 0; entry < tree->GetEntries(); ++entry)
    {
      tree -> GetEntry(entry);
      if( entry%1000 == 0) std::cout << ">>>>>> reading entry " << entry << " of " << tree->GetEntries() << "\r" << std::flush;
      
      double weight = lumi * 1000 * 1. / totEvents * crossSection * eventWeight * PUWeight;
      
      
      
      //------------------
      // symmetric windows
      for(unsigned int point = 0; point < (S[200]).size(); ++point)
      {
	//std::cout << "point: " << point << std::endl;
        float window = ((S[200]).at(point)).first;
        
        
        // loop on the masses
        for(unsigned int iMass = 0; iMass < nMasses; ++iMass)
        {
          int mass = masses[iMass];
          
          if( (lepNuW_m >= mass-window) && (lepNuW_m < mass+window) )
          {
            if( (mH > 0) && (int(mH)%1000 == mass) ) 
              ((S[mass]).at(point)).second += weight;
            if( mH <= 0 )
              ((B[mass]).at(point)).second += weight;
          }
	  
          
          if( (lepNuW_m_KF >= mass-window) && (lepNuW_m_KF < mass+window) )
          {
            if( (mH > 0) && (int(mH)%1000 == mass) ) 
              ((S_KF[mass]).at(point)).second += weight;
            if( mH <= 0 )
              ((B_KF[mass]).at(point)).second += weight;
	  }
        }
      }
      
      
      /*
      //-------------------
      // asymmetric windows
      for(unsigned int point = 0; point < (asymmS[200]).size(); ++point)
      {
	//std::cout << "point: " << point << std::endl;
        float windowLow = (((asymmS[200]).at(point)).first).first;
        float windowHig = (((asymmS[200]).at(point)).first).second;
        
        
        // loop on the masses
        for(unsigned int iMass = 0; iMass < nMasses; ++iMass)
        {
          int mass = masses[iMass];
          char token[50];
          sprintf(token,"M-%d",mass);          
          
	  
          if( (lepNuW_m >= mass-windowLow) && (lepNuW_m < mass+windowHig) )
          {
            if( (mH > 0) && ( (token == token2) || (token == token3) ) ) 
              ((asymmS[mass]).at(point)).second += weight;
            if( mH <= 0 )
              ((asymmB[mass]).at(point)).second += weight;
          }
          
	  
          if( (lepNuW_m_KF >= mass-windowLow) && (lepNuW_m_KF < mass+windowHig) )
          {
            if( (mH > 0) && ( (token == token2) || (token == token3) ) ) 
              ((asymmS_KF[mass]).at(point)).second += weight;
            if( mH <= 0 )
              ((asymmB_KF[mass]).at(point)).second += weight;
          }
	}
      }*/
    }
  }
  
  
  
  
  
  
  outFile -> cd();
  
  for(unsigned int iMass = 0; iMass < nMasses; ++iMass)
  {
    int mass = masses[iMass];
    
    
    // symmetric windows
    for(unsigned int point = 0; point < (S[mass]).size(); ++point)
    {
      float window = ((S[mass]).at(point)).first;    
      float numS = ((S[mass]).at(point)).second;
      float numB = ((B[mass]).at(point)).second;
      float numS_KF = ((S_KF[mass]).at(point)).second;
      float numB_KF = ((B_KF[mass]).at(point)).second;
      
      if( numS+numB > 0 )
        g_significance[mass] -> SetPoint(point,window,numS/sqrt(numS+numB));
      else
        g_significance[mass] -> SetPoint(point,window,0.);
      
      if( numS_KF+numB_KF > 0 )
        g_significance_KF[mass] -> SetPoint(point,window,numS_KF/sqrt(numS_KF+numB_KF));
      else
        g_significance_KF[mass] -> SetPoint(point,window,0.);
    }
    
    std::stringstream graphName;
    graphName << "g_significance_" << mass; 
    g_significance[mass] -> Write(graphName.str().c_str());

    std::stringstream graphName_KF;
    graphName_KF << "g_significance_KF_" << mass; 
    g_significance_KF[mass] -> Write(graphName_KF.str().c_str());
    
    
    
    /*
    // asymmetric windows
    for(unsigned int point = 0; point < (asymmS[mass]).size(); ++point)
    {
      float windowLow = (((asymmS[mass]).at(point)).first).first;
      float windowHig = (((asymmS[mass]).at(point)).first).second;
      float numS = ((asymmS[mass]).at(point)).second;
      float numB = ((asymmB[mass]).at(point)).second;
      float numS_KF = ((asymmS_KF[mass]).at(point)).second;
      float numB_KF = ((asymmB_KF[mass]).at(point)).second;
      
      if( numS+numB > 0. )
        g_asymmSignificance[mass] -> SetPoint(point,windowLow,windowHig,numS/sqrt(numS+numB));
      else
        g_asymmSignificance[mass] -> SetPoint(point,windowLow,windowHig,0.);
      
      if( numS_KF+numB_KF > 0. )
        g_asymmSignificance_KF[mass] -> SetPoint(point,windowLow,windowHig,numS_KF/sqrt(numS_KF+numB_KF));
      else
        g_asymmSignificance_KF[mass] -> SetPoint(point,windowLow,windowHig,0.);
    }
    
    std::stringstream asymmGraphName;
    asymmGraphName << "g_asymmSignificance_" << mass; 
    g_asymmSignificance[mass] -> Write(asymmGraphName.str().c_str());

    std::stringstream asymmGraphName_KF;
    asymmGraphName_KF << "g_asymmSignificance_KF_" << mass; 
    g_asymmSignificance_KF[mass] -> Write(asymmGraphName_KF.str().c_str());
    */
  }
  
  outFile -> Close();
  
  
  return 0;
}
