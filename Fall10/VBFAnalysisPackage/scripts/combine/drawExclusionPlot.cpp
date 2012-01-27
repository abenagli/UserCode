// g++ -Wall -o drawExclusionPlot `root-config --glibs --cflags` drawExclusionPlot.cpp

#include <iostream>
#include <fstream>
#include <cmath>

#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TF1.h"
#include "TLegend.h"
#include "TAxis.h"
#include "TROOT.h"



int main(int argc, char** argv)
{
  if( argc != 3)
  {
    std::cout << ">>> usage: ./drawExclusionPlot folder name" << std::endl;
    return -1;
  }
  
  std::cout << ">>> drawExclusionPlot::name " << argv[2] << " <<<" << std::endl;
  
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  
  
  std::string folder(argv[1]);
  std::string name(argv[2]);
  
  
  double mass[200];
  double b_vals_exclusion_median_data[200];
  double b_vals_exclusion_median[200];
  double b_down_exclusion_68[200];
  double b_up_exclusion_68[200];
  double b_down_exclusion_95[200];
  double b_up_exclusion_95[200];
  
  std::ifstream inFile_mass  ((folder+"/results_masses.txt").c_str());
  std::ifstream inFile_data  ((folder+"/results_observed.txt").c_str());
  std::ifstream inFile_95low ((folder+"/results_2s_low.txt").c_str());
  std::ifstream inFile_68low ((folder+"/results_1s_low.txt").c_str());
  std::ifstream inFile_median((folder+"/results_median.txt").c_str());
  std::ifstream inFile_68hig ((folder+"/results_1s_hig.txt").c_str());
  std::ifstream inFile_95hig ((folder+"/results_2s_hig.txt").c_str());
  
  float val = -1;
  float val2 = -1;
  
  int numMass = 0;
  while(1)
  {
    inFile_mass >> val;
    if(!inFile_mass.good()) break;
    
    mass[numMass] = val;
    
    ++numMass;
  }
  
  numMass = 0;  
  while(1)
  {
    inFile_data >> val;
    if(!inFile_data.good()) break;
    
    b_vals_exclusion_median_data[numMass] = val;
    ++numMass;
  }
  
  numMass = 0;  
  while(1)
  {
    inFile_median >> val;
    if(!inFile_median.good()) break;
    
    b_vals_exclusion_median[numMass] = val;
    ++numMass;
  }
  
  numMass = 0;  
  while(1)
  {
    inFile_68low >> val;
    inFile_68hig >> val2;
    
    if(!inFile_68low.good()) break;
    if(!inFile_68hig.good()) break;
    
    b_down_exclusion_68[numMass] = fabs( val - b_vals_exclusion_median[numMass] );
    b_up_exclusion_68[numMass] = fabs( val2 - b_vals_exclusion_median[numMass] );
    
    ++numMass;
  }
  
  numMass = 0;  
  while(1)
  {
    inFile_95low >> val;
    inFile_95hig >> val2;
    
    if(!inFile_95low.good()) break;
    if(!inFile_95hig.good()) break;
    
    b_down_exclusion_95[numMass] = fabs( val - b_vals_exclusion_median[numMass] );
    b_up_exclusion_95[numMass] = fabs( val2 - b_vals_exclusion_median[numMass] );
    
    ++numMass;
  }
  
  int n_points = numMass;
  
  
  
  // bkg hypothesis line
  TGraph* m_exclusion_b_line_graph_obs = new TGraph(n_points, mass, b_vals_exclusion_median_data);
  m_exclusion_b_line_graph_obs->SetLineWidth(2);
  m_exclusion_b_line_graph_obs->SetLineStyle(1);
  m_exclusion_b_line_graph_obs->SetFillColor(kWhite);
  m_exclusion_b_line_graph_obs->SetLineColor(kBlack);
  m_exclusion_b_line_graph_obs->SetMarkerStyle(20);
  m_exclusion_b_line_graph_obs->SetMarkerSize(1.5);
  m_exclusion_b_line_graph_obs->SetMarkerColor(kBlack);
  
  // bkg hypothesis line
  TGraph* m_exclusion_b_line_graph = new TGraph(n_points, mass, b_vals_exclusion_median);
  m_exclusion_b_line_graph->SetLineWidth(2);
  m_exclusion_b_line_graph->SetLineStyle(2);
  m_exclusion_b_line_graph->SetFillColor(kWhite);
  m_exclusion_b_line_graph->SetLineColor(kBlack);
  m_exclusion_b_line_graph->SetMarkerSize(0.7);
  m_exclusion_b_line_graph->SetMarkerColor(kBlue);
  
  // bkg hypothesis band 1 sigma
  TGraphAsymmErrors* m_exclusion_b_band_graph_1sigma = new TGraphAsymmErrors(n_points,mass,b_vals_exclusion_median,0,0,b_down_exclusion_68,b_up_exclusion_68);
  m_exclusion_b_band_graph_1sigma->SetFillColor(kGreen);
  m_exclusion_b_band_graph_1sigma->SetFillStyle(3002);
  m_exclusion_b_band_graph_1sigma->SetLineColor(kGreen);
  m_exclusion_b_band_graph_1sigma->SetMarkerColor(kGreen);
  
  // bkg hypothesis band 2 sigma
  TGraphAsymmErrors* m_exclusion_b_band_graph_2sigma = new TGraphAsymmErrors(n_points,mass,b_vals_exclusion_median,0,0,b_down_exclusion_95,b_up_exclusion_95);
  m_exclusion_b_band_graph_2sigma->SetFillColor(kYellow);
  m_exclusion_b_band_graph_2sigma->SetFillStyle(3002);
  m_exclusion_b_band_graph_2sigma->SetLineColor(kYellow);
  m_exclusion_b_band_graph_2sigma->SetMarkerColor(kYellow);
  
  
  
  TLegend* m_exclusion_legend = new TLegend(0.25,0.85,0.75,0.99);
  m_exclusion_legend->SetFillColor(0);
  m_exclusion_legend->AddEntry(m_exclusion_b_line_graph,"95% CL exclusion: median expected");
  m_exclusion_legend->AddEntry(m_exclusion_b_band_graph_1sigma,"95% CL exclusion: 68% band");
  m_exclusion_legend->AddEntry(m_exclusion_b_band_graph_2sigma,"95% CL exclusion: 95% band");
  m_exclusion_legend->AddEntry(m_exclusion_b_line_graph_obs,"95% CL exclusion: observed");
  
  
  
  TCanvas *cExclusion = new TCanvas("cExclusion","",1000,750);
  cExclusion->SetGridx();
  cExclusion->SetGridy();
  cExclusion->SetLogy();
  
  m_exclusion_b_band_graph_2sigma->GetYaxis()->SetTitle("r = #sigma_{95%CL} / #sigma_{SM}");
  m_exclusion_b_band_graph_2sigma->GetXaxis()->SetTitle("Higgs mass (GeV/c^{2})");
  m_exclusion_b_band_graph_2sigma->SetMinimum(0.2);
  m_exclusion_b_band_graph_2sigma->SetMaximum(20.);
  m_exclusion_b_band_graph_2sigma->SetTitle("");
  m_exclusion_b_band_graph_2sigma->Draw("A3");
  
  m_exclusion_b_band_graph_1sigma->Draw("3,same");

  m_exclusion_b_line_graph->Draw("L,same");
  m_exclusion_b_line_graph_obs->Draw("PL,same");
  m_exclusion_legend->Draw("same");
  
  TF1* f = new TF1("f", "1.", 0., 1000.);
  f->SetLineColor(kRed+1);
  f->SetLineWidth(2);
  f->Draw("same");
  
  cExclusion->Print((name+".pdf").c_str(),"pdf");
  
  
  
  return 0;
}


