#include "HiggsAnalysis/VBFHiggsToVV/interface/MCDumperVBF.h"






// -----------------------------------------------------------------
// ctor
// -----------------------------------------------------------------

MCDumperVBF::MCDumperVBF(edm::Handle<reco::GenParticleCollection>& genParticles,
                   const int& eventType,
                   const bool& verbosity):
 verbosity_p(verbosity),
 eventType_p(eventType),
 isValid_p(true),
 mcHDecayMode_p(""),
 mcV1DecayMode_p(""),
 mcV2DecayMode_p(""),
 mcH_p(0),
 mcQ1_tag_p(0),
 mcQ2_tag_p(0),
 mcV1_p(0),
 mcV2_p(0),
 mcF1_fromV1_p(0),
 mcF2_fromV1_p(0),
 mcF1_fromV2_p(0),
 mcF2_fromV2_p(0)
 {
  if(verbosity_p)
    std::cerr << "*** MCDumperVBF::ctor ***" << std::endl;

  Analyze(genParticles);
}






// -----------------------------------------------------------------
// dtor
// -----------------------------------------------------------------

MCDumperVBF::~MCDumperVBF()
{
  if(verbosity_p)
    std::cerr << "*** MCDumperVBF::dtor ***" << std::endl;
}






// -----------------------------------------------------------------
// Analyze
// -----------------------------------------------------------------

