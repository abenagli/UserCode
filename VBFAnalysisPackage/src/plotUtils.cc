#include "plotUtils.h"
#include "setTDRStyle.h"



drawTStack::drawTStack(const std::string& inputDir,
                       const std::string& listFileName,
                       const std::string& baseRootFileName,
                       const std::string& outputDir):
 m_inputDir(inputDir),
 m_listFileName(listFileName),
 m_baseRootFileName(baseRootFileName),
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
 m_yTitle(""),
 m_drawLegend(true),
 m_xLowLegend(0.68),
 m_yLowLegend(0.78),
 m_xHighLegend(0.99),
 m_yHighLegend(0.99)
{
  
  //----------------------------------------------------------
  // read the file with the list of samples and cross sections
  //----------------------------------------------------------
  
  std::string listFullFileName = inputDir+listFileName;
  std::ifstream listFile( listFullFileName.c_str() );
  if(!listFile.is_open())
  {
    std::cout << "\n>>>plotUtils::Error opening file " << listFullFileName << std::endl;
    exit(-1);
  }
  else
    std::cout << "\n>>>plotUtils::Opening file " << listFullFileName << std::endl;
  
  
  
  while(!listFile.eof())
  {
    std::string sample;
    std::string sumName;
    int dataFlag;
    double mH;
    double crossSection;
    std::string jetAlgorithm;
    
    listFile >> sample >> sumName >> dataFlag >> mH >> crossSection >> jetAlgorithm;

    if(sample.size() == 0)
      continue;
    if(sample.at(0) == '#')
      continue;
    
    std::cout << sample << "\t"
              << sumName << "\t"
              << dataFlag << "\t"
              << mH << "\t"
              << crossSection << "\t" 
              << jetAlgorithm << "\t" 
              << std::endl;
    
    std::pair<std::string, std::string> dummyPair(sample, sumName);
    m_list.push_back(dummyPair);
    m_dataFlag[sample] = dataFlag;
    m_mH[sample] = mH;
    m_crossSection[sample] = crossSection;
    
    std::pair<std::string, std::string> dummyPair2(sample, jetAlgorithm);
    m_jetAlgorithm.push_back(dummyPair2);
  }
  
  listFile.close();
  std::cout << ">>>plotUtils::Read " << m_list.size() << " samples" << std::endl;
  std::cout << ">>>plotUtils::Closing file " << listFullFileName << "\n" << std::endl;
}






drawTStack::~drawTStack()
{
  delete c1;
}






