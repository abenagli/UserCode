#include "TTBarFullyLeptonicVariables.h"


void InitializeTTBarFullyLeptonicVariables(TTBarFullyLeptonicVariables& vars, const std::string& name, const int& nStep)
{
  //-----------
  // histograms
  //-----------
  
  vars.histograms = new hFactory(name);
  vars.histograms2 = new h2Factory(name);
  
  vars.histograms -> add_h1("ele_tkIso_EB",         "", 1000,  0.000,  0.500, nStep+1);
  vars.histograms -> add_h1("ele_emIso_EB",         "", 1000,  0.000,  0.500, nStep+1);
  vars.histograms -> add_h1("ele_hadIso_EB",        "", 1000,  0.000,  0.500, nStep+1);
  vars.histograms -> add_h1("ele_fbrem_EB",         "", 1000,  0.000,  1.000, nStep+1);
  vars.histograms -> add_h1("ele_HoverE_EB",        "", 1000,  0.000,  0.100, nStep+1);
  vars.histograms -> add_h1("ele_DphiIn_EB",        "", 1000, -0.100,  0.100, nStep+1);
  vars.histograms -> add_h1("ele_DetaIn_EB",        "", 1000, -0.015,  0.015, nStep+1);
  vars.histograms -> add_h1("ele_sigmaIetaIeta_EB", "", 1000,  0.000,  0.100, nStep+1);
  vars.histograms -> add_h1("ele_dxy_EB",           "", 1000, -0.100,  0.100, nStep+1);
  vars.histograms -> add_h1("ele_dz_EB",            "", 1000, -0.100,  0.100, nStep+1);
  vars.histograms -> add_h1("ele_tkIso_EE",         "", 1000,  0.000,  0.500, nStep+1);
  vars.histograms -> add_h1("ele_emIso_EE",         "", 4000,  0.000,  2.000, nStep+1);
  vars.histograms -> add_h1("ele_hadIso_EE",        "", 1000,  0.000,  0.500, nStep+1);
  vars.histograms -> add_h1("ele_fbrem_EE",         "", 1000,  0.000,  1.000, nStep+1);
  vars.histograms -> add_h1("ele_HoverE_EE",        "", 1000,  0.000,  0.100, nStep+1);
  vars.histograms -> add_h1("ele_DphiIn_EE",        "", 1000, -0.100,  0.100, nStep+1);
  vars.histograms -> add_h1("ele_DetaIn_EE",        "", 1000, -0.015,  0.015, nStep+1);
  vars.histograms -> add_h1("ele_sigmaIetaIeta_EE", "", 1000,  0.000,  0.100, nStep+1);
  vars.histograms -> add_h1("ele_dxy_EE",           "", 1000, -0.100,  0.100, nStep+1);
  vars.histograms -> add_h1("ele_dz_EE",            "", 1000, -0.100,  0.100, nStep+1);
  vars.histograms -> add_h1("ele_3DipSig_EB",       "", 2000, -50.0,   50.0,  nStep+1);
  vars.histograms -> add_h1("ele_3DipSig_EE",       "", 2000, -50.0,   50.0,  nStep+1);
  
  vars.histograms -> add_h1("mu_tkIso",   "", 1000,   0.0,  0.5, nStep+1);
  vars.histograms -> add_h1("mu_emIso",   "", 1000,   0.0,  0.5, nStep+1);
  vars.histograms -> add_h1("mu_hadIso",  "", 1000,   0.0,  0.5, nStep+1);
  vars.histograms -> add_h1("mu_dxy",     "", 1000,  -0.1,  0.1, nStep+1);
  vars.histograms -> add_h1("mu_dz",      "", 1000,  -1.0,  1.0, nStep+1);
  vars.histograms -> add_h1("mu_3DipSig", "", 2000, -50.0, 50.0, nStep+1);
  vars.histograms -> add_h1("mu_tracker",    "", 2, 0, 2, nStep+1);
  vars.histograms -> add_h1("mu_standalone", "", 2, 0, 2, nStep+1);
  vars.histograms -> add_h1("mu_global",     "", 2, 0, 2, nStep+1);
  vars.histograms -> add_h1("mu_normalizedChi2", "", 1000, 0., 10., nStep+1);
  vars.histograms -> add_h1("mu_numberOfValidTrackerHits", "", 100, 0, 100, nStep+1);
  vars.histograms -> add_h1("mu_numberOfValidMuonHits",    "", 100, 0, 100, nStep+1);
  
  vars.histograms -> add_h1("caloMetOverGenMet",  "", 1000, 0., 10., nStep+1);
  vars.histograms -> add_h1("type1MetOverGenMet", "", 1000, 0., 10., nStep+1);
  vars.histograms -> add_h1("pfMetOverGenMet",    "", 1000, 0., 10., nStep+1);  

  vars.histograms -> add_h1("jetEle_DR",            "", 5000,     0.,   10., nStep+1);  
  vars.histograms -> add_h1("jetMu_DR",             "", 5000,     0.,   10., nStep+1);
  vars.histograms -> add_h1("jet_emEnergyFraction", "",  1000,  -1.0,   1.0, nStep+1);
  vars.histograms -> add_h1("jet_etaEtaMoment",     "",  1000,   0.0,   0.1, nStep+1);
  vars.histograms -> add_h1("jet_phiPhiMoment",     "",  1000,   0.0,   0.1, nStep+1);
  vars.histograms -> add_h1("jet_etaPhiMoment",     "",  1000,  -0.1,   0.1, nStep+1);
  vars.histograms -> add_h1("jet_fHPD",             "",  1000,   0.0,   1.0, nStep+1);
  vars.histograms -> add_h1("jet_fRBX",             "",  1000,   0.0,   1.0, nStep+1);
  vars.histograms -> add_h1("jet_n90Hits",          "",   200,   0.0, 200.0, nStep+1);
  vars.histograms -> add_h1("jet_bTag1",            "", 2000,  -100.,  100., nStep+1);
  vars.histograms -> add_h1("jet_bTag2",            "", 2000,  -100.,  100., nStep+1);
  vars.histograms -> add_h1("jet_bTag3",            "", 2000,  -100.,  100., nStep+1);
  vars.histograms -> add_h1("jet_bTag4",            "", 2000,  -100.,  100., nStep+1);
  
  vars.histograms -> add_h1("tagJJ_max_bTag", "", 2000,  -100.,  100., nStep+1);
  vars.histograms -> add_h1("tagJJ_min_bTag", "", 2000,  -100.,  100., nStep+1);
  
  
  
  //--------------
  // stdHistograms
  //--------------
  
  vars.stdHistograms = new stdHisto(nStep+1, name);
  
  vars.stdHistograms -> Add1("electrons", nStep+1, false);
  vars.stdHistograms -> Add1("muons",     nStep+1, false);
  vars.stdHistograms -> Add2("leplep",    nStep+1, false);
  vars.stdHistograms -> Add1("met",       nStep+1, false);
  vars.stdHistograms -> Add1("jets",      nStep+1, false);
  vars.stdHistograms -> Add2("tagJJ",     nStep+1, false);
  vars.stdHistograms -> Add2("3rdJ",     nStep+1, false);
}






