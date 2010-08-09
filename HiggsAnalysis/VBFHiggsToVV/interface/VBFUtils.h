#ifndef VBFUtils_h
#define VBFUtils_h

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#include "TVector3.h"





template <class T>
struct deallocateVector: public std::unary_function<T, void>
{
  void operator() (T elem)
  {
    delete elem;
  }
};






template <class T>
void WriteNormalized(T* histo, double scale = 1.)
{
  histo -> Scale(scale / histo -> GetEntries());
  histo -> Write();
}






template <class T1, class T2>
std::map<float, std::pair<int, int> > MatchingDRMap(std::vector<T1>& collection1,
                                                    std::vector<T2>& collection2)
{
  // define map of all possible DRs
  typedef std::map<float, std::pair<int, int> > DRMap;
  DRMap myDRMap;
  myDRMap.clear();  

  
  // compute all DRs 
  for(unsigned int it1 = 0; it1 <  collection1.size(); ++it1)
  {
    for(unsigned int it2 = 0; it2 < collection2.size(); ++it2)
    {
      float DR = deltaR((collection1.at(it1)) -> eta(), (collection1.at(it1)) -> phi(),
                        (collection2.at(it2)) -> eta(), (collection2.at(it2)) -> phi());
      
  
      //std::cout << "eta1 = " << (collection1.at(it1)) -> eta() << "   eta2 = " << (collection2.at(it2)) -> eta() << "   DR = " << DR << std::endl;
    
      std::pair<int, int> dummy(it1, it2);
      myDRMap[DR] = dummy;
    }
  }
  
  
  //*************** Debugging ****************
  //std::cout << ">>> VBFUtils::MatchingDRMap <<<" << std::endl;
  //std::cout << "Collection1 size = " << collection1.size() << std::endl;
  //std::cout << "Collection2 size = " << collection2.size() << std::endl;
  //
  //for(DRMap::const_iterator mapIt = myDRMap.begin(); mapIt != myDRMap.end(); ++mapIt)
  //  std::cout << "   DR = "          << mapIt -> first
  //            << "   <it1, it2> = <" << (mapIt -> second).first
  //            << ","                 << (mapIt -> second).second
  //            << ">"
  //            << std::endl; 
  
  
  return myDRMap;
}

// --------------------------------------------------------------------------------------






template <class T1, class T2>
int GetMatching(std::vector<T1>& collection1,
                std::vector<T2>& collection2,
                const float& DRMax,
                bool checkCharge,
                float ptRatioMin,
                float ptRatioMax,
                std::vector<int>* matchIt1 = 0)

{
  // get the DR map between two collection of particles
  typedef std::map<float, std::pair<int, int> > DRMap;
  DRMap myDRMap = MatchingDRMap(collection1, collection2);
  
  
  
    
  // define boolean flags to avoid double usage of the same particle
  std::vector<bool> isUsed1;
  for(unsigned int i = 0; i < collection1.size(); ++ i)
    isUsed1.push_back(false);
  
  std::vector<bool> isUsed2;
  for(unsigned int i = 0; i < collection2.size(); ++ i)
    isUsed2.push_back(false);
  
  
  // intialize the vector which will store the result of the matching
  if(matchIt1 != 0)
  {
    (*matchIt1).clear();
    for(unsigned int i = 0; i < collection2.size(); ++i)
      (*matchIt1).push_back(-1);
  }
  
  // loop over the DRmap to get the smallest DR matchings
  unsigned int nMatching = 0;
  
  for(DRMap::const_iterator mapIt = myDRMap.begin();
      mapIt != myDRMap.end(); ++mapIt)
  {
    int it1 = (mapIt -> second).first;
    int it2 = (mapIt -> second).second;
    
    float DR = mapIt -> first;
    
    TVector3 momentum1(collection1.at(it1) -> px(),
                       collection1.at(it1) -> py(),
                       collection1.at(it1) -> pz());
    TVector3 momentum2(collection2.at(it2) -> px(),
                       collection2.at(it2) -> py(),
                       collection2.at(it2) -> pz());
    //float DTheta = fabs(momentum1.Angle(momentum2)); 
    
    //std::cout << "size = " << myDRMap.size() << "  DR = " << DR << "  it1 = " << it1 << "   it2 = " << it2 << std::endl;

    if( (checkCharge) && 
         (collection1.at(it1) -> charge() != (collection2.at(it2)) -> charge()) )
      continue;
    
    
    if( 1. * collection1.at(it1) -> pt() / (collection2.at(it2)) -> pt() < ptRatioMin) 
      continue;
    if( 1. * collection1.at(it1) -> pt() / (collection2.at(it2)) -> pt() > ptRatioMax) 
      continue;
    
    
    //if( (DTheta <= DRMax) && 
    //    (isUsed1.at(it1) == false) && (isUsed2.at(it2) == false) )
    if( (DR <= DRMax) && 
        (isUsed1.at(it1) == false) && (isUsed2.at(it2) == false) )
    {
      isUsed1.at(it1) = true; 
      isUsed2.at(it2) = true; 
      ++nMatching;
      
      if(matchIt1 != 0)
        (*matchIt1).at(it2) = it1;
    }
    
    
    if( (nMatching == collection2.size()) || (DR > DRMax) )
      break;    
  }
  
  
  //*************** Debugging ****************
  //std::cout << ">>> VBFUtils::GetMatching <<<" << std::endl;
  //std::cout << "nMatching = " << nMatching << std::endl;
  //std::cout << "   matchIt1.at(0) = " << matchIt1.at(0)
  //          << "   matchIt1.at(1) = " << matchIt1.at(1)
  //          << "   matchIt1.at(2) = " << matchIt1.at(2)
  //          << "   matchIt1.at(3) = " << matchIt1.at(3)
  //          << std::endl;
  
  
  return nMatching;
}

#endif
