/**
 * @file    PsHistory.h
 *
 * @brief   Base class for histories
 *
 * @version $Id: PsHistory.h 7956 2007-08-07 17:34:54Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_HISTORY_H
#define PS_HISTORY_H

// std includes
#include <vector>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// txbase includes
#include <TxTensor.h>

// psbase includes
#include <PsHistoryBase.h>
#include <PsSerialize.h>
#include <PsFieldBase.h>

/**
 * A history contains a time series data.  Examples
 * are field energy, particle energy, fields at points, particle
 * positions etc.  At each time step the history is updated.
 * At dump time, the values from all processors are gathered together
 * and then written to the data file (runname)_History.h5
 *
 * @param FLOATTYPE the data type of simulation
 * @param NDIM the dimensionality of simulation
 * @param ELEMENTTYPE data type of elements in DATATYPE
 * @param DATATYPE data type history structure (eg scalar, TxTensor<>...)
 */
template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
class PsHistory : public virtual PsHistoryBase<FLOATTYPE, NDIM>,
                  public virtual PsSerialize<ELEMENTTYPE, DATATYPE> {

  public:

/**
 * Constructor
 */
    PsHistory() {}

/**
 * Destructor
 */
    virtual ~PsHistory() {}

/**
 * Build object
 */
    virtual void buildData();

/**
 * Build object
 */
    virtual void buildSolvers();

/**
 * Dump specific history data
 *
 * @param txIoPtr the IO object pointer
 * @param fnode file node opened by hldr object
 */
    virtual void dump(TxIoBase* txIoPtr, TxIoNodeType fnode);

  protected:

    /** Field base name */
    std::string fieldName;

    /** Pointer to a field, if single field this is the only pointer */
    PsFieldBase<FLOATTYPE>* fieldHistPtr;

    /** Time-series of history data, the "fundamental" data structure  */
    std::vector<DATATYPE> histData;

    /** Return size of the history data structure */
    size_t getHistDataLength() {
      return histData.size();
    }

    /** Push back history data to the histData std::vector */
    virtual void appendData(DATATYPE d);

    /** Erase data histData (ie after dumping to file) */
    virtual void clearData();

  private:

    /** Private copy constructor to prevent use */
    PsHistory(const PsHistory<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>& psh);

    /** Private assignment to prevent use */
    PsHistory<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>& operator=(
       const PsHistory<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>& psh);
};

#endif // PS_HISTORY_H
