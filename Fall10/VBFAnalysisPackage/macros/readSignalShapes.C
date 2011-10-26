int readSignalShapes () 
{
  TFile* in = new TFile ("signalShapes.root") ;
  in->cd () ;

  // plot the distributions
  RooRealVar m ("m", "higgs mass", 0., 1000.) ;
  RooPlot *mplot = m.frame () ;
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

  TCanvas * c1 = new TCanvas () ;
  mplot->Draw () ;
  c1->Print ("readSignalShapes.pdf","pdf") ;



}