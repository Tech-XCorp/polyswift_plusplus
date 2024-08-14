/**
 *
 * @file    PsCommMakerMap.cpp
 *
 * @brief   Construct the makers for all polymer objects
 *
 * @version $Id: PsCommMakerMap.cpp 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// pscomm includes
#include <PsCommMakerMap.h>
#include <PsMpiComm.h>

// txbase includes
#include <TxMakerMap.h>

//
// Constructor for maker map
//
template <class FLOATTYPE, size_t NDIM>
PsCommMakerMap<FLOATTYPE, NDIM>::PsCommMakerMap() {

// Make sure map exists
  TxMakerMap<PsCommBase<FLOATTYPE, NDIM>>::getInstance();

// Add the makers (they are deleted by the makermap)
  new TxMaker< PsMpiComm<FLOATTYPE, NDIM>,
        PsCommBase<FLOATTYPE, NDIM> >("mpiComm");
}

template <class FLOATTYPE, size_t NDIM>
PsCommMakerMap<FLOATTYPE, NDIM>::~PsCommMakerMap() {
}

//
// Instantiate the maker maps
//
template class PsCommMakerMap<float, 1>;
template class PsCommMakerMap<float, 2>;
template class PsCommMakerMap<float, 3>;

template class PsCommMakerMap<double, 1>;
template class PsCommMakerMap<double, 2>;
template class PsCommMakerMap<double, 3>;

