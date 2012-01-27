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
  TH1F* metEffHlt[5];
  TH1F* metEffHltUp[5];
  TH1F* metEffHltDown[5];
  // Run11A - May10th
  metEffHlt[0] = (TH1F*) h_num_red[0] -> Clone("metEffHltZero");
  metEffHltUp[0] = (TH1F*) h_num_red[0] -> Clone("metEffHltZeroUp");
  metEffHltDown[0] = (TH1F*) h_num_red[0] -> Clone("metEffHltZeroDown");
  for ( int iBin = 1; iBin <= metEffHlt[0] -> GetXaxis() -> GetNbins(); iBin++ ) {
    metEffHlt[0] -> SetBinContent(iBin, 1.);
    metEffHlt[0] -> SetBinError(iBin, 0.);
    metEffHltUp[0] -> SetBinContent(iBin, 1.);
    metEffHltDown[0] -> SetBinContent(iBin, 1.);
  }
  // Run11A - v4-aug5-v6
  metEffHlt[1] = (TH1F*) h_num_red[1] -> Clone("metEffHltOne");
  metEffHltUp[1] = (TH1F*) h_num_red[1] -> Clone("metEffHltOneUp");
  metEffHltDown[1] = (TH1F*) h_num_red[1] -> Clone("metEffHltOneDown");
  for ( int iBin = 1; iBin <= metEffHlt[1] -> GetXaxis() -> GetNbins(); iBin++ ) {
    float thisPFmet =  metEffHlt[1] -> GetBinCenter(iBin);
    float thisCorrection  = erfit[1] -> Eval(thisPFmet)/erfit[4] -> Eval(thisPFmet);
    float thisError = thisCorrection * sqrt(
     h_erfit[1]->GetBinError(iBin)**2/erfit[1] -> Eval(thisPFmet)**2
    +h_erfit[4]->GetBinError(iBin)**2/erfit[4] -> Eval(thisPFmet)**2);
    metEffHlt[1] -> SetBinContent(iBin, thisCorrection);
    metEffHlt[1] -> SetBinError(iBin, thisError);
    metEffHltUp[1] -> SetBinContent(iBin, 1.);
    metEffHltDown[1] -> SetBinContent(iBin, 1.);
  }
  // Run11B - normal
  metEffHlt[2] = (TH1F*) h_num_red[2] -> Clone("metEffHltTwo");
  metEffHltUp[2] = (TH1F*) h_num_red[2] -> Clone("metEffHltTwoUp");
  metEffHltDown[2] = (TH1F*) h_num_red[2] -> Clone("metEffHltTwoDown");
  for ( int iBin = 1; iBin <= metEffHlt[2] -> GetXaxis() -> GetNbins(); iBin++ ) {
    float thisPFmet =  metEffHlt[2] -> GetBinCenter(iBin);
    float thisCorrection  = erfit[2] -> Eval(thisPFmet)/erfit[4] -> Eval(thisPFmet);
    float thisError = thisCorrection * sqrt(
     h_erfit[2]->GetBinError(iBin)**2/erfit[2] -> Eval(thisPFmet)**2
    +h_erfit[4]->GetBinError(iBin)**2/erfit[4] -> Eval(thisPFmet)**2);
    metEffHlt[2] -> SetBinContent(iBin, thisCorrection);
    metEffHlt[2] -> SetBinError(iBin, thisError);
    metEffHltUp[2] -> SetBinContent(iBin, thisCorrection + thisError);
    metEffHltDown[2] -> SetBinContent(iBin, thisCorrection - thisError);
  }
  // Run11B - backup
  metEffHlt[3] = (TH1F*) h_num_red[3] -> Clone("metEffHltThree");
  metEffHltUp[3] = (TH1F*) h_num_red[3] -> Clone("metEffHltThreeUp");
  metEffHltDown[3] = (TH1F*) h_num_red[3] -> Clone("metEffHltThreeDown");
  for ( int iBin = 1; iBin <= metEffHlt[3] -> GetXaxis() -> GetNbins(); iBin++ ) {
    float thisPFmet =  metEffHlt[3] -> GetBinCenter(iBin);
    float thisCorrection  = erfit[3] -> Eval(thisPFmet)/erfit[5] -> Eval(thisPFmet);
    float thisError = thisCorrection * sqrt(
     h_erfit[3]->GetBinError(iBin)**2/erfit[3] -> Eval(thisPFmet)**2
    +h_erfit[5]->GetBinError(iBin)**2/erfit[5] -> Eval(thisPFmet)**2);
    metEffHlt[3] -> SetBinContent(iBin, thisCorrection);
    metEffHlt[3] -> SetBinError(iBin, thisError);
    metEffHltUp[3] -> SetBinContent(iBin, thisCorrection + thisError);
    metEffHltDown[3] -> SetBinContent(iBin, thisCorrection - thisError);
  }
  // Run11B - PF
  metEffHlt[4] = (TH1F*) h_num_red[4] -> Clone("metEffHltFour");
  metEffHltUp[4] = (TH1F*) h_num_red[4] -> Clone("metEffHltFourUp");
  metEffHltDown[4] = (TH1F*) h_num_red[4] -> Clone("metEffHltFourDown");
  for ( int iBin = 1; iBin <= metEffHlt[4] -> GetXaxis() -> GetNbins(); iBin++ ) {
    float thisPFmet =  metEffHlt[4] -> GetBinCenter(iBin);
    float thisCorrection  = erfit[4] -> Eval(thisPFmet);
    float thisError = h_erfit[4]->GetBinError(iBin);
    metEffHlt[4] -> SetBinContent(iBin, thisCorrection);
    metEffHlt[4] -> SetBinError(iBin, thisError);
    metEffHltUp[4] -> SetBinContent(iBin, thisCorrection + thisError);
    metEffHltDown[4] -> SetBinContent(iBin, thisCorrection - thisError);
  }
  
  const int nCorrections = 5;
  TFile* outfile = new TFile ("../data/PFMETto/PFMETto_corrections.root","RECREATE");
  for ( int i = 0; i < nCorrections; i++ ) metEffHlt[i] -> Write();
  for ( int i = 0; i < nCorrections; i++ ) metEffHltUp[i] -> Write();
  for ( int i = 0; i < nCorrections; i++ ) metEffHltDown[i] -> Write();

}
