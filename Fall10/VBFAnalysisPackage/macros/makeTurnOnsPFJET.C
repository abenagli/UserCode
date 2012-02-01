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
  gStyle->SetStatY(0.5);
  gStyle->SetStatFontSize(15);
  gStyle->SetStatFont(4);

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
    c1 -> SaveAs("jet_erfit_"+s_iTO+".pdf","pdf");
    
//    erfit[iTO] -> Write();

  }
  
  // ----------------- Make the turn on fit with TAsymm errors to get the errors
  
  TF1 *erfit_support[nEtaBins];
  TGraphAsymmErrors* g_TurnOn[nEtaBins];
  TH1F *h_erfit[nEtaBins];
  
  std::vector<TString> latexNames;  
  latexNames.push_back("-2.4 < #eta < -1.5");
  latexNames.push_back("-1.5 < #eta < 0");
  latexNames.push_back("0 < #eta < 1.5");
  latexNames.push_back("1.5 < #eta < 2.4");

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
    c1 -> SaveAs("jet_erfit_"+s_iTO+".pdf","pdf");

  }

  // Now make the histograms for the analysis corrections
  TH2F* jetEffHLT[5];
  TH2F* jetEffHLTUp[5];
  TH2F* jetEffHLTDown[5];
  int theBins  = h_num_red[0] -> GetXaxis() -> GetNbins();
  float theMin = h_num_red[0] -> GetXaxis() -> GetXmin();
  float theMax = h_num_red[0] -> GetXaxis() -> GetXmax();
  Double_t etaBoundaries[5] = {-2.4, -1., 0, 1., 2.4};
  // Run11A - May10th
  jetEffHLT[0] = new TH2F("jetEffHLT0","jetEffHLT0",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  jetEffHLTUp[0] = new TH2F("jetEffHLT0Up","jetEffHLT0Up",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  jetEffHLTDown[0] = new TH2F("jetEffHLT0Down","jetEffHLT0Down",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  for ( int iBinX = 1; iBinX <= jetEffHLT[0] -> GetNbinsX(); iBinX++ ) {
    for ( int iBinY = 1; iBinY <= jetEffHLT[0] -> GetNbinsY(); iBinY++ ) {
      jetEffHLT[0] -> SetBinContent(iBinX, iBinY, 1.);
      jetEffHLT[0] -> SetBinError(iBin, iBinY, 0.);
    }
  }
  // Run11A - v4-aug5-v6
  jetEffHLT[1] = new TH2F("jetEffHLT1","jetEffHLT1",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  jetEffHLTUp[1] = new TH2F("jetEffHLT1Up","jetEffHLT1Up",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  jetEffHLTDown[1] = new TH2F("jetEffHLT1Down","jetEffHLT1Down",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  for ( int iBinX = 1; iBinX <= jetEffHLT[1] -> GetNbinsX(); iBinX++ ) {
    for ( int iBinY = 1; iBinY <= jetEffHLT[1] -> GetNbinsY(); iBinY++ ) {
      jetEffHLT[1] -> SetBinContent(iBinX, iBinY, 1.);
      jetEffHLT[1] -> SetBinError(iBin, iBinY, 0.);
      jetEffHLTUp[1] -> SetBinContent(iBinX, iBinY, 1.);
      jetEffHLTDown[1] -> SetBinContent(iBinX, iBinY, 1.);
    }
  }
  // Run11B - normal
  jetEffHLT[2] = new TH2F("jetEffHLT2","jetEffHLT2",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  jetEffHLTUp[2] = new TH2F("jetEffHLT2Up","jetEffHLT2Up",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  jetEffHLTDown[2] = new TH2F("jetEffHLT2Down","jetEffHLT2Down",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  for ( int iBinX = 1; iBinX <= jetEffHLT[2] -> GetNbinsX(); iBinX++ ) {
    for ( int iBinY = 1; iBinY <= jetEffHLT[2] -> GetNbinsY(); iBinY++ ) {
      jetEffHLT[2] -> SetBinContent(iBinX, iBinY, 1.);
      jetEffHLT[2] -> SetBinError(iBin, iBinY, 0.);
      jetEffHLTUp[2] -> SetBinContent(iBinX, iBinY, 1.);
      jetEffHLTDown[2] -> SetBinContent(iBinX, iBinY, 1.);
    }
  }
  // Run11B - backup
  jetEffHLT[3] = new TH2F("jetEffHLT3","jetEffHLT3",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  jetEffHLTUp[3] = new TH2F("jetEffHLT3Up","jetEffHLT3Up",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  jetEffHLTDown[3] = new TH2F("jetEffHLT3Down","jetEffHLT3Down",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  for ( int iBinX = 1; iBinX <= jetEffHLT[3] -> GetNbinsX(); iBinX++ ) {
    for ( int iBinY = 1; iBinY <= jetEffHLT[3] -> GetNbinsY(); iBinY++ ) {
      jetEffHLT[3] -> SetBinContent(iBinX, iBinY, 1.);
      jetEffHLT[3] -> SetBinError(iBin, iBinY, 0.);
      jetEffHLTUp[3] -> SetBinContent(iBinX, iBinY, 1.);
      jetEffHLTUp[3] -> SetBinContent(iBinX, iBinY, 1.);
    }
  }
  // Run11B - PF
  jetEffHLT[4] = new TH2F("jetEffHLT4","jetEffHLT4",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  jetEffHLTUp[4] = new TH2F("jetEffHLT4Up","jetEffHLT4Up",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  jetEffHLTDown[4] = new TH2F("jetEffHLT4Down","jetEffHLT4Down",
  theBins, theMin, theMax,
  nEtaBins, etaBoundaries);
  for ( int iBinY = 1; iBinY <= jetEffHLT[4] -> GetNbinsY(); iBinY++ ) {
    for ( int iBinX = 1; iBinX <= jetEffHLT[4] -> GetNbinsX(); iBinX++ ) {
      float thisPt =  jetEffHLT[4] -> GetXaxis() -> GetBinCenter(iBinX);
      float thisCorrection  = erfit[iBinY-1] -> Eval(thisPt);
      float thisError = h_erfit[iBinY-1]->GetBinError(iBinX);
      jetEffHLT[4] -> SetBinContent(iBinX, iBinY, thisCorrection);
      jetEffHLT[4] -> SetBinError(iBin, iBinY, thisError);
      jetEffHLTUp[4] -> SetBinContent(iBinX, iBinY, thisCorrection + thisError);
      jetEffHLTDown[4] -> SetBinContent(iBinX, iBinY, thisCorrection - thisError);
    }
  }
  
  const int nCorrections = 5;
  TFile* outfile = new TFile ("../data/PFJETto/PFJETto_corrections.root","RECREATE");
  for ( int i = 0; i < nCorrections; i++ ) jetEffHLT[i] -> Write();
  for ( int i = 0; i < nCorrections; i++ ) jetEffHLTUp[i] -> Write();
  for ( int i = 0; i < nCorrections; i++ ) jetEffHLTDown[i] -> Write();
}
