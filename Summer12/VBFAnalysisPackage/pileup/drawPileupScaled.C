#include "../interface/histoFunc.h"
#include <iomanip>



void drawPileupScaled(const std::string& fileName)
{
  gStyle -> SetOptStat(1100);
  
  
  TFile* f = TFile::Open(((fileName)+".root").c_str(),"READ");
  TH1F* h = (TH1F*)( f->Get("pileup") );
  h -> Scale(1./h->Integral());
  
  double distrPU_DATA[35];
  for(int bin = 1; bin <= 35; ++bin)
    distrPU_DATA[bin-1] = h->GetBinContent(bin); 
  
  
  
  // define histograms
  int nBins = 35;
  float xWidth = 1.;
  float xMin = 0. - 0.5*xWidth;
  float xMax = xMin + nBins * xWidth;
  
  TH1F* h_PU     = new TH1F("h_PU",    "",nBins,xMin,xMax);
  TH1F* h_PUUp   = new TH1F("h_PUUp",  "",nBins,xMin,xMax);
  TH1F* h_PUDown = new TH1F("h_PUDown","",nBins,xMin,xMax);
  
  
  
  // fill histograms
  for(int i = 0; i < nBins; ++i)
  {
    h_PU -> Fill(i,distrPU_DATA[i]);
  }
  
  
  histoFunc* templateHistoFunc = new histoFunc(h_PU);
  float n = 1.;
  
  
  TF1* f_up = new TF1("f",templateHistoFunc,xMin,xMax,3,"histoFunc");
  f_up -> FixParameter(0,1.);
  f_up -> SetParameter(1,1./(1.+n/h_PU->GetMean()));
  f_up -> FixParameter(2,0.);
  
  for(float x = xMin + 0.5*xWidth; x < xMax; x+=xWidth)
    h_PUUp -> Fill(x,f_up->Eval(x));
  
  
  TF1* f_down = new TF1("f",templateHistoFunc,xMin,xMax,3,"histoFunc");
  f_down -> FixParameter(0,1.);
  f_down -> SetParameter(1,1./(1.-n/h_PU->GetMean()));
  f_down -> FixParameter(2,0.);
  
  for(float x = xMin + 0.5*xWidth; x < xMax; x+=xWidth)
    h_PUDown -> Fill(x,f_down->Eval(x));
  
  
  
  // set maximum
  float maximum = -999999999.;
  if( h_PU->GetMaximum() > maximum ) maximum = h_PU -> GetMaximum();
  if( h_PUUp->GetMaximum() > maximum ) maximum = h_PUUp -> GetMaximum();
  if( h_PUDown->GetMaximum() > maximum ) maximum = h_PUDown -> GetMaximum();
  h_PU -> SetMaximum(1.1*maximum);
  
  h_PU -> GetXaxis() -> SetTitle("n_{PU}");
  h_PU -> GetYaxis() -> SetTitle("event fraction");
  
  
  
  // draw plots
  TCanvas* c1 = new TCanvas("c1","PU",600,600);
  c1 -> SetGridx();
  c1 -> SetGridy();
  
  h_PU -> SetLineWidth(3);  
  h_PU -> Draw();
    
  h_PUUp -> SetLineColor(kRed);
  h_PUUp -> SetLineWidth(2);
  h_PUUp -> Draw("sames");
  
  h_PUDown -> SetLineColor(kBlue);
  h_PUDown -> SetLineWidth(2);
  h_PUDown -> Draw("sames");
  
  gPad -> Update();
  

  TPaveStats* s_PU = (TPaveStats*)( h_PU->GetListOfFunctions()->FindObject("stats") );
  s_PU ->SetTextColor(kBlack);
  s_PU ->SetTextSize(0.03);
  s_PU->SetX1NDC(0.80);
  s_PU->SetX2NDC(0.99);
  s_PU->SetY1NDC(0.89);
  s_PU->SetY2NDC(0.99);
  s_PU -> SetOptStat(1100);
  s_PU -> Draw("sames");
  gPad -> Update();
  
  TPaveStats* s_PUUp = (TPaveStats*)( h_PUUp->GetListOfFunctions()->FindObject("stats") );
  s_PUUp ->SetTextColor(kRed);
  s_PUUp ->SetTextSize(0.03);
  s_PUUp->SetX1NDC(0.80);
  s_PUUp->SetX2NDC(0.99);
  s_PUUp->SetY1NDC(0.79);
  s_PUUp->SetY2NDC(0.89);
  s_PUUp -> SetOptStat(1100);
  s_PUUp -> Draw("sames");
  gPad -> Update();
  
  TPaveStats* s_PUDown = (TPaveStats*)( h_PUDown->GetListOfFunctions()->FindObject("stats") );
  s_PUDown ->SetTextColor(kBlue);
  s_PUDown ->SetTextSize(0.03);
  s_PUDown->SetX1NDC(0.80);
  s_PUDown->SetX2NDC(0.99);
  s_PUDown->SetY1NDC(0.69);
  s_PUDown->SetY2NDC(0.79);
  s_PUDown -> SetOptStat(1100);
  s_PUDown -> Draw("sames");
  gPad -> Update();
  
  
  
  
  
  
  std::cout << "\n--- PU ---" << std::endl;
  for(int bin = 1; bin <= h_PU->GetNbinsX(); ++bin)
  {
    if( int(h_PU->GetBinCenter(bin)) < 10 )
      std::cout << "distrPU_DATA[" << int(h_PU->GetBinCenter(bin)) <<"]  = ";
    else
      std::cout << "distrPU_DATA[" << int(h_PU->GetBinCenter(bin)) <<"] = ";      
    std::cout << std::fixed << std::setprecision(9) << std::setw(11) << h_PU -> GetBinContent(bin);
    std::cout << ";" << std::endl;
  }
  
  std::cout << "\n--- PUUp ---" << std::endl;
  for(int bin = 1; bin <= h_PUUp->GetNbinsX(); ++bin)
  {
    if( int(h_PUUp->GetBinCenter(bin)) < 10 )
      std::cout << "distrPUUp_DATA[" << int(h_PUUp->GetBinCenter(bin)) <<"]  = ";
    else
      std::cout << "distrPUUp_DATA[" << int(h_PUUp->GetBinCenter(bin)) <<"] = ";      
    std::cout << std::fixed << std::setprecision(9) << std::setw(11) << h_PUUp -> GetBinContent(bin);
    std::cout << ";" << std::endl;
  }
  
  std::cout << "\n--- PUDown ---" << std::endl;
  for(int bin = 1; bin <= h_PUDown->GetNbinsX(); ++bin)
  {
    if( int(h_PUDown->GetBinCenter(bin)) < 10 )
      std::cout << "distrPUDown_DATA[" << int(h_PUDown->GetBinCenter(bin)) <<"]  = ";
    else
      std::cout << "distrPUDown_DATA[" << int(h_PUDown->GetBinCenter(bin)) <<"] = ";      
    std::cout << std::fixed << std::setprecision(9) << std::setw(11) << h_PUDown -> GetBinContent(bin);
    std::cout << ";" << std::endl;
  }
  
}