void drawTStack::Draw(const std::string& histoName, const int& step,
                      const int& rebin, const bool& logy)
{ 
  std::cout << "\n>>>plotUtils::Drawing histogram " << histoName << std::endl;
  
  
  
  THStack* hs = new THStack("hs", "hs");
  TLegend legend(0.68, 0.78, 0.99, 0.99);
  legend.SetFillColor(kWhite);
  
  double globalMaximum = 0.;
  double globalMinimum = 1.;
  
  
  
  //---------------------------------------------
  // define the map with summed cross sections
  //---------------------------------------------
  
  std::map<std::string, double> crossSection_summed;
  std::map<std::string, bool> isFirstSample_summed;
  std::map<std::string, TH1F*> histo_summed;
  std::map<std::string, double> events_summed;
  std::map<std::string, double> eventsScaled_summed;

  for(std::vector<std::pair<std::string, std::string> >::const_iterator vecIt = m_list.begin();
      vecIt != m_list.end(); ++vecIt)
  {
    histo_summed[vecIt->second] = NULL;
    isFirstSample_summed[vecIt->second] = true;
  }
  
  
  
  //---------------------------------------------
  // loop over all the samples and fill the stack
  //---------------------------------------------

  std::vector<TFile*> rootFiles;
  int i = 0;
  for(std::vector<std::pair<std::string, std::string> >::const_iterator vecIt = m_list.begin();
      vecIt != m_list.end(); ++vecIt)
  {
    // open root file
    std::string fullRootFileName = m_inputDir+vecIt->first+"/"+m_baseRootFileName+"_"+m_jetAlgorithm.at(i).second+".root";
    //std::cout << "opening file: " << fullRootFileName << std::endl;
    rootFiles.push_back(new TFile(fullRootFileName.c_str(), "READ"));
    if(!(rootFiles.at(i))->IsOpen()) exit(-1);
    
    
    
    // get histogram
    char buffer[10];
    sprintf(buffer, "%d", step);
    std::string fullHistoName = histoName + "/" + "h_" + buffer + "_" + histoName;
    //std::cout << "getting histogram " << fullHistoName << std::endl;
    
    std::string eventsHistoName = "events";
    //std::cout << "getting histogram " << eventsHistoName << std::endl;
    
    TH1F* histo = NULL;
    rootFiles.at(i) -> GetObject(fullHistoName.c_str(), histo);
    if(histo == NULL)
    {
      std::cout << ">>>plotUtils::Error in getting object " << fullHistoName << std::endl;
      exit(-1);
    }
    
    TH1F* eventsHisto = NULL;
    rootFiles.at(i) -> GetObject(eventsHistoName.c_str(), eventsHisto);
    if(eventsHisto == NULL)
    {
      std::cout << ">>>plotUtils::Error in getting object " << eventsHistoName << std::endl;
      exit(-1);
    }
    
    histo -> Sumw2();    
    histo -> Rebin(rebin);
    if(histo->GetEntries() > 0.)
    {
      histo -> Scale(m_crossSection[vecIt->first]/eventsHisto->GetBinContent(1));
      crossSection_summed[vecIt->second] += m_crossSection[vecIt->first];
      events_summed[vecIt->second] += eventsHisto->GetBinContent(1);
    }
    
    
    
    // sum histograms scaled by cross section
    if( isFirstSample_summed[vecIt->second] == true )
    {
      histo_summed[vecIt->second] = (TH1F*)(histo -> Clone());
      isFirstSample_summed[vecIt->second] = false;
    }
    else
    {
      histo_summed[vecIt->second] -> Add(histo);
    }
    
    
    ++i;
  }
  
  
  
  
  
  
  
  i = 0;
  // loop over summed histograms
  std::map<std::string, double>::const_iterator mapIt_begin = crossSection_summed.begin();
  for(std::map<std::string, double>::const_iterator mapIt = crossSection_summed.begin();
      mapIt != crossSection_summed.end(); ++mapIt)
  {    
    TH1F* globalHisto = histo_summed[mapIt->first];
    if(globalHisto -> GetEntries() == 0) continue;
    
    
    //globalHisto -> Scale(events_summed[mapIt->first]/globalHisto->GetEntries()/mapIt->second);
    globalHisto -> Scale(1./globalHisto->Integral(1, globalHisto->GetNbinsX()));
    globalHisto -> SetLineColor(getColor(i));
    globalHisto -> SetLineStyle(i+1);
    if(m_xAxisRange)
      globalHisto->GetXaxis()->SetRangeUser(m_xRangeMin, m_xRangeMax);
    if(i == 0)
      globalHisto -> SetLineWidth(4);
    else
      globalHisto -> SetLineWidth(4);
        
    
    if(globalHisto->GetMaximum() > globalMaximum)
      globalMaximum = globalHisto -> GetMaximum();
    if( (globalHisto->GetMinimum() < globalMinimum) && (globalHisto->GetMinimum() > 0.) )
      globalMinimum = globalHisto -> GetMinimum();    
    
    
    hs -> Add(globalHisto);
    legend.AddEntry(globalHisto, (mapIt->first).c_str(), "L");
    
    ++i;
  }  // loop over summed histograms
  
  // if no histograms had entries, return
  if(i == 0)
  {
    std::cout << ">>>plotUtils::Error, histograms empty" << std::endl;
    return;
  }
  
  
  
  
  // draw the stack and save file
  TCanvas* c1 = new TCanvas();
  c1 -> cd();
  c1 -> SetGridx();
  c1 -> SetGridy();
  
  
  hs -> Draw("nostack,HISTO");
  if(m_drawLegend == true)
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
  
  struct stat st;
  if(stat(m_outputDir.c_str(), &st) != 0)
  {
    std::cout << ">>>plotUtils::Error accessing directory " << m_outputDir << std::endl;
    exit(-1);
  }
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






void drawTStack::DrawData(const std::string& histoName, const int& step,
                          const int& rebin, const bool& logy)
{ 
  std::cout << "\n>>>plotUtils::Drawing histogram " << histoName << std::endl;
  
  
  
  THStack* hs = new THStack("hs", "hs");
  TLegend legend(0.68, 0.78, 0.99, 0.99);
  legend.SetFillColor(kWhite);
  
  double globalMaximum = 0.;
  double globalMinimum = 1.;
  
  
  
  //---------------------------------------------
  // define the map with summed cross sections
  //---------------------------------------------
  
  std::map<std::string, double> crossSection_summed;
  std::map<std::string, bool> isFirstSample_summed;
  std::map<std::string, int> dataFlag_summed;
  std::map<std::string, TH1F*> histo_summed;
  std::map<std::string, double> eventsScaled_summed;

  for(std::vector<std::pair<std::string, std::string> >::const_iterator vecIt = m_list.begin();
      vecIt != m_list.end(); ++vecIt)
  {
    histo_summed[vecIt->second] = NULL;
    isFirstSample_summed[vecIt->second] = true;
  }
  
  
  
  //---------------------------------------------
  // loop over all the samples and fill the stack
  //---------------------------------------------

  std::vector<TFile*> rootFiles;
  TH1F* histoData = 0;
  int i = 0;
  for(std::vector<std::pair<std::string, std::string> >::const_iterator vecIt = m_list.begin();
      vecIt != m_list.end(); ++vecIt)
  {
    // open root file
    std::string fullRootFileName = m_inputDir+vecIt->first+"/"+m_baseRootFileName+"_"+m_jetAlgorithm.at(i).second+".root";
    //std::cout << "opening file: " << fullRootFileName << std::endl;
    rootFiles.push_back(new TFile(fullRootFileName.c_str(), "READ"));
    if(!(rootFiles.at(i))->IsOpen()) exit(-1);
    
    
    
    // get histogram
    char buffer[10];
    sprintf(buffer, "%d", step);
    std::string fullHistoName = histoName + "/" + "h_" + buffer + "_" + histoName;
    //std::cout << "getting histogram " << fullHistoName << std::endl;
    TH1F* histo = NULL;
    rootFiles.at(i) -> GetObject(fullHistoName.c_str(), histo);
    if(histo == NULL)
    {
      std::cout << ">>>plotUtils::Error in getting object " << fullHistoName << std::endl;
      exit(-1);
    }

    histo -> Sumw2();    
    histo -> Rebin(rebin);
    if(histo->GetEntries() > 0.)
    {
      histo -> Scale(m_crossSection[vecIt->first]/histo->GetEntries());
      crossSection_summed[vecIt->second] += m_crossSection[vecIt->first];
    }

    // save data histogram
    dataFlag_summed[vecIt->second] = m_dataFlag[vecIt->first];
    if(m_dataFlag[vecIt->first] == 1)
     histoData = histo;
    
    
    
    // sum histograms scaled by cross section
    if( isFirstSample_summed[vecIt->second] == true )
    {
      histo_summed[vecIt->second] = (TH1F*)(histo -> Clone());
      isFirstSample_summed[vecIt->second] = false;
    }
    else
    {
      histo_summed[vecIt->second] -> Add(histo);
    }
    
    
    ++i;
  }
  
  
  
  
  
  
  
  i = 0;
  // loop over summed histograms
  for(std::map<std::string, double>::const_iterator mapIt = crossSection_summed.begin();
      mapIt != crossSection_summed.end(); ++mapIt)
  {    
    TH1F* globalHisto = histo_summed[mapIt->first];
    if(globalHisto -> GetEntries() == 0) continue;
    
    
    globalHisto -> Scale(1./mapIt->second);
    // temp
    globalHisto -> Scale(histoData->GetEntries());
    // temp
    globalHisto -> SetLineColor(getColor(i));
    globalHisto -> SetLineStyle(i+1);
    if(m_xAxisRange)
      globalHisto->GetXaxis()->SetRangeUser(m_xRangeMin, m_xRangeMax);
    
        
    
    if(globalHisto->GetMaximum() > globalMaximum)
      globalMaximum = globalHisto -> GetMaximum();
    if( (globalHisto->GetMinimum() < globalMinimum) && (globalHisto->GetMinimum() > 0.) )
      globalMinimum = globalHisto -> GetMinimum();    
    
    
    if(dataFlag_summed[mapIt->first] != 1)
    {
      globalHisto -> SetFillColor(kYellow);
      hs -> Add(globalHisto);
    }
    legend.AddEntry(globalHisto, (mapIt->first).c_str(), "L");
    
    ++i;
  }  // loop over summed histograms
  
  // if no histograms had entries, return
  if(i == 0)
  {
    std::cout << ">>>plotUtils::Error, histograms empty" << std::endl;
    return;
  }
  
  
  
  
  // draw the stack and save file
  TCanvas* c1 = new TCanvas();
  c1 -> cd();
  c1 -> SetGridx();
  c1 -> SetGridy();
  
  
  hs -> Draw("HISTO");
  histoData -> SetMarkerStyle(20);  
  histoData -> Scale(histoData->GetEntries());
  histoData -> Draw("P,same");
  
  if(m_drawLegend == true)
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

  hs->GetYaxis()->SetTitle("events");  
  if(m_yAxisTitle)
    hs->GetYaxis()->SetTitle(m_yTitle.c_str());    
  hs -> GetYaxis() -> SetTitleSize(0.04);
  hs -> GetYaxis() -> SetLabelSize(0.03);
  hs -> GetYaxis() -> SetTitleOffset(1.50);
  
  
  if(m_xAxisRange)
    hs->GetXaxis()->SetRangeUser(m_xRangeMin, m_xRangeMax);
  
  hs->SetMinimum(0.1);
  hs->SetMaximum(globalMaximum+0.1*globalMaximum);
  
  if(m_yAxisRange)
  {
    hs->SetMinimum(m_yRangeMin);
    hs->SetMaximum(m_yRangeMax);
  }
  
  
  
  struct stat st;
  if(stat(m_outputDir.c_str(), &st) != 0)
  {
    std::cout << ">>>plotUtils::Error accessing directory " << m_outputDir << std::endl;
    exit(-1);
  }
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






void drawTStack::DrawEvents(const std::string& mode, const float& lumi, const int& step, const bool& logy)
{ 
  std::cout << "\n>>>plotUtils::Drawing " << mode << std::endl;
  
  
  
  THStack* hs = new THStack("hs", "hs");
  TLegend legend(m_xLowLegend, m_yLowLegend, m_xHighLegend, m_yHighLegend);
  legend.SetFillColor(kWhite);
  
  double globalMaximum = 0.;
  double globalMinimum = 1.;
  
  
  
  //---------------------------------------------
  // define the map with summed cross sections
  //---------------------------------------------
 
  std::map<std::string, double> crossSection_summed;
  std::map<std::string, bool> isFirstSample_summed;
  std::map<std::string, bool> isSignal_summed;
  std::map<std::string, TH1F*> histo_summed;

  for(std::vector<std::pair<std::string, std::string> >::const_iterator vecIt = m_list.begin();
      vecIt != m_list.end(); ++vecIt)
  {
    histo_summed[vecIt->second] = NULL;
    isFirstSample_summed[vecIt->second] = true;
    isSignal_summed[vecIt->second] = false;
  }
  
  
  
  //---------------------------------------------
  // loop over all the samples and fill the stack
  //---------------------------------------------

  std::vector<TFile*> rootFiles;
  int i = 0;
  for(std::vector<std::pair<std::string, std::string> >::const_iterator vecIt = m_list.begin();
      vecIt != m_list.end(); ++vecIt)
  {
    // open root file
    std::string fullRootFileName = m_inputDir+vecIt->first+"/"+m_baseRootFileName+"_"+m_jetAlgorithm.at(i).second+".root";
    //std::cout << "opening file: " << fullRootFileName << std::endl;
    rootFiles.push_back(new TFile(fullRootFileName.c_str(), "READ"));
    
    
    // get histogram
    std::string fullHistoName = "events";
    //std::cout << "getting histogram " << fullHistoName << std::endl;
    TH1F* histo = NULL;
    rootFiles.at(i) -> GetObject(fullHistoName.c_str(), histo);
    if(histo == NULL)
    {
      std::cout << ">>>plotUtils::Error in getting object " << fullHistoName << std::endl;
      exit(-1);
    }
    
    crossSection_summed[vecIt->second] += m_crossSection[vecIt->first];
    if(m_mH[vecIt->first] > 0)
      isSignal_summed[vecIt->second] = true;
    
    
    if( (mode == "eventsScaled") ||
        (mode == "efficiencies") ||
        (mode == "efficienciesRelativew") ||
        (mode == "significance") )
    {
      histo -> Scale(lumi*m_crossSection[vecIt->first]/histo->GetBinContent(1));
    }
    
    
    
    // sum histograms
    if( isFirstSample_summed[vecIt->second] == true )
    {
      histo_summed[vecIt->second] = (TH1F*)(histo -> Clone());
      isFirstSample_summed[vecIt->second] = false;
    }
    else
    {
      histo_summed[vecIt->second] -> Add(histo);
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
  if(mode == "significance")
    outFile = new std::ofstream((m_outputDir+"significance.txt").c_str(), std::ios::out);   
  
  
  
  
  
  int nSamples = 0;
  for(std::map<std::string, double>::const_iterator mapIt = crossSection_summed.begin();
      mapIt != crossSection_summed.end(); ++mapIt)
    ++nSamples;
  
  TH1F* stepHisto = new TH1F("stepHisto", "", nSamples, 0., 1.*nSamples);
  
  
  
  i = 0;
  for(std::map<std::string, double>::const_iterator mapIt = crossSection_summed.begin();
      mapIt != crossSection_summed.end(); ++mapIt)
  {    
    TH1F* globalHisto = histo_summed[mapIt->first];
    
    
    
    if(mode == "efficiencies")
    {
      int totalEvents = globalHisto -> GetBinContent(1);
      for(int bin = 1; bin <= globalHisto->GetNbinsX(); ++bin)
        globalHisto->SetBinContent(bin, globalHisto->GetBinContent(bin)/totalEvents);
    }
    
    if(mode == "efficienciesRelative")
    {
      std::map<int, int > totalEvents;
      totalEvents[0] = globalHisto->GetBinContent(1);
      for(int bin = 1; bin <= globalHisto->GetNbinsX(); ++bin)
        totalEvents[bin] = globalHisto->GetBinContent(bin);
      
      for(int bin = 1; bin <= globalHisto->GetNbinsX(); ++bin)        
        globalHisto->SetBinContent(bin, globalHisto->GetBinContent(bin)/totalEvents[bin-1]);
    }
    
    if(mode == "significance")
    {
      if(isSignal_summed[mapIt->first] == false) continue;
    
      TH1F* totalBkgHisto = NULL;
      for(std::map<std::string, double>::const_iterator mapIt2 = crossSection_summed.begin();
          mapIt2 != crossSection_summed.end(); ++mapIt2)
      {
        if(m_mH[mapIt2->first] < 0) continue;
        TH1F* globalHisto2 = histo_summed[mapIt2->first];
        
        if(totalBkgHisto == NULL)
          totalBkgHisto = (TH1F*)(globalHisto2 -> Clone());
        else
          totalBkgHisto -> Add(globalHisto2);
      }
      
      for(int bin = 1; bin <= globalHisto->GetNbinsX(); ++bin)
        globalHisto->SetBinContent(bin, globalHisto->GetBinContent(bin) / 
                                        sqrt(totalBkgHisto->GetBinContent(bin)) );
    }
    
    
    globalHisto -> SetLineColor(getColor(i));
    globalHisto -> SetLineStyle(i+1);
    if(m_xAxisRange)
      globalHisto->GetXaxis()->SetRangeUser(m_xRangeMin, m_xRangeMax);
    if(i == 0)
      globalHisto -> SetLineWidth(4);
    else
      globalHisto -> SetLineWidth(4);
        
    
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
    
    
    stepHisto -> SetBinContent(i+1, globalHisto->GetBinContent(step+1));
    stepHisto -> GetXaxis() -> SetBinLabel(i+1, (mapIt->first).c_str());
    ++i;
  }
  
  
  
  
  
  
  // draw the stack and save file
  TCanvas* c1 = new TCanvas();
  c1 -> cd();
  c1 -> SetGridx();
  c1 -> SetGridy();
  
  
  hs -> Draw("nostack");
  if(mode != "significance")
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
    stepHisto->GetYaxis()->SetTitle("events left");
    
    struct stat st;
    if(stat(m_outputDir.c_str(), &st) != 0)
    {
      std::cout << ">>>plotUtils::Error accessing directory " << m_outputDir << std::endl;
      exit(-1);
    }
    c1->Print((m_outputDir+"events.pdf").c_str(), "pdf");
  }
  
  if(mode == "eventsScaled")
  {
    char buffer[50];
    sprintf(buffer, "events / %.0f pb^{-1}", lumi);
    hs->GetYaxis()->SetTitle(buffer);
    stepHisto->GetYaxis()->SetTitle(buffer);
    
    struct stat st;
    if(stat(m_outputDir.c_str(), &st) != 0)
    {
      std::cout << ">>>plotUtils::Error accessing directory " << m_outputDir << std::endl;
      exit(-1);
    }
    c1->Print((m_outputDir+"eventsScaled.pdf").c_str(), "pdf");
  }
  
  if(mode == "efficiencies")
  {
    hs->GetYaxis()->SetTitle("efficiency");
    stepHisto->GetYaxis()->SetTitle("efficiency");
    
    struct stat st;
    if(stat(m_outputDir.c_str(), &st) != 0)
    {
      std::cout << ">>>plotUtils::Error accessing directory " << m_outputDir << std::endl;
      exit(-1);
    }
    c1->Print((m_outputDir+"efficiencies.pdf").c_str(), "pdf");
  }
  
  if(mode == "efficienciesRelative")
  {
    hs->GetYaxis()->SetTitle("relative efficiency");
    stepHisto->GetYaxis()->SetTitle("relative efficiency");
    
    struct stat st;
    if(stat(m_outputDir.c_str(), &st) != 0)
    {
      std::cout << ">>>plotUtils::Error accessing directory " << m_outputDir << std::endl;
      exit(-1);
    }
    c1->Print((m_outputDir+"efficienciesRelative.pdf").c_str(), "pdf");  
  }

  if(mode == "significance")
  {
    hs->GetYaxis()->SetTitle("S / #sqrt{B}");
    
    struct stat st;
    if(stat(m_outputDir.c_str(), &st) != 0)
    {
      std::cout << ">>>plotUtils::Error accessing directory " << m_outputDir << std::endl;
      exit(-1);
    }
    c1->Print((m_outputDir+"significance.pdf").c_str(), "pdf");
  }  
  
  
  m_xAxisRange = false;
  m_xAxisTitle = false;
  m_yAxisRange = false;
  m_yAxisTitle = false;
  
  delete c1;
  delete hs;
  
  
  
  c1 = new TCanvas();
  c1 -> cd();
  c1 -> SetGridx();
  c1 -> SetGridy();
  c1 -> SetLogy();
  
  stepHisto -> GetXaxis() -> SetTitleSize(0.04);
  stepHisto -> GetXaxis() -> SetLabelSize(0.03);
  stepHisto -> GetXaxis() -> SetTitleOffset(1.25);

  stepHisto -> GetYaxis() -> SetTitleSize(0.04);
  stepHisto -> GetYaxis() -> SetLabelSize(0.03);
  stepHisto -> GetYaxis() -> SetTitleOffset(1.50);
  
  stepHisto -> SetLineWidth(3);
  
  stepHisto -> Draw();

  struct stat st;
  if(stat(m_outputDir.c_str(), &st) != 0)
  {
    std::cout << ">>>plotUtils::Error accessing directory " << m_outputDir << std::endl;
    exit(-1);
  }
  c1->Print( (m_outputDir+mode+"_step.pdf").c_str(), "pdf");
  delete c1;
  
  
  
  // close root files
  if(outFile)
    outFile -> close();
  
  for(unsigned int i = 0; i < rootFiles.size(); ++i)
  {
    rootFiles.at(i) -> Close();
    delete rootFiles.at(i);
  }
  
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



void drawTStack::SetDrawLegend(const bool& drawLegend)
{
  m_drawLegend = drawLegend;
}

void drawTStack::SetXLegend(const double& xLow, const double& xHigh)
{
  m_xLowLegend = xLow;
  m_xHighLegend = xHigh;
}

void drawTStack::SetYLegend(const double& yLow, const double& yHigh)
{
  m_yLowLegend = yLow;
  m_yHighLegend = yHigh;
}