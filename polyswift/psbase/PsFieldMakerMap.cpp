/**
 *
 * @file    PsFieldMakerMap.cpp
 *
 * @brief   Construct the makers for all polymer objects
 *
 * @version $Id: PsFieldMakerMap.cpp 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 *
 */

// psbase includes
#include <PsFieldMakerMap.h>
#include <PsFieldBase.h>
#include <PsGridField.h>

// txbase includes
#include <TxMakerMap.h>

//
// Constructor for maker map
//
template <class FLOATTYPE, size_t NDIM>
PsFieldMakerMap<FLOATTYPE, NDIM>::PsFieldMakerMap() {

// Make sure map exists
  TxMakerMap<PsFieldBase<float>>::getInstance();
  TxMakerMap<PsFieldBase<double>>::getInstance();

// Add the makers (they are deleted by the makermap)
  new TxMaker<PsGridField<float,1>, PsFieldBase<float> >("fieldF1R");
  new TxMaker< PsGridField<float,2>, PsFieldBase<float> >("fieldF2R");
  new TxMaker< PsGridField<float,3>, PsFieldBase<float> >("fieldF3R");
  new TxMaker< PsGridField<float,4>, PsFieldBase<float> >("fieldF4R");
  new TxMaker< PsGridField<float,5>, PsFieldBase<float> >("fieldF5R");
  new TxMaker< PsGridField<float,6>, PsFieldBase<float> >("fieldF6R");
  new TxMaker< PsGridField<double,1>, PsFieldBase<double> >("fieldD1R");
  new TxMaker< PsGridField<double,2>, PsFieldBase<double> >("fieldD2R");
  new TxMaker< PsGridField<double,3>, PsFieldBase<double> >("fieldD3R");
  new TxMaker< PsGridField<double,4>, PsFieldBase<double> >("fieldD4R");
  new TxMaker< PsGridField<double,5>, PsFieldBase<double> >("fieldD5R");
  new TxMaker< PsGridField<double,6>, PsFieldBase<double> >("fieldD6R");

}

template <class FLOATTYPE, size_t NDIM>
PsFieldMakerMap<FLOATTYPE, NDIM>::~PsFieldMakerMap() {
}

//
// Instantiate the maker maps
//
template class PsFieldMakerMap<float, 1>;
template class PsFieldMakerMap<float, 2>;
template class PsFieldMakerMap<float, 3>;

template class PsFieldMakerMap<double, 1>;
template class PsFieldMakerMap<double, 2>;
template class PsFieldMakerMap<double, 3>;

