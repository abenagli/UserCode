#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "readJSONFile.h"

#include <iomanip>

#include "TH1F.h"






int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there                                                                                                                               
  if(argc != 2)
  {
    std::cerr << ">>>>> RunAndLumiSectionSelection::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file                                                                                                                                                          
  parseConfigFile (argv[1]) ;
  
  std::string inputFileList = gConfigParser -> readStringOption("Input::inputFileList");
  std::string jetAlgorithm  = gConfigParser -> readStringOption("Input::jetAlgorithm");
  std::string jsonFile      = gConfigParser -> readStringOption("Input::jsonFile");
  
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  
  int entryMAX = gConfigParser -> readIntOption("Options::entryMAX");
  int entryMODULO = gConfigParser -> readIntOption("Options::entryMODULO");
  
  std::map<int,int> totalEvents = GetTotalEvents("VBFPtMinLeptonCountFilterAll/totalEvents", inputFileList.c_str());  
  
  
  
  
  
  
  // Open old tree
  std::string treeName = "VBFNtuple" + jetAlgorithm + "/SimpleNtple";
  TChain* chain = new TChain(treeName.c_str());
  if(!FillChain(*chain, inputFileList.c_str())) return 1;
  treeReader reader((TTree*)(chain));
  
  
  
  // Open output root file for clone tree
  outputRootFileName += "_" + jetAlgorithm + ".root";
  TFile outputRootFile((outputRootFilePath+outputRootFileName).c_str(), "RECREATE");
  outputRootFile.cd();
  
  TTree* cloneTree = chain -> CloneTree(0);
  
  
  
  
  
  
  // define map of run/LS
  std::map<int, std::vector<std::pair<int, int> > > jsonMap = readJSONFile(jsonFile);  
  //for(std::map<int, std::vector<std::pair<int, int> > >::const_iterator mapIt = jsonMap.begin();
  //    mapIt != jsonMap.end(); ++mapIt)
  //{
  //  std::cout << ">>> run " << mapIt ->first << ":";
  //  for(unsigned int i = 0; i < (mapIt->second).size(); ++i)
  //    std::cout << "[" << ((mapIt->second).at(i)).first 
  //		<< "," << ((mapIt->second).at(i)).second
  //              << "] ";
  //  std::cout << std::endl;
  //}
  
  
  
  
  
  // Loop over events
  std::cout << ">>>>> RunAndLumiSectionSelection::Read " << chain -> GetEntries() << " entries" << std::endl;  
  for(int entry = 0; entry < chain -> GetEntries(); ++entry)
  {
    reader.GetEntry(entry);
    if((entry%entryMODULO) == 0) std::cout << ">>>>> RunAndLumiSectionSelection::GetEntry " << entry << std::endl;   
    if(entry == entryMAX) break;
    
    
    int runId  = reader.GetInt("runId")->at(0);
    int lumiId = reader.GetInt("lumiId")->at(0);
    
    if(AcceptEventByRunAndLumiSection(runId, lumiId, jsonMap) == false) continue;
    
    
    cloneTree -> Fill();
    
  } // loop over the events
  
  
  
  cloneTree -> AutoSave();
  outputRootFile.Close();
  
  return 0;
}
