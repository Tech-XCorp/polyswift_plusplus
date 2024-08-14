/**
 *
 * @file    PsMpiComm.cpp
 *
 * @brief   Implementation for an MPI communication center
 *
 * @version $Id: PsMpiComm.cpp 9936 2008-05-22 19:29:36Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// pscomm includes
#include <PsMpiComm.h>

// replacing getMpiComm() --> MPI_COMM_WORLD

//
// Construct - store data
//
template <class FLOATTYPE, size_t NDIM>
PsMpiComm<FLOATTYPE, NDIM>::PsMpiComm() {

// Set tag and sending array flags
  mpiTag = 32767;
  sendingFltArray = false;
  sendingIntArray = false;

// Set up the communicators
#ifndef HAVE_MPI
  mpiGroup = 0;
  mpiComm = 0;
  mpiCommSize = 1;
  mpiRank = 0;
  return;
#else

// Set up communicator
  MPI_Comm_group(MPI_COMM_WORLD, &mpiGroup);
  MPI_Comm_create(MPI_COMM_WORLD, mpiGroup, &mpiComm);
  MPI_Comm_size(mpiComm, &mpiCommSize);
  MPI_Comm_rank(mpiComm, &mpiRank);

// Create float type
  if (sizeof(FLOATTYPE) == sizeof(float)) mpiFloatType = MPI_FLOAT;
  else if (sizeof(FLOATTYPE) == sizeof(double)) mpiFloatType = MPI_DOUBLE;

#endif

}

template <class FLOATTYPE, size_t NDIM>
PsTinyVector<int, NDIM> PsMpiComm<FLOATTYPE, NDIM>::broadcastVec(
     PsTinyVector<int, NDIM> tv, size_t sendNode) {

  size_t vecSize = NDIM;
  int* intVec = new int[vecSize];

#ifdef HAVE_MPI
  for (size_t i=0; i<vecSize; ++i) {
    intVec[i] = tv[i];
  }
  MPI_Bcast(&intVec, vecSize, MPI_INT, sendNode, MPI_COMM_WORLD);
  for (size_t i=0; i<vecSize; ++i) {
    tv[i] = intVec[i];
  }
#endif
  return tv;
}

template <class FLOATTYPE, size_t NDIM>
void PsMpiComm<FLOATTYPE, NDIM>::allGatherData(size_t* sendData,
    size_t sendSize, size_t* recvData, size_t recvSize) const {
#ifdef HAVE_MPI
  MPI_Allgather(sendData, (int)sendSize, MPI_UNSIGNED_LONG, recvData,
      (int)recvSize, MPI_UNSIGNED_LONG, MPI_COMM_WORLD);
#else
  size_t minSize = sendSize < recvSize ? sendSize : recvSize;
  for (size_t i=0; i<minSize; ++i) recvData[i] = sendData[i];
#endif
}

template <class FLOATTYPE, size_t NDIM>
void PsMpiComm<FLOATTYPE, NDIM>::allGatherData(FLOATTYPE* sendData,
    size_t sendSize, FLOATTYPE* recvData, size_t recvSize) const {
#ifdef HAVE_MPI
  MPI_Allgather(sendData, (int)sendSize, this->mpiFloatType, recvData,
    (int)recvSize, this->mpiFloatType, MPI_COMM_WORLD);
#else
  size_t minSize = sendSize < recvSize ? sendSize : recvSize;
  for (size_t i=0; i<minSize; ++i) recvData[i] = sendData[i];
#endif
}

template <class FLOATTYPE, size_t NDIM>
size_t PsMpiComm<FLOATTYPE, NDIM>::allReduceSum(size_t x) const {
#ifdef HAVE_MPI
  size_t rcv;
  MPI_Allreduce(&x, &rcv, 1, MPI_UNSIGNED_LONG, MPI_SUM, MPI_COMM_WORLD);
  return rcv;
#else
  return x;
#endif
}

template <class FLOATTYPE, size_t NDIM>
FLOATTYPE PsMpiComm<FLOATTYPE, NDIM>::allReduceSum(FLOATTYPE x) const {
#ifdef HAVE_MPI
  FLOATTYPE rcv;
  MPI_Allreduce(&x, &rcv, 1, getFloatType(), MPI_SUM, MPI_COMM_WORLD);
  return rcv;
#else
  return x;
#endif
}

template <class FLOATTYPE, size_t NDIM>
std::vector<FLOATTYPE> PsMpiComm<FLOATTYPE, NDIM>::allReduceMin(std::vector<FLOATTYPE> x) const {
#ifdef HAVE_MPI

  size_t vecSize = x.size();
  FLOATTYPE* snd = new FLOATTYPE[vecSize];
  FLOATTYPE* rcv = new FLOATTYPE[vecSize];
  for (size_t i=0; i<vecSize; ++i) {
    snd[i] = x[i];
  }
  MPI_Allreduce(snd, rcv, vecSize, getFloatType(), MPI_MIN, MPI_COMM_WORLD);

  std::vector<FLOATTYPE> rcvVec;
  rcvVec.resize(vecSize);
   for (size_t i=0; i<vecSize; ++i)
     rcvVec[i] = rcv[i];
   return rcvVec;
#else
  return x;
#endif
}

template <class FLOATTYPE, size_t NDIM>
std::vector<FLOATTYPE> PsMpiComm<FLOATTYPE, NDIM>::allReduceMax(
       std::vector<FLOATTYPE> x) const {

#ifdef HAVE_MPI

  size_t vecSize = x.size();

  FLOATTYPE* snd = new FLOATTYPE[vecSize];
  FLOATTYPE* rcv = new FLOATTYPE[vecSize];
  for (size_t i=0; i<vecSize; ++i) {
    snd[i] = x[i];
  }
  MPI_Allreduce(snd, rcv, vecSize, getFloatType(), MPI_MAX, MPI_COMM_WORLD);
  std::vector<FLOATTYPE> rcvVec;
  rcvVec.resize(vecSize);
   for (size_t i=0; i<vecSize; ++i)
     rcvVec[i] = rcv[i];
   return rcvVec;
#else
  return x;
#endif
}

template <class FLOATTYPE, size_t NDIM>
FLOATTYPE PsMpiComm<FLOATTYPE, NDIM>::allReduceMax(FLOATTYPE xloc) const {

#ifdef HAVE_MPI
  FLOATTYPE xmax;
  MPI_Allreduce(&xloc, &xmax, 1, getFloatType(), MPI_MAX, MPI_COMM_WORLD);
  return xmax;
#else
  return xloc;
#endif
}

template <class FLOATTYPE, size_t NDIM>
void PsMpiComm<FLOATTYPE, NDIM>::allReduceSumVec(FLOATTYPE* vec, size_t numElem,
    FLOATTYPE* vecSum) const {
#ifdef HAVE_MPI
  MPI_Allreduce(vec, vecSum, numElem, getFloatType(), MPI_SUM, MPI_COMM_WORLD);
#else
  for (size_t i=0; i<numElem; ++i) {
    vecSum[i] = vec[i];
  }
#endif
}

template <class FLOATTYPE, size_t NDIM>
void PsMpiComm<FLOATTYPE, NDIM>::allReduceSumVec(int* vec, size_t numElem, int* vecSum) const {
#ifdef HAVE_MPI
  MPI_Allreduce(vec, vecSum, numElem, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
#else
  for (size_t i=0; i<numElem; ++i) {
    vecSum[i] = vec[i];
  }
#endif
}

template <class FLOATTYPE, size_t NDIM>
void PsMpiComm<FLOATTYPE, NDIM>::allReduceSumVec(
     std::vector<PsTinyVector<int, NDIM> >& tvVec,
     std::vector<PsTinyVector<int, NDIM> >& tvVecSum) {

  if (NDIM != 3) {
    TxDebugExcept tde("PsMpiComm::allReduceSumVec: ");
    tde << " hard coded for now to NDIM=3 ";
    std::cerr << tde.getMessage() << std::endl;
    throw tde;
  }

  tvVecSum.clear();
  size_t numElementsVec = tvVec.size();
  size_t vecSize = numElementsVec*NDIM;

#ifdef HAVE_MPI

  int* snd = new int[vecSize];
  int* rcv = new int[vecSize];

  for (size_t ivec=0; ivec<numElementsVec; ++ivec) {
    size_t vecIndx = ivec*NDIM;
    PsTinyVector<int, NDIM> tv = tvVec[ivec];
    snd[vecIndx+0] = tv[0];
    snd[vecIndx+1] = tv[1];
    snd[vecIndx+2] = tv[2];
    rcv[vecIndx+0] = 0;
    rcv[vecIndx+1] = 0;
    rcv[vecIndx+2] = 0;
  }

  MPI_Allreduce(snd, rcv, vecSize, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  for (size_t ivec=0; ivec<numElementsVec; ++ivec) {
    size_t vecIndx = ivec*NDIM;
    PsTinyVector<int, NDIM> tv(0);
    tv[0] = rcv[vecIndx+0];
    tv[1] = rcv[vecIndx+1];
    tv[2] = rcv[vecIndx+2];
    tvVecSum.push_back(tv);
  }
#else
  tvVecSum = tvVec;
#endif

  if (tvVecSum.size() != numElementsVec) {
    TxDebugExcept tde("PsMpiComm::allReduceSumVec: ");
    tde << " reduced vector length != input vector length";
    std::cerr << tde.getMessage() << std::endl;
    throw tde;
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsMpiComm<FLOATTYPE, NDIM>::allReduceMin(size_t* x, size_t numElem,
     size_t* xmin) const {
#ifdef HAVE_MPI
  MPI_Allreduce(x, xmin, numElem, MPI_UNSIGNED_LONG, MPI_MIN, MPI_COMM_WORLD);
#else
  for (size_t i=0; i<numElem; ++i) {
    xmin[i] = x[i];
  }
#endif
}

template <class FLOATTYPE, size_t NDIM>
void PsMpiComm<FLOATTYPE, NDIM>::allReduceMax(size_t* x, size_t numElem,
     size_t* xmax) const {
#ifdef HAVE_MPI
  MPI_Allreduce(x, xmax, numElem, MPI_UNSIGNED_LONG, MPI_MAX, MPI_COMM_WORLD);
#else
  for (size_t i=0; i<numElem; ++i) {
    xmax[i] = x[i];
  }
#endif
}

template <class FLOATTYPE, size_t NDIM>
void PsMpiComm<FLOATTYPE, NDIM>::sendArray(FLOATTYPE* array, size_t numElem,
     size_t recvRank) const {

#ifdef HAVE_MPI
// All ignored if no MPI

// Wait for previous send to complete
  if (sendingFltArray) {
    MPI_Status waitStatus;
    int waitres = MPI_Wait(&arrayFltSndReq, &waitStatus);
  }

// check and resize the buffer if needed
  this->resizeFltSendBuf(numElem);

// copy array into buffer
  for (size_t i=0; i<numElem; ++i) this->sendFltBuf[i] = array[i];

// send array
  int res = MPI_Isend(this->sendFltBuf, numElem, getFloatType(), recvRank,
      mpiTag, MPI_COMM_WORLD, &arrayFltSndReq);

  sendingFltArray = true;

#endif

  return;
}

template <class FLOATTYPE, size_t NDIM>
void PsMpiComm<FLOATTYPE, NDIM>::sendArray(int* array, size_t numElem,
     size_t recvRank) const {

#ifdef HAVE_MPI
// All ignored if no MPI

// Wait for previous send to complete
  if (sendingIntArray) {
    MPI_Status waitStatus;
    int waitres = MPI_Wait(&arrayIntSndReq, &waitStatus);
  }

// check and resize the buffer
  this->resizeIntSendBuf(numElem);

// copy array into buffer
  for (size_t i=0; i<numElem; ++i)
    this->sendIntBuf[i] = array[i];

// send array
  int res = MPI_Isend(this->sendIntBuf, numElem, MPI_INT,
      recvRank, mpiTag, MPI_COMM_WORLD, &arrayIntSndReq);

  sendingIntArray = true;

#endif

  return;
}

template <class FLOATTYPE, size_t NDIM>
void PsMpiComm<FLOATTYPE, NDIM>::recvArray(FLOATTYPE* array, size_t numElem,
    size_t sendRank) const {

#ifdef HAVE_MPI
// All ignored if no MPI
  int res;
  MPI_Status locMpiStatus;

// check to see if buffer is large enough
  this->resizeFltRecvBuf(numElem);

// Receive the array into the buffer
  res = MPI_Recv(this->recvFltBuf, numElem, getFloatType(),
      sendRank, mpiTag, MPI_COMM_WORLD, &locMpiStatus);

// copy buffer into array
  for (size_t i=0; i<numElem; ++i)
    array[i] = this->recvFltBuf[i];

#endif
  return;
}

template <class FLOATTYPE, size_t NDIM>
void PsMpiComm<FLOATTYPE, NDIM>::recvArray(int* array, size_t numElem,
     size_t sendRank) const {

#ifdef HAVE_MPI
// All ignored if no MPI
  int res;
  MPI_Status locMpiStatus;

// check to see if buffer is large enough
  this->resizeIntRecvBuf(numElem);

// Receive the particles into the buffer
  res = MPI_Recv(this->recvIntBuf, numElem, MPI_INT,
      sendRank, mpiTag, MPI_COMM_WORLD, &locMpiStatus);

// copy buffer into array
  for (size_t i=0; i<numElem; ++i)
    array[i] = this->recvIntBuf[i];

#endif
  return;
}

template <class FLOATTYPE, size_t NDIM>
void PsMpiComm<FLOATTYPE, NDIM>::recvArrayAndResize(FLOATTYPE* &array,
     size_t& numElem, size_t sendRank) const {

#ifdef HAVE_MPI
// All ignored if no MPI
  int res, numIncElem;
  MPI_Status locMpiStatus;

// Determine the size of the incoming data
  res = MPI_Probe(sendRank, mpiTag, MPI_COMM_WORLD, &locMpiStatus);
  res = MPI_Get_count(&locMpiStatus, getFloatType(), &numIncElem);

// Resize the receiving array so it is large enough
  this->resizeArray(array, numElem, numIncElem);

// Receive the data into the array
  res = MPI_Recv(array, numElem, getFloatType(), sendRank, mpiTag,
       MPI_COMM_WORLD, &locMpiStatus);
#else
  numElem = 0;
#endif
  return;
}

template <class FLOATTYPE, size_t NDIM>
void PsMpiComm<FLOATTYPE, NDIM>::allGatherV(FLOATTYPE* sendData,
    size_t sendSize, FLOATTYPE* &recvData,
    size_t* &recvSize, size_t* &displs) const {
#ifdef HAVE_MPI
  const size_t nProcs = this->getSize();
  const size_t thisRank = getRank();
  int* counts = new int[nProcs];
  int* displacements = new int[nProcs];
  int count = sendSize;

  MPI_Allgather(&count, 1, MPI_INT, counts, 1, MPI_INT, MPI_COMM_WORLD);

  displacements[0] = 0;
  for (size_t i=1;i<nProcs;i++) {
    displacements[i] = counts[i-1] + displacements[i-1];
  }

  // set up buffer for all particles
  size_t totalNum = displacements[nProcs-1] + counts[nProcs-1];
  recvData = new FLOATTYPE[totalNum];

  //
  MPI_Allgatherv(sendData, count,  mpiFloatType, recvData, counts, displacements,  mpiFloatType, MPI_COMM_WORLD);

  recvSize = new size_t[nProcs];
  displs =  new size_t[nProcs];
  for (size_t i=0;i<nProcs;i++) {
    recvSize[i] = counts[i];
    displs[i] = displacements[i];
  }

  delete[] counts;
  delete[] displacements;
#endif
}

template <class FLOATTYPE, size_t NDIM>
void PsMpiComm<FLOATTYPE, NDIM>::allGatherVS(size_t* sendData, size_t sendSize,
    size_t* &recvData,  size_t* &recvSize, size_t* &displs) const {
#ifdef HAVE_MPI
  size_t numProc = getSize();
  size_t thisRank = getRank();

// check to see if recvSize has been initialized. If so, delete.
  recvSize = new size_t[numProc];
  for (size_t i=0; i<numProc; ++i) {
    recvSize[i] = 0;
  }

// Use Allgather to get sizes of receive arrays
  MPI_Allgather(&sendSize, 1, MPI_UNSIGNED_LONG, recvSize, 1,
      MPI_UNSIGNED_LONG, MPI_COMM_WORLD);

  size_t globalSize = 0;
  for (size_t i=0; i<numProc; ++i) {
    globalSize += recvSize[i];
  }

// check to see if recvdata has been initialized. If so, delete.
  recvData = new size_t[globalSize];

// set up displacement list
// check to see if displs has been initialized. If so, delete.
  displs = new size_t[numProc];
  displs[0] = 0;
  for (size_t i=1; i<numProc; ++i) {
    displs[i] = recvSize[i-1] + displs[i-1];
  }

// set up recvData
  for (size_t i=0; i<sendSize; ++i) {
    recvData[displs[thisRank] + i] = sendData[i];
  }
// broadcast from each rank
  for (size_t i=0; i<numProc; ++i) MPI_Bcast(&recvData[displs[i]], recvSize[i],
      MPI_UNSIGNED_LONG, i, MPI_COMM_WORLD);
  return;
#endif
}

template <class FLOATTYPE, size_t NDIM>
void PsMpiComm<FLOATTYPE, NDIM>::allGatherVS(FLOATTYPE* sendData,
    size_t sendSize, FLOATTYPE* &recvData, size_t* &recvSize,
    size_t* &displs) const {
#ifdef HAVE_MPI
  size_t numProc = getSize();
  size_t thisRank = getRank();

// check to see if recvSize has been initialized. If so, delete.
  recvSize = new size_t[numProc];
  for (size_t i=0; i<numProc; ++i) {
    recvSize[i] = 0;
  }

// Use Allgather to get sizes of receive arrays
  MPI_Allgather(&sendSize, 1, MPI_UNSIGNED_LONG, recvSize, 1,
       MPI_UNSIGNED_LONG, MPI_COMM_WORLD);
  // allGatherData(&sendSize, 1, recvSize, 1);

  size_t globalSize = 0;
  for (size_t i=0; i<numProc; ++i) {
    globalSize += recvSize[i];
  }

// check to see if recvdata has been initialized. If so, delete.
  recvData = new FLOATTYPE[globalSize];

// set up displacement list
// check to see if displs has been initialized. If so, delete.
  displs = new size_t[numProc];
  displs[0] = 0;
  for (size_t i=1; i<numProc; ++i) {
    displs[i] = recvSize[i-1] + displs[i-1];
  }

// set up recvData
  for (size_t i=0; i<sendSize; ++i) {
    recvData[displs[thisRank] + i] = sendData[i];
  }
// broadcast from each rank
  for (size_t i=0; i<numProc; ++i)
    MPI_Bcast(&recvData[displs[i]], recvSize[i], mpiFloatType, i, MPI_COMM_WORLD);
    return;
#endif
}

// Wrapper for MPI_Alltoallv call
template <class FLOATTYPE, size_t NDIM>
void PsMpiComm<FLOATTYPE, NDIM>::allToAllV(FLOATTYPE* sendData, size_t* sendSize,
    size_t* sendDispls, FLOATTYPE* &recvData,
    size_t* &recvSize, size_t* &displs) const {

#ifdef HAVE_MPI
  size_t mpi_err;
  size_t thisRank = getRank();
  const size_t nProcs = getSize();

  int* sendSizeInt= new int[nProcs];
  int* recvSizeInt= new int[nProcs];
  int* sendDisplsInt= new int[nProcs];
  int* recvDisplsInt= new int[nProcs];

  recvSize= new size_t[nProcs];
  displs= new size_t[nProcs];

  size_t i;
  for (i=0;i<nProcs;i++) {
    sendSizeInt[i]= sendSize[i];
  }

  mpi_err = MPI_Alltoall(sendSizeInt, 1, MPI_INT,
       recvSizeInt, 1, MPI_INT, MPI_COMM_WORLD);

  sendDisplsInt[0]=0;
  for (i=1;i<nProcs;i++) {
    sendDisplsInt[i]=sendDispls[i];
  }
  recvDisplsInt[0]=0;
  for (i=1;i<nProcs;i++) {
    recvDisplsInt[i]=recvSizeInt[i-1]+recvDisplsInt[i-1];
  }

  size_t recvTotalSize=0;
  for (i=0;i<nProcs;i++) {
    recvTotalSize=recvTotalSize+recvSizeInt[i];
  }

// setup recv buffer
  recvData= new FLOATTYPE[recvTotalSize];

  mpi_err = MPI_Alltoallv(sendData, sendSizeInt, sendDisplsInt, mpiFloatType,
      recvData, recvSizeInt, recvDisplsInt, mpiFloatType, MPI_COMM_WORLD);

  for (i=0;i<nProcs;i++) {
    recvSize[i]=recvSizeInt[i];
    displs[i]=recvDisplsInt[i];
  }

  delete[] sendSizeInt;
  delete[] recvSizeInt;
  delete[] sendDisplsInt;
  delete[] recvDisplsInt;

#endif
}

template class PsMpiComm<float, 1>;
template class PsMpiComm<float, 2>;
template class PsMpiComm<float, 3>;

template class PsMpiComm<double, 1>;
template class PsMpiComm<double, 2>;
template class PsMpiComm<double, 3>;
