/**
 *
 * @file    PsFFTW.cpp
 *
 * @brief   Fourier transform using FFTW
 *
 * @version $Id: PsFFTW.cpp 6319 2006-11-14 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psfft includes
#include <PsFFTW.h>

template <class FLOATTYPE, size_t NDIM>
PsFFTW<FLOATTYPE, NDIM>::PsFFTW() {

  // Setting defaults for serial) so
  // these can be used universally to setup data structures
  total_local_size = 1;

  // Number of data sets to transform
  n_fields = 1;

  // Set pointers
  in = NULL;
  out = NULL;
  work = NULL;
}

template <class FLOATTYPE, size_t NDIM>
PsFFTW<FLOATTYPE, NDIM>::~PsFFTW() {

  delete[] in;
  delete[] out;
  delete[] work;

  fftwnd_destroy_plan(forwardPlan);
  fftwnd_destroy_plan(backwardPlan);
}

template <class FLOATTYPE, size_t NDIM>
void PsFFTW<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Scoping call to base class
  PsFFT<FLOATTYPE, NDIM>::setAttrib(tas);

  this->dbprt("PsFFTW::setAttrib() ");
}

template <class FLOATTYPE, size_t NDIM>
void PsFFTW<FLOATTYPE, NDIM>::buildData() {

  // Scoping call to base class
  PsFFT<FLOATTYPE, NDIM>::buildData();

  this->dbprt("PsFFTW::buildData() ");

  // *************************************************
  // Set rank/dimensions for plans
  // *************************************************
  std::vector<size_t> dims;
  dims = this->globalDims;
  int rank = (int)dims.size();

  int* planDims = new int[rank];
  for (int n=0; n<rank; ++n) planDims[n] = dims[n];

  // This sets plans and sizes
  forwardPlan  = fftwnd_create_plan(rank,planDims, FFTW_FORWARD,
      FFTW_ESTIMATE);

  backwardPlan = fftwnd_create_plan(rank,planDims, FFTW_BACKWARD,
      FFTW_ESTIMATE);

// Setting FFTW size info from global
  int total_size = 1;
  for (int n=0; n<rank; ++n) {
    total_size = total_size * planDims[n];
  }

// Explicitly free local memory
  delete[] planDims;

// Set common data member in FFTW base class
  setFFTWMem(total_size);
}

//
// Local helper method to unify memory allocation
// during build cycle
//
template <class FLOATTYPE, size_t NDIM>
void PsFFTW<FLOATTYPE, NDIM>::setFFTWMem(int fftwSize) {

  // Set common data member in FFTW base class
  total_local_size = (int)fftwSize;

  this->dbprt("total_local_size = ", total_local_size);

  // Setup internal work spaces
  in   = new fftw_complex[total_local_size];
  out  = new fftw_complex[total_local_size];
  work = new fftw_complex[total_local_size];
}

template <class FLOATTYPE, size_t NDIM>
void PsFFTW<FLOATTYPE, NDIM>::buildSolvers() {

  // Scoping call to base class
  PsFFT<FLOATTYPE, NDIM>::buildSolvers();
}

/*
 * *************************
 * FFTW calls in serial
 * *************************
 */

