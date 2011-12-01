#include "../interface/histoFunc.h"
#include <iomanip>



void drawPileupScaled()
{
  gStyle -> SetOptStat(1100);
  
  
  double distrPU_DATA[] = {
    0.002859418, // 0
    0.012560310, // 1
    0.029963130, // 2
    0.051312994, // 3
    0.070971273, // 4
    0.084786385, // 5
    0.091462672, // 6
    0.091925524, // 7
    0.087999381, // 8
    0.081412680, // 9
    0.073399492, // 10
    0.064719088, // 11
    0.055832654, // 12
    0.047066260, // 13
    0.038698819, // 14
    0.030981110, // 15
    0.024117511, // 16
    0.018240983, // 17
    0.013399664, // 18
    0.009560713, // 19
    0.006628135, // 20
    0.004467347, // 21
    0.002929458, // 22
    0.001870567, // 23
    0.001164142, // 24
    0.000706805, // 25
    0.000419059, // 26
    0.000242856, // 27
    0.000137700, // 28
    0.000076458, // 29
    0.000041610, // 30
    0.000022214, // 31
    0.000011642, // 32
    0.000005994, // 33
    0.000005955, // 34
    0.000000000  // 35
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
