/**
 *
 * @file    PsCommBase.h
 *
 * @brief   Implementation for a communication center This is to contain
 *          the basic communication methods, which are implemented
 *          in a derived class by some messaging system, such as MPI.
 *          Methods are intended to be consistent with MPI interface,
 *          but could be implemented some other way if MPI is not available.
 *
 * @version $Id: PsCommBase.h 9936 2008-05-22 19:29:36Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_COMM_BASE_H
#define PS_COMM_BASE_H

// std includes
#include <string>
#include <vector>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// txbase includes
#include <TxDebugExcept.h>
// #include <TxThroughStream.h>

// psstd includes
#include <PsTinyVector.h>

// psbase includes
#include <PsDynObj.h>

/**
 * Provides a basic interface for communication between different processes,
 * particularly for passing message involving floattype (float or double) and
 * integer data.  Most methods are pure virtual required to be implemented in
 * derived class using some communication library such as MPI or PVM or some
 * communication scheme (such as self-messaging).
 */

template <class FLOATTYPE, size_t NDIM>
class PsCommBase : public virtual PsDynObj<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor.  Initializes buffers and buffer sizes.
 */
    PsCommBase() {

      //      thruStrmPtr = new TxThroughStream(std::cout);
      //      thruStrmOwner = true;

      sendFltBuf = 0;
      sendIntBuf = 0;
      recvFltBuf = 0;
      recvIntBuf = 0;
      sendFltSize = 0;
      sendIntSize = 0;
      recvFltSize = 0;
      recvIntSize = 0;

    }

/**
 * Destructor
 */
    virtual ~PsCommBase() {
      //      if (thruStrmOwner) delete thruStrmPtr;
      delete [] sendFltBuf;
      delete [] sendIntBuf;
      delete [] recvFltBuf;
      delete [] recvIntBuf;
    }

/**
 * Set the parameters
 *
 * @param tas the parameters
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Set the stream to send comments to.
 *
 * @param tstr per processor stream to send comments to
 */
//    void setThruStrm(TxThroughStream& tstr) {
//      if (thruStrmOwner) delete thruStrmPtr;
//      thruStrmPtr = &tstr;
//      thruStrmOwner = false;
//    }

/**
 * Get the stream to send comments to.
 *
 * @return per processor stream to send comments to
 */
//    TxThroughStream* getThruStrm() const {
//      return thruStrmPtr;
//    }

/**
 * Return this rank
 *
 * @return this rank
 */
    virtual size_t getRank() const = 0;

/**
 * Return the size of this communicator
 *
 * @return the size
 */
    virtual size_t getSize() const = 0;

/**
 * Return the ID of the communicator
 *
 * @return the size
 */
    virtual size_t getComm() const = 0;

/**
 * Wait for all processes to check in.
 *
 * @return the size
 */
    virtual void barrier() const = 0;

/**
 * Broadcast vector to all other ranks.
 *
 * @param tv tiny vector to broadcast
 * @param sendNode rank of domain to broadcast
 */
    virtual PsTinyVector<int, NDIM> broadcastVec(
       PsTinyVector<int, NDIM> tv, size_t sendNode) = 0;

/**
 * Gather data from other ranks
 *
 * @param sendData data to be sent by this rank
 * @param sendSize is the length of the data array
 * @param recvData is the data gathered by this rank
 * @param recvSize is the size of the data being gathered
 */
    virtual void allGatherData(size_t* sendData, size_t sendSize,
        size_t* recvData, size_t recvSize) const = 0;

/**
 * Sum size_t data from all ranks and distribute the sum
 *
 * @param x the local part of the sum
 * @return the sum over all ranks
 */
    virtual size_t allReduceSum(size_t x) const = 0;

/**
 * Sum floattype data from all ranks and distribute the sum
 *
 * @param x the local part of the sum
 * @return the sum over all ranks
 */
    virtual FLOATTYPE allReduceSum(FLOATTYPE x) const = 0;

