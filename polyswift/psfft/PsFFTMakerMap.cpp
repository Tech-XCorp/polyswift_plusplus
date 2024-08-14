/**
 *
 * @file    PsFFTMakerMap.cpp
 *
 * @brief   Construct the makers for FFT objects
 *
 * @version $Id: PsFFTMakerMap.cpp 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psfft includes
#include <PsFFTMakerMap.h>
#include <PsNormalFFTW.h>
#include <PsTransposeFFTW.h>

// txbase includes
#include <TxMakerMap.h>

//
// Constructor for maker map
//
template <class FLOATTYPE, size_t NDIM>
PsFFTMakerMap<FLOATTYPE, NDIM>::PsFFTMakerMap() {

// Make sure map exists
  TxMakerMap<PsFFTBase<FLOATTYPE, NDIM>>::getInstance();

// Add the makers (they are deleted by the makermap)
  new TxMaker< PsNormalFFTW<FLOATTYPE, NDIM>,
        PsFFTBase<FLOATTYPE, NDIM> >("normalfftw");

  new TxMaker< PsTransposeFFTW<FLOATTYPE, NDIM>,
        PsFFTBase<FLOATTYPE, NDIM> >("transposefftw");
}

template <class FLOATTYPE, size_t NDIM>
PsFFTMakerMap<FLOATTYPE, NDIM>::~PsFFTMakerMap() {
}

//
// Instantiate the maker maps
//
template class PsFFTMakerMap<float, 1>;
template class PsFFTMakerMap<float, 2>;
template class PsFFTMakerMap<float, 3>;

template class PsFFTMakerMap<double, 1>;
template class PsFFTMakerMap<double, 2>;
template class PsFFTMakerMap<double, 3>;

