#include "ntpleUtils.h"



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


bool FillChain(TChain* chain, const std::string& inputFileList)
{
  std::ifstream inFile(inputFileList.c_str());
  std::string buffer;
  std::string bufferbeg;
  
  if(!inFile.is_open())
  {
    std::cerr << "** ERROR: Can't open '" << inputFileList << "' for input" << std::endl;
    return false;
  }
  
  while(1)
  {
    inFile >> buffer;
    bufferbeg = buffer.substr(0,1);    
    
    if(!inFile.good()) break;
    if( bufferbeg == "#" ) continue;
    
    chain -> Add(buffer.c_str());
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






bool maggiore(double i, double j)
{
  return i > j;
}

bool  minore(double i, double j)
{
 return i < j;
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

//  ------------------------------------------------------------
