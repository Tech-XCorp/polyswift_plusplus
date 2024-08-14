/**
 *
 * @file    PsCanonicalMF.cpp
 *
 * @brief   Specifics for canonical mean-field model
 *
 * @version $Id: PsCanonicalMF.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// pseffhamil includes
#include <PsCanonicalMF.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsCanonicalMF<FLOATTYPE, NDIM>::PsCanonicalMF() {
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsCanonicalMF<FLOATTYPE, NDIM>::~PsCanonicalMF() {
}

template <class FLOATTYPE, size_t NDIM>
void PsCanonicalMF<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

// Scoping call to base class
  PsEffHamil<FLOATTYPE, NDIM>::setAttrib(tas);
}

template <class FLOATTYPE, size_t NDIM>
void PsCanonicalMF<FLOATTYPE, NDIM>::buildData() {

// Scoping call to base class
  PsEffHamil<FLOATTYPE, NDIM>::buildData();
}

template <class FLOATTYPE, size_t NDIM>
void PsCanonicalMF<FLOATTYPE, NDIM>::buildSolvers() {

// Scoping call to base class
  PsEffHamil<FLOATTYPE, NDIM>::buildSolvers();
}

template <class FLOATTYPE, size_t NDIM>
void PsCanonicalMF<FLOATTYPE, NDIM>::update(double t) {

// Scoping call to base class
  PsEffHamil<FLOATTYPE, NDIM>::update(t);
}

//
// Instantiate classes
//
template class PsCanonicalMF<float, 1>;
template class PsCanonicalMF<float, 2>;
template class PsCanonicalMF<float, 3>;

template class PsCanonicalMF<double, 1>;
template class PsCanonicalMF<double, 2>;
template class PsCanonicalMF<double, 3>;

