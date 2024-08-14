/**
 * @file    PsHistoryMakerMap.cpp
 *
 * @brief   Construct the makers for all history objects
 *
 * @version $Id: PsHistoryMakerMap.cpp 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// pshist includes
#include <PsHistoryMakerMap.h>
#include <PsFreeEnergy.h>
#include <PsFloryConstChi.h>
#include <PsFloryChiAtPoint.h>

// txbase includes
#include <TxTensor.h>
#include <TxMakerMap.h>

//
// Constructor for maker map
//
template <class FLOATTYPE, size_t NDIM>
PsHistoryMakerMap<FLOATTYPE, NDIM>::PsHistoryMakerMap() {

// Make sure map exists
  TxMakerMap<PsHistoryBase<FLOATTYPE, NDIM>>::getInstance();

// Add the makers (they are deleted by the makermap)
  new TxMaker< PsFreeEnergy<FLOATTYPE, NDIM, FLOATTYPE, FLOATTYPE>,
       PsHistoryBase<FLOATTYPE, NDIM> >("freeEnergy");

  new TxMaker< PsFloryConstChi<FLOATTYPE, NDIM, FLOATTYPE, FLOATTYPE>,
       PsHistoryBase<FLOATTYPE, NDIM> >("floryConstChi");

  new TxMaker< PsFloryChiAtPoint<FLOATTYPE, NDIM, FLOATTYPE, TxTensor<FLOATTYPE, 1> >,
       PsHistoryBase<FLOATTYPE, NDIM> >("floryChiAtPoint");
}

//
// Remove any makers created in the constructor.
//
template <class FLOATTYPE, size_t NDIM>
PsHistoryMakerMap<FLOATTYPE, NDIM>::~PsHistoryMakerMap() {
}

//
// Instantiate the maker maps
//
template class PsHistoryMakerMap<float, 1>;
template class PsHistoryMakerMap<float, 2>;
template class PsHistoryMakerMap<float, 3>;

template class PsHistoryMakerMap<double, 1>;
template class PsHistoryMakerMap<double, 2>;
template class PsHistoryMakerMap<double, 3>;

