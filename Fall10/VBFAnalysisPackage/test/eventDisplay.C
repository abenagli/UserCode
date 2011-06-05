#include <iomanip>



void eventDisplay(const int& entry = 0)
{
  std::cout << ">>> eventDisplay::displaying event " << entry << std::endl;
  
   
  TFile* f = TFile::Open("MiBiCommonNT_1_1_ewv.root","READ");
  
  TTree* ntu = (TTree*)f->Get("MiBiCommonNTOneLeptonTwoJetsAK5PF/SimpleNtuple");
  
  
    
  // branch variables
  std::vector<ROOT::Math::XYZTVector>* mcQ1_tag = new std::vector<ROOT::Math::XYZTVector>;
  std::vector<float>* mcQ1_tag_pdgId = new std::vector<float>;
  
  std::vector<ROOT::Math::XYZTVector>* mcQ2_tag = new std::vector<ROOT::Math::XYZTVector>;
  std::vector<float>* mcQ2_tag_pdgId = new std::vector<float>;
  
  std::vector<ROOT::Math::XYZTVector>* mcF1_fromV1 = new std::vector<ROOT::Math::XYZTVector>;
  std::vector<float>* mcF1_fromV1_pdgId = new std::vector<float>;
  
  std::vector<ROOT::Math::XYZTVector>* mcF2_fromV1 = new std::vector<ROOT::Math::XYZTVector>;
  std::vector<float>* mcF2_fromV1_pdgId = new std::vector<float>;
  
  std::vector<ROOT::Math::XYZTVector>* mcF1_fromV2 = new std::vector<ROOT::Math::XYZTVector>;
  std::vector<float>* mcF1_fromV2_pdgId = new std::vector<float>;
  
  std::vector<ROOT::Math::XYZTVector>* mcF2_fromV2 = new std::vector<ROOT::Math::XYZTVector>;
  std::vector<float>* mcF2_fromV2_pdgId = new std::vector<float>;
  
    
  
  // branch addresses
  ntu -> SetBranchAddress("mcQ1_tag", &mcQ1_tag);
  ntu -> SetBranchAddress("mcQ1_tag_pdgId", &mcQ1_tag_pdgId);
  
  ntu -> SetBranchAddress("mcQ2_tag", &mcQ2_tag);
  ntu -> SetBranchAddress("mcQ2_tag_pdgId", &mcQ2_tag_pdgId);
  
  ntu -> SetBranchAddress("mcF1_fromV1", &mcF1_fromV1);
  ntu -> SetBranchAddress("mcF1_fromV1_pdgId", &mcF1_fromV1_pdgId);
  
  ntu -> SetBranchAddress("mcF2_fromV1", &mcF2_fromV1);
  ntu -> SetBranchAddress("mcF2_fromV1_pdgId", &mcF2_fromV1_pdgId);
  
  ntu -> SetBranchAddress("mcF1_fromV2", &mcF1_fromV2);
  ntu -> SetBranchAddress("mcF1_fromV2_pdgId", &mcF1_fromV2_pdgId);
    
  ntu -> SetBranchAddress("mcF2_fromV2", &mcF2_fromV2);
  ntu -> SetBranchAddress("mcF2_fromV2_pdgId", &mcF2_fromV2_pdgId);  
  
  ntu -> GetEntry(entry);
  
  
  // variables
  ROOT::Math::XYZTVector tagQ1 = mcQ1_tag->at(0);
  int tagQ1_pdgId = (int)(mcQ1_tag_pdgId->at(0));
  
  ROOT::Math::XYZTVector tagQ2 = mcQ2_tag->at(0);
  int tagQ2_pdgId = (int)(mcQ2_tag_pdgId->at(0));
  
  ROOT::Math::XYZTVector lep = mcF1_fromV1->at(0);
  int lep_pdgId = (int)(mcF1_fromV1_pdgId->at(0));
  
  ROOT::Math::XYZTVector nu = mcF2_fromV1->at(0);
  int nu_pdgId = (int)(mcF2_fromV1_pdgId->at(0));
    
  ROOT::Math::XYZTVector WQ1 = mcF1_fromV2->at(0);
  int WQ1_pdgId = (int)(mcF1_fromV2_pdgId->at(0));

  ROOT::Math::XYZTVector WQ2 = mcF2_fromV2->at(0);
  int WQ2_pdgId = (int)(mcF2_fromV2_pdgId->at(0));  
  
    
  
  
  
  
  // order leptons
  if( (lep_pdgId%2) == 0 )
  {
    lep = mcF2_fromV1->at(0);
    lep_pdgId = mcF2_fromV1_pdgId->at(0);
    
    nu  = mcF1_fromV1->at(0);
    nu_pdgId = mcF1_fromV1_pdgId->at(0);
  }
  
  // order quarks
  if( (WQ1_pdgId%2) == 0 )
  {
    WQ1 = mcF2_fromV2->at(0);
    WQ1_pdgId = mcF2_fromV2_pdgId->at(0);
    
    WQ2 = mcF1_fromV2->at(0);
    WQ2_pdgId = mcF1_fromV2_pdgId->at(0);  
  }
  
  
  
  
  
  // printout
  std::cout << fixed;
  
  std::cout << ">>> tag" << std::endl;
  PrintEventInfo(tagQ1_pdgId,tagQ1.eta(),tagQ1.phi(),tagQ1.pt());
  PrintEventInfo(tagQ2_pdgId,tagQ2.eta(),tagQ2.phi(),tagQ2.pt());
  
  std::cout << std::endl;
  
  std::cout << ">>> W1" << std::endl;
  PrintEventInfo(lep_pdgId,lep.eta(),lep.phi(),lep.pt());
  PrintEventInfo(nu_pdgId,nu.eta(),nu.phi(),nu.pt());
  
  std::cout << std::endl;
  
  std::cout << ">>> W2" << std::endl;
  PrintEventInfo(WQ1_pdgId,WQ1.eta(),WQ1.phi(),WQ1.pt());
  PrintEventInfo(WQ2_pdgId,WQ2.eta(),WQ2.phi(),WQ2.pt());
  
  
  
  
  
    
  TCanvas* c_xy = new TCanvas("c_xy","c_xy");
  c_xy -> SetGridx();
  c_xy -> SetGridy();
  
  
  TGraph* axis_xy = new TGraph();
  axis_xy -> SetPoint(0,-100.,-100.);
  axis_xy -> SetPoint(1,100.,100.);
  axis_xy -> SetMarkerColor(kWhite);
  axis_xy ->Draw("AP");
  
  
  TGraph* g_lep_xy = new TGraph();
  g_lep_xy -> SetPoint(0,0.,0.);
  g_lep_xy -> SetPoint(1,lep.px(),lep.py());
  g_lep_xy -> SetLineWidth(2);
  g_lep_xy -> SetMarkerSize(0.5);
  g_lep_xy -> Draw("PL,same");
  
  
  TGraph* g_nu_xy = new TGraph();
  g_nu_xy -> SetPoint(0,0.,0.);
  g_nu_xy -> SetPoint(1,nu.px(),nu.py());
  g_nu_xy -> SetLineWidth(2);
  g_nu_xy -> SetLineStyle(2);
  g_nu_xy -> SetMarkerSize(0.5);
  g_nu_xy -> Draw("PL,same");
  
  
  TGraph* g_WQ1_xy = new TGraph();
  g_WQ1_xy -> SetPoint(0,0.,0.);
  g_WQ1_xy -> SetPoint(1,WQ1.px(),WQ1.py());
  g_WQ1_xy -> SetLineWidth(2);
  g_WQ1_xy -> SetLineColor(kRed);
  g_WQ1_xy -> SetMarkerSize(0.5);
  g_WQ1_xy -> SetMarkerColor(kRed);
  g_WQ1_xy -> Draw("PL,same");
  
  
  TGraph* g_WQ2_xy = new TGraph();
  g_WQ2_xy -> SetPoint(0,0.,0.);
  g_WQ2_xy -> SetPoint(1,WQ2.px(),WQ2.py());
  g_WQ2_xy -> SetLineWidth(2);
  g_WQ2_xy -> SetLineStyle(2);
  g_WQ2_xy -> SetLineColor(kRed);
  g_WQ2_xy -> SetMarkerSize(0.5);
  g_WQ2_xy -> SetMarkerColor(kRed);
  g_WQ2_xy -> Draw("PL,same");
  
  
  
  
  
  
  TCanvas* c_zy = new TCanvas("c_zy","c_zy");
  c_zy -> SetGridx();
  c_zy -> SetGridy();
  
  
  TGraph* axis_zy = new TGraph();
  axis_zy -> SetPoint(0,-250.,-100.);
  axis_zy -> SetPoint(1,250.,100.);
  axis_zy -> SetMarkerColor(kWhite);
  axis_zy ->Draw("AP");
  
  
  TGraph* g_tagQ1_zy = new TGraph();
  g_tagQ1_zy -> SetPoint(0,0.,0.);
  g_tagQ1_zy -> SetPoint(1,tagQ1.pz(),tagQ1.py());
  g_tagQ1_zy -> SetLineWidth(3);
  g_tagQ1_zy -> SetLineColor(kBlue);
  g_tagQ1_zy -> SetMarkerSize(0.5);
  g_tagQ1_zy -> SetMarkerColor(kBlue);
  g_tagQ1_zy -> Draw("PL,same");
  
  TGraph* g_tagQ2_zy = new TGraph();
  g_tagQ2_zy -> SetPoint(0,0.,0.);
  g_tagQ2_zy -> SetPoint(1,tagQ2.pz(),tagQ2.py());
  g_tagQ2_zy -> SetLineWidth(3);
  g_tagQ2_zy -> SetLineColor(kBlue);
  g_tagQ2_zy -> SetMarkerSize(0.5);
  g_tagQ2_zy -> SetMarkerColor(kBlue);
  g_tagQ2_zy -> Draw("PL,same");
  
  
  TGraph* g_lep_zy = new TGraph();
  g_lep_zy -> SetPoint(0,0.,0.);
  g_lep_zy -> SetPoint(1,lep.pz(),lep.py());
  g_lep_zy -> SetLineWidth(2);
  g_lep_zy -> SetMarkerSize(0.5);
  g_lep_zy -> Draw("PL,same");
  
  
  TGraph* g_nu_zy = new TGraph();
  g_nu_zy -> SetPoint(0,0.,0.);
  g_nu_zy -> SetPoint(1,nu.pz(),nu.py());
  g_nu_zy -> SetLineWidth(2);
  g_nu_zy -> SetLineStyle(2);
  g_nu_zy -> SetMarkerSize(0.5);
  g_nu_zy -> Draw("PL,same");
  
  
  TGraph* g_WQ1_zy = new TGraph();
  g_WQ1_zy -> SetPoint(0,0.,0.);
  g_WQ1_zy -> SetPoint(1,WQ1.pz(),WQ1.py());
  g_WQ1_zy -> SetLineWidth(2);
  g_WQ1_zy -> SetLineColor(kRed);
  g_WQ1_zy -> SetMarkerSize(0.5);
  g_WQ1_zy -> SetMarkerColor(kRed);
  g_WQ1_zy -> Draw("PL,same");
  
  
  TGraph* g_WQ2_zy = new TGraph();
  g_WQ2_zy -> SetPoint(0,0.,0.);
  g_WQ2_zy -> SetPoint(1,WQ2.pz(),WQ2.py());
  g_WQ2_zy -> SetLineWidth(2);
  g_WQ2_zy -> SetLineStyle(2);
  g_WQ2_zy -> SetLineColor(kRed);
  g_WQ2_zy -> SetMarkerSize(0.5);
  g_WQ2_zy -> SetMarkerColor(kRed);
  g_WQ2_zy -> Draw("PL,same");
  
  
  
  f -> Close();
}






