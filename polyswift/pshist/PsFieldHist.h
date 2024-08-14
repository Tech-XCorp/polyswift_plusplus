/**
 * @file    PsFieldHist.h
 *
 * @brief   Base class for field diagnostics
 *
 * @version $Id: PsFieldHist.h 11989 2009-03-25 05:14:55Z dws $
 *
 * Copyright &copy; 2009-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_FIELD_HIST_H
#define PS_FIELD_HIST_H

// std includes
#include <vector>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// pshist includes
#include <PsCommHistory.h>

// psbase includes
#include <PsFieldBase.h>

/**
 * Holds references to PsFieldBase-s to be used in derived implementation classes.
 *
 * @param FLOATTYPE the data type of simulation
 * @param NDIM the dimensionality of simulation
 * @param ELEMENTTYPE data type of elements in DATATYPE
 * @param DATATYPE data type history structure (eg scalar, TxTensor<>...)
 */
template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
class PsFieldHist : public PsCommHistory<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE> {

  public:

/**
 * constructor
 */
    PsFieldHist();

/**
 * Destructor
 */
    virtual ~PsFieldHist();

/**
 * Sets lists of field pointers
 */
    virtual void buildSolvers();

  protected:

    /** Vector of field names */
    std::vector<std::string> fieldNames;

    /** Pointer to a field, if single field this is the only pointer */
    PsFieldBase<FLOATTYPE, NDIM>* fieldHistPtr;

  private:

    // Make private to prevent use
    PsFieldHist(const PsFieldHist<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>& vpfh);

    PsFieldHist<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>& operator=(
     const PsFieldHist<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>& vpfh);

};

#endif  // PS_FIELD_HIST_H
