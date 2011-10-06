#include "hColl.h"
#include "THStack.h"

#include <iostream>

using namespace std ;

hColl::hColl (string pN, int b, double m, double M) :
 plotName (pN),
 bins (b),
 min (m),
 max (M) 
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
  TH1F * dummy = new TH1F ((plotName + "_" + sampleName).c_str (), sampleName.c_str (), bins, min, max) ;
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


