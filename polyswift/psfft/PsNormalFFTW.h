/**
 *
 * @file    PsNormalFFTW.h
 *
 * @brief   Fourier transform using FFTW with 'normal' data layout
 *
 * @version $Id: PsNormalFFTW.h 8329 2007-09-21 16:12:04Z swsides $
 *
 * Copyright &copy; 2010-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_NORMAL_FFTW_H
#define PS_NORMAL_FFTW_H

// std includes
#include <string>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psfft includes
#include <PsFFTW.h>

// include MPI/FFTW
#ifdef HAVE_MPI
#define MPICH_IGNORE_CXX_SEEK
#include <mpi.h>
#include <fftw_mpi.h>
typedef fftwnd_mpi_plan planTypeNormal;
#else
#include <fftw.h>
typedef fftwnd_plan     planTypeNormal;
#endif

/**
 * Fastest Fourier-transform in the West interface class
 */
template <class FLOATTYPE, size_t NDIM>
class PsNormalFFTW : public virtual PsFFTW<FLOATTYPE, NDIM> {

 public:

/**
 * constructor
 */
  PsNormalFFTW();

/**
 * Destructor
 */
  virtual ~PsNormalFFTW();

/**
 * Store the data needed to build this object.
 *
 * @param tas the attribute set containing
 *            the initial conditions
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
 * Forward transform real input data and return |a+bi| elementwise
 *
 * @param data1  pointer to REAL data to transform
 * @param resPtr pointer to Re[result] (also supplied by caller)
 */
   virtual void forwardFFTAbs(const FLOATTYPE* data1,
       FLOATTYPE* resPtr);

/**
 * Forward transform real input data, multiply elementwise and
 * backward transform
 *
 * @param data1  pointer to REAL data to transform
 * @param data2  pointer to REAL to scale transform
 * @param resPtr pointer to Re[result] (also supplied by caller)
 */
   virtual void convolveRe(const FLOATTYPE* data1,
       const FLOATTYPE* data2, FLOATTYPE* resPtr);

/**
 * Perform multi-dimensional FFT pair, scaling the
 * result by kdata in-between transform pair
 *
 * @param data  pointer to REAL data to transform
 * @param kdata pointer to data to scale transform
 * @param resPtr pointer to Re[result] (also supplied by caller)
 */
   virtual void scaledFFTPair(const FLOATTYPE* data,
       const FLOATTYPE* kdata, FLOATTYPE* resPtr);

/**
 * Perform multi-dimensional FFT pair, scaling the
 * result by kdata in-between transform pair
 *
 * @param data  pointer to IMAGINARY data to transform
 * @param kdata pointer to data to scale transform
 * @param resPtr pointer to Re[result] (also supplied by caller)
 */
   virtual void scaledFFTPairIm(const FLOATTYPE* data,
       const FLOATTYPE* kdata, FLOATTYPE* resPtr);

/**
 * Perform forward multi-dimensional FFT
 *
 * @param data pointer to REAL data to transform
 * @param resPtr pointer to Re[result] (also supplied by caller)
 */
   virtual void calcForwardFFT(const FLOATTYPE* data,
       FLOATTYPE* resPtr);

/**
 * Perform backward (inverse) multi-dimensional FFT
 *
 * @param data pointer to REAL data to transform
 * @param resPtr pointer to Re[result] (also supplied by caller)
 */
   virtual void calcBackwardFFT(const FLOATTYPE* data,
       FLOATTYPE* resPtr);

 protected:

 private:

   /** FFTW plan for forward transforms */
   planTypeNormal forwardNPlan;

   /** FFTW plan for backward transforms */
   planTypeNormal backwardNPlan;

};

#endif // PS_NORMAL_FFTW_H
