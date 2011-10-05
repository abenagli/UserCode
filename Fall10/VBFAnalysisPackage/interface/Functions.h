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


/*** parabola + double exponential tail smoothly joined ***/
double superParabolaCumDoubleCauda(double* x, double* par)
{
  // variable
   double xx = x[0];
  // parameters
  double a = par[0];
  double x0 = par[1];
  double N1 = par[2];
  double lambda1 = par[3];
  double N2 = par[4];
  double lambda2 = par[5];
  //std::cout << "a: " << a "   x0: " << x0 << "   N1: " << N1 << "   lambda1: " << lambda1 << "   N2: " << N2 << "   lambda2: " << lambda2 << std::endl;
  
  if( xx < x0 )
  {
    float b = x0 + lambda1/(2.*a)*exp(N1-lambda1*x0) + lambda2/(2.*a)*exp(N2-lambda2*x0);
    float sum = lambda1 * exp(N1-lambda1*x0) + lambda2 * exp(N2-lambda2*x0);
    float c = exp(N1-lambda1*x0) + exp(N2-lambda2*x0) - 1./(4.*a)*sum*sum;
    float val = a * (xx-b)*(xx-b) + c;
    if( val <= 0. ) return 1e-10;
    else return val;
  }
  
  else
  {
    return exp(N1) * exp(-1.*lambda1*xx) + exp(N2) * exp(-1.*lambda2*xx);
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


double attenuatedDoubleExponentialCumLeadingEdge(double* x, double* par)
{
  //variable
  double xx = x[0];
  // parameters
  double mu = par[0];
  double kT = par[1];
  double N1 = par[2];
  double lambda1 = par[3];
  double N2 = par[4];
  double lambda2 = par[5];
  double x0 = par[6];
  double lambda = par[7];
  
  double alpha = 1/(x0-lambda) * log( (exp(N1-lambda1*x0)+exp(N2-lambda2*x0)) / (exp(-1.*(x0-mu)/kT)+1) );  
  
  if( xx < x0 ) return exp(alpha*(xx-lambda));
  else return antiFermi(x,par) * doubleExponential(x,&par[2]);
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


double attenuatedCB(double* x, double* par)
{
  return crystalBallHigh (x, par) * antiFermi (x, &par[5]) ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


#endif
