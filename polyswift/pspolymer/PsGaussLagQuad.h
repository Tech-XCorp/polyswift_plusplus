/**
 * @file    PsGaussLagQuad.h
 *
 * @brief   Calculate abscissas and weights for Gauss-Laguerre quadrature
 *
 * @version $Id: PsGaussLagQuad.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_GAUSS_LAG_QUAD_H
#define PS_GAUSS_LAG_QUAD_H

// standard headers
#include <list>
#include <string>
#include <vector>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// txbase headers
#include <TxDebugExcept.h>

#include <math.h> // gamma --> tgamma

/**
 * A PsGaussLagQuad object
 * calculate abscissas and weights for Gauss-Laguerre quadrature
 *
 * Modified: gamma() --> tgamma() in include <math.h>
 *
 * From original code (modified sligthly for specific case):
 *
 *    This program computes a standard or exponentially weighted
 *    Gauss-Laguerre quadrature rule and writes it to a file.
 *
 *    The caller specifies:
 *    * the ORDER (number of points) in the rule;
 *    * ALPHA, the exponent of |X|;
 *    * A, the left endpoint;
 *    * B, the scale factor in the exponential;
 *
 *  Licensing:
 *
 *    This code is distributed under the GNU LGPL license.
 *
 *  Modified:
 *
 *    23 February 2010
 *
 *  Author:
 *
 *    John Burkardt
 *
 *
 * code()
  //
  // (number of points, alpha)
  //
  PsGaussLagQuad* objGL = new PsGaussLagQuad(4, 0.0);
  std::vector<double> w_vec = objGL->getWeights();
  std::vector<double> x_vec = objGL->getAbscissas();

  for (size_t n=0; n<w_vec.size(); n++) {
    std::cout << "x[" << n << "] = " << x_vec[n]         << std::endl;
    std::cout << "w[" << n << "] = " << w_vec[n] << "\n" << std::endl;
  }
  *
  * e.g. output:
  *
  * x[0] = 0.322548
  * w[0] = 0.603154
  *
  * x[1] = 1.74576
  * w[1] = 0.357419
  *
  * x[2] = 4.53662
  * w[2] = 0.0388879
  *
  * x[3] = 9.39507
  * w[3] = 0.000539295
  *
 */
class PsGaussLagQuad {

  public:

/**
 * Constructor
 *
 * @param m_order number of quadrature points
 * @param m_alpha exponent
 */
  PsGaussLagQuad(int m_order, double m_alpha);

/**
 * Destructor must destroy blocks
 */
  virtual ~PsGaussLagQuad();

/**
 * Return n weights
 * @return vector<double> of (order) weights
 */
  std::vector<double> getWeights() {
    return wVec;
  }

/**
 * Return n abscissas
 * @return vector<double> of (order) abscissas
 */
  std::vector<double> getAbscissas() {
    return xVec;
  }

/**
 * Return nth weight
 *
 * @param ni index
 * @return (double) weight
 */
  double getWeight(size_t ni) {
    return wVec.at(ni);
  }

/**
 * Return nth abscissa
 *
 * @param ni index
 * @return (double) abscissa
 */
  double getAbscissa(size_t ni) {
    return xVec.at(ni);
  }

/**
 * Gamma function (not working for windows)
 *
 * @param x arg
 * @return gamma(x)
 */
  double psGamma(double x) {
    return tgamma(x);

    /*
#ifdef _WIN32
    //return gamma(x);
    std::cout << "polydisperse will not work for WIN32" << std::endl;
    return (double)0.0;
#else
    return tgamma(x);
#endif
    */
  }

  protected:

  private:

   // Output

   /** vector of weights */
   std::vector<double> wVec;

   /** vector of abscissas */
   std::vector<double> xVec;

   // Input

   /** Number of quadrature points */
   int order;

   /** exponent */
   double alpha;

   // **********************************

   /** Pointer to weights */
   double *w;

   /** Pointer to abscissas */
   double *x;

   /** left endpoint (integral limit) */
   double a;

   /** dummy value (needed for other quadrature settings */
   double beta;

   /** scale factor in exponential */
   double b;

   //
   // Auxillary calls from gen_laguerre
   //
   void cdgqf (int nt, int kind, double alpha, double beta, double t[], double wts[] );
   void cgqf (int nt, int kind, double alpha, double beta, double a,
              double b, double t[], double wts[] );
   double class_matrix (int kind, int m, double alpha, double beta, double aj[],
                        double bj[] );
   void imtqlx ( int n, double d[], double e[], double z[] );
   void parchk ( int kind, int m, double alpha, double beta );
   double r8_abs ( double x );
   double r8_epsilon( );
   double r8_huge ();
   double r8_sign ( double x );
   void scqf ( int nt, double t[], int mlt[], double wts[], int nwts, int ndx[],
               double swts[], double st[], int kind, double alpha, double beta, double a,
               double b );
   void sgqf ( int nt, double aj[], double bj[], double zemu, double t[], double wts[] );

   /** Make private to prevent use */
   PsGaussLagQuad(const PsGaussLagQuad& psfb);

   /** Make private to prevent use */
   PsGaussLagQuad& operator=(const PsGaussLagQuad& psfb);
};

#endif // PS_GAUSS_LAG_QUAD_H
