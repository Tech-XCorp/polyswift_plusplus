/**
 *
 * @file    PsSimpleIons.h
 *
 * @brief   Class for simple solvents whose interactions depend
 *          only on electrostatics
 *
 * @version $Id: PsSimpleIons.h 6319 2006-11-14 22:39:46Z swsides $
 *
 * Copyright &copy; 2010-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_SIMPLE_IONS_H
#define PS_SIMPLE_IONS_H

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
 * Solves for simple molecular ions, that do
 * not interact through excluded volume... only charge.
 *
 * @param FLOATTYPE the data type of simulation
 * @param NDIM the dimensionality of simulation
 */
template <class FLOATTYPE, size_t NDIM>
class PsSimpleIons : public PsSolvent<FLOATTYPE, NDIM> {

  public:

/**
 * constructor
 */
  PsSimpleIons();

/**
 * Destructor
 */
  virtual ~PsSimpleIons();

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

    /** Charge strength per counter-ion */
    FLOATTYPE zc;

    /** Charge neutral factor (for now needs info from other blocks) */
    FLOATTYPE neutralFactor;

    /** Constructor private to prevent use */
    PsSimpleIons(const PsSimpleIons<FLOATTYPE, NDIM>& psbcp);

   /** Assignment private to prevent use */
    PsSimpleIons<FLOATTYPE, NDIM>& operator=(
      const PsSimpleIons<FLOATTYPE, NDIM>& psbcp);

};

#endif // PS_SIMPLE_IONS_H
