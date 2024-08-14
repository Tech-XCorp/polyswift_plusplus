/**
 * @file    PsFloryConstChi.cpp
 *
 * @brief   Tracks time-varying constant chi
 *
 * @version $Id: PsFloryConstChi.cpp 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// pshist includes
#include <PsFloryConstChi.h>

template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
void PsFloryConstChi<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Using base class to set attributes for a particular diagnostic
  PsHistoryBase<FLOATTYPE, NDIM>::setAttrib(tas);

  // Parse name and get field
  if (tas.hasString("interactionName")) {
    interactionName = tas.getString("interactionName");
    this->dbprt("History found interaction object ", interactionName);

  }
  else {
    TxDebugExcept tde("PsFloryConstChi::setAttrib: no interaction object name set");
    tde << " in <History " << this->getName() << " >";
    throw tde;
  }

}

template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
void PsFloryConstChi<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::buildData() {

  // Scoping call to buildData
  PsHistory<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::buildData();
}

template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
void PsFloryConstChi<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::buildSolvers() {

  // Scoping call to buildSolvers
  PsHistory<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::buildSolvers();

  // Get Hamiltonian method pointer
  interactPtr =
    PsNamedObject::getObject<PsInteraction<FLOATTYPE, NDIM> >(interactionName);
  if (!interactPtr) {
    TxDebugExcept tde("PsHistory::buildSolvers: interaction pointer not set");
    tde << " in <PsHistory " << this->getName() << " >";
    throw tde;
  }
}

//
//
template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
void PsFloryConstChi<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::update(double t) {

  // Scoping call to update(t) so as not to bypass
  PsHistoryBase<FLOATTYPE, NDIM>::update(t);

  // Append chi values to history data
  FLOATTYPE constChi = interactPtr->getParamUniform();

  // Scale for averaging in PsCommHistory
  size_t nprocs = this->getCommBase().getSize();
  constChi = constChi/((FLOATTYPE) nprocs);
  this->appendData(constChi);
}

//
//
template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
void PsFloryConstChi<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::setHistAttributes() {

  // Scoping call to setHistAttributes() so as not to bypass
  PsHistoryBase<FLOATTYPE, NDIM>::setHistAttributes();
}

// Instantiate history particles classes
template class PsFloryConstChi<float, 1, float, float>;
template class PsFloryConstChi<float, 2, float, float>;
template class PsFloryConstChi<float, 3, float, float>;

template class PsFloryConstChi<double, 1, double, double>;
template class PsFloryConstChi<double, 2, double, double>;
template class PsFloryConstChi<double, 3, double, double>;
