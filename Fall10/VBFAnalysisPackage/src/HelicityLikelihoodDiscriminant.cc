#include "HelicityLikelihoodDiscriminant.h"
#include <cmath>

//using namespace RooFit;


HelicityLikelihoodDiscriminant::HelicityLikelihoodDiscriminant():
  mzz(0.0),
  costheta1( 0.0 ),
  costheta2( 0.0 ),
  costhetastar(0.0 ),
  phi( 0.0 ),
  phistar1(  0.0 )
{
  parfilename_="./ParametersPDF.txt";
  //
  NVARS=6;
  initBkgdPDFs();
  // cout<<" Background -> OK!"<<flush;
  initSignalPDFs();
  // cout<<"\tSignal->OK!"<<endl;
}//end constructor

HelicityLikelihoodDiscriminant::~HelicityLikelihoodDiscriminant(){
  delete background;
  delete signal;
  //
}

void HelicityLikelihoodDiscriminant::initBkgdPDFs(){
  //------------------ parameters for Bkg PDF -------------------------                                                    



     //===================== cos theta 1 ====================                                                     
  bkgd_h1_acca0_=-1.0;
  bkgd_h1_acca2_=-1.0;
  bkgd_h1_acca4_=-1.0;
  
  //===================== cos theta 2 ===================                                                      
  bkgd_h2_acca0_=0.0;
  bkgd_h2_acca2_=0.0;
  bkgd_h2_acca4_=0.0;
  bkgd_h2_g_=0.0;
  bkgd_h2_cutOff_=0.0;
  
  //===================== cos theta * ==================                                                       
  bkgd_hs_para2_=0.0;
  bkgd_hs_para4_=0.0;
  //===================== phi =========================
  bkgd_p_acca0_=-1.0;
  bkgd_p_acca1_=-1.0;
  bkgd_p_acca2_=-1.0;
  
  //===================== phi1* =======================                                                       
  bkgd_p1_acca0_=-1.0;
  bkgd_p1_acca1_=-1.0;
  bkgd_p1_acca2_=-1.0;

 setBkgdParameters();
 
 background = new RooBkgd2L2JV2("background","background",costheta1,costheta2,
				costhetastar,phi,phistar1,mzz,
				bkgd_h1_acca0_,
				bkgd_h1_acca2_,
				bkgd_h1_acca4_,
				bkgd_h2_acca0_,
				bkgd_h2_acca2_,
				bkgd_h2_acca4_,
				bkgd_h2_g_,
				bkgd_h2_cutOff_,
				bkgd_hs_para2_,
				bkgd_hs_para4_,
				bkgd_p_acca0_,
				bkgd_p_acca1_,
				bkgd_p_acca2_,
				bkgd_p1_acca0_,
				bkgd_p1_acca1_,
				bkgd_p1_acca2_);

}//end initBkgdPDFs

void HelicityLikelihoodDiscriminant::initSignalPDFs(){

sig_fppVal_=  0.0 ;
sig_fmmVal_=  0.0 ;
sig_fpmVal_=  0.0 ;
sig_fp0Val_=  0.0 ;
sig_f0mVal_=  0.0 ;
			       
sig_phippVal_= 0.0 ;
sig_phimmVal_= 0.0 ;
sig_phipmVal_= 0.0 ;
sig_phip0Val_= 0.0 ;
sig_phi0mVal_= 0.0 ;
		       
sig_fz1Val_= 0.0 ;
sig_fz2Val_= 0.0 ;
			       
sig_R1Val_= 0.0 ;
sig_R2Val_= 0.0 ;
			       
sig_para2_=0.0;
sig_para4_=0.0;
sig_acca0_=-999;
sig_acca1_=-999 ;
sig_acca2_= -999;
sig_a2_= -999;
sig_a4_= -999;
sig_b2_= -999;
sig_b4_= -999;
sig_N_= -999;
sig_g_= 0.0;
sig_cutOff_= 0.0;

  setSignalParameters();

  signal=new RooSpinZero5DV2("signal","signal",costheta1,costheta2,
		      costhetastar,phi,
		      phistar1,mzz,
		      sig_fppVal_, 
		      sig_fmmVal_, 
		      sig_fpmVal_, 
		      sig_fp0Val_, 
		      sig_f0mVal_, 
		      sig_phippVal_,
		      sig_phimmVal_,
		      sig_phipmVal_,
		      sig_phip0Val_,
		      sig_phi0mVal_,
		      sig_fz1Val_,
		      sig_fz2Val_, 
		      sig_R1Val_,
		      sig_R2Val_,
		      sig_para2_,
		      sig_para4_,
		      sig_acca0_,
		      sig_acca1_,
		      sig_acca2_,
		      sig_a2_,
		      sig_a4_,
		      sig_cutOff_,
		      sig_g_,
		      sig_b2_,
		      sig_b4_,
		      sig_N_);


}//end initSignalPDFs
void HelicityLikelihoodDiscriminant::init(){
  setSignalParameters();
  setBkgdParameters();

}//end init()


