#ifndef analysisUtils_h
#define analysisUtils_h

#include "ntpleUtils.h"

#include <iostream>

#include "TMath.h"
#include "TLorentzVector.h"

#include "Math/Vector3D.h"
#include "Math/Vector4D.h"
#include "Math/Boost.h"

/** get the electron Id with fixed reference efficiency **/
bool GetEleId(float pt,
              float eta,
              float mva, 
              float combIso, 
              float WPval, 
              bool Trig);

/** get the effective area for the isolation PU correction **/
float GetEffArea( int flavour,
                  float eta );

/** get the neutrino from W mass constraint **/
int GetNeutrino(ROOT::Math::XYZTVector& nu1,
                ROOT::Math::XYZTVector& nu2,
		const ROOT::Math::XYZTVector& lep,
		const ROOT::Math::XYZTVector& met);

/** get the neutrino from W mass constraint a' la FNAL **/
int GetNeutrino_FNAL(ROOT::Math::XYZTVector& nu1,
                     ROOT::Math::XYZTVector& nu2,
                     const ROOT::Math::XYZTVector& lep,
                     const ROOT::Math::XYZTVector& met);

/** get lnujj angles **/
void GetLNuJJAngles(float& cphi_lvjj,
                    float& cphi_lvz,
                    float& ctheta_l,
                    float& ctheta_j1,
                    float& ctheta_lv,
                    ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& l,
                    ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& v,
                    ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& j1,
                    ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& j2);

/** get lnujj angles a' la FNAL **/
void GetLNuJJAngles_FNAL(float& phi,
                         float& phistar2,
                         float& costheta1,
                         float& costheta2,
                         float& costhetastar,
                         ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& l,
                         ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& v,
                         ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& j1,
                         ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& j2);

#endif
