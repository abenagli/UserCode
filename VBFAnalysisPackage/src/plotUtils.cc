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
    
    if(sample.at(0) == '#')
      continue;
    
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
  
  double globalMaximum = 0.;
  double globalMinimum = 1.;
  
  
  
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
  
  
  
  
  
  
  
  i = 0;
  for(std::map<std::string, double>::const_iterator mapIt = crossSection_summed.begin();
      mapIt != crossSection_summed.end(); ++mapIt)
  {    
    TH1F* globalHisto = histo_summed[mapIt->first];
    
    
    globalHisto -> Scale(1./mapIt->second);
    globalHisto -> SetLineColor(getColor(i));
    if(m_xAxisRange)
      globalHisto->GetXaxis()->SetRangeUser(m_xRangeMin, m_xRangeMax);
    if(i == 0)
      globalHisto -> SetLineWidth(4);
    else
      globalHisto -> SetLineWidth(2);
        
    
    if(globalHisto->GetMaximum() > globalMaximum)
      globalMaximum = globalHisto -> GetMaximum();
    if( (globalHisto->GetMinimum() < globalMinimum) && (globalHisto->GetMinimum() > 0.) )
      globalMinimum = globalHisto -> GetMinimum();    
    
    
    hs -> Add(globalHisto);
    legend.AddEntry(globalHisto, (mapIt->first).c_str(), "L");
    
    ++i;
  }
  
  
  
  
  
  
  // draw the stack and save file
  TCanvas* c1 = new TCanvas();
  c1 -> cd();
  
  
  hs -> Draw("nostack");
  legend.Draw("same");
  
  
  if(logy)
    c1 -> SetLogy();
  if(logy)
    hs->GetYaxis()->SetRangeUser(globalMinimum - 0.1*globalMinimum,
                                 globalMaximum + 0.1*globalMaximum);
  
  
  hs->GetXaxis()->SetTitle(histoName.c_str());
  if(m_xAxisTitle)
    hs->GetXaxis()->SetTitle(m_xTitle.c_str());
  hs -> GetXaxis() -> SetTitleSize(0.04);
  hs -> GetXaxis() -> SetLabelSize(0.03);
  hs -> GetXaxis() -> SetTitleOffset(1.25);

  hs->GetYaxis()->SetTitle("event fraction");  
  if(m_yAxisTitle)
    hs->GetYaxis()->SetTitle(m_yTitle.c_str());    
  hs -> GetYaxis() -> SetTitleSize(0.04);
  hs -> GetYaxis() -> SetLabelSize(0.03);
  hs -> GetYaxis() -> SetTitleOffset(1.50);
  
  
  if(m_xAxisRange)
    hs->GetXaxis()->SetRangeUser(m_xRangeMin, m_xRangeMax);
  
  hs->GetYaxis()->SetRangeUser(0., globalMaximum+0.1*globalMaximum);
  if(m_yAxisRange)
    hs->GetYaxis()->SetRangeUser(m_yRangeMin, m_yRangeMax);
  
  
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






