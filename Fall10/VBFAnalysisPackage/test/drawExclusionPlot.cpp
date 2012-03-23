#include "setTDRStyle.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>

#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TF1.h"
#include "TLegend.h"
#include "TAxis.h"



int drawObserved = 1;

float yMin_lin = 0.;
float yMax_lin = 15.;

float yMin_log = 0.1;
float yMax_log = 15.;

int ReadVals(const std::string& folder,
             double* vals_masses,
             double* vals_observed,
             double* vals_2s_low,
             double* vals_1s_low,
             double* vals_median,
             double* vals_1s_hig,
             double* vals_2s_hig);

void SetGraphStyle(TGraph* g_lin, TGraph* g_log, const std::string& type);






int main(int argc, char** argv)
{
  setTDRStyle();
  gStyle -> SetPadTopMargin(0.05);
  gStyle -> SetPadBottomMargin(0.13);
  gStyle -> SetPadLeftMargin(0.13);
  gStyle -> SetPadRightMargin(0.10);
    
  if( argc < 3 )
  {
    std::cout << ">>> usage: ./drawExclusionPlot folder name   drawObserved(=1)   yMin_lin(0.) yMax_lin(15.)   yMin_log(0.1) yMax_log(15.)" << std::endl;
    return -1;
  }
  
  std::string folder(argv[1]);
  std::string name(argv[2]);
  
  std::cout << ">>> drawExclusionPlot::name " << name << " <<<" << std::endl;
  
  
  
  //------------------------
  // set optional parameters
  
  if( argc == 4 ) drawObserved = atoi(argv[3]);
  if( argc == 5 ) yMin_lin     = atoi(argv[4]);
  if( argc == 6 ) yMax_lin     = atoi(argv[5]);
  if( argc == 7 ) yMin_log     = atoi(argv[6]);
  if( argc == 8 ) yMax_log     = atoi(argv[7]);
  
  
  
  //------------
  // read values
  
  int NMAX = 200;
  double* vals_masses   = new double[NMAX];
  double* vals_observed = new double[NMAX];
  double* vals_2s_low   = new double[NMAX];
  double* vals_1s_low   = new double[NMAX];
  double* vals_median   = new double[NMAX];
  double* vals_1s_hig   = new double[NMAX];
  double* vals_2s_hig   = new double[NMAX];
  
  int nPoints = ReadVals(folder,vals_masses,vals_observed,vals_2s_low,vals_1s_low,vals_median,vals_1s_hig,vals_2s_hig);
  
  
  
  //-------------
  // fill TGraphs
  
  // observed
  TGraph* g_lin_observed = new TGraph(nPoints,vals_masses,vals_observed);
  TGraph* g_log_observed = new TGraph(nPoints,vals_masses,vals_observed);
  SetGraphStyle(g_lin_observed,g_log_observed,"observed");
  
  // median
  TGraph* g_lin_median = new TGraph(nPoints,vals_masses,vals_median);
  TGraph* g_log_median = new TGraph(nPoints,vals_masses,vals_median);
  SetGraphStyle(g_lin_median,g_log_median,"median");
  
  // 1-sigma band
  TGraphAsymmErrors* g_lin_1s = new TGraphAsymmErrors(nPoints,vals_masses,vals_median,0,0,vals_1s_low,vals_1s_hig);
  TGraphAsymmErrors* g_log_1s = new TGraphAsymmErrors(nPoints,vals_masses,vals_median,0,0,vals_1s_low,vals_1s_hig);
  SetGraphStyle(g_lin_1s,g_log_1s,"1s");
  
  // 2-sigma band
  TGraphAsymmErrors* g_lin_2s = new TGraphAsymmErrors(nPoints,vals_masses,vals_median,0,0,vals_2s_low,vals_2s_hig);
  TGraphAsymmErrors* g_log_2s = new TGraphAsymmErrors(nPoints,vals_masses,vals_median,0,0,vals_2s_low,vals_2s_hig);
  SetGraphStyle(g_lin_2s,g_log_2s,"2s");
  
  
  
  //--------------
  // define legend
  
  TLegend* legend = new TLegend(0.280,0.84,0.75,0.99);
  legend -> SetFillColor(kWhite);
  legend -> SetFillStyle(1001);  
  legend -> SetTextFont(42);  
  legend -> SetTextSize(0.03);
  
  legend -> AddEntry(g_lin_median,  "95% CL exclusion: median expected","L");
  legend -> AddEntry(g_lin_1s,      "95% CL exclusion: 68% band",       "F");
  legend -> AddEntry(g_lin_2s,      "95% CL exclusion: 95% band",       "F");
  legend -> AddEntry(g_lin_observed,"95% CL exclusion: observed",       "PL");
  
  TF1* f = new TF1("f", "1.", 0., 1000.);
  f -> SetLineColor(kRed+1);
  f -> SetLineWidth(2);
  
  
  
  //-------------
  // draw TGraphs
  
  TCanvas* c1 = new TCanvas("c1","",1000,750);
  c1 -> SetGridx();
  c1 -> SetGridy();
  
  g_lin_median -> Draw("AL");
  g_lin_2s     -> Draw("3,same");
  g_lin_1s     -> Draw("3,same");
  g_lin_median -> Draw("L,same");
  legend       -> Draw("same");
  f            -> Draw("same");
  if( drawObserved == 1 ) g_lin_observed -> Draw("PL,same");
  
  c1 -> Print(("lin_"+name+".pdf").c_str(),"pdf");
  c1 -> Print(("lin_"+name+".png").c_str(),"png");
  
  
  
  TCanvas* c2 = new TCanvas("c2","",1000,750);
  c2 -> SetGridx();
  c2 -> SetGridy();
  c2 -> SetLogy();
  
  g_log_median -> Draw("AL");
  g_log_2s     -> Draw("3,same");
  g_log_1s     -> Draw("3,same");
  g_log_median -> Draw("L,same");
  legend       -> Draw("same");
  f            -> Draw("same");
  if( drawObserved == 1 ) g_log_observed -> Draw("PL,same");
  
  c2 -> Print(("log_"+name+".pdf").c_str(),"pdf");
  c2 -> Print(("log_"+name+".png").c_str(),"png");
  
  
  
  //-------------
  // save TGraphs
  
  TFile* output = new TFile((name+".root").c_str(),"RECREATE");
  
  output -> cd();
  
  output -> mkdir("lin");
  output -> cd("lin");
  
  g_lin_2s       -> Write("limit_2s");
  g_lin_1s       -> Write("limit_1s");
  g_lin_median   -> Write("limit_median");
  g_lin_observed -> Write("limit_observed");
  
  output -> cd();
  
  output -> mkdir("log");
  output -> cd("log");
  
  g_log_2s       -> Write("limit_2s");
  g_log_1s       -> Write("limit_1s");
  g_log_median   -> Write("limit_median");
  g_log_observed -> Write("limit_observed");
  
  output -> Close();
  
  return 0;
}






