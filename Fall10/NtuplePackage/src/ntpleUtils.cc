#include "ntpleUtils.h"


bool maggiore(double i, double j)
{
  return( i>j );
}






std::map<int, int> GetTotalEvents(const std::string& histoName, const std::string& inputFileList)
{
  std::ifstream inFile(inputFileList.c_str());
  std::string buffer;
  std::map<int, int> totalEvents;
  
  if(!inFile.is_open())
  {
    std::cerr << "** ERROR: Can't open '" << inputFileList << "' for input" << std::endl;
    return totalEvents;
  }
  
  while(1)
  {
    inFile >> buffer;
    if(!inFile.good()) break;

    TFile* f = TFile::Open(buffer.c_str());
    TH1F* histo = NULL;
    f -> GetObject(histoName.c_str(), histo);
    if(histo == NULL)
    {
      std::cout << ">>>ntpleUtils::Error in getting object " << histoName << std::endl;
      exit(-1);
    }
    
    
    for(int bin = 1; bin <= histo -> GetNbinsX(); ++bin)
      totalEvents[bin] += int(histo -> GetBinContent(bin));
    
    f -> Close();
    delete f;
  }

  return totalEvents;
}

//  ------------------------------------------------------------



TH1F* GetTotalHisto(const std::string& histoName, const std::string& inputFileList)
{
  std::ifstream inFile(inputFileList.c_str());
  std::string buffer;
  TH1F* totalHisto = NULL;
  
  if(!inFile.is_open())
  {
    std::cerr << ">>>ntpleUtils::GetTotalHisto::can't open file " << inputFileList << " for input" << std::endl;
    return totalHisto;
  }
  
  bool isFirstFile = true;
  while(1)
  {
    inFile >> buffer;
    if(!inFile.good()) break;
    
    //std::cout << "ntpleUtils::GetTotalHisto::getting histogram " << histoName << " from file " << buffer << std::endl;
    TFile* f = TFile::Open(buffer.c_str());
    TH1F* histo = NULL;
    f -> GetObject(histoName.c_str(), histo);
    if(histo == NULL)
    {
      std::cout << ">>>ntpleUtils::GetTotalHisto::Error in getting object " << histoName << std::endl;
      exit(-1);
    }
    
    if( isFirstFile )
      totalHisto = (TH1F*)(histo->Clone());
    else
      totalHisto -> Add(histo);
    
    if( !isFirstFile )
    {
      f -> Close();
      delete f;
      isFirstFile = false;
    }
  }

  return totalHisto;
}

//  ------------------------------------------------------------



std::map<int, std::string> GetBinLabels(const std::string& histoName, const std::string& inputFileList)
{
  std::ifstream inFile(inputFileList.c_str());
  std::string buffer;
  std::map<int, std::string> binLabels;
  
  if(!inFile.is_open())
  {
    std::cerr << "** ERROR: Can't open '" << inputFileList << "' for input" << std::endl;
    return binLabels;
  }
  
  while(1)
  {
    inFile >> buffer;
    if(!inFile.good()) break;

    TFile* f = TFile::Open(buffer.c_str());
    TH1F* histo = NULL;
    f -> GetObject(histoName.c_str(), histo);
    if(histo == NULL)
    {
      std::cout << ">>>ntpleUtils::Error in getting object " << histoName << std::endl;
      exit(-1);
    }
    
    
    for(int bin = 1; bin <= histo -> GetNbinsX(); ++bin)
      binLabels[bin] = std::string(histo -> GetXaxis() -> GetBinLabel(bin));
    
    f -> Close();
    delete f;
  }

  return binLabels;
}

//  ------------------------------------------------------------


bool FillChain(TChain& chain, const std::string& inputFileList)
{
  std::ifstream inFile(inputFileList.c_str());
  std::string buffer;

  if(!inFile.is_open())
  {
    std::cerr << "** ERROR: Can't open '" << inputFileList << "' for input" << std::endl;
    return false;
  }
  
  while(1)
  {
    inFile >> buffer;
    if(!inFile.good()) break;
    chain.Add(buffer.c_str());
    //std::cout << ">>> ntupleUtils::FillChain - treeName = " << chain.GetName() << " from file " << buffer << std::endl;
  }

  return true;
}

//  ------------------------------------------------------------

