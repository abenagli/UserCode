ggH500_0->SetLineColor (kBlack)
ggH500_PDFUp->SetLineColor (kRed)
ggH500_PDFDown->SetLineColor (kBlue)

TCanvas c1 ;

ggH500_PDFUp->Draw ()
ggH500_0->Draw ("histsame")
ggH500_PDFDown->Draw ("histsame")

TH1F * ratio_ggH500_PDFUp = ggH500_PDFUp->Clone ("ratio_ggH500_PDFUp")
ratio_ggH500_PDFUp->Divide (ggH500_0) ;
TH1F * ratio_ggH500_PDFDown = ggH500_PDFDown->Clone ("ratio_ggH500_PDFDown")
ratio_ggH500_PDFDown->Divide (ggH500_0) ;
TH1F * ratio_ggH500_0 = ggH500_0->Clone ("ratio_ggH500_0")
ratio_ggH500_0->Divide (ggH500_0) ;

TCanvas c2 ;

ratio_ggH500_PDFUp->Draw ("hist")
ratio_ggH500_PDFDown->Draw ("histsame")
ratio_ggH500_0->Draw ("histsame")

TH1F * scale_ggH500_PDFUp = ggH500_PDFUp->Clone ("scale_ggH500_PDFUp")
scale_ggH500_PDFUp->Scale (1./scale_ggH500_PDFUp->Integral ()) ;

TH1F * scale_ggH500_PDFDown = ggH500_PDFDown->Clone ("scale_ggH500_PDFDown")
scale_ggH500_PDFDown->Scale (1./scale_ggH500_PDFDown->Integral ()) ;

TH1F * scale_ggH500_0 = ggH500_0->Clone ("scale_ggH500_0")
scale_ggH500_0->Scale (1./scale_ggH500_0->Integral ()) ;

//scale_ggH500_PDFUp->Divide (scale_ggH500_0) ;
//scale_ggH500_PDFDown->Divide (scale_ggH500_0) ;
//scale_ggH500_0->Divide (scale_ggH500_0) ;

TCanvas c3 ;

scale_ggH500_PDFUp->Draw ("hist")
scale_ggH500_PDFDown->Draw ("histsame")
scale_ggH500_0->Draw ("histsame")