void HelicityLikelihoodDiscriminant::setParamFile(string myfilename){
  parfilename_=myfilename;
}//end setParamFile


double HelicityLikelihoodDiscriminant::getSignalProbability(){

  double sigL=-1.0;
  sigL=signal->evaluate();
  return sigL;
}

double HelicityLikelihoodDiscriminant::getBkgdProbability(){
  double bkgdL=-1.0;
  bkgdL=background->evaluate();
  return bkgdL;
}

void HelicityLikelihoodDiscriminant::setMeasurables(double newmzz,double newcostheta1,double newcostheta2,double newcosthetastar, double newphi, double newphistar1){

  vector<double> newVar;
  
  newVar.push_back(newcostheta1);
  newVar.push_back(newcostheta2);
  newVar.push_back(newcosthetastar);
  newVar.push_back(newphi);
  newVar.push_back(newphistar1);
  newVar.push_back(newmzz);
  signal->setVars(newVar);
  signal->SetAcceptanceParameters();

  background->setVars(newVar);
  background->SetParameters();


}

void HelicityLikelihoodDiscriminant::setMeasurables(vector<double> myvars){
  //
  if(int(myvars.size())<NVARS){
    cout<<"ERROR in HelicityLikelihoodDiscriminant::setRooVar ! Not enough values to set all RooRealVars: "<<myvars.size()<<endl;
      return;
  }
  costheta1=myvars.at(0);
  costheta2=myvars.at(1);
  costhetastar=myvars.at(2);
  phi=myvars.at(3);
  phistar1=myvars.at(4);
  mzz=myvars.at(5);

  signal->setVars(myvars);
  signal->SetAcceptanceParameters();

  background->setVars(myvars);
  background->SetParameters();
}

void HelicityLikelihoodDiscriminant::setMeasurables(HelicityAngles ha){
 costheta1=ha.helCosTheta1;
 costheta2=ha.helCosTheta2;
 costhetastar=ha.helCosThetaStar;
 phi=ha.helPhi;
 phistar1=ha.helPhi1;
 mzz=ha.mzz;

 signal->setVars(costheta1,costheta2,costhetastar,phi,phistar1,mzz);
 signal->SetAcceptanceParameters();

 background->setVars(costheta1,costheta2,
		     costhetastar,phi,
		     phistar1,mzz);
background->SetParameters();

}




void HelicityLikelihoodDiscriminant::setBkgdParameters(){
 bkgd_p_acca0_=-18.1027;
 bkgd_p_acca1_=1.31479;
 bkgd_p_acca2_=108.14;

 bkgd_p1_acca0_=-18.1027;
 bkgd_p1_acca1_=1.31479;
 bkgd_p1_acca2_=108.14;

 bkgd_hs_para2_=-2.40606;
 bkgd_hs_para4_=1.64613;

 bkgd_h2_acca0_=.780741;
 bkgd_h2_acca2_=.288787;
 bkgd_h2_acca4_=2.54104;
 bkgd_h2_g_=.0212292;
 bkgd_h2_cutOff_=.867301;

 bkgd_h1_acca0_=2.44216;
 bkgd_h1_acca2_=-1.74955;
 bkgd_h1_acca4_=1.57681;

  
}//end initBkgdParameters