void MCDumperVBF::Analyze(edm::Handle<reco::GenParticleCollection>& genParticles)
{
  if(verbosity_p)
    std::cerr << "*** MCDumperVBF::Analyze ***" << std::endl;
  
  
  
  // loop over gen particles
  int counter = 0;
  for(reco::GenParticleCollection::const_iterator p = genParticles -> begin();
      p != genParticles -> end(); ++p)
  {
    const reco::Candidate* pCurrent = &(*p);
    const reco::Candidate* pMother = 0;
    if(pCurrent -> mother())
      pMother = pCurrent -> mother();
    
    int pdgId  = p -> pdgId();
    int status = p -> status();
    int charge = p -> charge();
    int motherPdgId = 0;
    if(pCurrent -> mother())
      motherPdgId = pMother -> pdgId();
    
        
    // charged
    if( (abs(charge) != 0) && (status == 1) )
      mcCharged_p.push_back(pCurrent);

    // electrons
    if( (abs(pdgId) == 11) && (status == 1) )
      mcE_p.push_back(pCurrent);
    
    // muons
    if( (abs(pdgId) == 13) && (status == 1) )
      mcMu_p.push_back(pCurrent);
    
    // taus and tauJ
    if( (abs(pdgId) == 16) && (status == 1) && (abs(motherPdgId) == 15))
    {
      mcTau_p.push_back(pMother);
      
      std::vector<const reco::Candidate*> tauJBuffer;
      std::vector<const reco::Candidate*> nuBuffer;
      FindTauDecayParticles(tauJBuffer, nuBuffer, pMother, verbosity_p);
      
      mcTauJ_p.push_back(tauJBuffer);
    }
    
    // higgs
    if( (pdgId == 25) && (status == 3) )
    {
      mcH_p = pCurrent;
    }
    
    // tag quarks
    if(counter == 6)
      mcQ1_tag_p = pCurrent;

    if(counter == 7)
      mcQ2_tag_p = pCurrent;

    //if(abs(pdgId) == 13)
    //if(counter < 50)
    //{
    //  std::cout << "n: " << counter;
    //  PrintParticleInfo(pCurrent);
    //}
    
    ++counter;
    
  } // loop over gen particles
  
  
  
  // return if background
  if(eventType_p != 0) return;
  
  
  
  
  
  
  // check if it is signal
  if(!mcH_p)
  {
    std::cerr << ">>> MCDumperVBF::Analyze::Warning: no H in the event" << std::endl;
    
    isValid_p = false;
    return;
  }
  
  //if(verbosity_p)
  //PrintDecayTree(mcH_p);
  // check if it is signal
  
  
  
  
  
    
  // find vector bosons
  std::vector<const reco::Candidate*> VBuffer;
  FindDaughterParticles(&mcH_p, verbosity_p, 1, &VBuffer);
  
  
  if(VBuffer.size() != 2)
  {
     std::cerr << "MCDumperVBF::Analyze::Warning: not a H -> VV decay" << std::endl;

     isValid_p = false;
     return;
  }
  
  mcV1_p = VBuffer.at(0);
  mcV2_p = VBuffer.at(1);
  
  if(mcV2_p -> pt() > mcV1_p -> pt())
  {
    mcV1_p = VBuffer.at(1);
    mcV2_p = VBuffer.at(0);
  }
  
  GetHDecayMode();
  // find vector bosons
  
  
  
  
  
  
  // find fermions from vector boson decay
  std::vector<const reco::Candidate*> fFromV1Buffer;
  FindDaughterParticles(&mcV1_p, verbosity_p, 1, &fFromV1Buffer);
  
  if(fFromV1Buffer.size() != 2)
  {
     std::cerr << "MCDumperVBF::Analyze::Warning: not a V1 -> ff decay" << std::endl;

     isValid_p = false;
     return;
  }
  
  mcF1_fromV1_p = fFromV1Buffer.at(0);
  mcF2_fromV1_p = fFromV1Buffer.at(1);
  
  // If leptons, see if there is a photon emission
  if(abs(mcF1_fromV1_p -> pdgId()) >= 11)
    FindDaughterParticles(&mcF1_fromV1_p, verbosity_p, 1);
  if(abs(mcF2_fromV1_p -> pdgId()) >= 11)
    FindDaughterParticles(&mcF2_fromV1_p, verbosity_p, 1);
  
  if(mcF2_fromV1_p -> pt() > mcF1_fromV1_p -> pt())
  {
    const reco::Candidate* buffer = mcF1_fromV1_p;
    mcF1_fromV1_p = mcF2_fromV1_p;
    mcF2_fromV1_p = buffer;
  }
  
  
  
  
  
  
  std::vector<const reco::Candidate*> fFromV2Buffer;
  FindDaughterParticles(&mcV2_p, verbosity_p, 1, &fFromV2Buffer);

  if(fFromV2Buffer.size() != 2)
  {
     std::cerr << "MCDumperVBF::Analyze::Warning: not a V2 -> ff decay" << std::endl;

     isValid_p = false;
     return;
  }
  
  mcF1_fromV2_p    = fFromV2Buffer.at(0);
  mcF2_fromV2_p    = fFromV2Buffer.at(1);
  
  // If leptons, see if there is a photon emission
  if(abs(mcF1_fromV2_p -> pdgId()) >= 11)
    FindDaughterParticles(&mcF1_fromV2_p, verbosity_p, 1);
  if(abs(mcF2_fromV2_p -> pdgId()) >= 11)
    FindDaughterParticles(&mcF2_fromV2_p, verbosity_p, 1);
  
  if(mcF2_fromV2_p -> pt() > mcF1_fromV2_p -> pt())
  {
    const reco::Candidate* buffer = mcF1_fromV2_p;
    mcF1_fromV2_p = mcF2_fromV2_p;
    mcF2_fromV2_p = buffer;
  }
  
  
  
  
  
 
  // sort vector bosons
  if( ( (abs(mcF1_fromV1_p -> pdgId()) < 11)  && (abs(mcF2_fromV1_p -> pdgId()) < 11) ) &&  
      ( (abs(mcF1_fromV2_p -> pdgId()) >= 11) && (abs(mcF2_fromV2_p -> pdgId()) >= 11) ) )  
  {
    const reco::Candidate* buffer;
    
    buffer = mcV1_p;
    mcV1_p = mcV2_p;
    mcV2_p = buffer;

    buffer = mcF1_fromV1_p;
    mcF1_fromV1_p = mcF1_fromV2_p;
    mcF1_fromV2_p = buffer;
    
    buffer = mcF2_fromV1_p;
    mcF2_fromV1_p = mcF2_fromV2_p;
    mcF2_fromV2_p = buffer;
  }
  
  GetVDecayMode(1);
  GetVDecayMode(2);
  
  
  
  
  
  
  // Print event summary
  if(verbosity_p)
    PrintEventInfo();
  
  
  
  return;
}






// -----------------------------------------------------------------
// Print Event Info
// -----------------------------------------------------------------

