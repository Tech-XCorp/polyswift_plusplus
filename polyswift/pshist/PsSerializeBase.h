/**
 * @file    PsSerializeBase.h
 *
 * @brief   Base class for serializing data for sync/dump data methods
 *
 * @version $Id: PsSerializeBase.h   swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_SERIALIZE_BASE_H
#define PS_SERIALIZE_BASE_H

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// txbase includes
#include <vector>
#include <TxDebugExcept.h>

/**
 * A PsSerializeBase serializes data for sync/dump data methods
 *
 * @param ELEMENTTYPE data type of elements in DATATYPE
 */
template <class ELEMENTTYPE>
  class PsSerializeBase {

 public:

/**
 * Constructor
 */
    PsSerializeBase() {
      serialPtrSize = 0;
      serialPtr = 0;
      isIntType = 0;
    }

/**
 * Destructor
 */
    virtual ~PsSerializeBase() {
      delete[] serialPtr;
      serialShape.clear();
    }

/**
 * ELEMENTTYPE default is for NOT integer
 * If ELEMENTTYPE = int needed this method sets integer flag
 */
    virtual void setIntType() {
      isIntType = 1;
    }

 protected:

/** Pointer to serialized data */
    ELEMENTTYPE* serialPtr;

/** Number of elements serialPtr accesses */
    size_t serialPtrSize;

/**
 * Information about shape (ie rank, size) of DATATYPE object
 * to be serialized. First dimension of this data always will
 * correspond to time.
 */
    std::vector<size_t> serialShape;

/**
 * Flag for serialized data type
 *  0 -- not int,  1 -- int
 */
    bool isIntType;

};

#endif // PS_SERIALIZE_BASE_H
