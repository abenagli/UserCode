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





double PURescaleFactor(const double& nPU_MC)
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
      
      0.006755499, // 0
      0.020532122, // 1
      0.048637504, // 2
      0.082269881, // 3
      0.110949130, // 4
      0.127005838, // 5
      0.128378483, // 6
      0.117700769, // 7
      0.099745771, // 8
      0.079192727, // 9
      0.059468251, // 10
      0.042517089, // 11
      0.029071980, // 12
      0.019069500, // 13
      0.012024521, // 14
      0.007300039, // 15
      0.004272141, // 16
      0.002412701, // 17
      0.001316296, // 18
      0.000694463, // 19
      0.000354691, // 20
      0.000175558, // 21
      0.000084299, // 22
      0.000039311, // 23
      0.000031436  // 24
      
      //0.0132558,
      //0.0316993,
      //0.0719455,
      //0.115284,
      //0.145239,
      //0.152783,
      //0.139182,
      //0.112847,
      //0.082904,
      //0.055968,
      //0.0351001,
      //0.0206271,
      //0.0114405,
      //0.00602595,
      //0.00303009,
      //0.00146112,
      //0.000678137,
      //0.000303988,
      //0.000132051,
      //5.57086e-05,
      //2.28897e-05,
      //9.17508e-06,
      //3.59522e-06,
      //1.3797e-06,
      //8.16915e-07
      
      /*0.0185058,
      0.0305444,
      0.0694579,
      0.112061, 
      0.142602,
      0.152644,
      0.139466,
      0.112813,
      0.0833194,
      0.0565895,
      0.0358211,
      0.0213224,
      0.012013,
      0.00644175,
      0.00331013,
      0.00164017,
      0.000786272,
      0.000366279,
      0.000166604,
      7.40097e-05,
      3.22769e-05,
      1.3882e-05,
      5.84891e-06,
      2.42198e-06,
      1.63229e-06 */
    };
    
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






int GetNeutrino(ROOT::Math::XYZTVector& nu,
                const ROOT::Math::XYZTVector& lep,
                const ROOT::Math::XYZTVector& met,
                const ROOT::Math::XYZTVector& WJ1,
                const ROOT::Math::XYZTVector& WJ2,
                const float& mH)
{
  // neutrino
  float alpha = lep.px()*met.px() + lep.py()*met.py();

  float delta = (alpha + 0.5*80.399*80.399)*(alpha + 0.5*80.399*80.399) - lep.pt()*lep.pt()*met.pt()*met.pt();

  if( alpha - lep.pt()*met.pt() + 0.5*80.399*80.399 < 0 )
    delta = 0.;

  float pz1 = ( lep.pz()*(alpha + 0.5*80.399*80.399) + lep.energy()*sqrt(delta) ) / lep.pt() / lep.pt();
  float pz2 = ( lep.pz()*(alpha + 0.5*80.399*80.399) - lep.energy()*sqrt(delta) ) / lep.pt() / lep.pt();

  ROOT::Math::XYZTVector* nu1 = new ROOT::Math::XYZTVector(met.px(), met.py(), pz1, sqrt(met.px()*met.px() + met.py()*met.py() + pz1*pz1));
  ROOT::Math::XYZTVector* nu2 = new ROOT::Math::XYZTVector(met.px(), met.py(), pz2, sqrt(met.px()*met.px() + met.py()*met.py() + pz2*pz2));
  
  if( fabs(mH - (lep+(*nu1)+WJ1+WJ2).mass()) < fabs(mH - (lep+(*nu2)+WJ1+WJ2).mass()) )
  {
    delete nu2;
    nu = *nu1;
  }
  else
  {
    delete nu1;
    nu = *nu2;
  }
  
  if( delta > 0. ) return 2;
  else return 1;
}



