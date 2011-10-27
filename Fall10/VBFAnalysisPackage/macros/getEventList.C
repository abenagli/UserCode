#include <iomanip>



void getEventList(const std::string& inputFileName)
{
  TFile* f = TFile::Open(inputFileName.c_str(),"READ");
  TTree* t = (TTree*)( f->Get("ntu_15") );
  
  int runId;
  int lumiId;
  int eventId;
  int lep_flavour;
  float lepNuW_m;
  float chi2_KF;
  
  t -> SetBranchAddress("runId",      &runId);
  t -> SetBranchAddress("lumiId",     &lumiId);
  t -> SetBranchAddress("eventId",    &eventId);
  t -> SetBranchAddress("lep_flavour",&lep_flavour);
  t -> SetBranchAddress("lepNuW_m",   &lepNuW_m);
  t -> SetBranchAddress("chi2_KF",    &chi2_KF);
  
  int iSelected = 0;
  for(int entry = 0; entry < t->GetEntries(); ++entry)
  {
    t -> GetEntry(entry);
    
    if( (lepNuW_m > 390.) && (lepNuW_m < 410.) && (chi2_KF < 3.) && (lep_flavour == 13) )
    {
      std::cout << ">>> selected event: " << std::fixed << std::setw(4)  << iSelected 
                << "   ***   runId:lumiId:eventId   "
                << std::fixed << std::setw(6)  << runId   << " "
                << std::fixed << std::setw(4)  << lumiId  << " "
                << std::fixed << std::setw(10) << eventId << " "
                << std::endl;
      ++iSelected;
    }
  }
}
