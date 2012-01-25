{
  
  int nBins_red = 100;
  int min_red = 0;
  int max_red = 100;

  const char *TurnOnTitle = "PFMHT15 Efficiency;PFMet (GeV);#epsilon";
  TString inFolder = "../data/PFMETto/";
  std::vector<TString> fileIn;
  fileIn.push_back("Run11A_ntu_15_MetTurnOn");
  fileIn.push_back("Run11B_Normal_ntu_15_MetTurnOn");
  fileIn.push_back("Run11B_Backup_ntu_15_MetTurnOn");
  fileIn.push_back("Run11B_PF_ntu_15_MetTurnOn");
  fileIn.push_back("WJets_PFMHT20_ntu_15_MetTurnOn");
  fileIn.push_back("WJets_PFMHT25_ntu_15_MetTurnOn");
  
  
  // ----------------- Make the turn on fit with TEfficiency
  
  TF1 *erfit[6];
  TH1F *h_num_red[6];
  TH1F *h_den_red[6];
  
  gStyle->SetStatX(0.9);
  gStyle->SetStatY(0.6);
  
  for ( unsigned int iFile = 0; iFile < fileIn.size(); iFile++ ) {
  
    TCanvas* c1 = new TCanvas();
    TFile* thisFile = new TFile(inFolder+fileIn.at(iFile)+".root","READ");
    TH1F* num = (TH1F*) thisFile->Get("h_num");
    TH1F* den = (TH1F*) thisFile->Get("h_den");
    
    // reduce the histos
    TString s_iFile = "";
    s_iFile += iFile;
    TH1F* h_num_red[iFile] = new TH1F ("h_num_red"+s_iFile,"h_den_red"+s_iFile,nBins_red,min_red,max_red);
    TH1F* h_den_red[iFile] = new TH1F ("h_den_red"+s_iFile,"h_den_red"+s_iFile,nBins_red,min_red,max_red);
    int iBin_red = 1;
    for ( int iBin = 1; iBin <= num->GetXaxis()->GetNbins(); iBin++ ) {
    
      if ( num->GetBinCenter(iBin) != h_num_red[iFile]->GetBinCenter(iBin_red) ) continue;
      h_num_red[iFile]->SetBinContent(iBin_red, num->GetBinContent(iBin) );
      h_den_red[iFile]->SetBinContent(iBin_red, den->GetBinContent(iBin) );
      iBin_red++;
    
    }
    
    h_num_red[iFile]->Rebin(2);
    h_den_red[iFile]->Rebin(2);
    
    TEfficiency *TurnOn = new TEfficiency(*h_num_red[iFile],*h_den_red[iFile]);
    erfit[iFile] = new TF1("erfit","([0] + ([1]-[0])/2 * (1 +TMath::Erf((x-[2])/([3]*sqrt(2)))))",10,100);
    erfit[iFile]->SetParNames("A","B","Mu","Sigma");
    erfit[iFile]->SetParameters(0,1,10,10);
    erfit[iFile]->SetParLimits(1, -1, 1);
    TurnOn->Fit(erfit[iFile]);
    c1 -> cd();
    c1 -> SetGridx();
    c1 -> SetGridy();
    TurnOn -> SetMarkerStyle(20);
    TurnOn -> SetTitle(TurnOnTitle);
    TurnOn -> SetName("TurnOn");
    TurnOn -> Draw("AP");
//    c1 -> SaveAs(fileIn.at(iFile)+".gif","gif");

  }

  // ----------------- Make the turn on fit with TAsymm errors to get the errors
  
  TF1 *erfit_support[6];
  TGraphAsymmErrors* g_TurnOn[6];
  TH1F *h_erfit[6];
    
  for ( unsigned int iFile = 0; iFile < fileIn.size(); iFile++ ) {
  
    TCanvas* c1 = new TCanvas();
    
    g_TurnOn[iFile] = new TGraphAsymmErrors();
    g_TurnOn[iFile] -> BayesDivide(h_num_red[iFile],h_den_red[iFile]);
    //, "cl=0.683 b(0.5,0.5) mode");
    
    h_erfit[iFile] = (TH1F*) h_num_red[iFile] -> Clone("ciao");

    erfit_support[iFile] = new TF1("erfit_support"+s_iFile,"([0] + ([1]-[0])/2 * (1 +TMath::Erf((x-[2])/([3]*sqrt(2)))))",10,100);
    erfit_support[iFile]->SetParNames("A","B","Mu","Sigma");
    erfit_support[iFile]->SetParameters(erfit[iFile]->GetParameter(0),erfit[iFile]->GetParameter(1),erfit[iFile]->GetParameter(2),erfit[iFile]->GetParameter(3));
    erfit_support[iFile]->SetParLimits(1, -1, 1);
    g_TurnOn[iFile] -> Fit(erfit_support[iFile],"RSE+");
    (TVirtualFitter::GetFitter()) -> GetConfidenceIntervals(h_erfit[iFile],0.68);
    c1 -> cd();
    c1 -> SetGridx();
    c1 -> SetGridy();
    g_TurnOn[iFile] -> SetMarkerStyle(20);
    g_TurnOn[iFile] -> SetTitle(TurnOnTitle);
    g_TurnOn[iFile] -> SetName("TurnOn");
    g_TurnOn[iFile] -> Draw("AP");
    c1 -> SaveAs(fileIn.at(iFile)+".gif","gif");
    h_erfit[iFile]->SetMarkerStyle(0);
    h_erfit[iFile]->SetFillColor(kRed);
    h_erfit[iFile]->SetFillStyle(3003);
    h_erfit[iFile]->Draw("same,e3");
    c1 -> SaveAs(fileIn.at(iFile)+".gif","gif");

  }
  
  // Now make the histograms for the analysis corrections
  TH1F* h_correction_PFMET[5];
  // Run11A - May10th
  h_correction_PFMET[0] = (TH1F*) h_num_red[0] -> Clone("h_correction_PFMET_0");
  for ( int iBin = 1; iBin <= h_correction_PFMET[0] -> GetXaxis() -> GetNbins(); iBin++ ) {
    h_correction_PFMET[0] -> SetBinContent(iBin, 1.);
    h_correction_PFMET[0] -> SetBinError(iBin, 0.);
  }
  // Run11A - v4-aug5-v6
  h_correction_PFMET[1] = (TH1F*) h_num_red[1] -> Clone("h_correction_PFMET_1");
  for ( int iBin = 1; iBin <= h_correction_PFMET[1] -> GetXaxis() -> GetNbins(); iBin++ ) {
    float thisPFmet =  h_correction_PFMET[1] -> GetBinCenter(iBin);
    float thisCorrection  = erfit[1] -> Eval(thisPFmet)/erfit[4] -> Eval(thisPFmet);
    float thisError = thisCorrection * sqrt(
     h_erfit[1]->GetBinError(iBin)**2/erfit[1] -> Eval(thisPFmet)**2
    +h_erfit[4]->GetBinError(iBin)**2/erfit[4] -> Eval(thisPFmet)**2);
    h_correction_PFMET[1] -> SetBinContent(iBin, thisCorrection);
    h_correction_PFMET[1] -> SetBinError(iBin, thisError);
  }
  // Run11B - normal
  h_correction_PFMET[2] = (TH1F*) h_num_red[2] -> Clone("h_correction_PFMET_2");
  for ( int iBin = 1; iBin <= h_correction_PFMET[2] -> GetXaxis() -> GetNbins(); iBin++ ) {
    float thisPFmet =  h_correction_PFMET[2] -> GetBinCenter(iBin);
    float thisCorrection  = erfit[2] -> Eval(thisPFmet)/erfit[4] -> Eval(thisPFmet);
    float thisError = thisCorrection * sqrt(
     h_erfit[2]->GetBinError(iBin)**2/erfit[2] -> Eval(thisPFmet)**2
    +h_erfit[4]->GetBinError(iBin)**2/erfit[4] -> Eval(thisPFmet)**2);
    h_correction_PFMET[2] -> SetBinContent(iBin, thisCorrection);
    h_correction_PFMET[2] -> SetBinError(iBin, thisError);
  }
  // Run11B - backup
  h_correction_PFMET[3] = (TH1F*) h_num_red[3] -> Clone("h_correction_PFMET_3");
  for ( int iBin = 1; iBin <= h_correction_PFMET[3] -> GetXaxis() -> GetNbins(); iBin++ ) {
    float thisPFmet =  h_correction_PFMET[3] -> GetBinCenter(iBin);
    float thisCorrection  = erfit[3] -> Eval(thisPFmet)/erfit[5] -> Eval(thisPFmet);
    float thisError = thisCorrection * sqrt(
     h_erfit[3]->GetBinError(iBin)**2/erfit[3] -> Eval(thisPFmet)**2
    +h_erfit[5]->GetBinError(iBin)**2/erfit[5] -> Eval(thisPFmet)**2);
    h_correction_PFMET[3] -> SetBinContent(iBin, thisCorrection);
    h_correction_PFMET[3] -> SetBinError(iBin, thisError);
  }
  // Run11B - PF
  h_correction_PFMET[4] = (TH1F*) h_num_red[4] -> Clone("h_correction_PFMET_4");
  for ( int iBin = 1; iBin <= h_correction_PFMET[4] -> GetXaxis() -> GetNbins(); iBin++ ) {
    float thisPFmet =  h_correction_PFMET[4] -> GetBinCenter(iBin);
    float thisCorrection  = erfit[4] -> Eval(thisPFmet);
    float thisError = h_erfit[4]->GetBinError(iBin);
    h_correction_PFMET[4] -> SetBinContent(iBin, thisCorrection);
    h_correction_PFMET[4] -> SetBinError(iBin, thisError);
  }
  
  const int nCorrections = 5;
  TFile* outfile = new TFile ("../data/PFMETto/PFMETto_corrections.root","RECREATE");
  for ( int i = 0; i < nCorrections; i++ ) h_correction_PFMET[i] -> Write();

}
