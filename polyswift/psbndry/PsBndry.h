/**
 * @file    PsBndry.h
 *
 * @brief   Base interface for boundaries (walls)
 *
 * @version $Id: PsBndry.h 6319 2006-11-14 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_BNDRY_H
#define PS_BNDRY_H

// std includes
#include <vector>
#include <map>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// txbase includes
#include <TxNotImplExcept.h>

// psbase includes
#include <PsBndryBase.h>

/**
 * Base class for boundary
 *
 * @param FLOATTYPE the data type of simulation
 * @param NDIM the dimensionality of simulation
 */
template <class FLOATTYPE, size_t NDIM>
class PsBndry : public PsBndryBase<FLOATTYPE, NDIM> {

  public:

/**
 * constructor
 */
  PsBndry();

/**
 * Destructor
 */
  virtual ~PsBndry();

/**
 * Store the data needed to build this object.
 *
 * @param tas the attribute set containing the initial conditions
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Build the solvers for this object
 */
    virtual void buildSolvers();

/**
 * Build the data for this object
 */
    virtual void buildData();

/**
 * Initialize object before update steps
 */
    virtual void initialize();

/**
 * Update the object
 *
 * @param t update time
 */
    virtual void update(double t);

  protected:

  private:

    /** Constructor private to prevent use */
    PsBndry(const PsBndry<FLOATTYPE, NDIM>& psbcp);

   /** Assignment private to prevent use */
    PsBndry<FLOATTYPE, NDIM>& operator=(
      const PsBndry<FLOATTYPE, NDIM>& psbcp);

};

#endif // PS_BNDRY_H
