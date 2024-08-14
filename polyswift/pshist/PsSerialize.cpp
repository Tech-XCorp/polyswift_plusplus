/**
 * @file    PsSerialize.cpp
 *
 * @brief   Serializes data for sync/dump data methods
 *
 * @version $Id: PsSerialize.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// txbase includes
#include <TxTensor.h>

// pshist
#include <PsSerialize.h>

//
// Constructor
//
template <class ELEMENTTYPE, class DATATYPE>
PsSerialize<ELEMENTTYPE, DATATYPE>::PsSerialize() { }

//
// main tensor method
//
template <class ELEMENTTYPE, class DATATYPE>
void PsSerialize<ELEMENTTYPE, DATATYPE>::serialize(
     std::vector<DATATYPE> &data) {

  size_t dataLength = data.size();
  size_t datumSize;

  // Set serialShape list with rank/size of TxTensor
  // and check for zero length history

  if (dataLength == 0) {
    this->serialShape.resize(1);
    this->serialShape[0] = dataLength;
    datumSize = 0;
  }
  else {
    size_t txRank = data[0].getTensorRank();
    this->serialShape.resize(txRank+1);
    for (size_t j=0; j<txRank; ++j) {
      this->serialShape[j+1] = data[0].getLength(j);
    }
    datumSize = data[0].getSize();
    this->serialShape[0] = dataLength;
  }

  // Total size of possibly new serialPtr
  size_t trialPtrSize = this->serialShape[0]*datumSize;

  // Check and allocate pointer size
  if (trialPtrSize != this->serialPtrSize) {
    delete this->serialPtr;
    this->serialPtrSize = trialPtrSize;
    this->serialPtr = new ELEMENTTYPE[this->serialPtrSize];
  }

  // set serialPtr
  size_t n = 0;
  for (size_t i=0; i<this->serialShape[0]; ++i) {
    for (size_t j=0; j<datumSize; ++j) {
      this->serialPtr[n] = data[i](j);
      ++n;
    }
  }

}

//
// main scalar method
//
template <class ELEMENTTYPE>
void PsSerialize<ELEMENTTYPE, ELEMENTTYPE>::serialize(
     std::vector<ELEMENTTYPE> &data) {

  // Set info and total size of possibly new serialPtr
  this->serialShape.resize(1);
  this->serialShape[0] = data.size();
  size_t trialPtrSize = this->serialShape[0];

  // Check and allocate pointer size
  if (trialPtrSize != this->serialPtrSize) {
    delete this->serialPtr;
    this->serialPtrSize = trialPtrSize;
    this->serialPtr = new ELEMENTTYPE[this->serialPtrSize];
  }

  // set serialPtr
  for (size_t i=0; i<this->serialShape[0]; ++i) {
    this->serialPtr[i] = data[i];
  }

}

// Instantiate history classes

// Scalar classes
template class PsSerialize<int,    int>;
template class PsSerialize<float,  float>;
template class PsSerialize<double, double>;

// TxTensor classes
template class PsSerialize<float,  TxTensor<float, 1>  >;
template class PsSerialize<double, TxTensor<double, 1> >;

template class PsSerialize<float,  TxTensor<float, 2>  >;
template class PsSerialize<double, TxTensor<double, 2> >;

template class PsSerialize<float,  TxTensor<float, 3>  >;
template class PsSerialize<double, TxTensor<double, 3> >;

/*
 Code above replaces this ordering
  size_t n = 0;
  for (size_t i=0; i<serialShape[0]; ++i) {
    for (size_t j=0; j<serialShape[1]; ++j) {
      for (size_t k=0; k<serialShape[2]; ++k) {
        for (size_t l=0; l<serialShape[3]; ++l) {
          serialPtr[n] = data[i](j, k, l);
          ++n;
        }
      }
    }
  }
*/
