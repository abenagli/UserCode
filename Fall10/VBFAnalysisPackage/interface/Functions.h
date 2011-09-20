/*** single crystal ball ***/
double crystalBallHigh (double* x, double* par)
{
 //[0] = N           normalization factor                                                                                                                                                             
 //[1] = mean        gaussian mean
 //[2] = sigma       gaussian sigma
 //[3] = alpha       junction point
 //[4] = n           power law

 double xx = x[0];
 double mean = par[1];
 double sigma = par[2];
 double alpha = par[3];
 double n = par[4];

 if( (xx-mean)/sigma > fabs(alpha) )
 {
   double A = pow(n/fabs(alpha), n) * exp(-0.5 * alpha*alpha);
   double B = n/fabs(alpha) - fabs(alpha);

   return par[0] * A * pow(B + (xx-mean)/sigma, -1.*n);
 }

 else
 {
   return par[0] * exp(-1. * (xx-mean)*(xx-mean) / (2*sigma*sigma) );
 }
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


double antiFermi (double * x, double * par)
{
// [0] fermi energy
// [1] temperature

 return 1. / (exp ((par[0] - x[0]) / par[1]) + 1) ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


double attenuatedCB (double * x, double * par)
{
  return crystalBallHigh (x, par) * antiFermi (x, &par[5]) ;
}