int parseConfigFile (const TString& config)
{
  std::cout << ">>> Parsing " << config << " file" << std::endl ;
  
  if (gConfigParser) return 1 ;
  gConfigParser = new ConfigParser();
  
  if( !(gConfigParser -> init(config)) )
  {
    std::cout << ">>> parseConfigFile::Could not open configuration file "
              << config << std::endl;
     return -1;
  }
  
  //gConfigParser -> print();
  
  return 0 ;
}

//  ------------------------------------------------------------





double PURescaleFactor(const double& nPU_MC, const int& PUScale)
{
  if(nPU_MC < 0.)
    return 1.;
  
  else if(nPU_MC > 24.)
    return 0.;
  
  else
  {
    double distrPU_MC[] = {
      
      0.104109000, //0
      0.070357300, //1
      0.069844500, //2
      0.069825400, //3
      0.069705400, //4
      0.069790700, //5
      0.069675100, //6
      0.069448600, //7
      0.068033200, //8
      0.065104400, //9
      0.059803600, //10
      0.052739500, //11
      0.043951300, //12
      0.035220200, //13
      0.026671400, //14
      0.019411000, //15
      0.013397400, //16
      0.008985360, //17
      0.005751600, //18
      0.003514930, //19
      0.002120870, //20
      0.001228910, //21
      0.000705920, //22
      0.000384744, //23
      0.000219377  //24
      
      //0.0698146584, // 0
      //0.0698146584, // 1
      //0.0698146584, // 2
      //0.0698146584, // 3
      //0.0698146584, // 4
      //0.0698146584, // 5
      //0.0698146584, // 6
      //0.0698146584, // 7
      //0.0698146584, // 8
      //0.0698146584, // 9
      //0.0698146584, // 10
      //0.0630151648, // 11
      //0.0526654164, // 12
      //0.0402754482, // 13
      //0.0292988928, // 14
      //0.0194384503, // 15
      //0.0122016783, // 16
      //0.0072070420, // 17
      //0.0040036370, // 18
      //0.0020278322, // 19
      //0.0010739954, // 20
      //0.0004595759, // 21
      //0.0002229748, // 22
      //0.0001028162, // 23
      //0.0000458337  // 24
      
    };
    
    double distrPU_DATA[] = {
      
      0.005929510, // 0
      0.025542751, // 1
      0.059146766, // 2
      0.097015992, // 3
      0.126287282, // 4
      0.138848349, // 5
      0.134116739, // 6
      0.116909698, // 7
      0.093739837, // 8
      0.070092745, // 9
      0.049362671, // 10
      0.032974087, // 11
      0.020997589, // 12
      0.012791663, // 13
      0.007474022, // 14
      0.004196492, // 15
      0.002267743, // 16
      0.001181017, // 17
      0.000593481, // 18
      0.000288109, // 19
      0.000135272, // 20
      0.000061498, // 21
      0.000027102, // 22
      0.000011591, // 23
      0.000004816, // 24
      
    };
    
    double distrPUUp_DATA[] = {
      
      0.005101111, // 0
      0.019616924, // 1
      0.042805802, // 2
      0.069807649, // 3
      0.094571561, // 4
      0.111901611, // 5
      0.118791729, // 6
      0.115053244, // 7
      0.102318265, // 8
      0.085773773, // 9
      0.068378158, // 10
      0.052039314, // 11
      0.037905209, // 12
      0.026473617, // 13
      0.017752802, // 14
      0.011679574, // 15
      0.007506388, // 16
      0.004667663, // 17
      0.002805023, // 18
      0.001627864, // 19
      0.000911975, // 20
      0.000493192, // 21
      0.000267186, // 22
      0.000144416, // 23
      0.000075309  // 24
      
    };
    
    double distrPUDown_DATA[] = {
      
      0.007079131, // 0
      0.038273368, // 1
      0.088145472, // 2
      0.136151940, // 3
      0.162402108, // 4
      0.160292283, // 5
      0.135059431, // 6
      0.101830713, // 7
      0.070044346, // 8
      0.044357833, // 9
      0.025943525, // 10
      0.014429307, // 11
      0.007645215, // 12
      0.003811661, // 13
      0.001780620, // 14
      0.000772926, // 15
      0.000325338, // 16
      0.000135430, // 17
      0.000053305, // 18
      0.000019695, // 19
      0.000006740, // 20
      0.000000000, // 21
      0.000000000, // 22
      0.000000000, // 23
      0.000000000  // 24
      
    };
    
    
    if( PUScale == 1 )
      return 1. * distrPUUp_DATA[int(nPU_MC)] / distrPU_MC[int(nPU_MC)];
    
    else if( PUScale == -1 )
      return 1. * distrPUDown_DATA[int(nPU_MC)] / distrPU_MC[int(nPU_MC)];
    
    else
      return 1. * distrPU_DATA[int(nPU_MC)] / distrPU_MC[int(nPU_MC)];
  }
}



