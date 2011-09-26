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
  // drawTStack::read the file with the list of samples and cross sections
  std::string listFullFileName = inputDir+listFileName;
  std::ifstream listFile( listFullFileName.c_str() );
  if(!listFile.is_open())
  {
    std::cout << "\n>>>plotUtils::drawTStack::Error opening file " << listFullFileName << std::endl;
    exit(-1);
  }
  else
    std::cout << "\n>>>plotUtils::drawTStack::Opening file " << listFullFileName << std::endl;
  
  
  
  // drawTStack::loop over list of samples
  while(!listFile.eof())
  {
    std::string sample;
    std::string sumName;
    std::string jetAlgorithm;
    double crossSection;
    double scaleFactor;
    int color;
    int linestyle;
    int fillstyle;
    int dataFlag;
    double mH;
    
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
    m_crossSection[sample] = crossSection*scaleFactor;
    m_color[sample] = color;
    m_linestyle[sample] = linestyle;
    m_fillstyle[sample] = fillstyle;
    m_dataFlag[sample] = dataFlag;
    m_mH[sample] = mH;
    
    std::pair<std::string, std::string> dummyPair2(sample, jetAlgorithm);
    m_jetAlgorithm.push_back(dummyPair2);
  }
  
  listFile.close();
  //std::cout << ">>>plotUtils::drawTStack::Read " << m_list.size() << " samples" << std::endl;
  //std::cout << ">>>plotUtils::drawTStack::Closing file " << listFullFileName << "\n" << std::endl;
}






drawTStack::~drawTStack()
{}






void drawTStack::Initialize()
{ 
  //std::cout << "\n>>>plotUtils::Initialize " << std::endl;
  
  
  // Initialize::clear vector with root files
   m_rootFiles.clear();
  
  
  // Initialize::initialize summed vectors
  for(std::vector<std::pair<std::string, std::string> >::const_iterator vecIt = m_list.begin();
      vecIt != m_list.end(); ++vecIt)
  {
    m_crossSection_summed[vecIt->second] += m_crossSection[vecIt->first];
    m_color_summed[vecIt->second]         = m_color[vecIt->first];
    m_linestyle_summed[vecIt->second]     = m_linestyle[vecIt->first];
    m_fillstyle_summed[vecIt->second]     = m_fillstyle[vecIt->first];
    m_mH_summed[vecIt->second]            = m_mH[vecIt->first];
    m_dataFlag_summed[vecIt->second]      = m_dataFlag[vecIt->first];
    m_histo_summed[vecIt->second] = NULL;
    if( (m_dataFlag_summed[vecIt->second] != 1) && (m_mH_summed[vecIt->second] <= 0) )
      m_bkgHisto_summed[vecIt->second] = NULL;
    if( (m_dataFlag_summed[vecIt->second] != 1) && (m_mH_summed[vecIt->second] > 0) )
      m_sigHisto_summed[vecIt->second] = NULL;
    if( (m_dataFlag_summed[vecIt->second] == 1) && (m_mH_summed[vecIt->second] <= 0) )
      m_dataHisto_summed[vecIt->second] = NULL;
  }
  
  // Initialize::initialize global histograms
  m_globalHisto = NULL;
  m_bkgGlobalHisto = NULL;
  m_sigGlobalHisto = NULL;
  m_dataGlobalHisto = NULL;
  
  m_bkgGlobalIntegral = 0.;
  m_dataGlobalIntegral = 0.;
  
  m_xAxisRange = false;
  m_xAxisTitle = false;
  m_yAxisRange = false;
  m_yAxisTitle = false;
}






