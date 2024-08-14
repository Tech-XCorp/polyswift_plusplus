/**
 *
 * @file    PsMpiComm.h
 *
 * @brief   Interface for a communication center using MPI
 *
 * @version $Id: PsMpiComm.h 9936 2008-05-22 19:29:36Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_MPI_COMM_H
#define PS_MPI_COMM_H

// Standard includes
#include <TxDebugExcept.h>

// psbase includes
#include <PsCommBase.h>

// MPI includes
#ifdef HAVE_MPI
#define MPICH_IGNORE_CXX_SEEK
#include <mpi.h>
#define Ps_MPI_Comm     MPI_Comm
#define Ps_MPI_Group    MPI_Group
#define Ps_MPI_Datatype MPI_Datatype
#else
#define Ps_MPI_Comm int
#define Ps_MPI_Group int
#define Ps_MPI_Datatype int
//#define MPI_Info int
//#define MPI_INFO_NULL 0
#endif

/**
 * A message center is an object that lets one know whether a message
 * is available and if so, from what rank.
 */
template <class FLOATTYPE, size_t NDIM>
class PsMpiComm : public PsCommBase<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor: get and store the MPI variables
 *
 */
    PsMpiComm();

/**
 * Destructor
 */
    virtual ~PsMpiComm() {}

/**
 * Get the MPI type for FLOATTYPE
 *
 * @return the MPI type for float type
 */
    Ps_MPI_Datatype getFloatType() const {
      return mpiFloatType;
    }

/**
 * Return this rank
 *
 * @return this rank
 */
    virtual size_t getRank() const {
      return (size_t) mpiRank;
    }

/**
 * Return the size of this communicator
 *
 * @return the size of communicator
 */
    virtual size_t getSize() const {
      return (size_t) mpiCommSize;
    }

/**
 * Return the  ID of the communicator
 *
 * @return the ID of communicator
 */
    virtual size_t getComm() const {
      return (size_t) mpiComm;
    }

/**
 * Stop until all ranks check in.
 *
 */
    virtual void barrier() const {
#ifdef HAVE_MPI
      MPI_Barrier(mpiComm);
#endif
    }

/**
 * Return the communicator
 *
 * @return the communicator
 */
    virtual Ps_MPI_Comm getMpiComm() const {
      return mpiComm;
    }

/**
 * Return the MPI_Info.  I am not sure what this needs to be so it
 * is currently set to null.
 *
 * @return the MPI_Info
 */
//    virtual MPI_Info getMpiInfo() const {
//      return MPI_INFO_NULL;
//    }

/**
 * Broadcast vector to all other ranks.
 *
 * @param tv tiny vector to broadcast
 * @param sendNode rank of domain to broadcast
 */
    virtual PsTinyVector<int, NDIM> broadcastVec(
        PsTinyVector<int, NDIM> tv, size_t sendNode);

/**
 * Gather data from other ranks.
 *
 * @param sendData data to be sent by processors
 * @param sendSize is the length of the data array
 * @param recvData is the data received by any processor
 * @param recvSize isthe size of the data being gathered
 */
    virtual void allGatherData(size_t* sendData, size_t sendSize,
        size_t* recvData, size_t recvSize) const;

/**
 * Gather data from other ranks.
 *
 * @param sendData data to be sent by processors
 * @param sendSize is the length of the data array
 * @param recvData is the data received by any processor
 * @param recvSize isthe size of the data being gathered
 */
    virtual void allGatherData(FLOATTYPE* sendData, size_t sendSize,
        FLOATTYPE* recvData, size_t recvSize) const;

/**
 * Sum data from all ranks and distribute the sum
 *
 * @param x the local part of the sum
 *
 * @return the sum over all ranks
 */
    virtual size_t allReduceSum(size_t x) const;

/**
 * Sum data from all ranks and distribute the sum
 *
 * @param x the local part of the sum
 *
 * @return the sum over all ranks
 */
    virtual FLOATTYPE allReduceSum(FLOATTYPE x) const;