void MCDumperVBF::PrintEventInfo()
{
  std::cout << "\n>>> MCDumperVBF::PrintHInfo \n" << std::endl;
  
  
  
  std::cout << "Higgs:           H  -> " << mcHDecayMode_p << std::endl;
  if(mcH_p)
    PrintParticleInfo(mcH_p);
  
  std::cout << std::endl;
  
  
  
  std::cout << "Vector bosons:   V1 -> " << mcV1DecayMode_p << std::endl;
  if(mcV1_p)
    PrintParticleInfo(mcV1_p);

  std::cout << "Vector bosons:   V2 -> " << mcV2DecayMode_p << std::endl;  
  if(mcV2_p)
    PrintParticleInfo(mcV2_p);
  
  std::cout << std::endl;
  
  
  
  std::cout << "Fermions from vector bosons: " << std::endl;
  if(mcF1_fromV1_p)
    PrintParticleInfo(mcF1_fromV1_p);
  if(mcF2_fromV1_p)
    PrintParticleInfo(mcF2_fromV1_p);
  if(mcF1_fromV2_p)
    PrintParticleInfo(mcF1_fromV2_p);
  if(mcF2_fromV2_p)
    PrintParticleInfo(mcF2_fromV2_p);
  
  std::cout << std::endl;
}







// -----------------------------------------------------------------
// Get Higgs decay mode
// -----------------------------------------------------------------

void MCDumperVBF::GetHDecayMode()
{
  if( (mcV1_p -> pdgId() == 23) && (mcV2_p -> pdgId() == 23) )
    mcHDecayMode_p = "Z_Z";
  
  if( (mcV1_p -> pdgId() == 24) && (mcV2_p -> pdgId() == -24) )
    mcHDecayMode_p = "W_W";
  
  if( (mcV1_p -> pdgId() == -24) && (mcV2_p -> pdgId() == 24) )
    mcHDecayMode_p = "W_W";
}






// -----------------------------------------------------------------
// Get Vector Boson decay mode
// -----------------------------------------------------------------

void MCDumperVBF::GetVDecayMode(const int& Vindex)
{
  std::vector<const reco::Candidate*> buffer;
  std::string stringBuffer;
  if(Vindex == 1)
  {
    buffer.push_back(mcF1_fromV1_p);
    buffer.push_back(mcF2_fromV1_p);
  }
  if(Vindex == 2)
  {
    buffer.push_back(mcF1_fromV2_p);
    buffer.push_back(mcF2_fromV2_p);
  }
  
  // sort: first e/mu/tau - d/s/b 
  // sort: then nue/numu/nutau - u/c/t
  if( ((buffer.at(0) -> pdgId())%2) == 0 )
  {
    const reco::Candidate* temp = buffer.at(0);
    buffer.at(0) = buffer.at(1);
    buffer.at(1) = temp;
  }
  
  
  
  for(unsigned int i = 0; i < buffer.size(); ++i)
  {
    if( abs(buffer.at(i) -> pdgId()) == 11 )
      stringBuffer += "e";
    if( abs(buffer.at(i) -> pdgId()) == 12 )
      stringBuffer += "nue";
    
    if( abs(buffer.at(i) -> pdgId()) == 13 )
      stringBuffer += "mu";
    if( abs(buffer.at(i) -> pdgId()) == 14 )
      stringBuffer += "numu";
    
    if( abs(buffer.at(i) -> pdgId()) == 15 )
      stringBuffer += "tau";
    if( abs(buffer.at(i) -> pdgId()) == 16 )
      stringBuffer += "nutau";
    
    
    
    if( abs(buffer.at(i) -> pdgId()) == 1 )
      stringBuffer += "d";
    if( abs(buffer.at(i) -> pdgId()) == 2 )
      stringBuffer += "u";
    
    if( abs(buffer.at(i) -> pdgId()) == 3 )
      stringBuffer += "s";
    if( abs(buffer.at(i) -> pdgId()) == 4 )
      stringBuffer += "c";
    
    if( abs(buffer.at(i) -> pdgId()) == 5 )
      stringBuffer += "b";
    if( abs(buffer.at(i) -> pdgId()) == 6 )
      stringBuffer += "t";
    
    
    
    if(i == 0)  stringBuffer += "_";
  }
  
  
  
 if(Vindex == 1)
   mcV1DecayMode_p = stringBuffer;
 if(Vindex == 2)
   mcV2DecayMode_p = stringBuffer;
}