int drawTStack::MakeHistograms(std::vector<std::string>& variableNames, const std::string& histoName,
                               const std::string& mode,
                               const float& lumi, const int& step,
                               const int& nBins,
                               const bool& PURescale,
                               const bool& weightEvent,
                               const bool& stackSig,
                               std::vector<std::string>* cut)
{ 
  std::cout << "\n>>>plotUtils::MakeHistograms::make " << histoName;
  
  
  
  // MakeHistogram::Initialize
  Initialize();
  
  
  
  // MakeHistogram::loop over all the samples
  int i = 0;
  for(std::vector<std::pair<std::string, std::string> >::const_iterator vecIt = m_list.begin(); vecIt != m_list.end(); ++vecIt)
  {
    // MakeHistogram::open root file
    std::string fullRootFileName = m_inputDir+vecIt->first+"/"+m_baseRootFileName+"_"+m_jetAlgorithm.at(i).second+".root";
    //std::cout << "opening file: " << fullRootFileName << std::endl;//
    TFile* inFile = TFile::Open(fullRootFileName.c_str(),"READ");
    if( !inFile->IsOpen() )
    {
      std::cout << "\n>>>plotUtils::MakeHistograms::error in opening file " << fullRootFileName << std::endl;
      exit(-1);
    }
    m_rootFiles.push_back(inFile);
    
    
    // MakeHistogram::get tree
    char treeName[50];
    sprintf(treeName, "ntu_%d", step);
    TTree* tree = (TTree*)(inFile -> Get(treeName));
    
    // MakeHistogram:: dump tree variable(s) into histogram
    TH1F* histo = new TH1F(histoName.c_str(),"", nBins, m_xRangeMin, m_xRangeMax);    
    histo -> Sumw2();
    
    for(unsigned int jj = 0; jj < variableNames.size(); ++jj)
    {
      //std::cout << "MakeHistogram::Dumping tree variable " << (variableNames.at(jj)+">>"+histoName).c_str() << std::endl;
      
      std::string cutExtended = "(1==1)";
      if(PURescale)
        cutExtended += " * (PURescaleFactor((PUit_n+PUoot_n)/3.))";
      if(weightEvent)
        cutExtended += " * (eventWeight)";
      if(cut)
        cutExtended += " * ("+cut->at(jj)+")";

      c1 = new TCanvas();
      c1 -> cd();      
      tree -> Draw( (variableNames.at(jj)+" >>+ "+histoName).c_str(), cutExtended.c_str());      
      delete c1;
      c1 = NULL;
    }
    
    
    // MakeHistogram::get event histogram
    //std::cout << "getting histogram events" << std::endl;
    TH1F* eventsHisto = (TH1F*)(inFile -> Get("events"));
    if(eventsHisto == NULL)
    {
      std::cout << ">>>plotUtils::Error in getting object events" << std::endl;
      exit(-1);
    }
    
    
    
    // MakeHistogram::scale histograms normalizing to lumi (in fb^-1)
    // MakeHistogram::if data do not apply any scale factor
    if( (histo->GetEntries() > 0.) && (m_dataFlag[vecIt->first] != 1) )
    {
      histo -> Scale(1000.*lumi*m_crossSection[vecIt->first]/eventsHisto->GetBinContent(1));
      m_crossSection_summed[vecIt->second] += m_crossSection[vecIt->first];
    }
    
    // MakeHistogram::add histograms with same summed name
    // all
    if( m_histo_summed[vecIt->second] != NULL )
      m_histo_summed[vecIt->second] -> Add(histo);
    else
      m_histo_summed[vecIt->second] = (TH1F*)(histo -> Clone((histoName+"_summed").c_str()));
    
    // bkg    
    if( (m_dataFlag_summed[vecIt->second] != 1) && (m_mH_summed[vecIt->second] <= 0) )
    {
      if( m_bkgHisto_summed[vecIt->second] != NULL )
        m_bkgHisto_summed[vecIt->second] -> Add(histo);
      else
        m_bkgHisto_summed[vecIt->second] = (TH1F*)(histo -> Clone((histoName+"_bkgSummed").c_str()));
    }
    
    // signal    
    if( (m_dataFlag_summed[vecIt->second] != 1) && (m_mH_summed[vecIt->second] > 0) )
    {
      if( m_sigHisto_summed[vecIt->second] != NULL )
        m_sigHisto_summed[vecIt->second] -> Add(histo);
      else
        m_sigHisto_summed[vecIt->second] = (TH1F*)(histo -> Clone((histoName+"_sigSummed").c_str()));
    }
    
    // data
    if( (m_dataFlag_summed[vecIt->second] == 1) && (m_mH_summed[vecIt->second] <= 0) )
    {
      if( m_dataHisto_summed[vecIt->second] != NULL )
        m_dataHisto_summed[vecIt->second] -> Add(histo);
      else
        m_dataHisto_summed[vecIt->second] = (TH1F*)(histo -> Clone((histoName+"_dataSummed").c_str()));
    }    
    
    
    ++i;
  } // MakeHistogram::end loop over all the samples
  
  
  
  
  
  
  //MakeHistogram::make stacks
  m_stack = new THStack("stack","stack");
  m_bkgStack = new THStack("bkgStack","bkgStack");
  m_sigStack = new THStack("sigStack","sigStack");
  
  //MakeHistogram::make legend    
  m_legend = new TLegend(m_xLowLegend, m_yLowLegend, m_xHighLegend, m_yHighLegend);
  m_legend -> SetFillColor(kWhite);
  m_legend -> SetFillStyle(4000);  
  m_legend -> SetTextFont(42);  
  m_legend -> SetTextSize(0.025);
  
  
  
  
  
  
  // MakeHistogram::get the normalizations
  if( mode == "sameAreaStack" )
  {  
    for(std::map<std::string, double>::const_iterator mapIt = m_crossSection_summed.begin(); mapIt != m_crossSection_summed.end(); ++mapIt)
    {
      TH1F* histo_summed = m_histo_summed[mapIt->first];
      if( histo_summed->GetEntries() == 0 ) continue;
      
      if( m_dataFlag_summed[mapIt->first] != 1 )
      {
        m_globalIntegral += histo_summed -> Integral();
        if( m_mH_summed[mapIt->first] <= 0 )
          m_bkgGlobalIntegral += histo_summed -> Integral();
        else
          m_sigGlobalIntegral += histo_summed -> Integral();
      }
      else
      {
        m_dataGlobalIntegral += histo_summed -> Integral();
      }
    }
  }
  
  
  
  // MakeHistogram::integral histograms
  if( mode == "integralNoStack" )
  {  
    for(std::map<std::string, double>::const_iterator mapIt = m_crossSection_summed.begin(); mapIt != m_crossSection_summed.end(); ++mapIt)
    {
      TH1F* histo_summed = m_histo_summed[mapIt->first];
      TH1F* histo_integral = (TH1F*)(histo_summed->Clone());
      if( histo_summed->GetEntries() == 0 ) continue;
      
      int nBins = histo_summed -> GetNbinsX();
      for(int bin = 1; bin <= nBins; ++bin)
      {
        histo_summed -> SetBinContent(bin,histo_integral->Integral(1,bin)/histo_integral->Integral(1,nBins));
        histo_summed -> SetBinError(bin,sqrt(histo_integral->Integral(1,bin))/histo_integral->Integral(1,nBins));
      }
    }
  }
  
  
  
  
  
  
  // MakeHistogram::loop over summed samples
  // MakeHistogram::make global histograms
  bool isFirstSample  = true;
  bool isFirstBkgSample  = true;
  bool isFirstSigSample  = true;
  bool isFirstDataSample = true;
  int nHists = 0;
  for(std::map<std::string, double>::const_iterator mapIt = m_crossSection_summed.begin(); mapIt != m_crossSection_summed.end(); ++mapIt)
  {
    TH1F* histo_summed = m_histo_summed[mapIt->first];
    if( histo_summed->GetEntries() == 0 ) continue;
    ++nHists;
    
    
    //MakeHistogram::set normalizations
    if( mode == "sameAreaStack" )
    {
      if( m_dataFlag_summed[mapIt->first] != 1 )
      {
        if( stackSig == false )
          histo_summed -> Scale(m_dataGlobalIntegral/m_bkgGlobalIntegral);
        else
          histo_summed -> Scale(m_dataGlobalIntegral/(m_bkgGlobalIntegral+m_sigGlobalIntegral));
      }
    }
    if( mode == "sameAreaNoStack" )
      histo_summed -> Scale(1./histo_summed->Integral());
    
    
    //MakeHistogram::graphical settings
    // y-axis properties
    histo_summed -> GetXaxis() -> SetTitleSize(0.04);
    histo_summed -> GetXaxis() -> SetLabelSize(0.03);
    histo_summed -> GetXaxis() -> SetTitleOffset(1.25);
    
    if( m_xAxisRange )
      histo_summed->GetXaxis()->SetRangeUser(m_xRangeMin, m_xRangeMax);
    
    std::string xAxisTitle = histoName;
    if( m_xAxisTitle ) xAxisTitle = m_xTitle;
    if( m_unit != "" ) xAxisTitle += "   ("+m_unit+")";
    histo_summed->GetXaxis()->SetTitle(xAxisTitle.c_str());
    
    // y-axis properties
    histo_summed -> GetYaxis() -> SetTitleSize(0.04);
    histo_summed -> GetYaxis() -> SetLabelSize(0.03);
    histo_summed -> GetYaxis() -> SetTitleOffset(1.50);    
    
    if( m_yAxisRange )
      histo_summed->GetYaxis()->SetRangeUser(m_yRangeMin, m_yRangeMax);      
    
    char binWidthBuffer[50];
    sprintf(binWidthBuffer, "%.2e", histo_summed->GetBinWidth(1));
    if( mode == "eventsScaled" )
      histo_summed->GetYaxis()->SetTitle(("events / " + std::string(binWidthBuffer)+" "+m_unit).c_str());
    if( mode == "sameAreaStack" )
      histo_summed->GetYaxis()->SetTitle(("events / " + std::string(binWidthBuffer)+" "+m_unit).c_str());
    if( mode == "sameAreaNoStack" )
      histo_summed->GetYaxis()->SetTitle(("event fraction / " + std::string(binWidthBuffer)+" "+m_unit).c_str());
    if( mode == "integralStack" )
      histo_summed->GetYaxis()->SetTitle(("event integral / " + std::string(binWidthBuffer)+" "+m_unit).c_str());
    if( mode == "integralNoStack" )
      histo_summed->GetYaxis()->SetTitle(("event integral / " + std::string(binWidthBuffer)+" "+m_unit).c_str());
    
    // colors
    if( m_dataFlag_summed[mapIt->first] != 1 )
    {
      histo_summed -> SetFillColor(m_color_summed[mapIt->first]);
      histo_summed -> SetFillStyle(m_fillstyle_summed[mapIt->first]);
      if( (mode == "sameAreaNoStack") || (mode == "integralNoStack") )
      {
        histo_summed -> SetFillStyle(0);
        histo_summed -> SetLineColor(m_color_summed[mapIt->first]);
        histo_summed -> SetLineWidth(3);
      }
      if( (mode == "eventsScaled") || (mode == "sameAreaStack") || (mode == "integralStack") )
      {
        histo_summed -> SetLineColor(kBlack);
        histo_summed -> SetLineWidth(1);
        if(m_mH_summed[mapIt->first] > 0.)
        {
          histo_summed -> SetLineColor(m_color_summed[mapIt->first]);
          histo_summed -> SetLineWidth(2);
        }
      }
      histo_summed -> SetLineStyle(m_linestyle_summed[mapIt->first]);
    }
    else
    {
      histo_summed -> SetMarkerStyle(20);
      histo_summed -> SetMarkerColor(kBlack);      
    }
    
    
    //MakeHistogram::make global histograms    
    // all
    if( (isFirstSample == false) && (m_dataFlag_summed[mapIt->first] != 1) )
    {
      m_globalHisto -> Add(histo_summed);
    }
    if( (isFirstSample == true) && (m_dataFlag_summed[mapIt->first] != 1) )
    {
      m_globalHisto = (TH1F*)(histo_summed -> Clone((histoName+"_global").c_str()));
      isFirstSample = false;
    }
    // bkg
    if( (isFirstBkgSample == false) && (m_dataFlag_summed[mapIt->first] != 1) && (m_mH_summed[mapIt->first] <= 0) )
    {
      m_bkgGlobalHisto -> Add(histo_summed);
    }
    if( (isFirstBkgSample == true) && (m_dataFlag_summed[mapIt->first] != 1) && (m_mH_summed[mapIt->first] <= 0) )
    {
      m_bkgGlobalHisto = (TH1F*)(histo_summed -> Clone((histoName+"_bkgGlobal").c_str()));
      isFirstBkgSample = false;
    }
    // sig
    if( (isFirstSigSample == false) && (m_dataFlag_summed[mapIt->first] != 1) && (m_mH_summed[mapIt->first] > 0) )
    {
      m_sigGlobalHisto -> Add(histo_summed);
    }
    if( (isFirstSigSample == true) && (m_dataFlag_summed[mapIt->first] != 1) && (m_mH_summed[mapIt->first] > 0) )
    {
      m_sigGlobalHisto = (TH1F*)(histo_summed -> Clone((histoName+"_sigGlobal").c_str()));
      isFirstSigSample = false;
    }
    // data
    if( (isFirstDataSample == false) && (m_dataFlag_summed[mapIt->first] == 1) && (m_mH_summed[mapIt->first] <= 0) )
    {
      m_dataGlobalHisto -> Add(histo_summed);
    }
    if( (isFirstDataSample == true) && (m_dataFlag_summed[mapIt->first] == 1) && (m_mH_summed[mapIt->first] <= 0) )
    {
      m_dataGlobalHisto = (TH1F*)(histo_summed -> Clone((histoName+"_dataGlobal").c_str()));
      isFirstDataSample = false;
    }
    
    
    //MakeHistogram::add summed histograms to the stacks
    // all
    if( (m_dataFlag_summed[mapIt->first] != 1) )
      m_stack -> Add(histo_summed);
    // bkg
    if( (m_dataFlag_summed[mapIt->first] != 1) && (m_mH_summed[mapIt->first] <= 0) )
      m_bkgStack -> Add(histo_summed);
    // sig
    if( (m_dataFlag_summed[mapIt->first] != 1) && (m_mH_summed[mapIt->first] > 0) )
      m_sigStack -> Add(histo_summed);
    
    
    //MakeHistogram::add summed histograms to the legend    
    if( (m_dataFlag_summed[mapIt->first] != 1) )
    {
      if( mode == "eventsScaled" )
        m_legend -> AddEntry(histo_summed, (mapIt->first).c_str(), "F");
      if( mode == "sameAreaNoStack" )
        m_legend -> AddEntry(histo_summed, (mapIt->first).c_str(), "L");
      if( mode == "sameAreaStack" )
        m_legend -> AddEntry(histo_summed, (mapIt->first).c_str(), "F");
      if( mode == "integralStack" )
        m_legend -> AddEntry(histo_summed, (mapIt->first).c_str(), "F");
      if( mode == "integralNoStack" )
        m_legend -> AddEntry(histo_summed, (mapIt->first).c_str(), "L");
    }
    else
      m_legend -> AddEntry(histo_summed, (mapIt->first).c_str(), "P");
    
    
  } // loop over summed samples
  
  
  
  if( nHists == 0 ) return nHists;
  
  
  
  // MakeHistogram::stack signal, if requested
  if( stackSig == true )
  {
    for(std::map<std::string, double>::const_iterator mapIt = m_crossSection_summed.begin(); mapIt != m_crossSection_summed.end(); ++mapIt)
    {
      TH1F* histo_summed = m_histo_summed[mapIt->first];
      if( histo_summed->GetEntries() == 0 ) continue;
      
      if( m_mH_summed[mapIt->first] > 0 )
        m_bkgStack -> Add(histo_summed);
    }
  }
  
  
  
  // MakeHistogram::integral histograms
  if( mode == "integralStack" )
  {
    // stacks
    delete m_bkgStack;
    delete m_sigStack;
    
    m_bkgStack = new THStack("bkgStack","bkgStack");
    m_sigStack = new THStack("sigStack","sigStack");  
    
    for(std::map<std::string, double>::const_iterator mapIt = m_crossSection_summed.begin(); mapIt != m_crossSection_summed.end(); ++mapIt)
    {
      TH1F* histo_summed = m_histo_summed[mapIt->first];
      TH1F* histo_integral = (TH1F*)(histo_summed->Clone());
      if( histo_summed->GetEntries() == 0 ) continue;
      
      int nBins = histo_summed -> GetNbinsX();

      if( (m_dataFlag_summed[mapIt->first] != 1) && (m_mH_summed[mapIt->first] <= 0) && (m_bkgGlobalHisto != NULL) )
      {      
        for(int bin = 1; bin <= nBins; ++bin)
        {
          histo_summed -> SetBinContent(bin,histo_integral->Integral(1,bin)/m_bkgGlobalHisto->Integral(1,nBins));
          histo_summed -> SetBinError(bin,sqrt(histo_integral->Integral(1,bin))/m_bkgGlobalHisto->Integral(1,nBins));
        }
        
        m_bkgStack -> Add(histo_summed);
      }
      if( (m_dataFlag_summed[mapIt->first] != 1) && (m_mH_summed[mapIt->first] > 0)  && (m_sigGlobalHisto != NULL) )
      {
        for(int bin = 1; bin <= nBins; ++bin)
        {
          histo_summed -> SetBinContent(bin,histo_integral->Integral(1,bin)/m_sigGlobalHisto->Integral(1,nBins));
          histo_summed -> SetBinError(bin,sqrt(histo_integral->Integral(1,bin))/m_sigGlobalHisto->Integral(1,nBins));
        }
        
        m_sigStack -> Add(histo_summed);
      } 
      if( (m_dataFlag_summed[mapIt->first] == 1) && (m_dataGlobalHisto != NULL) )
      {
        for(int bin = 1; bin <= nBins; ++bin)
        {
          histo_summed -> SetBinContent(bin,histo_integral->Integral(1,bin)/m_dataGlobalHisto->Integral(1,nBins));
          histo_summed -> SetBinError(bin,sqrt(histo_integral->Integral(1,bin))/m_dataGlobalHisto->Integral(1,nBins));
        }
      } 
    }
    
    
    // global histograms
    // bkg
    if( m_bkgGlobalHisto != NULL )
    {
      TH1F* bkgGlobalHisto_integral = (TH1F*)(m_bkgGlobalHisto->Clone());
      int nBins = m_bkgGlobalHisto -> GetNbinsX();
      for(int bin = 1; bin <= nBins; ++bin)
      {
        m_bkgGlobalHisto -> SetBinContent(bin,bkgGlobalHisto_integral->Integral(1,bin)/bkgGlobalHisto_integral->Integral(1,nBins));
        m_bkgGlobalHisto -> SetBinError(bin,sqrt(bkgGlobalHisto_integral->Integral(1,bin))/bkgGlobalHisto_integral->Integral(1,nBins));
      }
    }
    // sig
    if( m_sigGlobalHisto != NULL )
    {
      TH1F* sigGlobalHisto_integral = (TH1F*)(m_sigGlobalHisto->Clone());
      int nBins = m_sigGlobalHisto -> GetNbinsX();
      for(int bin = 1; bin <= nBins; ++bin)
      {
        m_sigGlobalHisto -> SetBinContent(bin,sigGlobalHisto_integral->Integral(1,bin)/sigGlobalHisto_integral->Integral(1,nBins));
        m_sigGlobalHisto -> SetBinError(bin,sqrt(sigGlobalHisto_integral->Integral(1,bin))/sigGlobalHisto_integral->Integral(1,nBins));
      }
    }
    // data
    if( m_dataGlobalHisto != NULL )
    {
      TH1F* dataGlobalHisto_integral = (TH1F*)(m_dataGlobalHisto->Clone());
      int nBins = m_dataGlobalHisto -> GetNbinsX();
      for(int bin = 1; bin <= nBins; ++bin)
      {
        m_dataGlobalHisto -> SetBinContent(bin,dataGlobalHisto_integral->Integral(1,bin)/dataGlobalHisto_integral->Integral(1,nBins));
        m_dataGlobalHisto -> SetBinError(bin,sqrt(dataGlobalHisto_integral->Integral(1,bin))/dataGlobalHisto_integral->Integral(1,nBins));
      }
    }
  }
  
  
  
  return nHists;
}