////////////////STOPPED HERE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
void HelicityLikelihoodDiscriminant::setSignalParameters(){


  sig_fppVal_=.005;
  sig_fmmVal_=.005;
  sig_fpmVal_=0;
  sig_fp0Val_=0;
  sig_f0mVal_=0;
  
  sig_phippVal_=3.14159;
  sig_phimmVal_=3.14159;
  sig_phipmVal_=0;
  sig_phip0Val_=0;
  sig_phi0mVal_=0;
  
  sig_fz1Val_=.4;
  sig_fz2Val_=.4;
  
  sig_R1Val_=.15;
  sig_R2Val_=0;
  
  sig_para2_=-.475;
  sig_para4_=-.430;
  sig_acca0_=173.742;
  sig_acca1_=-1.41212;
  sig_acca2_=-25.4878;
  sig_a2_=.0303;
  sig_a4_=-1.07;
  sig_b2_=.043;
  sig_b4_=-1.02;
  sig_N_=8;
  sig_g_=.05;
  sig_cutOff_=.813;

}//end initSignalParameters


void HelicityLikelihoodDiscriminant::setParametersFromFile(){
  ifstream parfile(parfilename_.c_str(),ios::in);
  if( !parfile ) {
    cerr << "\nCouldn´t open parameter file" << endl<<endl;
    return;
  }


  bool readval=false;
  vector<double> h1bkgd_par, h2bkgd_par, cstarbkgd_par,phi1bkgd_par;
  vector<double> signal_par;

 while(!parfile.eof()){  

   string label;

   string tmplabel;
   float tmpval;
   if(!readval){
     parfile>>label;
     readval = true;
     continue;
   }
   else parfile>>tmpval;

   /*
   if(tmplabel=="H1BKGDPARS"){
   }
   else if(tmplabel=="H2BKGDPARS"){
   }
   else if(tmplabel=="CSTARBKGDPARS"){
   }
   else if(tmplabel=="PHI1BKGDPARS"){
   }
   else if(tmplabel=="SIGNALPARS"){
   }
   else{
     cout<<" HelicityLikelihoodDiscriminant::setParametersFromFile(): Unrecognized string in LD parameter file: "<<label.c_str()<<"  . Ignoring it."<<endl;
   }
   */

 }//end while loop

 // signal->setParams(signal_par);
 // h1Background->setParams(h1bkgd_par);
 // h2Background->setParams(h2bkgd_par);
 // cStarBackground->setParams(cstarbkgd_par);
 // tPhiBackground->setParams(tPhibkgd_par);


}