void PrintEventInfo(int pdgId, float eta, float phi, float pt)
{
  if( pdgId == +11 )
    std::cout << ">>>>>>           e   charge:   -1";
  if( pdgId == -11 )
    std::cout << ">>>>>>      anti-e   charge:   +1";
  if( pdgId == +13 )
    std::cout << ">>>>>>          mu   charge:   -1";
  if( pdgId == -13 )
    std::cout << ">>>>>>     anti-mu   charge:   +1";
  if( pdgId == +15 )
    std::cout << ">>>>>>         tau   charge:   -1";
  if( pdgId == -15 )
    std::cout << ">>>>>>    anti-tau   charge:   +1";
  
  
  
  if( pdgId == +12 )
    std::cout << ">>>>>>        nu_e   charge:    0";
  if( pdgId == -12 )
    std::cout << ">>>>>>   anti-nu_e   charge:    0";
  if( pdgId == +14 )
    std::cout << ">>>>>>       nu_mu   charge:    0";
  if( pdgId == -14 )
    std::cout << ">>>>>>  anti-nu_mu   charge:    0";
  if( pdgId == +16 )
    std::cout << ">>>>>>      nu_tau   charge:    0";
  if( pdgId == -16 )
    std::cout << ">>>>>> anti-nu_tau   charge:    0";
  
  
  
  if( pdgId == +1 )
    std::cout << ">>>>>>           d   charge: -1/3";
  if( pdgId == -1 )
    std::cout << ">>>>>>      anti-d   charge: +1/3";
  
  if( pdgId == +2 )
    std::cout << ">>>>>>           u   charge: +2/3";
  if( pdgId == -2 )
    std::cout << ">>>>>>      anti-u   charge: -2/3";
  
  if( pdgId == +3 )
    std::cout << ">>>>>>           s   charge: -1/3";
  if( pdgId == -3 )
    std::cout << ">>>>>>      anti-s   charge: +1/3";
  
  if( pdgId == +4 )
    std::cout << ">>>>>>           c   charge: +2/3";
  if( pdgId == -4 )
    std::cout << ">>>>>>      anti-c   charge: -2/3";
  
  if( pdgId == +5 )
    std::cout << ">>>>>>           b   charge: -1/3";
  if( pdgId == -5 )
    std::cout << ">>>>>>      anti-b   charge: +1/3";
  
  if( pdgId == +6 )
    std::cout << ">>>>>>           t   charge: +2/3";
  if( pdgId == -6 )
    std::cout << ">>>>>>      anti-t   charge: -2/3";
   
   
  
  std::cout << std::setprecision(2)
            << "      eta: " << setw(5) << eta
            << "   phi: "    << setw(5) << phi
            << std::setprecision(1)
            << "      pt: "  << setw(5) << pt
            << std::endl;
}
