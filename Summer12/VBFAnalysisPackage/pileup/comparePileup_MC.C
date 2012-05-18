#include <iomanip>



void comparePileup()
{
  // twiki - true
  double* distrPU1_MC = new double[35];
  distrPU1_MC[0]  = 0.003388501;
  distrPU1_MC[1]  = 0.010357558;
  distrPU1_MC[2]  = 0.024724258;
  distrPU1_MC[3]  = 0.042348605;
  distrPU1_MC[4]  = 0.058279812;
  distrPU1_MC[5]  = 0.068851751;
  distrPU1_MC[6]  = 0.072914824;
  distrPU1_MC[7]  = 0.071579609;
  distrPU1_MC[8]  = 0.066811668;
  distrPU1_MC[9]  = 0.060672356;
  distrPU1_MC[10] = 0.054528356;
  distrPU1_MC[11] = 0.049193540;
  distrPU1_MC[12] = 0.044886042;
  distrPU1_MC[13] = 0.041341896;
  distrPU1_MC[14] = 0.038467900;
  distrPU1_MC[15] = 0.035871463;
  distrPU1_MC[16] = 0.033419520;
  distrPU1_MC[17] = 0.030915649;
  distrPU1_MC[18] = 0.028395374;
  distrPU1_MC[19] = 0.025798107;
  distrPU1_MC[20] = 0.023237445;
  distrPU1_MC[21] = 0.020602754;
  distrPU1_MC[22] = 0.018068800;
  distrPU1_MC[23] = 0.015559693;
  distrPU1_MC[24] = 0.013211063;
  distrPU1_MC[25] = 0.010964293;
  distrPU1_MC[26] = 0.008920993;
  distrPU1_MC[27] = 0.007080504;
  distrPU1_MC[28] = 0.005499239;
  distrPU1_MC[29] = 0.004187022;
  distrPU1_MC[30] = 0.003096474;
  distrPU1_MC[31] = 0.002237361;
  distrPU1_MC[32] = 0.001566428;
  distrPU1_MC[33] = 0.001074149;
  distrPU1_MC[34] = 0.000721755;
  
  // twiki - in-time
  double* distrPU2_MC = new double[35];
  distrPU2_MC[0]  = 0.014583699;
  distrPU2_MC[1]  = 0.025682975;
  distrPU2_MC[2]  = 0.038460562;
  distrPU2_MC[3]  = 0.049414536;
  distrPU2_MC[4]  = 0.056931087;
  distrPU2_MC[5]  = 0.061182816;
  distrPU2_MC[6]  = 0.062534625;
  distrPU2_MC[7]  = 0.061476918;
  distrPU2_MC[8]  = 0.058677499;
  distrPU2_MC[9]  = 0.055449877;
  distrPU2_MC[10] = 0.051549051;
  distrPU2_MC[11] = 0.047621024;
  distrPU2_MC[12] = 0.043923799;
  distrPU2_MC[13] = 0.040569076;
  distrPU2_MC[14] = 0.037414654;
  distrPU2_MC[15] = 0.034227033;
  distrPU2_MC[16] = 0.031437714;
  distrPU2_MC[17] = 0.028825596;
  distrPU2_MC[18] = 0.026218978;
  distrPU2_MC[19] = 0.023727061;
  distrPU2_MC[20] = 0.021365645;
  distrPU2_MC[21] = 0.01918743;
  distrPU2_MC[22] = 0.016972815;
  distrPU2_MC[23] = 0.014920601;
  distrPU2_MC[24] = 0.013038989;
  distrPU2_MC[25] = 0.011293777;
  distrPU2_MC[26] = 0.009612465;
  distrPU2_MC[27] = 0.008193556;
  distrPU2_MC[28] = 0.006888047;
  distrPU2_MC[29] = 0.005715239;
  distrPU2_MC[30] = 0.004711232;
  distrPU2_MC[31] = 0.003869926;
  distrPU2_MC[32] = 0.003154521;
  distrPU2_MC[33] = 0.002547417;
  distrPU2_MC[34] = 0.002024714;
  
  // wjets
  double* distrPU3_MC = new double[35];
  distrPU3_MC[0]  = 0.008528684;
  distrPU3_MC[1]  = 0.019303381;
  distrPU3_MC[2]  = 0.031652782;
  distrPU3_MC[3]  = 0.043396104;
  distrPU3_MC[4]  = 0.052644081;
  distrPU3_MC[5]  = 0.058805779;
  distrPU3_MC[6]  = 0.061888184;
  distrPU3_MC[7]  = 0.062111977;
  distrPU3_MC[8]  = 0.060493879;
  distrPU3_MC[9]  = 0.057280354;
  distrPU3_MC[10] = 0.053492013;
  distrPU3_MC[11] = 0.049663045;
  distrPU3_MC[12] = 0.045949634;
  distrPU3_MC[13] = 0.042473182;
  distrPU3_MC[14] = 0.039085291;
  distrPU3_MC[15] = 0.036020033;
  distrPU3_MC[16] = 0.033270631;
  distrPU3_MC[17] = 0.030571645;
  distrPU3_MC[18] = 0.027872622;
  distrPU3_MC[19] = 0.025353206;
  distrPU3_MC[20] = 0.022884255;
  distrPU3_MC[21] = 0.020527728;
  distrPU3_MC[22] = 0.018277336;
  distrPU3_MC[23] = 0.016084203;
  distrPU3_MC[24] = 0.014050878;
  distrPU3_MC[25] = 0.012167806;
  distrPU3_MC[26] = 0.010399838;
  distrPU3_MC[27] = 0.008802013;
  distrPU3_MC[28] = 0.007376147;
  distrPU3_MC[29] = 0.006085729;
  distrPU3_MC[30] = 0.004994500;
  distrPU3_MC[31] = 0.004056721;
  distrPU3_MC[32] = 0.003254354;
  distrPU3_MC[33] = 0.002597702;
  distrPU3_MC[34] = 0.002046269;  
  
  // matt
  double* distrPU4_MC = new double[35];
  distrPU4_MC[0]  = 0.00875519114066;
  distrPU4_MC[1]  = 0.0188785985195;
  distrPU4_MC[2]  = 0.0314423751832;
  distrPU4_MC[3]  = 0.0439492798584;
  distrPU4_MC[4]  = 0.0539559449073;
  distrPU4_MC[5]  = 0.0609091258166;
  distrPU4_MC[6]  = 0.062956033794;
  distrPU4_MC[7]  = 0.0614224245283;
  distrPU4_MC[8]  = 0.0601866194333;
  distrPU4_MC[9]  = 0.0573963133217;
  distrPU4_MC[10] = 0.0528821476023;
  distrPU4_MC[11] = 0.0491004505364;
  distrPU4_MC[12] = 0.0453019240045;
  distrPU4_MC[13] = 0.0417565165095;
  distrPU4_MC[14] = 0.0387848583941;
  distrPU4_MC[15] = 0.03570110056;
  distrPU4_MC[16] = 0.0329548025347;
  distrPU4_MC[17] = 0.0305922937102;
  distrPU4_MC[18] = 0.0279554839347;
  distrPU4_MC[19] = 0.0253289265925;
  distrPU4_MC[20] = 0.0229138982276;
  distrPU4_MC[21] = 0.0205876598038;
  distrPU4_MC[22] = 0.0182827000151;
  distrPU4_MC[23] = 0.0160811350488;
  distrPU4_MC[24] = 0.0140802663002;
  distrPU4_MC[25] = 0.0121320138129;
  distrPU4_MC[26] = 0.0103720772465;
  distrPU4_MC[27] = 0.008758479657;
  distrPU4_MC[28] = 0.0072948964451;
  distrPU4_MC[29] = 0.00606257330853;
  distrPU4_MC[30] = 0.00501266610755;
  distrPU4_MC[31] = 0.00404535869927;
  distrPU4_MC[32] = 0.00322439026796;
  distrPU4_MC[33] = 0.00257935746066;
  distrPU4_MC[34] = 0.00202427524727;
  
  TH1F* h_PU1 = new TH1F("h_PU1","",35,-0.5,34.5);
  TH1F* h_PU2 = new TH1F("h_PU2","",35,-0.5,34.5);
  TH1F* h_PU3 = new TH1F("h_PU3","",35,-0.5,34.5);
  TH1F* h_PU4 = new TH1F("h_PU4","",35,-0.5,34.5);
  
  for(int bin = 1; bin <= h_PU1->GetNbinsX(); ++bin)
  {
    h_PU1 -> SetBinContent(bin,distrPU1_MC[bin-1]);
    h_PU2 -> SetBinContent(bin,distrPU2_MC[bin-1]);
    h_PU3 -> SetBinContent(bin,distrPU3_MC[bin-1]);
    h_PU4 -> SetBinContent(bin,distrPU4_MC[bin-1]);
  }
  
  
  
  TCanvas* c = new TCanvas();
  c -> cd();
  c -> SetGridx();
  c -> SetGridy();
  
  //h_PU1 -> GetXaxis() -> SetTitle("n_{PU}");
  //h_PU1 -> GetXaxis() -> SetLabelSize(0.04);
  //h_PU1 -> GetYaxis() -> SetLabelSize(0.04);
  //h_PU1 -> GetYaxis() -> SetTitle("event fraction");
  //h_PU1 -> Scale(1./h_PU1->Integral());
  //h_PU1 -> SetMarkerStyle(20);
  //h_PU1 -> SetLineColor(kRed);
  //h_PU1 -> Draw("");
  //c -> Update();
  
  h_PU2 -> GetXaxis() -> SetTitle("n_{PU}");
  h_PU2 -> GetXaxis() -> SetLabelSize(0.04);
  h_PU2 -> GetYaxis() -> SetLabelSize(0.04);
  h_PU2 -> GetYaxis() -> SetTitle("event fraction");
  h_PU2 -> Scale(1./h_PU2->Integral());
  h_PU2 -> SetMarkerStyle(20);
  h_PU2 -> SetLineColor(kBlue);
  h_PU2 -> Draw("");
  c -> Update();
  
  h_PU3 -> GetXaxis() -> SetTitle("n_{PU}");
  h_PU3 -> GetXaxis() -> SetLabelSize(0.04);
  h_PU3 -> GetYaxis() -> SetLabelSize(0.04);
  h_PU3 -> GetYaxis() -> SetTitle("event fraction");
  h_PU3 -> Scale(1./h_PU3->Integral());
  h_PU3 -> SetMarkerStyle(20);
  h_PU3 -> SetLineColor(kGreen);
  h_PU3 -> Draw("same");
  c -> Update();
  
  h_PU4 -> GetXaxis() -> SetTitle("n_{PU}");
  h_PU4 -> GetXaxis() -> SetLabelSize(0.04);
  h_PU4 -> GetYaxis() -> SetLabelSize(0.04);
  h_PU4 -> GetYaxis() -> SetTitle("event fraction");
  h_PU4 -> Scale(1./h_PU4->Integral());
  h_PU4 -> SetMarkerStyle(20);
  h_PU4 -> SetLineColor(kMagenta);
  h_PU4 -> Draw("same");
  c -> Update();
  
  TLegend* legend = new TLegend(0.70,0.80,0.90,0.90);
  legend -> SetFillColor(kWhite);
  legend -> SetFillStyle(4000);
  legend -> SetTextFont(42);
  legend -> SetTextSize(0.03);
  legend -> AddEntry(h_PU1,"twiki - true","L");
  legend -> AddEntry(h_PU2,"twiki - intime","L");
  legend -> AddEntry(h_PU3,"WJets","L");
  legend -> AddEntry(h_PU4,"matt","L");
  legend -> Draw("same");
}
