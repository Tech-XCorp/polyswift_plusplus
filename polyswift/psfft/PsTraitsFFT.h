/**
 *
 * @file    PsTraitsFFT.h
 *
 * @brief   Define numeric traits for FFT's
 *
 * @version $Id: $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_TRAITS_FFT_H
#define PS_TRAITS_FFT_H

// system includes
#include <float.h>
#include <stdio.h>

// std includes
#include <complex>
#include <vector>

// configure flags
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// include MPI/FFTW
#ifdef HAVE_MPI
#define MPICH_IGNORE_CXX_SEEK
#include <mpi.h>
#include <fftw_mpi.h>
#else
#include <fftw.h>
#endif

// Block end specification
enum FFTCommType {SERIAL, MPI};

  /*
Called with:
  typedef typename PsTraitsFFT<double>::RealVecType realVec;
  realVec tstVec;
  for (size_t n=0; n<5; ++n) {
    tstVec.push_back(n);
    std::cout << "tstVec[" << n << "] = " << tstVec[n] << std::endl;
  }
  */

/** A templated struct for defining some traits of FFT types/classes.
 *  The traits should be used through "typename".
 *
 *  @author Scott W. Sides
 */

template <class U>
struct PsTraitsFFT { };

/* specialization to serial/(float?) */
template <>
struct PsTraitsFFT<float> {
  typedef float              RealType;
  typedef std::vector<float> RealVecType;
  typedef float*             PtrType;
};

/* specialization to serial/(float?) */
template <>
struct PsTraitsFFT<double> {
  fftwnd_plan planTst;
  typedef double              RealType;
  typedef std::vector<double> RealVecType;
  typedef double*             PtrType;
};

/* specialization to serial FFTW
template <>
struct PsTraitsFFT<SERIAL> {
  typedef fftwnd_plan PlanType;
};
 */

/* specialization to MPI FFTW
template <>
struct PsTraitsFFT<fftwnd_mpi_plan> {
  typedef fftwnd_mpi_plan PlanType;
};
*/

#endif  // PS_TRAITS_FFT_H
