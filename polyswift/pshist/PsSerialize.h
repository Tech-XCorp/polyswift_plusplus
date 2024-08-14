/**
 * @file    PsSerialize.h
 *
 * @brief   Serializes data for sync/dump data methods
 *
 * @version $Id: PsSerialize.h   swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_SERIALIZE_H
#define PS_SERIALIZE_H

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// pshist includes
#include <PsSerializeBase.h>

/**
 * A PsSerialize serializes data for sync/dump data methods
 *
 * @param ELEMENTTYPE data type of elements in DATATYPE
 * @param DATATYPE data type history structure (eg scalar, TxTensor<>...)
 */
template <class ELEMENTTYPE, class DATATYPE>
  class PsSerialize : public virtual PsSerializeBase<ELEMENTTYPE> {

 public:

/**
 * Constructor
 */
    PsSerialize();

/**
 * Destructor
 */
    virtual ~PsSerialize() {}

 protected:

/**
 * Main method which sets pointer to serialized data
 *
 * @param data Reference to history data
 */
    virtual void serialize(std::vector<DATATYPE> &data);

};

/**
 * Specialized methods for purely scalar methods
 *
 * @param ELEMENTTYPE data type of elements in DATATYPE
 */
template <class ELEMENTTYPE>
class PsSerialize<ELEMENTTYPE, ELEMENTTYPE> : public virtual PsSerializeBase<ELEMENTTYPE> {

 public:

/**
 * Constructor
 */
  PsSerialize() {}

/**
 * Destructor
 */
    virtual ~PsSerialize() {}

/**
 * Main method which sets pointer to serialized data
 *
 * @param data Reference to scalar history data
 */
    virtual void serialize(std::vector<ELEMENTTYPE> &data);

};

#endif // PS_SERIALIZE_H
