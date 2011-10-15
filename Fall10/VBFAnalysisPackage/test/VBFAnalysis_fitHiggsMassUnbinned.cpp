#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"

#include <iostream>

#include "TH1F.h"
#include "TF1.h"
#include "TFitter.h"



int nBins = 200;
float xMin = 0.;
float xMax = 1000.;
double superGausCumCauda(double* x, double* par);

std::vector<double>* mydata;
std::vector<double>* myweights;

void mylike(int& /*npar*/, double* /*gin*/, double& fval, double* par, int /*iflag*/);






int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> VBFAnalysis_fitHiggsMass::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
  
  
  //[Input]
  std::string baseDir      = gConfigParser -> readStringOption("Input::baseDir");
  std::string jetAlgorithm = gConfigParser -> readStringOption("Input::jetAlgorithm");
  std::string jetType      = gConfigParser -> readStringOption("Input::jetType");
  float lumi = gConfigParser -> readFloatOption("Input::lumi");
  int higgsMass = gConfigParser -> readIntOption("Input::higgsMass");
  char higgsMassChar[50];
  sprintf(higgsMassChar,"%d",higgsMass);
  
  std::string inputFileName = gConfigParser -> readStringOption("Input::inputFileName");
  std::vector<std::string> inputSigDirs  = gConfigParser -> readStringListOption("Input::inputSigDirs");
  std::vector<std::string> inputBkgDirs  = gConfigParser -> readStringListOption("Input::inputBkgDirs");
  std::vector<std::string> inputDataDirs = gConfigParser -> readStringListOption("Input::inputDataDirs");
  
  unsigned int nSigTrees  = inputSigDirs.size();
  unsigned int nBkgTrees  = inputBkgDirs.size();
  unsigned int nTotTrees = nSigTrees + nBkgTrees;
  unsigned int nDataTrees = inputDataDirs.size();
  
  std::vector<std::string> inputTotDirs;
  for(unsigned int i = 0; i < nSigTrees; ++ i) inputTotDirs.push_back(inputSigDirs.at(i));
  for(unsigned int i = 0; i < nBkgTrees; ++ i) inputTotDirs.push_back(inputBkgDirs.at(i));
  
  
  //[Output]
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  
  
  //[Options]
  int entryFIRST  = gConfigParser -> readIntOption("Options::entryFIRST");
  int entryMAX    = gConfigParser -> readIntOption("Options::entryMAX");
  int entryMODULO = gConfigParser -> readIntOption("Options::entryMODULO");
  int step        = gConfigParser -> readIntOption("Options::step");
  
  //[Cuts]
  float lepNuWMMIN = gConfigParser -> readFloatOption("Cuts::lepNuWMMIN");
  float lepNuWMMAX = gConfigParser -> readFloatOption("Cuts::lepNuWMMAX");
  float xFitMIN = gConfigParser -> readFloatOption("Cuts::xFitMIN");
  float xFitMAX = gConfigParser -> readFloatOption("Cuts::xFitMAX");
  
  
  
  
  
  
  // Define the histogram
  TH1F* h_lepNuW_m = new TH1F("h_lepNuW_m","",nBins,xMin,xMax);
  h_lepNuW_m -> Sumw2();
  
  
  
  // Define the event container for unbinned fit
  mydata = new std::vector<double>;
  myweights = new std::vector<double>;
  
  
  
  // Define tree variables
  float mH;
  int totEvents;
  float crossSection;
  int PUit_n;
  int PUoot_n;
  float lepNuW_m;
  
  
  
  // Define the output file
  std::string outputRootFullFileName = outputRootFilePath + "/" + outputRootFileName + "_" + jetAlgorithm + "_H" + std::string(higgsMassChar) + ".root";
  TFile* outFile = new TFile(outputRootFullFileName.c_str(), "RECREATE");
  
  
  
  
  
  
  //-------------------
  // loop on the events
  
  for(unsigned int i = 0; i < nDataTrees; ++i)
  {
    // open root file
    std::string inputFullFileName;
    inputFullFileName = baseDir + "/" + inputDataDirs.at(i)   + "/" + inputFileName + ".root";
    TFile* inputFile = TFile::Open(inputFullFileName.c_str());
    
    std::cout << ">>>>> VBFAnalysis_fitHiggsMass:: data tree in " << inputDataDirs.at(i) << " opened" << std::endl;
    
    
    // get the tree at nth step
    TTree* tree = NULL;
    char treeName[50];
    sprintf(treeName, "ntu_%d", step);
    inputFile -> GetObject(treeName, tree);
    if ( tree -> GetEntries() == 0 ) continue; 
    
    
    // set tree branches
    tree -> SetBranchAddress("lepNuW_m", &lepNuW_m);
    
    
    std::cout << ">>>>>>>>> Read " << tree -> GetEntries() << " entries" << std::endl;    
    for(int entry = 0; entry < tree->GetEntries(); ++entry)
    {
      //std::cout << "reading entry " << entry << std::endl;
      tree -> GetEntry(entry);
      double weight = 1.;
      
      h_lepNuW_m -> Fill(lepNuW_m,weight);
      
      if( ( (lepNuW_m < lepNuWMMIN)  || (lepNuW_m >= lepNuWMMAX) ) &&
          ( (lepNuW_m >= xFitMIN) && (lepNuW_m < xFitMAX) ) )
      {
        mydata -> push_back(lepNuW_m);
        myweights -> push_back(weight);
      }
    }    
  }
  
  
  
  
  
  
  TF1* f_gaus = new TF1("f_gaus","[0]*exp(-1.*(x-[1])*(x-[1])/(2*[2]*[2]))",xMin,xMax);
  f_gaus -> SetParameters(1.,200.,20.);
  f_gaus -> SetNpx(10000);
  f_gaus -> SetLineWidth(1);
  f_gaus -> SetLineColor(kBlue);
  h_lepNuW_m -> Fit("f_gaus","+","",175.,225.);
  
  TF1* f_expo = new TF1("f_expo","[0]*exp(-1.*[1]*x)",xMin,xMax);
  f_expo -> SetNpx(10000);
  f_expo -> SetLineWidth(1);
  f_expo -> SetLineColor(kBlue);
  h_lepNuW_m -> Fit("f_expo","+","",225.,xMax);
  
  TF1* f_sgcc = new TF1("f_sgcc",superGausCumCauda,xMin,xMax,4);  
  f_sgcc -> SetParameter(0,50000.);
  f_sgcc -> SetParameter(1,f_gaus->GetParameter(1));
  f_sgcc -> SetParameter(2,f_gaus->GetParameter(2));
  f_sgcc -> SetParameter(3,f_gaus->GetParameter(2)*f_expo->GetParameter(1));
  f_sgcc -> SetNpx(10000);
  f_sgcc -> SetLineWidth(2);
  f_sgcc -> SetLineColor(kGreen);
  h_lepNuW_m -> Fit("f_sgcc","+","",175.,xMax);  

  
  
  TFitter* myfit = new TFitter(3);
  myfit->SetFCN(mylike);
  myfit->Clear();
  myfit->SetParameter(0, "mean", f_gaus->GetParameter(1),0.0005,100.,300.);
  myfit->SetParameter(1, "sigma",f_gaus->GetParameter(2),0.0005,  0.,50.);
  myfit->SetParameter(2, "alpha",f_gaus->GetParameter(2)*f_expo->GetParameter(1),0.0005,0.,1.);
  
  double arglist[2];
  arglist[0] = 10000; // Max number of function calls
  arglist[1] = 1e-5;  // Tolerance on likelihood ?????????

  int fStatus = myfit->ExecuteCommand("MIGRAD",arglist,2); 
  
  TF1* fitFunc = new TF1("fitFunc",superGausCumCauda,xMin,xMax,4);
  fitFunc -> SetNpx(10000);
  fitFunc -> SetLineWidth(2);
  fitFunc -> SetLineColor(kRed);
  
  fitFunc -> SetParameter(0,1.);  
  for(int parIt = 1; parIt < 4; ++parIt)
    fitFunc -> SetParameter(parIt,myfit->GetParameter(parIt-1));

  fitFunc -> SetParameter(0, 1. * (xMax-xMin)/nBins * mydata->size() / (fitFunc->Integral(xFitMIN,lepNuWMMIN)+fitFunc->Integral(lepNuWMMAX,xFitMAX)) );
  
  
  
  outFile -> cd();
  h_lepNuW_m -> Write();
  fitFunc -> Write();
  outFile -> Close();
  
  
  return 0;
}






