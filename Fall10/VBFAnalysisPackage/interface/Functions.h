#ifndef Functions_h
#define Functions_h

#include <iostream>






// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// SIMPLE FUNCTIONS
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


/*** powerLaw ***/
double powerLaw(double* x, double* par)
{
  //[0] = N   normalization factor
  //[1] = n   slope
  
  // variable
  double xx = x[0];
  // parameters
  double N = par[0];
  double x0 = par[1];
  double lambda = par[2];
  double n = par[3];
  
  //std::cout << "N: " << N << "   x0: " << x0 << "   lambda: " << lambda << "   n: " << n << std::endl;
  return pow(N,n) * pow(fabs((xx+x0)/lambda),-1.*n);
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


/*** exponential ***/
double exponential(double* x, double* par)
{
  //[0] = N   normalization factor
  //[1] = lambda   slope
  
  double xx = x[0];
  double N = par[0];
  double lambda = par[1];
  
  //std::cout << "N: " << N << "   lambda: " << lambda << std::endl;
  return exp(N) * exp(-1.*lambda*xx);
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


/*** double exponential ***/
double doubleExponential(double* x, double* par)
{
  //[0] = N   normalization factor
  //[1] = lambda   slope
  double xx = x[0];
  double N1 = par[0];
  double lambda1 = par[1];
  double N2 = par[2];
  double lambda2 = par[3];

  //std::cout << "N: " << N << "   lambda: " << lambda << std::endl;
  return exp(N1) * exp(-1.*lambda1*xx) + exp(N2) * exp(-1.*lambda2*xx);
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


/*** turn-on fermi function ***/
double antiFermi(double* x, double* par)
{
  // [0] = mu   fermi energy
  // [1] = kT   temperature
  
  // variable
  double xx = x[0];
  // parameters
  double mu = par[0];
  double kT = par[1];
  //std::cout << "mu: " << mu << "   kT: " << kT << std::endl; 
  
  return 1. / ( exp( -1.*(xx-mu)/ kT ) + 1.);
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


/*** gaussian + exponential tail smoothly joined ***/
double superGausCumCauda(double* x, double* par)
{
  //[0] = N
  //[1] = mean
  //[2] = sigma
  //[3] = alpha
  
  // variable
  double xx = x[0];
  // parameters
  double mean = par[1];
  double sigma = par[2];
  double alpha = par[3];
  //std::cout << "mean: " << mean << "   sigma: " << sigma << "   alpha: " << alpha << std::endl;
  
  if( xx < (mean+alpha*sigma) )
    {
      return par[0] / sigma / sqrt(2.*3.14159) * exp(-1.*(xx-mean)*(xx-mean)/(2.*sigma*sigma));
    }

  else
    {
      double N = par[0] / sigma / sqrt(2.*3.14159) * exp( 0.5*alpha*alpha + alpha/sigma*mean );
      double K = alpha/sigma;

      return N * exp(-1.*K*xx);
    }
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


/*** single crystal ball with high tail ***/
double crystalBallHigh(double* x, double* par)
{
  //[0] = N           normalization factor
  //[1] = mean        gaussian mean
  //[2] = sigma       gaussian sigma
  //[3] = alpha       junction point
  //[4] = n           power law
  
  // variable
  double xx = x[0];
  // parameters
  double mean = par[1];
  double sigma = par[2];
  double alpha = par[3];
  double n = par[4];
  // std::cout << "mean: " << mean << "   sigma: " << sigma << "   alpha: " << alpha << "   n: " << n << std::endl;
  
  if( (xx-mean)/sigma > fabs(alpha) )
  {
    double A = pow(n/fabs(alpha), n) * exp(-0.5 * alpha*alpha);
    double B = n/fabs(alpha) - fabs(alpha);
    
    std::cout << "N = " << par[0] * A << "   B: " << B << "   par: " << B + (xx-mean)/sigma << "   n: " << n << "   pow: " << pow(B + (xx-mean)/sigma, -1.*n) <<std::endl;
    return par[0] * A * pow(B + (xx-mean)/sigma, -1.*n);
  }
  
  else
  {
    return par[0] * exp(-1. * (xx-mean)*(xx-mean) / (2*sigma*sigma) );
  }
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----






// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// COMPOSITE FUNCTIONS
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


double attenuatedPowerLaw(double* x, double* par)
{
  return antiFermi(x,par) * powerLaw(x,&par[2]);
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


double attenuatedExponential(double* x, double* par)
{
  return antiFermi(x,par) * exponential(x,&par[2]);
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


double attenuatedDoubleExponential(double* x, double* par)
{
  return antiFermi(x,par) * doubleExponential(x,&par[2]);
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


double attenuatedCB(double* x, double* par)
{
  return crystalBallHigh (x, par) * antiFermi (x, &par[5]) ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


#endif
