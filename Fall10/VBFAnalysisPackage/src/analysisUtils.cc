#include "analysisUtils.h"



int GetNeutrino(ROOT::Math::XYZTVector& nu1,
                ROOT::Math::XYZTVector& nu2,
                const ROOT::Math::XYZTVector& lep,
                const ROOT::Math::XYZTVector& met)
{
  float alpha = lep.px()*met.px() + lep.py()*met.py();

  float delta = (alpha + 0.5*80.399*80.399)*(alpha + 0.5*80.399*80.399) - lep.pt()*lep.pt()*met.pt()*met.pt();
  if( delta < 0. ) delta = 0.;

  float pz1 = ( lep.pz()*(alpha + 0.5*80.399*80.399) + lep.energy()*sqrt(delta) ) / lep.pt() / lep.pt();
  float pz2 = ( lep.pz()*(alpha + 0.5*80.399*80.399) - lep.energy()*sqrt(delta) ) / lep.pt() / lep.pt();
  
  nu1.SetPxPyPzE(met.px(), met.py(), pz1, sqrt(met.px()*met.px() + met.py()*met.py() + pz1*pz1));
  nu2.SetPxPyPzE(met.px(), met.py(), pz2, sqrt(met.px()*met.px() + met.py()*met.py() + pz2*pz2));
  
  
  if( delta > 0. )
  {
    if( fabs(pz1) > fabs(pz2) )
    {
      nu1.SetPxPyPzE(met.px(), met.py(), pz2, sqrt(met.px()*met.px() + met.py()*met.py() + pz2*pz2));
      nu2.SetPxPyPzE(met.px(), met.py(), pz1, sqrt(met.px()*met.px() + met.py()*met.py() + pz1*pz1));  
    }
  }
  
  else
  {
    /*
    float Dphi = deltaPhi(lep.phi(),met.phi());
    float newMet = 80.399*80.399 / ( 2. * lep.pt() * (1-cos(Dphi)) ); 

    
    nu1.SetPxPyPzE(newMet*cos(met.phi()), newMet*sin(met.phi()), pz1, sqrt(newMet*newMet + pz1*pz1));
    nu2.SetPxPyPzE(newMet*cos(met.phi()), newMet*sin(met.phi()), pz1, sqrt(newMet*newMet + pz1*pz1));
    */
  }
  
  
  if( delta > 0. ) return 2;
  else return 1;
}


//  ------------------------------------------------------------