double superGausCumCauda(double* x, double* par)
{
  //[0] = N
  //[1] = mean
  //[2] = sigma
  //[3] = alpha
  
  double xx = x[0];
  double mean = par[1];
  double sigma = par[2];
  double alpha = par[3];

  if( xx < (mean+alpha*sigma) )
    {
      return par[0] / sigma / sqrt(2.*3.14159) * exp(-1.*(xx-mean)*(xx-mean)/(2.*sigma*sigma));
    }

  else
    {
      double N = par[0] / sigma / sqrt(2.*3.14159) * exp( 0.5*alpha*alpha + alpha/sigma*mean );
      double K = alpha/sigma;

      return N * exp(-1.*K*xx);
    }
}



void mylike(int& /*npar*/, double* /*gin*/, double& fval, double* par, int /*iflag*/)
{
  TF1* func = new TF1("func",superGausCumCauda,xMin,xMax,4);
  
  // initialize parameters
  func -> SetParameter(0,1.);
  for(int parIt = 1; parIt < 4; ++parIt)
    func -> SetParameter(parIt,par[parIt-1]);
  
  // normalize pdf to integral = 1
  func -> SetParameter(0, func->GetParameter(0) / func->Integral(xMin,xMax) );
  
  // compute logL
  double sumlog = 0;
  for(unsigned int i = 0; i < mydata->size(); ++i)
    {
      double p = func->Eval( mydata->at(i) );
      double w = myweights->at(i);
      sumlog += log(p) + log(w);
      //std::cout << "i: " << i << "   fval: " << sumlog << std::endl;
    }
  
  fval = -2. * sumlog;
}
