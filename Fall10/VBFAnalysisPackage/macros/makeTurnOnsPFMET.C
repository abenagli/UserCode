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
  gStyle->SetStatY(0.5);
  gStyle->SetStatFontSize(15);
  gStyle->SetStatFont(4);
  
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
//    c1 -> SaveAs(fileIn.at(iFile)+".pdf","pdf");

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
    c1 -> SaveAs(fileIn.at(iFile)+".pdf","pdf");
    h_erfit[iFile]->SetMarkerStyle(0);
    h_erfit[iFile]->SetFillColor(kRed);
    h_erfit[iFile]->SetFillStyle(3003);
    h_erfit[iFile]->Draw("same,e3");
    c1 -> SaveAs(fileIn.at(iFile)+".pdf","pdf");

  }
  
  // Now make the histograms for the analysis corrections
  TH1F* metEffHLT[5];
  TH1F* metEffHLTUp[5];
  TH1F* metEffHLTDown[5];
  // Run11A - May10th
  metEffHLT[0] = (TH1F*) h_num_red[0] -> Clone("metEffHLT0");
  metEffHLTUp[0] = (TH1F*) h_num_red[0] -> Clone("metEffHLT0Up");
  metEffHLTDown[0] = (TH1F*) h_num_red[0] -> Clone("metEffHLT0Down");
  for ( int iBin = 1; iBin <= metEffHLT[0] -> GetXaxis() -> GetNbins(); iBin++ ) {
    metEffHLT[0] -> SetBinContent(iBin, 1.);
    metEffHLT[0] -> SetBinError(iBin, 0.);
    metEffHLTUp[0] -> SetBinContent(iBin, 1.);
    metEffHLTDown[0] -> SetBinContent(iBin, 1.);
  }
  // Run11A - v4-aug5-v6
  metEffHLT[1] = (TH1F*) h_num_red[1] -> Clone("metEffHLT1");
  metEffHLTUp[1] = (TH1F*) h_num_red[1] -> Clone("metEffHLT1Up");
  metEffHLTDown[1] = (TH1F*) h_num_red[1] -> Clone("metEffHLT1Down");
  for ( int iBin = 1; iBin <= metEffHLT[1] -> GetXaxis() -> GetNbins(); iBin++ ) {
    float thisPFmet =  metEffHLT[1] -> GetBinCenter(iBin);
    float thisCorrection  = erfit[0] -> Eval(thisPFmet)/erfit[4] -> Eval(thisPFmet);
    float thisError = thisCorrection * sqrt(
     h_erfit[0]->GetBinError(iBin)**2/erfit[0] -> Eval(thisPFmet)**2
    +h_erfit[4]->GetBinError(iBin)**2/erfit[4] -> Eval(thisPFmet)**2);
    metEffHLT[1] -> SetBinContent(iBin, thisCorrection);
    metEffHLT[1] -> SetBinError(iBin, thisError);
    metEffHLTUp[1] -> SetBinContent(iBin, thisCorrection + thisError);
    metEffHLTDown[1] -> SetBinContent(iBin, thisCorrection - thisError);
  }
  // Run11B - normal
  metEffHLT[2] = (TH1F*) h_num_red[2] -> Clone("metEffHLT2");
  metEffHLTUp[2] = (TH1F*) h_num_red[2] -> Clone("metEffHLT2Up");
  metEffHLTDown[2] = (TH1F*) h_num_red[2] -> Clone("metEffHLT2Down");
  for ( int iBin = 1; iBin <= metEffHLT[2] -> GetXaxis() -> GetNbins(); iBin++ ) {
    float thisPFmet =  metEffHLT[2] -> GetBinCenter(iBin);
    float thisCorrection  = erfit[1] -> Eval(thisPFmet)/erfit[4] -> Eval(thisPFmet);
    float thisError = thisCorrection * sqrt(
     h_erfit[1]->GetBinError(iBin)**2/erfit[1] -> Eval(thisPFmet)**2
    +h_erfit[4]->GetBinError(iBin)**2/erfit[4] -> Eval(thisPFmet)**2);
    metEffHLT[2] -> SetBinContent(iBin, thisCorrection);
    metEffHLT[2] -> SetBinError(iBin, thisError);
    metEffHLTUp[2] -> SetBinContent(iBin, thisCorrection + thisError);
    metEffHLTDown[2] -> SetBinContent(iBin, thisCorrection - thisError);
  }
  // Run11B - backup
  metEffHLT[3] = (TH1F*) h_num_red[3] -> Clone("metEffHLT3");
  metEffHLTUp[3] = (TH1F*) h_num_red[3] -> Clone("metEffHLT3Up");
  metEffHLTDown[3] = (TH1F*) h_num_red[3] -> Clone("metEffHLT3Down");
  for ( int iBin = 1; iBin <= metEffHLT[3] -> GetXaxis() -> GetNbins(); iBin++ ) {
    float thisPFmet =  metEffHLT[3] -> GetBinCenter(iBin);
    float thisCorrection  = erfit[2] -> Eval(thisPFmet)/erfit[5] -> Eval(thisPFmet);
    float thisError = thisCorrection * sqrt(
     h_erfit[2]->GetBinError(iBin)**2/erfit[2] -> Eval(thisPFmet)**2
    +h_erfit[5]->GetBinError(iBin)**2/erfit[5] -> Eval(thisPFmet)**2);
    metEffHLT[3] -> SetBinContent(iBin, thisCorrection);
    metEffHLT[3] -> SetBinError(iBin, thisError);
    metEffHLTUp[3] -> SetBinContent(iBin, thisCorrection + thisError);
    metEffHLTDown[3] -> SetBinContent(iBin, thisCorrection - thisError);
  }
  // Run11B - PF
  metEffHLT[4] = (TH1F*) h_num_red[4] -> Clone("metEffHLT4");
  metEffHLTUp[4] = (TH1F*) h_num_red[4] -> Clone("metEffHLT4Up");
  metEffHLTDown[4] = (TH1F*) h_num_red[4] -> Clone("metEffHLT4Down");
  for ( int iBin = 1; iBin <= metEffHLT[4] -> GetXaxis() -> GetNbins(); iBin++ ) {
    float thisPFmet =  metEffHLT[4] -> GetBinCenter(iBin);
    float thisCorrection  = erfit[3] -> Eval(thisPFmet);
    float thisError = h_erfit[3]->GetBinError(iBin);
    metEffHLT[4] -> SetBinContent(iBin, thisCorrection);
    metEffHLT[4] -> SetBinError(iBin, thisError);
    metEffHLTUp[4] -> SetBinContent(iBin, thisCorrection + thisError);
    metEffHLTDown[4] -> SetBinContent(iBin, thisCorrection - thisError);
  }
  
  const int nCorrections = 5;
  TFile* outfile = new TFile ("../data/PFMETto/PFMETto_corrections.root","RECREATE");
  for ( int i = 0; i < nCorrections; i++ ) metEffHLT[i] -> Write();
  for ( int i = 0; i < nCorrections; i++ ) metEffHLTUp[i] -> Write();
  for ( int i = 0; i < nCorrections; i++ ) metEffHLTDown[i] -> Write();

}
