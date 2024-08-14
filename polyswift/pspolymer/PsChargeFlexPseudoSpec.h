/**
 * @file    PsChargeFlexPseudoSpec.h
 *
 * @brief   Class for solving propagators for flexible Gaussian blocks
 *          with charges using with pseudo-spectral algorithm
 *
 * @version $Id: PsFlexChargePseudoSpec.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2010-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_CHARGE_FLEX_PSEUDO_SPEC_H
#define PS_CHARGE_FLEX_PSEUDO_SPEC_H

// standard headers
#include <list>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#undef HAVE_CONFIG_H
#endif

// pspolymer includes
#include <PsFlexPseudoSpec.h>

/**
 * A PsFlexChargePseudoSpec object contains the data structures
 * and solvers for flexible Gaussian polymer model with charges
 * This implements a specific model from
 * "Microphase separation in polyelectrolytic diblock copolymer melt: Weak segregation limit",
 * R Kumar, M Muthukumar, Journal of Chemical Physics, 2007 vol. 126 (21) pp. 214902-21
 *
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 * @param QTYPE PsField type for the q(X) part of q(X,s)
 */

template <class FLOATTYPE, size_t NDIM, class QTYPE>
class PsChargeFlexPseudoSpec : public virtual PsFlexPseudoSpec<FLOATTYPE, NDIM, QTYPE> {

  public:

/**
 * Constructor
 *
 */
    PsChargeFlexPseudoSpec();

/**
 * Destructor must destroy blocks
 */
    virtual ~PsChargeFlexPseudoSpec();

/**
 * Set the parameters
 *
 * @param tas the parameters
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Build the data, such as updaters, initial condition setters, ...
 */
    virtual void buildData();

/**
 * Build the solvers etc.
 */
    virtual void buildSolvers();

/**
 * Reset specific flexible block values before update calls
 *  - initial q(x,s) values at block free ends
 *  - calculate new "solver" values for w-fields including the
 *    contribution from the charge density
 */
    virtual void reset();

/**
 * Uses result from monomer density (in PsBlock) to set charge contribution
 * from this charged block
 */
    virtual void setPhysFields();

  protected:

  private:

    /** Charge strength per segment on block */
    FLOATTYPE za;

    /** Charge distribution along block */
    FLOATTYPE alpha;

    /** Make private to prevent use */
    PsChargeFlexPseudoSpec(const PsChargeFlexPseudoSpec<FLOATTYPE, NDIM, QTYPE>& psfb);

    /** Make private to prevent use */
    PsChargeFlexPseudoSpec<FLOATTYPE, NDIM, QTYPE>& operator=(
        const PsChargeFlexPseudoSpec<FLOATTYPE, NDIM, QTYPE>& psfb);
};

#endif  // PS_CHARGE_FLEX_PSEUDO_SPEC_H
