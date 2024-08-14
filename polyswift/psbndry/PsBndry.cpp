/**
 * @file    PsBndry.cpp
 *
 * @brief   Base interface for boundaries (walls)
 *
 * @version $Id: PsBndry.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psbndry includes
#include <PsBndry.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsBndry<FLOATTYPE, NDIM>::PsBndry() {
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsBndry<FLOATTYPE, NDIM>::~PsBndry() {
}

template <class FLOATTYPE, size_t NDIM>
void PsBndry<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

// Scoping call to base class
  PsBndryBase<FLOATTYPE, NDIM>::setAttrib(tas);
}

template <class FLOATTYPE, size_t NDIM>
void PsBndry<FLOATTYPE, NDIM>::buildData() {

// Scoping call to base class
  PsBndryBase<FLOATTYPE, NDIM>::buildData();
}

template <class FLOATTYPE, size_t NDIM>
void PsBndry<FLOATTYPE, NDIM>::buildSolvers() {

// Scoping call to base class
  PsBndryBase<FLOATTYPE, NDIM>::buildSolvers();
}

template <class FLOATTYPE, size_t NDIM>
void PsBndry<FLOATTYPE, NDIM>::initialize() {
}

template <class FLOATTYPE, size_t NDIM>
void PsBndry<FLOATTYPE, NDIM>::update(double t) {
}

// Instantiate
template class PsBndry<float, 1>;
template class PsBndry<float, 2>;
template class PsBndry<float, 3>;

template class PsBndry<double, 1>;
template class PsBndry<double, 2>;
template class PsBndry<double, 3>;

