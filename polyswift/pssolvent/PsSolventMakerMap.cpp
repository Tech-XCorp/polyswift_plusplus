/**
 *
 * @file    PsSolventMakerMap.cpp
 *
 * @brief   Construct the makers for all polymer objects
 *
 * @version $Id: PsSolventMakerMap.cpp 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// pssolvent includes
#include <PsSolventMakerMap.h>
#include <PsSimpleSolvent.h>
#include <PsSimpleIons.h>

// txbase includes
#include <TxMakerMap.h>

//
// Constructor for maker map
//
template <class FLOATTYPE, size_t NDIM>
PsSolventMakerMap<FLOATTYPE, NDIM>::PsSolventMakerMap() {

// Make sure map exists
  TxMakerMap<PsSolvent<FLOATTYPE, NDIM>>::getInstance();

// Add the makers (they are deleted by the makermap)
  new TxMaker< PsSimpleSolvent<FLOATTYPE, NDIM>,
        PsSolvent<FLOATTYPE, NDIM> >("simpleSolvent");

  new TxMaker< PsSimpleIons<FLOATTYPE, NDIM>,
        PsSolvent<FLOATTYPE, NDIM> >("simpleIons");
}

template <class FLOATTYPE, size_t NDIM>
PsSolventMakerMap<FLOATTYPE, NDIM>::~PsSolventMakerMap() {
}

//
// Instantiate the maker maps
//
template class PsSolventMakerMap<float, 1>;
template class PsSolventMakerMap<float, 2>;
template class PsSolventMakerMap<float, 3>;

template class PsSolventMakerMap<double, 1>;
template class PsSolventMakerMap<double, 2>;
template class PsSolventMakerMap<double, 3>;

