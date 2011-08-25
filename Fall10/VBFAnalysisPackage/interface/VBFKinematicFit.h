#ifndef VBFKinematicFit_h
#define VBFKinematicFit_h

#include "treeReader.h"
#include "ntpleUtils.h"
#include "VBFAnalysisVariables.h"
#include "TKinFitterUtils.h"

#include "TFitConstraintMGaus.h"
#include "TFitConstraintEp.h"
#include "TFitParticleEtEtaPhi.h"
#include "TFitParticleCart.h"
#include "TKinFitter.h"



void DoKinematicFit(VBFAnalysisVariables& vars);

#endif
