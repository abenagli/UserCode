#include "readEvents.h"






readEvents::readEvents(const std::string& inputDir,
                       const std::string& listFileName,
                       const std::string& baseRootFileName,
                       const std::string& histoName):
 m_inputDir(inputDir),
 m_listFileName(listFileName),
 m_baseRootFileName(baseRootFileName),
 m_histoName(histoName)
{
  
  //----------------------------------------------------------
  // read the file with the list of samples and cross sections
  //----------------------------------------------------------
  
  std::string listFullFileName = inputDir+listFileName;
  std::ifstream listFile( listFullFileName.c_str() );
  if(!listFile.is_open())
  {
    std::cout << "\n>>>readEvents::Error opening file " << listFullFileName << std::endl;
    exit(-1);
  }
  else
    std::cout << "\n>>>readEvents::Opening file " << listFullFileName << std::endl;
  
  
  
  while(!listFile.eof())
  {
    std::string sample;
    std::string sumName;
    int dataFlag;
    double mH;
    double crossSection;
    std::string jetAlgorithm;
    
    listFile >> sample >> sumName >> dataFlag >> mH >> crossSection >> jetAlgorithm;

    if(sample.size() == 0)
      continue;
    if(sample.at(0) == '#')
      continue;
    
    //std::cout << sample << "\t"
    //          << sumName << "\t"
    //          << dataFlag << "\t"
    //          << mH << "\t"
    //          << crossSection << "\t" 
    //          << jetAlgorithm << "\t" 
    //          << std::endl;
    
    std::pair<std::string, std::string> dummyPair(sample, sumName);
    m_list.push_back(dummyPair);
    m_dataFlag[sample] = dataFlag;
    m_mH[sample] = mH;
    m_crossSection[sample] = crossSection;
    
    std::pair<std::string, std::string> dummyPair2(sample, jetAlgorithm);
    m_jetAlgorithm.push_back(dummyPair2);
  }
  
  listFile.close();
  std::cout << ">>>readEvents::Read " << m_list.size() << " samples" << std::endl;
  std::cout << ">>>readEvents::Closing file " << listFullFileName << "\n" << std::endl;
  
  
  
  //---------------------------------------------
  // loop over all the samples and fill event map
  //---------------------------------------------

  std::vector<TFile*> rootFiles;
  int i = 0;
  for(std::vector<std::pair<std::string, std::string> >::const_iterator vecIt = m_list.begin();
      vecIt != m_list.end(); ++vecIt)
  {
    // open root file
    std::string fullRootFileName = m_inputDir+vecIt->first+"/"+m_baseRootFileName+"_"+m_jetAlgorithm.at(i).second+".root";
    //std::cout << "opening file: " << fullRootFileName << std::endl;
    rootFiles.push_back(new TFile(fullRootFileName.c_str(), "READ"));
    
    
    // get histogram
    //std::cout << "getting histogram " << m_fullHistoName << std::endl;
    TH1F* histo = NULL;
    rootFiles.at(i) -> GetObject(m_histoName.c_str(), histo);
    if(histo == NULL)
    {
      std::cout << ">>>readEvents::Error in getting object " << m_histoName << std::endl;
      exit(-1);
    }
    
    
    // buil event map
    std::map<int, double> eventMap;
    for(int bin = 1; bin <= histo->GetNbinsX(); ++bin)
      eventMap[bin] = histo -> GetBinContent(bin);
      
    // build event pair
    std::pair<std::string, std::map<int, double> > eventPair(vecIt->first, eventMap);
    
    // fill vector
    m_events.push_back(eventPair);
    
    
    ++i;
  }
  
}






readEvents::~readEvents()
{}
