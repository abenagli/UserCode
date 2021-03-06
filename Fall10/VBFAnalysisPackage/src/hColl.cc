#include "hColl.h"
#include "THStack.h"

#include "plotUtils.h"

#include <iostream>

using namespace std ;

hColl::hColl (string pN, int b, Float_t * xAxis) :
 plotName (pN),
 bins (b),
 axis (xAxis),
 logB (0), 
 isBinWidthNormalized (0)
  {
    colors.push_back (kBlue+2   ) ;
    colors.push_back (kRed      ) ;
    colors.push_back (10        ) ;
    colors.push_back (kCyan+2   ) ;
    colors.push_back (kOrange+7 ) ;
    colors.push_back (kGray     ) ;
    colors.push_back (kMagenta+1) ;
    colors.push_back (kGreen+2  ) ;
    colors.push_back (kOrange   ) ;
    colors.push_back (kViolet+2 ) ;
    colors.push_back (kRed+3    ) ;
  } 


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


hColl::hColl (string pN, int b, double m, double M, bool logBins) :
 plotName (pN),
 bins (b),
 min (m),
 max (M), 
 axis (0), 
 logB (logBins), 
 isBinWidthNormalized (0)
  {
    colors.push_back (kBlue+2   ) ;
    colors.push_back (kRed      ) ;
    colors.push_back (10        ) ;
    colors.push_back (kCyan+2   ) ;
    colors.push_back (kOrange+7 ) ;
    colors.push_back (kGray     ) ;
    colors.push_back (kMagenta+1) ;
    colors.push_back (kGreen+2  ) ;
    colors.push_back (kOrange   ) ;
    colors.push_back (kViolet+2 ) ;
    colors.push_back (kRed+3    ) ;
  } 


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


hColl::~hColl ()
{
  for (unsigned int i = 0 ; i < collection.size () ; ++i) 
    {
      delete collection.at (i) ;
    }
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


TH1F *
hColl::addSample (std::string sampleName) 
{

  TH1F * dummy ; 
  if (axis == 0) dummy = new TH1F ((plotName + "_" + sampleName).c_str (), sampleName.c_str (), bins, min, max) ;
  else if (logB)
    {
      int nLogBins = bins ;
      float xLogMin = 1. ;
      float xLogMax = 3. ;
      //float xLogWidth = (xLogMax-xLogMin) / nLogBins ;
      dummy = new TH1F ((plotName + "_" + sampleName).c_str (), sampleName.c_str (), nLogBins, xLogMin, xLogMax) ;
      BinLogX (dummy) ;
    }
  else dummy = new TH1F ((plotName + "_" + sampleName).c_str (), sampleName.c_str (), bins, axis) ;
  dummy->SetStats (0) ;
  dummy->SetFillColor (colors.at (collection.size ())) ;
//  dummy->SetFillColor (20 + 5 * collection.size ()) ;
  collection.push_back (dummy) ;
  return dummy ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void
hColl::save (TFile * f) 
{
  f->cd () ;
  if (collection.size () == 0) 
    {
      cout << "nothing to be saved for " << plotName << endl ;
      return ;
    }
  THStack stack (("stack_" + plotName).c_str (), "") ;
  for (unsigned int i = 0 ; i < collection.size () ; ++i) 
    {
      collection.at (i)->Write () ;
      stack.Add (collection.at (i)) ;
    }  
  stack.Write () ;
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


THStack
hColl::makeStack () 
{
  THStack stack (("stack_" + plotName).c_str (), "") ;
  for (unsigned int i = 0 ; i < collection.size () ; ++i) 
    {
      stack.Add (collection.at (i)) ;
    }  
  return stack ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


TH1F * 
hColl::findHisto (string name)
{
  for (unsigned int i = 0 ; i < collection.size () ; ++i)
    {
      if (string (collection.at (i)->GetName ()) == name)
        return collection.at (i) ;
    }
  return 0 ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void 
hColl::normToBinWidths ()
{
  if (isBinWidthNormalized)
    {
      cerr << "ERROR, histograms already normalized, not normalizing again" << endl ;
      return ;
    }
  for (unsigned int i = 0 ; i < collection.size () ; ++i)
    {
      normalizeToBinWidths (collection.at (i)) ;
    }
  isBinWidthNormalized = true ;  
  return ;
}