void drawTStack::FindMinimumMaximum(const std::string& mode)
{ 
  m_globalMinimum = 999999999999.;
  m_globalMaximum = 0.;
  
  int binMin = -1;
  int binMax = -1;
  
  
  // FindMinimumMaximum::loop over summed samples to get single minimum
  for(std::map<std::string, double>::const_iterator mapIt = m_crossSection_summed.begin(); mapIt != m_crossSection_summed.end(); ++mapIt)
  {    
    TH1F* histo_summed = m_histo_summed[mapIt->first];
    binMin = histo_summed -> FindBin(m_xRangeMin+0.5*histo_summed->GetBinWidth(1));
    binMax = histo_summed -> FindBin(m_xRangeMax-0.5*histo_summed->GetBinWidth(1));
    if( histo_summed -> GetEntries() == 0 ) continue;
    
    
    // sample minimum
    if( MyGetMinimum(histo_summed, 1.E-15, binMin, binMax) < m_globalMinimum )
      m_globalMinimum = MyGetMinimum(histo_summed, 1.E-15, binMin, binMax);
    
    // sample maximum    
    if( MyGetMaximum(histo_summed, 1.E15, binMin, binMax) > m_globalMaximum )
      m_globalMaximum = MyGetMaximum(histo_summed, 1.E15, binMin, binMax);
  }
  
  
  // FindMinimumMaximum::loop over summed samples to get global minimum and maximum
  if( (m_globalHisto != NULL) && (mode != "sameAreaNoStack") && (mode != "integralNoStack") && (mode != "integralNoStack") )
  {
    if( MyGetMaximum(m_globalHisto, 1.E15, binMin, binMax) > m_globalMaximum )
      m_globalMaximum = MyGetMaximum(m_globalHisto, 1.E15, binMin, binMax);
  }
  
  if( (m_dataGlobalHisto != NULL) && (mode != "sameAreaNoStack") && (mode != "integralNoStack") && (mode != "integralNoStack") )
  {
    if( MyGetMaximum(m_dataGlobalHisto, 1.E15, binMin, binMax) > m_globalMaximum )
      m_globalMaximum = MyGetMaximum(m_dataGlobalHisto, 1.E15, binMin, binMax);
  }
  
  if( mode == "integralStack" )
    m_globalMaximum = 1.;
}






