/**
 * @file    PsFreeEnergy.cpp
 *
 * @brief   Tracks total free-energy
 *
 * @version $Id: PsFreeEnergy.cpp 11989 2009-03-25 05:14:55Z swsides $
 *
 * Copyright &copy; 2009-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// pshist includes
#include <PsFreeEnergy.h>

template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
void
PsFreeEnergy<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::
setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Using base class to set attributes for a particular diagnostic
  PsHistoryBase<FLOATTYPE, NDIM>::setAttrib(tas);

  // Parse name and get field
  if (tas.hasString("updaterName")) {
    updaterName = tas.getString("updaterName");
    this->dbprt("History found updater object ", updaterName);
  }
  else {
    TxDebugExcept tde("PsFreeEnergy::setAttrib: no updater object name set");
    tde << " in <History " << this->getName() << " >";
    throw tde;
  }

  // Disorder calc flag
  if (tas.hasOption("calcDisorder")) {
    calcDisorder = tas.getOption("calcDisorder");
  }
  else {
    calcDisorder = true; // Default behavior
  }

}

template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
void
PsFreeEnergy<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::
buildData() {

  // Scoping call to buildData
  PsHistory<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::buildData();
}

template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
void
PsFreeEnergy<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::
buildSolvers() {

  // Scoping call to buildSolvers
  PsHistory<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::buildSolvers();

  // Get Hamiltonian method pointer
  updaterPtr =
    PsNamedObject::getObject<PsUpdater<FLOATTYPE, NDIM> >(updaterName);
  if (!updaterPtr) {
    TxDebugExcept tde("PsHistory::buildSolvers: updater pointer not set");
    tde << " in <PsHistory " << this->getName() << " >";
    throw tde;
  }

}

//
//
template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
void
PsFreeEnergy<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::
update(double t) {

  // Scoping call to update(t) so as not to bypass
  PsHistoryBase<FLOATTYPE, NDIM>::update(t);

  // Append free-energy values to history data
  FLOATTYPE freeE = updaterPtr->getFreeE(calcDisorder);

  // Scale for averaging in PsCommHistory
  size_t nprocs = this->getCommBase().getSize();
  freeE = freeE/((FLOATTYPE) nprocs);
  this->appendData(freeE);
}

//
//
template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
void
PsFreeEnergy<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::
setHistAttributes() {

  // Scoping call to setHistAttributes() so as not to bypass
  PsHistoryBase<FLOATTYPE, NDIM>::setHistAttributes();
}

// Instantiate history particles classes
template class PsFreeEnergy<float, 1, float, float>;
template class PsFreeEnergy<float, 2, float, float>;
template class PsFreeEnergy<float, 3, float, float>;

template class PsFreeEnergy<double, 1, double, double>;
template class PsFreeEnergy<double, 2, double, double>;
template class PsFreeEnergy<double, 3, double, double>;
