#ifndef readJSONFile_h
#define readJSONFile_h

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <map>





std::map<int, std::vector<std::pair<int, int> > >
 readJSONFile(const std::string& inFileName);



#endif