double deltaPhi(const double& phi1, const double& phi2)
{ 
  double deltaphi = fabs(phi1 - phi2);
  if (deltaphi > 6.283185308) deltaphi -= 6.283185308;
  if (deltaphi > 3.141592654) deltaphi = 6.283185308 - deltaphi;
  return deltaphi;
}

//  ------------------------------------------------------------

double deltaEta(const double& eta1, const double& eta2)
{ 
  double deltaeta = fabs(eta1 - eta2);
  return deltaeta;
}

//  ------------------------------------------------------------

double deltaR(const double& eta1, const double& phi1,
              const double& eta2, const double& phi2)
{ 
 double deltaphi = deltaPhi(phi1, phi2);
 double deltaeta = deltaEta(eta1, eta2);
 double deltar = sqrt(deltaphi*deltaphi + deltaeta*deltaeta);
 return deltar;
}

//  ------------------------------------------------------------






double SelectJets(std::vector<int>& it, std::vector<ROOT::Math::XYZTVector>& jets,
                  const std::string& method,
                  const double& ptMin,
                  const std::vector<int>* blacklist)
{
  // initialize vector with result
  it.clear();
  it.push_back(-1);
  it.push_back(-1);
  
  
  
  // initialize the selection variable
  double maxDeta = -999999.;
  double tempDeta = 0.;
  
  double maxMJJ = -999999.;
  double tempMJJ = 0.;
  
  double maxPt = -999999.;
  double tempPt = 0.;
  
  double maxSumPt = -999999.;
  double tempSumPt = 0.;
  
  
  
  // loop over 1st jet
  for(unsigned int i = 0; i < jets.size(); ++i)
  {
    if(jets.at(i).pt() < ptMin) continue;
    
    bool skipJet1 = false;
    if(blacklist)
      for(unsigned int kk = 0; kk < blacklist -> size(); ++kk)
        if(blacklist -> at(kk) == static_cast<int>(i)) skipJet1 = true;
    if(skipJet1) continue;
    
    
    
    // loop over 2nd jet
    for(unsigned int j = i+1; j < jets.size(); ++j)
    {
      if(jets.at(j).pt() < ptMin) continue;
      
      bool skipJet2 = false;
      if(blacklist)
        for(unsigned int kk = 0; kk < blacklist -> size(); ++kk)
          if(blacklist -> at(kk) == static_cast<int>(j)) skipJet2 = true;
      if(skipJet2) continue;
      
      
      
      // -------------------------------------
      // select jets with different techniques
      // -------------------------------------
      
      if(method == "maxDeta")
      {
        tempDeta = deltaEta(jets.at(i).Eta(), jets.at(j).Eta());
        if(tempDeta > maxDeta)
        {
          maxDeta = tempDeta;
          
    	    it.at(0) = i;
	        it.at(1) = j;
        }
      }
      
      // -------------------------------------
      
      if(method == "maxMJJ")
      {
        tempMJJ = (jets.at(i) + jets.at(j)).mass();
        if(tempMJJ > maxMJJ)
        {
          maxMJJ = tempMJJ;
          
    	    it.at(0) = i;
	        it.at(1) = j;
        }
      }
      
      // -------------------------------------
      else if(method == "maxPt")
      {
        tempPt = sqrt( (jets.at(i) + jets.at(j)).perp2() );
        if(tempPt > maxPt)
        {
          maxPt = tempPt;
          
	        it.at(0) = i;
	        it.at(1) = j;
        }
      }
      
      // -------------------------------------
      
      else if(method == "maxSumPt")
      {
        tempSumPt = sqrt(jets.at(i).perp2()) + sqrt(jets.at(j).perp2());
        if(tempSumPt > maxSumPt)
        {
          maxSumPt = tempSumPt;
          
   	      it.at(0) = i;
	        it.at(1) = j;
        }
      }
      
      // -------------------------------------
      
      
      
    } // loop over 2nd jet
  } // loop over 1st jet
  
  
  
  if(method == "maxMJJ")
    return maxMJJ;
  
  else if(method == "maxPt")
    return maxPt;
  
  else if(method == "maxSumPt")
    return maxSumPt;
  
  else return -1.;
}