void drawTStack::Draw(std::vector<std::string>& variableNames, const std::string& histoName,
                      const std::string& mode,
                      const float& lumi, const int& step,
                      const int& nBins,
                      const bool& PURescale,
                      const bool& weightEvent,
                      const bool& stackSig,
                      std::vector<std::string>* cut)
{ 
  //std::cout << "\n>>>plotUtils::Draw::Drawing histogram " << histoName;
  
  
  // Draw::MakeHistograms
  int nHists = MakeHistograms(variableNames,histoName,
                              mode,
                              lumi,step,
                              nBins,
                              PURescale,
                              weightEvent,
                              stackSig,
                              cut);
  
  if( nHists == 0 )
  {
    std::cout << "\n>>>plotUtils::Draw::Error, histogram empty" << std::endl;
    return;
  }
  
  FindMinimumMaximum(mode);
  
  Draw(c1,histoName,mode,stackSig,false,lumi);
  Draw(c2,histoName,mode,stackSig,true,lumi);
  
  
  // close root files
  for(unsigned int i = 0; i < m_rootFiles.size(); ++i)
  {
    m_rootFiles.at(i) -> Close();
    delete m_rootFiles.at(i);
  }
  
}






void drawTStack::Draw(TCanvas* c, const std::string& histoName, const std::string& mode, const bool& stackSig, const bool& logy, const float& lumi)
{
  c = new TCanvas("c","c",800,800);
  c -> cd();
 
  
  // eventsScaled or sameAreaStack modes
  if( (mode == "eventsScaled") || (mode == "sameAreaStack") )
  {
    TPad* p1 = new TPad("p1","p1",0., 0.25, 1., 1.);
    TPad* p2 = new TPad("p2","p2",0., 0., 1., 0.25);
    p1 -> Draw();
    p2 -> Draw();
    
    
    // p1
    p1 -> cd();
    p1 -> SetGridx();
    p1 -> SetGridy();
    if( logy == true ) p1 -> SetLogy();
    
    if( stackSig == false )
    {
      m_bkgStack -> Draw("HISTO");
      m_sigStack -> Draw("HISTO,same");
    }
    else
    {
      m_bkgStack -> Draw("HISTO");
    }

    DrawTStackError(m_bkgStack,false,0.);
    if( m_dataGlobalHisto != NULL ) m_dataGlobalHisto -> Draw("P,same");
    
    
    // p2
    if( m_dataGlobalHisto != NULL )
    {
      p2 -> cd();
      p2 -> SetGridx();
      p2 -> SetGridy();
      
      TGraph* ratioGraph1s = new TGraph();
      TGraph* ratioGraph2s = new TGraph();
      TH1F* ratioHisto = DrawTStackDataMCRatio(m_bkgStack,m_dataGlobalHisto,ratioGraph1s,ratioGraph2s);
      ratioHisto -> GetYaxis() -> SetRangeUser(0.4, 1.6);
      ratioHisto -> GetXaxis() -> SetLabelSize(0.09);
      ratioHisto -> GetYaxis() -> SetLabelSize(0.09);
      ratioHisto -> GetYaxis() -> SetTitleSize(0.12);
      ratioHisto -> GetYaxis() -> SetTitleOffset(0.5);
      ratioHisto -> GetYaxis() -> SetNdivisions(206);
      ratioHisto -> GetXaxis() -> SetTitle("");
      ratioHisto -> GetYaxis() -> SetTitle("data / MC");

      ratioHisto -> Draw("P");
      ratioGraph2s -> Draw("F,same");
      ratioGraph1s -> Draw("F,same");
      ratioHisto -> Draw("P,same");
        
      TF1* line = new TF1("line", "1.", -1000000., 1000000.);
      line -> SetLineWidth(2);
      line -> SetLineColor(kRed);
      line -> Draw("same");
    }
    
    p1 -> cd();
  } 
  
  
  // sameAreaNoStack or integralNoStack modes  
  if( (mode == "sameAreaNoStack") || (mode == "integralNoStack") )
  {
    TPad* p1 = new TPad("p1","p1",0., 0.25, 1., 1.);
    TPad* p2 = new TPad("p2","p2",0., 0., 1., 0.25);
    p1 -> Draw();
    p2 -> Draw();
    p1 -> cd();
    p1 -> SetGridx();
    p1 -> SetGridy();
    if( logy == true ) p1 -> SetLogy();
    
    if( stackSig == false )
    {
      m_bkgStack -> Draw("nostack,HISTO");
      m_sigStack -> Draw("nostack,HISTO,same");
    }
    else
    {
      m_bkgStack -> Draw("nostack,HISTO");    
    }
    
    if(m_dataGlobalHisto != NULL) m_dataGlobalHisto -> Draw("P,same");
  }
  
  
  
  // integralStack modes  
  if( mode == "integralStack" )
  {
    TPad* p1 = new TPad("p1","p1",0., 0.25, 1., 1.);
    TPad* p2 = new TPad("p2","p2",0., 0., 1., 0.25);
    p1 -> Draw();
    p2 -> Draw();
    p1 -> cd();
    p1 -> SetGridx();
    p1 -> SetGridy();
    if( logy == true ) p1 -> SetLogy();
    
    m_bkgStack -> Draw("HISTO");
    m_sigStack -> Draw("HISTO,same");
    if(m_dataGlobalHisto != NULL) m_dataGlobalHisto -> Draw("P,same");
  }
  
  
  
  // set x-axis properties
  std::string xAxisTitle = histoName;
  if( m_xAxisTitle ) xAxisTitle = m_xTitle;
  if( m_unit != "" ) xAxisTitle += "   ("+m_unit+")";
  m_bkgStack -> GetXaxis() -> SetTitle(xAxisTitle.c_str());
  m_bkgStack -> GetXaxis() -> SetTitleSize(0.04);
  m_bkgStack -> GetXaxis() -> SetLabelSize(0.03);
  m_bkgStack -> GetXaxis() -> SetTitleOffset(1.25);
  if(m_xAxisRange) m_bkgStack->GetXaxis()->SetRangeUser(m_xRangeMin, m_xRangeMax);
  
  // set y-axis properties
  m_bkgStack->GetYaxis()->SetTitle(((TH1F*)(m_bkgStack->GetStack()->Last()))->GetYaxis()->GetTitle());
  if(m_yAxisTitle) m_bkgStack->GetYaxis()->SetTitle(m_yTitle.c_str());    
  m_bkgStack -> GetYaxis() -> SetTitleSize(0.04);
  m_bkgStack -> GetYaxis() -> SetLabelSize(0.03);
  m_bkgStack -> GetYaxis() -> SetTitleOffset(1.50);
  
  if( logy == false)
  {
    m_bkgStack -> SetMinimum(0.);
    m_bkgStack -> SetMaximum(m_globalMaximum+0.1*m_globalMaximum);
  }
  else
  {
    m_bkgStack -> SetMinimum(pow(10.,log10(m_globalMinimum) - 0.1));
    m_bkgStack -> SetMaximum(pow(10.,log10(m_globalMaximum) + 0.1));
  }
  
  if(m_yAxisRange)
  {
    m_bkgStack->SetMinimum(m_yRangeMin);
    m_bkgStack->SetMaximum(m_yRangeMax);
  }
  
  
  
  if(m_drawLegend == true)
  {
    m_legend -> Draw("same");
    
    // Draw::Define the caption
    char lumiBuffer[200];
    if( mode == "eventsScaled" )
      sprintf(lumiBuffer, "CMS Preliminary 2011   -   #sqrt{s}=7 TeV   L=%.2f fb^{-1}",lumi);
    if( mode == "sameAreaNoStack" )
      sprintf(lumiBuffer, "CMS Preliminary 2011   -   histograms normalized to unit area");
    if( mode == "sameAreaStack" )
      sprintf(lumiBuffer, "CMS Preliminary 2011   -   #sqrt{s}=7 TeV   L=%.2f fb^{-1}   MC norm. to data",lumi);
    if( mode == "integralStack" )
      sprintf(lumiBuffer, "CMS Preliminary 2011   -   integral distributions");
    if( mode == "integralNoStack" )
      sprintf(lumiBuffer, "CMS Preliminary 2011   -   integral distributions");
    
    TLatex *latex = new TLatex(0.13, 0.96, lumiBuffer);
    
    latex->SetNDC();
    latex->SetTextFont(42);
    latex->SetTextSize(0.03);
    latex->Draw("same");
  }
  
  
  
  // write plots
  struct stat st;
  if(stat(m_outputDir.c_str(), &st) != 0)
  {
    std::cout << ">>>plotUtils::Error accessing directory " << m_outputDir;
    exit(-1);
  }
  
  if( logy == false )
    c->Print((m_outputDir+"lin_"+histoName+"."+m_imgFormat).c_str(), m_imgFormat.c_str());
  else
    c->Print((m_outputDir+"log_"+histoName+"."+m_imgFormat).c_str(), m_imgFormat.c_str());
  
  
  delete c;
}






