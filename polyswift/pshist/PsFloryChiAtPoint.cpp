/**
 * @file    PsFloryChiAtPoint.cpp
 *
 * @brief   Tracks time-varying constant chi
 *
 * @version $Id: PsFloryChiAtPoint.cpp 1351 2011-08-25 06:08:56Z swsides $
 *
 * Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// txbase includes
#include <TxTensor.h>

// std includes
#include <limits.h>

// psbase includes
//#include <PsGridField.h>
#include <PsGridField.h>

// pshist includes
#include <PsFloryChiAtPoint.h>

template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
void PsFloryChiAtPoint<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Using base class to set attributes for a particular diagnostic
  PsHistoryBase<FLOATTYPE, NDIM>::setAttrib(tas);

  // Parse name and get field
  if (tas.hasString("interactionName")) {
    interactionName = tas.getString("interactionName");
    this->dbprt("History found interaction object ", interactionName);
  }
  else {
    TxDebugExcept tde("PsFloryChiAtPoint::setAttrib: no interaction object name set");
    tde << " in <History " << this->getName() << " >";
    throw tde;
  }

  // Point for recording chi value
  if (tas.hasOptVec("point")) {
    std::vector<int> point;
    point = tas.getOptVec("point");
    for (size_t n=0; n<point.size(); ++n)
      chiGridPoint.push_back(static_cast<size_t>(point[n]));
  }
  else {
    TxDebugExcept tde("PsFloryChiAtPoint::setAttrib: point not set");
    tde << " in <History " << this->getName() << " >";
    throw tde;
  }

}

template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
void PsFloryChiAtPoint<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::buildData() {

  // Scoping call to buildData
  PsHistory<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::buildData();
}

template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
void PsFloryChiAtPoint<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::buildSolvers() {

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

  //
  // Setup/allocate diagnostic tensor data stuctures
  //
  numDumpComp = 3;
  histDataDims[0] = numDumpComp;
  histDatum.setLengths(histDataDims);
  histDatum = 0.0;

  // Setup workspace for gathering max/min values
  // and initialize
  size_t nprocs = this->getCommBase().getSize();
  for (size_t n=0; n<nprocs; ++n) {
    maxLocalVec.push_back(std::numeric_limits<FLOATTYPE>::min());
    minLocalVec.push_back(std::numeric_limits<FLOATTYPE>::max());
  }
}

//
//
template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
void PsFloryChiAtPoint<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::update(double t) {

  // Scoping call to update(t) so as not to bypass
  PsHistoryBase<FLOATTYPE, NDIM>::update(t);

  // This needs to be changed with the Chompst changes
  PsFieldBase<FLOATTYPE>* chiNPtr = &(interactPtr->getParam());
  //  PsGridField<FLOATTYPE, NDIM>* chiNGridPtr =
  //    dynamic_cast< PsGridField<FLOATTYPE, NDIM>* >(chiNPtr);
  //  PsGridField<FLOATTYPE, NDIM>& chiNGrid = *chiNGridPtr;
  PsGridField<FLOATTYPE, NDIM>* chiNGridPtr =
    dynamic_cast< PsGridField<FLOATTYPE, NDIM>* >(chiNPtr);
  PsGridField<FLOATTYPE, NDIM>& chiNGrid = *chiNGridPtr;

  FLOATTYPE maxChi = findMaxChi(chiNGrid);
  FLOATTYPE minChi = findMinChi(chiNGrid);
  FLOATTYPE chiVal = findChiAtPoint(chiNGrid);

  // Load (chi(x,y,z) min_chi, max_chi)
  // and send to history data in base
  size_t nprocs = this->getCommBase().getSize();
  histDatum((size_t)0) = chiVal;
  histDatum((size_t)1) = minChi/(FLOATTYPE)nprocs;
  histDatum((size_t)2) = maxChi/(FLOATTYPE)nprocs;
  this->appendData(histDatum);
}

//
//
template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
void PsFloryChiAtPoint<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::setHistAttributes() {

  // Scoping call to setHistAttributes() so as not to bypass
  PsHistoryBase<FLOATTYPE, NDIM>::setHistAttributes();

  //
  // Set specific attribute names and vector integer data
  //
  std::vector<int> attrIntVec;

  for (size_t idir=0; idir<3; ++idir) {
    attrIntVec.push_back((int)chiGridPoint[idir]);
  }
  this->histAttrIntVec["chiGridPoint"] = attrIntVec;
  attrIntVec.clear();
}

// Helper method to find maximum value of chi
template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
FLOATTYPE PsFloryChiAtPoint<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::findMaxChi(
   PsGridField<FLOATTYPE, NDIM>& fld) {

  // Find max chi value
  FLOATTYPE maxLocVal = fld.maxVal();

  // Load local max val vector
  size_t nrank = this->getCommBase().getRank();
  maxLocalVec[nrank] = maxLocVal;

  std::vector<FLOATTYPE> maxValAcrossProc =
    this->getCommBase().allReduceMax(maxLocalVec);

  FLOATTYPE maxChi =
    *max_element(maxValAcrossProc.begin(), maxValAcrossProc.end());

  return maxChi;
}

// Helper method to find minimum value of chi
template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
FLOATTYPE PsFloryChiAtPoint<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::findMinChi(
   PsGridField<FLOATTYPE, NDIM>& fld) {

  // Find min chi value
  FLOATTYPE minLocVal = fld.minVal();

  // Load local min val vector
  size_t nrank = this->getCommBase().getRank();
  minLocalVec[nrank] = minLocVal;

  std::vector<FLOATTYPE> minValAcrossProc =
    this->getCommBase().allReduceMin(minLocalVec);

  FLOATTYPE minChi =
    *min_element(minValAcrossProc.begin(), minValAcrossProc.end());

  return minChi;
}

// Helper method to find minimum value of chi
template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
FLOATTYPE PsFloryChiAtPoint<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::findChiAtPoint(
   PsGridField<FLOATTYPE, NDIM>& fld) {

  // This is hardwired
  size_t compIndex= 0;

  // Check if point is owned by domain
  size_t xg = chiGridPoint[0];
  size_t yg = chiGridPoint[1];
  size_t zg = chiGridPoint[2];
  PsTinyVector<int, NDIM> posIntVec((int)xg, (int)yg, (int)zg);
  bool ptLocal = this->getGridBase().getDecomp().hasPosition(posIntVec);

  // Find chi value at specified point across all procs
  FLOATTYPE chiVal;
  PsTinyVector<int, NDIM> locPosVec;

  if (ptLocal) {
    locPosVec = this->getGridBase().mapToLocalVec(posIntVec);
    size_t xloc = (size_t)locPosVec[0];
    size_t yloc = (size_t)locPosVec[1];
    size_t zloc = (size_t)locPosVec[2];
    chiVal = fld(xloc, yloc, zloc, compIndex);
  }
  else { // Sum in PsCommHistory expects zero from other procs
    chiVal = 0.0;
  }

  return chiVal;
}

// Instantiate history particles classes
template class PsFloryChiAtPoint<float, 1, float, TxTensor<float, 1> >;
template class PsFloryChiAtPoint<float, 2, float, TxTensor<float, 1> >;
template class PsFloryChiAtPoint<float, 3, float, TxTensor<float, 1> >;

template class PsFloryChiAtPoint<double, 1, double, TxTensor<double, 1> >;
template class PsFloryChiAtPoint<double, 2, double, TxTensor<double, 1> >;
template class PsFloryChiAtPoint<double, 3, double, TxTensor<double, 1> >;
