/**
 * @file    PsSchulzDistrib.h
 *
 * @brief   Functions associated w/ Schulz distribution
 *
 * @version $Id: PsSchulzDistrib.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_SCHULZ_DISTRIB_H
#define PS_SCHULZ_DISTRIB_H

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// txbase headers
#include <TxDebugExcept.h>

/**
 * A PsSchulzDistrib object for calculating the distribution
 * functions and associated quantities
 */
class PsSchulzDistrib {

  public:

/**
 * Constructor
 *
 * @param m_alpha exponent
 * @param m_nb    characteristic length
 */
  PsSchulzDistrib(double m_alpha, double m_nb);

/**
 * Destructor must destroy blocks
 */
  virtual ~PsSchulzDistrib();

/**
 * Calculate Schulz distribution
 *            N                  exp(-N/n_b)
 *  p(N) = (----)^(alpha-1) -------------------
 *           nb               nb gamma(alpha)
 *
 * @param n input value
 * @return p(n)
 */
  double pDist(double n);

/**
 * Calculate number average of Schulz distribution
 *
 * @return number average
 */
  double getNumAverage() {
    return numAver;
  }

/**
 * Calculate weight average of Schulz distribution
 *
 * @return weight average
 */
  double getWeightAverage() {
    return weightAver;
  }

  protected:

  private:

   /** exponent */
   double alpha;

   /** characteristic length */
   double nb;

   /** polydispersity index */
   double ipdi;

   /** number average */
   double numAver;

   /** number average */
   double weightAver;

   /** Make private to prevent use */
   PsSchulzDistrib(const PsSchulzDistrib& psfb);

   /** Make private to prevent use */
   PsSchulzDistrib& operator=(const PsSchulzDistrib& psfb);
};

#endif // PS_SCHULZ_DISTRIB_H
