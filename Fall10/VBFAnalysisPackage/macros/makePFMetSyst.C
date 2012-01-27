#include"TVirtualFitter.h"

void makePFMetSyst(){
  
  int nBins_red = 300;
  int min_red = 0;
  int max_red = 300;
  
  float lumiTot = 4680;
  std::vector<float> lumiPeriods;
  lumiPeriods.push_back(211.6);
  lumiPeriods.push_back(1956);
  lumiPeriods.push_back(337.5);
  lumiPeriods.push_back(1359);
  lumiPeriods.push_back(816);
  
  std::vector<TString> histoNames;
  histoNames.push_back("metEffHltZero");
  histoNames.push_back("metEffHltOne");
  histoNames.push_back("metEffHltTwo");
  histoNames.push_back("metEffHltThree");
  histoNames.push_back("metEffHltFour");
  
  const int n_periods = 5;
  
  TString inFileCorrName = "../data/PFMETto/PFMETto_corrections.root";
  TH1F* h_corr[n_periods];
  TFile* inFileCorr = new TFile(inFileCorrName,"READ");
  for ( int iFile = 0; iFile < n_periods; iFile++ ) {
    TString s_iFile = "";
    s_iFile += iFile;
    h_corr[iFile] = (TH1F*) inFileCorr -> Get(histoNames[iFile]);
  }
  
  TString inFileSigName_1 = "/gwterax2/users/ldimatt/NTUPLES/Fall11_v3/EGMu/VBFAnalysis_PFlow_allH_PT30_maxSumPt_maxDeta_Fall11_v3_EGMu_Run2011AB_WPt20/";
  TString inFileSigName_2 = "_7TeV-powheg-pythia6_abenagli-SQWaT_PAT_42X_Fall11_v3/VBFAnalysis_PFlow.root";
  std::vector<TString> vs_HiggsType;
  vs_HiggsType.push_back("GluGluToHToWWToLNuQQ_M-");
  vs_HiggsType.push_back("GluGluToHToWWToTauNuQQ_M-");
  vs_HiggsType.push_back("VBF_HToWWToLNuQQ_M-");
  vs_HiggsType.push_back("VBF_HToWWToTauNuQQ_M-");

  TCanvas* canSig = new TCanvas();
  TH1F* h_met_et[9];
  int massIndex = 0;

  for ( int iMass = 200; iMass <= 600; iMass += 50 ) {
    TString s_iMass = "";
    s_iMass += iMass;
    float crossSection; 
    int totEvents;
    // crazy root thing workaround
    TFile* supportFile = new TFile("supportFile.root","RECREATE");
    h_met_et[massIndex] = new TH1F("h_met_et_"+s_iMass,"h_met_et_"+s_iMass,nBins_red, min_red, max_red);
    for ( int iHiggs = 0; iHiggs < vs_HiggsType.size(); iHiggs++ ) {
 
      TString inFileSigName = inFileSigName_1 + vs_HiggsType[iHiggs] + s_iMass + inFileSigName_2;
      TFile* inFileSig = new TFile(inFileSigName,"READ");
      TTree* inTreeSig = (TTree*) inFileSig -> Get("ntu_15");
  
      inTreeSig -> SetBranchAddress("crossSection", &crossSection);
      inTreeSig -> SetBranchAddress("totEvents", &totEvents);
      inTreeSig -> GetEntry(0);
    
      float thisWeight = 1./totEvents * crossSection * lumiTot;
      TString thisCut = Form (" %f ", thisWeight) ;
      TString histoName = h_met_et[massIndex] -> GetName();
     
      supportFile->cd();
      inTreeSig -> Draw("met_et >>+ " + histoName,thisCut);
 
    }
    
    float error = 0;
    float integral = 0;
    for ( int iLumi = 0; iLumi < n_periods; iLumi++ ) {
      for ( int iBin = 1; iBin <= h_met_et[massIndex] -> GetXaxis() -> GetNbins(); iBin++ ) {
    
        int iBinCorr = h_corr[iLumi] -> GetXaxis() -> GetNbins();
        if ( h_met_et[massIndex] -> GetBinCenter(iBin) < h_corr[iLumi] -> GetXaxis() -> GetXmax() ) 
          iBinCorr = h_corr[iLumi] -> FindBin(h_met_et[massIndex] -> GetBinCenter(iBin));
        
        integral += h_met_et[massIndex] -> GetBinContent(iBin) * h_corr[iLumi] -> GetBinContent(iBinCorr) * lumiPeriods[iLumi]/lumiTot;
        error += h_met_et[massIndex] -> GetBinContent(iBin) * h_corr[iLumi] -> GetBinError(iBinCorr) * lumiPeriods[iLumi]/lumiTot;
 
      }
    }

    std::cout << "errore syst relativo per il segnale a " << s_iMass << " GeV/c^2 = " << error/integral * 100. << std::endl;
    massIndex++;
  }

}