/**
 * Find min of data from all ranks
 *
 * @param x the local part of the data
 *
 * @return the minimum values from all ranks
 */
    virtual std::vector<FLOATTYPE> allReduceMin(std::vector<FLOATTYPE> x) const;

/**
 * Find max of data from all ranks
 *
 * @param x the local part of the data
 *
 * @return the maximum values from all ranks
 */
    virtual std::vector<FLOATTYPE> allReduceMax(std::vector<FLOATTYPE> x) const;

/**
 * Sum floattype 1d-vector data from all ranks and distribute the sum
 *
 * @param vec     the local 1d-vector part of the sum
 * @param numElem the 1d-vector length
 * @param vecSum  the summed 1d-vector
 */
    virtual void allReduceSumVec(FLOATTYPE* vec, size_t numElem,
        FLOATTYPE* vecSum) const;

/**
 * Sum integer 1d-vector data from all ranks and distribute the sum
 *
 * @param vec     the local 1d-vector part of the sum
 * @param numElem the 1d-vector length
 * @param vecSum  the summed 1d-vector
 */
    virtual void allReduceSumVec(int* vec, size_t numElem, int* vecSum) const;

/**
 * Unpack vector of tiny vectors and call all reduce and return
 * sync-ed data structure
 *
 * @param tvVec    vector of tiny vector
 * @param tvVecSum vector of tiny vector
 */
    virtual void allReduceSumVec(
        std::vector<PsTinyVector<int, NDIM> >& tvVec,
        std::vector<PsTinyVector<int, NDIM> >& tvVecSum);

/**
 * Find minima of size_t data from all ranks and distribute
 *
 * @param x the local values
 * @param numElem number of elements
 * @param xmin the minima over all processors
 */
    virtual void allReduceMin(size_t* x, size_t numElem, size_t* xmin) const;

/**
 * Find maxima of size_t data from all ranks and distribute
 *
 * @param x the local values
 * @param numElem number of elements
 * @param xmax the maxima over all processors
 */
    virtual void allReduceMax(size_t* x, size_t numElem, size_t* xmax) const;

/**
 * Find maxima of floattype data from all ranks and distribute
 *
 * @param xloc the local value
 * @return maximum global element
 */
    virtual FLOATTYPE allReduceMax(FLOATTYPE xloc) const;

/**
 * Send a float array to another rank.
 *
 * @param array float array being sent
 * @param count number of array elements being sent
 * @param recvRank rank that will receive array
 */
    virtual void sendArray(FLOATTYPE* array, size_t count,
        size_t recvRank) const;

/**
 * Send an integer array to another rank.
 *
 * @param array integer array being sent
 * @param count number of array elements being sent
 * @param recvRank rank that will receive array
 */
    virtual void sendArray(int* array, size_t count, size_t recvRank) const;

/**
 * Receive an integer array from another rank.
 *
 * @param array integer array that is filled with received values
 * @param count number of array elements being received
 * @param sendRank rank that sent array
 */
    virtual void recvArray(int* array, size_t count, size_t sendRank) const;

/**
 * Receive a float array from another rank.
 *
 * @param array float that is filled with received values
 * @param count number of array elements that are received
 * @param sendRank rank that sent array
 */
    virtual void recvArray(FLOATTYPE* array, size_t count,
        size_t sendRank) const;

/**
 * Receive a float array from another rank and resize if
 * necessary.  Used for situations where the receiving rank
 * does not know size of the data ahead of time. The array is not
 * owned but it may be deleted and reset if too small.
 *
 * @param array float array being received and possibly resized
 * @param numElem number of array elements being received.  Is
 *                set to the size of received array.
 * @param sendRank rank that sent array
 */
    virtual void recvArrayAndResize(FLOATTYPE* &array, size_t& numElem,
        size_t sendRank) const;

/**
 * Gather variable length data from other ranks for size_t data.
 *
 * @param sendData data to be sent by this rank
 * @param sendSize is the length of the data array to be sent
 * The following arguments are placeholders for arrays to be returned.
 *     They WILL be resized and modified in the function.
 * @param recvData is the data gathered by this rank
 * @param recvSize is a list of sizes of the data being gathered, one per rank
 * @param displs a list of displacements in the recvData array for data
 *     from each rank.  This information is redundant, since it can be
 *     obtained from recvSize, but it is consistent with the
 *     MPI_Allgatherv interface.
 */
    virtual void allGatherVS(size_t* sendData, size_t sendSize,
        size_t* &recvData, size_t* &recvSize, size_t* &displs) const;

