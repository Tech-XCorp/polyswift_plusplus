/**
 *
 * @file    PsDomainMakerMap.cpp
 *
 * @brief   Construct the makers for all domains.
 *
 * @version $Id: PsDomainMakerMap.cpp 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2020-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// vpem includes
#include <PsDomainMakerMap.h>

// The possible domains
#include <PsDomain.h>

PsDomainMakerMap::PsDomainMakerMap() {

// Make sure map exists
  TxMakerMap<PsDomainBase>::getInstance();

// Add the makers (they are deleted by the makermap)
  new TxMaker<PsDomain<float, 1>, PsDomainBase>("Domain1F");
  new TxMaker<PsDomain<float, 2>, PsDomainBase>("Domain2F");
  new TxMaker<PsDomain<float, 3>, PsDomainBase>("Domain3F");

  new TxMaker<PsDomain<double, 1>, PsDomainBase>("Domain1D");
  new TxMaker<PsDomain<double, 2>, PsDomainBase>("Domain2D");
  new TxMaker<PsDomain<double, 3>, PsDomainBase>("Domain3D");
}

PsDomainMakerMap::~PsDomainMakerMap() {
}

// Instantiate the maker map
template class TxMakerMap<PsDomainBase>;

