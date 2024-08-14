/**
 *
 * @file    PsDecompMakerMap.cpp
 *
 * @brief   Construct the makers for all polymer objects
 *
 * @version $Id: PsDecompMakerMap.cpp 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psdecomp includes
#include <PsDecompMakerMap.h>
#include <PsDecompRegular.h>
#include <PsDecompFFTW.h>

// txbase includes
#include <TxMakerMap.h>

//
// Constructor for maker map
//
template <class FLOATTYPE, size_t NDIM>
PsDecompMakerMap<FLOATTYPE, NDIM>::PsDecompMakerMap() {

// Make sure map exists
  TxMakerMap<PsDecompBase<FLOATTYPE, NDIM>>::getInstance();

// Add the makers (they are deleted by the makermap)
  new TxMaker< PsDecompRegular<FLOATTYPE, NDIM>,
        PsDecompBase<FLOATTYPE, NDIM> >("regular");

  new TxMaker< PsDecompFFTW<FLOATTYPE, NDIM>,
        PsDecompBase<FLOATTYPE, NDIM> >("fftw");
}

template <class FLOATTYPE, size_t NDIM>
PsDecompMakerMap<FLOATTYPE, NDIM>::~PsDecompMakerMap() {
}

//
// Instantiate the maker maps
//
template class PsDecompMakerMap<float, 1>;
template class PsDecompMakerMap<float, 2>;
template class PsDecompMakerMap<float, 3>;

template class PsDecompMakerMap<double, 1>;
template class PsDecompMakerMap<double, 2>;
template class PsDecompMakerMap<double, 3>;

