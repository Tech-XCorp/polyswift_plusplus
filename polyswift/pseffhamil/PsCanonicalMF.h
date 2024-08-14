/**
 *
 * @file    PsCanonicalMF.h
 *
 * @brief   Specifics for canonical mean-field model
 *
 * @version $Id: PsCanonicalMF.h 6319 2006-11-14 22:39:46Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_CANONICAL_MF_H
#define PS_CANONICAL_MF_H

// std includes
#include <vector>
#include <map>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// txbase includes
#include <TxTensor.h>

// pseffhamil includes
#include <PsEffHamil.h>

/**
 * Specific quantities for canonical mean-field model
 *
 * @param FLOATTYPE the data type of simulation
 * @param NDIM the dimensionality of simulation
 */
template <class FLOATTYPE, size_t NDIM>
class PsCanonicalMF : public PsEffHamil<FLOATTYPE, NDIM> {

  public:

/**
 * constructor
 */
  PsCanonicalMF();

/**
 * Destructor
 */
  virtual ~PsCanonicalMF();

/**
 * Store the data needed to build this object.
 *
 * @param tas the attribute set containing the initial conditions
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Build the data for this object
 */
    virtual void buildData();

/**
 * Build the solvers for this object
 */
    virtual void buildSolvers();

/**
 * Update specific quantities for canonical mean-field model
 *
 * @param t simulation time
 */
    virtual void update(double t);

  protected:

  private:

    /** Constructor private to prevent use */
    PsCanonicalMF(const PsCanonicalMF<FLOATTYPE, NDIM>& psbcp);

   /** Assignment private to prevent use */
    PsCanonicalMF<FLOATTYPE, NDIM>& operator=(
      const PsCanonicalMF<FLOATTYPE, NDIM>& psbcp);

};

#endif // PS_CANONICAL_MF_H