/**
 * Find min of data from all ranks
 *
 * @param x the local part of the data
 *
 * @return the minimum values from all ranks
 */
    virtual std::vector<FLOATTYPE> allReduceMin(std::vector<FLOATTYPE> x) const = 0;

/**
 * Find max of data from all ranks
 *
 * @param x the local part of the data
 *
 * @return the maximum values from all ranks
 */
    virtual std::vector<FLOATTYPE> allReduceMax(std::vector<FLOATTYPE> x) const = 0;

/**
 * Sum floattype 1d-vector data from all ranks and distribute the sum
 *
 * @param vec     the local 1d-vector part of the sum
 * @param numElem the 1d-vector length
 * @param vecSum  the summed 1d-vector
 */
    virtual void allReduceSumVec(FLOATTYPE* vec, size_t numElem,
        FLOATTYPE* vecSum) const = 0;

/**
 * Sum floattype 1d-vector data from all ranks and distribute the sum
 *
 * @param vec     the local 1d-vector part of the sum
 * @param numElem the 1d-vector length
 * @param vecSum  the summed 1d-vector
 */
    virtual void allReduceSumVec(int* vec, size_t numElem, int* vecSum)
        const = 0;

/**
 * Unpack vector of tiny vectors and call all reduce and return
 * sync-ed data structure
 *
 * @param tvVec    vector of tiny vector
 * @param tvVecSum vector of tiny vector
 */
    virtual void allReduceSumVec(
        std::vector<PsTinyVector<int, NDIM> >& tvVec,
        std::vector<PsTinyVector<int, NDIM> >& tvVecSum) = 0;
/**
 * Find minima of size_t data from all ranks and distribute
 *
 * @param x the local values
 * @param numElem number of elements
 * @param xmin the minima over all processors
 */
    virtual void allReduceMin(size_t* x, size_t numElem, size_t* xmin) const = 0;

/**
 * Find maxima of size_t data from all ranks and distribute
 *
 * @param x the local values
 * @param numElem number of elements
 * @param xmax the maxima over all processors
 */
    virtual void allReduceMax(size_t* x, size_t numElem, size_t* xmax) const = 0;

/**
 * Find maxima of floattype data from all ranks and distribute
 *
 * @param xloc the local value
 * @return maximum global element
 */
    virtual FLOATTYPE allReduceMax(FLOATTYPE xloc) const = 0;

/**
 * Gather vector data from other ranks.
 *
 * @param sendData data to be sent by this rank
 * @param sendSize is the length of the data array to be sent
 * The following arguments are placeholders for arrays to be returned.
 *     They WILL be resized and modified in the function.
 * @param recvData is the data gathered by this rank
 * @param recvSize is a list of sizes of the data being gathered
 * @param displs a list of displacements in the recvData array for
 *     data from each rank.  This information is redundant, since it
 *     can be obtained from recvSize, but it is consistent with the
 *     MPI_Allgatherv interface.
 */
    virtual void allGatherV(FLOATTYPE* sendData, size_t sendSize,
        FLOATTYPE* &recvData, size_t* &recvSize, size_t* &displs) const = 0;

/**
 * Gather vector data from other ranks
 *
 * @param sendData data to be sent by this rank
 * @param sendSize is the length of the data array to be sent
 * The following arguments are placeholders for arrays to be returned.
 *     They WILL be resized and modified in the function.
 * @param recvData is the data gathered by this rank
 * @param recvSize is a list of sizes of the data being gathered
 * @param displs a list of displacements in the recvData array for data
 *     from each rank.  This information is redundant, since it can be
 *     obtained from recvSize, but it is consistent with the
 *     MPI_Allgatherv interface.
 */
    virtual void allGatherVS(size_t* sendData, size_t sendSize,
        size_t* &recvData, size_t* &recvSize, size_t* &displs) const = 0;

