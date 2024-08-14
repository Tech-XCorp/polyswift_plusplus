/**
 *
 * @file    PsCommBase.cpp
 *
 * @brief   Implementation for a communication center
 *
 * @version $Id: PsCommBase.cpp 9936 2008-05-22 19:29:36Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psbase includes
#include <PsCommBase.h>

template <class FLOATTYPE, size_t NDIM>
void PsCommBase<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Using base class to set attributes for base polymer class
  PsDynObjBase::setAttrib(tas);

  // Debug flag
  if (tas.hasString("printdebug")) {
    std::string dbPrint = tas.getString("printdebug");
    if (dbPrint == "on") this->dbStatus = PSDB_ON;
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsCommBase<FLOATTYPE, NDIM>::resizeArray(FLOATTYPE* &array,
    size_t& numElem, size_t reqNumElem) const {

  if (numElem < reqNumElem) {

// Delete old array and allocate new memory
    delete[] array;
    array = new FLOATTYPE[reqNumElem];
  }

// Set number of elements
  numElem = reqNumElem;

  return;
}

template <class FLOATTYPE, size_t NDIM>
void PsCommBase<FLOATTYPE, NDIM>::resizeFltSendBuf(size_t reqSize) const {
  if (sendFltSize < reqSize) {
    delete [] sendFltBuf;
    sendFltSize = reqSize;
    sendFltBuf = new FLOATTYPE[sendFltSize];
  }
  return;
}

template <class FLOATTYPE, size_t NDIM>
void PsCommBase<FLOATTYPE, NDIM>::resizeFltRecvBuf(size_t reqSize) const {
  if (recvFltSize < reqSize) {
    delete [] recvFltBuf;
    recvFltSize = reqSize;
    recvFltBuf = new FLOATTYPE[recvFltSize];
  }
  return;
}

template <class FLOATTYPE, size_t NDIM>
void PsCommBase<FLOATTYPE, NDIM>::resizeIntSendBuf(size_t reqSize) const {
  if (sendIntSize < reqSize) {
    delete [] sendIntBuf;
    sendIntSize = reqSize;
    sendIntBuf = new int[sendIntSize];
  }
  return;
}

template <class FLOATTYPE, size_t NDIM>
void PsCommBase<FLOATTYPE, NDIM>::resizeIntRecvBuf(size_t reqSize) const {
  if (recvIntSize < reqSize) {
    delete [] recvIntBuf;
    recvIntSize = reqSize;
    recvIntBuf = new int[recvIntSize];
  }
  return;
}

template class PsCommBase<float, 1>;
template class PsCommBase<float, 2>;
template class PsCommBase<float, 3>;

template class PsCommBase<double, 1>;
template class PsCommBase<double, 2>;
template class PsCommBase<double, 3>;
