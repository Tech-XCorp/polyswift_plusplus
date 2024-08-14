/**
 *
 * @file    PsCommHldr.h
 *
 * @brief   Base class owning instances of input/output object(s)
 *
 * @version $Id: PsCommHldr.h 6532 2007-02-01 18:37:00Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_COMM_HLDR_H
#define PS_COMM_HLDR_H

// std includes
#include <vector>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsDynObj.h>
#include <PsCommBase.h>

/**
 * PsCommHldr contains a (vector) of pointer(s) to PsComm object(s)
 *
 * @param FLOATTYPE the data type
 * @param NDIM the dimensionality
 */
template <class FLOATTYPE, size_t NDIM>
class PsCommHldr : public PsDynObj<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 */
    PsCommHldr() {
      this->setName("CommHdlr");
    }

/**
 * Destructor
 */
    virtual ~PsCommHldr() {}

/**
 * Set from an attribute set.
 *
 * Stores attribute set and sets name.  By storing the settings
 * in the base class, any derived class has the information it
 * needs to rebuild itself.
 *
 * @param tas the attribute set
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Build object... single build method
 */
    virtual void build(PsNamedObject* inOwner);

/**
 * Dump all PsComm objects
 */
    virtual void dump();

  protected:

    /** Number of Comm objects */
    size_t numComms;

    /** Pointers to the base PsComm objects */
    std::vector< PsCommBase<FLOATTYPE, NDIM>* > comms;

  private:

    /** List of io names */
    std::vector< std::string > commNames;

    /** List of attrib sets for the ios */
    std::vector< TxHierAttribSetIntDbl > commAttribs;

    /** Make private to prevent use */
    PsCommHldr(const PsCommHldr<FLOATTYPE, NDIM>& vphh);

    /** Make private to prevent use */
    PsCommHldr<FLOATTYPE, NDIM>& operator=(
        const PsCommHldr<FLOATTYPE, NDIM>& vphh);

};

#endif // PS_COMM_HLDR_H