/**
 * Gather vector data from other ranks
 *
 * @param sendData data to be sent by this rank
 * @param sendSize is the length of the data array to be sent
 * The following arguments are placeholders for arrays to be returned.
 *     They WILL be resized and modified in the function.
 * @param recvData is the data gathered by this rank
 * @param recvSize is a list of sizes of the data being gathered
 * @param displs a list of displacements in the recvData array for data
 *     from each rank.  This information is redundant, since it can be
 *     obtained from recvSize, but it is consistent with the
 *     MPI_Allgatherv interface.
 */
    virtual void allGatherVS(FLOATTYPE* sendData, size_t sendSize,
        FLOATTYPE* &recvData, size_t* &recvSize, size_t* &displs) const = 0;

/**
 * All to all data transfer with other ranks, using MPI_Alltotallv
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
        size_t* sendDispls, FLOATTYPE* &recvData, size_t* &recvSize,
        size_t* &displs) const = 0;

/**
 * Send a float array to another rank.
 *
 * @param array float array being sent
 * @param numElem number of array elements being sent
 * @param recvRank rank that will receive array
 */
    virtual void sendArray(FLOATTYPE* array, size_t numElem,
        size_t recvRank) const = 0;

/**
 * Receive a float array from another rank.
 *
 * @param array float array that is filled with received values
 * @param numElem number of array elements that are received
 * @param sendRank rank that sent array
 */
    virtual void recvArray(FLOATTYPE* array, size_t numElem,
        size_t sendRank) const = 0;

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
        size_t sendRank) const = 0;

/**
 * Send an integer array to another rank.
 *
 * @param array integer array being sent
 * @param numElems the number of integers to send
 * @param recvRank rank that will receive array
 */
    virtual void sendArray(int* array, size_t numElems,
        size_t recvRank) const = 0;

/**
 * Receive an integer array from another rank.
 *
 * @param array integer array that is filled with received values
 * @param numElem number of array elements being received
 * @param sendRank rank that sent array
 */
    virtual void recvArray(int* array, size_t numElem,
        size_t sendRank) const = 0;

  protected:

/**
 * Resizes a float array.  Sets number of elements to the exact number
 * of elements in the data.
 *
 * @param array the array to be resized.  This is not owned
 *              but may be deleted and resized
 * @param numElem the number of elements in the array. Is set to reqNumElem
 * @param reqNumElem the number of array elements needed
 */
    void resizeArray(FLOATTYPE* &array, size_t& numElem, size_t reqNumElem)
        const;

/**
 * Checks the float buffer and resizes it if necessary
 *
 * @param reqSize the needed size for the buffer
 */
    void resizeFltSendBuf(size_t reqSize) const;

/**
 * Checks the float buffer and resizes it if necessary
 *
 * @param reqSize the needed size for the buffer
 */
    void resizeFltRecvBuf(size_t reqSize) const;

/**
 * Checks the int buffer and resizes it if necessary
 *
 * @param reqSize the needed size for the buffer
 */
    void resizeIntSendBuf(size_t reqSize) const;

/**
 * Checks the int buffer and resizes it if necessary
 *
 * @param reqSize the needed size for the buffer
 */
    void resizeIntRecvBuf(size_t reqSize) const;

    /** Place to send my comments (per processor) */
//    TxThroughStream* thruStrmPtr;

    /** Whether I own the through stream */
//    bool thruStrmOwner;

    /** Communication buffer for sending floattype arrays */
    mutable FLOATTYPE* sendFltBuf;

    /** Communication buffer for sending integer arrays */
    mutable int* sendIntBuf;

    /** size of send float buffer */
    mutable size_t sendFltSize;

    /** size of send int buffer */
    mutable size_t sendIntSize;

    /** Communication buffer for receiving floattype arrays */
    mutable FLOATTYPE* recvFltBuf;

    /** Communication buffer for receiving floattype arrays */
    mutable int* recvIntBuf;

    /** size of receive float buffer */
    mutable size_t recvFltSize;

    /** size of receive int buffer */
    mutable size_t recvIntSize;

  private:

    // To prevent use
    PsCommBase(const PsCommBase<FLOATTYPE, NDIM>& vpfb) {}

    PsCommBase<FLOATTYPE, NDIM>& operator=(
      const PsCommBase<FLOATTYPE, NDIM>& vpfb) {return *this;}
};

#endif // PS_COMM_BASE_H
