#include "../interface/histoFunc.h"
#include <iomanip>



void drawPileupScaled()
{
  gStyle -> SetOptStat(1100);
  
  
  double distrPU_DATA[] = {
    0.005929510, // 0
    0.025542751, // 1
    0.059146766, // 2
    0.097015992, // 3
    0.126287282, // 4
    0.138848349, // 5
    0.134116739, // 6
    0.116909698, // 7
    0.093739837, // 8
    0.070092745, // 9
    0.049362671, // 10
    0.032974087, // 11
    0.020997589, // 12
    0.012791663, // 13
    0.007474022, // 14
    0.004196492, // 15
    0.002267743, // 16
    0.001181017, // 17
    0.000593481, // 18
    0.000288109, // 19
    0.000135272, // 20
    0.000061498, // 21
    0.000027102, // 22
    0.000011591, // 23
    0.000004816, // 24
    0.000001946, // 25
    0.000000766, // 26
    0.000000294, // 27
    0.000000110, // 28
    0.000000040, // 29
    0.000000014, // 30
    0.000000005, // 31
    0.000000002, // 32
    0.000000001, // 33
    0.000000000  // 34
  };
  
  
  // define histograms
  int nBins = 35;
  float xWidth = 1.;
  float xMin = 0. - 0.5*xWidth;
  float xMax = xMin + nBins * xWidth;
  
  TH1F* h_PU     = new TH1F("h_PU",    "",nBins,xMin,xMax);
  TH1F* h_PUUp   = new TH1F("h_PUUp",  "",nBins,xMin,xMax);
  TH1F* h_PUDown = new TH1F("h_PUDodn","",nBins,xMin,xMax);
  
  
  
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
  
  
  
  
  
  
  std::cout << "\n--- PUUp ---" << std::endl;
  for(int bin = 1; bin <= h_PUUp->GetNbinsX(); ++bin)
  {
    std::cout << std::fixed << std::setprecision(9) << std::setw(11) << h_PUUp -> GetBinContent(bin);
    
    if( bin < h_PUUp->GetNbinsX() )
      std::cout << "," << " // " << std::setprecision(0) << int(h_PUUp->GetBinCenter(bin)) << std::endl;
    else
      std::cout << " " << " // " << std::setprecision(0) << int(h_PUUp->GetBinCenter(bin)) << std::endl;
  }
  
  std::cout << "\n--- PUDown ---" << std::endl;
  for(int bin = 1; bin <= h_PUDown->GetNbinsX(); ++bin)
  {
    std::cout << std::fixed << std::setprecision(9) << std::setw(11) << h_PUDown -> GetBinContent(bin);
    
    if( bin < h_PUDown->GetNbinsX() )
      std::cout << "," << " // " << std::setprecision(0) << int(h_PUDown->GetBinCenter(bin)) << std::endl;
    else
      std::cout << " " << " // " << std::setprecision(0) << int(h_PUDown->GetBinCenter(bin)) << std::endl;
  }
  
}
