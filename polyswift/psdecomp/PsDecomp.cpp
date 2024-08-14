/**
 *
 * @file    PsDecomp.cpp
 *
 * @brief   Base interface for decomposition
 *
 * @version $Id: PsDecomp.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psdecomp includes
#include <PsDecomp.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsDecomp<FLOATTYPE, NDIM>::PsDecomp() {
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsDecomp<FLOATTYPE, NDIM>::~PsDecomp() {
}

template <class FLOATTYPE, size_t NDIM>
void PsDecomp<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

// Scoping call to base class
  PsDecompBase<FLOATTYPE, NDIM>::setAttrib(tas);
}

template <class FLOATTYPE, size_t NDIM>
void PsDecomp<FLOATTYPE, NDIM>::build() {
}

// Instantiate
template class PsDecomp<float, 1>;
template class PsDecomp<float, 2>;
template class PsDecomp<float, 3>;

template class PsDecomp<double, 1>;
template class PsDecomp<double, 2>;
template class PsDecomp<double, 3>;