//  ------------------------------------------------------------

double SelectJets(std::vector<int>& it, std::vector<ROOT::Math::XYZTVector>& jets,
                  const std::string& method,
                  const double& ptMin,
                  const double& DetaMin,
                  const double& DetaMax,
                  const std::vector<int>* blacklist)
{
  // initialize vector with result
  it.clear();
  it.push_back(-1);
  it.push_back(-1);
  
  
  
  // initialize the selection variable
  double maxDeta = -999999.;
  double tempDeta = 0.;
  
  double maxMJJ = -999999.;
  double tempMJJ = 0.;
  
  double maxPt = -999999.;
  double tempPt = 0.;
  
  double maxSumPt = -999999.;
  double tempSumPt = 0.;
  
  
  
  // loop over 1st jet
  for(unsigned int i = 0; i < jets.size(); ++i)
  {
    if(jets.at(i).pt() < ptMin) continue;
    
    bool skipJet1 = false;
    if(blacklist)
      for(unsigned int kk = 0; kk < blacklist -> size(); ++kk)
        if(blacklist -> at(kk) == static_cast<int>(i)) skipJet1 = true;
    if(skipJet1) continue;
    
    
    
    // loop over 2nd jet
    for(unsigned int j = i+1; j < jets.size(); ++j)
    {
      if(jets.at(j).pt() < ptMin) continue;
      
      bool skipJet2 = false;
      if(blacklist)
        for(unsigned int kk = 0; kk < blacklist -> size(); ++kk)
          if(blacklist -> at(kk) == static_cast<int>(j)) skipJet2 = true;
      if(skipJet2) continue;
      
      
      
      // -------------------------------------
      // select jets with different techniques
      // -------------------------------------
      
      if(method == "maxDeta")
      {
        tempDeta = deltaEta(jets.at(i).Eta(), jets.at(j).Eta());
        if( (tempDeta > maxDeta) && 
            (tempDeta > DetaMin) &&
            (tempDeta < DetaMax) )
        {
          maxDeta = tempDeta;
          
    	    it.at(0) = i;
	        it.at(1) = j;
        }
      }
      
      // -------------------------------------
      
      if(method == "maxMJJ")
      {
        tempMJJ = (jets.at(i) + jets.at(j)).mass();
        tempDeta = deltaEta(jets.at(i).Eta(), jets.at(j).Eta());
        
        if( (tempMJJ > maxMJJ) &&
            (tempDeta > DetaMin) &&
            (tempDeta < DetaMax) )
        {
          maxMJJ = tempMJJ;
          
    	    it.at(0) = i;
	        it.at(1) = j;
        }
      }
      
      // -------------------------------------
      else if(method == "maxPt")
      {
        tempPt = sqrt( (jets.at(i) + jets.at(j)).perp2() );
        tempDeta = deltaEta(jets.at(i).Eta(), jets.at(j).Eta());
        if( (tempPt > maxPt) &&
            (tempDeta > DetaMin) &&
            (tempDeta < DetaMax) )
        {
          maxPt = tempPt;
          
	        it.at(0) = i;
	        it.at(1) = j;
        }
      }
      
      // -------------------------------------
      
      else if(method == "maxSumPt")
      {
        tempSumPt = sqrt(jets.at(i).perp2()) + sqrt(jets.at(j).perp2());
        tempDeta = deltaEta(jets.at(i).Eta(), jets.at(j).Eta());
        if( (tempSumPt > maxSumPt) &&
            (tempDeta > DetaMin) &&
            (tempDeta < DetaMax) )
        {
          maxSumPt = tempSumPt;
          
   	      it.at(0) = i;
	        it.at(1) = j;
        }
      }
      
      // -------------------------------------
      
      
      
    } // loop over 2nd jet
  } // loop over 1st jet
  
  
  
  if(method == "maxMJJ")
    return maxMJJ;
  
  else if(method == "maxPt")
    return maxPt;
  
  else if(method == "maxSumPt")
    return maxSumPt;
  
  else return -1.;
}

