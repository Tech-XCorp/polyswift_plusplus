/**
 * @file    PsCommHistory.cpp
 *
 * @brief   Base class for a diagnostic that performs aggregation
 *
 * @version $Id: PsCommHistory.cpp 11989 2009-03-25 05:14:55Z dws $
 *
 * Copyright &copy; 2009-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// txbase includes
#include <TxTensor.h>

// pshist includes
#include <PsCommHistory.h>

#include <PsCommBase.h>

//
template <class FLOATTYPE, size_t  NDIM, class ELEMENTTYPE, class DATATYPE>
void PsCommHistory<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::sync() {

  // Serialize history data suitable for sync/dump
  this->serialize(this->histData);

  // Length needed for comm routine
  size_t serialSize = 1;
  for (size_t i=0; i<this->serialShape.size(); ++i) {
    serialSize = serialSize*this->serialShape[i];
  }

  ELEMENTTYPE* vecSum;
  vecSum = new ELEMENTTYPE[serialSize];

  // Call comm routine
  this->getCommBase().allReduceSumVec(this->serialPtr, serialSize, vecSum);

  // Load elements of vecSum back to serialized data pointer
  for (size_t n=0; n<serialSize; ++n) {
    this->serialPtr[n] = vecSum[n];
  }

  // Clear summed list
  delete [] vecSum;
}

// Instantiate history classes scalar
// Scalar DATATYPE represented by int
template class PsCommHistory<float, 1, int, int>;
template class PsCommHistory<float, 2, int, int>;
template class PsCommHistory<float, 3, int, int>;

template class PsCommHistory<double, 1, int, int>;
template class PsCommHistory<double, 2, int, int>;
template class PsCommHistory<double, 3, int, int>;

template class PsCommHistory<float, 1, float, float>;
template class PsCommHistory<float, 2, float, float>;
template class PsCommHistory<float, 3, float, float>;

template class PsCommHistory<double, 1, double, double>;
template class PsCommHistory<double, 2, double, double>;
template class PsCommHistory<double, 3, double, double>;

// Instantiate history classes rank = 1
template class PsCommHistory<float, 1, float, TxTensor<float, 1> >;
template class PsCommHistory<float, 2, float, TxTensor<float, 1> >;
template class PsCommHistory<float, 3, float, TxTensor<float, 1> >;

template class PsCommHistory<double, 1, double, TxTensor<double, 1> >;
template class PsCommHistory<double, 2, double, TxTensor<double, 1> >;
template class PsCommHistory<double, 3, double, TxTensor<double, 1> >;

// Instantiate history classes rank = 2
template class PsCommHistory<float, 1, float, TxTensor<float, 2> >;
template class PsCommHistory<float, 2, float, TxTensor<float, 2> >;
template class PsCommHistory<float, 3, float, TxTensor<float, 2> >;

template class PsCommHistory<double, 1, double, TxTensor<double, 2> >;
template class PsCommHistory<double, 2, double, TxTensor<double, 2> >;
template class PsCommHistory<double, 3, double, TxTensor<double, 2> >;

// Instantiate history field classes rank = 3
template class PsCommHistory<float, 2, float, TxTensor<float, 3> >;
template class PsCommHistory<double, 2, double, TxTensor<double, 3> >;

template class PsCommHistory<float, 3, float, TxTensor<float, 3> >;
template class PsCommHistory<double, 3, double, TxTensor<double, 3> >;
