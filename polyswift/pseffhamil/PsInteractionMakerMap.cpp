/**
 *
 * @file    PsInteractionMakerMap.cpp
 *
 * @brief   Construct the makers for all interaction objects
 *
 * @version $Id: PsInteractionMakerMap.cpp 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// pseffhamil includes
#include <PsInteractionMakerMap.h>
#include <PsFloryInteraction.h>
#include <PsFloryWallInteraction.h>

// txbase includes
#include <TxMakerMap.h>

//
// Constructor for maker map
//
template <class FLOATTYPE, size_t NDIM>
PsInteractionMakerMap<FLOATTYPE, NDIM>::PsInteractionMakerMap() {

// Make sure map exists
  TxMakerMap<PsInteraction<FLOATTYPE, NDIM>>::getInstance();

// Add the makers (they are deleted by the makermap)
  new TxMaker< PsFloryInteraction<FLOATTYPE, NDIM>,
        PsInteraction<FLOATTYPE, NDIM> >("flory");

  new TxMaker< PsFloryWallInteraction<FLOATTYPE, NDIM>,
        PsInteraction<FLOATTYPE, NDIM> >("floryWall");
}

template <class FLOATTYPE, size_t NDIM>
PsInteractionMakerMap<FLOATTYPE, NDIM>::~PsInteractionMakerMap() {
}

//
// Instantiate the maker maps
//
template class PsInteractionMakerMap<float, 1>;
template class PsInteractionMakerMap<float, 2>;
template class PsInteractionMakerMap<float, 3>;

template class PsInteractionMakerMap<double, 1>;
template class PsInteractionMakerMap<double, 2>;
template class PsInteractionMakerMap<double, 3>;

