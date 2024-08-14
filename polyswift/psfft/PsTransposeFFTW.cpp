/**
 *
 * @file    PsTransposeFFTW.cpp
 *
 * @brief   Fourier transform using FFTW with TRANSPOSE_ORDER
 *          implementation
 *
 * @version $Id: PsTransposeFFTW.cpp 6319 2006-11-14 22:39:46Z swsides $
 *
 * Copyright &copy; 2010-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_MPI
#define MPICH_IGNORE_CXX_SEEK
#include <mpi.h>
#include <fftw_mpi.h>
#else
#include <fftw.h>
#endif

// psfft includes
#include <PsTransposeFFTW.h>

template <class FLOATTYPE, size_t NDIM>
PsTransposeFFTW<FLOATTYPE, NDIM>::PsTransposeFFTW() {
}

template <class FLOATTYPE, size_t NDIM>
PsTransposeFFTW<FLOATTYPE, NDIM>::~PsTransposeFFTW() {

#ifdef HAVE_MPI
  fftwnd_mpi_destroy_plan(forwardTPlan);
  fftwnd_mpi_destroy_plan(backwardTPlan);
#endif
}

template <class FLOATTYPE, size_t NDIM>
void PsTransposeFFTW<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Scoping call to base class
  PsFFTW<FLOATTYPE, NDIM>::setAttrib(tas);
}

template <class FLOATTYPE, size_t NDIM>
void PsTransposeFFTW<FLOATTYPE, NDIM>::buildData() {

#ifdef HAVE_MPI

  // Scoping call to base class
  PsFFT<FLOATTYPE, NDIM>::buildData();

  this->dbprt("PsTransposeFFTW::buildData() ");

  // *************************************************
  // Set rank/dimensions for plans
  // *************************************************
  std::vector<size_t> dims;
  dims = this->globalDims;
  int rank = (int)dims.size();

  // Translate std::vector to int pointer to setup
  // special transpose plan for backward MPI FFT
  int* planDims = new int[rank];
  int* planTransposeDims = new int[rank];
  for (int n=0; n<rank; ++n) {
    planDims[n] = dims[n];
    planTransposeDims[n] = dims[n];
  }

  // Switch x dim and y dim
  int tmp;
  tmp = planTransposeDims[0];
  planTransposeDims[0] = planTransposeDims[1];
  planTransposeDims[1] = tmp;

  forwardTPlan =
      fftwnd_mpi_create_plan(MPI_COMM_WORLD,rank,planDims,
      FFTW_FORWARD, FFTW_ESTIMATE);
  backwardTPlan =
      fftwnd_mpi_create_plan(MPI_COMM_WORLD,rank,planTransposeDims,
      FFTW_BACKWARD, FFTW_ESTIMATE);

  // Explicitly free local memory
  delete[] planDims;
  delete[] planTransposeDims;

  // Set FFTW parameters only need total_size from here
  // Decomp sets everything else
  int local_nx;
  int local_x_start = 0;
  int local_ny_after_transpose;
  int local_y_start_after_transpose = 0;
  int total_size = 0;

  fftwnd_mpi_local_sizes(forwardTPlan, &local_nx, &local_x_start,
      &local_ny_after_transpose, &local_y_start_after_transpose,
      &total_size);

  // Set common data member in FFTW base class
  this->setFFTWMem(total_size);

#else

  // Scoping call to base class
  PsFFTW<FLOATTYPE, NDIM>::buildData();
#endif
}

template <class FLOATTYPE, size_t NDIM>
void PsTransposeFFTW<FLOATTYPE, NDIM>::buildSolvers() {

  // Scoping call to base class
  PsFFTW<FLOATTYPE, NDIM>::buildSolvers();
}

#ifdef HAVE_MPI

template <class FLOATTYPE, size_t NDIM>
void PsTransposeFFTW<FLOATTYPE, NDIM>::forwardFFTAbs(
    const FLOATTYPE* data1, FLOATTYPE* resPtr) {

  TxDebugExcept tde("PsTransposeFFTW::forwardFFTAbs MPI");
  tde << "... not yet implemented";
  throw tde;
}

//
// Used for convolution integral calculation
//
template <class FLOATTYPE, size_t NDIM>
void PsTransposeFFTW<FLOATTYPE, NDIM>::convolveRe(
    const FLOATTYPE* data1, const FLOATTYPE* data2, FLOATTYPE* resPtr){

  // Local space... must be managed by this method
  fftw_complex* in2 = new fftw_complex[this->total_local_size];
  fftw_complex tmp;

  this->dbprt("PsTransposeFFTW::convolveRe MPI ");

  // Format data1/data2 for fft_complex data type
  for (int n=0; n<this->total_local_size; ++n) {
    this->in[n].re = data1[n];
    this->in[n].im = 0.0;
    in2[n].re = data2[n];
    in2[n].im = 0.0;
  }

  // FFT returned in-place to the "in" arrary
  fftwnd_mpi(forwardTPlan, this->n_fields, this->in,  this->work, FFTW_TRANSPOSED_ORDER);
  fftwnd_mpi(forwardTPlan, this->n_fields, in2, this->work, FFTW_TRANSPOSED_ORDER);

  // Multiply transforms
  for (int n=0; n<this->total_local_size; ++n) {
    tmp.re = (this->in[n].re * in2[n].re) - (this->in[n].im * in2[n].im);
    tmp.im = (this->in[n].im * in2[n].re) + (this->in[n].re * in2[n].im);
    this->in[n].re = tmp.re;
    this->in[n].im = tmp.im;
  }

  // FFT returned in-place to the "in" arrary
  //  fftwnd_mpi(backwardPlan, this->n_fields, this->in, this->work, FFTW_NORMAL_ORDER);
  fftwnd_mpi(backwardTPlan, this->n_fields, this->in, this->work, FFTW_TRANSPOSED_ORDER);

  // Format data for output
  for (int n=0; n<this->total_local_size; ++n) {
    resPtr[n] = this->in[n].re;
  }

  delete[] in2;
}

//
// Utility method where by F(data)*kdata elementwise
// ie. the real and imaginary parts of the transform of data is
// scaled by the real kdata array
//
template <class FLOATTYPE, size_t NDIM>
void PsTransposeFFTW<FLOATTYPE, NDIM>::scaledFFTPair(
    const FLOATTYPE* data, const FLOATTYPE* kdata, FLOATTYPE* resPtr){

  this->dbprt("PsTransposeFFTW::scaledFFTPair MPI ");

  // Format data for fft_complex data type
  for (int n=0; n<this->total_local_size; ++n) {
    this->in[n].re = data[n];
    this->in[n].im = 0.0;
  }

  // FFT returned in-place to the "in" arrary
  fftwnd_mpi(forwardTPlan, this->n_fields, this->in, this->work,
      FFTW_TRANSPOSED_ORDER);

  // Scale transform result by kdata
  for (int n=0; n<this->total_local_size; ++n) {
    this->in[n].re = this->in[n].re * kdata[n];
    this->in[n].im = this->in[n].im * kdata[n];
  }

  // FFT returned in-place to the "in" arrary
  fftwnd_mpi(backwardTPlan, this->n_fields, this->in, this->work,
      FFTW_TRANSPOSED_ORDER);

  // Format data for output
  for (int n=0; n<this->total_local_size; ++n)
    resPtr[n] = this->in[n].re;

}

template <class FLOATTYPE, size_t NDIM>
void PsTransposeFFTW<FLOATTYPE, NDIM>::scaledFFTPairIm(
    const FLOATTYPE* data, const FLOATTYPE* kdata, FLOATTYPE* resPtr){

  this->dbprt("PsTransposeFFTW::scaledFFTPairIm MPI ");

  // Format data for fft_complex data type
  for (int n=0; n<this->total_local_size; ++n) {
    this->in[n].re = 0.0;
    this->in[n].im = data[n];
  }

  // FFT returned in-place to the "in" arrary
  fftwnd_mpi(forwardTPlan, this->n_fields, this->in, this->work, FFTW_TRANSPOSED_ORDER);

  // Scale transform result by kdata
  for (int n=0; n<this->total_local_size; ++n) {
    this->in[n].re = this->in[n].re * kdata[n];
    this->in[n].im = this->in[n].im * kdata[n];
  }

  // FFT returned in-place to the "in" arrary
  fftwnd_mpi(backwardTPlan, this->n_fields, this->in, this->work,
      FFTW_TRANSPOSED_ORDER);

  // Format data for output
  for (int n=0; n<this->total_local_size; ++n)
    resPtr[n] = this->in[n].re;
}

template <class FLOATTYPE, size_t NDIM>
void PsTransposeFFTW<FLOATTYPE, NDIM>::calcForwardFFT(const FLOATTYPE* data, FLOATTYPE* resPtr) {

  this->dbprt("PsTransposeFFTW::calcForwardFFT MPI ");

  // Format data for fft_complex data type
  for (int n=0; n<this->total_local_size; ++n) {
    this->in[n].re = data[n];
    this->in[n].im = 0.0;
  }

  planTypeTrans fftPlan = forwardTPlan;
  fftwnd_mpi_output_order output_order = FFTW_TRANSPOSED_ORDER;

  // FFT returned in-place to the "in" arrary
  fftwnd_mpi(fftPlan, this->n_fields, this->in, this->work, output_order);

  // Format output data for float type
  for (int n=0; n<this->total_local_size; ++n) {
    resPtr[n] = this->in[n].re;
  }

} // calcForwardFFT

template <class FLOATTYPE, size_t NDIM>
void PsTransposeFFTW<FLOATTYPE, NDIM>::calcBackwardFFT(const FLOATTYPE* data, FLOATTYPE* resPtr) {

  this->dbprt("PsTransposeFFTW::calcBackwardFFT MPI ");

  // Format data for fft_complex data type
  for (int n=0; n<this->total_local_size; ++n) {
    this->in[n].re = data[n];
    this->in[n].im = 0.0;
  }

  planTypeTrans fftPlan = backwardTPlan;
  fftwnd_mpi_output_order output_order = FFTW_TRANSPOSED_ORDER;

  // FFT returned in-place to the "in" arrary
  fftwnd_mpi(fftPlan, this->n_fields, this->in, this->work, output_order);

  // Format output data for float type
  for (int n=0; n<this->total_local_size; ++n) {
    resPtr[n] = this->in[n].re;
  }

}
#endif // HAVE_MPI

#ifndef HAVE_MPI
/*
 * *************************
 * FFTW calls in serial
 * *************************
 */

