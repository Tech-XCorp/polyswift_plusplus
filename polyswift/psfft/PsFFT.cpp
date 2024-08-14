/**
 *
 * @file    PsFFT.cpp
 *
 * @brief   Instantiate the class, PsFFT.
 *
 * @version $Id: PsFFT.cpp 6319 2006-11-14 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#include <PsFFT.h>

template <class FLOATTYPE, size_t NDIM>
PsFFT<FLOATTYPE, NDIM>::PsFFT() { }

template <class FLOATTYPE, size_t NDIM>
PsFFT<FLOATTYPE, NDIM>::~PsFFT() { }

template <class FLOATTYPE, size_t NDIM>
void PsFFT<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Scoping call to base class
  PsFFTBase<FLOATTYPE, NDIM>::setAttrib(tas);
  this->dbprt("PsFFT::setAttrib() ");

  // Effecitvely the decomp type
  if (tas.hasString("gridKind")) {
    gridKind = tas.getString("gridKind");
  }
  else {
    TxDebugExcept tde("PsFFT::setAttrib: gridKind not set");
    tde << " in <FFT " << this->getName() << " >";
    throw tde;
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsFFT<FLOATTYPE, NDIM>::buildData() {

  this->dbprt("PsFFT::buildData() ");

  // Set global grid sizes
  gridPtr = PsNamedObject::getObject<PsGridBase<FLOATTYPE, NDIM> >(gridKind);
  globalDims = gridPtr->getNumCellsGlobal();
}

template <class FLOATTYPE, size_t NDIM>
void PsFFT<FLOATTYPE, NDIM>::buildSolvers() {}

template class PsFFT<float, 1>;
template class PsFFT<float, 2>;
template class PsFFT<float, 3>;

template class PsFFT<double, 1>;
template class PsFFT<double, 2>;
template class PsFFT<double, 3>;
