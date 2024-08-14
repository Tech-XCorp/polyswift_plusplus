/**
 * @file    PsBndryMakerMap.cpp
 *
 * @brief   Construct the makers for all polymer objects
 *
 * @version $Id: PsBndryMakerMap.cpp 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psbndry includes
#include <PsBndryMakerMap.h>
#include <PsFixedWall.h>
#include <PsInteractingSphere.h>
#include <PsProbeSphere.h>

// txbase includes
#include <TxMakerMap.h>

//
// Constructor for maker map
//
template <class FLOATTYPE, size_t NDIM>
PsBndryMakerMap<FLOATTYPE, NDIM>::PsBndryMakerMap() {

// Make sure map exists
  TxMakerMap<PsBndryBase<FLOATTYPE, NDIM>>::getInstance();

// Add the makers (they are deleted by the makermap)
  new TxMaker< PsFixedWall<FLOATTYPE, NDIM>,
        PsBndryBase<FLOATTYPE, NDIM> >("fixedWall");

  new TxMaker< PsInteractingSphere<FLOATTYPE, NDIM>,
        PsBndryBase<FLOATTYPE, NDIM> >("interactingSphere");

  new TxMaker< PsInteractingSphere<FLOATTYPE, NDIM>,
        PsBndryBase<FLOATTYPE, NDIM> >("probeSphere");
}

template <class FLOATTYPE, size_t NDIM>
PsBndryMakerMap<FLOATTYPE, NDIM>::~PsBndryMakerMap() {
}

//
// Instantiate the maker maps
//
template class PsBndryMakerMap<float, 1>;
template class PsBndryMakerMap<float, 2>;
template class PsBndryMakerMap<float, 3>;

template class PsBndryMakerMap<double, 1>;
template class PsBndryMakerMap<double, 2>;
template class PsBndryMakerMap<double, 3>;