//  ------------------------------------------------------------
void GetLNuJJAngles(float& cphi_lvjj,
                    float& cphi_lvz,
                    float& ctheta_l,
                    float& ctheta_j1,
                    float& ctheta_lv,
       		    ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& l,
		    ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& v,
		    ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& j1,
		    ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& j2)
{
  ROOT::Math::XYZTVector l_lab  = l;
  ROOT::Math::XYZTVector v_lab  = v;
  ROOT::Math::XYZTVector j1_lab = j1;
  ROOT::Math::XYZTVector j2_lab = j2;
  
  ROOT::Math::XYZTVector lvjj_lab = l_lab+v_lab+j1_lab+j2_lab;
  ROOT::Math::XYZTVector lv_lab = l_lab+v_lab;
  ROOT::Math::XYZTVector jj_lab = j1_lab+j2_lab;
  
  
  
  
  
  
  // define boost in the Higgs system
  ROOT::Math::Boost boost_h;
  boost_h.SetComponents(-1./lvjj_lab.E()*lvjj_lab.Vect());
  
  // boost momenta in the Higgs system
  ROOT::Math::XYZTVector  l_h = boost_h *  l_lab;
  ROOT::Math::XYZTVector  v_h = boost_h *  v_lab;
  ROOT::Math::XYZTVector lv_h = boost_h * lv_lab;
  
  ROOT::Math::XYZTVector j1_h = boost_h * j1_lab;
  ROOT::Math::XYZTVector j2_h = boost_h * j2_lab;
  ROOT::Math::XYZTVector jj_h = boost_h * jj_lab;
  
  if( j2_h.E() > j1_h.E() )
  {
    j1_h = boost_h * j2_lab;;
    j2_h = boost_h * j1_lab;;
  }
  
  
  
  
  
  
  // find the angle between the two decay planes
  ROOT::Math::XYZVector lvplane_h = l_h.Vect().Cross(v_h.Vect());
  ROOT::Math::XYZVector jjplane_h = j1_h.Vect().Cross(j2_h.Vect());
  cphi_lvjj = lvplane_h.Dot(jjplane_h) / ( lvplane_h.R() * jjplane_h.R() );
  
  
  
  // find the angle between the production and the decay planes
  ROOT::Math::XYZVector z_lab(0.,0.,1.);
  
  ROOT::Math::XYZVector prodplane_h = z_lab.Cross(lv_h.Vect());
  ROOT::Math::XYZVector decplane_h = lv_h.Vect().Cross(l_h.Vect());
  cphi_lvz = prodplane_h.Dot(decplane_h) / ( prodplane_h.R() * decplane_h.R() );
  
  
  
  // define the (x',y',z') axes
  // z' is aligined wiht lv_h
  // x' is such that j1_h lays in the x',z' plane
  // y' follows as a consequence
  
  ROOT::Math::XYZVector z_h = lv_h.Vect();
  z_h /= z_h.R();
  
  ROOT::Math::XYZVector x_h = j1_h.Vect() - (j1_h.Vect().Dot(z_h))*z_h;
  x_h /= x_h.R();
  
  ROOT::Math::XYZVector y_h = z_h.Cross(x_h);
  y_h /= y_h.R();
  
  //std::cout << "ctheta(x-y) = " << x_h.Dot(y_h) << std::endl;
  //std::cout << "ctheta(y-z) = " << y_h.Dot(z_h) << std::endl;
  //std::cout << "ctheta(z-x) = " << z_h.Dot(x_h) << std::endl;
  
  
  
  // define boost in the lv system
  ROOT::Math::Boost boost_h_lv;
  boost_h_lv.SetComponents(-1./lv_h.E()*lv_h.Vect());
  
  // boost momenta in the lv system
  ROOT::Math::XYZTVector l_h_lv = boost_h_lv * l_h;
  
  
  
  // define boost in the jj system
  ROOT::Math::Boost boost_h_jj;
  boost_h_jj.SetComponents(-1./jj_h.E()*jj_h.Vect());
  
  // boost momenta in the jj system
  ROOT::Math::XYZTVector j1_h_jj = boost_h_jj * j1_h;
  
  
  
  
  
  
  // get the result
  ctheta_l  =  l_h_lv.Vect().Dot(+1.*z_h) /  l_h_lv.Vect().R();
  ctheta_j1 = j1_h_jj.Vect().Dot(-1.*z_h) / j1_h_jj.Vect().R();
  ctheta_lv = lv_h.Vect().Z() / lv_h.Vect().R();
  
  //std::cout << "ctheta(l-z')  = " << ctheta_l  << std::endl;
  //std::cout << "ctheta(j1-z') = " << ctheta_j1 << std::endl;
  //std::cout << "ctheta(lv-z)  = " << ctheta_lv << std::endl;
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
