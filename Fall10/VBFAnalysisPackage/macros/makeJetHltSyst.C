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
    
  TString inFileSigName_1 = "/gwterax2/users/ldimatt/NTUPLES/Fall11_v3/EGMu/VBFAnalysis_PFlow_allH_PT15_maxSumPt_maxDeta_Fall11_v3_EGMu_Run2011AB_NoHLT/";
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

  for ( int iMass = 400; iMass <= 400; iMass += 50 ) {
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
  
  gStyle->SetOptStat(0);

  // Draw the nice histo
  TCanvas* c1 = new TCanvas("c1","c1",600,600);
  c1 -> cd();
  TPad* p1 = new TPad("p1","p1",0., 0.25, 1., 1.);
  TPad* p2 = new TPad("p2","p2",0., 0., 1., 0.25);
  p1 -> Draw();
  p2 -> Draw();

  p1 -> cd();
  p1 -> SetGridx();
  p1 -> SetGridy();

  TLegend* leg = new TLegend(0.8,0.8,0.95,0.95);
  leg -> SetFillColor(kWhite);
  h_mass_HLT[0] -> SetLineColor(kRed);
  h_mass_HLT[0] -> SetLineStyle(2);
  h_mass_HLT[0] -> GetXaxis() -> SetRangeUser(200,600);  
  h_mass_HLT[0] -> GetXaxis() -> SetTitle("m_{l#nujj} (GeV/c^{2})");  
  h_mass_HLT[0] -> GetYaxis() -> SetTitle("a.u.");  
  h_mass_HLT[0] -> DrawNormalized();
  leg -> AddEntry(h_mass_HLT[0],"HLT","l");
  h_mass_noHLT[0] -> SetLineColor(kBlack);
  h_mass_noHLT[0] -> DrawNormalized("same");
  leg -> AddEntry(h_mass_noHLT[0],"no HLT","l");
  leg -> Draw();
  
  p2 -> cd();
  p2 -> SetGridx();
  p2 -> SetGridy();

  TH1F* h_ratio      = ratioHisto(h_mass_noHLT[0],h_mass_noHLT[0],0.95,1.05);
  TH1F* h_ratio_Up   = ratioHisto(h_mass_HLT[0],h_mass_noHLT[0],0.95,1.05);
  h_ratio      -> SetLineColor(kBlack);
  h_ratio_Up   -> SetLineColor(kRed);
  h_ratio_Up   -> SetLineStyle(2);
  h_ratio      -> SetLineWidth(2);
  h_ratio_Up   -> SetLineWidth(2);
  h_ratio -> GetXaxis() -> SetRangeUser(200,600);  
  h_ratio_Up -> GetXaxis() -> SetRangeUser(200,600);  
  h_ratio -> Draw();
  h_ratio_Up -> Draw("same");

  c1 -> Print("HLTonOff_mH.pdf","pdf");

}


TH1F* ratioHisto(TH1F* h_num, TH1F* h_den, const float& yMin, const float& yMax)
{
  TH1F* h_ratio = (TH1F*)( h_num->Clone() );
  h_ratio -> Divide(h_den);
  
  h_ratio -> GetYaxis() -> SetRangeUser(yMin,yMax);
  h_ratio -> GetXaxis() -> SetLabelSize(0.09);
  h_ratio -> GetYaxis() -> SetLabelSize(0.09);
  h_ratio -> GetXaxis() -> SetLabelFont(42);
  h_ratio -> GetYaxis() -> SetLabelFont(42);
  h_ratio -> GetYaxis() -> SetTitleSize(0.15);
  h_ratio -> GetYaxis() -> SetTitleFont(42);
  h_ratio -> GetYaxis() -> SetTitleOffset(0.42);
  h_ratio -> GetYaxis() -> SetNdivisions(206);
  h_ratio -> GetXaxis() -> SetTitle("");
  h_ratio -> GetYaxis() -> SetTitle("ratio");
  
  return h_ratio;
}