template <class FLOATTYPE, size_t NDIM>
void PsTransposeFFTW<FLOATTYPE, NDIM>::forwardFFTAbs(
   const FLOATTYPE* data1, FLOATTYPE* resPtr) {

  // Scoping call to common serial methods
  PsFFTW<FLOATTYPE, NDIM>::forwardFFTAbs(data1, resPtr);
}

template <class FLOATTYPE, size_t NDIM>
void PsTransposeFFTW<FLOATTYPE, NDIM>::convolveRe(
   const FLOATTYPE* data1, const FLOATTYPE* data2, FLOATTYPE* resPtr){

  // Scoping call to common serial methods
  PsFFTW<FLOATTYPE, NDIM>::convolveRe(data1, data2, resPtr);
}

//
// Utility method where by F(data)*kdata elementwise
// ie. the real and imaginary parts of the transform of data is
// scaled by the real kdata array
//
template <class FLOATTYPE, size_t NDIM>
void PsTransposeFFTW<FLOATTYPE, NDIM>::scaledFFTPair(
   const FLOATTYPE* data, const FLOATTYPE* kdata, FLOATTYPE* resPtr){

  // Scoping call to common serial methods
  PsFFTW<FLOATTYPE, NDIM>::scaledFFTPair(data, kdata, resPtr);
}