void drawTStack::DrawEvents(const std::string& mode, const float& lumi, const bool& logy)
{ 
  std::cout << "Drawing " << mode << std::endl;
  
  
  
  THStack* hs = new THStack("hs", "hs");
  TLegend legend(0.15, 0.15, 0.35, 0.45);
  legend.SetFillColor(kWhite);
  
  double globalMaximum = 0.;
  double globalMinimum = 1.;
  
  
  
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
    std::string fullHistoName = "events";
    //std::cout << "getting histogram " << fullHistoName << std::endl;
    TH1F* histo = (TH1F*)(rootFiles.at(i) -> Get(fullHistoName.c_str()));
    
    
    crossSection_summed[mapIt->second] += m_crossSection[mapIt->first];
    
    if(mode == "eventsScaled")
    {
      histo -> Scale(lumi*m_crossSection[mapIt->first]/histo->GetBinContent(1));
    }
    
    
    
    // sum histograms
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
  
  
  
  
  
  std::ofstream* outFile = NULL;

  if(mode == "events")
    outFile = new std::ofstream((m_outputDir+"events.txt").c_str(), std::ios::out);  
  if(mode == "eventsScaled")
    outFile = new std::ofstream((m_outputDir+"eventsScaled.txt").c_str(), std::ios::out);
  if(mode == "efficiencies")
    outFile = new std::ofstream((m_outputDir+"efficiencies.txt").c_str(), std::ios::out);
  if(mode == "efficienciesRelative")
    outFile = new std::ofstream((m_outputDir+"efficienciesRelative.txt").c_str(), std::ios::out);  
  
  
  
  i = 0;
  for(std::map<std::string, double>::const_iterator mapIt = crossSection_summed.begin();
      mapIt != crossSection_summed.end(); ++mapIt)
  {    
    TH1F* globalHisto = histo_summed[mapIt->first];
    
    
    
    if(mode == "efficiencies")
    {
      int totalEvents = (int)(globalHisto -> GetBinContent(1));
      for(int bin = 1; bin <= globalHisto->GetNbinsX(); ++bin)
        globalHisto->SetBinContent(bin, globalHisto->GetBinContent(bin)/totalEvents);
    }
    
    if(mode == "efficienciesRelative")
    {
      std::map<int, int > totalEvents;
      totalEvents[0] = (int)(globalHisto->GetBinContent(1));
      for(int bin = 1; bin <= globalHisto->GetNbinsX(); ++bin)
        totalEvents[bin] = (int)(globalHisto->GetBinContent(bin));
      
      for(int bin = 1; bin <= globalHisto->GetNbinsX(); ++bin)        
        globalHisto->SetBinContent(bin, globalHisto->GetBinContent(bin)/totalEvents[bin-1]);
    }

    globalHisto -> SetLineColor(getColor(i));
    if(m_xAxisRange)
      globalHisto->GetXaxis()->SetRangeUser(m_xRangeMin, m_xRangeMax);
    if(i == 0)
      globalHisto -> SetLineWidth(4);
    else
      globalHisto -> SetLineWidth(2);
        
    
    if(globalHisto->GetMaximum() > globalMaximum)
      globalMaximum = globalHisto -> GetMaximum();
    if( (globalHisto->GetMinimum() < globalMinimum) && (globalHisto->GetMinimum() > 0.) )
      globalMinimum = globalHisto -> GetMinimum();    
    
    
    
    if(outFile)
    {
      (*outFile) << "\n" << mapIt->first;
      for(int bin = 1; bin <= globalHisto->GetNbinsX(); ++bin)
      {
        const char* binLabel = globalHisto -> GetXaxis() -> GetBinLabel(bin);
        (*outFile) << "   " << binLabel << ":   " << std::fixed
                                                  << std::setprecision(3)
                                                  << std::scientific
                                                  << 1. * globalHisto -> GetBinContent(bin);
      }
      
      (*outFile) << "\n";
    }
    
    
    
    hs -> Add(globalHisto);
    legend.AddEntry(globalHisto, (mapIt->first).c_str(), "L");
    
    ++i;
  }
  
  
  
  
  
  
  // draw the stack and save file
  TCanvas* c1 = new TCanvas();
  c1 -> cd();
  
  
  hs -> Draw("nostack");
  legend.Draw("same");
  
  
  if(logy)
    c1 -> SetLogy();
  if(logy)
    hs->GetYaxis()->SetRangeUser(globalMinimum - 0.1*globalMinimum,
                                 globalMaximum + 0.1*globalMaximum);
  
  
  hs -> GetXaxis() -> SetTitleSize(0.04);
  hs -> GetXaxis() -> SetLabelSize(0.03);
  hs -> GetXaxis() -> SetTitleOffset(1.25);

  hs -> GetYaxis() -> SetTitleSize(0.04);
  hs -> GetYaxis() -> SetLabelSize(0.03);
  hs -> GetYaxis() -> SetTitleOffset(1.50);
  
  
  if(m_xAxisRange)
    hs->GetXaxis()->SetRangeUser(m_xRangeMin, m_xRangeMax);
  
  hs->GetYaxis()->SetRangeUser(0., globalMaximum+0.1*globalMaximum);
  if(m_yAxisRange)
    hs->GetYaxis()->SetRangeUser(m_yRangeMin, m_yRangeMax);
  
  
  if(mode == "events")
  {
    hs->GetYaxis()->SetTitle("events left"); 
    c1->Print((m_outputDir+"events.pdf").c_str(), "pdf");
  }
  
  if(mode == "eventsScaled")
  {
    char buffer[50];
    sprintf(buffer, "events / %.0f pb^{-1}", lumi);
    hs->GetYaxis()->SetTitle(buffer);
    c1->Print((m_outputDir+"eventsScaled.pdf").c_str(), "pdf");
  }
  
  if(mode == "efficiencies")
  {
    hs->GetYaxis()->SetTitle("efficiency");
    c1->Print((m_outputDir+"efficiencies.pdf").c_str(), "pdf");
  }
  
  if(mode == "efficienciesRelative")
  {
    hs->GetYaxis()->SetTitle("relative efficiency");
    c1->Print((m_outputDir+"efficienciesRelative.pdf").c_str(), "pdf");  
  }
  
  
  
  // close root files
  if(outFile)
    outFile -> close();
  
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

