#include "TF1.h"
#include "plotUtils.h"
#include "setTDRStyle.h"



void normalizeToBinWidths (TH1F * input)
{
  for (int iBin = 1 ; iBin <= input->GetNbinsX () ; ++iBin)
    {
      double content = input->GetBinContent (iBin) ;
      double error = input->GetBinError (iBin) ;
      double invBinWidth = 1. / input->GetBinWidth (iBin) ;
      input->SetBinContent (iBin, content * invBinWidth) ;
      input->SetBinError (iBin, error * invBinWidth) ;
    }
  return ;
}



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
 m_globalHisto(NULL),
 m_bkgGlobalHisto(NULL),
 m_sigGlobalHisto(NULL),
 m_dataGlobalHisto(NULL),
 m_stack(NULL),
 m_bkgStack(NULL),
 m_sigStack(NULL),
 m_legend(NULL),
 m_generalCut("(1 == 1)"),
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
 m_yLowLegend(0.60),
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
    int MCFlag;
    double mH;
    
    listFile >> sample >> sumName >> color >> linestyle >> fillstyle >> dataFlag >> MCFlag >> mH >> crossSection >> scaleFactor >> jetAlgorithm;

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
              << std::setw(5)
              << MCFlag << " "
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
    m_scaleFactor[sample] = scaleFactor;
    m_color[sample] = color;
    m_linestyle[sample] = linestyle;
    m_fillstyle[sample] = fillstyle;
    m_dataFlag[sample] = dataFlag;
    m_mH[sample] = mH;
    
    std::pair<std::string, std::string> dummyPair2(sample, jetAlgorithm);
    m_jetAlgorithm.push_back(dummyPair2);
  }
  
  
  for(std::vector<std::pair<std::string, std::string> >::const_iterator vecIt = m_list.begin();
      vecIt != m_list.end(); ++vecIt)
  {
    m_histo_summed[vecIt->second] = NULL;
    m_bkgHisto_summed[vecIt->second] = NULL;
    m_sigHisto_summed[vecIt->second] = NULL;
    m_dataHisto_summed[vecIt->second] = NULL;
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
  
  
  // Initialize::initialize summed vectors
  for(std::vector<std::pair<std::string, std::string> >::const_iterator vecIt = m_list.begin();
      vecIt != m_list.end(); ++vecIt)
  {
    m_crossSection_summed[vecIt->second] += m_crossSection[vecIt->first];
    m_scaleFactor_summed[vecIt->second]   = m_scaleFactor[vecIt->first];
    m_color_summed[vecIt->second]         = m_color[vecIt->first];
    m_linestyle_summed[vecIt->second]     = m_linestyle[vecIt->first];
    m_fillstyle_summed[vecIt->second]     = m_fillstyle[vecIt->first];
    m_mH_summed[vecIt->second]            = m_mH[vecIt->first];
    m_dataFlag_summed[vecIt->second]      = m_dataFlag[vecIt->first];
    
    if(  m_histo_summed[vecIt->second] != NULL ) delete  m_histo_summed[vecIt->second];
    if(  m_bkgHisto_summed[vecIt->second] != NULL ) delete  m_bkgHisto_summed[vecIt->second];
    if(  m_sigHisto_summed[vecIt->second] != NULL ) delete  m_sigHisto_summed[vecIt->second];
    if(  m_dataHisto_summed[vecIt->second] != NULL ) delete  m_dataHisto_summed[vecIt->second];
    
    m_histo_summed[vecIt->second] = NULL;
    m_bkgHisto_summed[vecIt->second] = NULL;
    m_sigHisto_summed[vecIt->second] = NULL;
    m_dataHisto_summed[vecIt->second] = NULL;
  }
  
  // Initialize::initialize global histograms
  if( m_globalHisto != NULL ) delete m_globalHisto; 
  if( m_bkgGlobalHisto != NULL ) delete m_bkgGlobalHisto; 
  if( m_sigGlobalHisto != NULL ) delete m_sigGlobalHisto; 
  if( m_dataGlobalHisto != NULL ) delete m_dataGlobalHisto; 
  
  m_globalHisto = NULL;
  m_bkgGlobalHisto = NULL;
  m_sigGlobalHisto = NULL;
  m_dataGlobalHisto = NULL;
  
  m_bkgGlobalIntegral = 0.;
  m_dataGlobalIntegral = 0.;
  
  
  
  // Initialize::clear vector with root files
  CloseRootFiles();
   m_rootFiles.clear();
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
    float m_xWidth = (m_xRangeMax-m_xRangeMin) / nBins;
    char xRangeMinChar[50];
    sprintf(xRangeMinChar,"%.10f",m_xRangeMin);
    char xRangeMaxChar[50];
    sprintf(xRangeMaxChar,"%.10f",m_xRangeMax);
    TH1F* histoTemp = new TH1F((histoName+"Temp").c_str(),"",nBins,  m_xRangeMin, m_xRangeMax);
    histoTemp -> Sumw2();
    
    // to include the upper edge of the last bin
    float* new_bins = new float[nBins+1];
    for(int bin = 0; bin <= nBins; ++bin) new_bins[bin] = histoTemp->GetBinLowEdge(bin+1);
    new_bins[nBins] = new_bins[nBins] + 0.001*m_xWidth;
    TAxis* axis = histoTemp -> GetXaxis();
    axis -> Set(nBins,new_bins);
    
    TH1F* histo = new TH1F(histoName.c_str(),"",nBins+2,m_xRangeMin-m_xWidth,m_xRangeMax+m_xWidth);
    histo -> Sumw2();
    
    for(unsigned int jj = 0; jj < variableNames.size(); ++jj)
    {
      //std::cout << "MakeHistogram::Dumping tree variable " << (variableNames.at(jj)+">>"+histoName).c_str() << std::endl;
      
      std::string cutExtended = m_generalCut;
      
      if(PURescale)
        cutExtended += " * (PUWeight)";
      if(weightEvent)
        cutExtended += " * (eventWeight)";
      if(cut)
        cutExtended += " * ("+cut->at(jj)+")";
      
      tree -> Draw( (variableNames.at(jj)+" >>+ "+histoName+"Temp").c_str(), cutExtended.c_str(),"goff");
      //std::cout << "nBins: " << nBins << "   min: " << m_xRangeMin << "   max: " <<m_xRangeMax << std::endl;
      //std::cout << (variableNames.at(jj)+" >>+ "+histoName+"Temp").c_str() << "      " << cutExtended.c_str() << std::endl;
    }
    
    histo -> SetBinContent(1,histoTemp->GetBinContent(0));
    histo -> SetBinError(1,histoTemp->GetBinError(0));
    for(int bin = 1; bin <= nBins; ++bin)
    {
      histo -> SetBinContent(bin+1,histoTemp->GetBinContent(bin));
      histo -> SetBinError(bin+1,histoTemp->GetBinError(bin));
    }

    histo -> SetBinContent(nBins+2,histoTemp->GetBinContent(nBins+1));
    histo -> SetBinError(nBins+2,histoTemp->GetBinError(nBins+1));
    
    
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
    delete histoTemp;
    delete histo;
    delete new_bins;
  } // MakeHistogram::end loop over all the samples
  
  
  
  
  
  
  //MakeHistogram::make stacks
  if( m_stack != NULL ) delete m_stack;
  if( m_bkgStack != NULL ) delete m_bkgStack;
  if( m_sigStack != NULL ) delete m_sigStack;
  
  m_stack = new THStack("stack","stack");
  m_bkgStack = new THStack("bkgStack","bkgStack");
  m_sigStack = new THStack("sigStack","sigStack");
  
  //MakeHistogram::make legend    
  if( m_legend != NULL ) delete m_legend;
  
  m_legend = new TLegend(m_xLowLegend, m_yLowLegend, m_xHighLegend, m_yHighLegend);
  m_legend -> SetFillColor(kWhite);
  m_legend -> SetFillStyle(4000);  
  m_legend -> SetTextFont(42);  
  m_legend -> SetTextSize(0.057);
  
  
  
  
  
  
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
        if( stackSig == true )
          histo_summed -> Scale(m_dataGlobalIntegral/(m_bkgGlobalIntegral+m_sigGlobalIntegral));
        else
        {
	  if( m_mH_summed[mapIt->first] <= 0 )
            histo_summed -> Scale(m_dataGlobalIntegral/m_bkgGlobalIntegral);
        }
      }
    }
    if( mode == "sameAreaNoStack" )
      histo_summed -> Scale(1./histo_summed->Integral());
    
    
    //MakeHistogram::graphical settings
    // y-axis properties
    histo_summed -> GetXaxis() -> SetTitleSize(0.071);
    histo_summed -> GetXaxis() -> SetLabelSize(0.057);
    histo_summed -> GetXaxis() -> SetTitleOffset(1.00);
    
    if( m_xAxisRange )
      histo_summed->GetXaxis()->SetRangeUser(m_xRangeMin-m_xWidth, m_xRangeMax+m_xWidth);
    
    std::string xAxisTitle = histoName;
    if( m_xAxisTitle ) xAxisTitle = m_xTitle;
    if( m_unit != "" ) xAxisTitle += " ("+m_unit+")";
    histo_summed->GetXaxis()->SetTitle(xAxisTitle.c_str());
    
    // y-axis properties
    histo_summed -> GetYaxis() -> SetTitleSize(0.071);
    histo_summed -> GetYaxis() -> SetLabelSize(0.057);
    histo_summed -> GetYaxis() -> SetTitleOffset(1.00);    
    
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
      histo_summed -> SetLineStyle(m_linestyle_summed[mapIt->first]);
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
          histo_summed -> SetFillColor(0);
          histo_summed -> SetFillStyle(0);
          histo_summed -> SetLineColor(m_color_summed[mapIt->first]);
          histo_summed -> SetLineWidth(2);
        }
      }
    }
    else
    {
      histo_summed -> SetMarkerStyle(20);
      histo_summed -> SetMarkerSize(0.7);
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
    std::string sampleName      = mapIt->first;
    std::string sampleNameClean = "";
    std::string::const_iterator strIt = sampleName.begin();
    int strCounter = 0;
    while( strIt != sampleName.end() )
    {
      if( (strCounter > 1) && ((*strIt) != '_') )
        sampleNameClean += *strIt;
      if( (strCounter > 1) && ((*strIt) == '_') )
        sampleNameClean += " ";
      
      ++strIt;
      ++strCounter;
    }
    
    if( (m_dataFlag_summed[mapIt->first] != 1) )
    {
      if( mode == "eventsScaled" )
        m_legend -> AddEntry(histo_summed, sampleNameClean.c_str(), "F");
      if( mode == "sameAreaNoStack" )
        m_legend -> AddEntry(histo_summed, sampleNameClean.c_str(), "L");
      if( mode == "sameAreaStack" )
        m_legend -> AddEntry(histo_summed, sampleNameClean.c_str(), "F");
      if( mode == "integralStack" )
        m_legend -> AddEntry(histo_summed, sampleNameClean.c_str(), "F");
      if( mode == "integralNoStack" )
        m_legend -> AddEntry(histo_summed, sampleNameClean.c_str(), "L");
    }
    else
      m_legend -> AddEntry(histo_summed, sampleNameClean.c_str(), "P");
    
    
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
  
  
  // FindMinimumMaximum::loop over summed samples to get single minimum and maximum
  for(std::map<std::string, double>::const_iterator mapIt = m_crossSection_summed.begin(); mapIt != m_crossSection_summed.end(); ++mapIt)
  {    
    TH1F* histo_summed = m_histo_summed[mapIt->first];
    if(m_xAxisRange) binMin = histo_summed -> FindBin(m_xRangeMin+0.5*histo_summed->GetBinWidth(1));
    if(m_xAxisRange) binMax = histo_summed -> FindBin(m_xRangeMax-0.5*histo_summed->GetBinWidth(1));
    if( histo_summed -> GetEntries() == 0 ) continue;
    
    // sample minimum
    if( MyGetMinimum(histo_summed, 1.E-15, binMin, binMax) < m_globalMinimum )
      m_globalMinimum = MyGetMinimum(histo_summed, 1.E-15, binMin, binMax);
    
    // sample maximum    
    if( MyGetMaximum(histo_summed, 1.E15, binMin, binMax) > m_globalMaximum )
      m_globalMaximum = MyGetMaximum(histo_summed, 1.E15, binMin, binMax);
  }
  
  
  // FindMinimumMaximum::loop over summed samples to get global maximum
  if( (mode == "eventsScaled") || (mode == "sameAreaStack") || (mode == "integralStack") || (mode == "eventsScaledStack") )
  {
    m_globalMinimum = 999999999999.;
    
    if( m_bkgGlobalHisto != NULL )
    {
      if( MyGetMaximum(m_bkgGlobalHisto, 1.E15, binMin, binMax) > m_globalMaximum )
        m_globalMaximum = MyGetMaximum(m_bkgGlobalHisto, 1.E15, binMin, binMax);
      
      if( MyGetMinimum(m_bkgGlobalHisto, 1.E-15, binMin, binMax) < m_globalMinimum )
        m_globalMinimum = MyGetMinimum(m_bkgGlobalHisto, 1.E-15, binMin, binMax);
    }
    
    if( m_dataGlobalHisto != NULL )
    {
      if( MyGetMaximum(m_dataGlobalHisto, 1.E15, binMin, binMax) > m_globalMaximum )
        m_globalMaximum = MyGetMaximum(m_dataGlobalHisto, 1.E15, binMin, binMax);
      
      if( MyGetMinimum(m_dataGlobalHisto, 1.E-15, binMin, binMax) < m_globalMinimum )
        m_globalMinimum = MyGetMinimum(m_dataGlobalHisto, 1.E-15, binMin, binMax);
    }
  }
  
  if( mode == "integralStack" )
    m_globalMaximum = 1.;
  
  
  
  if( m_globalMinimum < m_globalMaximum/1.E+08)
    m_globalMinimum = m_globalMaximum/1.E+08;
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
  
  m_xAxisRange = false;
  m_xAxisTitle = false;
  m_yAxisRange = false;
  m_yAxisTitle = false;
}