int GetNeutrino_FNAL(ROOT::Math::XYZTVector& nu1,
                     ROOT::Math::XYZTVector& nu2,
                     const ROOT::Math::XYZTVector& lep,
                     const ROOT::Math::XYZTVector& met)
{
  double M_W  = 80.4;
  double M_mu =  lep.mass();
  double emu = lep.E();
  double pxmu = lep.px();
  double pymu = lep.py();
  double pzmu = lep.pz();
  double pxnu = met.px();
  double pynu = met.py();
  double pznu = 0.;
  double otherSol_ = 0.;
  
  double a = M_W*M_W - M_mu*M_mu + 2.0*pxmu*pxnu + 2.0*pymu*pynu;
  double A = 4.0*(emu*emu - pzmu*pzmu);
  double B = -4.0*a*pzmu;
  double C = 4.0*emu*emu*(pxnu*pxnu + pynu*pynu) - a*a;

  double tmproot = B*B - 4.0*A*C;
  
  bool isComplex_;
  
  if (tmproot<0)
  {
    isComplex_= true;
    pznu = - B/(2*A); // take real part of complex roots
    otherSol_ = pznu;

    // recalculate the neutrino pT
    // solve quadratic eq. discriminator = 0 for pT of nu
    double pnu = met.E();
    double Delta = (M_W*M_W - M_mu*M_mu);
    double alpha = (pxmu*pxnu/pnu + pymu*pynu/pnu);
    double ptmu = TMath::Sqrt( pxmu*pxmu + pymu*pymu); ptmu = TMath::Sqrt( pxmu*pxmu + pymu*pymu);
    double ptnu = TMath::Sqrt( pxnu*pxnu + pynu*pynu); // old
    double AA = 4.*pzmu*pzmu - 4*emu*emu + 4*alpha*alpha;
    double BB = 4.*alpha*Delta;
    double CC = Delta*Delta;
    
    double tmpdisc = BB*BB - 4.0*AA*CC;
    double tmpsolpt1 = (-BB + TMath::Sqrt(tmpdisc))/(2.0*AA);
    double tmpsolpt2 = (-BB - TMath::Sqrt(tmpdisc))/(2.0*AA);
    
    double newPtneutrino1_;
    double newPtneutrino2_;
    
    if ( fabs( tmpsolpt1 - ptnu ) < fabs( tmpsolpt2 - ptnu) ) { newPtneutrino1_ = tmpsolpt1; newPtneutrino2_ = tmpsolpt2;}
    else { newPtneutrino1_ = tmpsolpt2; newPtneutrino2_ = tmpsolpt1; }
    
    nu1.SetPxPyPzE(newPtneutrino1_*cos(met.phi()),newPtneutrino1_*sin(met.phi()),pznu,sqrt(newPtneutrino1_*newPtneutrino1_+pznu*pznu));
    nu2.SetPxPyPzE(newPtneutrino2_*cos(met.phi()),newPtneutrino2_*sin(met.phi()),pznu,sqrt(newPtneutrino2_*newPtneutrino2_+pznu*pznu));
    
    return 1;
  }
  
  else
  {
    isComplex_ = false;
    double tmpsol1 = (-B + TMath::Sqrt(tmproot))/(2.0*A);
    double tmpsol2 = (-B - TMath::Sqrt(tmproot))/(2.0*A);

    //std::cout << " Neutrino Solutions: " << tmpsol1 << ", " << tmpsol2 << std::endl;
    
    // two real roots, pick the one closest to pz of muon
    if (TMath::Abs(tmpsol2-pzmu) < TMath::Abs(tmpsol1-pzmu)) { pznu = tmpsol2; otherSol_ = tmpsol1;}
    else { pznu = tmpsol1; otherSol_ = tmpsol2; } 
    // if pznu is > 300 pick the most central root
    if ( pznu > 300. ) {
      if (TMath::Abs(tmpsol1)<TMath::Abs(tmpsol2) ) { pznu = tmpsol1; otherSol_ = tmpsol2; }
      else { pznu = tmpsol2; otherSol_ = tmpsol1; }
    }
    
    nu1.SetPxPyPzE(met.px(), met.py(), pznu, sqrt(met.px()*met.px() + met.py()*met.py() + pznu*pznu));
    nu2.SetPxPyPzE(met.px(), met.py(), otherSol_, sqrt(met.px()*met.px() + met.py()*met.py() + otherSol_*otherSol_));
    
    return 2;
  }

}


//  ------------------------------------------------------------


