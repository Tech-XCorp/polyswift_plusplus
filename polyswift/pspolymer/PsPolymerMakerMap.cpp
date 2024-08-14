/**
 * @file    PsPolymerMakerMap.cpp
 *
 * @brief   Construct the makers for all polymer objects
 *
 * @version $Id: PsPolymerMakerMap.cpp 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// pspolymer includes
#include <PsPolymerMakerMap.h>
#include <PsBlockCopolymer.h>
#include <PsPolyDisperseBCP.h>

// txbase includes
#include <TxMakerMap.h>

//
// Constructor for maker map
//
template <class FLOATTYPE, size_t NDIM>
PsPolymerMakerMap<FLOATTYPE, NDIM>::PsPolymerMakerMap() {

// Make sure map exists
  TxMakerMap<PsPolymer<FLOATTYPE, NDIM>>::getInstance();

// Add the makers (they are deleted by the makermap)
// Fields that exist in all cases
  new TxMaker< PsBlockCopolymer<FLOATTYPE, NDIM>,
        PsPolymer<FLOATTYPE, NDIM> >("blockCopolymer");

  new TxMaker< PsPolyDisperseBCP<FLOATTYPE, NDIM>,
        PsPolymer<FLOATTYPE, NDIM> >("polyDisperseBCP");
}

template <class FLOATTYPE, size_t NDIM>
PsPolymerMakerMap<FLOATTYPE, NDIM>::~PsPolymerMakerMap() {
}

//
// Instantiate the maker maps
//
template class PsPolymerMakerMap<float, 1>;
template class PsPolymerMakerMap<float, 2>;
template class PsPolymerMakerMap<float, 3>;

template class PsPolymerMakerMap<double, 1>;
template class PsPolymerMakerMap<double, 2>;
template class PsPolymerMakerMap<double, 3>;

