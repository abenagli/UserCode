#include "readEvents.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "TH1F.h"
#include "TRandom3.h"






int main(int argc, char** argv)
{
    std::cout << "ciao" << std::endl;
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> WplusWminsPseudoExperiments::usage: " << argv[0] << " inFileName" << std::endl ;
    return 1;
  }
  
  std::string inFileName = argv[1];
  
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
 
  std::string inputDir  = gConfigParser -> readStringOption("Input::inputDir");
  std::string outputDir = gConfigParser -> readStringOption("Output::outputDir");

  int step = gConfigParser -> readIntOption("Options::step");  
  float lumi = gConfigParser -> readFloatOption("Options::lumi");
  int expMAX = gConfigParser -> readIntOption("Options::expMAX");
  int expMODULO = gConfigParser -> readIntOption("Options::expMODULO");
  
  
  
  // define out file names
  std::string outputRootFileName = outputDir+"/WplusWminusPseudoExperiments.root";
  TFile* outputRootFile = new TFile(outputRootFileName.c_str(), "RECREATE");
  
  
  
  // get events
  readEvents* reader_plus  = new readEvents(inputDir, "listOfSamplesAndCrossSections.txt", "WplusWminusRatioAnalysis", "events_plus_2jets");
  readEvents* reader_minus = new readEvents(inputDir, "listOfSamplesAndCrossSections.txt", "WplusWminusRatioAnalysis", "events_minus_2jets");
  
  std::vector<std::pair<std::string, std::string > > samples_plus = reader_plus -> GetSamples();
  std::vector<std::pair<std::string, std::string > > samples_minus = reader_minus -> GetSamples();

  std::vector<std::pair<std::string, std::map<int, int> > > events_plus  = reader_plus  -> GetEvents();
  std::vector<std::pair<std::string, std::map<int, int> > > events_minus = reader_minus -> GetEvents();
  
  std::map<std::string, double> crossSections_plus  = reader_plus  -> GetCrossSections();  
  std::map<std::string, double> crossSections_minus = reader_minus -> GetCrossSections();
    
  delete reader_plus;
  delete reader_minus;
  
  
  
  
  
  
  // maps to store result for summed samples
  std::map<std::string, int> result_plus;
  std::map<std::string, int> result_minus;
  
  // create map of histos to store result of pseudoexperiments
  std::map<std::string, TH1F*> histoMap_plus;
  std::map<std::string, TH1F*> histoMap_minus;
  std::map<std::string, TH1F*> histoMap;
  std::map<std::string, TH1F*> histoMap_ratio;
  TH1F* histoAll = new TH1F("all", "", 1100, -99.5, 1000.5);
  TH1F* histoAll_ratio = new TH1F("all_ratio", "", 2000, -10., 10.);
  
  
  
  //****************************
  // LOOP OVER PSEUDOEXPERIMENTS
  
  TRandom3 r;
  for(int expIt = 0; expIt < expMAX; ++expIt)
  {
    if((expIt%expMODULO) == 0) std::cout << ">>>>> WplusWminusPseudoExperiments::PseudoExperiment " << expIt << std::endl;   
    
    
    // maps to store result for summed samples
    result_plus.clear();
    result_minus.clear();
    
    
    // loop over samples
    unsigned int i = 0;
    std::vector<std::pair<std::string, std::map<int, int> > >::const_iterator vecIt_plus  = events_plus.begin();    
    std::vector<std::pair<std::string, std::map<int, int> > >::const_iterator vecIt_minus = events_minus.begin();
    while( (vecIt_plus != events_plus.end()) && (vecIt_minus != events_minus.end()) )
    {
      // get sample name
      std::string sample_plus = vecIt_plus->first;
      std::string sample_minus = vecIt_minus->first;
      if(sample_plus != sample_minus)
      {
        std::cout << ">>>>> WplusWminusPseudoExperiments::Error!!! " << std::endl;
        return(1);
      }
      
      // get summed sample name
      std::string sampleSummed_plus  = (samples_plus.at(i)).second;
      std::string sampleSummed_minus = (samples_minus.at(i)).second;
      if(sampleSummed_plus != sampleSummed_minus)
      {
        std::cout << ">>>>> WplusWminusPseudoExperiments::Error!!! " << std::endl;
        return(1);
      }      
      
      
      std::map<int, int> eventMap_plus = vecIt_plus->second;
      std::map<int, int> eventMap_minus = vecIt_minus->second;
      //std::cout << sample << "   step = " << step << "  " << lumi*crossSections[sample]*eventMap[step]/eventMap[1] << std::endl;
      
      double mean_plus =  lumi * crossSections_plus[sample_plus]   * eventMap_plus[step]  / eventMap_plus[1];
      double mean_minus = lumi * crossSections_minus[sample_minus] * eventMap_minus[step] / eventMap_minus[1];
      
      double sigma_plus =  lumi * crossSections_plus[sample_plus]   * sqrt(eventMap_plus[step])  / eventMap_plus[1];
      double sigma_minus = lumi * crossSections_minus[sample_minus] * sqrt(eventMap_minus[step]) / eventMap_minus[1];
      
      double smearedMean_plus  = 0.;
      double smearedMean_minus = 0.;
      
      if( (mean_plus > 0.) && (sigma_plus > 0.) )
        while (smearedMean_plus <= 0.)
         smearedMean_plus = r.Gaus(mean_plus, sigma_plus);
      if( (mean_minus > 0.) && (sigma_minus > 0.) )
        while (smearedMean_minus <= 0.)
         smearedMean_minus = r.Gaus(mean_minus, sigma_minus);
      
      double val_plus = r.Poisson(smearedMean_plus);
      double val_minus = r.Poisson(smearedMean_minus);
      
      result_plus[sampleSummed_plus]   += val_plus;
      result_minus[sampleSummed_minus] += val_minus;
      
      ++i;
      ++vecIt_plus;
      ++vecIt_minus;
    } // loop over samples
    
    
    
    // sum samples
    double valAll_plus = 0.;
    double valAll_minus = 0.;
    std::map<std::string, int>::const_iterator mapIt_plus  = result_plus.begin();
    std::map<std::string, int>::const_iterator mapIt_minus = result_minus.begin();
    while( (mapIt_plus != result_plus.end()) && (mapIt_minus != result_minus.end()) )
    {
      if(histoMap[mapIt_plus->first] == NULL)
        histoMap[mapIt_plus->first] = new TH1F((mapIt_plus->first).c_str(), "", 1100, -99.5, 1000.5);

      if(histoMap_ratio[mapIt_plus->first] == NULL)
        histoMap_ratio[mapIt_plus->first] = new TH1F((mapIt_plus->first+"_ratio").c_str(), "", 2000, -10., 10.);

      if(histoMap_plus[mapIt_plus->first] == NULL)
        histoMap_plus[mapIt_plus->first] = new TH1F((mapIt_plus->first+"_plus").c_str(), "", 10000, 0.5, 10000.5);

      if(histoMap_minus[mapIt_minus->first] == NULL)
        histoMap_minus[mapIt_minus->first] = new TH1F((mapIt_minus->first+"_minus").c_str(), "", 10000, 0.5, 10000.5);
                      
      histoMap_plus[mapIt_plus->first]  -> Fill(mapIt_plus->second);
      histoMap_minus[mapIt_plus->first] -> Fill(mapIt_minus->second);
      histoMap[mapIt_plus->first] -> Fill(mapIt_plus->second - mapIt_minus->second);
      if( mapIt_plus->second > 0 && mapIt_minus->second > 0)
        histoMap_ratio[mapIt_plus->first] -> Fill(1.*mapIt_plus->second/mapIt_minus->second);
      valAll_plus += mapIt_plus->second;
      valAll_minus += mapIt_minus->second;
      
      ++mapIt_plus;
      ++mapIt_minus;
    }
    
    histoAll -> Fill(valAll_plus-valAll_minus);
    histoAll_ratio -> Fill(1.*valAll_plus/valAll_minus);
  }
  
  
  
  outputRootFile -> cd();
  for(std::map<std::string, TH1F*>::const_iterator mapIt = histoMap.begin();
      mapIt != histoMap.end(); ++mapIt)
  {
    (mapIt->second) -> Fit("gaus");
    (mapIt->second) -> Write();
    delete mapIt->second;
  }

  for(std::map<std::string, TH1F*>::const_iterator mapIt = histoMap_ratio.begin();
      mapIt != histoMap_ratio.end(); ++mapIt)
  {
    (mapIt->second) -> Fit("gaus");
    (mapIt->second) -> Write();
    delete mapIt->second;
  }
  
  for(std::map<std::string, TH1F*>::const_iterator mapIt = histoMap_plus.begin();
      mapIt != histoMap_plus.end(); ++mapIt)
  {
    (mapIt->second) -> Fit("gaus");
    (mapIt->second) -> Write();
    delete mapIt->second;
  }

  for(std::map<std::string, TH1F*>::const_iterator mapIt = histoMap_minus.begin();
      mapIt != histoMap_minus.end(); ++mapIt)
  {
    (mapIt->second) -> Fit("gaus");
    (mapIt->second) -> Write();
    delete mapIt->second;
  }
      
  histoAll -> Write();
  histoAll_ratio -> Write();
  
  outputRootFile -> Close();
  delete outputRootFile;
  
  return 0;
}
