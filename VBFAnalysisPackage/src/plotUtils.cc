#include "plotUtils.h"
#include "setTDRStyle.h"



drawTStack::drawTStack(const std::string& inputDir,
                       const std::string& listFileName,
                       const std::string& baseRootFileName,
                       const std::string& jetAlgorithm,
                       const std::string& outputDir):
 m_inputDir(inputDir),
 m_listFileName(listFileName),
 m_baseRootFileName(baseRootFileName),
 m_jetAlgorithm(jetAlgorithm),
 m_outputDir(outputDir),
 m_xAxisRange(false),
 m_xRangeMin(0.),
 m_xRangeMax(1.),
 m_xAxisTitle(false),
 m_xTitle(""),
 m_yAxisRange(false),
 m_yRangeMin(0.),
 m_yRangeMax(1.),
 m_yAxisTitle(false),
 m_yTitle("")
{
  
  //----------------------------------------------------------
  // read the file with the list of samples and cross sections
  //----------------------------------------------------------
  
  std::string listFullFileName = inputDir+listFileName;
  std::ifstream listFile( listFullFileName.c_str() );
  //std::cout << "Opening file " << listFullFileName << std::endl;
  
  while(!listFile.eof())
  {
    std::string sample;
    std::string sumName;
    double crossSection;
    
    listFile >> sample >> sumName >> crossSection;
    
    m_list[sample] = sumName;
    m_crossSection[sample] = crossSection;
  }  
}






drawTStack::~drawTStack()
{
  delete c1;
}






void drawTStack::Draw(const std::string& histoName, const int& step,
                      const int& rebin, const bool& logy)
{ 
  std::cout << "Drawing histogram " << histoName << std::endl;
  
  
  
  THStack* hs = new THStack("hs", "hs");
  TLegend legend(0.15, 0.15, 0.35, 0.45);
  legend.SetFillColor(kWhite);
  
  
  //---------------------------------------------
  // define the map with summed cross sections
  //---------------------------------------------
  
  std::map<std::string, double> crossSection_summed;
  std::map<std::string, bool> isFirstSample_summed;
  std::map<std::string, TH1F*> histo_summed;

  for(std::map<std::string, std::string>::const_iterator mapIt = m_list.begin();
      mapIt != m_list.end(); ++mapIt)
  {
    histo_summed[mapIt->second] = NULL;
    isFirstSample_summed[mapIt->second] = true;
  }
  
  
  
  //---------------------------------------------
  // loop over all the samples and fill the stack
  //---------------------------------------------

  std::vector<TFile*> rootFiles;
  int i = 0;
  for(std::map<std::string, std::string>::const_iterator mapIt = m_list.begin();
      mapIt != m_list.end(); ++mapIt)
  {
    // open root file
    std::string fullRootFileName = m_inputDir+mapIt->first+"/"+m_baseRootFileName+"_"+m_jetAlgorithm+".root";
    //std::cout << "opening file: " << fullRootFileName << std::endl;
    rootFiles.push_back(new TFile(fullRootFileName.c_str(), "READ"));
    
    
    // get histogram
    char buffer[10];
    sprintf(buffer, "%d", step);
    std::string fullHistoName = histoName + "/" + "h_" + buffer + "_" + histoName;
    //std::cout << "getting histogram " << fullHistoName << std::endl;
    TH1F* histo = (TH1F*)(rootFiles.at(i) -> Get(fullHistoName.c_str()));
    
    histo -> Rebin(rebin);
    if(histo->GetEntries() > 0.)
    {
      histo -> Scale(m_crossSection[mapIt->first]/histo->GetEntries());
      crossSection_summed[mapIt->second] += m_crossSection[mapIt->first];
    }
    
    
    
    // sum histograms scaled by cross section
    if( isFirstSample_summed[mapIt->second] == true )
    {
      histo_summed[mapIt->second] = (TH1F*)(histo -> Clone());
      isFirstSample_summed[mapIt->second] = false;
    }
    else
    {
      histo_summed[mapIt->second] -> Add(histo);
    }
    
    
    ++i;
  }
  
  
  
  TCanvas* c1 = new TCanvas();
  c1 -> cd();
  if(logy)
    c1 -> SetLogy();
  
  
  
  i = 0;
  for(std::map<std::string, double>::const_iterator mapIt = crossSection_summed.begin();
      mapIt != crossSection_summed.end(); ++mapIt)
  {    
    // fill stack
    histo_summed[mapIt->first] -> Scale(1./mapIt->second);
    histo_summed[mapIt->first] -> SetLineColor(getColor(i));
    if(i == 0)
      histo_summed[mapIt->first] -> SetLineWidth(4);
    else
      histo_summed[mapIt->first] -> SetLineWidth(2);
    
    hs -> Add(histo_summed[mapIt->first]);
    legend.AddEntry(histo_summed[mapIt->first], (mapIt->first).c_str(), "L");
    
    ++i;
  }

  hs -> Draw("nostack");
  legend.Draw("same");
  
  
  if(m_xAxisRange)
    hs->GetXaxis()->SetRangeUser(m_xRangeMin, m_xRangeMax);
  if(m_xAxisTitle)
    hs->GetXaxis()->SetTitle(m_xTitle.c_str());
  hs -> GetXaxis() -> SetTitleSize(0.04);
  hs -> GetXaxis() -> SetLabelSize(0.03);
  hs -> GetXaxis() -> SetTitleOffset(1.25);

  if(m_yAxisRange)
    hs->GetYaxis()->SetRangeUser(m_yRangeMin, m_yRangeMax);
  if(m_yAxisTitle)
    hs->GetYaxis()->SetTitle(m_yTitle.c_str());    
  hs -> GetYaxis() -> SetTitleSize(0.04);
  hs -> GetYaxis() -> SetLabelSize(0.03);
  hs -> GetYaxis() -> SetTitleOffset(1.50);
  
  
  
  c1->Print((m_outputDir+histoName+".pdf").c_str(), "pdf");
  
  
  
  // close root files
  for(unsigned int i = 0; i < rootFiles.size(); ++i)
  {
    rootFiles.at(i) -> Close();
    delete rootFiles.at(i);
  }
  
  m_xAxisRange = false;
  m_xAxisTitle = false;
  m_yAxisRange = false;
  m_yAxisTitle = false;
  
  delete c1;
  delete hs;
}






void drawTStack::SetXaxisRange(const double& xMin, const double& xMax)
{
  m_xAxisRange = true;
  m_xRangeMin = xMin;
  m_xRangeMax = xMax;
}

void drawTStack::SetXaxisTitle(const std::string& xTitle)
{
  m_xAxisTitle = true;
  m_xTitle = xTitle;
}



void drawTStack::SetYaxisRange(const double& yMin, const double& yMax)
{
  m_yAxisRange = true;
  m_yRangeMin = yMin;
  m_yRangeMax = yMax;
}

void drawTStack::SetYaxisTitle(const std::string& yTitle)
{
  m_yAxisTitle = true;
  m_yTitle = yTitle;
}
