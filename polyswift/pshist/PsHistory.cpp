/**
 * @file    PsHistory.cpp
 *
 * @brief   Main base class for a diagnostics
 *
 * @version $Id: PsHistory.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// pshist
#include <PsHistory.h>
#include <PsSerialize.h>

//
//
template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
void PsHistory<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::buildData() {

  // Scoping call to buildSolvers
  PsHistoryBase<FLOATTYPE, NDIM>::buildData();

  // Parse name and get field
  if (this->getAttrib().hasString("field")) {
    fieldName = this->getAttrib().getString("field");
    this->dbprt("History found field name --> ", fieldName);
  }
}

//
//
template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
void PsHistory<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::buildSolvers() {}

//
// Format data for IO method write
// Write data to file
//
template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
void PsHistory<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::dump(TxIoBase* txIoPtr,
                                                             TxIoNodeType fnode) {

  std::string datasetName = this->getName();
  TxIoNodeType ds = txIoPtr->extendData(fnode, datasetName,
                                        this->serialPtr,
                                        this->serialShape);

  // Set attribute names/data in implementation classes
  this->setHistAttributes();

  // Write out specific attribute datamaps
  this->appendHistAttribs(txIoPtr, ds, false);

  // Append dataset attributes
  txIoPtr->writeAttribute(ds, "vsType","variable");
  txIoPtr->writeAttribute(ds, "vsMesh","timeSeries");

  // Explicitly closing dataset after attributes attached
  txIoPtr->closeDataSet(ds);

  // Access time-series group
  TxIoNodeType gn;
  std::string groupName = "timeSeries";
  if (txIoPtr->groupExists(fnode, groupName)) {
    gn = txIoPtr->openGroup(fnode, groupName);
  }
  else {
    gn = txIoPtr->createGroup(fnode, groupName);
  }

  // Write attributes to timeseries group
  std::vector<int> startCell(1);
  std::vector<int> numCells(1);
  std::vector<FLOATTYPE> lowerBounds(1);
  std::vector<FLOATTYPE> upperBounds(1);

  startCell[0] = 0;
  numCells[0] = (int)PsDynObjBase::getCurrDomainStep();
  lowerBounds[0] = 0.0;
  upperBounds[0] = (FLOATTYPE)numCells[0];
  txIoPtr->writeAttribute(gn, "vsType", "mesh");
  txIoPtr->writeAttribute(gn, "vsKind", "uniform");
  txIoPtr->writeAttribute(gn, "vsStartCell", startCell);
  txIoPtr->writeAttribute(gn, "vsNumCells" , numCells);
  txIoPtr->writeAttribute(gn, "vsLowerBounds", lowerBounds);
  txIoPtr->writeAttribute(gn, "vsUpperBounds", upperBounds);
}

//
//
template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
void PsHistory<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::appendData(DATATYPE d) {

  // Update time
  this->histTimes.push_back(this->currentHistTime);

  // Update data
  histData.push_back(d);
}

template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
void PsHistory<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::clearData() {
  histData.clear();
}

// Instantiate history classes scalar
// Scalar DATATYPE represented by int
template class PsHistory<float, 1, int, int>;
template class PsHistory<float, 2, int, int>;
template class PsHistory<float, 3, int, int>;

template class PsHistory<double, 1, int, int>;
template class PsHistory<double, 2, int, int>;
template class PsHistory<double, 3, int, int>;

template class PsHistory<float, 1, float, float>;
template class PsHistory<float, 2, float, float>;
template class PsHistory<float, 3, float, float>;

template class PsHistory<double, 1, double, double>;
template class PsHistory<double, 2, double, double>;
template class PsHistory<double, 3, double, double>;

// Instantiate history classes rank = 1
template class PsHistory<float, 1, float, TxTensor<float, 1> >;
template class PsHistory<float, 2, float, TxTensor<float, 1> >;
template class PsHistory<float, 3, float, TxTensor<float, 1> >;

template class PsHistory<double, 1, double, TxTensor<double, 1> >;
template class PsHistory<double, 2, double, TxTensor<double, 1> >;
template class PsHistory<double, 3, double, TxTensor<double, 1> >;

// Instantiate history classes rank = 2
template class PsHistory<float, 1, float, TxTensor<float, 2> >;
template class PsHistory<float, 2, float, TxTensor<float, 2> >;
template class PsHistory<float, 3, float, TxTensor<float, 2> >;

template class PsHistory<double, 1, double, TxTensor<double, 2> >;
template class PsHistory<double, 2, double, TxTensor<double, 2> >;
template class PsHistory<double, 3, double, TxTensor<double, 2> >;

// Instantiate history field classes rank = 3
template class PsHistory<float, 2, float, TxTensor<float, 3> >;
template class PsHistory<double, 2, double, TxTensor<double, 3> >;

template class PsHistory<float, 3, float, TxTensor<float, 3> >;
template class PsHistory<double, 3, double, TxTensor<double, 3> >;
