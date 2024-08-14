/**
 *
 * @file    PsPoissonUpdater.h
 *
 * @brief   Class for updating fields with Poisson solve
 *
 * @version $Id: PsPoissonUpdater.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2010-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_POISSON_UPDATER_H
#define PS_POISSON_UPDATER_H

// standard headers
#include <list>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#undef HAVE_CONFIG_H
#endif

// txbase headers
#include <TxDebugExcept.h>
#include <txc_math.h>

// psbase includes
#include <PsPhysField.h>
#include <PsFieldBase.h>
#include <PsFFTBase.h>

// pseffhamil includes
#include <PsUpdater.h>

/**
 * A PsPoissonUpdater object updates fields using a Poisson solve
 * of the basic form  nabla psi(r) = -rho(r)
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 */

template <class FLOATTYPE, size_t NDIM>
class PsPoissonUpdater : public virtual PsUpdater<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 */
    PsPoissonUpdater();

/**
 * Destructor
 */
    virtual ~PsPoissonUpdater();

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

    // SWS: history access
/**
 * Get free-energy value
 *
 * @return the free-energy
 */
    virtual FLOATTYPE getFreeE(bool calcDisorder=true) {
      TxDebugExcept tde("PsPoissonUpdater::getFreeE not yet implemented");
      throw tde;
      // return 0.0;
    }

  protected:

    /** Input hierarchy set for STFunc */
    TxHierAttribSetIntDbl funcAttrib;

    /** Names of STFuncs */
    std::vector<std::string> funcNames;

    /** Function for updating cutoff parameter */
    PsSTFunc<FLOATTYPE, NDIM>* cutSTFunc;

    /** Flag for cutSTFunc */
    bool hasCutSTFunc;

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

  private:

    /** FFT object name */
    std::string fftKind;

    /** Relaxation parameter */
    FLOATTYPE relaxlambda;

    /** Bjeerum length */
    FLOATTYPE bjerrumLen;        // RK: should this be here?

    /** The Laplacian factor */
    FLOATTYPE*  k2;

    /** Initialize the "Laplacian" list */
    void build_k2();

    /** Constructor private to prevent use */
    PsPoissonUpdater(const PsPoissonUpdater<FLOATTYPE, NDIM>& psb);

   /** Assignment private to prevent use */
    PsPoissonUpdater<FLOATTYPE, NDIM>& operator=(
       const PsPoissonUpdater<FLOATTYPE, NDIM>& psb);
};

#endif  // PS_POISSON_UPDATER_H