void drawTStack::DrawEvents(const std::string& mode,
                            const float& lumi,
                            const int& step,
                            const bool& logy,
			                      const bool& PURescale)
{ 
  std::cout << "\n>>>plotUtils::Drawing " << mode;
  
  
  
  THStack* hs = new THStack("hs", "hs");
  THStack* hs_signal = new THStack("hs_signal", "hs_signal");
  int nHists = 0;
  TLegend legend(m_xLowLegend, m_yLowLegend, m_xHighLegend, m_yHighLegend);
  legend.SetFillColor(kWhite);
  legend.SetFillStyle(4000);
  
  char lumiBuffer[200];
  if( mode == "eventsScaled" )
    sprintf(lumiBuffer, "CMS Preliminary 2011   -   #sqrt{s}=7 TeV   L=%.2f fb^{-1}",lumi);
  if( mode == "eventsScaledStack" )
    sprintf(lumiBuffer, "CMS Preliminary 2011   -   #sqrt{s}=7 TeV   L=%.2f fb^{-1}",lumi);
  if( mode == "efficiencies" )
    sprintf(lumiBuffer, "CMS Preliminary 2011   -   efficiencies");
  if( mode == "efficiencies" )
    sprintf(lumiBuffer, "CMS Preliminary 2011   -   relative efficiencies");
  if( mode == "significance" )
    sprintf(lumiBuffer, "CMS Preliminary 2011   -   significance S/#sqrt{S+B}");
  
  TLatex *latex = new TLatex(0.13, 0.96, lumiBuffer);
  
  latex->SetNDC();
  latex->SetTextFont(42);
  latex->SetTextSize(0.03);
  latex->Draw("same");
  
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
    {
      if( PURescale == true )
        fullHistoName = "events_PURescaled";
      else
        fullHistoName = "events";
    }
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
    
    
    if( (mode == "eventsScaled") ||
        (mode == "eventsScaledStack") ||
        (mode == "efficiencies") ||
        (mode == "efficienciesRelative") ||
        (mode == "significance") )
    {
      if( m_crossSection[vecIt->first] != 1. )
        histo -> Scale(1000*lumi*m_crossSection[vecIt->first]/histo->GetBinContent(1));
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
  
  if(mode == "eventsScaled")
    outFile = new std::ofstream((m_outputDir+"eventsScaled.txt").c_str(), std::ios::out);
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
      if(mH_summed[mapIt->first] == 0) continue;
    
      TH1F* totalBkgHisto = NULL;
      for(std::map<std::string, double>::const_iterator mapIt2 = crossSection_summed.begin();
          mapIt2 != crossSection_summed.end(); ++mapIt2)
      {
        if( (mH_summed[mapIt2->first] > 0) || (dataFlag_summed[mapIt2->first] == 1) ) continue;
        TH1F* globalHisto2 = histo_summed[mapIt2->first];
        
        if(totalBkgHisto == NULL)
          totalBkgHisto = (TH1F*)(globalHisto2 -> Clone());
        else
          totalBkgHisto -> Add(globalHisto2);
      }
      
      for(int bin = 1; bin <= globalHisto->GetNbinsX(); ++bin)
        globalHisto->SetBinContent(bin, globalHisto->GetBinContent(bin) / 
                                        sqrt(globalHisto->GetBinContent(bin)+totalBkgHisto->GetBinContent(bin)) );
    }
    
    
    if( dataFlag_summed[mapIt->first] != 1 )
    {
      globalHisto -> SetLineColor(color_summed[mapIt->first]);
      globalHisto -> SetLineStyle(linestyle_summed[mapIt->first]);
      globalHisto -> SetLineWidth(3);
      globalHisto -> SetFillColor(color_summed[mapIt->first]);
      globalHisto -> SetFillStyle(0);
    }
    
    if( mode == "eventsScaledStack")
    {
      globalHisto -> SetLineColor(kBlack);
      globalHisto -> SetLineWidth(1);
      globalHisto -> SetFillStyle(fillstyle_summed[mapIt->first]);
      
      if(mH_summed[mapIt->first] > 0)
	{
	  globalHisto -> SetLineColor(color_summed[mapIt->first]);
	  globalHisto -> SetLineWidth(2);
	}
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
    
    
    
    ++i;
  }
  
  
  
  
  if( nHists == 0 ) return;
  
  
  
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
    
    hs -> Draw("HISTO");
    hs_signal -> Draw("HISTO,same");
    DrawTStackError(hs,false,0.);
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
    
    if(dataHisto != NULL)
    {
      dataHisto -> SetMarkerStyle(20);
      dataHisto -> Draw("P,same");
      
      p2 -> cd();
      p2 -> SetGridx();
      p2 -> SetGridy();
      
      TGraph* ratioGraph1s = new TGraph();
      TGraph* ratioGraph2s = new TGraph();
      TH1F* ratioHisto = DrawTStackDataMCRatio(hs,dataHisto,ratioGraph1s,ratioGraph2s);
      ratioHisto -> GetYaxis() -> SetRangeUser(0.4, 1.6);
      ratioHisto -> GetXaxis() -> SetLabelSize(0.09);
      ratioHisto -> GetYaxis() -> SetLabelSize(0.09);
      ratioHisto -> GetYaxis() -> SetTitleSize(0.12);
      ratioHisto -> GetYaxis() -> SetTitleOffset(0.5);
      ratioHisto -> GetYaxis() -> SetNdivisions(206);
      ratioHisto -> GetXaxis() -> SetTitle("");
      ratioHisto -> GetYaxis() -> SetTitle("data / MC");

      ratioHisto -> Draw("P");
      ratioGraph2s -> Draw("F,same");
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
    hs->GetYaxis()->SetTitle("S / #sqrt{S+B}");
    
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






//-------------------------------------------------------------------


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


//-------------------------------------------------------------------


double MyGetMaximum(const TH1F* histo, const double& maxval, int binMin, int binMax)
{
  if(binMin == -1) binMin = 1;
  if(binMax == -1) binMax = histo -> GetNbinsX();
  
  double maximum = 0.;
  double value = 0.;
  
  for(int bin = binMin; bin <= binMax; ++bin)
  {
    value = histo -> GetBinContent(bin);
    if( (value < maxval) && (value > maximum) )
      maximum = value;
  }
  
  return maximum;
}


//-------------------------------------------------------------------


void DrawTStackError(THStack* hs, bool poisson, double syst)
{
  TObjArray* histos = hs -> GetStack();
  int nHistos = histos -> GetEntries();
  TH1F* lastHisto = (TH1F*)(histos->At(nHistos-1));
  TH1F* lastHistoClone = (TH1F*)(histos->At(nHistos-1))->Clone();
  
  for(int bin = 0; bin <= lastHistoClone->GetNbinsX(); ++bin)
  {
    double oldErr = lastHistoClone -> GetBinError(bin);
    double poissonErr = 0.;
    if( poisson ) poissonErr = sqrt(lastHistoClone -> GetBinContent(bin));
    double systErr = syst * lastHistoClone -> GetBinContent(bin);

    lastHisto -> SetBinError(bin,sqrt(oldErr*oldErr + poissonErr*poissonErr + systErr*systErr));    
    lastHistoClone -> SetBinError(bin,sqrt(oldErr*oldErr + poissonErr*poissonErr + systErr*systErr));
    //std::cout << "bin: " << bin << "   err: " << std::setprecision(4) << lastHistoClone -> GetBinError(bin) << std::endl;
  }

  lastHistoClone -> SetMarkerSize(0);
  lastHistoClone -> SetFillColor(kBlack);
  lastHistoClone -> SetFillStyle(3005);
  lastHistoClone -> DrawClone("same,E2");
}


//-------------------------------------------------------------------


TH1F* DrawTStackDataMCRatio(THStack* hs, TH1F* dataGlobalHisto,
                            TGraph* ratioGraph1s, TGraph* ratioGraph2s)
{
  TH1F* ratioHisto = (TH1F*)(dataGlobalHisto -> Clone());
  ratioHisto -> Reset();
  
  TObjArray* histos = hs -> GetStack();
  int nHistos = histos -> GetEntries();
  TH1F* lastHisto = (TH1F*)(histos->At(nHistos-1))->Clone();
  
  int nPoints = lastHisto->GetNbinsX();
  
  ratioGraph1s -> Set(2*nPoints);
  ratioGraph1s -> SetFillColor(kGreen);
  ratioGraph1s -> SetLineColor(kGreen);
  ratioGraph2s -> Set(2*nPoints);
  ratioGraph2s -> SetFillColor(kYellow);
  ratioGraph2s -> SetLineColor(kYellow);
  
  for(int bin = 1; bin <= dataGlobalHisto->GetNbinsX(); ++bin)
  {
    if( lastHisto->GetBinContent(bin) == 0. ) continue;
    ratioHisto -> SetBinContent(bin, 1.*dataGlobalHisto->GetBinContent(bin)/lastHisto->GetBinContent(bin));
    ratioHisto -> SetBinError(bin, 1.*dataGlobalHisto->GetBinError(bin)/lastHisto->GetBinContent(bin));
  }
  
  int point = 0;
  for(int bin = 1; bin <= lastHisto->GetNbinsX(); ++bin)
  {
    float binCenter = lastHisto->GetBinCenter(bin);
    float binError = sqrt( lastHisto->GetBinError(bin)*lastHisto->GetBinError(bin) );
    float binErrorM;
    float binErrorP;
    float binError2M;
    float binError2P;
    if( lastHisto->GetBinContent(bin) != 0. )
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
  
  return ratioHisto;
}