void ClearTTBarFullyLeptonicVariables(TTBarFullyLeptonicVariables& vars)
{
  vars.selectIt_ele.clear();
  vars.selectIt_mu.clear();
  vars.selectIt_lep.clear();
  vars.electrons.clear();
  vars.muons.clear();
  vars.leptons.clear();
  vars.leptonCharges.clear();
  vars.leptonFlavours.clear();    
  vars.leptons_tkIso.clear();
  vars.leptons_emIso.clear();
  vars.leptons_hadIso.clear();
  vars.leptons_dxy.clear();
  vars.leptons_dz.clear();
  vars.leptons_3DipSig.clear();
  vars.electrons_isEB.clear();
  vars.electrons_fbrem.clear();
  vars.electrons_HoverE.clear();
  vars.electrons_DphiIn.clear();
  vars.electrons_DetaIn.clear();
  vars.electrons_sigmaIetaIeta.clear();
  vars.muons_tracker.clear();
  vars.muons_standalone.clear();
  vars.muons_global.clear();
  vars.muons_normalizedChi2.clear();
  vars.muons_numberOfValidTrackerHits.clear();
  vars.muons_numberOfValidMuonHits.clear();
  
  vars.met = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.caloMet = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.type1Met = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.pfMet = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.genMet = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  
  vars.selectIt_tag.clear();
  vars.selectIt_3rd.clear();
  vars.jets.clear();
  vars.nJets = 0;
  vars.jets_bTag.clear();
  vars.jets_emEnergyFraction.clear();
  vars.jets_etaEtaMoment.clear();
  vars.jets_phiPhiMoment.clear();
  vars.jets_etaPhiMoment.clear();
  vars.jets_fHPD.clear();
  vars.jets_fRBX.clear();
  vars.jets_n90Hits.clear();
  vars.jets_etOrdered.clear();
  vars.jets_bTagOrdered.clear();
}