/**
 * Gather variable length data from other ranks for FLOATTYPE data.
 *
 * @param sendData data to be sent by this rank
 * @param sendSize is the length of the data array to be sent
 * The following arguments are placeholders for arrays to be returned.
 *     They WILL be resized and modified in the function.
 * @param recvData is the data gathered by this rank
 * @param recvSize is a list of sizes of the data being gathered, one per rank
 * @param displs a list of displacements in the recvData array for data
 *     from each rank.  This information is redundant, since it can be
 *     obtained from recvSize, but it is consistent with the
 *     MPI_Allgatherv interface.
 */
    virtual void allGatherVS(FLOATTYPE* sendData, size_t sendSize,
        FLOATTYPE* &recvData, size_t* &recvSize, size_t* &displs) const;

/**
 * Gather variable length data from other ranks for FLOATTYPE data.
 *
 * @param sendData data to be sent by this rank
 * @param sendSize is the length of the data array to be sent
 * The following arguments are placeholders for arrays to be returned.
 *     They WILL be resized and modified in the function.
 * @param recvData is the data gathered by this rank
 * @param recvSize is a list of sizes of the data being gathered, one per rank
 * @param displs a list of displacements in the recvData array for data
 *     from each rank.  This information is redundant, since it can be
 *     obtained from recvSize, but it is consistent with the
 *     MPI_Allgatherv interface.
 */
    virtual void allGatherV(FLOATTYPE* sendData, size_t sendSize,
        FLOATTYPE* &recvData, size_t* &recvSize, size_t* &displs) const;

/**
 * All to all data transfer with other ranks, using MPI_Alltoallv
 *
 * @param sendData data to be sent by this rank
 * @param sendSize is the length of the data array to be sent
 * @param sendDispls a list of displacements in the sendData array for data
 *                   to send to each rank.
 * The following arguments are placeholders for arrays to be returned.
 *     They WILL be resized and modified in the function.
 * @param recvData is the data gathered by this rank
 * @param recvSize is a list of sizes of the data being gathered, one per rank
 * @param displs a list of displacements in the recvData array for data
 *     from each rank.  This information is redundant, since it can be
 *     obtained from recvSize, but it is consistent with the
 *     MPI_Allgatherv interface.
 */
    virtual void allToAllV(FLOATTYPE* sendData, size_t* sendSize,
        size_t* sendDispls, FLOATTYPE* &recvData,  size_t* &recvSize,
        size_t* &displs) const;

  protected:

  private:

// To prevent use
    PsMpiComm(const PsMpiComm<FLOATTYPE, NDIM>& vpfb) {}
    PsMpiComm<FLOATTYPE, NDIM>& operator=(
        const PsMpiComm<FLOATTYPE, NDIM>& vpfb) { return *this;}

/** The MPI communicator */
    Ps_MPI_Comm mpiComm;

/** The MPI group */
//    MPI_Group mpiGroup;
    Ps_MPI_Group mpiGroup;

/** The size of the mpi communicator */
    int mpiCommSize;

/** The rank of this processor */
    int mpiRank;

/** The float type */
    Ps_MPI_Datatype mpiFloatType;

/**
 * The tag for the message - defaults to 32767 (2^15 -1) so it
 * does not conflict with other tags.
 */
    int mpiTag;

#ifdef HAVE_MPI
/** float array send request */
    mutable MPI_Request arrayFltSndReq;

/** integer array send request */
    mutable MPI_Request arrayIntSndReq;

/** array receive request */
    mutable MPI_Status arrayRecvStatus;
#endif

/** Flag indicating float array send status */
    mutable bool sendingFltArray;

/** Flag indicating float array send status */
    mutable bool sendingIntArray;

};

#endif // PS_MPI_COMM_H
