#include "analysisUtils.h"



int GetNeutrino(ROOT::Math::XYZTVector& nu1,
                ROOT::Math::XYZTVector& nu2,
                const ROOT::Math::XYZTVector& lep,
                const ROOT::Math::XYZTVector& met)
{
  float alpha = lep.px()*met.px() + lep.py()*met.py();

  float delta = (alpha + 0.5*80.399*80.399)*(alpha + 0.5*80.399*80.399) - lep.pt()*lep.pt()*met.pt()*met.pt();
  if( delta < 0 ) delta = 0.;

  float pz1 = ( lep.pz()*(alpha + 0.5*80.399*80.399) + lep.energy()*sqrt(delta) ) / lep.pt() / lep.pt();
  float pz2 = ( lep.pz()*(alpha + 0.5*80.399*80.399) - lep.energy()*sqrt(delta) ) / lep.pt() / lep.pt();
  
  if( fabs(pz1) < fabs(pz2) )
  {
    nu1.SetPxPyPzE(met.px(), met.py(), pz1, sqrt(met.px()*met.px() + met.py()*met.py() + pz1*pz1));
    nu2.SetPxPyPzE(met.px(), met.py(), pz2, sqrt(met.px()*met.px() + met.py()*met.py() + pz2*pz2));
  }
  else
  {
    nu1.SetPxPyPzE(met.px(), met.py(), pz2, sqrt(met.px()*met.px() + met.py()*met.py() + pz2*pz2));
    nu2.SetPxPyPzE(met.px(), met.py(), pz1, sqrt(met.px()*met.px() + met.py()*met.py() + pz1*pz1));  
  }
  
  if( delta > 0. ) return 2;
  else return 1;
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