void DeleteTTBarFullyLeptonicVariables(TTBarFullyLeptonicVariables& vars)
{
  delete vars.histograms;
  delete vars.histograms2;
  delete vars.stdHistograms;
}






void FillLeptonHistograms(TTBarFullyLeptonicVariables& vars, const int& step)
{
  for(unsigned int lepIt = 0; lepIt < vars.selectIt_lep.size(); ++lepIt)
  {
    ROOT::Math::XYZTVector tempLep = vars.leptons.at(vars.selectIt_lep.at(lepIt));
        
    
    
    //----------
    // electrons
    if( vars.leptonFlavours.at(vars.selectIt_lep.at(lepIt)) == "electron" )
    {
      if( vars.electrons_isEB.at(vars.selectIt_ele.at(lepIt)) == 1 )
      {
        vars.histograms -> Fill("ele_tkIso_EB",  step, vars.leptons_tkIso.at(vars.selectIt_lep.at(lepIt))/tempLep.pt());        
        vars.histograms -> Fill("ele_emIso_EB",  step, vars.leptons_emIso.at(vars.selectIt_lep.at(lepIt))/tempLep.pt());
        vars.histograms -> Fill("ele_hadIso_EB", step, vars.leptons_hadIso.at(vars.selectIt_lep.at(lepIt))/tempLep.pt());
        vars.histograms -> Fill("ele_fbrem_EB",  step, vars.electrons_fbrem.at(vars.selectIt_ele.at(lepIt)));
        vars.histograms -> Fill("ele_HoverE_EB", step, vars.electrons_HoverE.at(vars.selectIt_ele.at(lepIt)));
        vars.histograms -> Fill("ele_DphiIn_EB", step, vars.electrons_DphiIn.at(vars.selectIt_ele.at(lepIt)));
        vars.histograms -> Fill("ele_DetaIn_EB", step, vars.electrons_DetaIn.at(vars.selectIt_ele.at(lepIt)));
        vars.histograms -> Fill("ele_sigmaIetaIeta_EB", step, vars.electrons_sigmaIetaIeta.at(vars.selectIt_ele.at(lepIt)));
        vars.histograms -> Fill("ele_dxy_EB",           step, vars.leptons_dxy.at(vars.selectIt_lep.at(lepIt)));
        vars.histograms -> Fill("ele_dz_EB",            step, vars.leptons_dz.at(vars.selectIt_lep.at(lepIt)));
        vars.histograms -> Fill("ele_3DipSig_EB",       step, vars.leptons_3DipSig.at(vars.selectIt_lep.at(lepIt)));
      }
      
      else
      {
        vars.histograms -> Fill("ele_tkIso_EE",  step, vars.leptons_tkIso.at(vars.selectIt_lep.at(lepIt))/tempLep.pt());        
        vars.histograms -> Fill("ele_emIso_EE",  step, vars.leptons_emIso.at(vars.selectIt_lep.at(lepIt))/tempLep.pt());
        vars.histograms -> Fill("ele_hadIso_EE", step, vars.leptons_hadIso.at(vars.selectIt_lep.at(lepIt))/tempLep.pt());
        vars.histograms -> Fill("ele_fbrem_EE",  step, vars.electrons_fbrem.at(vars.selectIt_ele.at(lepIt)));
        vars.histograms -> Fill("ele_HoverE_EE", step, vars.electrons_HoverE.at(vars.selectIt_ele.at(lepIt)));
        vars.histograms -> Fill("ele_DphiIn_EE", step, vars.electrons_DphiIn.at(vars.selectIt_ele.at(lepIt)));
        vars.histograms -> Fill("ele_DetaIn_EE", step, vars.electrons_DetaIn.at(vars.selectIt_ele.at(lepIt)));
        vars.histograms -> Fill("ele_sigmaIetaIeta_EE", step, vars.electrons_sigmaIetaIeta.at(vars.selectIt_ele.at(lepIt)));
        vars.histograms -> Fill("ele_dxy_EE",           step, vars.leptons_dxy.at(vars.selectIt_lep.at(lepIt)));
        vars.histograms -> Fill("ele_dz_EE",            step, vars.leptons_dz.at(vars.selectIt_lep.at(lepIt)));
        vars.histograms -> Fill("ele_3DipSig_EE",       step, vars.leptons_3DipSig.at(vars.selectIt_lep.at(lepIt)));
      }
    }
    
    
    
    //----------
    // muons
    
    if( vars.leptonFlavours.at(vars.selectIt_lep.at(lepIt)) == "muon" )
    {
      vars.histograms -> Fill("mu_tkIso",   step, vars.leptons_tkIso.at(vars.selectIt_lep.at(lepIt))/tempLep.pt());        
      vars.histograms -> Fill("mu_emIso",   step, vars.leptons_emIso.at(vars.selectIt_lep.at(lepIt))/tempLep.pt());
      vars.histograms -> Fill("mu_hadIso",  step, vars.leptons_hadIso.at(vars.selectIt_lep.at(lepIt))/tempLep.pt());
      vars.histograms -> Fill("mu_dxy",     step, vars.leptons_dxy.at(vars.selectIt_lep.at(lepIt)));
      vars.histograms -> Fill("mu_dz",      step, vars.leptons_dz.at(vars.selectIt_lep.at(lepIt)));
      vars.histograms -> Fill("mu_3DipSig", step, vars.leptons_3DipSig.at(vars.selectIt_lep.at(lepIt)));
      vars.histograms -> Fill("mu_tracker",    step, vars.muons_tracker.at(vars.selectIt_mu.at(lepIt)));    
      vars.histograms -> Fill("mu_standalone", step, vars.muons_standalone.at(vars.selectIt_mu.at(lepIt)));
      vars.histograms -> Fill("mu_global",     step, vars.muons_global.at(vars.selectIt_mu.at(lepIt)));    
      vars.histograms -> Fill("mu_normalizedChi2", step, vars.muons_normalizedChi2.at(vars.selectIt_mu.at(lepIt)));
      vars.histograms -> Fill("mu_numberOfValidTrackerHits", step, vars.muons_numberOfValidTrackerHits.at(vars.selectIt_mu.at(lepIt)));
      vars.histograms -> Fill("mu_numberOfValidMuonHits",    step, vars.muons_numberOfValidMuonHits.at(vars.selectIt_mu.at(lepIt)));
    }
  }
  
  vars.stdHistograms -> Fill1(vars.electrons, "electrons", step);
  vars.stdHistograms -> Fill1(vars.muons, "muons", step);
  vars.stdHistograms -> Fill2(vars.leptons.at(vars.selectIt_lep.at(0)), vars.leptons.at(vars.selectIt_lep.at(1)), "leplep", step);
}



