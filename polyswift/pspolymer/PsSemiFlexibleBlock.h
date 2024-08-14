/**
 * @file    PsSemiFlexibleBlock.h
 *
 * @brief   Class for solving propagators for semiflexible blocks.
 *
 * @version $Id: PsSemiFlexibleBlock.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_SEMI_FLEXIBLE_BLOCK_H
#define PS_SEMI_FLEXIBLE_BLOCK_H

// standard headers
#include <list>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
// #undef HAVE_CONFIG_H
#endif

// txbase headers
#include <TxDebugExcept.h>

// pspolymer includes
#include <PsBlock.h>

/**
 * A PsSemiFlexibleBlock object contains the data structures
 * and solvers for semi-flexible polymer model.
 *
 * This class and methods within know only about the
 * structure of the propagator for the flexible model ie
 * q(r,u,s+ds) = exp[ ds( nabla^2_u - nabla_r - w(r,u) ) ] q(r,u,s) (check)
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 * @param QTYPE PsField type for the q(X) part of q(X,s)
 */

template <class FLOATTYPE, size_t NDIM, class QTYPE>
class PsSemiFlexibleBlock : public virtual PsBlock<FLOATTYPE, NDIM, QTYPE> {

  public:

/**
 * Constructor
 *
 */
    PsSemiFlexibleBlock();

/**
 * Destructor must destroy blocks
 */
    virtual ~PsSemiFlexibleBlock();

//
// Accessors
//

/**
 * Set the parameters
 *
 * @param tas the parameters
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Build the solvers structures, fields etc.
 */
    virtual void buildSolvers();

/**
 * Build the data, such as updaters, initial condition setters, ...
 */
    virtual void buildData();

/**
 * Reset specific flexible block values before update calls
 */
    virtual void reset();

/**
 * Update forward/backward propagators
 *
 * @param solveFromEnd block end to start calculation from
 */
    virtual void solveQ(BlockEndType solveFromEnd);

  protected:

    /** Persistence length */
    FLOATTYPE persistLength;

    /** Fundamental propagator q(r,u) function */
    QTYPE qX;

  private:

    /** Make private to prevent use */
    PsSemiFlexibleBlock(const PsSemiFlexibleBlock<FLOATTYPE, NDIM, QTYPE>& pssfb);

    /** Make private to prevent use */
    PsSemiFlexibleBlock<FLOATTYPE, NDIM, QTYPE>& operator=(
        const PsSemiFlexibleBlock<FLOATTYPE, NDIM, QTYPE>& pssfb);
};

#endif  // PS_SEMI_FLEXIBLE_BLOCK_H
