int readSignalShapes () 
{
  RooRealVar m ("m", "higgs mass", 0., 1000.) ;
  RooPlot *mplot = m.frame () ;

  TFile* in = new TFile ("signalShapes.root") ;
  in->cd () ;

  // plot the distributions
  RooHistPdf * PDF_250 = (RooHistPdf *) in->Get ("PDF_250") ;
  RooHistPdf * PDF_300 = (RooHistPdf *) in->Get ("PDF_300") ;
  RooHistPdf * PDF_350 = (RooHistPdf *) in->Get ("PDF_350") ;
  RooHistPdf * PDF_400 = (RooHistPdf *) in->Get ("PDF_400") ;
  RooHistPdf * PDF_450 = (RooHistPdf *) in->Get ("PDF_450") ;
  RooHistPdf * PDF_500 = (RooHistPdf *) in->Get ("PDF_500") ;
  RooHistPdf * PDF_550 = (RooHistPdf *) in->Get ("PDF_550") ;
  RooHistPdf * PDF_600 = (RooHistPdf *) in->Get ("PDF_600") ;

  PDF_250->plotOn (mplot) ;
  PDF_300->plotOn (mplot) ;
  PDF_350->plotOn (mplot) ;
  PDF_400->plotOn (mplot) ;
  PDF_450->plotOn (mplot) ;
  PDF_500->plotOn (mplot) ;
  PDF_550->plotOn (mplot) ;
  PDF_600->plotOn (mplot) ;

  TFile* in2 = new TFile ("testRooFit_003.root") ;

  RooKeysPdf * KPDF_250 = (RooKeysPdf *) in2->Get ("KPDF_ggH250") ;
  RooKeysPdf * KPDF_300 = (RooKeysPdf *) in2->Get ("KPDF_ggH300") ;
  RooKeysPdf * KPDF_350 = (RooKeysPdf *) in2->Get ("KPDF_ggH350") ;
  RooKeysPdf * KPDF_400 = (RooKeysPdf *) in2->Get ("KPDF_ggH400") ;
  RooKeysPdf * KPDF_450 = (RooKeysPdf *) in2->Get ("KPDF_ggH450") ;
  RooKeysPdf * KPDF_500 = (RooKeysPdf *) in2->Get ("KPDF_ggH500") ;
  RooKeysPdf * KPDF_550 = (RooKeysPdf *) in2->Get ("KPDF_ggH550") ;
  RooKeysPdf * KPDF_600 = (RooKeysPdf *) in2->Get ("KPDF_ggH600") ;

  int i = 0 ;
//  KPDF_250->plotOn (mplot, LineColor (kRed)) ;
//  KPDF_300->plotOn (mplot, LineColor (kRed)) ;
//  KPDF_350->plotOn (mplot, LineColor (kRed)) ;
//  KPDF_400->plotOn (mplot, LineColor (kRed)) ;
//  KPDF_450->plotOn (mplot, LineColor (kRed)) ;
//  KPDF_500->plotOn (mplot, LineColor (kRed)) ;
//  KPDF_550->plotOn (mplot, LineColor (kRed)) ;
//  KPDF_600->plotOn (mplot, LineColor (kRed)) ;

  TCanvas * c1 = new TCanvas () ;
  mplot->Draw () ;
  c1->Print ("readSignalShapes.pdf","pdf") ;



}