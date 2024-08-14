/**
 *
 * @file    PsSTFuncMakerMap.cpp
 *
 * @brief   Construct the makers for all stfunc objects
 *
 * @version $Id: PsSTFuncMakerMap.cpp 2137 2020-04-16 20:37:29Z alexanda $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

// psstd includes
#include <PsSTFuncMakerMap.h>
#include <PsExpression.h>
#include <PsCutExpression.h>
#include <PsChiCutExpression.h>
#include <PsMovTanhSlabExpression.h>
#include <PsSwitchMovTanhSlab.h>
#include <PsSTPyFunc.h>

// txbase includes
#include <TxMakerMap.h>

//
// Constructor for maker map
//
template <class FLOATTYPE, size_t NDIM>
PsSTFuncMakerMap<FLOATTYPE, NDIM>::PsSTFuncMakerMap() {

// Make sure map exists
  TxMakerMap<PsSTFunc<FLOATTYPE, NDIM>>::getInstance();

// Add the makers (they are deleted by the makermap)
  new TxMaker< PsExpression<FLOATTYPE, NDIM>,
        PsSTFunc<FLOATTYPE, NDIM> >("expression");

  new TxMaker< PsCutExpression<FLOATTYPE, NDIM>,
        PsSTFunc<FLOATTYPE, NDIM> >("cutExpression");

  new TxMaker< PsChiCutExpression<FLOATTYPE, NDIM>,
        PsSTFunc<FLOATTYPE, NDIM> >("chiCutExpression");

  new TxMaker< PsMovTanhSlabExpression<FLOATTYPE, NDIM>,
        PsSTFunc<FLOATTYPE, NDIM> >("movTanhSlab");

  new TxMaker< PsSwitchMovTanhSlab<FLOATTYPE, NDIM>,
        PsSTFunc<FLOATTYPE, NDIM> >("switchMovTanhSlab");

  new TxMaker< PsSTPyFunc<FLOATTYPE, NDIM>,
        PsSTFunc<FLOATTYPE, NDIM> >("pyfunc");

}

template <class FLOATTYPE, size_t NDIM>
PsSTFuncMakerMap<FLOATTYPE, NDIM>::~PsSTFuncMakerMap() {
}

//
// Instantiate the maker maps
//
template class PsSTFuncMakerMap<float, 1>;
template class PsSTFuncMakerMap<float, 2>;
template class PsSTFuncMakerMap<float, 3>;

template class PsSTFuncMakerMap<double, 1>;
template class PsSTFuncMakerMap<double, 2>;
template class PsSTFuncMakerMap<double, 3>;


