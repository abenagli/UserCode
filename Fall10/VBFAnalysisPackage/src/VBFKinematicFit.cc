#include "VBFKinematicFit.h"



void DoKinematicFit(VBFAnalysisVariables& vars, const double& metEtaRes, const std::string& method,
                    const int& flag)
{
  // lepton
  TLorentzVector vl(vars.lep.px(),vars.lep.py(),vars.lep.pz(),vars.lep.energy());
  // neutrino
  TLorentzVector vv;
  if( flag == 1 )      vv.SetXYZT(vars.nu1.px(),vars.nu1.py(),vars.nu1.pz(),vars.nu1.energy());
  else if( flag == 2 ) vv.SetXYZT(vars.nu2.px(),vars.nu2.py(),vars.nu2.pz(),vars.nu2.energy());
  else                 vv.SetXYZT( vars.nu.px(), vars.nu.py(), vars.nu.pz(), vars.nu.energy());
  // W-jet 1
  TLorentzVector v1(vars.WJ1.px(),vars.WJ1.py(),vars.WJ1.pz(),vars.WJ1.energy());
  // W-jet 2
  TLorentzVector v2(vars.WJ2.px(),vars.WJ2.py(),vars.WJ2.pz(),vars.WJ2.energy());
  // unclusterized met
  double u_x = (vars.met+vars.lep+vars.WJ1+vars.WJ2).Px();
  double u_y = (vars.met+vars.lep+vars.WJ1+vars.WJ2).Py();
  
  
  
  // define error matrices
  TMatrixD ml(3,3);
  TMatrixD mv(3,3);
  TMatrixD m1(3,3);
  TMatrixD m2(3,3);
  ml.Zero();
  mv.Zero();
  m1.Zero();
  m2.Zero();
  
  
  // lepton
  double lepEtRes,lepEtaRes,lepPhiRes;
  if(vars.lep_flavour == 11) electronResolution(vars.lep.Et(),vars.lep.eta(),lepEtRes,lepEtaRes,lepPhiRes);
  if(vars.lep_flavour == 13)     muonResolution(vars.lep.Et(),vars.lep.eta(),lepEtRes,lepEtaRes,lepPhiRes);
  ml(0,0) = lepEtRes*lepEtRes;
  ml(1,1) = lepEtaRes*lepEtaRes;
  ml(2,2) = lepPhiRes*lepPhiRes;
  
  // neutrino
  double metEtRes,metPhiRes;
  PFMETResolution(vars.met.Et(),metEtRes,metPhiRes);
  mv(0,0) = metEtRes*metEtRes;
  mv(1,1) = metEtaRes*metEtaRes;
  mv(2,2) = metPhiRes*metPhiRes;
  
  // jets
  m1(0,0) = ErrEt (vars.WJ1.Et(),vars.WJ1.eta());
  m1(1,1) = ErrEta(vars.WJ1.Et(),vars.WJ1.eta());;
  m1(2,2) = ErrPhi(vars.WJ1.Et(),vars.WJ1.eta());;
  
  m2(0,0) = ErrEt (vars.WJ2.Et(),vars.WJ2.eta());
  m2(1,1) = ErrEta(vars.WJ2.Et(),vars.WJ2.eta());;
  m2(2,2) = ErrPhi(vars.WJ2.Et(),vars.WJ2.eta());;
  
  
  
  // define the constraints
  TLorentzVector tmpl = vl;
  TLorentzVector tmpv = vv;
  TLorentzVector tmp1 = v1;
  TLorentzVector tmp2 = v2;
  
  TFitParticleEtEtaPhi* l  = new TFitParticleEtEtaPhi("lepton","lepton",&tmpl,&ml);
  TFitParticleEtEtaPhi* v  = new TFitParticleEtEtaPhi("neutrino","neutrino",&tmpv,&mv);
  TFitParticleEtEtaPhi* j1 = new TFitParticleEtEtaPhi("WJet1","WJet1",&tmp1,&m1);
  TFitParticleEtEtaPhi* j2 = new TFitParticleEtEtaPhi("WJet2","WJet2",&tmp2,&m2);
  
  //std::cout << "flag: " << flag << "   ";
  //std::cout << "l: " << tmpl.Pz() << "   mass: " << tmpl.M() << std::endl;
  //std::cout << "v: " << tmpv.Pz() << "   mass: " << tmpv.M() << std::endl;
  //std::cout << "nSolutions: " << nSolutions << std::endl;
  //std::cout << "lv: mass: " << (tmpl+tmpv).M() << std::endl;
  //std::cout << "jj: mass: " << (tmp1+tmp2).M() << std::endl;
  TFitConstraintMGaus* mCons1 = new TFitConstraintMGaus( "W1MassConstraint", "W1Mass-Constraint", 0, 0 , 80.399, 2.085);
  mCons1->addParticles1( l, v );
  TFitConstraintMGaus* mCons2 = new TFitConstraintMGaus( "W2MassConstraint", "W2Mass-Constraint", 0, 0 , 80.399, 2.085);
  mCons2->addParticles1( j1, j2 );
  TFitConstraintEp* pxCons = new TFitConstraintEp( "PxConstraint", "Px-Constraint", 0, TFitConstraintEp::pX , u_x );
  pxCons->addParticles( l, v, j1, j2 );
  TFitConstraintEp* pyCons = new TFitConstraintEp( "PyConstraint", "Py-Constraint", 0, TFitConstraintEp::pY , u_y );
  pyCons->addParticles( l, v, j1, j2 );
  
  
  //Definition of the fitter
  //Add three measured particles(jets)
  //Add two constraints
  TKinFitter* fitter = new TKinFitter("fitter", "fitter");
  if( method == "MIB" )
  {
    fitter->addMeasParticle( l );
    fitter->addMeasParticle( v );
    fitter->addMeasParticle( j1 );
    fitter->addMeasParticle( j2 );
    fitter->addConstraint( mCons1 );
    fitter->addConstraint( mCons2 );
  }
  if( method == "FNAL" )
  {
    fitter->addMeasParticle( l );
    fitter->addMeasParticle( v );
    fitter->addMeasParticle( j1 );
    fitter->addMeasParticle( j2 );
    fitter->addConstraint( mCons1 );
    fitter->addConstraint( mCons2 );
    fitter->addConstraint( pxCons );
    fitter->addConstraint( pyCons );
  }
  if( method == "MIB_jetsOnly" )
  {
    fitter->addMeasParticle( j1 );
    fitter->addMeasParticle( j2 );
    fitter->addConstraint( mCons2 );
  }
    
  //Set convergence criteria
  fitter->setMaxNbIter( 500 );
  fitter->setMaxDeltaS( 1e-2 );
  fitter->setMaxF( 1e-2 );
  fitter->setVerbosity(0);
  
  //Perform the fit
  if( flag == 1 )      vars.status_KF1 = fitter->fit();
  else if( flag == 2 ) vars.status_KF2 = fitter->fit();
  else                 vars.status_KF  = fitter->fit();
  
  
  // update the higgs mass after kinematic fit
  if( (method == "MIB") || (method == "FNAL") )
  {
    if( flag == 1 )
    {
      vars.lep_KF1.SetPxPyPzE(l->getCurr4Vec()->Px(),l->getCurr4Vec()->Py(),l->getCurr4Vec()->Pz(),l->getCurr4Vec()->E()  );
      vars.p_lep_KF1 = &vars.lep_KF1;
      
      vars.nu_KF1.SetPxPyPzE(v->getCurr4Vec()->Px(),v->getCurr4Vec()->Py(),v->getCurr4Vec()->Pz(),v->getCurr4Vec()->E()  );
      vars.p_nu_KF1 = &vars.nu_KF1;
      
      vars.WJ1_KF1.SetPxPyPzE(j1->getCurr4Vec()->Px(),j1->getCurr4Vec()->Py(),j1->getCurr4Vec()->Pz(),j1->getCurr4Vec()->E()  );
      vars.p_WJ1_KF1 = &vars.WJ1_KF1;
      
      vars.WJ2_KF1.SetPxPyPzE(j2->getCurr4Vec()->Px(),j2->getCurr4Vec()->Py(),j2->getCurr4Vec()->Pz(),j2->getCurr4Vec()->E()  );
      vars.p_WJ2_KF1 = &vars.WJ2_KF1;
      
      vars.lepNu_m_KF1 = (vars.lep_KF1+vars.nu_KF1).mass();
      vars.lepNuW_m_KF1 = ( (*(l->getCurr4Vec())) + (*(v->getCurr4Vec())) + (*(j1->getCurr4Vec())) + (*(j2->getCurr4Vec())) ).M();
      vars.chi2_KF1 = fitter->getS();
      vars.ndf_KF1 = fitter->getNDF();
    }
    
    else if( flag == 2 )
    {
      vars.lep_KF2.SetPxPyPzE(l->getCurr4Vec()->Px(),l->getCurr4Vec()->Py(),l->getCurr4Vec()->Pz(),l->getCurr4Vec()->E()  );
      vars.p_lep_KF2 = &vars.lep_KF2;
      
      vars.nu_KF2.SetPxPyPzE(v->getCurr4Vec()->Px(),v->getCurr4Vec()->Py(),v->getCurr4Vec()->Pz(),v->getCurr4Vec()->E()  );
      vars.p_nu_KF2 = &vars.nu_KF2;
      
      vars.WJ1_KF2.SetPxPyPzE(j1->getCurr4Vec()->Px(),j1->getCurr4Vec()->Py(),j1->getCurr4Vec()->Pz(),j1->getCurr4Vec()->E()  );
      vars.p_WJ1_KF2 = &vars.WJ1_KF2;
      
      vars.WJ2_KF2.SetPxPyPzE(j2->getCurr4Vec()->Px(),j2->getCurr4Vec()->Py(),j2->getCurr4Vec()->Pz(),j2->getCurr4Vec()->E()  );
      vars.p_WJ2_KF2 = &vars.WJ2_KF2;
      
      vars.lepNu_m_KF2 = (vars.lep_KF2+vars.nu_KF2).mass();
      vars.lepNuW_m_KF2 = ( (*(l->getCurr4Vec())) + (*(v->getCurr4Vec())) + (*(j1->getCurr4Vec())) + (*(j2->getCurr4Vec())) ).M();
      vars.chi2_KF2 = fitter->getS();
      vars.ndf_KF2 = fitter->getNDF();
    }
    
    else
    {
      vars.lep_KF.SetPxPyPzE(l->getCurr4Vec()->Px(),l->getCurr4Vec()->Py(),l->getCurr4Vec()->Pz(),l->getCurr4Vec()->E()  );
      vars.p_lep_KF = &vars.lep_KF;
      
      vars.nu_KF.SetPxPyPzE(v->getCurr4Vec()->Px(),v->getCurr4Vec()->Py(),v->getCurr4Vec()->Pz(),v->getCurr4Vec()->E()  );
      vars.p_nu_KF = &vars.nu_KF;
      
      vars.WJ1_KF.SetPxPyPzE(j1->getCurr4Vec()->Px(),j1->getCurr4Vec()->Py(),j1->getCurr4Vec()->Pz(),j1->getCurr4Vec()->E()  );
      vars.p_WJ1_KF = &vars.WJ1_KF;
      
      vars.WJ2_KF.SetPxPyPzE(j2->getCurr4Vec()->Px(),j2->getCurr4Vec()->Py(),j2->getCurr4Vec()->Pz(),j2->getCurr4Vec()->E()  );
      vars.p_WJ2_KF = &vars.WJ2_KF;

      vars.lepNu_m_KF = (vars.lep_KF+vars.nu_KF).mass(); 
      vars.lepNuW_m_KF = ( (*(l->getCurr4Vec())) + (*(v->getCurr4Vec())) + (*(j1->getCurr4Vec())) + (*(j2->getCurr4Vec())) ).M();
      vars.chi2_KF = fitter->getS();
      vars.ndf_KF = fitter->getNDF();
    }
  }
  
  
  if( method == "MIB_jetsOnly" )
  {
    double metNew_px = vars.met.px() + (vars.WJ1.px() - j1->getCurr4Vec()->Px()) + (vars.WJ2.px() - j2->getCurr4Vec()->Px());
    double metNew_py = vars.met.py() + (vars.WJ1.py() - j1->getCurr4Vec()->Py()) + (vars.WJ2.py() - j2->getCurr4Vec()->Py());
    ROOT::Math::XYZTVector metNew(metNew_px,metNew_py,0.,sqrt(metNew_px*metNew_px+metNew_py*metNew_py));
    
    vars.lepNu_nSolutions_KF = GetNeutrino(vars.nu_KF1,vars.nu_KF2,vars.lep,metNew);
    vars.p_nu_KF1 = &(vars.nu_KF1);
    vars.p_nu_KF2 = &(vars.nu_KF2);
    
    if( flag == 1 )
    {
      TLorentzVector vNew(vars.nu_KF1.px(),vars.nu_KF1.py(),vars.nu_KF1.pz(),vars.nu_KF1.energy());
      
      vars.lep_KF1.SetPxPyPzE(l->getCurr4Vec()->Px(),l->getCurr4Vec()->Py(),l->getCurr4Vec()->Pz(),l->getCurr4Vec()->E()  );
      vars.p_lep_KF1 = &vars.lep_KF1;
      
      vars.WJ1_KF1.SetPxPyPzE(j1->getCurr4Vec()->Px(),j1->getCurr4Vec()->Py(),j1->getCurr4Vec()->Pz(),j1->getCurr4Vec()->E()  );
      vars.p_WJ1_KF1 = &vars.WJ1_KF1;
      
      vars.WJ2_KF1.SetPxPyPzE(j2->getCurr4Vec()->Px(),j2->getCurr4Vec()->Py(),j2->getCurr4Vec()->Pz(),j2->getCurr4Vec()->E()  );
      vars.p_WJ2_KF1 = &vars.WJ2_KF1;
      
      vars.lepNu_m_KF1 = (vars.lep_KF1+vars.nu_KF1).mass();
      vars.lepNuW_m_KF1 = ( (*(l->getCurr4Vec())) + vNew + (*(j1->getCurr4Vec())) + (*(j2->getCurr4Vec())) ).M();
      vars.chi2_KF1 = fitter->getS();
      vars.ndf_KF1 = fitter->getNDF();
    }
    
    else if( flag == 2 )
    {
      TLorentzVector vNew(vars.nu_KF2.px(),vars.nu_KF2.py(),vars.nu_KF2.pz(),vars.nu_KF2.energy());
      
      vars.lep_KF2.SetPxPyPzE(l->getCurr4Vec()->Px(),l->getCurr4Vec()->Py(),l->getCurr4Vec()->Pz(),l->getCurr4Vec()->E()  );
      vars.p_lep_KF2 = &vars.lep_KF2;
      
      vars.WJ1_KF2.SetPxPyPzE(j1->getCurr4Vec()->Px(),j1->getCurr4Vec()->Py(),j1->getCurr4Vec()->Pz(),j1->getCurr4Vec()->E()  );
      vars.p_WJ1_KF2 = &vars.WJ1_KF2;
      
      vars.WJ2_KF2.SetPxPyPzE(j2->getCurr4Vec()->Px(),j2->getCurr4Vec()->Py(),j2->getCurr4Vec()->Pz(),j2->getCurr4Vec()->E()  );
      vars.p_WJ2_KF2 = &vars.WJ2_KF2;
      
      vars.lepNu_m_KF2 = (vars.lep_KF2+vars.nu_KF2).mass();
      vars.lepNuW_m_KF2 = ( (*(l->getCurr4Vec())) + vNew + (*(j1->getCurr4Vec())) + (*(j2->getCurr4Vec())) ).M();
      vars.chi2_KF2 = fitter->getS();
      vars.ndf_KF2 = fitter->getNDF();
    }
    
    else
    {
      if( fabs(vars.nu_KF1.Pz()) < fabs(vars.nu_KF2.Pz()) )
        vars.nu_KF = vars.nu_KF1;
      else
        vars.nu_KF = vars.nu_KF2;
      
      vars.p_nu_KF = &(vars.nu_KF);
      TLorentzVector vNew(vars.nu_KF.px(),vars.nu_KF.py(),vars.nu_KF.pz(),vars.nu_KF.energy());
      
      vars.lep_KF.SetPxPyPzE(l->getCurr4Vec()->Px(),l->getCurr4Vec()->Py(),l->getCurr4Vec()->Pz(),l->getCurr4Vec()->E()  );
      vars.p_lep_KF = &vars.lep_KF;
      
      vars.WJ1_KF.SetPxPyPzE(j1->getCurr4Vec()->Px(),j1->getCurr4Vec()->Py(),j1->getCurr4Vec()->Pz(),j1->getCurr4Vec()->E()  );
      vars.p_WJ1_KF = &vars.WJ1_KF;
      
      vars.WJ2_KF.SetPxPyPzE(j2->getCurr4Vec()->Px(),j2->getCurr4Vec()->Py(),j2->getCurr4Vec()->Pz(),j2->getCurr4Vec()->E()  );
      vars.p_WJ2_KF = &vars.WJ2_KF;
      
      vars.lepNu_m_KF = (vars.lep_KF+vars.nu_KF).mass();
      vars.lepNuW_m_KF = ( (*(l->getCurr4Vec())) + vNew + (*(j1->getCurr4Vec())) + (*(j2->getCurr4Vec())) ).M();
      vars.chi2_KF = fitter->getS();
      vars.ndf_KF = fitter->getNDF();
    }
  }
  //std::cout << "lv: mass after KF: " << (vars.lep_KF+vars.nu_KF).M() << std::endl;
  //std::cout << "jj: mass after KF: " << (vars.WJ1_KF+vars.WJ2_KF).M() << std::endl;  
}