//  ------------------------------------------------------------

int SelectLepton(std::vector<ROOT::Math::XYZTVector>& leptons,
                 const std::string& method,
                 const double& ptMin,
                 const std::vector<int>* blacklist)
{
  // initialize variable with result
  int it = -1;
  
  
  
  // initialize the selection variable
  double maxPt = -999999.;
  double tempPt = 0.;
  
  
  
  // loop over leptons
  for(unsigned int i = 0; i < leptons.size(); ++i)
  {
    if( sqrt(leptons.at(i).Perp2()) < ptMin ) continue;
    
    bool skipLep = false;
    if(blacklist)
      for(unsigned int kk = 0; kk < blacklist -> size(); ++kk)
        if(blacklist -> at(kk) == static_cast<int>(i)) skipLep = true;
    if(skipLep) continue;
    
    
    // -------------------------------------
    // select jets with different techniques
    // -------------------------------------
    
    if(method == "maxPt")
    {
      tempPt = sqrt(leptons.at(i).perp2());
      if(tempPt > maxPt)
      {
        maxPt = tempPt;
        
        it = i;
      }
    }
    
    // -------------------------------------
    
    
    
  } // loop over leptons
  
  
  
  if(method == "maxPt")
    return it;
  
  else return -1;
}

//  ------------------------------------------------------------






double SelectTagJets(std::vector<int>& it, std::vector<ROOT::Math::XYZTVector>& jets,
                     const std::string& method,
                     const double& ptMin,
                     const double& DetaMIN,
                     const double& mjjMIN,
                     const std::vector<int>* blacklist)
{
  // initialize vector with result
  it.clear();
  it.push_back(-1);
  it.push_back(-1);
  
  
  
  // initialize the selection variable
  double maxMjj = -999999.;
  double maxDeta = -999999.;
  double maxSumPt = -999999.;
  double tempMjj = 0.;
  double tempDeta = 0.; 
  double tempSumPt = 0.;
  double prodEta = 0.;
  
  
  
  // loop over 1st jet
  for(unsigned int i = 0; i < jets.size(); ++i)
  {
    if(jets.at(i).pt() < ptMin) continue;
    
    bool skipJet1 = false;
    if(blacklist)
      for(unsigned int kk = 0; kk < blacklist -> size(); ++kk)
        if(blacklist -> at(kk) == static_cast<int>(i)) skipJet1 = true;
    if(skipJet1) continue;
    
    
    
    // loop over 2nd jet
    for(unsigned int j = i+1; j < jets.size(); ++j)
    {
      if(jets.at(j).pt() < ptMin) continue;
      
      bool skipJet2 = false;
      if(blacklist)
        for(unsigned int kk = 0; kk < blacklist -> size(); ++kk)
          if(blacklist -> at(kk) == static_cast<int>(j)) skipJet2 = true;
      if(skipJet2) continue;
      
      
      
      // -------------------------------------
      // select jets with different techniques
      // -------------------------------------
      
      tempMjj = (jets.at(i) + jets.at(j)).mass();
      tempDeta = deltaEta(jets.at(i).eta(), jets.at(j).eta());
      tempSumPt = jets.at(i).pt() + jets.at(j).pt();  
      prodEta = jets.at(i).eta() * jets.at(j).eta(); 
      
      if(method == "maxMjj")
      {
        if( (tempMjj > maxMjj) && 
            (tempDeta > DetaMIN) &&
            (tempMjj > mjjMIN) )
        {
          maxMjj = tempMjj;
          
          it.at(0) = i;
          it.at(1) = j;
        }
      }
      
      if(method == "maxMjjEtaOpposite")
      {
        if( (tempMjj > maxMjj) && 
            (tempDeta > DetaMIN) &&
            (tempMjj > mjjMIN) && 
            (prodEta < 0.) )
        {
          maxMjj = tempMjj;
          
          it.at(0) = i;
          it.at(1) = j;
        }
      }
      
      if(method == "maxDeta")
      {
        if( (tempDeta > maxDeta) && 
            (tempDeta > DetaMIN) &&
            (tempMjj > mjjMIN) )
        {
          maxDeta = tempDeta;
          
          it.at(0) = i;
          it.at(1) = j;
        }
      }
      
      if(method == "maxDetaEtaOpposite")
      {
        if( (tempDeta > maxDeta) && 
            (tempDeta > DetaMIN) &&
            (tempMjj > mjjMIN) && 
            (prodEta < 0.) )
        {
          maxDeta = tempDeta;
          
          it.at(0) = i;
          it.at(1) = j;
        }
      }
      
      if(method == "maxSumPt")
      {
        if( (tempSumPt > maxSumPt) && 
            (tempDeta > DetaMIN) &&
            (tempMjj > mjjMIN) )
        {
          maxSumPt = tempSumPt;
          
          it.at(0) = i;
          it.at(1) = j;
        }
      }
      
      if(method == "maxSumPtEtaOpposite")
      {
        if( (tempSumPt > maxSumPt) && 
            (tempDeta > DetaMIN) &&
            (tempMjj > mjjMIN) && 
            (prodEta < 0.) )
        {
          maxSumPt = tempSumPt;
          
          it.at(0) = i;
          it.at(1) = j;
        }
      }
      
      
      
    } // loop over 2nd jet
  } // loop over 1st jet
  
  
  
  return maxMjj;
}






