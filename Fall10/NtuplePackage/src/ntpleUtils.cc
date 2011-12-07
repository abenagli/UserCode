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





double PURescaleFactor(const double& nPU_MC, const int& PUScale, const int& dataRunFlag, const int& MCFlag)
{
  if(nPU_MC < 0.)
    return 1.;
  
  else if(nPU_MC > 34.)
    return 1.;
  
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
    
    if(MCFlag == 20113) // Fall11 - in-time
    {
      distrPU_MC[0]  = 0.003388501;
      distrPU_MC[1]  = 0.010357558;
      distrPU_MC[2]  = 0.024724258;
      distrPU_MC[3]  = 0.042348605;
      distrPU_MC[4]  = 0.058279812;
      distrPU_MC[5]  = 0.068851751;
      distrPU_MC[6]  = 0.072914824;
      distrPU_MC[7]  = 0.071579609;
      distrPU_MC[8]  = 0.066811668;
      distrPU_MC[9]  = 0.060672356;
      distrPU_MC[10] = 0.054528356;
      distrPU_MC[11] = 0.049193540;
      distrPU_MC[12] = 0.044886042;
      distrPU_MC[13] = 0.041341896;
      distrPU_MC[14] = 0.038467900;
      distrPU_MC[15] = 0.035871463;
      distrPU_MC[16] = 0.033419520;
      distrPU_MC[17] = 0.030915649;
      distrPU_MC[18] = 0.028395374;
      distrPU_MC[19] = 0.025798107;
      distrPU_MC[20] = 0.023237445;
      distrPU_MC[21] = 0.020602754;
      distrPU_MC[22] = 0.018068800;
      distrPU_MC[23] = 0.015559693;
      distrPU_MC[24] = 0.013211063;
      distrPU_MC[25] = 0.010964293;
      distrPU_MC[26] = 0.008920993;
      distrPU_MC[27] = 0.007080504;
      distrPU_MC[28] = 0.005499239;
      distrPU_MC[29] = 0.004187022;
      distrPU_MC[30] = 0.003096474;
      distrPU_MC[31] = 0.002237361;
      distrPU_MC[32] = 0.001566428;
      distrPU_MC[33] = 0.001074149;
      distrPU_MC[34] = 0.000721755;
    }
    
    
    
    double* distrPU_DATA = new double[35];
    double* distrPUUp_DATA = new double[35];
    double* distrPUDown_DATA = new double[35];
    
    
    if(dataRunFlag == 20111)  // Run2011A
    {
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
