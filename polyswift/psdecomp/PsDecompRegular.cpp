/**
 *
 * @file    PsDecompRegular.cpp
 *
 * @brief   Class containing info for a regular decomposition
 *
 * @version $Id: PsDecompRegular.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psgrid includes
#include <PsDecompRegular.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsDecompRegular<FLOATTYPE, NDIM>::PsDecompRegular() {
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsDecompRegular<FLOATTYPE, NDIM>::~PsDecompRegular() {
}

template <class FLOATTYPE, size_t NDIM>
void PsDecompRegular<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

// Scoping call to base class
  PsDecomp<FLOATTYPE, NDIM>::setAttrib(tas);
}

template <class FLOATTYPE, size_t NDIM>
void PsDecompRegular<FLOATTYPE, NDIM>::buildData() {

// Scoping call to base class
  PsDecomp<FLOATTYPE, NDIM>::buildData();
}

template <class FLOATTYPE, size_t NDIM>
void PsDecompRegular<FLOATTYPE, NDIM>::buildSolvers() {

// Scoping call to base class
  PsDecomp<FLOATTYPE, NDIM>::buildSolvers();
}

//
// overloaded for vector of int's
//
template <class FLOATTYPE, size_t NDIM>
bool PsDecompRegular<FLOATTYPE, NDIM>::hasPosition(
       std::vector<int> globalPos) {

  size_t localxStart = localToGlobalShifts[0];
  size_t localxEnd = localxStart + this->numCellsLocal[0];

  if ( ((int)localxStart <= globalPos[0]) && (globalPos[0] < (int)localxEnd) ) {
    return 1;
  }
  else {
    return 0;
  }
}

//
// overloaded for tiny vectors
//
template <class FLOATTYPE, size_t NDIM>
bool PsDecompRegular<FLOATTYPE, NDIM>::hasPosition(
       PsTinyVector<int, NDIM> globalPos) {

  size_t localxStart = localToGlobalShifts[0];
  size_t localxEnd = localxStart + this->numCellsLocal[0];

  if ( ( (int)localxStart <= globalPos[0]) &&
       ( globalPos[0] < (int)localxEnd) ) {
    return 1;
  }
  else {
    return 0;
  }

}

// Instantiate
template class PsDecompRegular<float, 1>;
template class PsDecompRegular<float, 2>;
template class PsDecompRegular<float, 3>;

template class PsDecompRegular<double, 1>;
template class PsDecompRegular<double, 2>;
template class PsDecompRegular<double, 3>;

