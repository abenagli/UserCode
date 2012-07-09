#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "PUUtils.h"
#include "HiggsMassWindows.h"
#include "HiggsMassFits.h"
#include "EfficiencyCorrector.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <map>

#include "TH1F.h"



void RegularizeHistogram(TH1F* h);

float GetCut_lepMet_Dphi(const float& mH);
float GetCut_WJJ_Dphi(const float& mH);

bool CheckPath(const std::string& s1, const std::string& s2);





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
  float lumiEle = gConfigParser -> readFloatOption("Input::lumiEle");
  float lumiMu  = gConfigParser -> readFloatOption("Input::lumiMu");
  
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
  flavours.push_back("e");
  flavours.push_back("mu");
  unsigned int nFlavours = flavours.size();
  
  std::vector<std::string> charges;
  charges.push_back("");
  charges.push_back("p");
  charges.push_back("m");
  unsigned int nCharges = charges.size();
  
  std::stringstream label;
  
  
  
  std::map<std::string,TH1F*> h_data;
  std::map<std::string,TH1F*> h_H;
  //std::map<std::string,TH1F*> h_qqH;
  std::map<std::string,TH1F*> h_ggH;
  
  for(unsigned int massIt = 0; massIt < nMasses; ++massIt)
    for(unsigned int flavourIt = 0; flavourIt < nFlavours; ++flavourIt)
      for(unsigned int chargeIt = 0; chargeIt < nCharges; ++chargeIt)
      {
        int mass = masses.at(massIt);
        
        // data histograms
        if( onData == 1 )
        {
          label.str(std::string());
          if( charges.at(chargeIt) == "" )
            label << "h_data" << mass << "_" << flavours.at(flavourIt);
          else
	    label << "h_data" << mass << "_" << flavours.at(flavourIt) << "_" << charges.at(chargeIt);
          h_data[label.str()] = new TH1F(label.str().c_str(),"",nBins,xMin,xMax);
          h_data[label.str()] -> Sumw2();
        }
        
        // signal histograms
        label.str(std::string());
        if( charges.at(chargeIt) == "" )
          label << "h_H" << mass << "_" << flavours.at(flavourIt);
        else
          label << "h_H" << mass << "_" << flavours.at(flavourIt) << "_" << charges.at(chargeIt);
        h_H[label.str()] = new TH1F(label.str().c_str(),"",nBins,xMin,xMax);
        h_H[label.str()] -> Sumw2();
        
        label.str(std::string());
        if( charges.at(chargeIt) == "" )
          label << "h_ggH" << mass << "_" << flavours.at(flavourIt);
        else
          label << "h_ggH" << mass << "_" << flavours.at(flavourIt) << "_" << charges.at(chargeIt);
        h_ggH[label.str()] = new TH1F(label.str().c_str(),"",nBins,xMin,xMax);
        h_ggH[label.str()] -> Sumw2();
        
        label.str(std::string());
        //if( charges.at(chargeIt) == "" )
        //  label << "h_qqH" << mass << "_" << flavours.at(flavourIt);
        //else
        //  label << "h_qqH" << mass << "_" << flavours.at(flavourIt) << "_" << charges.at(chargeIt);
        //h_qqH[label.str()] = new TH1F(label.str().c_str(),"",nBins,xMin,xMax);
        //h_qqH[label.str()] -> Sumw2();
                
        // signal histograms - fit
        float xWidthTemp = 15.;
        if( mass >= 250. ) xWidthTemp = 20.;
        if( mass >= 300. ) xWidthTemp = 30.;
        if( mass >= 400. ) xWidthTemp = 40.;
        if( mass >= 500. ) xWidthTemp = 60.;
//        if( mass >= 600. ) xWidthTemp = 50.;
        
        label.str(std::string());
        if( charges.at(chargeIt) == "" )
          label << "h_ggH" << mass << "_" << flavours.at(flavourIt) << "_fit";
        else
          label << "h_ggH" << mass << "_" << flavours.at(flavourIt) << "_" << charges.at(chargeIt) << "_fit";
        h_ggH[label.str()] = new TH1F(label.str().c_str(),"",int((xMax-xMin)/xWidthTemp),xMin,xMax);
        h_ggH[label.str()] -> Sumw2();
                
        label.str(std::string());
        //if( charges.at(chargeIt) == "" )
        //  label << "h_qqH" << mass << "_" << flavours.at(flavourIt) << "_fit";
        //else
        //  label << "h_qqH" << mass << "_" << flavours.at(flavourIt) << "_" << charges.at(chargeIt) << "_fit";
        //h_qqH[label.str()] = new TH1F(label.str().c_str(),"",int((xMax-xMin)/xWidthTemp),xMin,xMax);
        //h_qqH[label.str()] -> Sumw2();  
      }
  
  
  
  
  
  
  //----------------
  // Fill Histograms
  
  
  // Define tree variables
  float mH;
  int dataFlag;
  int totEvents;
  int period;
  int PUtrue_n;
  float eventWeight;
  float leptonWeight;
  float metWeight;
  float crossSection;
  float WJJ_m;
  float lepNuW_m_KF;
  float lepMet_mt;
  float lepMet_Dphi;
  float WJJ_Dphi;
  float lep_ctheta;
  float WJ1_ctheta;
  float lepNu_ctheta;
  float lep_pt;
  float lep_eta;
  int lep_flavour;
  float lep_charge;
  float lep_tkIso;
  float lep_emIso;
  float lep_hadIso;
  //std::vector<std::string>* p_HLT_Names = new std::vector<std::string>;
  //std::vector<float>* p_HLT_Accept = new std::vector<float>;

  
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
    tree -> SetBranchStatus("*",0);
    tree -> SetBranchStatus("mH",1);
    tree -> SetBranchStatus("dataFlag",1);
    tree -> SetBranchStatus("period",1);
    tree -> SetBranchStatus("eventWeight",1);
    tree -> SetBranchStatus("leptonWeight",1);
    tree -> SetBranchStatus("metWeight",1);
    tree -> SetBranchStatus("PUtrue_n",1);
    tree -> SetBranchStatus("totEvents",1);
    tree -> SetBranchStatus("crossSection",1);
    tree -> SetBranchStatus("WJJ_m",1);
    tree -> SetBranchStatus("lepMet_Dphi",1);
    tree -> SetBranchStatus("lepMet_mt",1);
    tree -> SetBranchStatus("WJJ_Dphi",1);
    tree -> SetBranchStatus("lep_ctheta",1);
    tree -> SetBranchStatus("WJ1_ctheta",1);
    tree -> SetBranchStatus("lepNu_ctheta",1);
    tree -> SetBranchStatus("lepNuW_m_KF",1);
    tree -> SetBranchStatus("lep_pt",1);
    tree -> SetBranchStatus("lep_eta",1);
    tree -> SetBranchStatus("lep_flavour",1);
    tree -> SetBranchStatus("lep_charge",1);
    
    tree -> SetBranchAddress("mH",            &mH);
    tree -> SetBranchAddress("dataFlag",      &dataFlag);
    tree -> SetBranchAddress("period",        &period);
    tree -> SetBranchAddress("eventWeight",   &eventWeight);
    tree -> SetBranchAddress("leptonWeight",  &leptonWeight);
    tree -> SetBranchAddress("metWeight",     &metWeight);
    tree -> SetBranchAddress("PUtrue_n",      &PUtrue_n);
    tree -> SetBranchAddress("totEvents",     &totEvents);
    tree -> SetBranchAddress("crossSection",  &crossSection);
    tree -> SetBranchAddress("WJJ_m",         &WJJ_m);
    tree -> SetBranchAddress("lepMet_Dphi",   &lepMet_Dphi);
    tree -> SetBranchAddress("lepMet_mt",     &lepMet_mt);
    tree -> SetBranchAddress("WJJ_Dphi",      &WJJ_Dphi);
    tree -> SetBranchAddress("lep_ctheta",    &lep_ctheta);
    tree -> SetBranchAddress("WJ1_ctheta",    &WJ1_ctheta);
    tree -> SetBranchAddress("lepNu_ctheta",  &lepNu_ctheta);
    tree -> SetBranchAddress("lepNuW_m_KF",   &lepNuW_m_KF);
    tree -> SetBranchAddress("lep_pt",        &lep_pt);
    tree -> SetBranchAddress("lep_eta",       &lep_eta);
    tree -> SetBranchAddress("lep_flavour",   &lep_flavour);
    tree -> SetBranchAddress("lep_charge",    &lep_charge);
    
    
    
    // fill counters and histograms
    for(int entry = 0; entry < tree->GetEntries(); ++entry)
    {
      tree -> GetEntry(entry);
      
      double weight, lumi;
      if ( lep_flavour == 11 ) lumi = lumiEle;
      if ( lep_flavour == 13 ) lumi = lumiMu;
      if( i < nSigTrees ) weight = lumi * 1000 * 1. / totEvents * crossSection * PURescaleFactor(distrPU_DATA,distrPU_MC,PUtrue_n) * eventWeight;
      else                weight = 1.;
          
      
      // non mass-dependent cuts
      if( (WJJ_m < 65.) || (WJJ_m >= 95.) ) continue;
      //if( fabs(lep_ctheta) > 0.85 )   continue; // theta1
      //if( WJ1_ctheta > 0.60 )         continue; // theta2
      //if( fabs(lepNu_ctheta) > 0.65 ) continue; // theta*
      
      
      
      //-------
      // signal
      
      if( i < nSigTrees )
      {
        // count events
        int mass = int(int(mH)%1000);
        
        
        // mass-dependent cuts
        //if( lepMet_Dphi > GetCut_lepMet_Dphi(mass) ) continue;
        //if( WJJ_Dphi > GetCut_WJJ_Dphi(mass) ) continue;
        
        
        for(unsigned int flavourIt = 0; flavourIt < nFlavours; ++flavourIt)
          for(unsigned int chargeIt = 0; chargeIt < nCharges; ++chargeIt)
          {
            label.str(std::string());
	    if( charges.at(chargeIt) == "" )
              label << "h_H" << mass << "_" << flavours.at(flavourIt);
	    else
              label << "h_H" << mass << "_" << flavours.at(flavourIt) << "_" << charges.at(chargeIt);
            
            if( (flavours.at(flavourIt) == "emu") ||
                ( (flavours.at(flavourIt) == "e")  && (lep_flavour == 11) ) ||
                ( (flavours.at(flavourIt) == "mu") && (lep_flavour == 13) ) )
              if( (charges.at(chargeIt) == "") ||
                  ( (charges.at(chargeIt) == "p") && (lep_charge > 0) ) || 
                  ( (charges.at(chargeIt) == "m") && (lep_charge < 0) ) ) 
              {
                h_H[label.str()] -> Fill(lepNuW_m_KF,weight);
              }
          }
        
        
        // ggH
	for(unsigned int flavourIt = 0; flavourIt < nFlavours; ++flavourIt)
	  for(unsigned int chargeIt = 0; chargeIt < nCharges; ++chargeIt)
	  {
	    label.str(std::string());
	    if( charges.at(chargeIt) == "" )
	      label << "h_ggH" << mass << "_" << flavours.at(flavourIt);
	    else
	      label << "h_ggH" << mass << "_" << flavours.at(flavourIt) << "_" << charges.at(chargeIt);
	    
	    if( (flavours.at(flavourIt) == "emu") ||
		( (flavours.at(flavourIt) == "e")  && (lep_flavour == 11) ) ||
		( (flavours.at(flavourIt) == "mu") && (lep_flavour == 13) ) )
	      if( (charges.at(chargeIt) == "") ||
		  ( (charges.at(chargeIt) == "p") && (lep_charge > 0) ) || 
		  ( (charges.at(chargeIt) == "m") && (lep_charge < 0) ) ) 
	      {
		h_ggH[label.str()]        -> Fill(lepNuW_m_KF,weight);
		h_ggH[label.str()+"_fit"] -> Fill(lepNuW_m_KF,weight);
	      }
	  }
      }
        
        // qqH
        /*else
        {
          for(unsigned int flavourIt = 0; flavourIt < nFlavours; ++flavourIt)
            for(unsigned int chargeIt = 0; chargeIt < nCharges; ++chargeIt)
            {
              label.str(std::string());
              if( charges.at(chargeIt) == "" )
                label << "h_qqH" << mass << "_" << flavours.at(flavourIt);
               else
                label << "h_qqH" << mass << "_" << flavours.at(flavourIt) << "_" << charges.at(chargeIt);
              
              if( (flavours.at(flavourIt) == "emu") ||
                  ( (flavours.at(flavourIt) == "e")  && (lep_flavour == 11) ) ||
                  ( (flavours.at(flavourIt) == "mu") && (lep_flavour == 13) ) )
                if( (charges.at(chargeIt) == "") ||
                    ( (charges.at(chargeIt) == "p") && (lep_charge > 0) ) || 
                    ( (charges.at(chargeIt) == "m") && (lep_charge < 0) ) ) 
                {
                  h_qqH[label.str()]        -> Fill(lepNuW_m_KF,weight);
                  h_qqH[label.str()+"_fit"] -> Fill(lepNuW_m_KF,weight);
                }
	          }
        }*/
      
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
            for(unsigned int chargeIt = 0; chargeIt < nCharges; ++chargeIt)
            {
              label.str(std::string());
              if( charges.at(chargeIt) == "" )
                label << "h_data" << mass << "_" << flavours.at(flavourIt);
              else
                label << "h_data" << mass << "_" << flavours.at(flavourIt) << "_" << charges.at(chargeIt);
              
              if( (flavours.at(flavourIt) == "emu") ||
                  ( (flavours.at(flavourIt) == "e")  && (lep_flavour == 11) ) ||
                  ( (flavours.at(flavourIt) == "mu") && (lep_flavour == 13) ) )
                if( (charges.at(chargeIt) == "") ||
                    ( (charges.at(chargeIt) == "p") && (lep_charge > 0) ) ||
                    ( (charges.at(chargeIt) == "m") && (lep_charge < 0) ) )
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
      for(unsigned int chargeIt = 0; chargeIt < nCharges; ++chargeIt)
      {
        int mass = masses.at(massIt);
        char massChar[50];
        sprintf(massChar,"H%d",mass);
        
        outFile -> cd(massChar);
        
        // data histograms
        if( onData == 1 )
        {
          label.str(std::string());
	  if( charges.at(chargeIt) == "" )
            label << masses.at(massIt) << "_" << flavours.at(flavourIt);
	  else
            label << masses.at(massIt) << "_" << flavours.at(flavourIt) << "_" << charges.at(chargeIt);
          h_data["h_data"+label.str()] -> Write();
          
          TF1* fitFunc_attenuatedPol5order;
          TF1* fitFunc_attenuatedExpPol3order;
          TF1* fitFunc_attenuatedDoubleExponential;
          TF1* fitFunc_attenuatedPowerLaw;
          
          // NOTALEO
          FitData(&fitFunc_attenuatedPol5order,"fitFunc_"+label.str()+"_attenuatedPol5order",
                  h_data["h_data"+label.str()],mass,step,flavours.at(flavourIt),additionalCuts,"attenuatedExpPol3order",false,180.,800.);
          
          FitData(&fitFunc_attenuatedExpPol3order,"fitFunc_"+label.str()+"_attenuatedExpPol3order",
                  h_data["h_data"+label.str()],mass,step,flavours.at(flavourIt),additionalCuts,"attenuatedExpPol3order",false,180.,800.);
          
          FitData(&fitFunc_attenuatedDoubleExponential,"fitFunc_"+label.str()+"_attenuatedDoubleExponential",
                  h_data["h_data"+label.str()],mass,step,flavours.at(flavourIt),additionalCuts,"attenuatedDoubleExponential",false,180.,800.);
          
          FitData(&fitFunc_attenuatedPowerLaw,"fitFunc_"+label.str()+"_attenuatedPowerLaw",
                  h_data["h_data"+label.str()],mass,step,flavours.at(flavourIt),additionalCuts,"attenuatedPowerLaw",false,180.,800.);
          
          fitFunc_attenuatedPol5order         -> Write();
          fitFunc_attenuatedExpPol3order      -> Write();
          fitFunc_attenuatedDoubleExponential -> Write();
          fitFunc_attenuatedPowerLaw          -> Write();
        }
        
        // signal histograms
        label.str(std::string());
        if( charges.at(chargeIt) == "" )
          label << "h_H" << mass << "_" << flavours.at(flavourIt);
        else
          label << "h_H" << mass << "_" << flavours.at(flavourIt) << "_" << charges.at(chargeIt);
        h_H[label.str()] -> Write();
        
        label.str(std::string());
        if( charges.at(chargeIt) == "" )
          label << "h_ggH" << mass << "_" << flavours.at(flavourIt);
        else
          label << "h_ggH" << mass << "_" << flavours.at(flavourIt) << "_" << charges.at(chargeIt);
        h_ggH[label.str()] -> Write();
        
        label.str(std::string());
        //if( charges.at(chargeIt) == "" )
        //  label << "h_qqH" << mass << "_" << flavours.at(flavourIt);
        //else
        //  label << "h_qqH" << mass << "_" << flavours.at(flavourIt) << "_" << charges.at(chargeIt);
        //h_qqH[label.str()] -> Write();
        
        // signal histograms - fit
        TF1* fitFunc_scb;
        
        label.str(std::string());
        if( charges.at(chargeIt) == "" )
          label << "h_ggH" << mass << "_" << flavours.at(flavourIt) << "_fit";
        else
          label << "h_ggH" << mass << "_" << flavours.at(flavourIt) << "_" << charges.at(chargeIt) << "_fit";
        RegularizeHistogram(h_ggH[label.str()]);
        FitHiggsMass(&fitFunc_scb,"fitFunc_scb",0.,1000.,h_ggH[label.str()],mass,"crystalBallLowHigh");
        h_ggH[label.str()] -> Write();
        
        label.str(std::string());
        //if( charges.at(chargeIt) == "" )
        //  label << "h_qqH" << mass << "_" << flavours.at(flavourIt) << "_fit";
        //else
        //  label << "h_qqH" << mass << "_" << flavours.at(flavourIt) << "_" << charges.at(chargeIt) << "_fit";
        //RegularizeHistogram(h_qqH[label.str()]);
        //FitHiggsMass(&fitFunc_scb,"fitFunc_scb",0.,1000.,h_qqH[label.str()],mass,"crystalBallLowHigh");
        //h_qqH[label.str()] -> Write();
        
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






bool CheckPath(const std::string& s1, const std::string& s2)
{
  std::string item;

  std::vector<std::string> v1;
  std::stringstream ss1(s1);
  while(std::getline(ss1,item,'_')) v1.push_back(item);
  
  std::vector<std::string> v2;
  std::stringstream ss2(s2);
  while(std::getline(ss2,item,'_')) v2.push_back(item);
  
  
  if( v1.size() != v2.size() ) return false;
  
  
  for(unsigned int i = 0; i < v1.size(); ++i)
  {
    if( v2.at(i) == "v*" ) continue;
    if( v2.at(i) != v1.at(i) ) return false;
  }
  
  return true;
}

