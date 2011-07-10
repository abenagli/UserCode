#include "TF1.h"
#include "plotUtils.h"
#include "setTDRStyle.h"






drawTStack::drawTStack(const std::string& inputDir,
                       const std::string& listFileName,
                       const std::string& baseRootFileName,
                       const std::string& outputDir,
                       const std::string& imgFormat):
 m_inputDir(inputDir),
 m_listFileName(listFileName),
 m_baseRootFileName(baseRootFileName),
 m_outputDir(outputDir),
 m_imgFormat(imgFormat),
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
 m_unit(""),
 m_drawLegend(true),
 m_xLowLegend(0.76),
 m_yLowLegend(0.78),
 m_xHighLegend(0.99),
 m_yHighLegend(0.99),
 c1(NULL),
 c2(NULL)
{
  //----------------------------------------------------------------------
  // drawTStack::read the file with the list of samples and cross sections
  //----------------------------------------------------------------------
  
  std::string listFullFileName = inputDir+listFileName;
  std::ifstream listFile( listFullFileName.c_str() );
  if(!listFile.is_open())
  {
    std::cout << "\n>>>plotUtils::drawTStack::Error opening file " << listFullFileName;
    exit(-1);
  }
  else
    std::cout << "\n>>>plotUtils::drawTStack::Opening file " << listFullFileName;
  
  
  std::cout << std::endl;
  
  
  // drawTStack::loop over list of samples
  while(!listFile.eof())
  {
    std::string sample;
    std::string sumName;
    int color;
    int linestyle;
    int fillstyle;
    int dataFlag;
    double mH;
    double crossSection;
    double scaleFactor;
    std::string jetAlgorithm;
    
    listFile >> sample >> sumName >> color >> linestyle >> fillstyle >> dataFlag >> mH >> crossSection >> scaleFactor >> jetAlgorithm;

    if(sample.size() == 0)
      continue;
    if(sample.at(0) == '#')
      continue;
    
    std::cout << std::fixed << std::setprecision(1) << std::setw(90)
              << sample << " "
              << std::setw(20)
              << sumName << " "
              << std::setw(3)
              << color << " "
              << std::setw(3)
              << linestyle << " "
              << std::setw(4)
              << fillstyle << " "
              << std::setw(3)
              << dataFlag << " "
              << std::setw(6)
              << mH << " "
              << std::setw(10)
              << crossSection << " "
              << scaleFactor << " "
              << jetAlgorithm << " "
              << std::endl;
    
    std::pair<std::string, std::string> dummyPair(sample, sumName);
    m_list.push_back(dummyPair);
    m_color[sample] = color;
    m_linestyle[sample] = linestyle;
    m_fillstyle[sample] = fillstyle;
    m_dataFlag[sample] = dataFlag;
    m_mH[sample] = mH;
    m_crossSection[sample] = crossSection*scaleFactor;
    
    std::pair<std::string, std::string> dummyPair2(sample, jetAlgorithm);
    m_jetAlgorithm.push_back(dummyPair2);
  }
  
  listFile.close();
  std::cout << ">>>plotUtils::drawTStack::Read " << m_list.size() << " samples" << std::endl;
  std::cout << ">>>plotUtils::drawTStack::Closing file " << listFullFileName << "\n" << std::endl;
}






drawTStack::~drawTStack()
{
}