HelicityLikelihoodDiscriminant::HelicityAngles HelicityLikelihoodDiscriminant::computeHelicityAngles(TLorentzVector leptMinus, TLorentzVector leptPlus, TLorentzVector jet1, TLorentzVector jet2 ) {

  HelicityLikelihoodDiscriminant::HelicityAngles returnAngles;

  TLorentzVector Zll = leptPlus + leptMinus;
  TLorentzVector Zjj = jet1 + jet2;

  TLorentzVector Higgs = Zjj + Zll;

  
  // define lept1 as negatively charged lepton:
  TLorentzVector lept1 = leptMinus;
  TLorentzVector lept2 = leptPlus;

  // no charge for jets (well, not really)
  // so choose jet with positive scalar product with Zjj 
  // in its restframe:
  TLorentzVector jet1_Zjjstar_tmp(jet1);
  jet1_Zjjstar_tmp.Boost(-Zjj.BoostVector());
  if( jet1_Zjjstar_tmp.Phi()<0. ) { //swap them
    TLorentzVector jet1_tmp(jet1);
    TLorentzVector jet2_tmp(jet2);
    jet1 = jet2_tmp;
    jet2 = jet1_tmp;
  }


  //     BOOSTS:

  // boosts in Higgs CoM frame:
  TLorentzVector lept1_Hstar(lept1);
  lept1_Hstar.Boost(-Higgs.BoostVector());
  TLorentzVector lept2_Hstar(lept2);
  lept2_Hstar.Boost(-Higgs.BoostVector());
  TLorentzVector jet1_Hstar(jet1);
  jet1_Hstar.Boost(-Higgs.BoostVector());
  TLorentzVector jet2_Hstar(jet2);
  jet2_Hstar.Boost(-Higgs.BoostVector());
  TLorentzVector Zll_Hstar(Zll);
  Zll_Hstar.Boost(-Higgs.BoostVector());
  TLorentzVector Zjj_Hstar(Zjj);
  Zjj_Hstar.Boost(-Higgs.BoostVector());

  // boosts in Zll CoM frame:
  TLorentzVector lept1_Zllstar(lept1);
  lept1_Zllstar.Boost(-Zll.BoostVector());
  TLorentzVector H_Zllstar(Higgs);
  H_Zllstar.Boost(-Zll.BoostVector());

  // boosts in Zjj CoM frame:
  TLorentzVector jet1_Zjjstar(jet1);
  jet1_Zjjstar.Boost(-Zjj.BoostVector());
  TLorentzVector H_Zjjstar(Higgs);
  H_Zjjstar.Boost(-Zjj.BoostVector());


  returnAngles.helCosThetaStar = Zll_Hstar.CosTheta();


  TVector3 v_pbeamLAB( 0.0, 0.0, 1.0 );

  //cross prod beam x Zll
  TVector3 v_1 = (v_pbeamLAB.Cross(  (Zll_Hstar.Vect()).Unit()) ).Unit();//versor normal to z-z' plane


  //v_2 = cross prod l1 x l2 = versor normal to Zll decay plane
  // careful to the order: L1, the z-axis and Z->ll make a right-handed (non-orthogonal) frame (xyz); at the end we want the angle btw x and y
  TVector3 v_2((Zll_Hstar.Vect().Cross(lept1_Hstar.Vect().Unit())).Unit());


  //v_3 = similar to v_2, BUT
  //now, if we want a right-handed set of Unit-vectors, keeping the same direction of the z-axis
  //we must swap the direction of one of the other two vectors of the Z bosons. 
  //Keeping the same direction of the z-axis
  //means measuring phiZll and phiZjj w.r.t. to the same v_1 vector (i.e. w.r.t. the same z'-Zll plane)
  TVector3 v_3(((-1.0*Zjj_Hstar.Vect()).Cross(jet1_Hstar.Vect().Unit())).Unit()) ;

  //in other terms: we can define v_3 as above and then do the crss prod with v_1
  //or define v_3 in a way consistent with v_2 and then do the cross product with a newly defined
  //Unit vector v_4 =  (v_pbeamLAB.Cross(  (ZjjboostedX->momentum()).Unit()) ).Unit();//versor normal to z-Zjj plane
 
  // helphiZll:
  float phiZll = fabs( acos(v_1.Dot(v_2)) );
  if(v_pbeamLAB.Dot(v_2)>0.0)phiZll=-1.0*phiZll;
  else phiZll=+1.0*phiZll;

  // helphiZjj:
  float phiZjj = fabs( acos(v_1.Dot(v_3)) );
  if(v_pbeamLAB.Dot(v_3)>0.0)phiZjj=+1.0*phiZjj; 
  else phiZjj=-1.0*phiZjj;


  float phi1 = phiZll;


  //phi
  float phi = fabs( acos(v_2.Dot(v_3)) );//two-fold ambiguity when doing the acos + pi ambiguity from sign of v_3 
  if(lept1_Hstar.Vect().Dot(v_3)>0.0)phi= +1.0 * phi;
  else phi= -1.0 * phi;

  returnAngles.helPhi1 = phi1;
  returnAngles.helPhi = phi;


  returnAngles.helCosTheta1 =  (-1.0*(lept1_Zllstar.X()* H_Zllstar.X()+
                                   lept1_Zllstar.Y()* H_Zllstar.Y()+
                                   lept1_Zllstar.Z()* H_Zllstar.Z())/
                                  (lept1_Zllstar.Vect().Mag()* H_Zllstar.Vect().Mag())  );


  returnAngles.helCosTheta2 =  fabs( (jet1_Zjjstar.X()* H_Zjjstar.X()+
                                   jet1_Zjjstar.Y()* H_Zjjstar.Y()+
                                   jet1_Zjjstar.Z()* H_Zjjstar.Z())/
                                  (jet1_Zjjstar.Vect().Mag()* H_Zjjstar.Vect().Mag())  );

  returnAngles.mzz = Higgs.M();


  return returnAngles;

}

