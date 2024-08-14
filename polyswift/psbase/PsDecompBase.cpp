/**
 *
 * @file    PsDecompBase.cpp
 *
 * @brief   Base interface for the decomposition
 *
 * @version $Id: PsDecompBase.cpp 8257 2007-09-12 22:05:20Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsDecompBase.h>

// txbase includes
#include <TxTensor.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsDecompBase<FLOATTYPE, NDIM>::PsDecompBase() {
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsDecompBase<FLOATTYPE, NDIM>::~PsDecompBase() {
}

template <class FLOATTYPE, size_t NDIM>
void PsDecompBase<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Using base class to set attributes for base polymer class
  PsDynObjBase::setAttrib(tas);

  // Debug flag
  if (tas.hasString("printdebug")) {
    std::string dbPrint = tas.getString("printdebug");
    if (dbPrint == "on") this->dbStatus = PSDB_ON;
  }

}

// Instantiation
template class PsDecompBase<float, 1>;
template class PsDecompBase<float, 2>;
template class PsDecompBase<float, 3>;

template class PsDecompBase<double, 1>;
template class PsDecompBase<double, 2>;
template class PsDecompBase<double, 3>;
