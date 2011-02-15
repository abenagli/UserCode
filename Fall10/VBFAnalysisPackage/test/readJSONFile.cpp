#include "readJSONFile.h"

#include <iostream>



int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> readJSONFile::usage: " << argv[0] << " inFileName" << std::endl ;
    return 1;
  }
  
  
  std::string inFileName = argv[1];
  std::map<int, std::vector<std::pair<int, int> > > jsonMap = readJSONFile(inFileName);
  
  for(std::map<int, std::vector<std::pair<int, int> > >::const_iterator mapIt = jsonMap.begin();
      mapIt != jsonMap.end(); ++mapIt)
  {
    std::cout << "run: " << mapIt -> first;
    
    for(std::vector<std::pair<int, int> >::const_iterator vecIt = (mapIt->second).begin();
        vecIt != (mapIt->second).end(); ++vecIt)
    {
      std::cout << "[" << vecIt -> first << "," << vecIt->second << "] ";
    }    
    
    std::cout << std::endl;
  }
  
  
  return 0;
}