double SelectTagJet(std::vector<int>& it, std::vector<ROOT::Math::XYZTVector>& jets,
                    const double& ptMin,
                    const double& etaMin,
                    const std::vector<int>* blacklist)
{
  // initialize vector with result
  it.clear();
  it.push_back(-1);
  it.push_back(-1);
  
  
  
  // initialize the selection variable
  double maxAbsEta = -999999.;
  double tempAbsEta = 0.;
  
  
  // loop over jets
  for(unsigned int i = 0; i < jets.size(); ++i)
  {
    if(jets.at(i).pt() < ptMin) continue;
    if(fabs(jets.at(i).eta()) < etaMin) continue;
    
    bool skipJet1 = false;
    if(blacklist)
      for(unsigned int kk = 0; kk < blacklist -> size(); ++kk)
        if(blacklist -> at(kk) == static_cast<int>(i)) skipJet1 = true;
    if(skipJet1) continue;
    
    
    
    // -------------------------------------
    // select jets with different techniques
    // -------------------------------------
    
    tempAbsEta = fabs(jets.at(i).eta());
    if( tempAbsEta > maxAbsEta )
    {
      maxAbsEta = tempAbsEta;
      
      it.at(0) = i;
    }
      
      
      
  } // loop over jets
  
  
  
  return maxAbsEta;
}