int ReadVals(const std::string& folder,
             double* vals_masses,
             double* vals_observed,
             double* vals_2s_low,
             double* vals_1s_low,
             double* vals_median,
             double* vals_1s_hig,
             double* vals_2s_hig)
{
  std::ifstream inFile_masses  ((folder+"/results_masses.txt").c_str());
  std::ifstream inFile_observed((folder+"/results_observed.txt").c_str());
  std::ifstream inFile_2s_low  ((folder+"/results_2s_low.txt").c_str());
  std::ifstream inFile_1s_low  ((folder+"/results_1s_low.txt").c_str());
  std::ifstream inFile_median  ((folder+"/results_median.txt").c_str());
  std::ifstream inFile_1s_hig  ((folder+"/results_1s_hig.txt").c_str());
  std::ifstream inFile_2s_hig  ((folder+"/results_2s_hig.txt").c_str());
  
  float val = -1;
  float val2 = -1;
  
  int nMasses = 0;
  
  while(1)
  {
    inFile_masses >> val;
    if(!inFile_masses.good()) break;
    
    vals_masses[nMasses] = val;
    
    ++nMasses;
  }
  
  nMasses = 0;  
  while(1)
  {
    inFile_observed >> val;
    if(!inFile_observed.good()) break;
    
    vals_observed[nMasses] = val;
    ++nMasses;
  }
  
  nMasses = 0;  
  while(1)
  {
    inFile_median >> val;
    if(!inFile_median.good()) break;
    
    vals_median[nMasses] = val;
    ++nMasses;
  }
  
  nMasses = 0;  
  while(1)
  {
    inFile_1s_low >> val;
    inFile_1s_hig >> val2;
    
    if(!inFile_1s_low.good()) break;
    if(!inFile_1s_hig.good()) break;
    
    vals_1s_low[nMasses] = fabs( val  - vals_median[nMasses] );
    vals_1s_hig[nMasses] = fabs( val2 - vals_median[nMasses] );
    
    ++nMasses;
  }
  
  nMasses = 0;  
  while(1)
  {
    inFile_2s_low >> val;
    inFile_2s_hig >> val2;
    
    if(!inFile_2s_low.good()) break;
    if(!inFile_2s_hig.good()) break;
    
    vals_2s_low[nMasses] = fabs( val  - vals_median[nMasses] );
    vals_2s_hig[nMasses] = fabs( val2 - vals_median[nMasses] );
    
    ++nMasses;
  }
  
  
  
  for(int i = 0; i < nMasses; ++i)
  {
    std::cout << std::fixed;
    std::cout << ">>> mass: "    << std::setprecision(0) << std::setw(3) << vals_masses[i]
              << "      exp. 50%: " << std::setprecision(2) << std::setw(5) << vals_median[i]
              << "      observed: " << std::setprecision(2) << std::setw(5) << vals_observed[i]
              << std::endl;
  }

  return nMasses;
}






