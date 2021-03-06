#ifndef ROOT_TREE_VECTOR_LINKDEF_H 
#define ROOT_TREE_VECTOR_LINKDEF_H 1

#include "Math/Vector4D.h"
#include "RooSuperCrystalBall.h"
#include "RooAttenuatedCrystalBallLowHigh.h"
#include "RooGaussWithSkirt.h"

#ifdef __CINT__

#pragma link off all classes;
#pragma link off all function;

#pragma link C++ class std::vector<float>;
#pragma link C++ class std::vector<Long64_t>;
#pragma link C++ class RooGaussWithSkirt;
#pragma link C++ class RooSuperCrystalBall;
#pragma link C++ class RooAttenuatedCrystalBallLowHigh;
#pragma link C++ function deltaEta (const double&, const double&);
#pragma link C++ function deltaPhi (const double&, const double&);
#pragma link C++ function deltaR (const double&, const double&, const double&, const double&);
#pragma link C++ function PURescaleFactor (const int&, const int&, const int&, const int&);


#endif

#endif
