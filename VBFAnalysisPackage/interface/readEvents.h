#ifndef readEvents_h
#define readEvents_h

#include <cstdlib>
#include <string>
#include <map>
#include <utility>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <sys/stat.h> 

#include "TFile.h"
#include "TH1F.h"



class readEvents
{
 public:
    
  //! ctor
  readEvents(const std::string& inputDir,
             const std::string& listFileName,
             const std::string& baseRootFileName,
             const std::string& histoName);
  
  //! dtor
  ~readEvents();
  
  
  //! methods
  std::vector<std::pair<std::string, std::string> > GetSamples() { return m_list; } ;
  std::vector<std::pair<std::string, std::map<int, int> > > GetEvents() { return m_events; } ;
  std::map<std::string, double> GetCrossSections() { return m_crossSection; } ;
  
  
 private:
  
  std::string m_inputDir;
  std::string m_listFileName;
  std::string m_baseRootFileName;
  std::string m_histoName;
  
  std::vector<std::pair<std::string, std::string> > m_list;
  std::vector<std::pair<std::string, std::map<int, int> > > m_events;
  std::map<std::string, int> m_dataFlag;
  std::map<std::string, double> m_mH;
  std::map<std::string, double> m_crossSection;
  std::vector<std::pair<std::string, std::string> > m_jetAlgorithm;
};

#endif

