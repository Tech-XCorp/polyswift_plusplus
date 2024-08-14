/**
 *
 * @file    PsSpecFilterUpdater.h
 *
 * @brief   Class for updating fields with spectral filter algorithm
 *
 * @version $Id: PsSpecFilterUpdater.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2009-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_SPEC_FILTER_UPDATER_H
#define PS_SPEC_FILTER_UPDATER_H

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#undef HAVE_CONFIG_H
#endif

// psbase includes
#include <PsFFTBase.h>

// pseffhamil includes
#include <PsConstraintUpdater.h>

/**
 * A PsSpecFilterUpdater object updates fields using spectral filter algorithms
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 */

template <class FLOATTYPE, size_t NDIM>
class PsSpecFilterUpdater : public virtual PsConstraintUpdater<FLOATTYPE, NDIM> {

  public:

/** Constructor */
    PsSpecFilterUpdater();

/** Destructor */
    virtual ~PsSpecFilterUpdater();

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

    // SWS: doesnt mean anything here... should refactor
    // SWS: for history access
/**
 * Get free-energy value
 *
 * @return the free-energy
 */
    virtual FLOATTYPE getFreeE(bool calcDisorder=true) {
      TxDebugExcept tde("PsSpecFilterUpdater::getFreeE not yet implemented");
      throw tde;
      //      return 0.0;
    }

  protected:

    /** Input hierarchy set for STFunc */
    TxHierAttribSetIntDbl funcAttrib;

    /** FFTW decomp info SWS: replace with other decomp methods? */
    size_t ny_trans;

    /** FFTW decomp info SWS: replace with other decomp methods? */
    size_t y_start_trans;

    /** Names of STFuncs */
    std::vector<std::string> funcNames;

    /** Function for updating cutoff parameter */
    PsSTFunc<FLOATTYPE, NDIM>* cutSTFunc;

    /** Flag for cutSTFunc */
    bool hasCutSTFunc;

    /**
     * Value of cutoff in filter:
     * algorithm finds maximum in spectrum and filters all
     * components < (cutoffFactor*maximum)
     */
    FLOATTYPE cutoffFactor;

    /**
     * Strength of filter used:
     * for those components (S_i) that are filtered,
     * afterwards they are left as (filterStrength*S_i)
     */
    FLOATTYPE filterStrength;

    //
    // Spectral filter members
    //

    /** Pointer to FFT interface object */
    PsFFTBase<FLOATTYPE, NDIM>* fftObjPtr;

    /** Size of the k2, wfac lists to be FFT'd */
    size_t fftSize;

    /** Global simulation size, scale factor for transform */
    FLOATTYPE scaleFFT;

    /** Local data for results */
    FLOATTYPE* resPtr;

    /** Local data for applying cutoff */
    FLOATTYPE* maskCutPtr;

    /** Helper for update to filter */
    void subtractAverage(PsFieldBase<FLOATTYPE>& wField);

    /** Find maximum value in array */
    size_t findMaxPos(FLOATTYPE* data, size_t dataSize);

    /** Find maximum value in array */
    FLOATTYPE findMaxVal(FLOATTYPE* data, size_t dataSize);

  private:

    /** FFT object name */
    std::string fftKind;

    /** Constructor private to prevent use */
    PsSpecFilterUpdater(const PsSpecFilterUpdater<FLOATTYPE, NDIM>& psb);

   /** Assignment private to prevent use */
    PsSpecFilterUpdater<FLOATTYPE, NDIM>& operator=(
       const PsSpecFilterUpdater<FLOATTYPE, NDIM>& psb);
};

#endif  // PS_SPEC_FILTER_UPDATER_H
