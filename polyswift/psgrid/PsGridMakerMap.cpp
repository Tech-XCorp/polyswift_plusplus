/**
 *
 * @file    PsGridMakerMap.cpp
 *
 * @brief   Construct the makers for all polymer objects
 *
 * @version $Id: PsGridMakerMap.cpp 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psgrid includes
#include <PsGridMakerMap.h>
#include <PsUniCartGrid.h>

// txbase includes
#include <TxMakerMap.h>

//
// Constructor for maker map
//
template <class FLOATTYPE, size_t NDIM>
PsGridMakerMap<FLOATTYPE, NDIM>::PsGridMakerMap() {

// Make sure map exists
  TxMakerMap<PsGridBase<FLOATTYPE, NDIM>>::getInstance();

// Add the makers (they are deleted by the makermap)

// Fields that exist in all cases
  new TxMaker< PsUniCartGrid<FLOATTYPE, NDIM>,
        PsGridBase<FLOATTYPE, NDIM> >("uniCartGrid");

}

template <class FLOATTYPE, size_t NDIM>
PsGridMakerMap<FLOATTYPE, NDIM>::~PsGridMakerMap() {
}

//
// Instantiate the maker maps
//
template class PsGridMakerMap<float, 1>;
template class PsGridMakerMap<float, 2>;
template class PsGridMakerMap<float, 3>;

template class PsGridMakerMap<double, 1>;
template class PsGridMakerMap<double, 2>;
template class PsGridMakerMap<double, 3>;

