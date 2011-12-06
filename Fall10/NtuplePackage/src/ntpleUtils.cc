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





double PURescaleFactor(const double& nPU_MC, const int& PUScale, const int& dataFlag, const int& MCFlag)
{
  if(nPU_MC < 0.)
    return 1.;
  
  else if(nPU_MC > 34.)
    return 1.;
  
  else
  {
    double* distrPU_MC = NULL;
    
    if(MCFlag == 20112)  // Summer11 - in-time
    {
      double dummyPU_MC[] = {
        1.45346E-01, // 0
        6.42802E-02, // 1
        6.95255E-02, // 2
        6.96747E-02, // 3
        6.92955E-02, // 4
        6.84997E-02, // 5
        6.69528E-02, // 6
        6.45515E-02, // 7
        6.09865E-02, // 8
        5.63323E-02, // 9
        5.07322E-02, // 10
        4.44681E-02, // 11
        3.79205E-02, // 12
        3.15131E-02, // 13
        2.54220E-02, // 14
        2.00184E-02, // 15
        1.53776E-02, // 16
        1.15387E-02, // 17
        8.47608E-03, // 18
        6.08715E-03, // 19
        4.28255E-03, // 20
        2.97185E-03, // 21
        2.01918E-03, // 22
        1.34490E-03, // 23
        8.81587E-04, // 24
        5.69954E-04, // 25
        3.61493E-04, // 26
        2.28692E-04, // 27
        1.40791E-04, // 28
        8.44606E-05, // 29
        5.10204E-05, // 30
        3.07802E-05, // 31
        1.81401E-05, // 32
        1.00201E-05, // 33
        5.80004E-06  // 34
      };
      distrPU_MC = dummyPU_MC;
    }
    
    else if(MCFlag == 20113) // Fall11 - in-time
    {
      double dummyPU_MC[] = {
	0.003388501, // 0
	0.010357558, // 1
	0.024724258, // 2
	0.042348605, // 3
	0.058279812, // 4
	0.068851751, // 5
	0.072914824, // 6
	0.071579609, // 7
	0.066811668, // 8
	0.060672356, // 9
	0.054528356, // 10
	0.049193540, // 11
	0.044886042, // 12
	0.041341896, // 13
	0.038467900, // 14
	0.035871463, // 15
	0.033419520, // 16
	0.030915649, // 17
	0.028395374, // 18
	0.025798107, // 19
	0.023237445, // 20
	0.020602754, // 21
	0.018068800, // 22
	0.015559693, // 23
	0.013211063, // 24
	0.010964293, // 25
	0.008920993, // 26
	0.007080504, // 27
	0.005499239, // 28
	0.004187022, // 29
	0.003096474, // 30
	0.002237361, // 31
	0.001566428, // 32
	0.001074149, // 33
	0.000721755  // 34
      };
      distrPU_MC = dummyPU_MC;
    }
    
    
    
    double* distrPU_DATA = NULL;
    double* distrPUUp_DATA = NULL;
    double* distrPUDown_DATA = NULL;
    
    
    if(dataFlag == 20111)  // Run2011A
    {
      double dummyPU_DATA[] = {
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
	0.000001946, // 25
	0.000000766, // 26
	0.000000294, // 27
	0.000000110, // 28
	0.000000040, // 29
	0.000000014, // 30
	0.000000005, // 31
	0.000000002, // 32
	0.000000001, // 33
	0.000000000, // 34
	0.000000000  // 35
      };
      distrPU_DATA = dummyPU_DATA;
      
      double dummyPUUp_DATA[] = {
	0.005101118, // 0
	0.019616973, // 1
	0.042805932, // 2
	0.069807865, // 3
	0.094571814, // 4
	0.111901835, // 5
	0.118791863, // 6
	0.115053281, // 7
	0.102318265, // 8
	0.085773677, // 9
	0.068378009, // 10
	0.052039150, // 11
	0.037905060, // 12
	0.026473492, // 13
	0.017752707, // 14
	0.011679462, // 15
	0.007506310, // 16
	0.004667612, // 17
	0.002804992, // 18
	0.001627845, // 19
	0.000911964, // 20
	0.000493186, // 21
	0.000267179, // 22
	0.000144413, // 23
	0.000075307, // 24
	0.000037894, // 25
	0.000018410, // 26
	0.000008643, // 27
	0.000003925, // 28
	0.000001801, // 29
	0.000000853, // 30
	0.000000387, // 31
	0.000000169, // 32
	0.000000071, // 33
	0.000000029  // 34
      };
      distrPUUp_DATA = dummyPUUp_DATA;   
      
      double dummyPUDown_DATA[] = {
	0.007079117, // 0
	0.038273200, // 1
	0.088145092, // 2
	0.136151433, // 3
	0.162401646, // 4
	0.160292029, // 5
	0.135059550, // 6
	0.101830974, // 7
	0.070044674, // 8
	0.044358157, // 9
	0.025943806, // 10
	0.014429441, // 11
	0.007645309, // 12
	0.003811724, // 13
	0.001780658, // 14
	0.000772949, // 15
	0.000325344, // 16
	0.000135433, // 17
	0.000053306, // 18
	0.000019695, // 19
	0.000006740, // 20
	0.000002223, // 21
	0.000000764, // 22
	0.000000250, // 23
	0.000000077, // 24
	0.000000022, // 25
	0.000000006, // 26
	0.000000002, // 27
	0.000000001, // 28
	0.000000000, // 29
	0.000000000, // 30
	0.000000000, // 31
	0.000000000, // 32
	0.000000000, // 33
	0.000000000  // 34
      };
      distrPUDown_DATA = dummyPUDown_DATA;   
    }
    
    
    if(dataFlag == 20112)  // Run2011B
    {
      double dummyPU_DATA[] = {
	0.000191236, // 0
	0.001277422, // 1
	0.004599972, // 2
	0.011593056, // 3
	0.022896772, // 4
	0.037801771, // 5
	0.054392517, // 6
	0.070212059, // 7
	0.083010412, // 8
	0.091250700, // 9
	0.094289611, // 10
	0.092308314, // 11
	0.086107404, // 12
	0.076853912, // 13
	0.065835930, // 14
	0.054259304, // 15
	0.043106884, // 16
	0.033067601, // 17
	0.024529367, // 18
	0.017619424, // 19
	0.012271007, // 20
	0.008296419, // 21
	0.005451862, // 22
	0.003486181, // 23
	0.002171698, // 24
	0.001319390, // 25
	0.000782592, // 26
	0.000453665, // 27
	0.000257278, // 28
	0.000142872, // 29
	0.000077761, // 30
	0.000041515, // 31
	0.000021758, // 32
	0.000011202, // 33
	0.000011131, // 34
	0.000000000  // 35
      };
      distrPU_DATA = dummyPU_DATA;
      
      double dummyPUUp_DATA[] = {
	0.000175480, // 0
	0.001090057, // 1
	0.003718595, // 2
	0.009051823, // 3
	0.017591955, // 4
	0.029053016, // 5
	0.042385317, // 6
	0.056055315, // 7
	0.068430483, // 8
	0.078125663, // 9
	0.084223576, // 10
	0.086350687, // 11
	0.084638610, // 12
	0.079417400, // 13
	0.071824975, // 14
	0.062796235, // 15
	0.053169530, // 16
	0.043655232, // 17
	0.034792859, // 18
	0.026938293, // 19
	0.020275261, // 20
	0.014843629, // 21
	0.010576481, // 22
	0.007338748, // 23
	0.004961861, // 24
	0.003306743, // 25
	0.002164230, // 26
	0.001386295, // 27
	0.000869299, // 28
	0.000533795, // 29
	0.000321086, // 30
	0.000189270, // 31
	0.000109382, // 32
	0.000062004, // 33
	0.000034492  // 34
      };
      distrPUUp_DATA = dummyPUUp_DATA;   
      
      double dummyPUDown_DATA[] = {
	0.000210101, // 0
	0.001763519, // 1
	0.006569504, // 2
	0.016411813, // 3
	0.031616811, // 4
	0.050520934, // 5
	0.070044838, // 6
	0.086847387, // 7
	0.098343395, // 8
	0.103216253, // 9
	0.101443574, // 10
	0.093736358, // 11
	0.082211003, // 12
	0.068738751, // 13
	0.054943100, // 14
	0.042068508, // 15
	0.030904604, // 16
	0.021809813, // 17
	0.014799979, // 18
	0.009663889, // 19
	0.006074341, // 20
	0.003726748, // 21
	0.002226559, // 22
	0.001290993, // 23
	0.000726991, // 24
	0.000397845, // 25
	0.000211670, // 26
	0.000109508, // 27
	0.000055085, // 28
	0.000026927, // 29
	0.000012779, // 30
	0.000000000, // 31
	0.000000000, // 32
	0.000000000, // 33
	0.000000000  // 34
      };
      distrPUDown_DATA = dummyPUDown_DATA;   
    }
    
    
    if(dataFlag == 201112)  // Run2011A + Run2011B
    {
      double dummyPU_DATA[] = {
	0.002859418, // 0
	0.012560310, // 1
	0.029963130, // 2
	0.051312994, // 3
	0.070971273, // 4
	0.084786385, // 5
	0.091462672, // 6
	0.091925524, // 7
	0.087999381, // 8
	0.081412680, // 9
	0.073399492, // 10
	0.064719088, // 11
	0.055832654, // 12
	0.047066260, // 13
	0.038698819, // 14
	0.030981110, // 15
	0.024117511, // 16
	0.018240983, // 17
	0.013399664, // 18
	0.009560713, // 19
	0.006628135, // 20
	0.004467347, // 21
	0.002929458, // 22
	0.001870567, // 23
	0.001164142, // 24
	0.000706805, // 25
	0.000419059, // 26
	0.000242856, // 27
	0.000137700, // 28
	0.000076458, // 29
	0.000041610, // 30
	0.000022214, // 31
	0.000011642, // 32
	0.000005994, // 33
	0.000005955, // 34
	0.000000000  // 35
      };
      distrPU_DATA = dummyPU_DATA;
      
      double dummyPUUp_DATA[] = {
	0.002568293, // 0
	0.010394400, // 1
	0.023729650, // 2
	0.040231571, // 3
	0.056554776, // 4
	0.069837347, // 5
	0.078487486, // 6
	0.082270078, // 7
	0.081912212, // 8
	0.078544840, // 9
	0.073231079, // 10
	0.066789739, // 11
	0.059858777, // 12
	0.052730761, // 13
	0.045623668, // 14
	0.038720887, // 15
	0.032187037, // 16
	0.026167355, // 17
	0.020778600, // 18
	0.016098727, // 19
	0.012193037, // 20
	0.009063362, // 21
	0.006585137, // 22
	0.004675657, // 23
	0.003243813, // 24
	0.002198756, // 25
	0.001456219, // 26
	0.000942487, // 27
	0.000596269, // 28
	0.000368885, // 29
	0.000226736, // 30
	0.000138432, // 31
	0.000082865, // 32
	0.000048636, // 33
	0.000027994  // 34
      };
      distrPUUp_DATA = dummyPUUp_DATA;   
      
      double dummyPUDown_DATA[] = {
	0.003224979, // 0
	0.016675379, // 1
	0.039950598, // 2
	0.066376626, // 3
	0.088012539, // 4
	0.100439124, // 5
	0.103556134, // 6
	0.099714980, // 7
	0.091615185, // 8
	0.081308804, // 9
	0.070202336, // 10
	0.058953498, // 11
	0.048042685, // 12
	0.037884112, // 13
	0.028827770, // 14
	0.021118648, // 15
	0.014915664, // 16
	0.010209606, // 17
	0.006741456, // 18
	0.004294284, // 19
	0.002638892, // 20
	0.001564150, // 21
	0.000893835, // 22
	0.000491965, // 23
	0.000265807, // 24
	0.000141758, // 25
	0.000073500, // 26
	0.000037046, // 27
	0.000018142, // 28
	0.000008623, // 29
	0.000006724, // 30
	0.000000000, // 31
	0.000000000, // 32
	0.000000000, // 33
	0.000000000  // 34
      };
      distrPUDown_DATA = dummyPUDown_DATA;   
    }
    
    
    
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
