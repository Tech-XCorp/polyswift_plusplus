/**
 * @file    PsFlexPseudoSpec.h
 *
 * @brief   Class for solving propagators for flexible Gaussian blocks
 *          with pseudo-spectral algorithm
 *
 * @version $Id: PsFlexPseudoSpec.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_FLEX_PSEUDO_SPEC_H
#define PS_FLEX_PSEUDO_SPEC_H

// standard headers
#include <list>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
// #undef HAVE_CONFIG_H
#endif

// txbase headers
#include <TxDebugExcept.h>

// psbase includes
#include <PsFFTBase.h>

// pspolymer includes
#include <PsBlock.h>

// psstd includes
#include <PsRandom.h>

/**
 * A PsFlexPseudoSpec object contains the data structures
 * and solvers for flexible Gaussian polymer model.
 *
 * This class and methods within know only about the
 * structure of the propagator for the flexible model ie
 * q(r,s+ds) = exp[ ds( nabla^2 - w(r) ) ] q(r,s)
 *
 * For now, the solve methods are hard-wired for the
 * pseudo-spectral method... therefore the decomp is
 * that set by the FFT class.
 *
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 * @param QTYPE PsField type for the q(X) part of q(X,s)
 */

template <class FLOATTYPE, size_t NDIM, class QTYPE>
class PsFlexPseudoSpec : public virtual PsBlock<FLOATTYPE, NDIM, QTYPE> {

  public:

/**
 * Constructor
 *
 */
    PsFlexPseudoSpec();

/**
 * Destructor must destroy blocks
 */
    virtual ~PsFlexPseudoSpec();

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
 *  - calculate new "solver" values for w-fields
 */
    virtual void reset();

/**
 * Update forward/backward propagators
 *
 * @param solveFromEnd block end to start calculation from
 */
    virtual void solveQ(BlockEndType solveFromEnd);

  protected:

    //
    // General data
    //

    /** Fundamental propagator q(r) function */
    QTYPE qX;

    /** Size of the k2, wfac lists to be FFT'd */
    size_t fftSize;

    /** The field factor (exponentiated) */
    FLOATTYPE* wfac;

    /** FFT grid object pointer */
    PsGridBase<FLOATTYPE, NDIM>* fftGridPtr;

  private:

    /** Ratio of statistical segment length reference b0 */
    FLOATTYPE bSegRatio;

    /** Global simulation size, scale factor for transform */
    FLOATTYPE scaleFFT;

    /** FFT object name */
    std::string fftKind;

    /** Pointer to FFT interface object */
    PsFFTBase<FLOATTYPE, NDIM>* fftObjPtr;

    /** The Laplacian factor */
    FLOATTYPE*  k2;

    /** Initialize the "Laplacian" list */
    void build_k2();

    /** Initialize the "Laplacian" list */
    void build_k2_transpose();

    /** The product list for q(r,s)*w(r) */
    FLOATTYPE* qw;

    /** A result holder for safety */
    FLOATTYPE* resPtr;

    /** Make private to prevent use */
    PsFlexPseudoSpec(const PsFlexPseudoSpec<FLOATTYPE, NDIM, QTYPE>& psfb);

    /** Make private to prevent use */
    PsFlexPseudoSpec<FLOATTYPE, NDIM, QTYPE>& operator=(
        const PsFlexPseudoSpec<FLOATTYPE, NDIM, QTYPE>& psfb);
};

#endif  // PS_FLEX_PSEUDO_SPEC_H
