/**
 *
 * @file    PsFFTBase.h
 *
 * @brief   Base class for PsFFT that provides interface to any field.
 *
 * @version $Id: PsFFT.h 8329 2007-09-21 16:12:04Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */
#ifndef PS_FFT_BASE_H
#define PS_FFT_BASE_H

// std includes
#include <string>
#include <set>
#include <vector>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// txbase includes
#include <TxDebugExcept.h>

// psbase includes
#include <PsDynObj.h>
#include <PsGridBase.h>
#include <PsDecompBase.h>

// Specification of transform and inverse
enum DirType {FORWARD, BACKWARD};

/**
 * A field interface (PsFFTBase) class
 */
template <class FLOATTYPE, size_t NDIM>
class PsFFTBase : public virtual PsDynObj<FLOATTYPE, NDIM> {

  public:

  PsFFTBase();

  virtual ~PsFFTBase();

/**
 * Set the parameters
 *
 * @param tas the parameters
 */
  virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Build the data, such as updaters, parses input etc.
 */
  virtual void buildData() = 0;

/**
 * Build the solvers structures
 */
  virtual void buildSolvers() = 0;

/**
 * Get grid assoicated with fft object creation
 *
 * @return reference to grid object base interface
 */
  virtual PsGridBase<FLOATTYPE, NDIM>& getGrid() = 0;

/**
 * Get size of data transform
 */
  virtual size_t getFFTSize() = 0;

/**
 * Forward transform real input data and return |a+bi| elementwise
 *
 * @param data1  pointer to REAL data to transform
 * @param resPtr pointer to Re[result] (also supplied by caller)
 */
  virtual void forwardFFTAbs(const FLOATTYPE* data1,
      FLOATTYPE* resPtr) = 0;

/**
 * Forward transform real input data, multiply elementwise and
 * backward transform
 *
 * @param data1  pointer to REAL data to transform
 * @param data2  pointer to REAL to scale transform
 * @param resPtr pointer to Re[result] (also supplied by caller)
 */
  virtual void convolveRe(const FLOATTYPE* data1,
      const FLOATTYPE* data2, FLOATTYPE* resPtr) = 0;

/**
 * Perform multi-dimensional FFT pair, scaling the
 * result by kdata in-between transform pair
 *
 * @param data  pointer to REAL data to transform
 * @param kdata pointer to data to scale transform
 * @param resPtr pointer to Re[result] (also supplied by caller)
 */
  virtual void scaledFFTPair(const FLOATTYPE* data,
      const FLOATTYPE* kdata, FLOATTYPE* resPtr) = 0;

/**
 * Perform multi-dimensional FFT pair, scaling the
 * result by kdata in-between transform pair
 *
 * @param data  pointer to IMAGINARY data to transform
 * @param kdata pointer to data to scale transform
 * @param resPtr pointer to Re[result] (also supplied by caller)
 */
  virtual void scaledFFTPairIm(const FLOATTYPE* data,
      const FLOATTYPE* kdata, FLOATTYPE* resPtr) = 0;

/**
 * Perform forward multi-dimensional FFT:
 *
 * @param data pointer to REAL data to transform
 * @param resPtr pointer to Re[result] (also supplied by caller)
 */
   virtual void calcForwardFFT(const FLOATTYPE* data,
       FLOATTYPE* resPtr) = 0;

/**
 * Perform backward (inverse) multi-dimensional FFT:
 *
 * @param data pointer to REAL data to transform
 * @param resPtr pointer to Re[result] (also supplied by caller)
 */
   virtual void calcBackwardFFT(const FLOATTYPE* data,
       FLOATTYPE* resPtr) = 0;

  protected:

  private:

    /** Make private to prevent use */
    PsFFTBase(const PsFFTBase<FLOATTYPE, NDIM>& vphh);

    /** Make private to prevent use */
    PsFFTBase<FLOATTYPE, NDIM>& operator=(
        const PsFFTBase<FLOATTYPE, NDIM>& vphh);
};

#endif // PS_FFT_BASE_H
