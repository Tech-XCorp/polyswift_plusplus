/**
 *
 * @file    PsFFTBase.cpp
 *
 * @brief   Instantiate the class, PsFFTBase.
 *
 * @version $Id: PsFFTBase.cpp 6319 2006-11-14 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsFFTBase.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsFFTBase<FLOATTYPE, NDIM>::PsFFTBase() {
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsFFTBase<FLOATTYPE, NDIM>::~PsFFTBase() {
}

template <class FLOATTYPE, size_t NDIM>
void PsFFTBase<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Using base class to set attributes for base polymer class
  PsDynObjBase::setAttrib(tas);

  // Debug flag
  if (tas.hasString("printdebug")) {
    std::string dbPrint = tas.getString("printdebug");
    if (dbPrint == "on") this->dbStatus = (DebugPrint)PSDB_ON;
  }

}

template class PsFFTBase<float, 1>;
template class PsFFTBase<float, 2>;
template class PsFFTBase<float, 3>;

template class PsFFTBase<double, 1>;
template class PsFFTBase<double, 2>;
template class PsFFTBase<double, 3>;