void GetLNuJJAngles(float& cphi_lvjj,
                    float& cphi_lvz,
                    float& ctheta_l,
                    float& ctheta_j1,
                    float& ctheta_lv,
       		    ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& l,
		    ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& v,
		    ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& j1,
		    ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& j2)
{
  ROOT::Math::XYZTVector l_lab  = l;
  ROOT::Math::XYZTVector v_lab  = v;
  ROOT::Math::XYZTVector j1_lab = j1;
  ROOT::Math::XYZTVector j2_lab = j2;
  
  ROOT::Math::XYZTVector lvjj_lab = l_lab+v_lab+j1_lab+j2_lab;
  ROOT::Math::XYZTVector lv_lab = l_lab+v_lab;
  ROOT::Math::XYZTVector jj_lab = j1_lab+j2_lab;
  
  
  
  
  
  
  // define boost in the Higgs system
  ROOT::Math::Boost boost_h;
  boost_h.SetComponents(-1./lvjj_lab.E()*lvjj_lab.Vect());
  
  // boost momenta in the Higgs system
  ROOT::Math::XYZTVector  l_h = boost_h *  l_lab;
  ROOT::Math::XYZTVector  v_h = boost_h *  v_lab;
  ROOT::Math::XYZTVector lv_h = boost_h * lv_lab;
  
  ROOT::Math::XYZTVector j1_h = boost_h * j1_lab;
  ROOT::Math::XYZTVector j2_h = boost_h * j2_lab;
  ROOT::Math::XYZTVector jj_h = boost_h * jj_lab;
  
  if( j2_h.E() > j1_h.E() )
  {
    j1_h = boost_h * j2_lab;;
    j2_h = boost_h * j1_lab;;
  }
  
  
  
  
  
  
  // find the angle between the two decay planes
  ROOT::Math::XYZVector lvplane_h = l_h.Vect().Cross(v_h.Vect());
  ROOT::Math::XYZVector jjplane_h = j1_h.Vect().Cross(j2_h.Vect());
  cphi_lvjj = lvplane_h.Dot(jjplane_h) / ( lvplane_h.R() * jjplane_h.R() );
  
  
  
  // find the angle between the production and the decay planes
  ROOT::Math::XYZVector z_lab(0.,0.,1.);
  
  ROOT::Math::XYZVector prodplane_h = z_lab.Cross(lv_h.Vect());
  ROOT::Math::XYZVector decplane_h = lv_h.Vect().Cross(l_h.Vect());
  cphi_lvz = prodplane_h.Dot(decplane_h) / ( prodplane_h.R() * decplane_h.R() );
  
  
  
  // define the (x',y',z') axes
  // z' is aligined wiht lv_h
  // x' is such that j1_h lays in the x',z' plane
  // y' follows as a consequence
  
  ROOT::Math::XYZVector z_h = lv_h.Vect();
  z_h /= z_h.R();
  
  ROOT::Math::XYZVector x_h = j1_h.Vect() - (j1_h.Vect().Dot(z_h))*z_h;
  x_h /= x_h.R();
  
  ROOT::Math::XYZVector y_h = z_h.Cross(x_h);
  y_h /= y_h.R();
  
  //std::cout << "ctheta(x-y) = " << x_h.Dot(y_h) << std::endl;
  //std::cout << "ctheta(y-z) = " << y_h.Dot(z_h) << std::endl;
  //std::cout << "ctheta(z-x) = " << z_h.Dot(x_h) << std::endl;
  
  
  
  // define boost in the lv system
  ROOT::Math::Boost boost_h_lv;
  boost_h_lv.SetComponents(-1./lv_h.E()*lv_h.Vect());
  
  // boost momenta in the lv system
  ROOT::Math::XYZTVector l_h_lv = boost_h_lv * l_h;
  
  
  
  // define boost in the jj system
  ROOT::Math::Boost boost_h_jj;
  boost_h_jj.SetComponents(-1./jj_h.E()*jj_h.Vect());
  
  // boost momenta in the jj system
  ROOT::Math::XYZTVector j1_h_jj = boost_h_jj * j1_h;
  
  
  
  
  
  
  // get the result
  ctheta_l  =  l_h_lv.Vect().Dot(+1.*z_h) /  l_h_lv.Vect().R();
  ctheta_j1 = j1_h_jj.Vect().Dot(-1.*z_h) / j1_h_jj.Vect().R();
  ctheta_lv = lv_h.Vect().Z() / lv_h.Vect().R();
  
  //std::cout << "ctheta(l-z')  = " << ctheta_l  << std::endl;
  //std::cout << "ctheta(j1-z') = " << ctheta_j1 << std::endl;
  //std::cout << "ctheta(lv-z)  = " << ctheta_lv << std::endl;
}


//  ------------------------------------------------------------


