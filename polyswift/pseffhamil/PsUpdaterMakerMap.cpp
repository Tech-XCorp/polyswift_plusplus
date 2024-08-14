/**
 *
 * @file    PsUpdaterMakerMap.cpp
 *
 * @brief   Construct the makers for all updater objects
 *
 * @version $Id: PsUpdaterMakerMap.cpp 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// pseffhamil includes
#include <PsUpdaterMakerMap.h>
#include <PsSteepDUpdater.h>
#include <PsPoissonUpdater.h>
#include <PsSimpleSpecFilter.h>
#include <PsMultiSpecFilter.h>

// txbase includes
#include <TxMakerMap.h>

//
// Constructor for maker map
//
template <class FLOATTYPE, size_t NDIM>
PsUpdaterMakerMap<FLOATTYPE, NDIM>::PsUpdaterMakerMap() {

// Make sure map exists
  TxMakerMap<PsUpdater<FLOATTYPE, NDIM>>::getInstance();

// Add the makers (they are deleted by the makermap)
  new TxMaker< PsSteepDUpdater<FLOATTYPE, NDIM>,
        PsUpdater<FLOATTYPE, NDIM> >("steepestDescent");

  new TxMaker< PsSimpleSpecFilter<FLOATTYPE, NDIM>,
        PsUpdater<FLOATTYPE, NDIM> >("simpleSpecFilter");

  new TxMaker< PsMultiSpecFilter<FLOATTYPE, NDIM>,
        PsUpdater<FLOATTYPE, NDIM> >("multiSpecFilter");

  new TxMaker< PsPoissonUpdater<FLOATTYPE, NDIM>,
        PsUpdater<FLOATTYPE, NDIM> >("poissonUpdater");
}

template <class FLOATTYPE, size_t NDIM>
PsUpdaterMakerMap<FLOATTYPE, NDIM>::~PsUpdaterMakerMap() {
}

//
// Instantiate the maker maps
//
template class PsUpdaterMakerMap<float, 1>;
template class PsUpdaterMakerMap<float, 2>;
template class PsUpdaterMakerMap<float, 3>;

template class PsUpdaterMakerMap<double, 1>;
template class PsUpdaterMakerMap<double, 2>;
template class PsUpdaterMakerMap<double, 3>;

