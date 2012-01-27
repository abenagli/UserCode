#include"TVirtualFitter.h"

// this are the MC indeces of the relevant HLT names
//HLT_Ele25_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL_v5 236
//HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_v1 350
//HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30_v5 339
//HLT_IsoMu24_eta2p1_v3 147

void makeJetHltSyst(){
  
  bool onlyEle = false;
  
  int nBins_red = 100;
  int min_red = 0;
  int max_red = 1000;
  
  float lumiTot = 4680;
  std::vector<float> lumiPeriods;
  lumiPeriods.push_back(1956+337.5);
  lumiPeriods.push_back(1359);
  
  std::vector<int> testHltIndex;
  testHltIndex.push_back(350);
  testHltIndex.push_back(339);
  
  int refHltIndex = 236;
  
  const int n_periods = 2;
    
  TString inFileSigName_1 = "/gwterax2/users/ldimatt/NTUPLES/Fall11_v3/EGMu/VBFAnalysis_PFlow_allH_PT30_maxSumPt_maxDeta_Fall11_v3_EGMu_Run2011AB_WPt20/";
  TString inFileSigName_2 = "_7TeV-powheg-pythia6_abenagli-SQWaT_PAT_42X_Fall11_v3/VBFAnalysis_PFlow.root";
  std::vector<TString> vs_HiggsType;
  vs_HiggsType.push_back("GluGluToHToWWToLNuQQ_M-");
  vs_HiggsType.push_back("GluGluToHToWWToTauNuQQ_M-");
  vs_HiggsType.push_back("VBF_HToWWToLNuQQ_M-");
  vs_HiggsType.push_back("VBF_HToWWToTauNuQQ_M-");

  TCanvas* canSig = new TCanvas();
  TH1F* h_mass_HLT[9];
  TH1F* h_mass_noHLT[9];
  int massIndex = 0;

  for ( int iMass = 200; iMass <= 600; iMass += 50 ) {
    TString s_iMass = "";
    s_iMass += iMass;
    float crossSection; 
    int totEvents;
    // crazy root thing workaround
    TFile* supportFile = new TFile("supportFile.root","RECREATE");
    h_mass_HLT[massIndex] = new TH1F("h_mass_HLT_"+s_iMass,"h_mass_HLT_"+s_iMass,nBins_red, min_red, max_red);
    h_mass_noHLT[massIndex] = new TH1F("h_mass_noHLT_"+s_iMass,"h_mass_noHLT_"+s_iMass,nBins_red, min_red, max_red);
    for ( int iHiggs = 0; iHiggs < vs_HiggsType.size(); iHiggs++ ) {
 
      TString inFileSigName = inFileSigName_1 + vs_HiggsType[iHiggs] + s_iMass + inFileSigName_2;
      TFile* inFileSig = new TFile(inFileSigName,"READ");
      TTree* inTreeSig = (TTree*) inFileSig -> Get("ntu_15");
  
      inTreeSig -> SetBranchAddress("crossSection", &crossSection);
      inTreeSig -> SetBranchAddress("totEvents", &totEvents);
      inTreeSig -> GetEntry(0);
    
      for ( int iPeriod = 0; iPeriod < n_periods; iPeriod++ ) {
        TString thisCut;
        float thisWeight = 1./totEvents * crossSection * lumiPeriods[iPeriod];
        if ( onlyEle ) thisCut = Form ("(lep_flavour == 11) * ( HLT_Accept[236] ) * %f ", thisWeight) ;
        else thisCut = Form ("((lep_flavour == 11) * ( HLT_Accept[236] ) + (lep_flavour == 13) * ( HLT_Accept[147] )) * %f ", thisWeight) ;
        TString histoName = h_mass_noHLT[massIndex] -> GetName();
        supportFile->cd();
        inTreeSig -> Draw("lepNuW_m_KF >>+ " + histoName,thisCut);
        
        if ( onlyEle ) thisCut = Form ("(lep_flavour == 11) * ( HLT_Accept[%d] ) * %f ", testHltIndex[iPeriod], thisWeight) ;
        else thisCut = Form ("((lep_flavour == 11) * ( HLT_Accept[%d] ) + (lep_flavour == 13) * ( HLT_Accept[147] )) * %f ", testHltIndex[iPeriod], thisWeight) ;
        TString histoName = h_mass_HLT[massIndex] -> GetName();
        supportFile->cd();
        inTreeSig -> Draw("lepNuW_m_KF >>+ " + histoName,thisCut);
      }
 
    }
    
    float error = 0;
    float integral = 0;
    for ( int iLumi = 0; iLumi < n_periods; iLumi++ ) {
      for ( int iBin = 1; iBin <= h_mass_HLT[massIndex] -> GetXaxis() -> GetNbins(); iBin++ ) {
            
        integral += h_mass_HLT[massIndex] -> Integral(0, nBins_red+1) * lumiPeriods[iLumi]/lumiTot;
        error += (h_mass_noHLT[massIndex] -> Integral(0, nBins_red+1) - h_mass_HLT[massIndex] -> Integral(0, nBins_red+1)) * lumiPeriods[iLumi]/lumiTot;
 
      }
    }

    std::cout << "errore syst relativo per il segnale a " << s_iMass << " GeV/c^2 = " << error/integral * 100. << std::endl;
    massIndex++;
  }

}
