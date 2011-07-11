{
  using namespace std;
  
  const int nBand = 16;
  
  TString ntupleName = "~/PHD_MiBi/VBF/Ntuples/VBFAnalysis_AK5PF_H400_PT30_maxDeta_minDeta_Spring11_Mu/data_Run2011A-May10ReRecoPlusPromptReco/VBFAnalysis_AK5PF.root";
//   TString ntupleName = "~/PHD_MiBi/VBF/Ntuples/VBFAnalysis_AK5PF_H400_PT30_maxDeta_minDeta_Spring11_Mu/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola_Spring11-PU_S1_START311_V1G1-v1/VBFAnalysis_AK5PF.root";
  
  TFile* fileIn = new TFile(ntupleName,"READ");
  TTree* myTree = (TTree*) fileIn -> Get("ntu_14");
  TH1F* inputHistos[19]; 
  
  TString isData = "data_";
  
  vector<TString> bandMin;
  bandMin.push_back("0");
  bandMin.push_back("10");
  bandMin.push_back("20");
  bandMin.push_back("30");
  bandMin.push_back("40");
  bandMin.push_back("50");
  bandMin.push_back("60");
  bandMin.push_back("70");
  bandMin.push_back("80");
  bandMin.push_back("90");
  bandMin.push_back("100");
  bandMin.push_back("110");
  bandMin.push_back("120");
  bandMin.push_back("130");
  bandMin.push_back("140");
  bandMin.push_back("150");
  bandMin.push_back("160");
  
  vector<TString> bandMax;
  bandMax.push_back("20");
  bandMax.push_back("30");
  bandMax.push_back("40");
  bandMax.push_back("50");
  bandMax.push_back("60");
  bandMax.push_back("70");
  bandMax.push_back("80");
  bandMax.push_back("90");
  bandMax.push_back("100");
  bandMax.push_back("110");
  bandMax.push_back("120");
  bandMax.push_back("130");
  bandMax.push_back("140");
  bandMax.push_back("150");
  bandMax.push_back("160");
  bandMax.push_back("170");
  
  vector<float> bandMean;
  bandMean.push_back(10.);
  bandMean.push_back(20.);
  bandMean.push_back(30.);
  bandMean.push_back(40.);
  bandMean.push_back(50.);
  bandMean.push_back(60.);
  bandMean.push_back(70.);
  bandMean.push_back(80.);
  bandMean.push_back(90.);
  bandMean.push_back(100.);
  bandMean.push_back(110.);
  bandMean.push_back(120.);
  bandMean.push_back(130.);
  bandMean.push_back(140.);
  bandMean.push_back(150.);
  bandMean.push_back(160.);
  
  int nBin = 30.;
  float xMin = 0.;
  float xMax = 600.;
  
  TGraphErrors* theFitResults = new TGraphErrors();
  TGraphErrors* theFitChiSquare = new TGraphErrors();
  
  for ( int iBand = 0; iBand < nBand; iBand++ ) {
    
    TString theCut = "WJJ_m > " + bandMin[iBand] + " && WJJ_m < " + bandMax[iBand]; 
    
    TString histoName = "theBandHisto_" + bandMin[iBand] + "_"+ bandMax[iBand];
    inputHistos[iBand] = new TH1F(histoName, histoName, nBin, xMin, xMax); 
    TCanvas* c = new TCanvas();
    
    myTree -> Draw( "lepNuW_m_KF >> " + histoName, theCut  );
    
    TF1 myExp("myExp","[0]*exp([1]*-1.*x)",200,1000);
    myExp.SetParameter(0,1.);
    myExp.SetParameter(0,0.01);
    inputHistos[iBand] -> Fit("myExp","rq+");
    
    if ( myExp.GetNDF() == 0 || myExp.GetChisquare()/myExp.GetNDF() > 10. ) continue;
    
    std::cout << "the tau >>>> " << myExp.GetParameter(1) << std::endl;
    theFitResults -> SetPoint(iBand,bandMean[iBand],myExp.GetParameter(1));
    theFitResults -> SetPointError(iBand,0.,myExp.GetParError(1));
    if ( myExp.GetNDF() == 0 ) continue;
    theFitChiSquare -> SetPoint(iBand,bandMean[iBand],myExp.GetChisquare()/myExp.GetNDF());
    
    c -> SaveAs(isData+"theFit.gif+","gif");
  }
  
  TCanvas* c2 = new TCanvas();
  theFitResults -> GetXaxis() -> SetTitle("M_{JJ}");
  theFitResults -> GetYaxis() -> SetTitle("tau");
  theFitResults -> Draw("AP");
  c2 -> SaveAs(isData+"theFitResults.gif","gif");
  
  TCanvas* c3 = new TCanvas();
  theFitChiSquare -> GetXaxis() -> SetTitle("M_{JJ}");
  theFitChiSquare -> GetYaxis() -> SetTitle("FitRedChiSquare");
  theFitChiSquare -> Draw("AP");

  c3 -> SaveAs(isData+"theFitChiSquare.gif","gif");
  
}