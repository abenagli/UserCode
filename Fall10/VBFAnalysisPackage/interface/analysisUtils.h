#ifndef analysisUtils_h
#define analysisUtils_h

#include <iostream>

#include "Math/Vector3D.h"
#include "Math/Vector4D.h"
#include "Math/Boost.h"



/** get the neutrino from W mass constraint  */
int GetNeutrino(ROOT::Math::XYZTVector& nu1,
                ROOT::Math::XYZTVector& nu2,
		const ROOT::Math::XYZTVector& lep,
		const ROOT::Math::XYZTVector& met);


/** get lnujj angles  */
void GetLNuJJAngles(float& cphi_lvjj,
                    float& cphi_lvz,
                    float& ctheta_l,
                    float& ctheta_j1,
                    float& ctheta_lv,
                    ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& l,
                    ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& v,
                    ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& j1,
                    ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& j2);

#endif