void SetGraphStyle(TGraph* g_lin, TGraph* g_log, const std::string& type)
{
  //------------
  // axis titles
  
  g_lin -> GetXaxis() -> SetTitle("m_{H}   (GeV/c^{2})");
  g_log -> GetXaxis() -> SetTitle("m_{H}   (GeV/c^{2})");
  
  g_lin -> GetYaxis() -> SetTitle("r = #sigma_{95%CL} / #sigma_{SM}");
  g_log -> GetYaxis() -> SetTitle("r = #sigma_{95%CL} / #sigma_{SM}");
  
  
  
  // -----------
  // axis ranges
  
  g_lin -> SetMinimum(yMin_lin);
  g_log -> SetMinimum(yMin_log);
  
  g_lin -> SetMaximum(yMax_lin);
  g_log -> SetMaximum(yMax_log);
  
  
  
  //---------------------
  // graphical attributes
  
  g_lin -> SetLineWidth(2);
  g_log -> SetLineWidth(2);
  
  g_lin -> SetMarkerStyle(20);
  g_log -> SetMarkerStyle(20);
  
  if( type == "observed" )
  {
    g_lin -> SetLineStyle(1);
    g_log -> SetLineStyle(1);
    
    g_lin -> SetLineColor(kBlack);
    g_log -> SetLineColor(kBlack);

    g_lin -> SetMarkerSize(1.);
    g_log -> SetMarkerSize(1.);
    
    g_lin -> SetMarkerColor(kBlack);
    g_log -> SetMarkerColor(kBlack);
  }
  
  if( type == "median" )
  {
    g_lin -> SetLineStyle(2);
    g_log -> SetLineStyle(2);
    
    g_lin -> SetLineColor(kBlue);
    g_log -> SetLineColor(kBlue);
    
    g_lin -> SetMarkerSize(0.);
    g_log -> SetMarkerSize(0.);
    
    g_lin -> SetMarkerColor(kBlue);
    g_log -> SetMarkerColor(kBlue);
  }
  
  if( type == "1s" )
  {
    g_lin -> SetLineStyle(1);
    g_log -> SetLineStyle(1);
    
    g_lin -> SetLineColor(kGreen);
    g_log -> SetLineColor(kGreen);
    
    g_lin -> SetLineWidth(0);
    g_log -> SetLineWidth(0);
    
    g_lin -> SetFillColor(kGreen);
    g_log -> SetFillColor(kGreen);
    
    g_lin -> SetFillStyle(3001);
    g_log -> SetFillStyle(3001);
    
    g_lin -> SetMarkerSize(0.);
    g_log -> SetMarkerSize(0.);
    
    g_lin -> SetMarkerSize(kGreen);
    g_log -> SetMarkerSize(kGreen);
  }
  
  if( type == "2s" )
  {
    g_lin -> SetLineStyle(1);
    g_log -> SetLineStyle(1);
    
    g_lin -> SetLineColor(kYellow);
    g_log -> SetLineColor(kYellow);
    
    g_lin -> SetLineWidth(0);
    g_log -> SetLineWidth(0);
    
    g_lin -> SetFillColor(kYellow);
    g_log -> SetFillColor(kYellow);
    
    g_lin -> SetFillStyle(3001);
    g_log -> SetFillStyle(3001);
    
    g_lin -> SetMarkerSize(0.);
    g_log -> SetMarkerSize(0.);
    
    g_lin -> SetMarkerSize(kYellow);
    g_log -> SetMarkerSize(kYellow);
  }
}
