/**
 * @file    PsBndryDataBaseMakerMap.cpp
 *
 * @brief   Construct the makers for all polymer objects
 *
 * @version $Id: PsBndryDataBaseMakerMap.cpp 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psbndry includes
#include <PsBndryDataBaseMakerMap.h>
#include <PsSphereData.h>
#include <PsWallData.h>

// txbase includes
#include <TxMakerMap.h>

//
// Constructor for maker map
//
template <class FLOATTYPE, size_t NDIM>
PsBndryDataBaseMakerMap<FLOATTYPE, NDIM>::PsBndryDataBaseMakerMap() {

// Make sure map exists
  TxMakerMap<PsBndryDataBase<FLOATTYPE, NDIM>>::getInstance();

// Add the makers (they are deleted by the makermap)
  new TxMaker< PsSphereData<FLOATTYPE, NDIM>,
         PsBndryDataBase<FLOATTYPE, NDIM> >("sphereData");

  new TxMaker< PsWallData<FLOATTYPE, NDIM>,
          PsBndryDataBase<FLOATTYPE, NDIM> >("wallData");
}

template <class FLOATTYPE, size_t NDIM>
PsBndryDataBaseMakerMap<FLOATTYPE, NDIM>::~PsBndryDataBaseMakerMap() {
}

//
// Instantiate the maker maps
//
template class PsBndryDataBaseMakerMap<float, 1>;
template class PsBndryDataBaseMakerMap<float, 2>;
template class PsBndryDataBaseMakerMap<float, 3>;

template class PsBndryDataBaseMakerMap<double, 1>;
template class PsBndryDataBaseMakerMap<double, 2>;
template class PsBndryDataBaseMakerMap<double, 3>;