double Select4Jets(std::vector<int>& it_W, std::vector<int>& it_tag, 
                   std::vector<ROOT::Math::XYZTVector>& jets,
                   const std::string& method,
                   const double& ptMin,
                   const double& etaMAX,
                   const double& DetaMAX,
                   const double& mjjMAX)
{
  // initialize vector with result
  it_W.clear();
  it_tag.clear();
  it_W.push_back(-1);
  it_W.push_back(-1);
  it_tag.push_back(-1);
  it_tag.push_back(-1);
  
  ROOT::Math::XYZTVector jet1_W;
  ROOT::Math::XYZTVector jet2_W;
  ROOT::Math::XYZTVector jet1_tag;
  ROOT::Math::XYZTVector jet2_tag;
  
  
  
  // initialize the selection variable
  double tempDeta = 0.;
  double minDeta = 999999999999.; 
  double tempDeta_W = 0.; 
  double tempMjj_W = 0.; 
  double tempDeta_tag = 0.; 
  double tempMjj_tag = 0.; 
  
  
  
  // loop over 1st jet
  for(unsigned int i = 0; i < jets.size(); ++i)
  {
    if(jets.at(i).pt() < ptMin) continue;
    if(fabs(jets.at(i).eta()) > etaMAX) continue;
    
    
    // loop over 2nd jet
    for(unsigned int j = i+1; j < jets.size(); ++j)
    {
      if(jets.at(j).pt() < ptMin) continue;
      if(fabs(jets.at(j).eta()) > etaMAX) continue;      
      
      
      jet1_W = jets.at(i);
      jet2_W = jets.at(j);
      tempDeta_W = deltaEta(jet1_W.eta(), jet2_W.eta());
      tempMjj_W = (jet1_W + jet2_W).mass();
      
      tempDeta = (tempDeta_W / 2.5);
      
      //std::cout << "1st jet: " << i << "   et = " << jet1_W.pt() << "   eta = " << jet1_W.eta() << std::endl;
      //std::cout << "2nd jet: " << j << "   et = " << jet2_W.pt() << "   eta = " << jet2_W.eta() << std::endl;
      //std::cout << "mJJ = " << (jet1_W+jet2_W).mass() << "   Deta = " << deltaEta(jet1_W.eta(),jet2_W.eta()) << std::endl; 
      bool isTagJetFound = false;
      
      
      // loop over 3rd jet
      for(unsigned int k = 0; k < jets.size(); ++k)
      {
        if(k == i) continue;
        if(k == j) continue;
        if(jets.at(k).pt() < ptMin) continue;
        
        
        // loop over 4th jet
        for(unsigned int l = k+1; l < jets.size(); ++l)
        {
          if(l == i) continue;
          if(l == j) continue;          
          if(jets.at(l).pt() < ptMin) continue;
          
          
          jet1_tag = jets.at(k);
          jet2_tag = jets.at(l);
          tempDeta_tag = deltaEta(jet1_tag.eta(), jet2_tag.eta());          
          tempMjj_tag = (jet1_tag + jet2_tag).mass();
          
          tempDeta += 1. / (tempMjj_tag / 80.399);
	  
          //std::cout << "3rd jet: " << k << "   et = " << jet1_tag.pt() << "   eta = " << jet1_tag.eta() << std::endl;
          //std::cout << "4th jet: " << l << "   et = " << jet2_tag.pt() << "   eta = " << jet2_tag.eta() << std::endl;
          //std::cout << "mJJ = " << (jet1_tag+jet2_tag).mass() << "   Deta = " << deltaEta(jet1_tag.eta(),jet2_tag.eta()) << std::endl;           

          if(method == "minDeta")
          {
            if( (tempDeta < minDeta) &&
                (tempDeta_W < DetaMAX) &&
                (tempMjj_W < mjjMAX) )
            {
              minDeta = tempDeta;
	      //std::cout << "trovati W e tag con " << minDeta << std::endl; 
              it_W.at(0) = i;
              it_W.at(1) = j;
              it_tag.at(0) = k;
              it_tag.at(1) = l;
              
              isTagJetFound = true;
            }
          }
          
        }
      }
      
      
      
      if( (method == "minDeta") && (isTagJetFound == false ) )
      {
        if( (tempDeta < minDeta) &&
            (tempDeta_W < DetaMAX) &&
            (tempMjj_W < mjjMAX) )
        {
          minDeta = tempDeta;
          //std::cout << "trovato W con " << minDeta << std::endl; 
          it_W.at(0) = i;
          it_W.at(1) = j;
          it_tag.at(0) = -1;
          it_tag.at(1) = -1;
        }
      }
      
    } // loop over 2nd jet
  } // loop over 1st jet
  
  
  
  return minDeta;
}