void drawTStack::Draw(std::vector<std::string>& variableNames, const std::string& histoName,
                      const std::string& mode,
                      const float& lumi, const int& step,
                      const int& nBins,
                      const bool& PURescale,
                      std::vector<std::string>* cut)
{ 
  std::cout << "\n>>>plotUtils::Draw::Drawing histogram " << histoName;
  
  
  
  //------------------------------------------------
  // Draw::define the map with summed cross sections
  //------------------------------------------------
  
  std::map<std::string, double> crossSection_summed;
  std::map<std::string, bool> isFirstSample_summed;
  std::map<std::string, int> color_summed;
  std::map<std::string, int> linestyle_summed;
  std::map<std::string, int> fillstyle_summed;
  std::map<std::string, int> dataFlag_summed;
  std::map<std::string, double> mH_summed;
  std::map<std::string, TH1F*> histo_summed;
  
  // Draw::initialize summed vectors
  for(std::vector<std::pair<std::string, std::string> >::const_iterator vecIt = m_list.begin();
      vecIt != m_list.end(); ++vecIt)
  {
    crossSection_summed[vecIt->second] = 0.;
    isFirstSample_summed[vecIt->second] = true;
    color_summed[vecIt->second] = -1;
    linestyle_summed[vecIt->second] = -1;
    fillstyle_summed[vecIt->second] = -1;
    dataFlag_summed[vecIt->second] = -1;
    mH_summed[vecIt->second] = -1;
    histo_summed[vecIt->second] = NULL;
  }
  
  
  
  //--------------------------------
  // Draw::loop over all the samples
  //--------------------------------
  c1 = new TCanvas();
  c1 -> cd();
  c1 -> SetGridx();
  c1 -> SetGridy();
  
   
  int binMin = -1;
  int binMax = -1;
  int i = 0;
  std::vector<TFile*> rootFiles;
  std::vector<TTree*> trees;
  for(std::vector<std::pair<std::string, std::string> >::const_iterator vecIt = m_list.begin();
      vecIt != m_list.end(); ++vecIt)
  {
    // Draw::open root file
    std::string fullRootFileName = m_inputDir+vecIt->first+"/"+m_baseRootFileName+"_"+m_jetAlgorithm.at(i).second+".root";
    //std::cout << "opening file: " << fullRootFileName << std::endl;//
    rootFiles.push_back(new TFile(fullRootFileName.c_str(), "READ"));
    if(!(rootFiles.at(i))->IsOpen()) exit(-1);
    
    
    // Draw::get tree
    TTree* tree;
    char treeName[50];
    sprintf(treeName, "ntu_%d", step);
    rootFiles.at(i) -> GetObject(treeName, tree);
    trees.push_back(tree);
    
    
    // Draw:: dump tree into histogram
    TH1F* histo = new TH1F(histoName.c_str(), "", nBins, m_xRangeMin, m_xRangeMax);    
    for(unsigned int jj = 0; jj < variableNames.size(); ++jj)
    {
      //std::cout << "Draw::Dumping tree variable " << (variableNames.at(jj)+">>"+histoName).c_str() << std::endl;//
      
      if(cut == NULL)
      {
        if(PURescale)
          tree -> Draw( (variableNames.at(jj)+" >>+ "+histoName).c_str(), "PURescaleFactor(PUit_n)");
        else
          tree -> Draw( (variableNames.at(jj)+" >>+ "+histoName).c_str());
       }
      
      else
      {
        if(PURescale)
          tree -> Draw( (variableNames.at(jj)+" >>+ "+histoName).c_str(), ("PURescaleFactor(PUit_n) * "+cut->at(jj)).c_str() );
        else
          tree -> Draw( (variableNames.at(jj)+" >>+ "+histoName).c_str(), (cut->at(jj)).c_str() );
      }
    }
    
    
    // Draw::get event histogram
    std::string eventsHistoName = "events";
    //std::cout << "getting histogram " << eventsHistoName << std::endl;//
    
    TH1F* eventsHisto = NULL;
    rootFiles.at(i) -> GetObject(eventsHistoName.c_str(), eventsHisto);
    if(eventsHisto == NULL)
    {
      std::cout << ">>>plotUtils::Error in getting object " << eventsHistoName;
      exit(-1);
    }
    
    
    // Draw::scale histograms normalizing to lumi (1. pb^-1)
    // Draw::if data do not apply any scale factor
    histo -> Sumw2();
    color_summed[vecIt->second] = m_color[vecIt->first];
    linestyle_summed[vecIt->second] = m_linestyle[vecIt->first];
    fillstyle_summed[vecIt->second] = m_fillstyle[vecIt->first];
    dataFlag_summed[vecIt->second] = m_dataFlag[vecIt->first];
    mH_summed[vecIt->second] = m_mH[vecIt->first];
    
    if( (histo->GetEntries() > 0.) && (m_dataFlag[vecIt->first] != 1) )
    {
      histo -> Scale(m_crossSection[vecIt->first]/eventsHisto->GetBinContent(1));
      crossSection_summed[vecIt->second] += m_crossSection[vecIt->first];
    }
    
    
    
    // Draw::sum histograms normalized to lumi (1. pb^-1)
    // Draw::if data, just sum histograms w/o normalization
    if( isFirstSample_summed[vecIt->second] == false )
    {
      histo_summed[vecIt->second] -> Add(histo);
    }
    if( isFirstSample_summed[vecIt->second] == true )
    {
      histo_summed[vecIt->second] = (TH1F*)(histo -> Clone());
      isFirstSample_summed[vecIt->second] = false;
      
      binMin = histo -> FindBin(m_xRangeMin);
      binMax = histo -> FindBin(m_xRangeMax);
    }
    
    
    
    ++i;
  } // Draw::end loop over all the samples
  
  
  
  
  
  
  //---------------------------------------------
  // Draw::Compute normalization factor
  //---------------------------------------------

  TH1F* globalGlobalHisto = NULL;
  TH1F* dataGlobalGlobalHisto = NULL;
  
  double globalGlobalIntegral = 0.;
  double dataGlobalGlobalIntegral = 0.;
  
  bool isFirstSample = true;
  bool isFirstSample_data = true;
  
  // loop over summed samples
  for(std::map<std::string, double>::const_iterator mapIt = crossSection_summed.begin();
      mapIt != crossSection_summed.end(); ++mapIt)
  {    
    TH1F* globalHisto = histo_summed[mapIt->first];
    
    // MC
    if( (isFirstSample == false) && (dataFlag_summed[mapIt->first] != 1) )
    {
      globalGlobalHisto -> Add(globalHisto);
    }
    if( (isFirstSample == true) && (dataFlag_summed[mapIt->first] != 1) )
    {
      globalGlobalHisto = (TH1F*)(globalHisto -> Clone());
      isFirstSample = false;
    }
    
    // DATA
    if( (isFirstSample_data == false) && (dataFlag_summed[mapIt->first] == 1) )
    {
      dataGlobalGlobalHisto -> Add(globalHisto);
    }
    if( (isFirstSample_data == true) && (dataFlag_summed[mapIt->first] == 1) )
    {
      dataGlobalGlobalHisto = (TH1F*)(globalHisto -> Clone());
      isFirstSample_data = false;
    }
    
  } // loop over summed samples
  
  
  if( mode == "sameAreaStack" )
  {
    globalGlobalIntegral = globalGlobalHisto -> Integral(1, globalGlobalHisto->GetNbinsX());
    dataGlobalGlobalIntegral = dataGlobalGlobalHisto -> Integral(1, dataGlobalGlobalHisto->GetNbinsX());
  } 
  // Draw::End compute normalization factor
  
  
  
  
  
  
  //---------------------------------------------
  // Draw::Define the final Stack/Histograms
  //---------------------------------------------
  
  THStack* hs = new THStack("hs", "hs");
  THStack* hs_signal = new THStack("hs_signal", "hs_signal");
  int nHists = 0;
  
  globalGlobalHisto = NULL;
  dataGlobalGlobalHisto = NULL;
  
  double globalMaximum = 0.;
  double globalMinimum = 999999999999.;
  
  char binWidthBuffer[50];
  
  TLegend legend(m_xLowLegend, m_yLowLegend, m_xHighLegend, m_yHighLegend);
  legend.SetFillColor(kWhite);
  legend.SetFillStyle(4000);
  
  char lumiBuffer[50];
  sprintf(lumiBuffer, "CMS Preliminary 2011");
  
  char lumiBuffer2[50];
  sprintf(lumiBuffer2, "#sqrt{s}=7 TeV   L=%.1f pb^{-1}", lumi);

  TLatex *latex = new TLatex(0.76, 0.91, lumiBuffer); 
  TLatex *latex2 = NULL;
  if( mode == "eventsScaled" )
    latex2 = new TLatex(0.76, 0.86, lumiBuffer2);
  else
    latex2 = new TLatex(0.76, 0.86, "norm. to same area");
  
  latex->SetNDC();
  latex->SetTextFont(42);
  latex->SetTextSize(0.03);
  latex2->SetNDC();
  latex2->SetTextFont(42);
  latex2->SetTextSize(0.03);
  
  
  
  // loop over summed histograms
  i = 0;
  isFirstSample = true;
  for(std::map<std::string, double>::const_iterator mapIt = crossSection_summed.begin();
      mapIt != crossSection_summed.end(); ++mapIt)
  {    
    TH1F* globalHisto = histo_summed[mapIt->first];
    if(globalHisto -> GetEntries() == 0) continue;
    
    
    
    if(m_xAxisRange)
      globalHisto->GetXaxis()->SetRangeUser(m_xRangeMin, m_xRangeMax);
    
    sprintf(binWidthBuffer, "%.2e", globalHisto->GetBinWidth(1));
    
    
    
    if( dataFlag_summed[mapIt->first] == 1 )
    {
      globalHisto -> SetMarkerStyle(20);
      legend.AddEntry(globalHisto, (mapIt->first).c_str(), "P");
    }
    
    
    
    
    
    
    if( (mode == "eventsScaled") && (dataFlag_summed[mapIt->first] != 1) )
    {
      globalHisto -> Scale(1. * lumi);
      //globalHisto -> SetLineColor(color_summed[mapIt->first]);
      globalHisto -> SetLineColor(kBlack);
      globalHisto -> SetFillColor(color_summed[mapIt->first]);
      globalHisto -> SetLineStyle(linestyle_summed[mapIt->first]);
      globalHisto -> SetFillStyle(fillstyle_summed[mapIt->first]);
      //globalHisto -> SetLineWidth(2);
      globalHisto -> SetLineWidth(1);
      
      if(mH_summed[mapIt->first] <= 0.)
        hs -> Add(globalHisto);
      else
        hs_signal -> Add(globalHisto);
      ++nHists;
      legend.AddEntry(globalHisto, (mapIt->first).c_str(), "F");
    }
    
    
    
    if( (mode == "sameAreaNoStack") )
    {
      globalHisto -> Scale(1./globalHisto->Integral(1, globalHisto->GetNbinsX()));
      if( dataFlag_summed[mapIt->first] != 1 )
      {
        globalHisto -> SetLineColor(color_summed[mapIt->first]);
        globalHisto -> SetLineStyle(linestyle_summed[mapIt->first]);
        globalHisto -> SetLineWidth(4);
      }
      
      if(globalHisto->GetMaximum() > globalMaximum)
        globalMaximum = globalHisto -> GetMaximum();
      if(MyGetMinimum(globalHisto, 1.E-15, binMin, binMax) < globalMinimum)
        globalMinimum = MyGetMinimum(globalHisto, 1.E-15, binMin, binMax);
      
      if( dataFlag_summed[mapIt->first] != 1 ) 
      {
        hs -> Add(globalHisto);      
        ++nHists;
       legend.AddEntry(globalHisto, (mapIt->first).c_str(), "L");      
      }
    }
    
    
    
    if( (mode == "integral") )
    {
      TH1F* integralHisto = (TH1F*)(globalHisto->Clone());
      
      for(int bin = 1; bin <= globalHisto->GetNbinsX(); ++bin)
        integralHisto -> SetBinContent(bin,globalHisto->Integral(1,bin)/globalHisto->Integral()); 
      
      integralHisto -> SetLineColor(color_summed[mapIt->first]);
      integralHisto -> SetLineStyle(linestyle_summed[mapIt->first]);
      integralHisto -> SetLineWidth(4);
      
      globalMaximum = 1.;
      if(MyGetMinimum(integralHisto, 1.E-15, binMin, binMax) < globalMinimum)
        globalMinimum = MyGetMinimum(integralHisto, 1.E-15, binMin, binMax);
      
      if( dataFlag_summed[mapIt->first] != 1 ) 
      {
        hs -> Add(integralHisto);      
        ++nHists;
       legend.AddEntry(integralHisto, (mapIt->first).c_str(), "L");      
      }
    }
    
    
    
    
    if( (mode == "sameAreaStack") && (dataFlag_summed[mapIt->first] == 1) )
    {
      globalHisto -> Scale(dataGlobalGlobalIntegral/globalHisto->Integral(1, globalHisto->GetNbinsX()));
    }
    
    if( (mode == "sameAreaStack") && (dataFlag_summed[mapIt->first] != 1) )
    {
      globalHisto -> Scale(dataGlobalGlobalIntegral/globalGlobalIntegral);
      //globalHisto -> SetLineColor(color_summed[mapIt->first]);
      globalHisto -> SetLineColor(kBlack);
      globalHisto -> SetFillColor(color_summed[mapIt->first]);
      globalHisto -> SetLineStyle(linestyle_summed[mapIt->first]);
      globalHisto -> SetFillStyle(fillstyle_summed[mapIt->first]);
      //globalHisto -> SetLineWidth(2);
      globalHisto -> SetLineWidth(1);
      
      if(mH_summed[mapIt->first] <= 0.)
        hs -> Add(globalHisto);      
      else
        hs_signal -> Add(globalHisto);      
      ++nHists;
      legend.AddEntry(globalHisto, (mapIt->first).c_str(), "F");      
    }
    
    
    
    // global minimum
   if(MyGetMinimum(globalHisto, 1.E-15, binMin, binMax) < globalMinimum)
      globalMinimum = MyGetMinimum(globalHisto, 1.E-15, binMin, binMax);
    
    
    
    // MC
    if( (globalGlobalHisto != 0) && (dataFlag_summed[mapIt->first] != 1) )
      globalGlobalHisto -> Add(globalHisto);
    if( (globalGlobalHisto == 0) && (dataFlag_summed[mapIt->first] != 1) )
      globalGlobalHisto = (TH1F*)(globalHisto -> Clone());
    
    // DATA
    if( (dataGlobalGlobalHisto != 0) && (dataFlag_summed[mapIt->first] == 1) )
      dataGlobalGlobalHisto -> Add(globalHisto);
    if( (dataGlobalGlobalHisto == 0) && (dataFlag_summed[mapIt->first] == 1) )
      dataGlobalGlobalHisto = (TH1F*)(globalHisto -> Clone());
    
    
    
    ++i;
  }  // loop over summed histograms
  
  
  
  if( (globalGlobalHisto != NULL) && (mode != "sameAreaNoStack") && (mode != "integral") )
  {
    if(globalGlobalHisto->GetMaximum() > globalMaximum)
      globalMaximum = globalGlobalHisto -> GetMaximum();
    //if(MyGetMinimum(globalGlobalHisto, 1.E-15, binMin, binMax) < globalMinimum)
    //  globalMinimum = MyGetMinimum(globalGlobalHisto, 1.E-15, binMin, binMax);
  }
  
  if( (dataGlobalGlobalHisto != NULL) && (mode != "sameAreaNoStack") && (mode != "integral") )
  {
    if(dataGlobalGlobalHisto->GetMaximum() > globalMaximum)
      globalMaximum = dataGlobalGlobalHisto -> GetMaximum();
    if(MyGetMinimum(dataGlobalGlobalHisto, 1.E-15, binMin, binMax) < globalMinimum)
      globalMinimum = MyGetMinimum(dataGlobalGlobalHisto, 1.E-15, binMin, binMax);
  } 
  
  
  
  
  
  
  // if no histograms had entries, return
  if(i == 0)
  {
    std::cout << ">>>plotUtils::Draw::Error, histograms empty";

    // close root files
    for(unsigned int i = 0; i < rootFiles.size(); ++i)
    {
      rootFiles.at(i) -> Close();
      delete rootFiles.at(i);
    }
    return;
  }
  
  
  
  
  
  
  // draw the stack and save file
  if( (nHists == 0) && (dataGlobalGlobalHisto != NULL) )
    hs -> Add(dataGlobalGlobalHisto);
  
  
  if( (mode == "eventsScaled") || (mode == "sameAreaStack") )
  {
    delete c1;
    c1 = new TCanvas("c1","c1",800,800);
    c1 -> cd();
        
    TPad* p1 = new TPad("p1","p1",0., 0.25, 1., 1.);
    TPad* p2 = new TPad("p2","p2",0., 0., 1., 0.25);
    p1 -> Draw();
    p2 -> Draw();



    p1 -> cd();
    p1 -> SetGridx();
    p1 -> SetGridy();
    
    hs -> Draw("HISTO");
    hs_signal -> Draw("HISTO,same");
    DrawTStackError(hs);
    if(dataGlobalGlobalHisto != NULL)
    {
      for(int bin = 1; bin <= dataGlobalGlobalHisto->GetNbinsX(); ++bin)
        dataGlobalGlobalHisto -> SetBinError(bin,0.);
      dataGlobalGlobalHisto -> Draw("P,same");
    }
    
    hs->GetYaxis()->SetTitle(("events / " + std::string(binWidthBuffer)+" "+m_unit).c_str());
    
    if(dataGlobalGlobalHisto != NULL)
    {
      TH1F* ratioHisto = (TH1F*)(dataGlobalGlobalHisto -> Clone());
      ratioHisto -> GetYaxis() -> SetTitleSize(0.09);
      ratioHisto -> GetYaxis() -> SetTitleOffset(1.0);
      ratioHisto -> GetYaxis() -> SetTitle("data/MC");
      
      TObjArray* histos = hs -> GetStack();
      int nHistos = histos -> GetEntries();
      TH1F* lastHisto = (TH1F*)(histos->At(nHistos-1))->Clone();
      
      int nPoints = lastHisto->GetNbinsX();
      TGraph* ratioGraph1s = new TGraph(2*nPoints);
      TGraph* ratioGraph2s = new TGraph(2*nPoints);

      for(int bin = 1; bin <= dataGlobalGlobalHisto->GetNbinsX(); ++bin)
      {
        if(globalGlobalHisto->GetBinContent(bin) == 0.) continue;
        ratioHisto -> SetBinContent(bin, 1.*dataGlobalGlobalHisto->GetBinContent(bin)/globalGlobalHisto->GetBinContent(bin));
        ratioHisto -> SetBinError(bin, 1.*dataGlobalGlobalHisto->GetBinError(bin)/globalGlobalHisto->GetBinContent(bin));
      }
      
      int point = 0;
      for(int bin = 1; bin <= lastHisto->GetNbinsX(); ++bin)
      {
        float binCenter = lastHisto->GetBinCenter(bin);
        float binError = sqrt(lastHisto->GetBinError(bin)*lastHisto->GetBinError(bin) + 
                              lastHisto->GetBinContent(bin));
        float binErrorM;
        float binErrorP;
        float binError2M;
        float binError2P;
        if( lastHisto->GetBinContent(bin) != 0 )
        {
          binErrorM = 1. - binError/lastHisto->GetBinContent(bin);
          binErrorP = 1. + binError/lastHisto->GetBinContent(bin);
          binError2M = 1. - 2.*binError/lastHisto->GetBinContent(bin);
          binError2P = 1. + 2.*binError/lastHisto->GetBinContent(bin);
        }
        else
        {
          binErrorM = 1.;
          binErrorP = 1.;
          binError2M = 1.;
          binError2P = 1.;
        }
        ratioGraph1s -> SetPoint(point,binCenter,binErrorM);
        ratioGraph1s -> SetPoint(2*nPoints-point-1,binCenter,binErrorP);
        ratioGraph2s -> SetPoint(point,binCenter,binError2M);
        ratioGraph2s -> SetPoint(2*nPoints-point-1,binCenter,binError2P);
        
        ++point;
      }
      
      
      p2 -> cd();
      p2 -> SetGridx();
      p2 -> SetGridy();
      
      ratioHisto -> GetYaxis() -> SetRangeUser(0., 2.);
      ratioHisto -> Draw("P");
      
      ratioGraph2s -> SetFillColor(kYellow);
      ratioGraph2s -> SetLineColor(kYellow);
      ratioGraph2s -> Draw("F,same");
      
      ratioGraph1s -> SetFillColor(kGreen);
      ratioGraph1s -> SetLineColor(kGreen);
      ratioGraph1s -> Draw("F,same");
      
      ratioHisto -> Draw("P,same");
        
      TF1* line = new TF1("line", "1.", -1000000., 1000000.);
      line -> SetLineWidth(2);
      line -> SetLineColor(kRed);
      line -> Draw("same");
    }
    p1 -> cd();
    
  }

  if( mode == "sameAreaNoStack" )
  {
    hs -> Draw("nostack,HISTO");
    if(dataGlobalGlobalHisto != NULL)
    {
      dataGlobalGlobalHisto -> Scale(1./dataGlobalGlobalHisto->Integral(1, dataGlobalGlobalHisto->GetNbinsX()));
      dataGlobalGlobalHisto -> Draw("P,same");
    }
    
    hs->GetYaxis()->SetTitle(("event fraction / " + std::string(binWidthBuffer)+" "+m_unit).c_str());
  }
  
  
  if( mode == "integral" )
  {
    hs -> Draw("nostack,HISTO");
    hs->GetYaxis()->SetTitle(("event integral / " + std::string(binWidthBuffer)+" "+m_unit).c_str());
  }
  
  
  
  if(m_drawLegend == true)
  {
    legend.SetTextFont(42);  
    legend.SetTextSize(0.025);
    legend.Draw("same");
    latex->Draw("same");
    latex2->Draw("same");
  }
  
  
  
  // set x-axis properties
  std::string fullTitle = histoName;
  char stepTitle[50];
  sprintf(stepTitle, "_%d", step);
  //fullTitle += std::string(stepTitle);
  hs->GetXaxis()->SetTitle((fullTitle+" "+m_unit).c_str());
  if(m_xAxisTitle) hs->GetXaxis()->SetTitle(m_xTitle.c_str());
  hs -> GetXaxis() -> SetTitleSize(0.04);
  hs -> GetXaxis() -> SetLabelSize(0.03);
  hs -> GetXaxis() -> SetTitleOffset(1.25);
  if(m_xAxisRange) hs->GetXaxis()->SetRangeUser(m_xRangeMin, m_xRangeMax);
  
  
  // set y-axis properties
  if(m_yAxisTitle) hs->GetYaxis()->SetTitle(m_yTitle.c_str());    
  hs -> GetYaxis() -> SetTitleSize(0.04);
  hs -> GetYaxis() -> SetLabelSize(0.03);
  hs -> GetYaxis() -> SetTitleOffset(1.50);
  
  hs->SetMinimum(0.);
  hs->SetMaximum(globalMaximum+0.1*globalMaximum);
  
  if(m_yAxisRange)
  {
    hs->SetMinimum(m_yRangeMin);
    hs->SetMaximum(m_yRangeMax);
  }
  
  
  
  // write plots
  struct stat st;
  if(stat(m_outputDir.c_str(), &st) != 0)
  {
    std::cout << ">>>plotUtils::Error accessing directory " << m_outputDir;
    exit(-1);
  }
  c1->Print((m_outputDir+"lin_"+fullTitle+"."+m_imgFormat).c_str(), m_imgFormat.c_str());
  
  
  
  
  
  
  c2 = new TCanvas("c2", "c2");
  c2 -> cd();
  c2 -> SetGridx();
  c2 -> SetGridy();
  c2 -> SetLogy();
  
  if( (mode == "eventsScaled") || (mode == "sameAreaStack") )
  {
    delete c2;
    c2 = new TCanvas("c2","c2",800,800);
    c2 -> cd();
        
    TPad* p3 = new TPad("p3","p3",0., 0.25, 1., 1.);
    TPad* p4 = new TPad("p4","p4",0., 0., 1., 0.25);
    p3 -> Draw();
    p4 -> Draw();



    p3 -> cd();
    p3 -> SetGridx();
    p3 -> SetGridy();
    p3 -> SetLogy();
    
    hs -> Draw("HISTO");
    hs_signal -> Draw("HISTO,same");
    DrawTStackError(hs);
    if(dataGlobalGlobalHisto != NULL)
    {
      for(int bin = 1; bin <= dataGlobalGlobalHisto->GetNbinsX(); ++bin)
        dataGlobalGlobalHisto -> SetBinError(bin,0.);
      dataGlobalGlobalHisto -> Draw("P,same");
    }
    
    if(dataGlobalGlobalHisto != NULL)
    {    
      TH1F* ratioHisto = (TH1F*)(dataGlobalGlobalHisto -> Clone());
      ratioHisto -> GetYaxis() -> SetTitleSize(0.09);
      ratioHisto -> GetYaxis() -> SetTitleOffset(1.0);
      ratioHisto -> GetYaxis() -> SetTitle("data/MC");
      
      TObjArray* histos = hs -> GetStack();
      int nHistos = histos -> GetEntries();
      TH1F* lastHisto = (TH1F*)(histos->At(nHistos-1))->Clone();
      
      int nPoints = lastHisto->GetNbinsX();
      TGraph* ratioGraph1s = new TGraph(2*nPoints);
      TGraph* ratioGraph2s = new TGraph(2*nPoints);

      for(int bin = 1; bin <= dataGlobalGlobalHisto->GetNbinsX(); ++bin)
      {
        if(globalGlobalHisto->GetBinContent(bin) == 0.) continue;
        ratioHisto -> SetBinContent(bin, 1.*dataGlobalGlobalHisto->GetBinContent(bin)/globalGlobalHisto->GetBinContent(bin));
        ratioHisto -> SetBinError(bin, 1.*dataGlobalGlobalHisto->GetBinError(bin)/globalGlobalHisto->GetBinContent(bin));
      }
      
      int point = 0;
      for(int bin = 1; bin <= lastHisto->GetNbinsX(); ++bin)
      {
        float binCenter = lastHisto->GetBinCenter(bin);
	float binError = sqrt(lastHisto->GetBinError(bin)*lastHisto->GetBinError(bin) +
                              lastHisto->GetBinContent(bin));
        float binErrorM;
        float binErrorP;
        float binError2M;
        float binError2P;
        if( lastHisto->GetBinContent(bin) != 0 )
        {
          binErrorM = 1. - binError/lastHisto->GetBinContent(bin);
          binErrorP = 1. + binError/lastHisto->GetBinContent(bin);
          binError2M = 1. - 2.*binError/lastHisto->GetBinContent(bin);
          binError2P = 1. + 2.*binError/lastHisto->GetBinContent(bin);
        }
        else
        {
          binErrorM = 1.;
          binErrorP = 1.;
          binError2M = 1.;
          binError2P = 1.;
        }
        ratioGraph1s -> SetPoint(point,binCenter,binErrorM);
        ratioGraph1s -> SetPoint(2*nPoints-point-1,binCenter,binErrorP);
        ratioGraph2s -> SetPoint(point,binCenter,binError2M);
        ratioGraph2s -> SetPoint(2*nPoints-point-1,binCenter,binError2P);
        
        ++point;
      }
      
      p4 -> cd();
      p4 -> SetGridx();
      p4 -> SetGridy();
      
      ratioHisto -> GetYaxis() -> SetRangeUser(0., 2.);
      ratioHisto -> Draw("P");
      
      ratioGraph2s -> SetFillColor(kYellow);
      ratioGraph2s -> SetLineColor(kYellow);
      ratioGraph2s -> Draw("F,same");
      
      ratioGraph1s -> SetFillColor(kGreen);
      ratioGraph1s -> SetLineColor(kGreen);
      ratioGraph1s -> Draw("F,same");
      
      ratioHisto -> Draw("P,same");
      
      TF1* line = new TF1("line", "1.", -1000000., 1000000.);
      line -> SetLineWidth(2);
      line -> SetLineColor(kRed);
      line -> Draw("same");
    }
    p3 -> cd();
    
  }
  
  if( mode == "sameAreaNoStack" )
  {
    hs -> Draw("nostack,HISTO");
    if(dataGlobalGlobalHisto != NULL)
    {
      dataGlobalGlobalHisto -> Scale(1./dataGlobalGlobalHisto->Integral(1, dataGlobalGlobalHisto->GetNbinsX()));
      dataGlobalGlobalHisto -> Draw("P,same");
    }
    
    hs->GetYaxis()->SetTitle(("event fraction / " + std::string(binWidthBuffer)+" "+m_unit).c_str());
  }
  
  
  
  if( mode == "integral" )
  {
    hs -> Draw("nostack,HISTO");
    
    hs->GetYaxis()->SetTitle(("event fraction / " + std::string(binWidthBuffer)+" "+m_unit).c_str());
  }
  
  
  
  if(m_drawLegend == true)
  {
    legend.SetTextFont(42);  
    legend.SetTextSize(0.025);
    legend.Draw("same");
    latex->Draw("same");
    latex2->Draw("same");
  }
  
  
  
  hs->SetMinimum(pow(10., log10(globalMinimum) - 0.1));
  hs->SetMaximum(pow(10., log10(globalMaximum) + 0.1));
  
  if(m_yAxisRange)
  {
    hs->SetMinimum(m_yRangeMin);
    hs->SetMaximum(m_yRangeMax);
  }
  
  
  
  // write plots
  if(stat(m_outputDir.c_str(), &st) != 0)
  {
    std::cout << ">>>plotUtils::Error accessing directory " << m_outputDir;
    exit(-1);
  }
  //c2->Print(("log_"+m_outputDir+fullTitle+".eps").c_str(), "eps");
  c2->Print((m_outputDir+"log_"+fullTitle+"."+m_imgFormat).c_str(), m_imgFormat.c_str());
  
  
  
  
  
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
  
  delete hs;
  delete c1;
  delete c2;
}












