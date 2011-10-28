//g++ -Wall -o makeKFactorsMap makeKFactorsMap.cpp

#include <iostream>
#include <fstream>
#include <vector>
#include <map>


int main()
{
  std::vector<int> masses;
  std::vector<std::string> inFileNames;

  inFileNames.push_back("kfactors_mh200_ren200_fac200.dat"); masses.push_back(200);
  inFileNames.push_back("kfactors_mh250_ren250_fac250.dat"); masses.push_back(250);
  inFileNames.push_back("kfactors_mh300_ren300_fac300.dat"); masses.push_back(300);
  inFileNames.push_back("kfactors_mh350_ren350_fac350.dat"); masses.push_back(350);
  inFileNames.push_back("kfactors_mh400_ren400_fac400.dat"); masses.push_back(400);
  inFileNames.push_back("kfactors_mh450_ren450_fac450.dat"); masses.push_back(450);
  inFileNames.push_back("kfactors_mh500_ren500_fac500.dat"); masses.push_back(500);
  inFileNames.push_back("kfactors_mh550_ren550_fac550.dat"); masses.push_back(550);
  inFileNames.push_back("kfactors_mh600_ren600_fac600.dat"); masses.push_back(600);
  
  std::map<int,std::vector<double> > xMins;
  std::map<int,std::vector<double> > xMaxs;
  std::map<int,std::vector<double> > kFactors;
  
  
  // loop on files
  unsigned int nFiles = inFileNames.size();
  for(unsigned int i = 0; i < nFiles; ++i)
  {
    std::ifstream inFile(inFileNames[i].c_str(),std::ios::in);
    std::cout << ">>> Reading file " << inFileNames[i] << std::endl;
    
    std::string buffer;
    for(int j = 0; j < 6; ++j) getline(inFile,buffer);
    
    int line = 0;
    double dummyXMin,dummyXMax,dummyKFactor;
    std::vector<double> xMin;
    std::vector<double> xMax;
    std::vector<double> kFactor;
    while(1)
    {
      inFile >> dummyXMin;
      inFile >> dummyXMax;
      inFile >> dummyKFactor;
      
      if(!inFile.good()) break;
      
      xMin.push_back(dummyXMin);
      xMax.push_back(dummyXMax);
      kFactor.push_back(dummyKFactor);
      //std::cout << "xMin: " << xMin[line] << "   xMax: " << xMax[line] << "   kFactor: " << kFactor[line] << std::endl;
      
      ++line; 
    }
    
    xMins[masses[i]] = xMin;
    xMaxs[masses[i]] = xMax;
    kFactors[masses[i]] = kFactor;
  }
  
  
  unsigned int nBins = xMins[masses[0]].size();
  
  
  
  std::ofstream outFile_cc("HiggsPtKFactors.cc",std::ios::out);

  outFile_cc << "#include \"HiggsPtKFactors.h\"" << std::endl;
  outFile_cc << std::endl;  
  outFile_cc << std::endl;  
  outFile_cc << std::endl;  
  outFile_cc << "double HiggsPtKFactors(const float& pt, const float& mH)" << std::endl;
  outFile_cc << "{" << std::endl;
  
  bool isFirstX = true;
  for(unsigned int bin = 0; bin < nBins; ++bin)
  {
    if(isFirstX == true)
    {
      outFile_cc << "  if(pt >= " << (xMins[masses[0]])[bin] << ". && pt <  " << (xMaxs[masses[0]])[bin] << ".)"<< std::endl;  
      isFirstX = false;
    }
    else
      outFile_cc << "  else if(pt >= " << (xMins[masses[0]])[bin] << ". && pt <  " << (xMaxs[masses[0]])[bin] << ".)"<< std::endl;      
    
    
    outFile_cc << "  {" << std::endl;
    outFile_cc << "    ";

    bool isFirstMass = true;
    for(unsigned int iFile = 0; iFile < nFiles; ++iFile)
    {
      if(isFirstMass == true)
      {
        outFile_cc << "if(mH == " << masses[iFile] << ".) return " << (kFactors[masses[iFile]])[bin] << "; ";
        isFirstMass = false;
      }
      else
        outFile_cc << "else if(mH == " << masses[iFile] << ".) return " << (kFactors[masses[iFile]])[bin] << "; ";
    }
    outFile_cc << "else return 1.; "<< std::endl;
    outFile_cc << "  }" << std::endl;
    
  }
  
  outFile_cc << "  else return 1.;" << std::endl;
  outFile_cc << "}" << std::endl;
  
  
  
  std::ofstream outFile_h("HiggsPtKFactors.h",std::ios::out);

  outFile_h << "#ifndef HiggsPtKFactors_h" << std::endl;
  outFile_h << "#define HiggsPtKFactors_h" << std::endl;
  outFile_h << std::endl;  
  outFile_h << std::endl;  
  outFile_h << std::endl;  
  outFile_h << "double HiggsPtKFactors(const float& pt, const float& mH);" << std::endl;
  outFile_h << std::endl;
  outFile_h << "#endif" << std::endl;
}
