/**
 *
 * @file    PsSTFunc.cpp
 *
 * @brief   Implementation for regions of space time: argument returns
 *              true for inside, false outside
 *
 * @version $Id: PsSTFunc.cpp 6319 2006-11-14 22:39:46Z sizemore $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

// txbase includes
#include <TxDebugExcept.h>

// psstd includes
#include <PsSTFunc.h>



// Constructor
template <class FLOATTYPE, size_t NDIM>
PsSTFunc<FLOATTYPE, NDIM>::PsSTFunc() {
  diagnoseLevel = 0;
  rank = 0;
  commSize = 1;
}


// Destructor
template <class FLOATTYPE, size_t NDIM>
PsSTFunc<FLOATTYPE, NDIM>::~PsSTFunc() { }

// Instantiations
template class PsSTFunc<float, 1>;
template class PsSTFunc<float, 2>;
template class PsSTFunc<float, 3>;

template class PsSTFunc<double, 1>;
template class PsSTFunc<double, 2>;
template class PsSTFunc<double, 3>;
