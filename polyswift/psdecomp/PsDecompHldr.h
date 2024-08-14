/**
 *
 * @file    PsDecompHldr.h
 *
 * @brief   Base class owning instances of decomp object(s)
 *
 * @version $Id: PsDecompHldr.h 6532 2007-02-01 18:37:00Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_DECOMP_HLDR_H
#define PS_DECOMP_HLDR_H

// std includes
#include <vector>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsDynObj.h>
#include <PsDecompBase.h>

/**
 * PsDecompHldr contains a (vector) of pointer(s) to PsDecomp object(s)
 *
 * @param FLOATTYPE the data type
 * @param NDIM the dimensionality
 */
template <class FLOATTYPE, size_t NDIM>
class PsDecompHldr : public PsDynObj<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 */
    PsDecompHldr() {
      this->setName("DecompHdlr");
    }

/**
 * Destructor
 */
    virtual ~PsDecompHldr() { }

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
 * Single build method for special decomp object
 */
    virtual void build(PsNamedObject* inOwner);

  protected:

    /** Number of Decomp objects */
    size_t numDecomps;

    /** Pointers to the base PsDecomp objects */
    std::vector< PsDecompBase<FLOATTYPE, NDIM>* > decomps;

  private:

    /** List of decomp names */
    std::vector< std::string > decompNames;

    /** List of attrib sets for the decomps */
    std::vector< TxHierAttribSetIntDbl > decompAttribs;

    /** Make private to prevent use */
    PsDecompHldr(const PsDecompHldr<FLOATTYPE, NDIM>& vphh);

    /** Make private to prevent use */
    PsDecompHldr<FLOATTYPE, NDIM>& operator=(
        const PsDecompHldr<FLOATTYPE, NDIM>& vphh);

};

#endif // PS_DECOMP_HLDR_H
