/**
 * @file    PsCommHistory.h
 *
 * @brief   Base class for a diagnostic that performs aggregation
 *
 * @version $Id: PsCommHistory.h 11989 2009-03-25 05:14:55Z swsides $
 *
 * Copyright &copy; 2009-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_COMM_HISTORY_H
#define PS_COMM_HISTORY_H

// std includes

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// txbase includes

// pshist includes
#include <PsHistory.h>

/**
 * Gathers all history diagnostic data over processors and synchronizes the values.
 *
 * @param FLOATTYPE the data type of simulation
 * @param NDIM the dimensionality of simulation
 * @param ELEMENTTYPE data type of elements in DATATYPE
 * @param DATATYPE data type history structure (eg scalar, TxTensor<>...)
 */
template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
class PsCommHistory : public virtual PsHistory<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE> {

  public:

/**
 * Constructor
 */
    PsCommHistory() {
    }

/**
 * Destructor
 */
    virtual ~PsCommHistory() {
    }

/**
 * Gathers diagnostic information for all PsHistories
 */
    virtual void sync();

  protected:

  private:

/**
 * Private copy constructor prevents use
 */
    PsCommHistory(const PsCommHistory<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>& psch);

/**
 * Private assignment prevents use
 */
    PsCommHistory<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>& operator=(
     const PsCommHistory<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>& psch);

};

#endif // PS_COMM_HISTORY_H
