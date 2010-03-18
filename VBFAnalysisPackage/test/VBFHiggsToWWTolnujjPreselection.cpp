#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"

#include <iomanip>

#include "TH1F.h"
#include "RooFitResult.h"
#include "RooWorkspace.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "RooAddPdf.h"
#include "RooGaussian.h"





int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there                                                                                                                               
  if(argc != 2)
  {
    std::cerr << ">>>>> VBFHiggsToWWTolnujjPreselection::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file                                                                                                                                                          
  parseConfigFile (argv[1]) ;
  
  std::string inputFileList = gConfigParser -> readStringOption("Input::inputFileList");
  std::string jetAlgorithm  = gConfigParser -> readStringOption("Input::jetAlgorithm");
  
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  
  int entryMAX = gConfigParser -> readIntOption("Options::entryMAX");
  int entryMODULO = gConfigParser -> readIntOption("Options::entryMODULO");
  
  int nJetMIN = gConfigParser -> readIntOption("Cuts::nJetMIN");
  float jetEtMIN = gConfigParser -> readFloatOption("Cuts::jetEtMIN");
  
  int nLepMIN = gConfigParser -> readIntOption("Cuts::nLepMIN");
  float lepPtMIN = gConfigParser -> readFloatOption("Cuts::lepPtMIN");
  float lepTkIsoOverPtMAX = gConfigParser -> readFloatOption("Cuts::lepTkIsoOverPtMAX");
  float eleIdValueMIN = gConfigParser -> readFloatOption("Cuts::eleIdValueMIN");
  
  float VBFDetaMIN = gConfigParser -> readFloatOption("Cuts::VBFDetaMIN");
  float VBFMjjMIN = gConfigParser -> readFloatOption("Cuts::VBFMjjMIN");
  
  int totalEvents = GetTotalEvents("MCDecayModeFilterAll/totalEvents", inputFileList.c_str());  

  std::string VBFPreselectionName = "VBFEtMinCaloJetCountFilter" + jetAlgorithm+"/passedEvents"; 
  int VBFPreselectionEvents = GetTotalEvents(VBFPreselectionName.c_str(), inputFileList.c_str());  
  
  
  
  
  // Open old tree
  std::string treeName = "VBFNtuple" + jetAlgorithm + "/SimpleNtple";
  TChain* chain = new TChain(treeName.c_str());
  if(!FillChain(*chain, inputFileList.c_str())) return 1;
  treeReader reader((TTree*)(chain));
  
  
  
  // Open output root file for clone tree
  outputRootFileName += "_" + jetAlgorithm + ".root";
  TFile outputRootFile((outputRootFilePath+outputRootFileName).c_str(), "RECREATE");
  outputRootFile.cd();
  
  TTree* cloneTree = chain -> CloneTree(0);
  
  
  
  // define histograms
  int nStep = 5;
  TH1F* events = new TH1F("events", "events", nStep, 0., 1.*nStep);
  std::map<int, int> stepEvents;
  std::map<int, std::string> stepName;

  int step = 0;
  stepEvents[step] = totalEvents;
  stepName[step] = "total events";

  step = step+1;
  stepEvents[step] = VBFPreselectionEvents;
  stepName[step] = "VBFPreselection";
  
  
  
  // define histograms
  
  // tag jets
  TH1F tag_etJ1  ("tag_etJ1",   "",  500, 0.,  500.);
  TH1F tag_etJ2  ("tag_etJ2",   "",  500, 0.,  500.);
  TH1F tag_etJJ  ("tag_etJJ",   "", 1000, 0., 1000.);
  TH1F tag_mJJ   ("tag_mJJ" ,   "", 3000, 0., 3000.);  
  TH1F tag_DetaJJ("tag_DetaJJ", "",  400, 0.,   10.);
  TH1F tag_DphiJJ("tag_DphiJJ", "",  200, 0.,    5.);
  
  // W jets
  TH1F W_etJ1    ("W_etJ1",     "", 500,  0., 500.);
  TH1F W_etJ2    ("W_etJ2",     "", 500,  0., 500.);  
  TH1F W_etJJ    ("W_etJJ",     "", 500,  0., 500.);
  TH1F W_mJJ     ("W_mJJ" ,     "", 500,  0., 500.);  
  TH1F W_mtJJ    ("W_mtJJ",     "", 500,  0., 500.);  
  TH1F W_DetaJJ  ("W_DetaJJ",   "", 400,  0.,  10.);  
  TH1F W_DphiJJ  ("W_DphiJJ",   "", 200,  0.,   5.);  
  TH1F W_zeppJ1  ("W_zeppJ1",   "", 400, -1.,   1.);  
  TH1F W_zeppJ2  ("W_zeppJ2",   "", 400, -1.,   1.);  
  TH1F W_zeppJJ  ("W_zeppJJ",   "", 400, -1.,   1.);
  TH1F W_tag_Dphi("W_tag_Dphi", "", 200,  0.,   5.);  
  
  // lepton
  TH1F lep_pt       ("lep_pt",         "", 500,  0., 500.);
  TH1F lep_zepp     ("lep_zepp",       "", 400, -1.,   1.);
  TH1F lep_W_Deta   ("lep_W_Deta",     "", 400,  0.,  10.);  
  TH1F lep_W_Dphi   ("lep_W_Dphi",     "", 200,  0.,   5.);  
  TH1F lep_tag_Dphi ("lep_tag_Dphi",   "", 200,  0.,   5.);
  TH1F lepMet_mt    ("lepMet_mt",      "", 500,  0., 500.);
  TH1F lepMet_W_Dphi("lepMet_W_Dphi",  "", 200,  0.,   5.);
    
  // met
  TH1F met_et      ("met_et",       "", 500, 0., 500.);
  TH1F met_lep_Dphi("met_lep_Dphi", "", 200, 0.,   5.);
  TH1F met_W_Dphi  ("met_W_Dphi",   "", 200, 0.,   5.);
  TH1F met_tag_Dphi("met_tag_Dphi", "", 200, 0.,   5.);  
  
  // higgs  
  TH1F lepMetW_pt      ("lepMetW_pt",       "",  500, 0.,  500.);
  TH1F lepMetW_mt      ("lepMetW_mt",       "", 1000, 0., 1000.);  
  TH1F lepMetW_tag_Dphi("lepMetW_tag_Dphi", "",  200, 0., 5.);  
  
  // pt tot  
  TH1F lepMetWTag_pt("lepMetWTag_pt", "", 500, 0., 500.);  
  
  
  
  
  
  
  // Loop over events
  std::cout << ">>>>> VBFHiggsToWWTolnujjPreselection::Read " << chain -> GetEntries() << " entries" << std::endl;  
  for(int entry = 0 ; entry < chain -> GetEntries() ; ++entry)
  {
    reader.GetEntry(entry);
    if((entry%entryMODULO) == 0) std::cout << ">>>>> VBFHiggsToWWTolnujjPreselection::GetEntry " << entry << std::endl;   
    if(entry == entryMAX) break;
    
    
    
    //***************
    // cut on leptons
    step = step+1;
    stepName[step] = "1 lepton";
    
    int nLep = 0;
    std::vector<ROOT::Math::XYZTVector> electrons;
    std::vector<ROOT::Math::XYZTVector> muons;
    std::vector<ROOT::Math::XYZTVector> leptons;
    std::vector<std::string> leptonFlavours;    
    std::vector<float> leptonTkIso;
        
    for(unsigned int eleIt = 0; eleIt < (reader.Get4V("electrons")->size()); ++eleIt)
    {
      if( reader.Get4V("electrons")->at(eleIt).pt() < lepPtMIN ) continue;
      if( (reader.GetFloat("electrons_tkIso")->at(eleIt)) / reader.Get4V("electrons")->at(eleIt).pt() > lepTkIsoOverPtMAX ) continue;
      if( (reader.GetFloat("electrons_IdRobustLoose")->at(eleIt)) < eleIdValueMIN ) continue;
      ++nLep;
      
      electrons.push_back( reader.Get4V("electrons")->at(eleIt) );
      leptons.push_back( reader.Get4V("electrons")->at(eleIt) );      
      leptonFlavours.push_back("electron");
      leptonTkIso.push_back(reader.GetFloat("electrons_tkIso")->at(eleIt));
    }
    
    for(unsigned int muIt = 0; muIt < (reader.Get4V("muons")->size()); ++muIt)
    {
      if( reader.Get4V("muons")->at(muIt).pt() < lepPtMIN ) continue;
      if( (reader.GetFloat("muons_tkIsoR03")->at(muIt)) / reader.Get4V("muons")->at(muIt).pt() > lepTkIsoOverPtMAX ) continue;
      ++nLep;
      
      muons.push_back( reader.Get4V("muons")->at(muIt) );
      leptons.push_back( reader.Get4V("muons")->at(muIt) );      
      leptonFlavours.push_back("muon");
    }
    
    if( nLep < nLepMIN ) continue;
    stepEvents[step] += 1;
    
    
    
    //************
    // cut on jets
    step = step+1;
    stepName[step] = "4 jets";
    
    int nJet = 0;
    std::vector<ROOT::Math::XYZTVector> jets;
    for(unsigned int jetIt = 0; jetIt < (reader.Get4V("jets")->size()); ++jetIt)
    {
      ROOT::Math::XYZTVector jet = reader.Get4V("jets")->at(jetIt);
      if( jet.Et() < jetEtMIN ) continue;
      
      bool skipJet = false;
      for(unsigned int eleIt = 0; eleIt < electrons.size(); ++eleIt)
      {
        ROOT::Math::XYZTVector ele = electrons.at(eleIt);
        if( deltaR(jet.eta(), jet.phi(), ele.eta(), ele.phi()) < 0.3 )
          skipJet = true;
      }
      if(skipJet == true) continue;
      
      ++nJet;
      jets.push_back( reader.Get4V("jets")->at(jetIt) );
    }
    
    if( nJet < nJetMIN ) continue;
    stepEvents[step] += 1;
    
    
    
    
        
    
    //*********
    // VBF cuts
    step =step+1;
    stepName[step] = "VBF";
    
    bool isVBFCutsOk = false;
    
    for(unsigned int jetIt1 = 0; jetIt1 < jets.size(); ++jetIt1)
      for(unsigned int jetIt2 = jetIt1+1; jetIt2 < jets.size(); ++jetIt2)
      {
        ROOT::Math::XYZTVector jet1 = jets.at(jetIt1);
        ROOT::Math::XYZTVector jet2 = jets.at(jetIt2);
        ROOT::Math::XYZTVector jet12 = jet1 + jet2;
        
        if( (fabs(jet1.eta() - jet2.eta()) > VBFDetaMIN) &&
            (jet12.mass() > VBFMjjMIN) )
          isVBFCutsOk = true;
      }    
    
    if(isVBFCutsOk == false) continue;
    stepEvents[step] += 1;
    
    cloneTree -> Fill();
    
    
    
    
    
    
    //**************
    // distributions
    
    std::vector<int> matchIt_tag;
    std::vector<int> blacklistIt_tag;
    std::vector<int> matchIt_W;
    
    
    // select jets
    SelectJets(matchIt_tag, jets, "maxMJJ", 30.);
    
    blacklistIt_tag.push_back(matchIt_tag.at(0));
    blacklistIt_tag.push_back(matchIt_tag.at(1));
    SelectJets(matchIt_W, jets, "maxPt", 30., &blacklistIt_tag);
    
    if( (matchIt_tag.at(0) == -1) || (matchIt_tag.at(1) == -1) ||
        (matchIt_W.at(0)   == -1) || (matchIt_W.at(1)   == -1) )
      continue;
    
    ROOT::Math::XYZTVector jet1_tag = jets.at(matchIt_tag.at(0));
    ROOT::Math::XYZTVector jet2_tag = jets.at(matchIt_tag.at(1));    
    ROOT::Math::XYZTVector jet12_tag = jet1_tag + jet2_tag;
    
    ROOT::Math::XYZTVector jet1_W = jets.at(matchIt_W.at(0));
    ROOT::Math::XYZTVector jet2_W = jets.at(matchIt_W.at(1));        
    ROOT::Math::XYZTVector jet12_W = jet1_W + jet2_W;
    
    float avgEta_tag = 0.5 * (jet1_tag.eta() + jet2_tag.eta());
    float absDeta_tag = deltaEta(jet1_tag.eta(), jet2_tag.eta());
    
    
    
    // select lepton
    int matchIt_lep = SelectLepton(leptons, "maxPt", 10.);    
    
    if(matchIt_lep == -1) continue;
    ROOT::Math::XYZTVector lepton = leptons.at(matchIt_lep);
    
    
    
    // get met
    ROOT::Math::XYZTVector met = reader.Get4V("met")->at(0);
    ROOT::Math::XYZTVector lepMet = lepton + met;
    ROOT::Math::XYZTVector lepMetW = lepMet + jet12_W;
    ROOT::Math::XYZTVector lepMetWTag = lepMetW + jet12_tag;
    
    
    
    // fill distributions
    
    // tag jets
    float etJ1_tag = jet1_tag.Et();
    tag_etJ1.Fill(etJ1_tag);

    float etJ2_tag = jet2_tag.Et();
    tag_etJ2.Fill(etJ2_tag);
    
    float etJJ_tag = jet12_tag.Et();
    tag_etJJ.Fill(etJJ_tag);
        
    float mJJ_tag = jet12_tag.mass();
    tag_mJJ.Fill(mJJ_tag);
    
    float DetaJJ_tag = deltaEta(jet1_tag.eta(), jet2_tag.eta());
    tag_DetaJJ.Fill(DetaJJ_tag);
    
    float DphiJJ_tag = deltaPhi(jet1_tag.phi(), jet2_tag.phi());
    tag_DphiJJ.Fill(DphiJJ_tag);    
    
    
    
    // W jets
    float etJ1_W = jet1_W.Et();
    W_etJ1.Fill(etJ1_W);
        
    float etJ2_W = jet2_W.Et();
    W_etJ2.Fill(etJ2_W);
    
    float etJJ_W = jet12_W.Et();
    W_etJJ.Fill(etJJ_W);
        
    float mJJ_W = jet12_W.mass();
    W_mJJ.Fill(mJJ_W);
        
    float mtJJ_W = jet12_W.mt();
    W_mtJJ.Fill(mtJJ_W);
    
    float DetaJJ_W = deltaEta(jet1_W.eta(), jet2_W.eta());
    W_DetaJJ.Fill(DetaJJ_W);

    float DphiJJ_W = deltaPhi(jet1_W.phi(), jet2_W.phi());
    W_DphiJJ.Fill(DphiJJ_W);
    
    float zeppJ1_W = (jet1_W.eta() - avgEta_tag) / absDeta_tag;
    W_zeppJ1.Fill(zeppJ1_W);
        
    float zeppJ2_W = (jet2_W.eta() - avgEta_tag) / absDeta_tag;
    W_zeppJ2.Fill(zeppJ2_W);
    
    float zeppJJ_W = (jet12_W.eta() - avgEta_tag) / absDeta_tag;
    W_zeppJJ.Fill(zeppJJ_W);
    
    float Dphi_W_tag = deltaPhi(jet12_W.phi(), jet12_tag.phi());
    W_tag_Dphi.Fill(Dphi_W_tag);    
    
    
    
    // lepton
    float ptLep = lepton.pt();
    lep_pt.Fill(ptLep);
    
    float zeppLep = (lepton.eta() - avgEta_tag) / absDeta_tag;
    lep_zepp.Fill(zeppLep);
        
    float DetaLep_W = deltaEta(lepton.eta(), jet12_W.eta());
    lep_W_Deta.Fill(DetaLep_W); 

    float DphiLep_W = deltaPhi(lepton.phi(), jet12_W.phi());
    lep_W_Dphi.Fill(DphiLep_W);
    
    float DphiLep_tag = deltaPhi(lepton.phi(), jet12_tag.phi());
    lep_tag_Dphi.Fill(DphiLep_tag);
    
    float mtLepMet = lepMet.mt();
    lepMet_mt.Fill(mtLepMet);
        
    float DphiLepMet_W = deltaPhi(lepMet.phi(), jet12_W.phi());
    lepMet_W_Dphi.Fill(DphiLepMet_W);
    
    
    
    // met
    float etMet = met.Et();
    met_et.Fill(etMet);
    
    float DphiMet_lep = deltaPhi(met.phi(), lepton.phi());
    met_lep_Dphi.Fill(DphiMet_lep);
    
    float DphiMet_W = deltaPhi(met.phi(), jet12_W.phi());
    met_W_Dphi.Fill(DphiMet_W);
    
    float DphiMet_tag = deltaPhi(met.phi(), jet12_tag.phi());
    met_tag_Dphi.Fill(DphiMet_tag);    
    
    
    
    // higgs
    float ptLepMetW = lepMetW.pt();
    lepMetW_pt.Fill(ptLepMetW);
        
    float mtLepMetW = lepMetW.mt();
    lepMetW_mt.Fill(mtLepMetW);
    
    float DphiLepMetW_tag = deltaPhi(lepMetW.phi(), jet12_tag.phi());
    lepMetW_tag_Dphi.Fill(DphiLepMetW_tag);    
    
    
    
    // pt tot
    float ptLepMetWTag = lepMetWTag.pt();
    lepMetWTag_pt.Fill(ptLepMetWTag);
    
    
    
} // loop over the events
  
  
  
  
  
  for(step = 0; step < nStep; ++step)
  {
    events -> SetBinContent(step+1, stepEvents[step]);
    events -> GetXaxis() -> SetBinLabel(step+1, stepName[step].c_str());
  }
  
  events -> Write();
  
  
  
  // write histograms in root file
  
  // tag jets
  tag_etJ1.Write();
  tag_etJ2.Write();
  tag_etJJ.Write();
  tag_mJJ.Write();  
  tag_DetaJJ.Write();
  tag_DphiJJ.Write();  
  
  // W jets
  W_etJ1.Write();
  W_etJ2.Write();
  W_etJJ.Write();
  W_mJJ.Write();
  W_mtJJ.Write();
  W_DetaJJ.Write();
  W_DphiJJ.Write();
  W_zeppJ1.Write();
  W_zeppJ2.Write();  
  W_zeppJJ.Write();
  W_tag_Dphi.Write();  
  
  // lepton
  lep_pt.Write();
  lep_zepp.Write();
  lep_W_Deta.Write();
  lep_W_Dphi.Write();
  lep_tag_Dphi.Write();
  lepMet_mt.Write();
  lepMet_W_Dphi.Write();
  
  // met
  met_et.Write();
  met_lep_Dphi.Write();  
  met_W_Dphi.Write();
  met_tag_Dphi.Write();
  
  // higgs
  lepMetW_pt.Write();
  lepMetW_mt.Write();
  lepMetW_tag_Dphi.Write();
  
  // pt tot
  lepMetWTag_pt.Write();
  
  
  
  cloneTree -> AutoSave();
  outputRootFile.Close();
  
  return 0;
}
