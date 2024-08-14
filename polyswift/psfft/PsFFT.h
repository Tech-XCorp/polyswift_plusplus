/**
 *
 * @file    PsFFT.h
 *
 * @brief   Fourier transform interface class
 *
 * @version $Id: PsFFT.h 8329 2007-09-21 16:12:04Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_FFT_H
#define PS_FFT_H

// std includes
#include <string>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsFFTBase.h>

/**
 * Fourier transform interface class
 */
template <class FLOATTYPE, size_t NDIM>
class PsFFT : public virtual PsFFTBase<FLOATTYPE, NDIM> {

  public:

  PsFFT();

  virtual ~PsFFT();

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
 * Get grid assoicated with fft object creation
 *
 * @return reference to grid object base interface
 */
    virtual PsGridBase<FLOATTYPE, NDIM>& getGrid() {
      return *gridPtr;
    }

  protected:

    /** Global dimensions of grid */
    std::vector<size_t> globalDims;

 private:

    /** grid associated with object */
    PsGridBase<FLOATTYPE, NDIM>* gridPtr;

    /** Name of grid object, picks decomp */
    std::string gridKind;
};

#endif // PS_FFT_H
