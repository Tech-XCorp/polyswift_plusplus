/**
 *
 * @file    PsMultiSpecFilter.h
 *
 * @brief   Class for updating fields with spectral filter algorithm
 *          w/multiple cells for calculating cutoff
 *
 * @version $Id: PsMultiSpecFilter.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2010-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_MULTI_SPEC_FILTER_H
#define PS_MULTI_SPEC_FILTER_H

// standard headers
#include <list>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#undef HAVE_CONFIG_H
#endif

// txbase headers
#include <TxDebugExcept.h>

// pseffhamil includes
#include <PsSpecFilterUpdater.h>

/**
 * A PsMultiSpecFilter object updates fields using spectral filter algorithm
 * with multiple cutoff values
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 */

template <class FLOATTYPE, size_t NDIM>
class PsMultiSpecFilter : public virtual PsSpecFilterUpdater<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 *
 */
    PsMultiSpecFilter();

/**
 * Destructor must destroy blocks
 */
    virtual ~PsMultiSpecFilter();

/**
 * Set the parameters
 *
 * @param tas the parameters
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Build the data
 */
    virtual void buildData();

/**
 * Build the solvers structures
 */
    virtual void buildSolvers();

/**
 * Initialize object
 */
    virtual void initialize();

/**
 * Update the updater
 *
 * @param t the time
 */
    virtual void update(double t);

  protected:

  private:

    /** Pointer to FFT object built w/transpose grid */
    PsFFTBase<FLOATTYPE, NDIM>* fftTransObjPtr;

    /** Sets up kcellMap */
    void build_specCells_transpose();

    /** Helper for update to filter */
    void applyFilter(PsFieldBase<FLOATTYPE>& wField);

    // SWS: NDIM length?
    /** Number of cells that sub-divide k-space */
    std::vector<size_t> numSpecCells;

    // SWS: NDIM length?
    /** Number of grid cells/per spec cells */
    std::vector<size_t> specCellSizes;

    /** Total number of spec cells */
    size_t totNumCells;

    /** List of k space locations mapped to a spectral cell index */
    size_t* kcellMap;

    /** fft Size for multi */
    size_t fftSizeMulti;

    /**
     * Value of cutoff in filter:
     * algorithm finds maximum in spectrum and filters all
     * components < (cutoffFactor*maximum)
     */
    std::vector<FLOATTYPE> cutoffFactors;

    /**
     * Temporary cutoffs for parallel calc
     */
    std::vector<FLOATTYPE> cutoffFactorsTmp;

    /** Constructor private to prevent use */
    PsMultiSpecFilter(const PsMultiSpecFilter<FLOATTYPE, NDIM>& psb);

   /** Assignment private to prevent use */
    PsMultiSpecFilter<FLOATTYPE, NDIM>& operator=(
       const PsMultiSpecFilter<FLOATTYPE, NDIM>& psb);
};

#endif  // PS_MULTI_SPEC_FILTER_H