void drawTStack::DrawEvents(const std::string& mode, const float& lumi, const int& step, const bool& logy)
{ 
  std::cout << "\n>>>plotUtils::Drawing " << mode;
  
  
  
  THStack* hs = new THStack("hs", "hs");
  THStack* hs_signal = new THStack("hs_signal", "hs_signal");
  int nHists = 0;
  TLegend legend(m_xLowLegend, m_yLowLegend, m_xHighLegend, m_yHighLegend);
  legend.SetFillColor(kWhite);
  legend.SetFillStyle(4000);
  
  char lumiBuffer[50];
  sprintf(lumiBuffer, "CMS Preliminary 2011");

  char lumiBuffer2[100];
  sprintf(lumiBuffer2, "#sqrt{s}=7 TeV   L=%.1f pb^{-1}", lumi);
  
  TLatex* latex = new TLatex(0.76, 0.91, lumiBuffer);
  TLatex* latex2 = new TLatex(0.76, 0.88, lumiBuffer2);
  latex->SetNDC();
  latex->SetTextFont(42);
  latex->SetTextSize(0.03);
  latex2->SetNDC();
  latex2->SetTextFont(42);
  latex2->SetTextSize(0.03);
  
  double globalMaximum = 0.;
  double globalMinimum = 1.;
  
  
  
  //---------------------------------------------
  // define the map with summed cross sections
  //---------------------------------------------
 
  std::map<std::string, double> crossSection_summed;
  std::map<std::string, bool> isFirstSample_summed;
  std::map<std::string, int> color_summed;
  std::map<std::string, int> linestyle_summed;
  std::map<std::string, int> fillstyle_summed;
  std::map<std::string, double> mH_summed;
  std::map<std::string, int> dataFlag_summed;
  std::map<std::string, bool> isSignal_summed;
  std::map<std::string, TH1F*> histo_summed;

  for(std::vector<std::pair<std::string, std::string> >::const_iterator vecIt = m_list.begin();
      vecIt != m_list.end(); ++vecIt)
  {
    histo_summed[vecIt->second] = NULL;
    isFirstSample_summed[vecIt->second] = true;
    color_summed[vecIt->second] = -1;
    linestyle_summed[vecIt->second] = -1;
    fillstyle_summed[vecIt->second] = -1;
    mH_summed[vecIt->second] = -1;
    dataFlag_summed[vecIt->second] = -1;
    isSignal_summed[vecIt->second] = false;
  }
  
  
  
  //---------------------------------------------
  // loop over all the samples and fill the stack
  //---------------------------------------------

  std::vector<TFile*> rootFiles;
  bool isFirstSample_data = true;
  TH1F* dataHisto = NULL;
  TH1F* globalGlobalHisto = NULL;
  int i = 0;
  int binMin = -1;
  int binMax = -1;
  for(std::vector<std::pair<std::string, std::string> >::const_iterator vecIt = m_list.begin();
      vecIt != m_list.end(); ++vecIt)
  {
    // open root file
    std::string fullRootFileName = m_inputDir+vecIt->first+"/"+m_baseRootFileName+"_"+m_jetAlgorithm.at(i).second+".root";
    //std::cout << "opening file: " << fullRootFileName << std::endl;
    rootFiles.push_back(new TFile(fullRootFileName.c_str(), "READ"));
    
    
    // get histogram
    std::string fullHistoName;
    if( mode == "events" )
      fullHistoName = "events";
    else
      fullHistoName = "events_PURescaled";      
    //std::cout << "getting histogram " << fullHistoName << std::endl;
    
    TH1F* histo = NULL;
    rootFiles.at(i) -> GetObject(fullHistoName.c_str(), histo);
    if(histo == NULL)
    {
      std::cout << ">>>plotUtils::Error in getting object " << fullHistoName;
      exit(-1);
    }
    
    if( mode == "eventsScaledStack" || mode == "eventsScaled" )
      histo -> Sumw2();
    
    
    
    color_summed[vecIt->second] = m_color[vecIt->first];
    linestyle_summed[vecIt->second] = m_linestyle[vecIt->first];
    fillstyle_summed[vecIt->second] = m_fillstyle[vecIt->first];
    mH_summed[vecIt->second] = m_mH[vecIt->first];
    dataFlag_summed[vecIt->second] = m_dataFlag[vecIt->first];
    crossSection_summed[vecIt->second] += m_crossSection[vecIt->first];
    
    if(m_mH[vecIt->first] > 0)
      isSignal_summed[vecIt->second] = true;
    
    
    if( (mode == "eventsScaled") ||
        (mode == "eventsScaledStack") ||
        (mode == "efficiencies") ||
        (mode == "efficienciesRelative") ||
        (mode == "significance") )
    {
      if( m_crossSection[vecIt->first] != 1. )
        histo -> Scale(lumi*m_crossSection[vecIt->first]/histo->GetBinContent(1));
     }
    
    
    
    // sum histograms
    if( isFirstSample_summed[vecIt->second] == false )
    {
      histo_summed[vecIt->second] -> Add(histo);
    }
    if( isFirstSample_summed[vecIt->second] == true )
    {
      histo_summed[vecIt->second] = (TH1F*)(histo -> Clone());
      isFirstSample_summed[vecIt->second] = false;
    }
    
    
    
    // save data histogram
    if( (isFirstSample_data == false) && (dataFlag_summed[vecIt->second] == 1) )
    {
      dataHisto -> Add(histo);
    }
    if( (isFirstSample_data == true) && (dataFlag_summed[vecIt->second] == 1) )
    {
      dataHisto = (TH1F*)(histo -> Clone());
      isFirstSample_data = false;
    }
    
    
    
    ++i;
  }
  
  
  
  std::ofstream* outFile = NULL;
  std::ofstream* outFile_CoLeMIB = NULL;

  if(mode == "eventsScaled")
  {
    outFile = new std::ofstream((m_outputDir+"eventsScaled.txt").c_str(), std::ios::out);
    outFile_CoLeMIB = new std::ofstream((m_outputDir+"events_CoLeMIB.csv").c_str(), std::ios::out);
  }
  if(mode == "eventsScaledStack")
    outFile = new std::ofstream((m_outputDir+"eventsScaledStack.txt").c_str(), std::ios::out);
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
  
  
  
  
  
  
  //----------------
  // data histograms
  //----------------
  
  if( dataHisto != NULL )
  {
    if(mode == "efficiencies")
    {
      float totalEvents = dataHisto -> GetBinContent(1);
      for(int bin = 1; bin <= dataHisto->GetNbinsX(); ++bin)
        dataHisto->SetBinContent(bin, 1.*dataHisto->GetBinContent(bin)/totalEvents);
    }
    
    if(mode == "efficienciesRelative")
    {
      std::map<int, float> totalEvents;
      totalEvents[0] = dataHisto->GetBinContent(1);
      for(int bin = 1; bin <= dataHisto->GetNbinsX(); ++bin)
        totalEvents[bin] = dataHisto->GetBinContent(bin);
      
      for(int bin = 1; bin <= dataHisto->GetNbinsX(); ++bin)
      {        
        if(totalEvents[bin-1] > 0.)
          dataHisto->SetBinContent(bin, 1.*dataHisto->GetBinContent(bin)/totalEvents[bin-1]);
        else
          dataHisto->SetBinContent(bin, 0.);
      }
    }
  }
  
  
  
  
  
  
  //-------------------------
  // loop over summed samples
  //-------------------------
  
  i = 0;
  std::map<int, float> nEventsScaled_sig;
  std::map<int, float> nEventsScaled_bkg;
  for(std::map<std::string, double>::const_iterator mapIt = crossSection_summed.begin();
      mapIt != crossSection_summed.end(); ++mapIt)
  {    
    TH1F* globalHisto = histo_summed[mapIt->first];
    
    
    
    if(mode == "efficiencies")
    {
      float totalEvents = globalHisto -> GetBinContent(1);
      for(int bin = 1; bin <= globalHisto->GetNbinsX(); ++bin)
        globalHisto->SetBinContent(bin, 1.*globalHisto->GetBinContent(bin)/totalEvents);
    }
    
    if(mode == "efficienciesRelative")
    {
      std::map<int, float> totalEvents;
      totalEvents[0] = globalHisto->GetBinContent(1);
      for(int bin = 1; bin <= globalHisto->GetNbinsX(); ++bin)
        totalEvents[bin] = globalHisto->GetBinContent(bin);
      
      for(int bin = 1; bin <= globalHisto->GetNbinsX(); ++bin)
      {        
        if(totalEvents[bin-1] > 0.)
          globalHisto->SetBinContent(bin, 1.*globalHisto->GetBinContent(bin)/totalEvents[bin-1]);
        else
          globalHisto->SetBinContent(bin, 0.);
      }
    }
    
    if(mode == "significance")
    {
      if(isSignal_summed[mapIt->first] == false) continue;
    
      TH1F* totalBkgHisto = NULL;
      for(std::map<std::string, double>::const_iterator mapIt2 = crossSection_summed.begin();
          mapIt2 != crossSection_summed.end(); ++mapIt2)
      {
        if(isSignal_summed[mapIt2->first] == true) continue;
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
    
    
    if( dataFlag_summed[mapIt->first] != 1 )
    {
      globalHisto -> SetLineColor(color_summed[mapIt->first]);
      globalHisto -> SetLineStyle(linestyle_summed[mapIt->first]);
      globalHisto -> SetLineWidth(4);
      globalHisto -> SetFillColor(color_summed[mapIt->first]);
      globalHisto -> SetFillStyle(0);
    }
    
    if( mode == "eventsScaledStack")
    {
      globalHisto -> SetLineColor(kBlack);
      globalHisto -> SetLineWidth(1);
      globalHisto -> SetFillStyle(fillstyle_summed[mapIt->first]);
    }
    if(m_xAxisRange)
      globalHisto->GetXaxis()->SetRangeUser(m_xRangeMin, m_xRangeMax);

        
    

    if(globalHisto->GetMaximum() > globalMaximum)
      globalMaximum = globalHisto -> GetMaximum();
    if( MyGetMinimum(globalHisto, 1.E-15, binMin, binMax) < globalMinimum )
      globalMinimum = MyGetMinimum(globalHisto, 1.E-15, binMin, binMax);
    
    
    if( mode == "eventsScaled" )
    {
      for(int bin = 1; bin <= globalHisto->GetNbinsX(); ++bin)
      {
        if( mH_summed[mapIt->first] > 0. && dataFlag_summed[mapIt->first] != 1 )
          nEventsScaled_sig[bin] += globalHisto->GetBinContent(bin);
        else if( mH_summed[mapIt->first] <= 0. && dataFlag_summed[mapIt->first] != 1 )
          nEventsScaled_bkg[bin] += globalHisto->GetBinContent(bin);
      }
    }
    
    
    
    if( dataFlag_summed[mapIt->first] != 1 )
    {
      if( ( (mode == "eventsScaledStack") && (mH_summed[mapIt->first] <= 0.) ) ||
          ( mode != "eventsScaledStack" ) )
        hs -> Add(globalHisto);
      if( (mode == "eventsScaledStack") && (mH_summed[mapIt->first] > 0.) )
	hs_signal -> Add(globalHisto);
                 
      ++nHists;
      
      if(globalGlobalHisto != NULL)
        globalGlobalHisto -> Add(globalHisto);
      else
        globalGlobalHisto = (TH1F*)(globalHisto -> Clone());
    }
    
    
    
    if( (mode != "eventsScaledStack") && (dataFlag_summed[mapIt->first] != 1) )
      legend.AddEntry(globalHisto, (mapIt->first).c_str(), "L");
    else if( (mode == "eventsScaledStack") && (dataFlag_summed[mapIt->first] != 1) )
      legend.AddEntry(globalHisto, (mapIt->first).c_str(), "F");
    else if( dataFlag_summed[mapIt->first] == 1  )
    {
      globalHisto -> SetMarkerStyle(20);
      legend.AddEntry(globalHisto, (mapIt->first).c_str(), "P");    
    }
    
    
    stepHisto -> SetBinContent(i+1, globalHisto->GetBinContent(step));
    stepHisto -> GetXaxis() -> SetBinLabel(i+1, (mapIt->first).c_str());
    
    
    
    
    
    
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
    
    
    
    if(outFile_CoLeMIB)
    {
      (*outFile_CoLeMIB) << std::setw(20) << mapIt->first << ",   " 
                         << std::scientific << std::setprecision(3) << std::setw(10) << 1.*globalHisto -> GetBinContent(step) << " "
                         << std::scientific << std::setprecision(2) << std::setw(10) << 1.*globalHisto -> GetBinError(step) << std::endl;
    }
    
    ++i;
  }
  
  
  
  
  
  // draw the stack and save file
  c1 = new TCanvas();
  c1 -> cd();
  c1 -> SetGridx();
  c1 -> SetGridy();
  if(logy) c1 -> SetLogy();
  TPad* p1 = NULL;
  TPad* p2 = NULL;
  
  
  
  if(mode != "eventsScaledStack")
  {
    if( nHists > 0 )
      hs -> Draw("nostack");
  }
  else
  {
    delete c1;
    c1 = new TCanvas("c1","c1",800,800);
    c1 -> cd();
        
    p1 = new TPad("p1","p1",0., 0.25, 1., 1.);
    p2 = new TPad("p2","p2",0., 0., 1., 0.25);
    p1 -> Draw();
    p2 -> Draw();
    
    p1 -> cd();
    p1 -> SetGridx();
    p1 -> SetGridy();
    if(logy) p1 -> SetLogy();
    
    if( hs->GetHists()->GetEntries() > 0 )
    {
      hs -> Draw("HISTO");
      hs_signal -> Draw("HISTO,same");
      DrawTStackError(hs);
    }
  }

  legend.SetTextFont(42);  
  legend.SetTextSize(0.025);
  legend.Draw("same");
  
  
  
  hs->SetMinimum(globalMinimum);
  hs->SetMaximum(globalMaximum+0.1*globalMaximum);
  if(logy && nHists > 0)
  {
    hs->SetMinimum(pow(10., log10(globalMinimum) - 0.1));
    hs->SetMaximum(pow(10., log10(globalMaximum) + 0.1));
  }

  if(nHists > 0)
  {
    hs -> GetXaxis() -> SetTitleSize(0.04);
    hs -> GetXaxis() -> SetLabelSize(0.03);
    hs -> GetXaxis() -> SetTitleOffset(1.25);
    
    hs -> GetYaxis() -> SetTitleSize(0.04);
    hs -> GetYaxis() -> SetLabelSize(0.03);
    hs -> GetYaxis() -> SetTitleOffset(1.50);
    
    
    
    if(m_xAxisRange)
    {
      hs->GetXaxis()->SetRangeUser(m_xRangeMin, m_xRangeMax);
    }
    if(m_yAxisRange)
    {
      hs->SetMinimum(m_yRangeMin);
      hs->SetMaximum(m_yRangeMax);
    }
  
  }
  
  
  if(mode == "events")
  {
    if(nHists > 0)
      hs->GetYaxis()->SetTitle("events left"); 
    stepHisto->GetYaxis()->SetTitle("events left");
    
    if(dataHisto != NULL)
    {
      dataHisto -> SetMarkerStyle(20);
      if(nHists > 0)
        dataHisto -> Draw("P,same");
      else
        dataHisto -> Draw("P");
    }
    
    struct stat st;
    if(stat(m_outputDir.c_str(), &st) != 0)
    {
      std::cout << ">>>plotUtils::Error accessing directory " << m_outputDir;
      exit(-1);
    }
    c1->Print((m_outputDir+"events."+m_imgFormat).c_str(), m_imgFormat.c_str());
  }
  
  if(mode == "eventsScaled")
  {
    if(nHists > 0)
      hs->GetYaxis()->SetTitle("events scaled");
    stepHisto->GetYaxis()->SetTitle("events scaled");
    
    latex->Draw("same");
    latex2->Draw("same");
    
    if(dataHisto != NULL)
    {
      dataHisto -> SetMarkerStyle(20);
      if(nHists > 0)
        dataHisto -> Draw("P,same");
      else
        dataHisto -> Draw("P");
    }
    
    struct stat st;
    if(stat(m_outputDir.c_str(), &st) != 0)
    {
      std::cout << ">>>plotUtils::Error accessing directory " << m_outputDir;
      exit(-1);
    }
    
    c1->Print((m_outputDir+"eventsScaled."+m_imgFormat).c_str(), m_imgFormat.c_str());
    
    
    
    (*outFile) << "\n";
    for(unsigned int bin = 1; bin <= nEventsScaled_sig.size(); ++bin)
    {
      (*outFile) << "bin = "          << std::setw(2) << bin
                 << "   S = "         << nEventsScaled_sig[bin]
                 << "   B = "         << nEventsScaled_bkg[bin]
                 << "   S/sqrt(B) = " << nEventsScaled_sig[bin]/sqrt(nEventsScaled_bkg[bin])
                 << std::endl;
    }
  }
  
  
  
  if(mode == "eventsScaledStack")
  {
    if(nHists > 0)
      hs->GetYaxis()->SetTitle("events scaled & stacked");
    stepHisto->GetYaxis()->SetTitle("events scaled & stacked");
    
    latex->Draw("same");
    latex2->Draw("same");
    
    if(dataHisto != NULL)
    {
      dataHisto -> SetMarkerStyle(20);
      
      for(int bin = 1; bin <= dataHisto->GetNbinsX(); ++bin)
        dataHisto->SetBinError(bin,0.); 
      
      dataHisto -> Draw("P,same");
      
      
      
      TH1F* ratioHisto = (TH1F*)(dataHisto -> Clone());
      
      TObjArray* histos = hs -> GetStack();
      int nHistos = histos -> GetEntries();
      TH1F* lastHisto = (TH1F*)(histos->At(nHistos-1))->Clone();

      int nPoints = lastHisto->GetNbinsX();
      TGraph* ratioGraph1s = new TGraph(2*nPoints);
      TGraph* ratioGraph2s = new TGraph(2*nPoints);
      
      for(int bin = 1; bin <= dataHisto->GetNbinsX(); ++bin)
      {
        if(globalGlobalHisto->GetBinContent(bin) == 0.) continue;
        ratioHisto -> SetBinContent(bin, 1.*dataHisto->GetBinContent(bin)/globalGlobalHisto->GetBinContent(bin));
        ratioHisto -> SetBinError(bin, 1.*dataHisto->GetBinError(bin)/globalGlobalHisto->GetBinContent(bin));
      }
      
      int point = 0;
      for(int bin = 1; bin <= lastHisto->GetNbinsX(); ++bin)
      {
        float binCenter = lastHisto->GetBinCenter(bin);
        float binError = sqrt(lastHisto->GetBinError(bin)*lastHisto->GetBinError(bin) +
                              lastHisto->GetBinContent(bin));
        float binErrorM;
        float binErrorP;
        float binError2M;
        float binError2P;
        if( lastHisto->GetBinContent(bin) != 0 )
        {
          binErrorM = 1. - binError/lastHisto->GetBinContent(bin);
          binErrorP = 1. + binError/lastHisto->GetBinContent(bin);
          binError2M = 1. - 2.*binError/lastHisto->GetBinContent(bin);
          binError2P = 1. + 2.*binError/lastHisto->GetBinContent(bin);
        }
        else
        {
          binErrorM = 1.;
          binErrorP = 1.;
          binError2M = 1.;
          binError2P = 1.;
        }
        ratioGraph1s -> SetPoint(point,binCenter,binErrorM);
        ratioGraph1s -> SetPoint(2*nPoints-point-1,binCenter,binErrorP);
        ratioGraph2s -> SetPoint(point,binCenter,binError2M);
        ratioGraph2s -> SetPoint(2*nPoints-point-1,binCenter,binError2P);
        
        ++point;
      }
      
      p2 -> cd();
      p2 -> SetGridx();
      p2 -> SetGridy();
      
      ratioHisto -> GetYaxis() -> SetRangeUser(0., 2.);
      ratioHisto -> Draw("P");
      
      ratioGraph2s -> SetFillColor(kYellow);
      ratioGraph2s -> SetLineColor(kYellow);
      ratioGraph2s -> Draw("F,same");
      
      ratioGraph1s -> SetFillColor(kGreen);
      ratioGraph1s -> SetLineColor(kGreen);
      ratioGraph1s -> Draw("F,same");
      
      ratioHisto -> Draw("P,same");
      
      TF1* line = new TF1("line", "1.", -1000000., 1000000.);
      line -> SetLineWidth(2);
      line -> SetLineColor(kRed);
      line -> Draw("same");
      
      p1 -> cd();
    }
    
    
    
    struct stat st;
    if(stat(m_outputDir.c_str(), &st) != 0)
    {
      std::cout << ">>>plotUtils::Error accessing directory " << m_outputDir;
      exit(-1);
    }
    
    c1->Print((m_outputDir+"eventsScaledStack."+m_imgFormat).c_str(), m_imgFormat.c_str());
  }
  
  
    
  if(mode == "efficiencies")
  {
    if(nHists > 0)
      hs->GetYaxis()->SetTitle("efficiency");
    stepHisto->GetYaxis()->SetTitle("efficiency");
    
    if(dataHisto != NULL)
    {
      dataHisto -> SetMarkerStyle(20);
      if(nHists > 0)
        dataHisto -> Draw("P,same");
      else
        dataHisto -> Draw("P");
    }
    
    struct stat st;
    if(stat(m_outputDir.c_str(), &st) != 0)
    {
      std::cout << ">>>plotUtils::Error accessing directory " << m_outputDir;
      exit(-1);
    }
    c1->Print((m_outputDir+"efficiencies."+m_imgFormat).c_str(), m_imgFormat.c_str());
  }
  
  
  
  if(mode == "efficienciesRelative")
  {
    if(nHists > 0)
      hs->GetYaxis()->SetTitle("relative efficiency");
    stepHisto->GetYaxis()->SetTitle("relative efficiency");
    
    if(dataHisto != NULL)
    {
      dataHisto -> SetMarkerStyle(20);
      if(nHists > 0)
        dataHisto -> Draw("P,same");
      else
        dataHisto -> Draw("P");
    }
    
    struct stat st;
    if(stat(m_outputDir.c_str(), &st) != 0)
    {
      std::cout << ">>>plotUtils::Error accessing directory " << m_outputDir;
      exit(-1);
    }
    c1->Print((m_outputDir+"efficienciesRelative."+m_imgFormat).c_str(), m_imgFormat.c_str());  
  }

  if(mode == "significance")
  {
    if(nHists > 0)
    hs->GetYaxis()->SetTitle("S / #sqrt{B}");
    
    struct stat st;
    if(stat(m_outputDir.c_str(), &st) != 0)
    {
      std::cout << ">>>plotUtils::Error accessing directory " << m_outputDir;
      exit(-1);
    }
    c1->Print((m_outputDir+"significance."+m_imgFormat).c_str(), m_imgFormat.c_str());
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
    std::cout << ">>>plotUtils::Error accessing directory " << m_outputDir;
    exit(-1);
  }
  c1->Print( (m_outputDir+mode+"_step."+m_imgFormat).c_str(), m_imgFormat.c_str());
  
  delete c1;
  
  
  
  // save stepHisto root file
  TFile* outRootFile = new TFile((m_outputDir+mode+"_step.root").c_str(), "RECREATE");
  outRootFile -> cd();
  stepHisto -> Write();
  outRootFile -> Close();
  delete stepHisto;
  delete outRootFile;
  
  
  
  // close root files
  if(outFile)
    outFile -> close();
  if(outFile_CoLeMIB)
    outFile_CoLeMIB -> close();
  
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

void drawTStack::SetUnit(const std::string& unit)
{
  m_unit = unit;
}






double MyGetMinimum(const TH1F* histo, const double& minval, int binMin, int binMax)
{
  if(binMin == -1) binMin = 1;
  if(binMax == -1) binMax = histo -> GetNbinsX();
  
  double minimum = histo -> GetMaximum();
  double value = 0.;
  
  for(int bin = binMin; bin <= binMax; ++bin)
  {
    value = histo -> GetBinContent(bin);
    if( (value > minval) && (value < minimum) )
      minimum = value;
  }

  return minimum;
}






void drawTStack::DrawTStackError(THStack* hs)
{
  TObjArray* histos = hs -> GetStack();
  int nHistos = histos -> GetEntries();
  TH1F* lastHisto = (TH1F*)(histos->At(nHistos-1))->Clone();
  
  lastHisto -> SetFillStyle(3005);
  lastHisto -> SetFillColor(kBlack);
  lastHisto -> SetMarkerSize(0);
 
  for(int bin = 0; bin <= lastHisto->GetNbinsX(); ++bin)
  {
    double oldErr = lastHisto -> GetBinError(bin);
    double poissonErr = sqrt(lastHisto -> GetBinContent(bin));
    lastHisto -> SetBinError(bin,sqrt(oldErr*oldErr + poissonErr*poissonErr));
    //std::cout << "bin: " << bin << "   err: " << std::setprecision(4) << lastHisto -> GetBinError(bin) << std::endl;
  }
  
  lastHisto -> DrawClone("same,E2");
}
