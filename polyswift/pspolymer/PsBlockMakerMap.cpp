/**
 * @file    PsBlockMakerMap.cpp
 *
 * @brief   Construct the makers for all polymer objects
 *
 * @version $Id: PsBlockMakerMap.cpp 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// pspolymer includes
#include <PsBlockMakerMap.h>
#include <PsBlockTypes.h>
#include <PsFlexPseudoSpec.h>
#include <PsChargeFlexPseudoSpec.h>

//#include <PsFlexCrankNic.h>
#include <PsSemiFlexibleBlock.h>

// txbase includes
#include <TxMakerMap.h>

//
// Constructor for maker map
//
template <class FLOATTYPE, size_t NDIM>
PsBlockMakerMap<FLOATTYPE, NDIM>::PsBlockMakerMap() {

// Make sure map exists
  TxMakerMap<PsBlockBase<FLOATTYPE, NDIM>>::getInstance();

// Add the makers (they are deleted by the makermap)
  new TxMaker< PsFlexPseudoSpec<FLOATTYPE, NDIM, typename PsBlockTypes<FLOATTYPE,
     NDIM>::flexQType >, PsBlockBase<FLOATTYPE, NDIM> >("flexPseudoSpec");

  new TxMaker< PsChargeFlexPseudoSpec<FLOATTYPE, NDIM, typename PsBlockTypes<FLOATTYPE,
     NDIM>::flexQType >, PsBlockBase<FLOATTYPE, NDIM> >("chargeFlexPseudoSpec");

  new TxMaker< PsSemiFlexibleBlock<FLOATTYPE, NDIM, typename PsBlockTypes<FLOATTYPE,
     NDIM>::semiflexQType >, PsBlockBase<FLOATTYPE, NDIM> >("semiflexibleBlock");

}

template <class FLOATTYPE, size_t NDIM>
PsBlockMakerMap<FLOATTYPE, NDIM>::~PsBlockMakerMap() {
}

//
// Instantiate the maker maps
//
template class PsBlockMakerMap<float, 1>;
template class PsBlockMakerMap<float, 2>;
template class PsBlockMakerMap<float, 3>;

template class PsBlockMakerMap<double, 1>;
template class PsBlockMakerMap<double, 2>;
template class PsBlockMakerMap<double, 3>;

