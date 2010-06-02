#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "hFactory.h"
#include "stdHisto.h"

#include <iomanip>

#include "TH1F.h"
#include "TObject.h"





int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> Collisions7TeVAnalysis::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
  
  std::string baseDir = gConfigParser -> readStringOption("Input::baseDir");
  std::string inputFileList = gConfigParser -> readStringOption("Input::inputFileList");
  std::string jetAlgorithm  = gConfigParser -> readStringOption("Input::jetAlgorithm");
  std::string jetType       = gConfigParser -> readStringOption("Input::jetType");
  
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  
  int entryMAX = gConfigParser -> readIntOption("Options::entryMAX");
  int entryMODULO = gConfigParser -> readIntOption("Options::entryMODULO");
  float mH = gConfigParser -> readFloatOption("Options::mH");
  float crossSection = gConfigParser -> readFloatOption("Options::crossSection");
  
  int lepNMIN = gConfigParser -> readIntOption("Cuts::lepNMIN");
  float lepPtMIN = gConfigParser -> readFloatOption("Cuts::lepPtMIN");
  float lepPtMAX = gConfigParser -> readFloatOption("Cuts::lepPtMAX");
  float lepTkIsoOverPtMAX = gConfigParser -> readFloatOption("Cuts::lepTkIsoOverPtMAX");
  float lepTipSigMAX = gConfigParser -> readFloatOption("Cuts::lepTipSigMAX");
  
  float metEtMIN = gConfigParser -> readFloatOption("Cuts::metEtMIN");
  float metEtMAX = gConfigParser -> readFloatOption("Cuts::metEtMAX");
  
  int jetNMIN = gConfigParser -> readIntOption("Cuts::jetNMIN");
  int jetNMAX = gConfigParser -> readIntOption("Cuts::jetNMAX");
  float jetEtMIN = gConfigParser -> readFloatOption("Cuts::jetEtMIN");
  
  std::map<int,int> totalEvents = GetTotalEvents("VBFPtMinLeptonCountFilterAll/totalEvents", inputFileList.c_str());  

  std::string VBFPreselectionName = "VBFPtMinLeptonCountFilterAll/passedEvents"; 
  std::map<int,int> VBFPreselectionEvents = GetTotalEvents(VBFPreselectionName.c_str(), inputFileList.c_str());
  
  
  
  
  
  
  // Open tree
  std::string treeName = "VBFNtuple"+jetAlgorithm+"/SimpleNtple";
  TChain* chain = new TChain(treeName.c_str());
  if(!FillChain(*chain, inputFileList.c_str())) return 1;
  treeReader reader((TTree*)(chain));  
  
  
  
  // define histograms
  int nStep = 6;
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
  std::string outputRootFullFileName = outputRootFilePath+outputRootFileName+"_"+jetAlgorithm+".root";
  
  // histograms
  hFactory* histograms = new hFactory(outputRootFullFileName);
  
  histograms -> add_h1("eleMet_mt", "", 3000, 0., 3000., nStep);
  histograms -> add_h1("muMet_mt",  "", 3000, 0., 3000., nStep);
  histograms -> add_h1("lepMet_mt", "", 3000, 0., 3000., nStep);
  
  histograms -> add_h1("lepJet_DR", "", 1000, 0., 10., nStep);
  
  
  // stdHistograms
  stdHisto* stdHistograms = new stdHisto(nStep, outputRootFullFileName);
  
  stdHistograms -> Add1("ele", nStep);
  stdHistograms -> Add1("mu",  nStep);
  stdHistograms -> Add1("lep", nStep);  
  
  stdHistograms -> Add1("met", nStep);
  
  stdHistograms -> Add1("jet", nStep);  
  stdHistograms -> Add2("WJJ", nStep); 
  stdHistograms -> Add2("tagJJ", nStep);
  
  
  
  
  
  
  
  
  
  
  
  
  //*********************
  // LOOP OVER THE EVENTS
  
  std::cout << ">>>>> Collisions7TeVAnalysis::Read " << chain -> GetEntries() << " entries" << std::endl;  
  for(int entry = 0 ; entry < chain -> GetEntries() ; ++entry)
  {
    reader.GetEntry(entry);
    if((entry%entryMODULO) == 0) std::cout << ">>>>> Collisions7TeVAnalysis::GetEntry " << entry << std::endl;   
    if(entry == entryMAX) break;
    
    
    
    //**************
    // SELECT LEPTON
    
    std::vector<ROOT::Math::XYZTVector> electrons;
    std::vector<ROOT::Math::XYZTVector> muons;
    std::vector<ROOT::Math::XYZTVector> leptons;
    std::vector<std::string> leptonFlavours;
    std::vector<float> leptonCharges;
    std::vector<float> leptons_tkIso;
    std::vector<float> leptons_lipSig;
    std::vector<float> leptons_tipSig;
    std::vector<float> leptons_3DipSig;
        
    for(unsigned int eleIt = 0; eleIt < (reader.Get4V("electrons")->size()); ++eleIt)
    {
      if( reader.Get4V("electrons")->at(eleIt).pt() < lepPtMIN ) continue;
      if( reader.Get4V("electrons")->at(eleIt).pt() > lepPtMAX ) continue;
      if( (reader.GetFloat("electrons_tkIso")->at(eleIt)) / reader.Get4V("electrons")->at(eleIt).pt() > lepTkIsoOverPtMAX ) continue;
      
      electrons.push_back( reader.Get4V("electrons")->at(eleIt) );
      leptons.push_back( reader.Get4V("electrons")->at(eleIt) );      
      leptonFlavours.push_back("electron");
      leptonCharges.push_back(reader.GetFloat("electrons_charge")->at(eleIt));
      leptons_tkIso.push_back(reader.GetFloat("electrons_tkIso")->at(eleIt));
      leptons_lipSig.push_back(reader.GetFloat("electrons_lipSignificance")->at(eleIt));
      leptons_tipSig.push_back(reader.GetFloat("electrons_tipSignificance")->at(eleIt));
      leptons_3DipSig.push_back(reader.GetFloat("electrons_3DipSignificance")->at(eleIt));
    }
    
    for(unsigned int muIt = 0; muIt < (reader.Get4V("muons")->size()); ++muIt)
    {
      if( reader.Get4V("muons")->at(muIt).pt() < lepPtMIN ) continue;
      if( reader.Get4V("muons")->at(muIt).pt() > lepPtMAX ) continue;
      if( (reader.GetFloat("muons_tkIsoR03")->at(muIt)) / reader.Get4V("muons")->at(muIt).pt() > lepTkIsoOverPtMAX ) continue;
      
      muons.push_back( reader.Get4V("muons")->at(muIt) );
      leptons.push_back( reader.Get4V("muons")->at(muIt) );      
      leptonFlavours.push_back("muon");
      leptonCharges.push_back(reader.GetFloat("muons_charge")->at(muIt));
      leptons_tkIso.push_back(reader.GetFloat("muons_tkIsoR03")->at(muIt));
      leptons_lipSig.push_back(reader.GetFloat("muons_lipSignificance")->at(muIt));
      leptons_tipSig.push_back(reader.GetFloat("muons_tipSignificance")->at(muIt));
      leptons_3DipSig.push_back(reader.GetFloat("muons_3DipSignificance")->at(muIt));
    }
    
    if( (int)(leptons.size()) < lepNMIN ) continue;
    
    
    
    
    
    
    //***********
    // SELECT MET
    
    ROOT::Math::XYZTVector met;
    if(jetType == "Calo")    
      met = reader.Get4V("met")->at(0);
    if(jetType == "PF")    
      met = reader.Get4V("PFMet")->at(0);
    
    
    
    
    
    
    //************
    // SELECT JETS
    
    std::vector<ROOT::Math::XYZTVector> jets;
    std::vector<ROOT::Math::XYZTVector> electrons_jetCleaning;
    std::vector<ROOT::Math::XYZTVector> muons_jetCleaning;
    std::vector<float> jets_bTag;
    
    // build the collection of leptons for jet cleaning
    //for(unsigned int eleIt = 0; eleIt < (reader.Get4V("electrons")->size()); ++eleIt)
    //{
    //  if( reader.Get4V("electrons")->at(eleIt).pt() < 10. ) continue;
    //  if( (reader.GetFloat("electrons_tkIso")->at(eleIt)) / reader.Get4V("electrons")->at(eleIt).pt() > 0.5 ) continue;
    //  if( (reader.GetFloat("electrons_IdTight")->at(eleIt)) < 1. ) continue;
    //  
    //  electrons_jetCleaning.push_back( reader.Get4V("electrons")->at(eleIt) );
    //}
    
    //for(unsigned int muIt = 0; muIt < (reader.Get4V("muons")->size()); ++muIt)
    //{
    //  if( reader.Get4V("muons")->at(muIt).pt() < 10. ) continue;
    //  if( (reader.GetFloat("muons_tkIsoR03")->at(muIt)) / reader.Get4V("muons")->at(muIt).pt() > 0.5 ) continue;
    //  
    //  muons_jetCleaning.push_back( reader.Get4V("muons")->at(muIt) );
    //}
    
    // jets
    for(unsigned int jetIt = 0; jetIt < (reader.Get4V("jets")->size()); ++jetIt)
    {
      ROOT::Math::XYZTVector jet = reader.Get4V("jets")->at(jetIt);
      if( jet.Et() < jetEtMIN ) continue;
      
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
    
    if( (int)(jets.size()) < jetNMIN ) continue;
    if( (int)(jets.size()) > jetNMAX ) continue;
    
    
    
    
    
    
    
    
    
    
    
    
    //*********************
    // STEP 2 - Lepton cuts
    step = 2;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "lepton cuts";
    
    // fill distributions
    stepEvents[step] += 1;
    
    stdHistograms -> Fill1(electrons, "ele", step);
    stdHistograms -> Fill1(muons,     "mu",  step);
    stdHistograms -> Fill1(leptons,   "lep", step);
    
    stdHistograms -> Fill1(met, "met", step);
    
    
    
    
    
    
    //*********************
    // STEP 3 - 1! Lepton
    step = 3;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "1! lepton";
    
    if(leptons.size() > 1) continue;
    
    ROOT::Math::XYZTVector lepton = leptons.at(0);
    float lepMet_mt = sqrt( lepton.mass()*lepton.mass() + 2.*lepton.pt()*met.pt()*(1-cos(deltaPhi(lepton.phi(), met.phi()))));
    
    // fill distributions
    stepEvents[step] += 1;
    
    stdHistograms -> Fill1(electrons, "ele", step);
    stdHistograms -> Fill1(muons,     "mu",  step);
    stdHistograms -> Fill1(leptons,   "lep", step);
    
    stdHistograms -> Fill1(met, "met", step);
    
    stdHistograms -> Fill1(jets, "jet", step);
    
    if(leptonFlavours.at(0) == "electron")
      histograms -> Fill("eleMet_mt", step, lepMet_mt);
    if(leptonFlavours.at(0) == "muon")
      histograms -> Fill("muMet_mt", step, lepMet_mt);    
    histograms -> Fill("lepMet_mt", step, lepMet_mt);
    
    for(unsigned int i = 0; i < jets.size(); ++i)
    {
      float lepJet_DR = deltaR(lepton.eta(), lepton.phi(), jets.at(i).eta(), jets.at(i).phi());
      histograms -> Fill("lepJet_DR", step, lepJet_DR);
    }
    
    
    
    
    
    
    //*******************
    // STEP 4 - >= 2 jets
    step = 4;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = ">= 2 jets";
    
    if(jets.size() < 2) continue;
    
    
    
    // select jets
    std::vector<int> selectIt_tag;
    selectIt_tag.push_back(-1);
    selectIt_tag.push_back(-1);
    SelectTagJets(selectIt_tag, jets, jetEtMIN, 2., 200.);
    std::sort(selectIt_tag.begin(), selectIt_tag.end());
    
    if( (selectIt_tag.at(0) != -1) || (selectIt_tag.at(1) != -1) )
    {
      ROOT::Math::XYZTVector jet1_tag = jets.at(selectIt_tag.at(0));
      ROOT::Math::XYZTVector jet2_tag = jets.at(selectIt_tag.at(1));        
      float jet1_tag_bTag = jets_bTag.at(selectIt_tag.at(0));
      float jet2_tag_bTag = jets_bTag.at(selectIt_tag.at(1));        
      ROOT::Math::XYZTVector jet12_tag = jet1_tag + jet2_tag;
      
      stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);      
    }
    
    
    
    std::vector<int> selectIt_W;
    selectIt_W.push_back(-1);
    selectIt_W.push_back(-1);
    SelectWJets(selectIt_W, jets, jetEtMIN, 2., 200.);
    std::sort(selectIt_W.begin(), selectIt_W.end());
    
    if( (selectIt_W.at(0) != -1) || (selectIt_W.at(1) != -1) )
    {
      ROOT::Math::XYZTVector jet1_W = jets.at(selectIt_W.at(0));
      ROOT::Math::XYZTVector jet2_W = jets.at(selectIt_W.at(1));        
      float jet1_W_bTag = jets_bTag.at(selectIt_W.at(0));
      float jet2_W_bTag = jets_bTag.at(selectIt_W.at(1));        
      ROOT::Math::XYZTVector jet12_W = jet1_W + jet2_W;
      
      stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step);      
    }
    
    
    // fill distributions
    stepEvents[step] += 1;
    
    stdHistograms -> Fill1(electrons, "ele", step);
    stdHistograms -> Fill1(muons,     "mu",  step);
    stdHistograms -> Fill1(leptons,   "lep", step);
    
    stdHistograms -> Fill1(met, "met", step);
    
    stdHistograms -> Fill1(jets, "jet", step);
        
    if(leptonFlavours.at(0) == "electron")
      histograms -> Fill("eleMet_mt", step, lepMet_mt);
    if(leptonFlavours.at(0) == "muon")
      histograms -> Fill("muMet_mt", step, lepMet_mt);    
    histograms -> Fill("lepMet_mt", step, lepMet_mt);
    
    for(unsigned int i = 0; i < jets.size(); ++i)
    {
      float lepJet_DR = deltaR(lepton.eta(), lepton.phi(), jets.at(i).eta(), jets.at(i).phi());
      histograms -> Fill("lepJet_DR", step, lepJet_DR);
    }
    
    
    
    
    
    
    //*******************
    // STEP 5 - >= 4 jets
    step = 5;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = ">= 4 jets";
    
    if(jets.size() < 4) continue;
    
    
    
    // select jets
    selectIt_tag.clear();
    selectIt_tag.push_back(-1);
    selectIt_tag.push_back(-1);
    SelectTagJets(selectIt_tag, jets, jetEtMIN, 2., 200.);
    std::sort(selectIt_tag.begin(), selectIt_tag.end());
    
    if( (selectIt_tag.at(0) != -1) || (selectIt_tag.at(1) != -1) )
    {
      ROOT::Math::XYZTVector jet1_tag = jets.at(selectIt_tag.at(0));
      ROOT::Math::XYZTVector jet2_tag = jets.at(selectIt_tag.at(1));        
      float jet1_tag_bTag = jets_bTag.at(selectIt_tag.at(0));
      float jet2_tag_bTag = jets_bTag.at(selectIt_tag.at(1));        
      ROOT::Math::XYZTVector jet12_tag = jet1_tag + jet2_tag;
      
      stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);      
    }
    
    
    
    selectIt_W.clear();
    std::vector<int> blacklistIt_tag;    
    selectIt_W.push_back(-1);
    selectIt_W.push_back(-1);
    blacklistIt_tag.push_back(selectIt_tag.at(0));
    blacklistIt_tag.push_back(selectIt_tag.at(1));
    SelectWJets(selectIt_W, jets, jetEtMIN, 2., 200., &blacklistIt_tag);
    std::sort(selectIt_W.begin(), selectIt_W.end());
    
    if( (selectIt_W.at(0) != -1) || (selectIt_W.at(1) != -1) )
    {
      ROOT::Math::XYZTVector jet1_W = jets.at(selectIt_W.at(0));
      ROOT::Math::XYZTVector jet2_W = jets.at(selectIt_W.at(1));        
      float jet1_W_bTag = jets_bTag.at(selectIt_W.at(0));
      float jet2_W_bTag = jets_bTag.at(selectIt_W.at(1));        
      ROOT::Math::XYZTVector jet12_W = jet1_W + jet2_W;
      
      stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step);      
    }
    
    
    // fill distributions
    stepEvents[step] += 1;
    
    stdHistograms -> Fill1(electrons, "ele", step);
    stdHistograms -> Fill1(muons,     "mu",  step);
    stdHistograms -> Fill1(leptons,   "lep", step);
    
    stdHistograms -> Fill1(met, "met", step);
    
    stdHistograms -> Fill1(jets, "jet", step);
        
    if(leptonFlavours.at(0) == "electron")
      histograms -> Fill("eleMet_mt", step, lepMet_mt);
    if(leptonFlavours.at(0) == "muon")
      histograms -> Fill("muMet_mt", step, lepMet_mt);    
    histograms -> Fill("lepMet_mt", step, lepMet_mt);
    
    for(unsigned int i = 0; i < jets.size(); ++i)
    {
      float lepJet_DR = deltaR(lepton.eta(), lepton.phi(), jets.at(i).eta(), jets.at(i).phi());
      histograms -> Fill("lepJet_DR", step, lepJet_DR);
    }
    
    
  } // loop over the events
  
  
  
  
  
  
  // save event histogram
  TFile* outputRootFile = new TFile(outputRootFullFileName.c_str(), "RECREATE");
  outputRootFile -> cd();
  for(step = 0; step < nStep; ++step)
  {
    events -> SetBinContent(step+1, stepEvents[step]);
    events -> GetXaxis() -> SetBinLabel(step+1, stepName[step].c_str());
  }
  
  events -> Write(); 
  outputRootFile -> Close(); 
  
  delete histograms;
  delete stdHistograms;
  
  
  return 0;
}
