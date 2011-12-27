#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"

#include <iomanip>
#include <string>

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"

using namespace std ;

int main(int argc, char** argv)
{
//  //Check if all nedeed arguments to parse are there
//  if(argc != 2)
//  {
//    std::cerr << ">>>>> VBFPreselection::usage: " << argv[0] << " configFileName" << std::endl ;
//    return 1;
//  }
//
//  // Parse the config file
//  parseConfigFile(argv[1]);
//  

  string inputFileList = argv[1] ;
  string treeName = "MiBiCommonNTOneLeptonTwoJetsPFlow/SimpleNtuple" ;
  TChain* chain = new TChain (treeName.c_str ()) ;
  if (!FillChain (*chain, inputFileList.c_str ())) return 1 ;
  treeReader reader ( (TTree*) (chain), false) ;

  TH2F h_pesi ("h_pesi", "h_pesi", 60, 0, 60, 1000, 0.0005, 2.0005) ;
  h_pesi.GetXaxis ()->SetTitle ("weight number") ;
  h_pesi.GetYaxis ()->SetTitle ("weight value") ;

  // LOOP OVER THE EVENTS
  for (int entry = 0 ; entry < chain->GetEntries () ; ++entry)
    {
      reader.GetEntry (entry) ;
      if ((entry % 1000) == 0) cout << "event " << entry << endl ;   
      reader.GetInt ("isEB") ;
      std::vector<float>                  * PDFWeights   = reader.GetFloat ("PDFWeights") ; 
      cout << "number of weights : " << PDFWeights->size () << endl ;
      for (unsigned int i = 0 ; i < PDFWeights->size () ; ++i)
        {
          h_pesi.Fill (i, PDFWeights->at (i)) ;
        }
    } // LOOP OVER THE EVENTS

  TFile outFile ("MiBi_testPDFWeights.root", "recreate") ;
  outFile.cd () ;
  h_pesi.Write () ;
  outFile.Close () ;
  
  return 0 ;
}  
