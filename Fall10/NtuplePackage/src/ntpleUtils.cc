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
    {
      totalHisto = (TH1F*)(histo->Clone());
      isFirstFile = false;
    }
    
    else
    {
      totalHisto -> Add(histo);
      f -> Close();
      delete f;
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





double PURescaleFactor(const double& nPU_MC, const int& PUScale, const int& dataRunFlag, const int& MCFlag)
{
  if(nPU_MC < 0.)
    return 1.;
  
  else if(nPU_MC > 34.)
    return 0.;
  
  else
  {
    double* distrPU_MC = new double[35];
    
    if(MCFlag == 20112)  // Summer11 - in-time
    {
      distrPU_MC[0]  = 1.45346E-01;
      distrPU_MC[1]  = 6.42802E-02;
      distrPU_MC[2]  = 6.95255E-02;
      distrPU_MC[3]  = 6.96747E-02;
      distrPU_MC[4]  = 6.92955E-02;
      distrPU_MC[5]  = 6.84997E-02;
      distrPU_MC[6]  = 6.69528E-02;
      distrPU_MC[7]  = 6.45515E-02;
      distrPU_MC[8]  = 6.09865E-02;
      distrPU_MC[9]  = 5.63323E-02;
      distrPU_MC[10] = 5.07322E-02;
      distrPU_MC[11] = 4.44681E-02;
      distrPU_MC[12] = 3.79205E-02;
      distrPU_MC[13] = 3.15131E-02;
      distrPU_MC[14] = 2.54220E-02;
      distrPU_MC[15] = 2.00184E-02;
      distrPU_MC[16] = 1.53776E-02;
      distrPU_MC[17] = 1.15387E-02;
      distrPU_MC[18] = 8.47608E-03;
      distrPU_MC[19] = 6.08715E-03;
      distrPU_MC[20] = 4.28255E-03;
      distrPU_MC[21] = 2.97185E-03;
      distrPU_MC[22] = 2.01918E-03;
      distrPU_MC[23] = 1.34490E-03;
      distrPU_MC[24] = 8.81587E-04;
      distrPU_MC[25] = 5.69954E-04;
      distrPU_MC[26] = 3.61493E-04;
      distrPU_MC[27] = 2.28692E-04;
      distrPU_MC[28] = 1.40791E-04;
      distrPU_MC[29] = 8.44606E-05;
      distrPU_MC[30] = 5.10204E-05;
      distrPU_MC[31] = 3.07802E-05;
      distrPU_MC[32] = 1.81401E-05;
      distrPU_MC[33] = 1.00201E-05;
      distrPU_MC[34] = 5.80004E-06;
    }
    
    if(MCFlag == 20113) // Fall11
    {
      /*
      // twiki - intime
      distrPU_MC[0]  = 0.014583699;
      distrPU_MC[1]  = 0.025682975;
      distrPU_MC[2]  = 0.038460562;
      distrPU_MC[3]  = 0.049414536;
      distrPU_MC[4]  = 0.056931087;
      distrPU_MC[5]  = 0.061182816;
      distrPU_MC[6]  = 0.062534625;
      distrPU_MC[7]  = 0.061476918;
      distrPU_MC[8]  = 0.058677499;
      distrPU_MC[9]  = 0.055449877;
      distrPU_MC[10] = 0.051549051;
      distrPU_MC[11] = 0.047621024;
      distrPU_MC[12] = 0.043923799;
      distrPU_MC[13] = 0.040569076;
      distrPU_MC[14] = 0.037414654;
      distrPU_MC[15] = 0.034227033;
      distrPU_MC[16] = 0.031437714;
      distrPU_MC[17] = 0.028825596;
      distrPU_MC[18] = 0.026218978;
      distrPU_MC[19] = 0.023727061;
      distrPU_MC[20] = 0.021365645;
      distrPU_MC[21] = 0.01918743;
      distrPU_MC[22] = 0.016972815;
      distrPU_MC[23] = 0.014920601;
      distrPU_MC[24] = 0.013038989;
      distrPU_MC[25] = 0.011293777;
      distrPU_MC[26] = 0.009612465;
      distrPU_MC[27] = 0.008193556;
      distrPU_MC[28] = 0.006888047;
      distrPU_MC[29] = 0.005715239;
      distrPU_MC[30] = 0.004711232;
      distrPU_MC[31] = 0.003869926;
      distrPU_MC[32] = 0.003154521;
      distrPU_MC[33] = 0.002547417;
      distrPU_MC[34] = 0.002024714;
      */
      
      // wjets
      distrPU_MC[0]  = 0.008528684;
      distrPU_MC[1]  = 0.019303381;
      distrPU_MC[2]  = 0.031652782;
      distrPU_MC[3]  = 0.043396104;
      distrPU_MC[4]  = 0.052644081;
      distrPU_MC[5]  = 0.058805779;
      distrPU_MC[6]  = 0.061888184;
      distrPU_MC[7]  = 0.062111977;
      distrPU_MC[8]  = 0.060493879;
      distrPU_MC[9]  = 0.057280354;
      distrPU_MC[10] = 0.053492013;
      distrPU_MC[11] = 0.049663045;
      distrPU_MC[12] = 0.045949634;
      distrPU_MC[13] = 0.042473182;
      distrPU_MC[14] = 0.039085291;
      distrPU_MC[15] = 0.036020033;
      distrPU_MC[16] = 0.033270631;
      distrPU_MC[17] = 0.030571645;
      distrPU_MC[18] = 0.027872622;
      distrPU_MC[19] = 0.025353206;
      distrPU_MC[20] = 0.022884255;
      distrPU_MC[21] = 0.020527728;
      distrPU_MC[22] = 0.018277336;
      distrPU_MC[23] = 0.016084203;
      distrPU_MC[24] = 0.014050878;
      distrPU_MC[25] = 0.012167806;
      distrPU_MC[26] = 0.010399838;
      distrPU_MC[27] = 0.008802013;
      distrPU_MC[28] = 0.007376147;
      distrPU_MC[29] = 0.006085729;
      distrPU_MC[30] = 0.004994500;
      distrPU_MC[31] = 0.004056721;
      distrPU_MC[32] = 0.003254354;
      distrPU_MC[33] = 0.002597702;
      distrPU_MC[34] = 0.002046269;
    }
    
    
    
    double* distrPU_DATA = new double[35];
    double* distrPUUp_DATA = new double[35];
    double* distrPUDown_DATA = new double[35];
    
    
    if(dataRunFlag == 20111)  // Run2011A
    {
      /*
      // mine
      distrPU_DATA[0]  = 0.005929510;
      distrPU_DATA[1]  = 0.025542751;
      distrPU_DATA[2]  = 0.059146766;
      distrPU_DATA[3]  = 0.097015992;
      distrPU_DATA[4]  = 0.126287282;
      distrPU_DATA[5]  = 0.138848349;
      distrPU_DATA[6]  = 0.134116739;
      distrPU_DATA[7]  = 0.116909698;
      distrPU_DATA[8]  = 0.093739837;
      distrPU_DATA[9]  = 0.070092745;
      distrPU_DATA[10] = 0.049362671;
      distrPU_DATA[11] = 0.032974087;
      distrPU_DATA[12] = 0.020997589;
      distrPU_DATA[13] = 0.012791663;
      distrPU_DATA[14] = 0.007474022;
      distrPU_DATA[15] = 0.004196492;
      distrPU_DATA[16] = 0.002267743;
      distrPU_DATA[17] = 0.001181017;
      distrPU_DATA[18] = 0.000593481;
      distrPU_DATA[19] = 0.000288109;
      distrPU_DATA[20] = 0.000135272;
      distrPU_DATA[21] = 0.000061498;
      distrPU_DATA[22] = 0.000027102;
      distrPU_DATA[23] = 0.000011591;
      distrPU_DATA[24] = 0.000004816;
      distrPU_DATA[25] = 0.000001946;
      distrPU_DATA[26] = 0.000000766;
      distrPU_DATA[27] = 0.000000294;
      distrPU_DATA[28] = 0.000000110;
      distrPU_DATA[29] = 0.000000040;
      distrPU_DATA[30] = 0.000000014;
      distrPU_DATA[31] = 0.000000005;
      distrPU_DATA[32] = 0.000000002;
      distrPU_DATA[33] = 0.000000001;
      distrPU_DATA[34] = 0.000000000;
      */
      
      // matt
      distrPU_DATA[0]  = 0.00548268488655;
      distrPU_DATA[1]  = 0.0221623012347;
      distrPU_DATA[2]  = 0.0496891094541;
      distrPU_DATA[3]  = 0.0812509216863;
      distrPU_DATA[4]  = 0.108077184946;
      distrPU_DATA[5]  = 0.123760426259;
      distrPU_DATA[6]  = 0.126206245433;
      distrPU_DATA[7]  = 0.117184065729;
      distrPU_DATA[8]  = 0.100600998899;
      distrPU_DATA[9]  = 0.0807709135897;
      distrPU_DATA[10] = 0.0611758616346;
      distrPU_DATA[11] = 0.0439921259819;
      distrPU_DATA[12] = 0.0301786759644;
      distrPU_DATA[13] = 0.0194251837551;
      distrPU_DATA[14] = 0.0122283775737;
      distrPU_DATA[15] = 0.00756679368734;
      distrPU_DATA[16] = 0.00453053906389;
      distrPU_DATA[17] = 0.0026254307415;
      distrPU_DATA[18] = 0.00147069224801;
      distrPU_DATA[19] = 0.00079626771087;
      distrPU_DATA[20] = 0.000416930356137;
      distrPU_DATA[21] = 0.000212005583528;
      distrPU_DATA[22] = 0.000104839263429;
      distrPU_DATA[23] = 5.00184750011e-05;
      distrPU_DATA[24] = 2.32068721987e-05;
      distrPU_DATA[25] = 1.03418434519e-05;
      distrPU_DATA[26] = 4.60177033163e-06;
      distrPU_DATA[27] = 1.9020062108e-06;
      distrPU_DATA[28] = 7.77577472784e-07;
      distrPU_DATA[29] = 3.37391685271e-07;
      distrPU_DATA[30] = 1.34956674108e-07;
      distrPU_DATA[31] = 6.74783370542e-08;
      distrPU_DATA[32] = 2.77481012186e-08;
      distrPU_DATA[33] = 5.67574797652e-09;
      distrPU_DATA[34] = 2.52255465623e-09;
      
      distrPUUp_DATA[0]  = 0.005101118;
      distrPUUp_DATA[1]  = 0.019616973;
      distrPUUp_DATA[2]  = 0.042805932;
      distrPUUp_DATA[3]  = 0.069807865;
      distrPUUp_DATA[4]  = 0.094571814;
      distrPUUp_DATA[5]  = 0.111901835;
      distrPUUp_DATA[6]  = 0.118791863;
      distrPUUp_DATA[7]  = 0.115053281;
      distrPUUp_DATA[8]  = 0.102318265;
      distrPUUp_DATA[9]  = 0.085773677;
      distrPUUp_DATA[10] = 0.068378009;
      distrPUUp_DATA[11] = 0.052039150;
      distrPUUp_DATA[12] = 0.037905060;
      distrPUUp_DATA[13] = 0.026473492;
      distrPUUp_DATA[14] = 0.017752707;
      distrPUUp_DATA[15] = 0.011679462;
      distrPUUp_DATA[16] = 0.007506310;
      distrPUUp_DATA[17] = 0.004667612;
      distrPUUp_DATA[18] = 0.002804992;
      distrPUUp_DATA[19] = 0.001627845;
      distrPUUp_DATA[20] = 0.000911964;
      distrPUUp_DATA[21] = 0.000493186;
      distrPUUp_DATA[22] = 0.000267179;
      distrPUUp_DATA[23] = 0.000144413;
      distrPUUp_DATA[24] = 0.000075307;
      distrPUUp_DATA[25] = 0.000037894;
      distrPUUp_DATA[26] = 0.000018410;
      distrPUUp_DATA[27] = 0.000008643;
      distrPUUp_DATA[28] = 0.000003925;
      distrPUUp_DATA[29] = 0.000001801;
      distrPUUp_DATA[30] = 0.000000853;
      distrPUUp_DATA[31] = 0.000000387;
      distrPUUp_DATA[32] = 0.000000169;
      distrPUUp_DATA[33] = 0.000000071;
      distrPUUp_DATA[34] = 0.000000029;
      
      distrPUDown_DATA[0]  = 0.007079117;
      distrPUDown_DATA[1]  = 0.038273200;
      distrPUDown_DATA[2]  = 0.088145092;
      distrPUDown_DATA[3]  = 0.136151433;
      distrPUDown_DATA[4]  = 0.162401646;
      distrPUDown_DATA[5]  = 0.160292029;
      distrPUDown_DATA[6]  = 0.135059550;
      distrPUDown_DATA[7]  = 0.101830974;
      distrPUDown_DATA[8]  = 0.070044674;
      distrPUDown_DATA[9]  = 0.044358157;
      distrPUDown_DATA[10] = 0.025943806;
      distrPUDown_DATA[11] = 0.014429441;
      distrPUDown_DATA[12] = 0.007645309;
      distrPUDown_DATA[13] = 0.003811724;
      distrPUDown_DATA[14] = 0.001780658;
      distrPUDown_DATA[15] = 0.000772949;
      distrPUDown_DATA[16] = 0.000325344;
      distrPUDown_DATA[17] = 0.000135433;
      distrPUDown_DATA[18] = 0.000053306;
      distrPUDown_DATA[19] = 0.000019695;
      distrPUDown_DATA[20] = 0.000006740;
      distrPUDown_DATA[21] = 0.000002223;
      distrPUDown_DATA[22] = 0.000000764;
      distrPUDown_DATA[23] = 0.000000250;
      distrPUDown_DATA[24] = 0.000000077;
      distrPUDown_DATA[25] = 0.000000022;
      distrPUDown_DATA[26] = 0.000000006;
      distrPUDown_DATA[27] = 0.000000002;
      distrPUDown_DATA[28] = 0.000000001;
      distrPUDown_DATA[29] = 0.000000000;
      distrPUDown_DATA[30] = 0.000000000;
      distrPUDown_DATA[31] = 0.000000000;
      distrPUDown_DATA[32] = 0.000000000;
      distrPUDown_DATA[33] = 0.000000000;
      distrPUDown_DATA[34] = 0.000000000;
    }
    
    
    if(dataRunFlag == 20112)  // Run2011B
    {
      /*
      // mine
      distrPU_DATA[0]  = 0.000191236;
      distrPU_DATA[1]  = 0.001277422;
      distrPU_DATA[2]  = 0.004599973;
      distrPU_DATA[3]  = 0.011593057;
      distrPU_DATA[4]  = 0.022896772;
      distrPU_DATA[5]  = 0.037801772;
      distrPU_DATA[6]  = 0.054392517;
      distrPU_DATA[7]  = 0.070212059;
      distrPU_DATA[8]  = 0.083010413;
      distrPU_DATA[9]  = 0.091250703;
      distrPU_DATA[10] = 0.094289608;
      distrPU_DATA[11] = 0.092308313;
      distrPU_DATA[12] = 0.086107403;
      distrPU_DATA[13] = 0.076853909;
      distrPU_DATA[14] = 0.065835930;
      distrPU_DATA[15] = 0.054259304;
      distrPU_DATA[16] = 0.043106884;
      distrPU_DATA[17] = 0.033067599;
      distrPU_DATA[18] = 0.024529368;
      distrPU_DATA[19] = 0.017619424;
      distrPU_DATA[20] = 0.012271007;
      distrPU_DATA[21] = 0.008296419;
      distrPU_DATA[22] = 0.005451862;
      distrPU_DATA[23] = 0.003486181;
      distrPU_DATA[24] = 0.002171698;
      distrPU_DATA[25] = 0.001319390;
      distrPU_DATA[26] = 0.000782592;
      distrPU_DATA[27] = 0.000453665;
      distrPU_DATA[28] = 0.000257278;
      distrPU_DATA[29] = 0.000142872;
      distrPU_DATA[30] = 0.000077761;
      distrPU_DATA[31] = 0.000041515;
      distrPU_DATA[32] = 0.000021758;
      distrPU_DATA[33] = 0.000011202;
      distrPU_DATA[34] = 0.000011131;
      */
      
      // matt
      distrPU_DATA[0]  = 0.000112737003406;
      distrPU_DATA[1]  = 0.000777646125266;
      distrPU_DATA[2]  = 0.00293953727088;
      distrPU_DATA[3]  = 0.00779607746446;
      distrPU_DATA[4]  = 0.0161886140039;
      distrPU_DATA[5]  = 0.0280489939304;
      distrPU_DATA[6]  = 0.0422732741973;
      distrPU_DATA[7]  = 0.0570578977823;
      distrPU_DATA[8]  = 0.0704498952494;
      distrPU_DATA[9]  = 0.0808285880957;
      distrPU_DATA[10] = 0.087175540055;
      distrPU_DATA[11] = 0.0891353619476;
      distrPU_DATA[12] = 0.0869369566986;
      distrPU_DATA[13] = 0.0812446797496;
      distrPU_DATA[14] = 0.07298581133;
      distrPU_DATA[15] = 0.0631825589687;
      distrPU_DATA[16] = 0.0528079996742;
      distrPU_DATA[17] = 0.0426800503642;
      distrPU_DATA[18] = 0.0334009356879;
      distrPU_DATA[19] = 0.0253413520202;
      distrPU_DATA[20] = 0.0186609176278;
      distrPU_DATA[21] = 0.0133518691661;
      distrPU_DATA[22] = 0.00929217896527;
      distrPU_DATA[23] = 0.00629666691546;
      distrPU_DATA[24] = 0.00415877084982;
      distrPU_DATA[25] = 0.00267988512703;
      distrPU_DATA[26] = 0.00168652054605;
      distrPU_DATA[27] = 0.00103754971688;
      distrPU_DATA[28] = 0.000624559819431;
      distrPU_DATA[29] = 0.000368196397673;
      distrPU_DATA[30] = 0.000212766035973;
      distrPU_DATA[31] = 0.000120615027302;
      distrPU_DATA[32] = 6.71298899774e-05;
      distrPU_DATA[33] = 3.67081722824e-05;
      distrPU_DATA[34] = 1.97348922108e-05;
      
      distrPUUp_DATA[0]  = 0.000175480;
      distrPUUp_DATA[1]  = 0.001090057;
      distrPUUp_DATA[2]  = 0.003718595;
      distrPUUp_DATA[3]  = 0.009051823;
      distrPUUp_DATA[4]  = 0.017591955;
      distrPUUp_DATA[5]  = 0.029053016;
      distrPUUp_DATA[6]  = 0.042385317;
      distrPUUp_DATA[7]  = 0.056055315;
      distrPUUp_DATA[8]  = 0.068430483;
      distrPUUp_DATA[9]  = 0.078125663;
      distrPUUp_DATA[10] = 0.084223576;
      distrPUUp_DATA[11] = 0.086350687;
      distrPUUp_DATA[12] = 0.084638610;
      distrPUUp_DATA[13] = 0.079417400;
      distrPUUp_DATA[14] = 0.071824975;
      distrPUUp_DATA[15] = 0.062796235;
      distrPUUp_DATA[16] = 0.053169530;
      distrPUUp_DATA[17] = 0.043655232;
      distrPUUp_DATA[18] = 0.034792859;
      distrPUUp_DATA[19] = 0.026938293;
      distrPUUp_DATA[20] = 0.020275261;
      distrPUUp_DATA[21] = 0.014843629;
      distrPUUp_DATA[22] = 0.010576481;
      distrPUUp_DATA[23] = 0.007338748;
      distrPUUp_DATA[24] = 0.004961861;
      distrPUUp_DATA[25] = 0.003306743;
      distrPUUp_DATA[26] = 0.002164230;
      distrPUUp_DATA[27] = 0.001386295;
      distrPUUp_DATA[28] = 0.000869299;
      distrPUUp_DATA[29] = 0.000533795;
      distrPUUp_DATA[30] = 0.000321086;
      distrPUUp_DATA[31] = 0.000189270;
      distrPUUp_DATA[32] = 0.000109382;
      distrPUUp_DATA[33] = 0.000062004;
      distrPUUp_DATA[34] = 0.000034492;
      
      distrPUDown_DATA[0]  = 0.000210101;
      distrPUDown_DATA[1]  = 0.001763519;
      distrPUDown_DATA[2]  = 0.006569504;
      distrPUDown_DATA[3]  = 0.016411813;
      distrPUDown_DATA[4]  = 0.031616811;
      distrPUDown_DATA[5]  = 0.050520934;
      distrPUDown_DATA[6]  = 0.070044838;
      distrPUDown_DATA[7]  = 0.086847387;
      distrPUDown_DATA[8]  = 0.098343395;
      distrPUDown_DATA[9]  = 0.103216253;
      distrPUDown_DATA[10] = 0.101443574;
      distrPUDown_DATA[11] = 0.093736358;
      distrPUDown_DATA[12] = 0.082211003;
      distrPUDown_DATA[13] = 0.068738751;
      distrPUDown_DATA[14] = 0.054943100;
      distrPUDown_DATA[15] = 0.042068508;
      distrPUDown_DATA[16] = 0.030904604;
      distrPUDown_DATA[17] = 0.021809813;
      distrPUDown_DATA[18] = 0.014799979;
      distrPUDown_DATA[19] = 0.009663889;
      distrPUDown_DATA[20] = 0.006074341;
      distrPUDown_DATA[21] = 0.003726748;
      distrPUDown_DATA[22] = 0.002226559;
      distrPUDown_DATA[23] = 0.001290993;
      distrPUDown_DATA[24] = 0.000726991;
      distrPUDown_DATA[25] = 0.000397845;
      distrPUDown_DATA[26] = 0.000211670;
      distrPUDown_DATA[27] = 0.000109508;
      distrPUDown_DATA[28] = 0.000055085;
      distrPUDown_DATA[29] = 0.000026927;
      distrPUDown_DATA[30] = 0.000012779;
      distrPUDown_DATA[31] = 0.000000000;
      distrPUDown_DATA[32] = 0.000000000;
      distrPUDown_DATA[33] = 0.000000000;
      distrPUDown_DATA[34] = 0.000000000;
    }
    
    
    if(dataRunFlag == 201112)  // Run2011A + Run2011B
    {
      /*
      // mine
      distrPU_DATA[0]  = 0.002859418;
      distrPU_DATA[1]  = 0.012560310;
      distrPU_DATA[2]  = 0.029963130;
      distrPU_DATA[3]  = 0.051312994;
      distrPU_DATA[4]  = 0.070971273;
      distrPU_DATA[5]  = 0.084786385;
      distrPU_DATA[6]  = 0.091462672;
      distrPU_DATA[7]  = 0.091925524;
      distrPU_DATA[8]  = 0.087999381;
      distrPU_DATA[9]  = 0.081412680;
      distrPU_DATA[10] = 0.073399492;
      distrPU_DATA[11] = 0.064719088;
      distrPU_DATA[12] = 0.055832654;
      distrPU_DATA[13] = 0.047066260;
      distrPU_DATA[14] = 0.038698819;
      distrPU_DATA[15] = 0.030981110;
      distrPU_DATA[16] = 0.024117511;
      distrPU_DATA[17] = 0.018240983;
      distrPU_DATA[18] = 0.013399664;
      distrPU_DATA[19] = 0.009560713;
      distrPU_DATA[20] = 0.006628135;
      distrPU_DATA[21] = 0.004467347;
      distrPU_DATA[22] = 0.002929458;
      distrPU_DATA[23] = 0.001870567;
      distrPU_DATA[24] = 0.001164142;
      distrPU_DATA[25] = 0.000706805;
      distrPU_DATA[26] = 0.000419059;
      distrPU_DATA[27] = 0.000242856;
      distrPU_DATA[28] = 0.000137700;
      distrPU_DATA[29] = 0.000076458;
      distrPU_DATA[30] = 0.000041610;
      distrPU_DATA[31] = 0.000022214;
      distrPU_DATA[32] = 0.000011642;
      distrPU_DATA[33] = 0.000005994;
      distrPU_DATA[34] = 0.000005955;
      */
      
      // matt
      distrPU_DATA[0]  = 0.00203414674627;
      distrPU_DATA[1]  = 0.0093989457795;
      distrPU_DATA[2]  = 0.0234955075915;
      distrPU_DATA[3]  = 0.0420865073886;
      distrPU_DATA[4]  = 0.0606952653102;
      distrPU_DATA[5]  = 0.0752888090933;
      distrPU_DATA[6]  = 0.0839215023063;
      distrPU_DATA[7]  = 0.0867348804906;
      distrPU_DATA[8]  = 0.085047816952;
      distrPU_DATA[9]  = 0.0804197580772;
      distrPU_DATA[10] = 0.0741109810939;
      distrPU_DATA[11] = 0.0669425322596;
      distrPU_DATA[12] = 0.0593852931679;
      distrPU_DATA[13] = 0.0517138171608;
      distrPU_DATA[14] = 0.0441299860278;
      distrPU_DATA[15] = 0.0368259963707;
      distrPU_DATA[16] = 0.0299948235968;
      distrPU_DATA[17] = 0.0238112095942;
      distrPU_DATA[18] = 0.0184057946082;
      distrPU_DATA[19] = 0.0138473260247;
      distrPU_DATA[20] = 0.0101388028497;
      distrPU_DATA[21] = 0.00722638449542;
      distrPU_DATA[22] = 0.00501616832518;
      distrPU_DATA[23] = 0.00339323049781;
      distrPU_DATA[24] = 0.00223854969582;
      distrPU_DATA[25] = 0.00144140292082;
      distrPU_DATA[26] = 0.000906652403698;
      distrPU_DATA[27] = 0.000557587401343;
      distrPU_DATA[28] = 0.000335569614276;
      distrPU_DATA[29] = 0.000197799663015;
      distrPU_DATA[30] = 0.000114289785038;
      distrPU_DATA[31] = 6.47858088544e-05;
      distrPU_DATA[32] = 3.60559625478e-05;
      distrPU_DATA[33] = 1.97157233488e-05;
      distrPU_DATA[34] = 1.05993072984e-05;
      
      distrPUUp_DATA[0]  = 0.002568293;
      distrPUUp_DATA[1]  = 0.010394400;
      distrPUUp_DATA[2]  = 0.023729650;
      distrPUUp_DATA[3]  = 0.040231571;
      distrPUUp_DATA[4]  = 0.056554776;
      distrPUUp_DATA[5]  = 0.069837347;
      distrPUUp_DATA[6]  = 0.078487486;
      distrPUUp_DATA[7]  = 0.082270078;
      distrPUUp_DATA[8]  = 0.081912212;
      distrPUUp_DATA[9]  = 0.078544840;
      distrPUUp_DATA[10] = 0.073231079;
      distrPUUp_DATA[11] = 0.066789739;
      distrPUUp_DATA[12] = 0.059858777;
      distrPUUp_DATA[13] = 0.052730761;
      distrPUUp_DATA[14] = 0.045623668;
      distrPUUp_DATA[15] = 0.038720887;
      distrPUUp_DATA[16] = 0.032187037;
      distrPUUp_DATA[17] = 0.026167355;
      distrPUUp_DATA[18] = 0.020778600;
      distrPUUp_DATA[19] = 0.016098727;
      distrPUUp_DATA[20] = 0.012193037;
      distrPUUp_DATA[21] = 0.009063362;
      distrPUUp_DATA[22] = 0.006585137;
      distrPUUp_DATA[23] = 0.004675657;
      distrPUUp_DATA[24] = 0.003243813;
      distrPUUp_DATA[25] = 0.002198756;
      distrPUUp_DATA[26] = 0.001456219;
      distrPUUp_DATA[27] = 0.000942487;
      distrPUUp_DATA[28] = 0.000596269;
      distrPUUp_DATA[29] = 0.000368885;
      distrPUUp_DATA[30] = 0.000226736;
      distrPUUp_DATA[31] = 0.000138432;
      distrPUUp_DATA[32] = 0.000082865;
      distrPUUp_DATA[33] = 0.000048636;
      distrPUUp_DATA[34] = 0.000027994;
      
      distrPUDown_DATA[0]  = 0.003224979;
      distrPUDown_DATA[1]  = 0.016675379;
      distrPUDown_DATA[2]  = 0.039950598;
      distrPUDown_DATA[3]  = 0.066376626;
      distrPUDown_DATA[4]  = 0.088012539;
      distrPUDown_DATA[5]  = 0.100439124;
      distrPUDown_DATA[6]  = 0.103556134;
      distrPUDown_DATA[7]  = 0.099714980;
      distrPUDown_DATA[8]  = 0.091615185;
      distrPUDown_DATA[9]  = 0.081308804;
      distrPUDown_DATA[10] = 0.070202336;
      distrPUDown_DATA[11] = 0.058953498;
      distrPUDown_DATA[12] = 0.048042685;
      distrPUDown_DATA[13] = 0.037884112;
      distrPUDown_DATA[14] = 0.028827770;
      distrPUDown_DATA[15] = 0.021118648;
      distrPUDown_DATA[16] = 0.014915664;
      distrPUDown_DATA[17] = 0.010209606;
      distrPUDown_DATA[18] = 0.006741456;
      distrPUDown_DATA[19] = 0.004294284;
      distrPUDown_DATA[20] = 0.002638892;
      distrPUDown_DATA[21] = 0.001564150;
      distrPUDown_DATA[22] = 0.000893835;
      distrPUDown_DATA[23] = 0.000491965;
      distrPUDown_DATA[24] = 0.000265807;
      distrPUDown_DATA[25] = 0.000141758;
      distrPUDown_DATA[26] = 0.000073500;
      distrPUDown_DATA[27] = 0.000037046;
      distrPUDown_DATA[28] = 0.000018142;
      distrPUDown_DATA[29] = 0.000008623;
      distrPUDown_DATA[30] = 0.000006724;
      distrPUDown_DATA[31] = 0.000000000;
      distrPUDown_DATA[32] = 0.000000000;
      distrPUDown_DATA[33] = 0.000000000;
      distrPUDown_DATA[34] = 0.000000000;
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
                   const std::vector<int>* blacklist,
                   std::vector<float>* bTags)
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
    if(bTags)
      if(bTags->at(i) > 3.30) continue;
    
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
      if(bTags)
        if(bTags->at(j) > 3.30) continue;
      
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
