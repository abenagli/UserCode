#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"

#include <iomanip>






int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there                                                                                                                               
  if(argc != 2)
  {
    std::cerr << ">>>>> VBFValidation::usage: " << argv[0] << " configFileName" << std::endl ;
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
  
  std::map<int,int> totalEvents = GetTotalEvents("MCDecayModeFilterAll/totalEvents", inputFileList.c_str());  

  std::string VBFPreselectionName = "VBFEtMinCaloJetCountFilter" + jetAlgorithm+"/passedEvents"; 
  std::map<int,int> VBFPreselectionEvents = GetTotalEvents(VBFPreselectionName.c_str(), inputFileList.c_str());  
  
  
  
  
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
  stepEvents[step] = totalEvents[1];
  stepName[step] = "total events";

  step = 1;
  stepEvents[step] = VBFPreselectionEvents[1];
  stepName[step] = "VBFPreselection";
  
  
  
  // define histograms
  
  // electrons
  TH1F ele_n  ("ele_n",   "",  10,  0.,  10.);
  TH1F ele_pt ("ele_pt",  "", 500,  0., 500.);
  TH1F ele_eta("ele_eta", "", 500, -5.,   5.);
  TH1F ele_phi("ele_phi", "", 500,  0.,   5.);
  
  // muons
  TH1F mu_n  ("mu_n",   "",  10,  0.,  10.);
  TH1F mu_pt ("mu_pt",  "", 500,  0., 500.);
  TH1F mu_eta("mu_eta", "", 500, -5.,   5.);
  TH1F mu_phi("mu_phi", "", 500,  0.,   5.);
  
  // leptons
  TH1F lep_n  ("lep_n",   "",  10,  0.,  10.);
  TH1F lep_pt ("lep_pt",  "", 500,  0., 500.);
  TH1F lep_eta("lep_eta", "", 500, -5.,   5.);
  TH1F lep_phi("lep_phi", "", 500,  0.,   5.);
  
  
  
  // met
  TH1F met_et("met_et", "", 500, 0., 500.);
  
  
  
  // jets
  TH1F jet_n ("jet_n",    "",   10,  0.,  10.);
  TH1F jet_et ("jet_et",  "",  500,  0., 500.);
  TH1F jet_eta("jet_eta", "",  500, -5.,   5.);
  TH1F jet_phi("jet_phi", "",  500,  0.,   5.);
  
  
  
  
  
  
  // Loop over events
  std::cout << ">>>>> VBFValidation::Read " << chain -> GetEntries() << " entries" << std::endl;  
  for(int entry = 0 ; entry < chain -> GetEntries() ; ++entry)
  {
    reader.GetEntry(entry);
    if((entry%entryMODULO) == 0) std::cout << ">>>>> VBFValidation::GetEntry " << entry << std::endl;   
    if(entry == entryMAX) break;
    
    
    
    
    
    
    //************
    // SELECT JETS
    
    std::vector<ROOT::Math::XYZTVector> jets;
    std::vector<ROOT::Math::XYZTVector> electrons_jetCleaning;
    std::vector<ROOT::Math::XYZTVector> muons_jetCleaning;
    std::vector<float> jets_bTag;
    
    // build the collection of leptons for jet cleaning
    for(unsigned int eleIt = 0; eleIt < (reader.Get4V("electrons")->size()); ++eleIt)
    {
      if( reader.Get4V("electrons")->at(eleIt).pt() < 10. ) continue;
      if( (reader.GetFloat("electrons_tkIso")->at(eleIt)) / reader.Get4V("electrons")->at(eleIt).pt() > 0.5 ) continue;
      if( (reader.GetFloat("electrons_IdTight")->at(eleIt)) < 1. ) continue;
      
      electrons_jetCleaning.push_back( reader.Get4V("electrons")->at(eleIt) );
    }
    
    for(unsigned int muIt = 0; muIt < (reader.Get4V("muons")->size()); ++muIt)
    {
      if( reader.Get4V("muons")->at(muIt).pt() < 10. ) continue;
      if( (reader.GetFloat("muons_tkIsoR03")->at(muIt)) / reader.Get4V("muons")->at(muIt).pt() > 0.5 ) continue;
      
      muons_jetCleaning.push_back( reader.Get4V("muons")->at(muIt) );
    }
    
    // jets
    for(unsigned int jetIt = 0; jetIt < (reader.Get4V("jets")->size()); ++jetIt)
    {
      ROOT::Math::XYZTVector jet = reader.Get4V("jets")->at(jetIt);
      if( jet.Et() < 30. ) continue;
      
      // clean jets
      bool skipJet = false;
      for(unsigned int eleIt = 0; eleIt < electrons_jetCleaning.size(); ++eleIt)
      {
        ROOT::Math::XYZTVector ele = electrons_jetCleaning.at(eleIt);
        if( deltaR(jet.eta(), jet.phi(), ele.eta(), ele.phi()) < 0.3 )
          skipJet = true;
      }
      for(unsigned int muIt = 0; muIt < muons_jetCleaning.size(); ++muIt)
      {
        ROOT::Math::XYZTVector mu = muons_jetCleaning.at(muIt);
        if( deltaR(jet.eta(), jet.phi(), mu.eta(), mu.phi()) < 0.3 )
          skipJet = true;
      }
      
      if(skipJet == true) continue;
      
      jets.push_back( reader.Get4V("jets")->at(jetIt) );
      jets_bTag.push_back( reader.GetFloat("jets_trackCountingHighEffBJetTags")->at(jetIt) );
    }
    
    
    
    
    
    
    //***************
    // SELECT LEPTONS
    
    std::vector<ROOT::Math::XYZTVector> electrons;
    std::vector<ROOT::Math::XYZTVector> muons;
    std::vector<ROOT::Math::XYZTVector> leptons;
    std::vector<float> leptonCharges;
    std::vector<std::string> leptonFlavours;    
    std::vector<float> leptons_tkIso;
    std::vector<float> leptons_emIso;
    std::vector<float> leptons_hadIso;
    std::vector<float> leptons_eleIdRobustTight;
    std::vector<float> leptons_lipSig;
    std::vector<float> leptons_tipSig;
    std::vector<float> leptons_3DipSig;
        
    for(unsigned int eleIt = 0; eleIt < (reader.Get4V("electrons")->size()); ++eleIt)
    {
      if( reader.Get4V("electrons")->at(eleIt).pt() < 10. ) continue;
      if( (reader.GetFloat("electrons_tkIso")->at(eleIt)) / reader.Get4V("electrons")->at(eleIt).pt() > 0.1 ) continue;
      if( (reader.GetFloat("electrons_hadIso03_1")->at(eleIt)) / reader.Get4V("electrons")->at(eleIt).pt() > 0.1 ) continue;
      if( (reader.GetFloat("electrons_IdRobustLoose")->at(eleIt)) < 1 ) continue;
      
      electrons.push_back( reader.Get4V("electrons")->at(eleIt) );
      leptons.push_back( reader.Get4V("electrons")->at(eleIt) );      
      leptonCharges.push_back( reader.GetFloat("electrons_charge")->at(eleIt) );
      leptonFlavours.push_back("electron");
      leptons_tkIso.push_back(reader.GetFloat("electrons_tkIso")->at(eleIt));
      leptons_emIso.push_back(reader.GetFloat("electrons_emIso03")->at(eleIt));
      leptons_hadIso.push_back(reader.GetFloat("electrons_hadIso03_1")->at(eleIt));    
      leptons_eleIdRobustTight.push_back(reader.GetFloat("electrons_IdRobustTight")->at(eleIt));
      leptons_lipSig.push_back(reader.GetFloat("electrons_lipSignificance")->at(eleIt));
      leptons_tipSig.push_back(reader.GetFloat("electrons_tipSignificance")->at(eleIt));
      leptons_3DipSig.push_back(reader.GetFloat("electrons_3DipSignificance")->at(eleIt));
    }
    
    for(unsigned int muIt = 0; muIt < (reader.Get4V("muons")->size()); ++muIt)
    {
      if( reader.Get4V("muons")->at(muIt).pt() < 10. ) continue;
      if( (reader.GetFloat("muons_emIsoR03")->at(muIt)) / reader.Get4V("muons")->at(muIt).pt() > 0.1 ) continue;
      if( (reader.GetFloat("muons_hadIsoR03")->at(muIt)) / reader.Get4V("muons")->at(muIt).pt() > 0.1 ) continue;
      
      muons.push_back( reader.Get4V("muons")->at(muIt) );
      leptons.push_back( reader.Get4V("muons")->at(muIt) );      
      leptonCharges.push_back( reader.GetFloat("muons_charge")->at(muIt) );
      leptonFlavours.push_back("muon");
      leptons_tkIso.push_back(reader.GetFloat("muons_tkIsoR03")->at(muIt));
      leptons_emIso.push_back(reader.GetFloat("muons_emIsoR03")->at(muIt));
      leptons_hadIso.push_back(reader.GetFloat("muons_hadIsoR03")->at(muIt));
      leptons_eleIdRobustTight.push_back(1.);
      leptons_lipSig.push_back(reader.GetFloat("muons_lipSignificance")->at(muIt));
      leptons_tipSig.push_back(reader.GetFloat("muons_tipSignificance")->at(muIt));
      leptons_3DipSig.push_back(reader.GetFloat("muons_3DipSignificance")->at(muIt));
    }
    
    
    
    
    
    
    //***********
    // SELECT MET
    
    ROOT::Math::XYZTVector met = reader.Get4V("met")->at(0);
    
    
    
    
    
    
    
    
    
    
    
    
    // fill distributions
    
    // electrons
    ele_n.Fill( electrons.size() );
    
    for(unsigned int i = 0; i < electrons.size(); ++i)
    {
      ele_pt.Fill ( electrons.at(i).pt()  );
      ele_eta.Fill( electrons.at(i).eta() );
      ele_phi.Fill( electrons.at(i).phi() );
    }
    
    
    // muons
    mu_n.Fill( muons.size() );
    
    for(unsigned int i = 0; i < muons.size(); ++i)
    {
      mu_pt.Fill ( muons.at(i).pt()  );
      mu_eta.Fill( muons.at(i).eta() );
      mu_phi.Fill( muons.at(i).phi() );
    }
    
    
    // leptons
    lep_n.Fill( leptons.size() );
    
    for(unsigned int i = 0; i < leptons.size(); ++i)
    {
      lep_pt.Fill ( leptons.at(i).pt()  );
      lep_eta.Fill( leptons.at(i).eta() );
      lep_phi.Fill( leptons.at(i).phi() );
    }
    
    
    
    
    // met
    met_et.Fill( met.Et() );
    
    
    
    
    // jets
    jet_n.Fill( jets.size() );
    
    for(unsigned int i = 0; i < jets.size(); ++i)
    {
      jet_et.Fill ( jets.at(i).Et()  );
      jet_eta.Fill( jets.at(i).eta() );
      jet_phi.Fill( jets.at(i).phi() );
    }
    
    
} // loop over the events
  
  
  
  
  
  for(step = 0; step < nStep; ++step)
  {
    events -> SetBinContent(step+1, stepEvents[step]);
    events -> GetXaxis() -> SetBinLabel(step+1, stepName[step].c_str());
  }
  
  events -> Write();
  
  
  
  // write histograms in root file
  
  ele_n.Write();
  ele_pt.Write();
  ele_eta.Write();
  ele_phi.Write();
  
  mu_n.Write();
  mu_pt.Write();
  mu_eta.Write();
  mu_phi.Write();
  
  lep_n.Write();
  lep_pt.Write();
  lep_eta.Write();
  lep_phi.Write();
  
  
  met_et.Write();
  
  
  jet_n.Write();
  jet_et.Write();
  jet_eta.Write();
  jet_phi.Write();
  
  
  
  cloneTree -> AutoSave();
  outputRootFile.Close();
  
  return 0;
}
