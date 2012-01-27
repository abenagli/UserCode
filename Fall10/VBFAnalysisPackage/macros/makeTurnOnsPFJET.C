{
  
  int nBins_red = 100;
  int min_red = 0;
  int max_red = 100;

  const char *TurnOnTitle = "PFJet25 Efficiency;PFJet P_{T} (GeV/c);#epsilon";
  TString inFolder = "../data/PFJETto/";
  std::vector<TString> fileIn;
  fileIn.push_back("Run11B_PF_ntu_15_JetTurnOn");
  const int nEtaBins = 4;
  
  TF1 *erfit[nEtaBins];
  TH1F *h_num_red[nEtaBins];
  TH1F *h_den_red[nEtaBins];
  
  gStyle->SetStatX(0.9);
  gStyle->SetStatY(0.6);

  TFile* thisFile = new TFile(inFolder+fileIn.at(0)+".root","READ");
  
//  TFile* outfile = new TFile ("testJetTurnOn.root","RECREATE");

  for ( unsigned int iTO = 0; iTO < nEtaBins; iTO++ ) {
  
    TString s_iTO = "";
    s_iTO += iTO;
    TCanvas* c1 = new TCanvas();
    TH1F* num = (TH1F*) thisFile->Get("h_num25_"+s_iTO);
    TH1F* den = (TH1F*) thisFile->Get("h_den_"+s_iTO);
    
    // reduce the histos
    h_num_red[iTO] = new TH1F ("h_num_red_"+s_iTO,"den_red_"+s_iTO,nBins_red,min_red,max_red);
    h_den_red[iTO] = new TH1F ("h_den_red_"+s_iTO,"den_red_"+s_iTO,nBins_red,min_red,max_red);
    int iBin_red = 1;
    for ( int iBin = 1; iBin <= num->GetXaxis()->GetNbins(); iBin++ ) {
    
      if ( num->GetBinCenter(iBin) != h_num_red[iTO]->GetBinCenter(iBin_red) ) continue;
      h_num_red[iTO]->SetBinContent(iBin_red, num->GetBinContent(iBin) );
      h_den_red[iTO]->SetBinContent(iBin_red, den->GetBinContent(iBin) );
      iBin_red++;
    
    }
    
    h_num_red[iTO]->Rebin(2);
    h_den_red[iTO]->Rebin(2);
    
    TEfficiency *TurnOn = new TEfficiency(*h_num_red[iTO],*h_den_red[iTO]);
    erfit[iTO] = new TF1("erfit_"+s_iTO,"([0] + ([1]-[0])/2 * (1 +TMath::Erf((x-[2])/([3]*sqrt(2)))))",15,100);
    erfit[iTO]->SetParNames("A","B","Mu","Sigma");
    erfit[iTO]->SetParameters(0,1,25,10);
    erfit[iTO]->SetParLimits(1, -1, 1);
    TurnOn->Fit(erfit[iTO]);
    c1 -> cd();
    c1 -> SetGridx();
    c1 -> SetGridy();
    TurnOn -> SetMarkerStyle(20);
    TurnOn -> SetTitle(TurnOnTitle);
    TurnOn -> SetName("TurnOn");
    TurnOn -> Draw("AP");
    c1 -> SaveAs("jet_erfit_"+s_iTO+".gif","gif");
    
//    erfit[iTO] -> Write();

  }
  
  // ----------------- Make the turn on fit with TAsymm errors to get the errors
  
  TF1 *erfit_support[nEtaBins];
  TGraphAsymmErrors* g_TurnOn[nEtaBins];
  TH1F *h_erfit[nEtaBins];
    
  for ( unsigned int iTO = 0; iTO < nEtaBins; iTO++ ) {
  
    TString s_iTO = "";
    s_iTO += iTO;

    TCanvas* c1 = new TCanvas();
    
    g_TurnOn[iTO] = new TGraphAsymmErrors();
    g_TurnOn[iTO] -> BayesDivide(h_num_red[iTO],h_den_red[iTO]);
    
    h_erfit[iTO] = (TH1F*) h_num_red[iTO] -> Clone("ciao");

    erfit_support[iTO] = new TF1("erfit_support"+s_iTO,"([0] + ([1]-[0])/2 * (1 +TMath::Erf((x-[2])/([3]*sqrt(2)))))",15,100);
    erfit_support[iTO]->SetParNames("A","B","Mu","Sigma");
    erfit_support[iTO]->SetParameters(erfit[iTO]->GetParameter(0),erfit[iTO]->GetParameter(1),erfit[iTO]->GetParameter(2),erfit[iTO]->GetParameter(3));
    erfit_support[iTO]->SetParLimits(1, -1, 1);
    g_TurnOn[iTO] -> Fit(erfit_support[iTO],"RSE+");
    (TVirtualFitter::GetFitter()) -> GetConfidenceIntervals(h_erfit[iTO],0.68);
    c1 -> cd();
    c1 -> SetGridx();
    c1 -> SetGridy();
    g_TurnOn[iTO] -> SetMarkerStyle(20);
    g_TurnOn[iTO] -> SetTitle(TurnOnTitle);
    g_TurnOn[iTO] -> SetName("TurnOn");
    g_TurnOn[iTO] -> Draw("AP");
    h_erfit[iTO]->SetMarkerStyle(0);
    h_erfit[iTO]->SetFillColor(kRed);
    h_erfit[iTO]->SetFillStyle(3003);
    h_erfit[iTO]->Draw("same,e3");
    c1 -> SaveAs("jet_erfit_"+s_iTO+".gif","gif");

  }

  // Now make the histograms for the analysis corrections
  TH2F* jetEffHlt[5];
  TH2F* jetEffHltUp[5];
  TH2F* jetEffHltDown[5];
  int theBins  = h_num_red[0] -> GetXaxis() -> GetNbins();
  float theMin = h_num_red[0] -> GetXaxis() -> GetXmin();
  float theMax = h_num_red[0] -> GetXaxis() -> GetXmax();
  Double_t etaBoundaries[5] = {-2.4, -1., 0, 1., 2.4};
  // Run11A - May10th
  jetEffHlt[0] = new TH2F("jetEffHltZero","jetEffHltZero",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  jetEffHltUp[0] = new TH2F("jetEffHltZeroUp","jetEffHltZeroUp",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  jetEffHltDown[0] = new TH2F("jetEffHltZeroDown","jetEffHltZeroDown",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  for ( int iBinX = 1; iBinX <= jetEffHlt[0] -> GetNbinsX(); iBinX++ ) {
    for ( int iBinY = 1; iBinY <= jetEffHlt[0] -> GetNbinsY(); iBinY++ ) {
      jetEffHlt[0] -> SetBinContent(iBinX, iBinY, 1.);
      jetEffHlt[0] -> SetBinError(iBin, iBinY, 0.);
    }
  }
  // Run11A - v4-aug5-v6
  jetEffHlt[1] = new TH2F("jetEffHltOne","jetEffHltOne",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  jetEffHltUp[1] = new TH2F("jetEffHltOneUp","jetEffHltOneUp",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  jetEffHltDown[1] = new TH2F("jetEffHltOneDown","jetEffHltOneDown",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  for ( int iBinX = 1; iBinX <= jetEffHlt[1] -> GetNbinsX(); iBinX++ ) {
    for ( int iBinY = 1; iBinY <= jetEffHlt[1] -> GetNbinsY(); iBinY++ ) {
      jetEffHlt[1] -> SetBinContent(iBinX, iBinY, 1.);
      jetEffHlt[1] -> SetBinError(iBin, iBinY, 0.);
      jetEffHltUp[1] -> SetBinContent(iBinX, iBinY, 1.);
      jetEffHltDown[1] -> SetBinContent(iBinX, iBinY, 1.);
    }
  }
  // Run11B - normal
  jetEffHlt[2] = new TH2F("jetEffHltTwo","jetEffHltTwo",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  jetEffHltUp[2] = new TH2F("jetEffHltTwoUp","jetEffHltTwoUp",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  jetEffHltDown[2] = new TH2F("jetEffHltTwoDown","jetEffHltTwoDown",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  for ( int iBinX = 1; iBinX <= jetEffHlt[2] -> GetNbinsX(); iBinX++ ) {
    for ( int iBinY = 1; iBinY <= jetEffHlt[2] -> GetNbinsY(); iBinY++ ) {
      jetEffHlt[2] -> SetBinContent(iBinX, iBinY, 1.);
      jetEffHlt[2] -> SetBinError(iBin, iBinY, 0.);
      jetEffHltUp[2] -> SetBinContent(iBinX, iBinY, 1.);
      jetEffHltDown[2] -> SetBinContent(iBinX, iBinY, 1.);
    }
  }
  // Run11B - backup
  jetEffHlt[3] = new TH2F("jetEffHltThree","jetEffHltThree",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  jetEffHltUp[3] = new TH2F("jetEffHltThreeUp","jetEffHltThreeUp",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  jetEffHltDown[3] = new TH2F("jetEffHltThreeDown","jetEffHltThreeDown",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  for ( int iBinX = 1; iBinX <= jetEffHlt[3] -> GetNbinsX(); iBinX++ ) {
    for ( int iBinY = 1; iBinY <= jetEffHlt[3] -> GetNbinsY(); iBinY++ ) {
      jetEffHlt[3] -> SetBinContent(iBinX, iBinY, 1.);
      jetEffHlt[3] -> SetBinError(iBin, iBinY, 0.);
      jetEffHltUp[3] -> SetBinContent(iBinX, iBinY, 1.);
      jetEffHltUp[3] -> SetBinContent(iBinX, iBinY, 1.);
    }
  }
  // Run11B - PF
  jetEffHlt[4] = new TH2F("jetEffHltFour","jetEffHltFour",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  jetEffHltUp[4] = new TH2F("jetEffHltFourUp","jetEffHltFourUp",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  jetEffHltDown[4] = new TH2F("jetEffHltFourDown","jetEffHltFourDown",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  for ( int iBinY = 1; iBinY <= jetEffHlt[4] -> GetNbinsY(); iBinY++ ) {
    for ( int iBinX = 1; iBinX <= jetEffHlt[4] -> GetNbinsX(); iBinX++ ) {
      float thisPt =  jetEffHlt[4] -> GetXaxis() -> GetBinCenter(iBinX);
      float thisCorrection  = erfit[iBinY-1] -> Eval(thisPt);
      float thisError = h_erfit[iBinY-1]->GetBinError(iBinX);
      jetEffHlt[4] -> SetBinContent(iBinX, iBinY, thisCorrection);
      jetEffHlt[4] -> SetBinError(iBin, iBinY, thisError);
      jetEffHltUp[4] -> SetBinContent(iBinX, iBinY, thisCorrection + thisError);
      jetEffHltDown[4] -> SetBinContent(iBinX, iBinY, thisCorrection - thisError);
    }
  }
  
  const int nCorrections = 5;
  TFile* outfile = new TFile ("../data/PFJETto/PFJETto_corrections.root","RECREATE");
  for ( int i = 0; i < nCorrections; i++ ) jetEffHlt[i] -> Write();
  for ( int i = 0; i < nCorrections; i++ ) jetEffHltUp[i] -> Write();
  for ( int i = 0; i < nCorrections; i++ ) jetEffHltDown[i] -> Write();
}
