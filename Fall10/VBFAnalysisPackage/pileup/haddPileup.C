#include <iostream>
#include <iomanip>
#include <string>



void haddPileup()
{
  std::vector<std::string> periods;
  //periods.push_back("Run2011AB");
  //periods.push_back("Run2011AB");
  //periods.push_back("Run2011AB");
  //periods.push_back("Run2011AB");
  //periods.push_back("Run2011AB");
  //periods.push_back("Run2011AB");
  //periods.push_back("Run2011AB");
  
  //periods.push_back("Run2011A");
  //periods.push_back("Run2011A");
  //periods.push_back("Run2011A");
  //periods.push_back("Run2011A");
  //periods.push_back("Run2011B");
  //periods.push_back("Run2011B");
  //periods.push_back("Run2011B");
  
  periods.push_back("Run2011A-May10ReReco-v1");
  periods.push_back("Run2011A-PromptReco-v4");
  periods.push_back("Run2011A-05Aug2011-v1");
  periods.push_back("Run2011A-PromptReco-v6");
  periods.push_back("Run2011B-PromptReco-v1");
  periods.push_back("Run2011B-PromptReco-v1");
  periods.push_back("Run2011B-PromptReco-v1");
  
  std::vector<std::string> uniquePeriods;
  //uniquePeriods.push_back("Run2011AB");
  
  //uniquePeriods.push_back("Run2011A");
  //uniquePeriods.push_back("Run2011B");
  
  uniquePeriods.push_back("Run2011A-May10ReReco-v1");
  uniquePeriods.push_back("Run2011A-PromptReco-v4");
  uniquePeriods.push_back("Run2011A-05Aug2011-v1");
  uniquePeriods.push_back("Run2011A-PromptReco-v6");
  uniquePeriods.push_back("Run2011B-PromptReco-v1");
  
  
  
  std::vector<std::string> inFileNames;
  
  inFileNames.push_back("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_160404-163869_7TeV_May10ReReco_Collisions11_JSON_v3.pileupTruth_v2.root");
  inFileNames.push_back("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_165088-167913_7TeV_PromptReco_JSON.pileupTruth_v2.root");
  inFileNames.push_back("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_170249-172619_7TeV_ReReco5Aug_Collisions11_JSON_v2.pileupTruth_v2.root");
  inFileNames.push_back("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_172620-173692_PromptReco_JSON.pileupTruth_v2.root");
  inFileNames.push_back("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_175832-177515_PromptReco_JSON.pileupTruth_v2.root");
  inFileNames.push_back("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_177718_178078_7TeV_PromptReco_Collisons11_JSON.pileupTruth_v2.root");
  inFileNames.push_back("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_178098-180252_7TeV_PromptReco_Collisions11_JSON.pileupTruth.root");
  
  //inFileNames.push_back("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_160404-163869_7TeV_May10ReReco_Collisions11_JSON_v3.pileup_v2.root");
  //inFileNames.push_back("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_165088-167913_7TeV_PromptReco_JSON.pileup_v2.root");
  //inFileNames.push_back("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_170249-172619_7TeV_ReReco5Aug_Collisions11_JSON_v2.pileup_v2.root");
  //inFileNames.push_back("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_172620-173692_PromptReco_JSON.pileup_v2.root");
  //inFileNames.push_back("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_175832-177515_PromptReco_JSON.pileup_v2.root");
  //inFileNames.push_back("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_177718_178078_7TeV_PromptReco_Collisons11_JSON.pileup_v2.root");
  //inFileNames.push_back("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_178098-180252_7TeV_PromptReco_Collisions11_JSON.pileup_v2.root");
  
  //inFileNames.push_back("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_160404-163869_7TeV_May10ReReco_Collisions11_JSON_v3.pileupTruth_v2_finebin.root");
  //inFileNames.push_back("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_165088-167913_7TeV_PromptReco_JSON.pileupTruth_v2_finebin.root");
  //inFileNames.push_back("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_170249-172619_7TeV_ReReco5Aug_Collisions11_JSON_v2.pileupTruth_v2_finebin.root");
  //inFileNames.push_back("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_172620-173692_PromptReco_JSON.pileupTruth_v2_finebin.root");
  //inFileNames.push_back("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_175832-177515_PromptReco_JSON.pileupTruth_v2_finebin.root");
  //inFileNames.push_back("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_177718_178078_7TeV_PromptReco_Collisons11_JSON.pileupTruth_v2_finebin.root");
  //inFileNames.push_back("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_178098-180252_7TeV_PromptReco_Collisions11_JSON.pileupTruth_v2_finebin.root");
  
  
  
  
  
  
  TH1F** pileup = new TH1F*[inFileNames.size()];
  for(unsigned int i = 0; i < inFileNames.size(); ++i)
  {
    pileup[i] = NULL;
  }
  
  TH1F** pileupSummed      = new TH1F*[uniquePeriods.size()];
  TH1F** pileupSummed_norm = new TH1F*[uniquePeriods.size()];
  for(unsigned int i = 0; i < uniquePeriods.size(); ++i)
  {
    pileupSummed[i]      = NULL;
    pileupSummed_norm[i] = NULL;
  }
  
  TH1F* pileupAll = NULL;
  
  
  
  
  
  
  for(unsigned int i = 0; i < inFileNames.size(); ++i)
  {
    std::cout << inFileNames.at(i) << std::endl;
    TFile* inFile = TFile::Open(inFileNames.at(i).c_str());
    
    std::string period = periods.at(i);
    std::string uniquePeriod;
    int uniquePeriodIt = -1;
    while(1)
    {
      ++uniquePeriodIt;
      uniquePeriod = uniquePeriods[uniquePeriodIt];
      if( period == uniquePeriod ) break;
    }
    
    pileup[i] = (TH1F*)( inFile->Get("pileup") );
    std::cout << ">>> Integral: " << pileup[i]->Integral() << std::endl; 
    
    if( pileupSummed[uniquePeriodIt] == NULL )
    {
      TH1F* temp = (TH1F*)( inFile->Get("pileup") );
      
      pileupSummed[uniquePeriodIt] = (TH1F*)( temp->Clone() );
      pileupSummed[uniquePeriodIt] -> SetName(("pileup_"+uniquePeriod).c_str());
      
      pileupSummed_norm[uniquePeriodIt] = (TH1F*)( temp->Clone() );
      pileupSummed_norm[uniquePeriodIt] -> SetName(("pileup_norm_"+uniquePeriod).c_str());
    }
    else
    {
      TH1F* temp = (TH1F*)( inFile->Get("pileup") );
      
      pileupSummed[uniquePeriodIt] -> Add(temp);
      
      pileupSummed_norm[uniquePeriodIt] -> Add(temp);
    }
    
    if( pileupAll == NULL )
    {
      TH1F* temp = (TH1F*)( inFile->Get("pileup") );
      pileupAll = (TH1F*)( temp->Clone() );
      pileupAll -> SetName("pileup_All");
    }
    else
    {
      TH1F* temp = (TH1F*)( inFile->Get("pileup") ); 
      pileupAll -> Add(temp);
    }
  }
  
  
  
  TCanvas* c = new TCanvas("c");
  c -> cd();
  c -> SetGridx();
  c -> SetGridy();
  
  TLegend* legend = new TLegend(0.60,0.60,0.80,0.90);
  legend -> SetFillColor(kWhite);
  legend -> SetFillStyle(4000);  
  legend -> SetTextFont(42);  
  legend -> SetTextSize(0.025);
  
  for(unsigned int i = 0; i < uniquePeriods.size(); ++i)
  {
    std::cout << std::fixed
              << std::setprecision(0) << std::setw(30) << uniquePeriods[i] << "   -   "
              << std::setprecision(0) << std::setw(10) << pileupSummed[i]->Integral() << "   -   "
              << std::setprecision(5) << std::setw(10) << pileupSummed[i]->Integral()/pileupAll->Integral() << std::endl;
    
    pileupSummed[i] -> SetLineWidth(2);
    pileupSummed[i] -> SetLineColor(51+i*10);
    if( i == 0)
      pileupSummed[i] -> Draw();
    else
      pileupSummed[i] -> Draw("same");
    c -> Update();
    
    legend -> AddEntry(pileupSummed[i],uniquePeriods[i].c_str(),"L");
    
    
    std::string outFileName = "pileup_68mb_truth_" + uniquePeriods[i] + ".root";
    TFile* outFile = new TFile(outFileName.c_str(),"RECREATE");
    outFile -> cd();
    pileupSummed[i] -> SetName("pileup");
    pileupSummed[i] -> Write();
    outFile -> Close();
  }
  
  legend -> Draw("same");
  c -> Update();
  
  
  
  TCanvas* c_norm = new TCanvas("c_norm");
  c_norm -> cd();
  c_norm -> SetGridx();
  c_norm -> SetGridy();
  
  for(unsigned int i = 0; i < uniquePeriods.size(); ++i)
  {
    pileupSummed_norm[i] -> Scale(1./pileupSummed_norm[i]->Integral());
    pileupSummed_norm[i] -> SetMaximum(0.4);
    pileupSummed_norm[i] -> SetLineWidth(2);
    pileupSummed_norm[i] -> SetLineColor(51+i*10);
    if( i == 0)
      pileupSummed_norm[i] -> Draw();
    else
      pileupSummed_norm[i] -> Draw("same");
    c_norm -> Update();
  }
  
  legend -> Draw("same");
  c_norm -> Update();
}