void GetLNuJJAngles_FNAL(float& phi,
                         float& phistar2,
                         float& costheta1,
                         float& costheta2,
                         float& costhetastar,
       		         ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& l,
		         ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& v,
		         ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& j1,
		         ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& j2)
{
  TLorentzVector thep4M11(l.px(),l.py(),l.pz(),l.energy());
  TLorentzVector thep4M12(v.px(),v.py(),v.pz(),v.energy());
  TLorentzVector thep4M21(j1.px(),j1.py(),j1.pz(),j1.energy());
  TLorentzVector thep4M22(j2.px(),j2.py(),j2.pz(),j2.energy());
  
  TLorentzVector thep4H = thep4M11 + thep4M12 + thep4M21 + thep4M22;
  TLorentzVector thep4Z1 = thep4M11 + thep4M12;
  TLorentzVector thep4Z2 = thep4M21 + thep4M22;
    
  double norm;
    
  TVector3 boostX = -(thep4H.BoostVector());
  TLorentzVector thep4Z1inXFrame( thep4Z1 );
  TLorentzVector thep4Z2inXFrame( thep4Z2 );      
  thep4Z1inXFrame.Boost( boostX );
  thep4Z2inXFrame.Boost( boostX );
  TVector3 theZ1X_p3 = TVector3( thep4Z1inXFrame.X(), thep4Z1inXFrame.Y(), thep4Z1inXFrame.Z() );
  TVector3 theZ2X_p3 = TVector3( thep4Z2inXFrame.X(), thep4Z2inXFrame.Y(), thep4Z2inXFrame.Z() );
    
  // calculate phi1, phi2, costhetastar
  ///phi1 = theZ1X_p3.Phi();
  ///phi2 = theZ2X_p3.Phi();
    
  ///////////////////////////////////////////////
  // check for z1/z2 convention, redefine all 4 vectors with convention
  /////////////////////////////////////////////// 
  TLorentzVector p4H, p4Z1, p4M11, p4M12, p4Z2, p4M21, p4M22;
  p4Z1 = thep4Z1; p4M11 = thep4M11; p4M12 = thep4M12;
  p4Z2 = thep4Z2; p4M21 = thep4M21; p4M22 = thep4M22;
  costhetastar = theZ1X_p3.CosTheta();
    
  // now helicity angles................................
  // ...................................................
  TVector3 boostZ1 = -(p4Z1.BoostVector());
  TLorentzVector p4Z2Z1(p4Z2);
  p4Z2Z1.Boost(boostZ1);
  //find the decay axis
  /////TVector3 unitx_1 = -Hep3Vector(p4Z2Z1);
  TVector3 unitx_1( -p4Z2Z1.X(), -p4Z2Z1.Y(), -p4Z2Z1.Z() );
  norm = 1/(unitx_1.Mag());
  unitx_1*=norm;
  //boost daughters of z2
  TLorentzVector p4M21Z1(p4M21);
  TLorentzVector p4M22Z1(p4M22);
  p4M21Z1.Boost(boostZ1);
  p4M22Z1.Boost(boostZ1);
  //create z and y axes
  /////TVector3 unitz_1 = Hep3Vector(p4M21Z1).cross(Hep3Vector(p4M22Z1));
  TVector3 p4M21Z1_p3( p4M21Z1.X(), p4M21Z1.Y(), p4M21Z1.Z() );
  TVector3 p4M22Z1_p3( p4M22Z1.X(), p4M22Z1.Y(), p4M22Z1.Z() );
  TVector3 unitz_1 = p4M21Z1_p3.Cross( p4M22Z1_p3 );
  norm = 1/(unitz_1.Mag());
  unitz_1 *= norm;
  TVector3 unity_1 = unitz_1.Cross(unitx_1);
    
  //caculate theta1
  TLorentzVector p4M11Z1(p4M11);
  p4M11Z1.Boost(boostZ1);
  TVector3 p3M11( p4M11Z1.X(), p4M11Z1.Y(), p4M11Z1.Z() );
  TVector3 unitM11 = p3M11.Unit();
  double x_m11 = unitM11.Dot(unitx_1); double y_m11 = unitM11.Dot(unity_1); double z_m11 = unitM11.Dot(unitz_1);
  TVector3 M11_Z1frame(y_m11, z_m11, x_m11);
  costheta1 = M11_Z1frame.CosTheta();
  //std::cout << "theta1: " << M11_Z1frame.Theta() << std::endl;
  //////-----------------------old way of calculating phi---------------/////////
  phi = M11_Z1frame.Phi();
    
  //set axes for other system
  TVector3 boostZ2 = -(p4Z2.BoostVector());
  TLorentzVector p4Z1Z2(p4Z1);
  p4Z1Z2.Boost(boostZ2);
  TVector3 unitx_2( -p4Z1Z2.X(), -p4Z1Z2.Y(), -p4Z1Z2.Z() );
  norm = 1/(unitx_2.Mag());
  unitx_2*=norm;
  //boost daughters of z2
  TLorentzVector p4M11Z2(p4M11);
  TLorentzVector p4M12Z2(p4M12);
  p4M11Z2.Boost(boostZ2);
  p4M12Z2.Boost(boostZ2);
  TVector3 p4M11Z2_p3( p4M11Z2.X(), p4M11Z2.Y(), p4M11Z2.Z() );
  TVector3 p4M12Z2_p3( p4M12Z2.X(), p4M12Z2.Y(), p4M12Z2.Z() );
  TVector3 unitz_2 = p4M11Z2_p3.Cross( p4M12Z2_p3 );
  norm = 1/(unitz_2.Mag());
  unitz_2*=norm;
  TVector3 unity_2 = unitz_2.Cross(unitx_2);
  //calcuate theta2
  TLorentzVector p4M21Z2(p4M21);
  p4M21Z2.Boost(boostZ2);
  TVector3 p3M21( p4M21Z2.X(), p4M21Z2.Y(), p4M21Z2.Z() );
  TVector3 unitM21 = p3M21.Unit();
  double x_m21 = unitM21.Dot(unitx_2); double y_m21 = unitM21.Dot(unity_2); double z_m21 = unitM21.Dot(unitz_2);
  TVector3 M21_Z2frame(y_m21, z_m21, x_m21);
  costheta2 = M21_Z2frame.CosTheta();
        
  // calculate phi
  //calculating phi_n
  TLorentzVector n_p4Z1inXFrame( p4Z1 );
  TLorentzVector n_p4M11inXFrame( p4M11 );
  n_p4Z1inXFrame.Boost( boostX );
  n_p4M11inXFrame.Boost( boostX );        
  TVector3 n_p4Z1inXFrame_unit = n_p4Z1inXFrame.Vect().Unit();
  TVector3 n_p4M11inXFrame_unit = n_p4M11inXFrame.Vect().Unit();  
  TVector3 n_unitz_1( n_p4Z1inXFrame_unit );
  //// y-axis is defined by neg lepton cross z-axis
  //// the subtle part is here...
  //////////TVector3 n_unity_1 = n_p4M11inXFrame_unit.Cross( n_unitz_1 );
  TVector3 n_unity_1 = n_unitz_1.Cross( n_p4M11inXFrame_unit );
  TVector3 n_unitx_1 = n_unity_1.Cross( n_unitz_1 );
    
  TLorentzVector n_p4M21inXFrame( p4M21 );
  n_p4M21inXFrame.Boost( boostX );
  TVector3 n_p4M21inXFrame_unit = n_p4M21inXFrame.Vect().Unit();
  //rotate into other plane
  TVector3 n_p4M21inXFrame_unitprime( n_p4M21inXFrame_unit.Dot(n_unitx_1), n_p4M21inXFrame_unit.Dot(n_unity_1), n_p4M21inXFrame_unit.Dot(n_unitz_1) );
    
  ///////-----------------new way of calculating phi-----------------///////
  //double phi_n =  n_p4M21inXFrame_unitprime.Phi();
  /*
     std::cout << "---------------------------" << std::endl;
     std::cout << "phi: " << phi << std::endl;
     std::cout << "phi_n: " << phi_n << std::endl;
     std::cout << "phi + phi_n: " << (phi+phi_n) << std::endl;
  */
  /// and then calculate phistar1
  TVector3 n_p4PartoninXFrame_unit( 0.0, 0.0, 1.0 );
  TVector3 n_p4PartoninXFrame_unitprime( n_p4PartoninXFrame_unit.Dot(n_unitx_1), n_p4PartoninXFrame_unit.Dot(n_unity_1), n_p4PartoninXFrame_unit.Dot(n_unitz_1) );
  // negative sign is for arrow convention in paper
  double phistar1 = (n_p4PartoninXFrame_unitprime.Phi());
    
  // and the calculate phistar2
  TLorentzVector n_p4Z2inXFrame( p4Z2 );
  n_p4Z2inXFrame.Boost( boostX );
  TVector3 n_p4Z2inXFrame_unit = n_p4Z2inXFrame.Vect().Unit();
  ///////TLorentzVector n_p4M21inXFrame( p4M21 );
  //////n_p4M21inXFrame.Boost( boostX );        
  ////TVector3 n_p4M21inXFrame_unit = n_p4M21inXFrame.Vect().Unit();  
  TVector3 n_unitz_2( n_p4Z2inXFrame_unit );
  //// y-axis is defined by neg lepton cross z-axis
  //// the subtle part is here...
  //////TVector3 n_unity_2 = n_p4M21inXFrame_unit.Cross( n_unitz_2 );
  TVector3 n_unity_2 = n_unitz_2.Cross( n_p4M21inXFrame_unit );
  TVector3 n_unitx_2 = n_unity_2.Cross( n_unitz_2 );
  TVector3 n_p4PartoninZ2PlaneFrame_unitprime( n_p4PartoninXFrame_unit.Dot(n_unitx_2), n_p4PartoninXFrame_unit.Dot(n_unity_2), n_p4PartoninXFrame_unit.Dot(n_unitz_2) );
  phistar2 = (n_p4PartoninZ2PlaneFrame_unitprime.Phi());
}
