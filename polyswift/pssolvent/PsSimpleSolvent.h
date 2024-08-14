/**
 *
 * @file    PsSimpleSolvent.h
 *
 * @brief   Class for simple solvents whose interactions depend
 *          only on monomer separation
 *
 * @version $Id: PsSimpleSolvent.h 6319 2006-11-14 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_SIMPLE_SOLVENT_H
#define PS_SIMPLE_SOLVENT_H

// std includes
#include <vector>
#include <map>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsSolvent.h>
#include <PsPolymer.h>
#include <PsPhysField.h>
#include <PsDecompBase.h>
#include <PsGridBase.h>
#include <PsCommBase.h>

/**
 * Solves for simple solvents
 *
 * @param FLOATTYPE the data type of simulation
 * @param NDIM the dimensionality of simulation
 */
template <class FLOATTYPE, size_t NDIM>
class PsSimpleSolvent : public PsSolvent<FLOATTYPE, NDIM> {

  public:

/**
 * constructor
 */
  PsSimpleSolvent();

/**
 * Destructor
 */
  virtual ~PsSimpleSolvent();

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
 * Initialize before update
 */
    virtual void initialize();

/**
 * Update solvent
 *
 * @param t simulation time
 */
    virtual void update(double t);

/**
 * Calculate solvent observables
 */
    virtual void setPhysFields();

  protected:

    /** Total change in updateField quantity (a placeholder) */
    PsFieldBase<FLOATTYPE>* dField;

    /** Scale length from polymer model */
    size_t scaleLength;

  private:

    /** Constructor private to prevent use */
    PsSimpleSolvent(const PsSimpleSolvent<FLOATTYPE, NDIM>& psbcp);

   /** Assignment private to prevent use */
    PsSimpleSolvent<FLOATTYPE, NDIM>& operator=(
      const PsSimpleSolvent<FLOATTYPE, NDIM>& psbcp);

};

#endif // PS_SIMPLE_SOLVENT_H