void drawTStack::Draw(TCanvas* c, const std::string& histoName, const std::string& mode, const bool& stackSig, const bool& logy, const float& lumi)
{
  c = new TCanvas("c","c",572,522);
  c -> cd();

  TPad* p1 = new TPad("p1","p1",0.0, 0.3, 1.0, 1.0);
  TPad* p2 = new TPad("p2","p2",0.0, 0.0, 1.0, 0.3);
  p1 -> SetTopMargin(0.08);
  p1 -> SetBottomMargin(0.02);
  p2 -> SetTopMargin(0.04);
  p2 -> SetBottomMargin(0.4);
  p1 -> SetGridx();
  p1 -> SetGridy();
  if( logy == true ) p1 -> SetLogy();
  p2 -> SetGridx();
  p2 -> SetGridy();
  p1 -> Draw();
  p2 -> Draw();
  
  TGraph* ratioGraph1s = NULL;
  TGraph* ratioGraph2s = NULL;
  TH1F* ratioHisto = NULL;
  TF1* line = NULL;
  TLatex* latex = NULL;
  
  THStack* firstStack = NULL;
  if( m_bkgGlobalHisto != NULL ) firstStack = m_bkgStack;
  else if( (m_bkgGlobalHisto == NULL) && (m_sigGlobalHisto != NULL) ) firstStack = m_sigStack;
  
  std::string xAxisTitle = histoName;
  if( m_xAxisTitle ) xAxisTitle = m_xTitle;
  if( m_unit != "" ) xAxisTitle += " ("+m_unit+")";
      
  
      
  // eventsScaled or sameAreaStack modes
  if( (mode == "eventsScaled") || (mode == "sameAreaStack") )
  {
    // p1
    p1 -> cd();
    
    if( stackSig == false )
    {
      if( m_bkgGlobalHisto != NULL )
      {
        m_bkgStack -> Draw("HISTO");
        if( m_sigGlobalHisto != NULL ) MyDraw(m_sigStack,"HISTO,same");
        DrawTStackError(m_bkgStack,false,0.);
      }
      else if( (m_bkgGlobalHisto == NULL) && (m_sigGlobalHisto != NULL) )
      {
        m_sigStack -> Draw("nostack,HISTO");      
      }
    }
    else
    {
      m_bkgStack -> Draw("HISTO");
    }
    
    if( m_dataGlobalHisto != NULL ) m_dataGlobalHisto -> Draw("P,same");
    
    // p2
    p2 -> cd();
    
    TH1F* den = m_dataGlobalHisto;
    if( (m_dataGlobalHisto == NULL) && (m_bkgGlobalHisto != NULL) )
    {
      TObjArray* histos = m_bkgStack -> GetStack();
      int nHistos = histos -> GetEntries();
      den = (TH1F*)(histos->At(nHistos-1))->Clone(); 
    }
    if( (m_dataGlobalHisto == NULL) && (m_bkgGlobalHisto == NULL) && (m_sigGlobalHisto != NULL) )
    {
      TObjArray* histos = m_sigStack -> GetStack();
      int nHistos = histos -> GetEntries();
      den = (TH1F*)(histos->At(nHistos-1))->Clone(); 
    }
    
    ratioGraph1s = new TGraph();
    ratioGraph2s = new TGraph();
    if( m_bkgGlobalHisto != NULL )
      ratioHisto = DrawTStackDataMCRatio(m_bkgStack,den,ratioGraph1s,ratioGraph2s);
    if( (m_bkgGlobalHisto == NULL) && (m_sigGlobalHisto != NULL) )    
      ratioHisto = DrawTStackDataMCRatio(m_sigStack,den,ratioGraph1s,ratioGraph2s);
    
    ratioHisto -> GetXaxis() -> SetLabelFont(42);
    ratioHisto -> GetXaxis() -> SetLabelSize(0.13);
    ratioHisto -> GetXaxis() -> SetTitle(xAxisTitle.c_str());
    ratioHisto -> GetXaxis() -> SetTitleFont(42);
    ratioHisto -> GetXaxis() -> SetTitleSize(0.17);
    ratioHisto -> GetXaxis() -> SetTitleOffset(0.95);
    
    ratioHisto -> GetYaxis() -> SetRangeUser(0.5, 1.5);
    ratioHisto -> GetYaxis() -> SetNdivisions(204,false);
    ratioHisto -> GetYaxis() -> SetLabelFont(42);
    ratioHisto -> GetYaxis() -> SetLabelSize(0.13);
    ratioHisto -> GetYaxis() -> SetTitle("data / MC");
    ratioHisto -> GetYaxis() -> SetTitleFont(42);
    ratioHisto -> GetYaxis() -> SetTitleSize(0.17);
    ratioHisto -> GetYaxis() -> SetTitleOffset(0.415);
    
    ratioHisto -> Draw("P");
    //ratioGraph2s -> Draw("F,same");
    ratioGraph1s -> Draw("F,same");
    ratioHisto -> Draw("P,same");
      
    line = new TF1("line", "1.", -1000000., 1000000.);
    line -> SetLineWidth(2);
    line -> SetLineColor(kBlack);
    line -> Draw("same");
    
    p1 -> cd();
  } 
  
  
  // sameAreaNoStack or integralNoStack modes  
  if( (mode == "sameAreaNoStack") || (mode == "integralNoStack") )
  {
    p1 -> cd();
    
    if( stackSig == false )
    {
      if( m_bkgGlobalHisto != NULL )
      {
        m_bkgStack -> Draw("nostack,HISTO");
        if( m_sigGlobalHisto != NULL) m_sigStack -> Draw("nostack,HISTO,same");
      }
      else if( (m_bkgGlobalHisto == NULL) && (m_sigGlobalHisto != NULL) )
      {
        m_sigStack -> Draw("nostack,HISTO");
      }
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
    m_bkgStack -> Draw("HISTO");
    m_sigStack -> Draw("HISTO,same");
    if(m_dataGlobalHisto != NULL) m_dataGlobalHisto -> Draw("P,same");
  }
  
  
  
  // set x-axis properties
  firstStack -> GetXaxis() -> SetLabelSize(0.);
  //firstStack -> GetXaxis() -> SetTitle(xAxisTitle.c_str());
  //firstStack -> GetXaxis() -> SetTitleSize(0.04);
  //firstStack -> GetXaxis() -> SetTitleOffset(1.25);
  if(m_xAxisRange) firstStack->GetXaxis()->SetRangeUser(m_xRangeMin-m_xWidth, m_xRangeMax+m_xWidth);
  
  
  // set y-axis properties
  firstStack->GetYaxis()->SetTitle(((TH1F*)(firstStack->GetStack()->Last()))->GetYaxis()->GetTitle());
  if(m_yAxisTitle) firstStack->GetYaxis()->SetTitle(m_yTitle.c_str());    
  firstStack -> GetYaxis() -> SetLabelSize(0.057);
  firstStack -> GetYaxis() -> SetTitleSize(0.071);
  firstStack -> GetYaxis() -> SetTitleOffset(1.00);
  
  
  if( logy == false)
  {
    firstStack -> SetMinimum(0.);
    firstStack -> SetMaximum(m_globalMaximum+0.1*m_globalMaximum);
    firstStack -> GetYaxis() -> SetRangeUser(0.,m_globalMaximum+0.1*m_globalMaximum);
  }
  else
  {
    firstStack -> SetMinimum(pow(10.,log10(m_globalMinimum) - 0.1));
    firstStack -> SetMaximum(pow(10.,log10(m_globalMaximum) + 0.1));
    firstStack -> GetYaxis() -> SetRangeUser(pow(10.,log10(m_globalMinimum) - 0.1),pow(10.,log10(m_globalMinimum) + 0.1));
  }
  
  if(m_yAxisRange)
  {
    firstStack->SetMinimum(m_yRangeMin);
    firstStack->SetMaximum(m_yRangeMax);
  }
  
  
  
  if(m_drawLegend == true)
  {
    m_legend -> Draw("same");
    
    // Draw::Define the caption
    char lumiBuffer[200];
    if( mode == "eventsScaled" )
      sprintf(lumiBuffer, "CMS Preliminary 2012   -   #sqrt{s}=8 TeV   L=%.1f fb^{-1}",lumi);
    if( mode == "sameAreaNoStack" )
      sprintf(lumiBuffer, "CMS Preliminary 2012   -   histograms normalized to unit area");
    if( mode == "sameAreaStack" )
      sprintf(lumiBuffer, "CMS Preliminary 2012   -   #sqrt{s}=8 TeV   L=%.1f fb^{-1}",lumi);
    if( mode == "integralStack" )
      sprintf(lumiBuffer, "CMS Preliminary 2012   -   integral distributions");
    if( mode == "integralNoStack" )
      sprintf(lumiBuffer, "CMS Preliminary 2012   -   integral distributions");
    
    latex = new TLatex(0.13, 0.96, lumiBuffer);
    
    latex->SetNDC();
    latex->SetTextFont(42);
    latex->SetTextSize(0.057);
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
  
  delete p1;
  delete p2;
  delete c;

  if( ratioGraph1s != NULL ) delete ratioGraph1s;
  if( ratioGraph2s != NULL ) delete ratioGraph2s;
  if( ratioHisto != NULL ) delete ratioHisto;
  if( line != NULL ) delete line;
  if( latex != NULL ) delete latex;
}






void drawTStack::DrawEvents(const std::string& mode,
                            const float& lumi,
                            const int& step,
                            const bool& logy,
                            const bool& PURescale,
                            const bool& stackSig)
{ 
  std::cout << "\n>>>plotUtils::Drawing " << mode;
  
  
  Initialize();
  
  
  // DrawEvents::loop over all the samples  
  std::string fullHistoName;
  int i = 0;
  for(std::vector<std::pair<std::string, std::string> >::const_iterator vecIt = m_list.begin(); vecIt != m_list.end(); ++vecIt)
  {
    // open root file
    std::string fullRootFileName = m_inputDir+vecIt->first+"/"+m_baseRootFileName+"_"+m_jetAlgorithm.at(i).second+".root";
    //std::cout << "opening file: " << fullRootFileName << std::endl;
    TFile* inFile = TFile::Open(fullRootFileName.c_str(),"READ");
    if( !inFile->IsOpen() )
    {
      std::cout << "\n>>>plotUtils::MakeHistograms::error in opening file " << fullRootFileName << std::endl;
      exit(-1);
    }
    m_rootFiles.push_back(inFile);
    
    
    // get histogram
    fullHistoName = "events";
    if( (mode != "events") && (PURescale == true) )
      fullHistoName = "events_PURescaled";
    
    TH1F* histo = (TH1F*)( m_rootFiles.at(i)->Get(fullHistoName.c_str()) );
    if(histo == NULL)
    {
      std::cout << ">>>plotUtils::Error in getting object " << fullHistoName;
      exit(-1);
    }
    
    if( mode == "eventsScaledStack" || mode == "eventsScaled" )
      histo -> Sumw2();
    
    
    // DrawEvents::scale histograms normalizing to lumi (in fb^-1)
    // DrawEvents::if data do not apply any scale factor    
    if( (mode != "events") && (m_dataFlag[vecIt->first] != 1) )
    {
      histo -> Scale(1000*lumi*m_crossSection[vecIt->first]/histo->GetBinContent(1));
    }
    
    
    // DrawEvents::add histograms with same summed name    
    // all
    if( m_histo_summed[vecIt->second] != NULL )
      m_histo_summed[vecIt->second] -> Add(histo);
    else
      m_histo_summed[vecIt->second] = (TH1F*)(histo -> Clone((fullHistoName+"_summed").c_str()));
    
    
    
    ++i;
  }
  
  
  
  // DrawEvents::make stacks
  if( m_stack != NULL ) delete m_stack;
  if( m_bkgStack != NULL ) delete m_bkgStack;
  if( m_sigStack != NULL ) delete m_sigStack;
  
  m_stack = new THStack("stack","stack");
  m_bkgStack = new THStack("bkgStack","bkgStack");
  m_sigStack = new THStack("sigStack","sigStack");
  
  
  // DrawEvents::make legend
  if( m_legend != NULL ) delete m_legend;
  
  m_legend = new TLegend(m_xLowLegend, m_yLowLegend, m_xHighLegend, m_yHighLegend);
  m_legend -> SetFillColor(kWhite);
  m_legend -> SetFillStyle(4000);
  m_legend -> SetTextFont(42);
  m_legend -> SetTextSize(0.057);
  
  // DrawEvents::make outfiles
  std::ofstream* outFile = new std::ofstream((m_outputDir+mode+".txt").c_str(), std::ios::out);
  
  int nSamples = m_crossSection_summed.size();
  TH1F* stepHisto = new TH1F("stepHisto", "", nSamples, 0., 1.*nSamples);
  
  
  
  
  
  
  // DrawEvents::loop over summed samples
  // DrawEvents::make global histograms
  bool isFirstSample  = true;
  bool isFirstBkgSample  = true;
  bool isFirstSigSample  = true;
  bool isFirstDataSample = true;
  i = 0;
  std::map<int, std::string> binLabels;
  std::map<int, float> nEventsScaled_sig;
  std::map<int, float> nEventsScaled_bkg;
  
  for(std::map<std::string, double>::const_iterator mapIt = m_crossSection_summed.begin(); mapIt != m_crossSection_summed.end(); ++mapIt)
  {    
    TH1F* histo_summed = m_histo_summed[mapIt->first];
    
    
    // DrawEvents::set normalizations    
    if( mode == "efficiencies" )
    {
      float totalEvents = histo_summed -> GetBinContent(1);
      for(int bin = 1; bin <= histo_summed->GetNbinsX(); ++bin)
        histo_summed -> SetBinContent(bin,1.*histo_summed->GetBinContent(bin)/totalEvents);
    }
    
    if(mode == "efficienciesRelative")
    {
      std::map<int, float> totalEvents;
      totalEvents[0] = histo_summed -> GetBinContent(1);
      for(int bin = 1; bin <= histo_summed->GetNbinsX(); ++bin)
        totalEvents[bin] = histo_summed -> GetBinContent(bin);
      
      for(int bin = 1; bin <= histo_summed->GetNbinsX(); ++bin)
      {        
        if(totalEvents[bin-1] > 0.)
          histo_summed -> SetBinContent(bin,1.*histo_summed->GetBinContent(bin)/totalEvents[bin-1]);
        else
          histo_summed -> SetBinContent(bin,0.);
      }
    }
    
    
    
    // DrawEvents::remove first bin    
    if( (mode == "eventsScaled") || (mode == "eventsScaledStack") || (mode == "efficiencies") || (mode == "efficienciesRelative") )
    {
      int nBins = histo_summed->GetNbinsX()-1;
      TH1F* histo = new TH1F(("events_"+mapIt->first+"_new").c_str(),"",nBins,0.,1.*nBins);
      for(int bin = 2; bin <= histo_summed->GetNbinsX(); ++bin)
      {
        histo -> SetBinContent(bin-1,1.*histo_summed->GetBinContent(bin));
        histo -> GetXaxis() -> SetBinLabel(bin-1,histo_summed->GetXaxis()->GetBinLabel(bin));
        if( mode == "eventsScaledStack" )
          histo -> SetBinError(bin-1,1.*histo_summed->GetBinError(bin));
      }
      histo_summed = histo;
      m_histo_summed[mapIt->first] = histo;
    }
    
    
    
    // DrawEvents::graphical settings
    // x-axis properties
    histo_summed -> GetXaxis() -> SetLabelSize(0.057);
    histo_summed -> GetXaxis() -> SetTitleSize(0.071);
    histo_summed -> GetXaxis() -> SetTitleOffset(1.00);
    
    // y-axis properties
    histo_summed -> GetYaxis() -> SetLabelSize(0.057);
    histo_summed -> GetYaxis() -> SetTitleSize(0.071);
    histo_summed -> GetYaxis() -> SetTitleOffset(1.00);
    
    if( m_yAxisRange )
      histo_summed->GetYaxis()->SetRangeUser(m_yRangeMin, m_yRangeMax);
    
    char binWidthBuffer[50];
    sprintf(binWidthBuffer, "%.2e", histo_summed->GetBinWidth(1));
    if( mode == "events" )
      histo_summed->GetYaxis()->SetTitle("events");
    if( mode == "eventsScaled" )
      histo_summed->GetYaxis()->SetTitle("events");
    if( mode == "eventsScaledStack" )
      histo_summed->GetYaxis()->SetTitle("events");
    if( mode == "efficiencies" )
      histo_summed->GetYaxis()->SetTitle("#varepsilon");
    if( mode == "efficienciesRelative" )
      histo_summed->GetYaxis()->SetTitle("#varepsilon");
    if( mode == "significance" )
      histo_summed->GetYaxis()->SetTitle("S / #sqrt{S+B}");
    
    // colors
    if( m_dataFlag_summed[mapIt->first] != 1 )
    {
      histo_summed -> SetFillColor(m_color_summed[mapIt->first]);
      histo_summed -> SetFillStyle(m_fillstyle_summed[mapIt->first]);
      histo_summed -> SetLineStyle(m_linestyle_summed[mapIt->first]);
      if( mode != "eventsScaledStack" )
      {
        histo_summed -> SetFillStyle(0);
        histo_summed -> SetLineColor(m_color_summed[mapIt->first]);
        histo_summed -> SetLineWidth(3);
      }
      if( mode == "eventsScaledStack" )
      {
        histo_summed -> SetLineColor(kBlack);
        histo_summed -> SetLineWidth(1);
        if(m_mH_summed[mapIt->first] > 0.)
        {
          histo_summed -> SetFillColor(0);
          histo_summed -> SetFillStyle(0);
          histo_summed -> SetLineColor(m_color_summed[mapIt->first]);
          histo_summed -> SetLineWidth(2);
        }
      }
    }
    else
    {
      histo_summed -> SetMarkerStyle(20);
      histo_summed -> SetMarkerSize(0.7);
      histo_summed -> SetMarkerColor(kBlack);      
    }
    
    
    
    // DrawEvents::make global histograms
    // all
    if( (isFirstSample == false) && (m_dataFlag_summed[mapIt->first] != 1) )
    {
      m_globalHisto -> Add(histo_summed);
    }
    if( (isFirstSample == true) && (m_dataFlag_summed[mapIt->first] != 1) )
    {
      m_globalHisto = (TH1F*)(histo_summed -> Clone((fullHistoName+"_global").c_str()));
      isFirstSample = false;
    }
    // bkg
    if( (isFirstBkgSample == false) && (m_dataFlag_summed[mapIt->first] != 1) && (m_mH_summed[mapIt->first] <= 0) )
    {
      m_bkgGlobalHisto -> Add(histo_summed);
    }
    if( (isFirstBkgSample == true) && (m_dataFlag_summed[mapIt->first] != 1) && (m_mH_summed[mapIt->first] <= 0) )
    {
      m_bkgGlobalHisto = (TH1F*)(histo_summed -> Clone((fullHistoName+"_bkgGlobal").c_str()));
      isFirstBkgSample = false;
    }
    // sig
    if( (isFirstSigSample == false) && (m_dataFlag_summed[mapIt->first] != 1) && (m_mH_summed[mapIt->first] > 0) )
    {
      m_sigGlobalHisto -> Add(histo_summed);
    }
    if( (isFirstSigSample == true) && (m_dataFlag_summed[mapIt->first] != 1) && (m_mH_summed[mapIt->first] > 0) )
    {
      m_sigGlobalHisto = (TH1F*)(histo_summed -> Clone((fullHistoName+"_sigGlobal").c_str()));
      isFirstSigSample = false;
    }
    // data
    if( (isFirstDataSample == false) && (m_dataFlag_summed[mapIt->first] == 1) && (m_mH_summed[mapIt->first] <= 0) )
    {
      m_dataGlobalHisto -> Add(histo_summed);
    }
    if( (isFirstDataSample == true) && (m_dataFlag_summed[mapIt->first] == 1) && (m_mH_summed[mapIt->first] <= 0) )
    {
      m_dataGlobalHisto = (TH1F*)(histo_summed -> Clone((fullHistoName+"_dataGlobal").c_str()));
      isFirstDataSample = false;
    }
    
    
    
    // DrawEvents::add summed histograms to the stacks
    // all
    if( (m_dataFlag_summed[mapIt->first] != 1) )
      m_stack -> Add(histo_summed);
    // bkg
    if( (m_dataFlag_summed[mapIt->first] != 1) && (m_mH_summed[mapIt->first] <= 0) )
      m_bkgStack -> Add(histo_summed);
    // sig
    if( (m_dataFlag_summed[mapIt->first] != 1) && (m_mH_summed[mapIt->first] > 0) )
      m_sigStack -> Add(histo_summed);
    
    
    
    // DrawEvents::add summed histograms to the legend
    std::string sampleName      = mapIt->first;
    std::string sampleNameClean = "";
    std::string::const_iterator strIt = sampleName.begin();
    int strCounter = 0;
    while( strIt != sampleName.end() )
    {
      if( (strCounter >1) && ((*strIt) != '_') )
        sampleNameClean += *strIt;
      if( (strCounter >1) && ((*strIt) == '_') )
        sampleNameClean += " ";
      
      ++strIt;
      ++strCounter;
    }
    
    if( (m_dataFlag_summed[mapIt->first] != 1) )
    {
      if( mode == "events" )
        m_legend -> AddEntry(histo_summed, sampleNameClean.c_str(), "L");
      if( mode == "eventsScaled" )
        m_legend -> AddEntry(histo_summed, sampleNameClean.c_str(), "L");
      if( mode == "eventsScaledStack" )
        m_legend -> AddEntry(histo_summed, sampleNameClean.c_str(), "F");
      if( mode == "efficiencies" )
        m_legend -> AddEntry(histo_summed, sampleNameClean.c_str(), "L");
      if( mode == "efficienciesRelative" )
        m_legend -> AddEntry(histo_summed, sampleNameClean.c_str(), "L");
      if( mode == "significance" )
        m_legend -> AddEntry(histo_summed, sampleNameClean.c_str(), "L");
    }
    else
      m_legend -> AddEntry(histo_summed, sampleNameClean.c_str(), "P");    
    
    
    
    // DrawEvents::fill outfiles
    stepHisto -> SetBinContent(i+1,histo_summed->GetBinContent(step));
    stepHisto -> GetXaxis() -> SetBinLabel(i+1,(mapIt->first).c_str());
    
    (*outFile) << "\n" << mapIt->first;
    for(int bin = 1; bin <= histo_summed->GetNbinsX(); ++bin)
    {
      const char* binLabel = histo_summed -> GetXaxis() -> GetBinLabel(bin);
      (*outFile) << "   " << binLabel << ":   " << std::fixed << std::setprecision(3) << std::scientific << 1.*histo_summed->GetBinContent(bin); 
    }
    (*outFile) << "\n";
    
    // DrawEvents::fill event counters    
    for(int bin = 1; bin <= histo_summed->GetNbinsX(); ++bin)
    {
      binLabels[bin] = histo_summed->GetXaxis()->GetBinLabel(bin);
      if( (m_mH_summed[mapIt->first] > 0.) && (m_dataFlag_summed[mapIt->first] != 1) )
        nEventsScaled_sig[bin] += histo_summed->GetBinContent(bin)/m_scaleFactor_summed[mapIt->first];
      else if( (m_mH_summed[mapIt->first] <= 0.) && (m_dataFlag_summed[mapIt->first] != 1) )
        nEventsScaled_bkg[bin] += histo_summed->GetBinContent(bin);
    }
    
    
    
    ++i;
  }
  
  

  // DrawEvents::stack signal, if requested
  if( stackSig == true )
  {
    for(std::map<std::string, double>::const_iterator mapIt = m_crossSection_summed.begin(); mapIt != m_crossSection_summed.end(); ++mapIt)
    {
      TH1F* histo_summed = m_histo_summed[mapIt->first];
      if( histo_summed->GetEntries() == 0 ) continue;
      if( m_mH_summed[mapIt->first] > 0 ) m_bkgStack -> Add(histo_summed);
    }
  }
  
  
  
  // DrawEvents::compute the axis ranges
  FindMinimumMaximum(mode);
  
  
  
  // DrawEvents::draw the stack and save file
  struct stat st;
  c1 = new TCanvas("c","c",800,800);
  c1 -> cd();
  TPad* p1 = new TPad("p1","p1",0.0, 0.3, 1.0, 1.0);
  TPad* p2 = new TPad("p2","p2",0.0, 0.0, 1.0, 0.3);
  p1 -> SetTopMargin(0.08);
  p1 -> SetBottomMargin(0.02);
  p2 -> SetTopMargin(0.04);
  p2 -> SetBottomMargin(0.4);
  p1 -> SetGridx();
  p1 -> SetGridy();
  if( logy == true ) p1 -> SetLogy();
  p2 -> SetGridx();
  p2 -> SetGridy();
  p2 -> Draw();
  p1 -> Draw();
  
  TGraph* ratioGraph1s = NULL;
  TGraph* ratioGraph2s = NULL;
  TH1F* ratioHisto = NULL;
  TF1* line = NULL;
  TLatex* latex = NULL;
  
  THStack* firstStack = NULL;
  if( m_bkgGlobalHisto != NULL ) firstStack = m_bkgStack;
  else if( (m_bkgGlobalHisto == NULL) && (m_sigGlobalHisto != NULL) ) firstStack = m_sigStack;  
  
  
  
  // eventsScaledStack mode
  if( mode == "eventsScaledStack" )
  {
    // p1
    p1 -> cd();
    
    if( stackSig == false )
    {
      if( m_bkgGlobalHisto != NULL )
      {
        m_bkgStack -> Draw("HISTO");
        if( m_sigGlobalHisto != NULL ) MyDraw(m_sigStack,"HISTO,same");
      }
      else if( (m_bkgGlobalHisto == NULL) && (m_sigGlobalHisto != NULL) )
      {
        m_sigStack -> Draw("nostack,HISTO");
      }
    }
    else
    {
      m_bkgStack -> Draw("HISTO");
    }
    
    if( m_bkgGlobalHisto != NULL )  DrawTStackError(m_bkgStack,false,0.);
    if( m_dataGlobalHisto != NULL ) m_dataGlobalHisto -> Draw("P,same");
    
    // p2
    p2 -> cd();
    
    TH1F* den = m_dataGlobalHisto;
    if( (m_dataGlobalHisto == NULL) && (m_bkgGlobalHisto != NULL) )
    {
      TObjArray* histos = m_bkgStack -> GetStack();
      int nHistos = histos -> GetEntries();
      den = (TH1F*)(histos->At(nHistos-1))->Clone(); 
    }
    if( (m_dataGlobalHisto == NULL) && (m_bkgGlobalHisto == NULL) && (m_sigGlobalHisto != NULL) )
    {
      TObjArray* histos = m_sigStack -> GetStack();
      int nHistos = histos -> GetEntries();
      den = (TH1F*)(histos->At(nHistos-1))->Clone(); 
    }
    
    ratioGraph1s = new TGraph();
    ratioGraph2s = new TGraph();
    
    if( m_bkgGlobalHisto != NULL )
      ratioHisto = DrawTStackDataMCRatio(m_bkgStack,den,ratioGraph1s,ratioGraph2s);
    if( (m_bkgGlobalHisto == NULL) && (m_sigGlobalHisto != NULL) )
      ratioHisto = DrawTStackDataMCRatio(m_sigStack,den,ratioGraph1s,ratioGraph2s);
    
    ratioHisto -> GetXaxis() -> SetLabelFont(42);
    ratioHisto -> GetXaxis() -> SetLabelSize(0.13);
    ratioHisto -> GetXaxis() -> SetTitleFont(42);
    ratioHisto -> GetXaxis() -> SetTitleSize(0.17);
    ratioHisto -> GetXaxis() -> SetTitleOffset(0.95);
    
    ratioHisto -> LabelsOption("v");
    ratioHisto -> GetYaxis() -> SetRangeUser(0.5, 1.5);
    ratioHisto -> GetYaxis() -> SetNdivisions(204,false);
    ratioHisto -> GetYaxis() -> SetLabelFont(42);
    ratioHisto -> GetYaxis() -> SetLabelSize(0.13);
    ratioHisto -> GetYaxis() -> SetTitle("data / MC");
    ratioHisto -> GetYaxis() -> SetTitleFont(42);
    ratioHisto -> GetYaxis() -> SetTitleSize(0.17);
    ratioHisto -> GetYaxis() -> SetTitleOffset(0.415);
    
    ratioHisto -> Draw("P");
    //ratioGraph2s -> Draw("F,same");
    ratioGraph1s -> Draw("F,same");
    ratioHisto -> Draw("P,same");
      
    line = new TF1("line", "1.", -1000000., 1000000.);
    line -> SetLineWidth(2);
    line -> SetLineColor(kRed);
    line -> Draw("same");
    
    p1 -> cd();
  } 
  
  // other modes  
  else
  {
    p1 -> cd();
    
    if( stackSig == false )
    {
      if( m_bkgGlobalHisto != NULL )
      {
        m_bkgStack -> Draw("nostack,HISTO");
        m_sigStack -> Draw("nostack,HISTO,same");
      }
      else if( (m_bkgGlobalHisto == NULL) && (m_sigGlobalHisto != NULL) )
      {
        m_sigStack -> Draw("nostack,HISTO");
      }
    }
    else
    {
      m_bkgStack -> Draw("nostack,HISTO");    
    }
    
    if(m_dataGlobalHisto != NULL) m_dataGlobalHisto -> Draw("P,same");
  }
  
  
  
  // set x-axis properties
  firstStack -> GetXaxis() -> SetTitleSize(0.057);
  firstStack -> GetXaxis() -> SetLabelSize(0.071);
  
  // set y-axis properties
  firstStack->GetYaxis()->SetTitle(((TH1F*)(firstStack->GetStack()->Last()))->GetYaxis()->GetTitle());
  if(m_yAxisTitle) firstStack->GetYaxis()->SetTitle(m_yTitle.c_str());    
  if( mode == "eventsScaledStack" )
    firstStack -> GetXaxis() -> SetLabelSize(0.);
  firstStack -> GetYaxis() -> SetLabelSize(0.057);
  firstStack -> GetYaxis() -> SetTitleSize(0.071);
  firstStack -> GetYaxis() -> SetTitleOffset(1.00);
  
  if( logy == false)
  {
    firstStack -> SetMinimum(0.);
    firstStack -> SetMaximum(m_globalMaximum+0.1*m_globalMaximum);
  }
  else
  {
    firstStack -> SetMinimum(pow(10.,log10(m_globalMinimum) - 0.1));
    firstStack -> SetMaximum(pow(10.,log10(m_globalMaximum) + 0.1));
  }
  
  if(m_yAxisRange)
  {
    firstStack->SetMinimum(m_yRangeMin);
    firstStack->SetMaximum(m_yRangeMax);
  }
  
  
  
  if(m_drawLegend == true)
  {
    m_legend -> Draw("same");
    
    // Draw::Define the caption
    char lumiBuffer[200];
    if( mode == "events" )
      sprintf(lumiBuffer, "CMS Preliminary 2012   -   no normalization");
    if( mode == "eventsScaled" )
      sprintf(lumiBuffer, "CMS Preliminary 2012   -   MC normalized to L=%.1f fb^{-1}",lumi);
    if( mode == "eventsScaledStack" )
      sprintf(lumiBuffer, "CMS Preliminary 2012   -   #sqrt{s}=8 TeV   L=%.1f fb^{-1}",lumi);
    if( mode == "efficiencies" )
      sprintf(lumiBuffer, "CMS Preliminary 2012   -   efficiencies");
    if( mode == "efficienciesRelative" )
      sprintf(lumiBuffer, "CMS Preliminary 2012   -   relative efficiencies");
    if( mode == "significance" )
      sprintf(lumiBuffer, "CMS Preliminary 2012   -   significance");
    
    latex = new TLatex(0.13, 0.96, lumiBuffer);
    
    latex->SetNDC();
    latex->SetTextFont(42);
    latex->SetTextSize(0.057);
    latex->Draw("same");
  }
  
  
  if(stat(m_outputDir.c_str(), &st) != 0)
  {
    std::cout << ">>>plotUtils::Error accessing directory " << m_outputDir;
    exit(-1);
  }
  c1->Print((m_outputDir+mode+"."+m_imgFormat).c_str(), m_imgFormat.c_str());
  
  delete p1;
  delete p2;
  delete c1;

  if( ratioGraph1s != NULL ) delete ratioGraph1s;
  if( ratioGraph2s != NULL ) delete ratioGraph2s;
  if( ratioHisto != NULL ) delete ratioHisto;
  if( line != NULL ) delete line;
  if( latex != NULL ) delete latex;  
  
  
  
  
  
  
  if( (mode == "eventsScaledStack") && (m_bkgGlobalHisto != NULL) && (m_sigGlobalHisto != NULL) )
  {
    (*outFile) << "\n";
    
    TH1F* significance = NULL;
    for(unsigned int bin = 1; bin <= nEventsScaled_sig.size(); ++bin)
    {
      (*outFile) << std::setw(30) << binLabels[bin]
                 << "   S = "           << nEventsScaled_sig[bin]
                 << "   B = "           << nEventsScaled_bkg[bin]
                 << "   S/sqrt(S+B) = " << nEventsScaled_sig[bin]/sqrt(nEventsScaled_sig[bin]+nEventsScaled_bkg[bin])
                 << std::endl;
      
      if( significance == NULL )
        significance = (TH1F*)( ((m_bkgStack->GetStack())->Last()) -> Clone("significance") );
      
      significance -> SetBinContent(bin,nEventsScaled_sig[bin]/sqrt(nEventsScaled_sig[bin]+nEventsScaled_bkg[bin]));
      significance -> SetBinError(bin,0.);
    }
    
    if( significance != NULL )
    {
      c1 = new TCanvas();
      c1 -> cd();
      c1 -> SetGridx();
      c1 -> SetGridy();
      
      significance -> GetXaxis() -> SetTitleSize(0.071);
      significance -> GetXaxis() -> SetLabelSize(0.057);
      significance -> GetXaxis() -> SetTitleOffset(0.95);
      
      significance -> GetYaxis() -> SetTitleSize(0.071);
      significance -> GetYaxis() -> SetLabelSize(0.057);
      significance -> GetYaxis() -> SetTitle("S / #sqrt{S+B}");
      significance -> GetYaxis() -> SetTitleOffset(1.00);
      significance -> GetYaxis() -> SetRangeUser(0.,2.);

      significance -> SetLineColor(kRed);
      significance -> SetFillStyle(0);
      significance -> SetLineWidth(3);
      
      significance -> Draw("L");
    
      if(stat(m_outputDir.c_str(), &st) != 0)
      {
        std::cout << ">>>plotUtils::Error accessing directory " << m_outputDir;
        exit(-1);
      }
      c1->Print( (m_outputDir+"significance."+m_imgFormat).c_str(), m_imgFormat.c_str());
      
      delete c1;    
      delete significance;
    }
  }
  
  
  
  c1 = new TCanvas();
  c1 -> cd();
  c1 -> SetGridx();
  c1 -> SetGridy();
  c1 -> SetLogy();
  
  stepHisto -> GetXaxis() -> SetTitleSize(0.04);
  stepHisto -> GetXaxis() -> SetLabelSize(0.03);
  stepHisto -> GetXaxis() -> SetTitleOffset(0.95);

  stepHisto -> GetYaxis() -> SetTitleSize(0.04);
  stepHisto -> GetYaxis() -> SetLabelSize(0.03);
  stepHisto -> GetYaxis() -> SetTitleOffset(1.00);
  
  stepHisto -> SetLineWidth(3);
  
  stepHisto -> Draw();

  if(stat(m_outputDir.c_str(), &st) != 0)
  {
    std::cout << ">>>plotUtils::Error accessing directory " << m_outputDir;
    exit(-1);
  }
  c1->Print( (m_outputDir+"step_"+mode+"."+m_imgFormat).c_str(), m_imgFormat.c_str());
  
  delete c1;
  
  
  
  m_xAxisRange = false;
  m_xAxisTitle = false;
  m_yAxisRange = false;
  m_yAxisTitle = false;
  
  // save stepHisto root file
  outFile -> close();
  
  //TFile* outRootFile = new TFile((m_outputDir+mode+"_step.root").c_str(), "RECREATE");
  //outRootFile -> cd();
  //stepHisto -> Write();
  //outRootFile -> Close();
  //delete stepHisto;
  //delete outRootFile;
  
  delete stepHisto;
}






void drawTStack::CloseRootFiles()
{
  for(unsigned int i = 0; i < m_rootFiles.size(); ++i)
  {
    m_rootFiles.at(i) -> Close();
    delete m_rootFiles.at(i);
  }
}






void drawTStack::SetGeneralCut(const std::string& generalCut)
{
  m_generalCut = generalCut;
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


void MyDraw(THStack* hs, const std::string& option)
{
  TObjArray* histos = hs -> GetStack();
  for(int histIt = histos->GetEntries()-1; histIt >=0 ; --histIt) 
  {
    TH1F* histo = (TH1F*)( histos -> At(histIt) );
    histo -> Draw(option.c_str());
  }
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
  ratioGraph1s -> SetFillColor(15);
  ratioGraph1s -> SetLineColor(15);
  ratioGraph2s -> Set(2*nPoints);
  ratioGraph2s -> SetFillColor(12);
  ratioGraph2s -> SetLineColor(12);
  
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


//-------------------------------------------------------------------


void setErrorForEmptyBins (TH1F * input)
{
  int counter = 0;
  double weight = 0;
  double weightSum = 0;
  
  for (int iBin = 1 ; iBin <= input->GetNbinsX () ; ++iBin)
    {
      //skip empty bins
      if (input->GetBinContent(iBin) == 0) continue;
      //evaluate the weight for every bin
      weight = input->GetBinError(iBin)*input->GetBinError(iBin) / input->GetBinContent(iBin);
      
      weightSum += weight;
      counter++;    
    }
  
  //weight = mean of all the previously evaluated weights
  weight = weightSum/counter;

  //set this weight as the error of the bins with zero content
  for (int iBin = 1 ; iBin <= input->GetNbinsX () ; ++iBin)
    {
      if (input->GetBinContent(iBin) == 0) input->SetBinError(iBin, weight);
    }
  
  return ;
}


//-------------------------------------------------------------------


void setErrorForEmptyBins_v2 (TH1F * input)
{
  double errorUp   = 0;
  double errorDown = 0;
  
  for (int iBin = 1 ; iBin <= input->GetNbinsX () ; ++iBin)
    {
      //skip non-empty bins
      if (input->GetBinContent(iBin) != 0) continue;
      
      //set the j position on the empty bin
      int j = iBin;

      //if you are not on the first bin
      if (j != 1) {

        while (input->GetBinContent(j) == 0 && j > 1) {
           j--;
           errorDown = input->GetBinError(j);
         }
      }

      j = iBin;
      
      //if you are not on the last bin
      if (j != input->GetNbinsX()) {

        while (input->GetBinContent(j) == 0 && j < input->GetNbinsX()) {
           j++;
           errorUp = input->GetBinError(j);        
         }
      }


      if (errorUp == 0)   input->SetBinError(iBin, errorDown);
      if (errorDown == 0) input->SetBinError(iBin, errorUp);
      
      //set the error of the empty bin as the mean of errorUp and errorDown
      else input->SetBinError(iBin, (errorDown+errorUp)/2.);
      
      errorUp   = 0;
      errorDown = 0;

    }
  
  return ;
}