template <class FLOATTYPE, size_t NDIM>
void PsFFTW<FLOATTYPE, NDIM>::forwardFFTAbs(
   const FLOATTYPE* data1, FLOATTYPE* resPtr) {

  this->dbprt("PsFFTW::forwardFFTAbs serial");

  // Format data for fft_complex data type
  for (int n=0; n<total_local_size; ++n) {
    in[n].re = data1[n];
    in[n].im = 0.0;
  }

  // FFT returned through the "out" arrary
  fftwnd_one(forwardPlan, in, out);

  // Calculate absolute value
  for (int n=0; n<total_local_size; ++n) {
    in[n].re = (out[n].re * out[n].re) + (out[n].im * out[n].im);
    in[n].im = 0.0;
  }

  // Format data for output
  for (int n=0; n<total_local_size; ++n) {
    resPtr[n] = in[n].re;
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsFFTW<FLOATTYPE, NDIM>::convolveRe(
   const FLOATTYPE* data1, const FLOATTYPE* data2, FLOATTYPE* resPtr){

  this->dbprt("PsFFTW::convolveRe serial");

  // Local space... must be managed by this method
  fftw_complex* in2  = new fftw_complex[total_local_size];
  fftw_complex* out2 = new fftw_complex[total_local_size];

  // Format data for fft_complex data type
  for (int n=0; n<total_local_size; ++n) {
    in[n].re = data1[n];
    in[n].im = 0.0;
    in2[n].re = data2[n];
    in2[n].im = 0.0;
  }

  // FFT returned through the "out" arrary
  fftwnd_one(forwardPlan, in, out);
  fftwnd_one(forwardPlan, in2, out2);

  // Scale transform result by kdata
  // (both Re/Im)
  for (int n=0; n<total_local_size; ++n) {
    in[n].re = (out[n].re * out2[n].re) - (out[n].im * out2[n].im);
    in[n].im = (out[n].im * out2[n].re) + (out[n].re * out2[n].im);
  }

  // FFT returned through the "out" arrary
  fftwnd_one(backwardPlan, in, out);

  // Format data for output
  for (int n=0; n<total_local_size; ++n) {
    resPtr[n] = out[n].re;
  }

  delete[] in2;
  delete[] out2;
}

//
// Utility method where by F(data)*kdata elementwise
// ie. the real and imaginary parts of the transform of data is
// scaled by the real kdata array
//
template <class FLOATTYPE, size_t NDIM>
void PsFFTW<FLOATTYPE, NDIM>::scaledFFTPair(
   const FLOATTYPE* data, const FLOATTYPE* kdata, FLOATTYPE* resPtr){

  this->dbprt("PsFFTW::scaledFFTPair serial");

  // Format data for fft_complex data type
  for (int n=0; n<total_local_size; ++n) {
    in[n].re = data[n];
    in[n].im = 0.0;
  }

  // FFT returned through the "out" arrary
  fftwnd_one(forwardPlan, in, out);

  // Scale transform result by kdata (both Re/Im)
  for (int n=0; n<total_local_size; ++n) {
    in[n].re = out[n].re * kdata[n];
    in[n].im = out[n].im * kdata[n];
  }

  // FFT returned through the "out" arrary
  fftwnd_one(backwardPlan, in, out);

  // Format data for output
  for (int n=0; n<total_local_size; ++n) {
    resPtr[n] = out[n].re;
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsFFTW<FLOATTYPE, NDIM>::scaledFFTPairIm(
    const FLOATTYPE* data, const FLOATTYPE* kdata, FLOATTYPE* resPtr){

  this->dbprt("PsFFTW::scaledFFTPairIm serial");

  // Format data for fft_complex data type
  for (int n=0; n<total_local_size; ++n) {
    in[n].re = 0.0;
    in[n].im = data[n];
  }

  // FFT returned through the "out" arrary
  fftwnd_one(forwardPlan, in, out);

  // Scale transform result by kdata (both Re/Im)
  for (int n=0; n<total_local_size; ++n) {
    in[n].re = out[n].re * kdata[n];
    in[n].im = out[n].im * kdata[n];
  }

  // FFT returned through the "out" arrary
  fftwnd_one(backwardPlan, in, out);

  // Format data for output
  for (int n=0; n<total_local_size; ++n) {
    resPtr[n] = out[n].re;
  }

  this->dbprt("PsFFTW::scaledFFTPairIm serial finished");

}

template <class FLOATTYPE, size_t NDIM>
void PsFFTW<FLOATTYPE, NDIM>::calcForwardFFT(const FLOATTYPE* data, FLOATTYPE* resPtr) {

  this->dbprt("PsFFTW::calcForwardFFT serial");

  // Format data for fft_complex data type
  for (int n=0; n<total_local_size; ++n) {
    in[n].re = data[n];
    in[n].im = 0.0;
  }

  // FFT returned through the "out" arrary
  fftwnd_one(forwardPlan, in, out);

  // Format data for float type
  for (int n=0; n<total_local_size; ++n) {
    resPtr[n] = out[n].re;
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsFFTW<FLOATTYPE, NDIM>::calcBackwardFFT(const FLOATTYPE* data, FLOATTYPE* resPtr) {

  this->dbprt("PsFFTW::calcBackwardFFT serial");

  // Format data for fft_complex data type
  for (int n=0; n<total_local_size; ++n) {
    in[n].re = data[n];
    in[n].im = 0.0;
  }

  // FFT returned through the "out" arrary
  fftwnd_one(backwardPlan, in, out);

  // Format data for float type
  for (int n=0; n<total_local_size; ++n)
    resPtr[n] = out[n].re;

}

template class PsFFTW<float, 1>;
template class PsFFTW<float, 2>;
template class PsFFTW<float, 3>;

template class PsFFTW<double, 1>;
template class PsFFTW<double, 2>;
template class PsFFTW<double, 3>;