template <class FLOATTYPE, size_t NDIM>
void PsTransposeFFTW<FLOATTYPE, NDIM>::scaledFFTPairIm(
    const FLOATTYPE* data, const FLOATTYPE* kdata, FLOATTYPE* resPtr){

  // Scoping call to common serial methods
  PsFFTW<FLOATTYPE, NDIM>::scaledFFTPairIm(data, kdata, resPtr);
}

template <class FLOATTYPE, size_t NDIM>
void PsTransposeFFTW<FLOATTYPE, NDIM>::calcForwardFFT(const FLOATTYPE* data, FLOATTYPE* resPtr) {

  // Scoping call to common serial methods
  PsFFTW<FLOATTYPE, NDIM>::calcForwardFFT(data, resPtr);

}

template <class FLOATTYPE, size_t NDIM>
void PsTransposeFFTW<FLOATTYPE, NDIM>::calcBackwardFFT(const FLOATTYPE* data, FLOATTYPE* resPtr) {

  // Scoping call to common serial methods
  PsFFTW<FLOATTYPE, NDIM>::calcBackwardFFT(data, resPtr);

}
#endif // SERIAL

template class PsTransposeFFTW<float, 1>;
template class PsTransposeFFTW<float, 2>;
template class PsTransposeFFTW<float, 3>;

template class PsTransposeFFTW<double, 1>;
template class PsTransposeFFTW<double, 2>;
template class PsTransposeFFTW<double, 3>;