void FillMetHistograms(TTBarFullyLeptonicVariables& vars, const int& step)
{
  vars.histograms -> Fill("caloMetOverGenMet",  step, vars.caloMet.Et()  / vars.genMet.Et());
  vars.histograms -> Fill("type1MetOverGenMet", step, vars.type1Met.Et() / vars.genMet.Et());
  vars.histograms -> Fill("pfMetOverGenMet",    step, vars.pfMet.Et()    / vars.genMet.Et());
  vars.stdHistograms -> Fill1(vars.met, "met", step);
}



void FillJetHistograms(TTBarFullyLeptonicVariables& vars, const int& step)
{
  for(unsigned int jetIt = 0; jetIt < vars.jets.size(); ++jetIt)
  {
    //float tempDR = deltaR(vars.lepton.eta(), vars.lepton.phi(), vars.jets.at(jetIt).eta(), vars.jets.at(jetIt).phi());

    vars.histograms -> Fill("jet_emEnergyFraction", step, vars.jets_emEnergyFraction.at(jetIt));
    vars.histograms -> Fill("jet_etaEtaMoment",     step, vars.jets_etaEtaMoment.at(jetIt));
    vars.histograms -> Fill("jet_phiPhiMoment",     step, vars.jets_phiPhiMoment.at(jetIt));
    vars.histograms -> Fill("jet_etaPhiMoment",     step, vars.jets_etaPhiMoment.at(jetIt));
    vars.histograms -> Fill("jet_fHPD",             step, vars.jets_fHPD.at(jetIt));
    vars.histograms -> Fill("jet_fRBX",             step, vars.jets_fRBX.at(jetIt));
    vars.histograms -> Fill("jet_n90Hits",          step, vars.jets_n90Hits.at(jetIt));

    //if(vars.leptonFlavours.at(vars.selectIt_lep) == "electron")
    //{
    //  vars.histograms -> Fill("jetEle_DR", step, tempDR);
    //  vars.histograms2 -> Fill("jetEle_emEnergyFraction_vs_DR", step, tempDR, vars.jets_emEnergyFraction.at(jetIt));
    //}
    //
    //if(vars.leptonFlavours.at(vars.selectIt_lep) == "muon")
    //{
    //  vars.histograms -> Fill("jetMu_DR", step, tempDR);
    //  vars.histograms2 -> Fill("jetMu_emEnergyFraction_vs_DR", step, tempDR, vars.jets_emEnergyFraction.at(jetIt));
    //}
    
    
    if( vars.jets_bTagOrdered.size() > 0)
      vars.histograms -> Fill("jet_bTag1", step, vars.jets_bTagOrdered.at(0));
    if( vars.jets_bTagOrdered.size() > 1)
      vars.histograms -> Fill("jet_bTag2", step, vars.jets_bTagOrdered.at(1));
    if( vars.jets_bTagOrdered.size() > 2)
      vars.histograms -> Fill("jet_bTag3", step, vars.jets_bTagOrdered.at(2));  
    if( vars.jets_bTagOrdered.size() > 3)
      vars.histograms -> Fill("jet_bTag4", step, vars.jets_bTagOrdered.at(3));
    
  }
  
  
  
  
  
  
  vars.stdHistograms -> Fill1(vars.jets, "jets", step);
  vars.stdHistograms -> Fill2(vars.jets.at(vars.selectIt_tag.at(0)), vars.jets.at(vars.selectIt_tag.at(1)), "tagJJ", step);
  
}






void Fill3rdJetHistograms(TTBarFullyLeptonicVariables& vars, const int& step)
{
  vars.stdHistograms -> Fill1(vars.jets.at(vars.selectIt_3rd.at(0)), "3rdJ", step);
}

