/**
 *
 * @file    PsEffHamilBase.h
 *
 * @brief   Base/interface for specifying energetics of field-theoretic model
 *
 * @version $Id: PsEffHamilBase.h 6319 2006-11-14 22:39:46Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_EFF_HAMIL_BASE_H
#define PS_EFF_HAMIL_BASE_H

// std includes
#include <vector>
#include <map>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// txbase includes
#include <TxTensor.h>

// psbase includes
#include <PsDynObj.h>

/**
 * Specifies energetics of field-theoretic model
 *
 * @param FLOATTYPE the data type of simulation
 * @param NDIM the dimensionality of simulation
 */
template <class FLOATTYPE, size_t NDIM>
class PsEffHamilBase : public PsDynObj<FLOATTYPE, NDIM> {

  public:

/**
 * constructor
 */
  PsEffHamilBase();

/**
 * Destructor
 */
  virtual ~PsEffHamilBase();

/**
 * Store the data needed to build this object.
 *
 * @param tas the attribute set containing the initial conditions
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Build the data for this object
 */
    virtual void buildSolvers();

/**
 * Build the data for this object
 */
    virtual void buildData();

/**
 * Update the Hamiltonian model
 *
 * @param t simulation time
 */
    virtual void update(double t) = 0;

/**
 * Dump the object
 */
    virtual void dump() = 0;

  protected:

  private:

    /** Constructor private to prevent use */
    PsEffHamilBase(const PsEffHamilBase<FLOATTYPE, NDIM>& psbcp);

   /** Assignment private to prevent use */
    PsEffHamilBase<FLOATTYPE, NDIM>& operator=(
      const PsEffHamilBase<FLOATTYPE, NDIM>& psbcp);
};

#endif // PS_EFF_HAMIL_BASE_H
