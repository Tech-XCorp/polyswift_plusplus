/**
 *
 * @file    PsEffHamilBase.cpp
 *
 * @brief   Base/interface for specifying energetics of field-theoretic model
 *
 * @version $Id: PsEffHamilBase.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// txbase includes
#include <TxNotImplExcept.h>

// psbase includes
#include <PsEffHamilBase.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsEffHamilBase<FLOATTYPE, NDIM>::PsEffHamilBase() {
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsEffHamilBase<FLOATTYPE, NDIM>::~PsEffHamilBase() {
}

template <class FLOATTYPE, size_t NDIM>
void PsEffHamilBase<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Scoping call to base class
  PsDynObjBase::setAttrib(tas);

  this->dbprt("PsEffHamilBase::setAttrib(): entered ");

  // Debug flag
  if (tas.hasString("printdebug")) {
    std::string dbPrint = tas.getString("printdebug");
    if (dbPrint == "on") this->dbStatus = PSDB_ON;
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsEffHamilBase<FLOATTYPE, NDIM>::buildData() {}

template <class FLOATTYPE, size_t NDIM>
void PsEffHamilBase<FLOATTYPE, NDIM>::buildSolvers() {}

// Instantiate
template class PsEffHamilBase<float, 1>;
template class PsEffHamilBase<float, 2>;
template class PsEffHamilBase<float, 3>;

template class PsEffHamilBase<double, 1>;
template class PsEffHamilBase<double, 2>;
template class PsEffHamilBase<double, 3>;

