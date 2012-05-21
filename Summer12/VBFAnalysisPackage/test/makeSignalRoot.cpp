#include "ntpleUtils.h"
#include <string>
#include <map>
#include "TH1.h"
#include "TMVA/Reader.h"
#include "hColl.h"

using namespace std ;



int ReadFile (map<string, TChain *> & output, string inputList, string treeName)
{
	std::ifstream inFile (inputList.c_str ()) ;
	std::string buffer ; 
	while (!inFile.eof ())
	{
		getline (inFile, buffer) ;
		if (buffer != "") 
		{ ///---> save from empty line at the end!
			if (buffer.at(0) != '#')
			{
				std::string dummyName ;
				std::stringstream line ( buffer );       
				line >> dummyName ; 
				std::cout << dummyName << " ";
				std::string dummyLocation ;
				line >> dummyLocation ; 
				std::cout << dummyLocation << "\n";

				// Open tree
				if (output.find (dummyName) == output.end ())
				{
					TChain* chain = new TChain (treeName.c_str ()) ;
					chain->Add (dummyLocation.c_str ()) ;
					output[dummyName] = chain ;
				}
				else 
					output[dummyName]->Add (dummyLocation.c_str ()) ; 
			} 
		}
	}
	return 0 ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int main (int argc, char** argv)
{
	//Check if all nedeed arguments to parse are there
	if (argc != 4)
	{
		std::cerr << ">>>>> VBFAnalysis::usage: " << argv[0] << " configFileName" <<" Outfilename" <<" flavor"<<std::endl ;
		return 1 ;
	}

	float LUMI = 4680. ; //PG to have output in 1/fb

	string inputFileList (argv[1]) ;
	cout << "samples " << inputFileList << endl ;

	map<string, TChain *> collections ;
	string treeName = "ntu_13" ;
	ReadFile (collections, inputFileList, treeName) ;

	TH1::SetDefaultSumw2 (kTRUE) ;

	int nbins = 100 ,m4min = 0 , m4max = 1000;


	TString OutFileName(argv[2]);
	TFile* outputRootFile = new TFile (OutFileName+"_"+argv[3]+".root", "RECREATE") ;
	outputRootFile->cd () ;

	TCut cutSignal = "WJJ_m > 65  && WJJ_m < 95" ;
	TCut cutFlavor = "";
	TString flavor = argv[3];
	if (flavor == "emu") cutFlavor = "";
	else if (flavor == "e")  cutFlavor = "lep_flavour == 11";
	else if (flavor == "mu") cutFlavor = "lep_flavour == 13";
	cutSignal = cutSignal && cutFlavor;

	TCut cutSignalExtended = Form ("(%s) * 1./totEvents * crossSection * %f * PUWeight * eventWeight", cutSignal.GetTitle (), LUMI);


	//PG loop over samples
	for (map<string, TChain *>::iterator iColl = collections.begin () ;	iColl != collections.end () ; ++iColl)
	{
		//PG search for signals
		if (iColl->first.find ("ggH") == string::npos && iColl->first.find ("qqH") == string::npos) continue ;

		cout << " reading " << iColl->first << endl ;
		TString mass = iColl->first ;
		//		mass.erase (0, 3) ;
		cout << "MASS : " << mass << endl ;

		TH1F * histo = new TH1F(mass,mass,nbins, m4min, m4max);
		iColl->second->Draw (TString ("lepNuW_m_KF >> ") + histo->GetName (), cutSignalExtended) ;
		histo->Write();

	} //PG loop over samples
	cout << "filling done " << endl ;    

	outputRootFile->Close () ;
	delete outputRootFile ;
	return 0 ;
}
