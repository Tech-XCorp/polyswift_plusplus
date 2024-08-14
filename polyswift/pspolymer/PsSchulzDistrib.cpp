/**
 * @file    PsSchulzDistrib.cpp
 *
 * @brief   Calculate abscissas and weights for Gauss-Laguerre quadrature
 *
 * @version $Id: PsSchulzDistrib.cpp 8257 2007-09-12 22:05:20Z swsides $
 *
 * Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

# include <cstdlib>
# include <math.h> // gamma --> tgamma
# include <iostream>
# include <fstream>
# include <iomanip>
# include <cstring>

// pspolymer includes
#include <PsSchulzDistrib.h>

//
// Constructor
//
//
//           N                 exp(-N/nb)
//  p(N) = (---)^(alpha-1) -------------------
//           nb              nb gamma(alpha)
//
PsSchulzDistrib::PsSchulzDistrib(double m_alpha, double m_nb) {

  alpha = m_alpha;   // power factor exponent
  nb    = m_nb;      // characteristic length of distribution

  numAver    = alpha*nb;           // number average
  weightAver = (alpha+1.0)*nb;     // weight average
  ipdi       = (alpha+1.0)/alpha;  // polydispersity index
}

// Destructor
PsSchulzDistrib::~PsSchulzDistrib() { }

//
// distribution value p(n)
//
double PsSchulzDistrib::pDist(double n) {

  if (nb < 1) {
    std::cout << "PsSchulzDistrib::pDist nb < 1" << std::endl;
    // TxDebugExcept tde("PsSchulzDistrib::pDist nb < 1");
    // throw tde;
  }
  double tmp1 = n/nb;
  double tmp2 = pow(tmp1,alpha);
  double numer = exp(-1.0*tmp1);
  double denom = nb*tgamma(alpha);
  return tmp2*numer/denom;
}
