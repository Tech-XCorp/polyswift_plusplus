/**
 *
 * @file    PsPhysFieldMakerMap.cpp
 *
 * @brief   Construct the makers for all em fields.
 *
 * @version $Id: PsPhysFieldMakerMap.cpp 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psphysf includes
#include <PsPhysFieldMakerMap.h>
#include <PsMonomerDens.h>
#include <PsChargeDens.h>
#include <PsConstraint.h>

// txbase includes
#include <TxMakerMap.h>

//
// Constructor for maker map
//
template <class FLOATTYPE, size_t NDIM>
PsPhysFieldMakerMap<FLOATTYPE, NDIM>::PsPhysFieldMakerMap() {

// Make sure map exists
  TxMakerMap<PsPhysField<FLOATTYPE, NDIM>>::getInstance();

// Add the makers (they are deleted by the makermap)

// Fields that exist in all cases
  new TxMaker< PsMonomerDens<FLOATTYPE, NDIM>,
        PsPhysField<FLOATTYPE, NDIM> >("monomerDens");

  new TxMaker< PsChargeDens<FLOATTYPE, NDIM>,
        PsPhysField<FLOATTYPE, NDIM> >("chargeDens");

  new TxMaker< PsConstraint<FLOATTYPE, NDIM>,
        PsPhysField<FLOATTYPE, NDIM> >("constraint");
}

template <class FLOATTYPE, size_t NDIM>
PsPhysFieldMakerMap<FLOATTYPE, NDIM>::~PsPhysFieldMakerMap() {
}

//
// Instantiate the maker maps
//
template class PsPhysFieldMakerMap<float, 1>;
template class PsPhysFieldMakerMap<float, 2>;
template class PsPhysFieldMakerMap<float, 3>;

template class PsPhysFieldMakerMap<double, 1>;
template class PsPhysFieldMakerMap<double, 2>;
template class PsPhysFieldMakerMap<double, 3>;