double SelectWJets(std::vector<int>& it, std::vector<ROOT::Math::XYZTVector>& jets,
                   const std::string& method,
                   const double& ptMin,
                   const double& etaMAX,
                   const double& DetaMAX,
                   const double& mjjMAX,
                   const std::vector<int>* blacklist)
{
  // initialize vector with result
  it.clear();
  it.push_back(-1);
  it.push_back(-1);
  
  
  
  // initialize the selection variable
  double minDeta = 999999.;
  double maxSumPt = -999999.;
  double minDMjj = 999999.;
  double tempSumPt = 0.;
  double tempMjj = 0.;
  double tempDMjj = 0.;
  double tempDeta = 0.; 
  
  
  
  // loop over 1st jet
  for(unsigned int i = 0; i < jets.size(); ++i)
  {
    if(jets.at(i).pt() < ptMin) continue;
    if(fabs(jets.at(i).eta()) > etaMAX) continue;
    
    bool skipJet1 = false;
    if(blacklist)
      for(unsigned int kk = 0; kk < blacklist -> size(); ++kk)
        if(blacklist -> at(kk) == static_cast<int>(i)) skipJet1 = true;
    if(skipJet1) continue;
    
    
    
    // loop over 2nd jet
    for(unsigned int j = i+1; j < jets.size(); ++j)
    {
      if(jets.at(j).pt() < ptMin) continue;
      if(fabs(jets.at(j).eta()) > etaMAX) continue;      

      bool skipJet2 = false;
      if(blacklist)
        for(unsigned int kk = 0; kk < blacklist -> size(); ++kk)
          if(blacklist -> at(kk) == static_cast<int>(j)) skipJet2 = true;
      if(skipJet2) continue;
      
      
      
      // -------------------------------------
      // select jets with different techniques
      // -------------------------------------
      tempSumPt = jets.at(i).pt() + jets.at(j).pt();
      tempDeta = deltaEta(jets.at(i).Eta(), jets.at(j).Eta());
      tempMjj = (jets.at(i) + jets.at(j)).mass();
      tempDMjj = fabs((jets.at(i) + jets.at(j)).mass() - 80.399);
      
      if(method == "maxSumPt")
      {
        if( (tempSumPt > maxSumPt) &&
            (tempDeta < DetaMAX) &&
            (tempMjj < mjjMAX) )
        {
          maxSumPt = tempSumPt;
          
          it.at(0) = i;
          it.at(1) = j;
        }
      }
      
      if(method == "minDeta")
      {  
        if( (tempDeta < minDeta) &&
            (tempDeta < DetaMAX) &&
            (tempMjj < mjjMAX) )
        {
          minDeta = tempDeta;
          
          it.at(0) = i;
          it.at(1) = j;
        }
      }
      
      if(method == "minDMjj")
      {  
        if( (tempDMjj < minDMjj) &&
            (tempDeta < DetaMAX) &&
            (tempMjj < mjjMAX) )
        {
          minDMjj = tempDMjj;
          
          it.at(0) = i;
          it.at(1) = j;
        }
      }
      
      
      
    } // loop over 2nd jet
  } // loop over 1st jet
  
  
  
  return maxSumPt;
}











int Build4JetCombinations(std::vector<std::vector<int> >& combinations, const int& nJets)
{
  combinations.clear();
  
  std::vector<int> vi;
  for(int i = 0; i < nJets; ++i)
 	  vi.push_back(i);
  
  std::vector<int> buffer;
  buffer.push_back(0);
  buffer.push_back(1);
  buffer.push_back(2);
  buffer.push_back(3);
  
  combinations.push_back(buffer);
  

  std::vector<int> oldCombination = buffer;
  while( next_permutation(vi.begin(), vi.end()) )      
  {
    if( (vi.at(0) < vi.at(1)) && (vi.at(2) < vi.at(3)) )
    {
      buffer.at(0) = vi.at(0);
      buffer.at(1) = vi.at(1);
      buffer.at(2) = vi.at(2);
      buffer.at(3) = vi.at(3);                  
      
      if(buffer == oldCombination) continue;
      
      combinations.push_back(buffer);
      oldCombination = buffer;
    }  
  }
  
  return combinations.size();
}

//  ------------------------------------------------------------

void Print4JetCombination(const std::vector<int>& combination)
{
  std::cout << "(" << combination.at(0) << "," << combination.at(1) << ")";
  std::cout << "   ---   ";
  std::cout << "(" << combination.at(2) << "," << combination.at(3) << ")";  
  std::cout << std::endl;
}

//  ------------------------------------------------------------







void Print4V(const ROOT::Math::XYZTVector& p)
{
  std::cout << std::fixed << std::setprecision(2)
            << "(E,px,py,pz) = " 
            << "(" << std::setw(8) << p.energy() 
            << "," << std::setw(8) << p.px()
            << "," << std::setw(8) << p.py()
            << "," << std::setw(8) << p.pz()
            << ")" 
            << "\tpt = " 
            << std::setw(7) << p.pt()
            << "\teta = " 
            << std::setw(6) << p.eta()
            << "\tphi = " 
            << std::setw(5) << p.phi()
            << std::endl;
}






bool GetElectronFlag(const std::string& flag)
{
  //std::cout << "flag = " << flag << std::endl;
  std::stringstream ss(flag);
  
  if( flag == "0" ) return true;
  else return false; 
}

bool GetElectronSeverityLevel(const std::string& severityLevel)
{
  //std::cout << "severityLevel = " << severityLevel << std::endl;
  
  if( severityLevel == "0" ) return true;
  else return false; 
}
