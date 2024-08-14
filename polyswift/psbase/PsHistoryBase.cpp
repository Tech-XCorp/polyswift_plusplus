/**
 *
 * @file    PsHistoryBase.cpp
 *
 * @brief   Base class for a diagnostic
 *
 * @version $Id: PsHistoryBase.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// vpbase includes
#include <PsHistoryBase.h>

/** Constructor */
template <class FLOATTYPE, size_t NDIM>
PsHistoryBase<FLOATTYPE, NDIM>::PsHistoryBase() {

  updatePeriod = 1;
  tstepsBeforeStart = 0;
  tstepsBeforeFinish = 0;
  t0 = -1;
  dt = -1;
}

template <class FLOATTYPE, size_t NDIM>
void PsHistoryBase<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Base class
  PsDynObj<FLOATTYPE, NDIM>::setAttrib(tas);

  // Number of timesteps between updates
  if (tas.hasOption("updatePeriodicity")) {
    updatePeriod = tas.getOption("updatePeriodicity");
  }

  // Number of timesteps before any update
  if (tas.hasOption("tstepsBeforeStart")) {
    tstepsBeforeStart = tas.getOption("tstepsBeforeStart");
  }

  // Number of timesteps after which to stop updating
  if (tas.hasOption("tstepsBeforeFinish")) {
    tstepsBeforeFinish = tas.getOption("tstepsBeforeFinish");
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsHistoryBase<FLOATTYPE, NDIM>::buildData() {}

template <class FLOATTYPE, size_t NDIM>
void PsHistoryBase<FLOATTYPE, NDIM>::buildSolvers() {}

template <class FLOATTYPE, size_t NDIM>
void PsHistoryBase<FLOATTYPE, NDIM>::initialize() {}

template <class FLOATTYPE, size_t NDIM>
void PsHistoryBase<FLOATTYPE, NDIM>::setHistAttributes() {

  // Sets "global" attributes to be included in all datasets
  // Old way of setting time info... will erase
  // histAttributes["time"] = this->t0;
  // Points to "global" time attributes
  // this->histAttrString["timeSequence"] = "times";
}

template <class FLOATTYPE, size_t NDIM>
void PsHistoryBase<FLOATTYPE, NDIM>::update(double t) {

  // Scoping call to update(t) so as not to bypass
  PsDynObj<FLOATTYPE, NDIM>::update(t);

  // Set times
  t0 = t;
  currentHistTime = t;
}

template <class FLOATTYPE, size_t NDIM>
void PsHistoryBase<FLOATTYPE, NDIM>::appendHistAttribs(TxIoBase* txIoPtr,
    TxIoNodeType dn, bool barrierFlag) {

  std::string attribName;

  typename std::map<std::string, std::string>::iterator iattrStr;
  std::string attribStr;
  typename std::map<std::string, FLOATTYPE>::iterator iattrFloat;
  FLOATTYPE attribFloat;
  typename std::map<std::string, std::vector<FLOATTYPE> >::iterator iattrFloatVec;
  std::vector<FLOATTYPE> attribFloatVec;
  typename std::map<std::string, std::vector<int> >::iterator iattrIntVec;
  std::vector<int> attribIntVec;

  // Write out string attributes
  for (iattrStr  = histAttrString.begin();
       iattrStr != histAttrString.end(); ++iattrStr) {
    attribName = iattrStr->first;
    attribStr = iattrStr->second;
    txIoPtr->writeAttribute(dn, attribName, attribStr, barrierFlag);
  }

  // Write out float attributes
  for (iattrFloat  = histAttributes.begin();
       iattrFloat != histAttributes.end(); ++iattrFloat) {

    attribName = iattrFloat->first;
    attribFloat = iattrFloat->second;
    txIoPtr->writeAttribute(dn, attribName, attribFloat, barrierFlag);
  }

  // Write out float vector attributes
  for (iattrFloatVec  = histAttrVec.begin();
       iattrFloatVec != histAttrVec.end(); ++iattrFloatVec) {
    attribName = iattrFloatVec->first;
    attribFloatVec = iattrFloatVec->second;
    txIoPtr->writeAttribute(dn, attribName, attribFloatVec, barrierFlag);
  }

  // Write out integer vector attributes
  for (iattrIntVec  = histAttrIntVec.begin();
       iattrIntVec != histAttrIntVec.end(); ++iattrIntVec) {
    attribName = iattrIntVec->first;
    attribIntVec = iattrIntVec->second;
    txIoPtr->writeAttribute(dn, attribName, attribIntVec, barrierFlag);
  }
}

// Instantiate base history classes
template class PsHistoryBase<float, 1>;
template class PsHistoryBase<float, 2>;
template class PsHistoryBase<float, 3>;

template class PsHistoryBase<double, 1>;
template class PsHistoryBase<double, 2>;
template class PsHistoryBase<double, 3>;

