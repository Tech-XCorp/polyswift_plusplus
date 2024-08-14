/**
 *
 * @file    PsEffHamilMakerMap.cpp
 *
 * @brief   Construct the makers for all polymer objects
 *
 * @version $Id: PsEffHamilMakerMap.cpp 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// pseffhamil includes
#include <PsEffHamilMakerMap.h>
#include <PsCanonicalMF.h>

// txbase includes
#include <TxMakerMap.h>

//
// Constructor for maker map
//
template <class FLOATTYPE, size_t NDIM>
PsEffHamilMakerMap<FLOATTYPE, NDIM>::PsEffHamilMakerMap() {

// Make sure map exists
  TxMakerMap<PsEffHamilBase<FLOATTYPE, NDIM>>::getInstance();

// Add the makers (they are deleted by the makermap)
  new TxMaker< PsCanonicalMF<FLOATTYPE, NDIM>,
        PsEffHamilBase<FLOATTYPE, NDIM> >("canonicalMF");
}

template <class FLOATTYPE, size_t NDIM>
PsEffHamilMakerMap<FLOATTYPE, NDIM>::~PsEffHamilMakerMap() {
}

//
// Instantiate the maker maps
//
template class PsEffHamilMakerMap<float, 1>;
template class PsEffHamilMakerMap<float, 2>;
template class PsEffHamilMakerMap<float, 3>;

template class PsEffHamilMakerMap<double, 1>;
template class PsEffHamilMakerMap<double, 2>;
template class PsEffHamilMakerMap<double, 3>;

