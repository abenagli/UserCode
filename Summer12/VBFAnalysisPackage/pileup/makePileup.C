#include <iomanip>



void makePileup(const std::string& fileName)
{
  TFile* f = TFile::Open((fileName).c_str(),"RECREATE");
  TH1F* h = new TH1F("pileup","",35,-0.5,34.5);
  
  double* distrPU_DATA = new double[35];
  
  
  
  /*
  // matt - Run2011A
  distrPU_DATA[0]  = 0.00548268488655;
  distrPU_DATA[1]  = 0.0221623012347;
  distrPU_DATA[2]  = 0.0496891094541;
  distrPU_DATA[3]  = 0.0812509216863;
  distrPU_DATA[4]  = 0.108077184946;
  distrPU_DATA[5]  = 0.123760426259;
  distrPU_DATA[6]  = 0.126206245433;
  distrPU_DATA[7]  = 0.117184065729;
  distrPU_DATA[8]  = 0.100600998899;
  distrPU_DATA[9]  = 0.0807709135897;
  distrPU_DATA[10] = 0.0611758616346;
  distrPU_DATA[11] = 0.0439921259819;
  distrPU_DATA[12] = 0.0301786759644;
  distrPU_DATA[13] = 0.0194251837551;
  distrPU_DATA[14] = 0.0122283775737;
  distrPU_DATA[15] = 0.00756679368734;
  distrPU_DATA[16] = 0.00453053906389;
  distrPU_DATA[17] = 0.0026254307415;
  distrPU_DATA[18] = 0.00147069224801;
  distrPU_DATA[19] = 0.00079626771087;
  distrPU_DATA[20] = 0.000416930356137;
  distrPU_DATA[21] = 0.000212005583528;
  distrPU_DATA[22] = 0.000104839263429;
  distrPU_DATA[23] = 5.00184750011e-05;
  distrPU_DATA[24] = 2.32068721987e-05;
  distrPU_DATA[25] = 1.03418434519e-05;
  distrPU_DATA[26] = 4.60177033163e-06;
  distrPU_DATA[27] = 1.9020062108e-06;
  distrPU_DATA[28] = 7.77577472784e-07;
  distrPU_DATA[29] = 3.37391685271e-07;
  distrPU_DATA[30] = 1.34956674108e-07;
  distrPU_DATA[31] = 6.74783370542e-08;
  distrPU_DATA[32] = 2.77481012186e-08;
  distrPU_DATA[33] = 5.67574797652e-09;
  distrPU_DATA[34] = 2.52255465623e-09;
  */
  
  /*
  // matt - Run2011B
  distrPU_DATA[0]  = 0.000112737003406;
  distrPU_DATA[1]  = 0.000777646125266;
  distrPU_DATA[2]  = 0.00293953727088;
  distrPU_DATA[3]  = 0.00779607746446;
  distrPU_DATA[4]  = 0.0161886140039;
  distrPU_DATA[5]  = 0.0280489939304;
  distrPU_DATA[6]  = 0.0422732741973;
  distrPU_DATA[7]  = 0.0570578977823;
  distrPU_DATA[8]  = 0.0704498952494;
  distrPU_DATA[9]  = 0.0808285880957;
  distrPU_DATA[10] = 0.087175540055;
  distrPU_DATA[11] = 0.0891353619476;
  distrPU_DATA[12] = 0.0869369566986;
  distrPU_DATA[13] = 0.0812446797496;
  distrPU_DATA[14] = 0.07298581133;
  distrPU_DATA[15] = 0.0631825589687;
  distrPU_DATA[16] = 0.0528079996742;
  distrPU_DATA[17] = 0.0426800503642;
  distrPU_DATA[18] = 0.0334009356879;
  distrPU_DATA[19] = 0.0253413520202;
  distrPU_DATA[20] = 0.0186609176278;
  distrPU_DATA[21] = 0.0133518691661;
  distrPU_DATA[22] = 0.00929217896527;
  distrPU_DATA[23] = 0.00629666691546;
  distrPU_DATA[24] = 0.00415877084982;
  distrPU_DATA[25] = 0.00267988512703;
  distrPU_DATA[26] = 0.00168652054605;
  distrPU_DATA[27] = 0.00103754971688;
  distrPU_DATA[28] = 0.000624559819431;
  distrPU_DATA[29] = 0.000368196397673;
  distrPU_DATA[30] = 0.000212766035973;
  distrPU_DATA[31] = 0.000120615027302;
  distrPU_DATA[32] = 6.71298899774e-05;
  distrPU_DATA[33] = 3.67081722824e-05;
  distrPU_DATA[34] = 1.97348922108e-05;
  */
  
  /*
  // matt - Run2011AB
  distrPU_DATA[0]  = 0.00203414674627;
  distrPU_DATA[1]  = 0.0093989457795;
  distrPU_DATA[2]  = 0.0234955075915;
  distrPU_DATA[3]  = 0.0420865073886;
  distrPU_DATA[4]  = 0.0606952653102;
  distrPU_DATA[5]  = 0.0752888090933;
  distrPU_DATA[6]  = 0.0839215023063;
  distrPU_DATA[7]  = 0.0867348804906;
  distrPU_DATA[8]  = 0.085047816952;
  distrPU_DATA[9]  = 0.0804197580772;
  distrPU_DATA[10] = 0.0741109810939;
  distrPU_DATA[11] = 0.0669425322596;
  distrPU_DATA[12] = 0.0593852931679;
  distrPU_DATA[13] = 0.0517138171608;
  distrPU_DATA[14] = 0.0441299860278;
  distrPU_DATA[15] = 0.0368259963707;
  distrPU_DATA[16] = 0.0299948235968;
  distrPU_DATA[17] = 0.0238112095942;
  distrPU_DATA[18] = 0.0184057946082;
  distrPU_DATA[19] = 0.0138473260247;
  distrPU_DATA[20] = 0.0101388028497;
  distrPU_DATA[21] = 0.00722638449542;
  distrPU_DATA[22] = 0.00501616832518;
  distrPU_DATA[23] = 0.00339323049781;
  distrPU_DATA[24] = 0.00223854969582;
  distrPU_DATA[25] = 0.00144140292082;
  distrPU_DATA[26] = 0.000906652403698;
  distrPU_DATA[27] = 0.000557587401343;
  distrPU_DATA[28] = 0.000335569614276;
  distrPU_DATA[29] = 0.000197799663015;
  distrPU_DATA[30] = 0.000114289785038;
  distrPU_DATA[31] = 6.47858088544e-05;
  distrPU_DATA[32] = 3.60559625478e-05;
  distrPU_DATA[33] = 1.97157233488e-05;
  distrPU_DATA[34] = 1.05993072984e-05;
  */
  
  
  
  for(int bin = 1; bin <= 35; ++bin)
  {
    h -> SetBinContent(bin,distrPU_DATA[bin-1]);
    
    if( int(h->GetBinCenter(bin)) < 10 )
      std::cout << "distrPU_DATA[" << int(h->GetBinCenter(bin)) <<"]  = ";
    else
      std::cout << "distrPU_DATA[" << int(h->GetBinCenter(bin)) <<"] = ";
    
    std::cout << distrPU_DATA[bin-1] << ";" << std::endl;
  }  
  
  
  TFile* o = new TFile((fileName).c_str(),"RECREATE");
  o -> cd();
  
  h -> Write();
  
  o -> Close();
}
