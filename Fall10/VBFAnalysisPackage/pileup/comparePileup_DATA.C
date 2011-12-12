#include <iomanip>



void comparePileup_DATA()
{
  // Run2011B - mine
  double* distrPU1_DATA = new double[35];
  distrPU1_DATA[0]  = 0.000191236;
  distrPU1_DATA[1]  = 0.001277422;
  distrPU1_DATA[2]  = 0.004599973;
  distrPU1_DATA[3]  = 0.011593057;
  distrPU1_DATA[4]  = 0.022896772;
  distrPU1_DATA[5]  = 0.037801772;
  distrPU1_DATA[6]  = 0.054392517;
  distrPU1_DATA[7]  = 0.070212059;
  distrPU1_DATA[8]  = 0.083010413;
  distrPU1_DATA[9]  = 0.091250703;
  distrPU1_DATA[10] = 0.094289608;
  distrPU1_DATA[11] = 0.092308313;
  distrPU1_DATA[12] = 0.086107403;
  distrPU1_DATA[13] = 0.076853909;
  distrPU1_DATA[14] = 0.065835930;
  distrPU1_DATA[15] = 0.054259304;
  distrPU1_DATA[16] = 0.043106884;
  distrPU1_DATA[17] = 0.033067599;
  distrPU1_DATA[18] = 0.024529368;
  distrPU1_DATA[19] = 0.017619424;
  distrPU1_DATA[20] = 0.012271007;
  distrPU1_DATA[21] = 0.008296419;
  distrPU1_DATA[22] = 0.005451862;
  distrPU1_DATA[23] = 0.003486181;
  distrPU1_DATA[24] = 0.002171698;
  distrPU1_DATA[25] = 0.001319390;
  distrPU1_DATA[26] = 0.000782592;
  distrPU1_DATA[27] = 0.000453665;
  distrPU1_DATA[28] = 0.000257278;
  distrPU1_DATA[29] = 0.000142872;
  distrPU1_DATA[30] = 0.000077761;
  distrPU1_DATA[31] = 0.000041515;
  distrPU1_DATA[32] = 0.000021758;
  distrPU1_DATA[33] = 0.000011202;
  distrPU1_DATA[34] = 0.000011131;
  
  // Run2011B - matt
  double* distrPU2_DATA = new double[35];
  distrPU2_DATA[0]  = 0.000112737003406;
  distrPU2_DATA[1]  = 0.000777646125266;
  distrPU2_DATA[2]  = 0.00293953727088;
  distrPU2_DATA[3]  = 0.00779607746446;
  distrPU2_DATA[4]  = 0.0161886140039;
  distrPU2_DATA[5]  = 0.0280489939304;
  distrPU2_DATA[6]  = 0.0422732741973;
  distrPU2_DATA[7]  = 0.0570578977823;
  distrPU2_DATA[8]  = 0.0704498952494;
  distrPU2_DATA[9]  = 0.0808285880957;
  distrPU2_DATA[10] = 0.087175540055;
  distrPU2_DATA[11] = 0.0891353619476;
  distrPU2_DATA[12] = 0.0869369566986;
  distrPU2_DATA[13] = 0.0812446797496;
  distrPU2_DATA[14] = 0.07298581133;
  distrPU2_DATA[15] = 0.0631825589687;
  distrPU2_DATA[16] = 0.0528079996742;
  distrPU2_DATA[17] = 0.0426800503642;
  distrPU2_DATA[18] = 0.0334009356879;
  distrPU2_DATA[19] = 0.0253413520202;
  distrPU2_DATA[20] = 0.0186609176278;
  distrPU2_DATA[21] = 0.0133518691661;
  distrPU2_DATA[22] = 0.00929217896527;
  distrPU2_DATA[23] = 0.00629666691546;
  distrPU2_DATA[24] = 0.00415877084982;
  distrPU2_DATA[25] = 0.00267988512703;
  distrPU2_DATA[26] = 0.00168652054605;
  distrPU2_DATA[27] = 0.00103754971688;
  distrPU2_DATA[28] = 0.000624559819431;
  distrPU2_DATA[29] = 0.000368196397673;
  distrPU2_DATA[30] = 0.000212766035973;
  distrPU2_DATA[31] = 0.000120615027302;
  distrPU2_DATA[32] = 6.71298899774e-05;
  distrPU2_DATA[33] = 3.67081722824e-05;
  distrPU2_DATA[34] = 1.97348922108e-05;
  
  TH1F* h_PU1 = new TH1F("h_PU1","",35,-0.5,34.5);
  TH1F* h_PU2 = new TH1F("h_PU2","",35,-0.5,34.5);
  TH1F* h_PU3 = new TH1F("h_PU3","",35,-0.5,34.5);
  TH1F* h_PU4 = new TH1F("h_PU4","",35,-0.5,34.5);
  
  for(int bin = 1; bin <= h_PU1->GetNbinsX(); ++bin)
  {
    h_PU1 -> SetBinContent(bin,distrPU1_DATA[bin-1]);
    h_PU2 -> SetBinContent(bin,distrPU2_DATA[bin-1]);
  }
  
  
  
  TCanvas* c = new TCanvas();
  c -> cd();
  c -> SetGridx();
  c -> SetGridy();
  
  h_PU1 -> GetXaxis() -> SetTitle("n_{PU}");
  h_PU1 -> GetXaxis() -> SetLabelSize(0.04);
  h_PU1 -> GetYaxis() -> SetLabelSize(0.04);
  h_PU1 -> GetYaxis() -> SetTitle("event fraction");
  h_PU1 -> Scale(1./h_PU1->Integral());
  h_PU1 -> SetMarkerStyle(20);
  h_PU1 -> SetLineColor(kRed);
  h_PU1 -> Draw("");
  c -> Update();
  
  h_PU2 -> GetXaxis() -> SetTitle("n_{PU}");
  h_PU2 -> GetXaxis() -> SetLabelSize(0.04);
  h_PU2 -> GetYaxis() -> SetLabelSize(0.04);
  h_PU2 -> GetYaxis() -> SetTitle("event fraction");
  h_PU2 -> Scale(1./h_PU2->Integral());
  h_PU2 -> SetMarkerStyle(20);
  h_PU2 -> SetLineColor(kBlue);
  h_PU2 -> Draw("same");
  c -> Update();
  
  TLegend* legend = new TLegend(0.70,0.80,0.90,0.90);
  legend -> SetFillColor(kWhite);
  legend -> SetFillStyle(4000);
  legend -> SetTextFont(42);
  legend -> SetTextSize(0.03);
  legend -> AddEntry(h_PU1,"Run2011B - mine","L");
  legend -> AddEntry(h_PU2,"Run2011B - matt","L");
  legend -> Draw("same");
}
