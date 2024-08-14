/**
 *
 * @file    PsFFTHldr.cpp
 *
 * @brief   Base class owning instances of input/output object(s)
 *
 * @version $Id: PsFFTHldr.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psfft includes
#include <PsFFTHldr.h>

#include <PsCommBase.h>

// txbase includes
#include <TxMakerMap.h>

template <class FLOATTYPE, size_t NDIM>
void PsFFTHldr<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Look for FFT blocks in input file
  fftNames = tas.getNamesOfType("FFT");

  // Set the number of FFT blocks found
  numFFTs = fftNames.size();

  // Put each fft attribute set into local list
  for (size_t i=0; i<numFFTs; ++i) {

    this->pprt("Found <FFT> block:  ", fftNames[i]);
    TxHierAttribSetIntDbl attribs = tas.getAttrib(fftNames[i]);
    fftAttribs.push_back(attribs);
  }
}

template <class FLOATTYPE, size_t  NDIM>
void PsFFTHldr<FLOATTYPE, NDIM>::buildData(PsNamedObject* inOwner) {

  this->dbprt("PsFFTHldr::buildData() ");

  for (size_t i=0; i<fftNames.size(); ++i) {

    // For each FFT look for kind
    std::string kind = fftAttribs[i].getString("kind");

    // Makermap returns pointer to a derived object of type PsFFT
    PsFFTBase<FLOATTYPE, NDIM>* fftPtr =
        TxMakerMap< PsFFTBase<FLOATTYPE, NDIM> >::getNew(kind);

    // Set parameters and attributes for particular fft and build
    fftPtr->setName(fftNames[i]);
    fftPtr->setDomainSingletons( this->getDomainSingletons() );
    fftPtr->setAttrib(fftAttribs[i]);

    // New name registration and build
    fftPtr->setOwner(inOwner);
    inOwner->makeAvail(fftPtr, fftNames[i]);
    fftPtr->buildData();

    // Push pointer into ffts structure
    ffts.push_back(fftPtr);
  }
}

template <class FLOATTYPE, size_t  NDIM>
void PsFFTHldr<FLOATTYPE, NDIM>::buildSolvers() {

  this->dbprt("PsFFTHldr::buildSolvers() ");

  // Loop over vector of grid pointers and call buildSolvers()
  typename std::vector< PsFFTBase<FLOATTYPE, NDIM>* >::iterator ifft;
  for (ifft = ffts.begin(); ifft != ffts.end(); ++ifft) {
    (*ifft)->buildSolvers();
  }

}

// Instantiate base fft holder classes
template class PsFFTHldr<float, 1>;
template class PsFFTHldr<float, 2>;
template class PsFFTHldr<float, 3>;

template class PsFFTHldr<double, 1>;
template class PsFFTHldr<double, 2>;
template class PsFFTHldr<double, 3>;

