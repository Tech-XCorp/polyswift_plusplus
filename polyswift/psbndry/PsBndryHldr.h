/**
 * @file    PsBndryHldr.h
 *
 * @brief   Base class owning instances of boundary object(s)
 *
 * @version $Id: PsBndryHldr.h 6532 2007-02-01 18:37:00Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_BNDRY_HLDR_H
#define PS_BNDRY_HLDR_H

// std includes
#include <vector>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsDynObj.h>
#include <PsBndryBase.h>

/**
 * PsBndryHldr contains a (vector) of pointer(s) to PsBndry object(s)
 *
 * @param FLOATTYPE the data type
 * @param NDIM the dimensionality
 */
template <class FLOATTYPE, size_t NDIM>
class PsBndryHldr : public PsDynObj<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 */
    PsBndryHldr() {
      this->setName("BndryHdlr");
    }

/**
 * Destructor
 */
    virtual ~PsBndryHldr() {
    }

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
 * Finishes object
 */
    virtual void buildSolvers();

/**
 * Creates object
 */
    virtual void buildData(PsNamedObject* inOwner);

/**
 * Initialize boundaries
 */
    virtual void initialize();

/**
 * Updates all PsBndry-s
 *
 * @param t simulation time
 */
    virtual void update(double t);

/**
 * Dump all PsBndry-s
 *
 * @param txIoPtr IO object pointer
 */
    virtual void dump(TxIoBase* txIoPtr);

/**
 * Restores boundary info
 *
 * @param txIoPtr IO object pointer
 */
    virtual void restore(TxIoBase* txIoPtr);

  protected:

    /** Number of Bndry objects */
    size_t numBoundaries;

    /** Pointers to the base PsBndry objects */
    std::vector< PsBndryBase<FLOATTYPE, NDIM>* > boundaries;

  private:

    // List of grid names
    std::vector< std::string > boundaryNames;

    // List of attrib sets for the grids
    std::vector< TxHierAttribSetIntDbl > boundaryAttribs;

    // Iterator for boundary list
    typename std::vector< PsBndryBase<FLOATTYPE, NDIM>* >::iterator ibndry;

    /** Make private to prevent use */
    PsBndryHldr(const PsBndryHldr<FLOATTYPE, NDIM>& vphh);

    /** Make private to prevent use */
    PsBndryHldr<FLOATTYPE, NDIM>& operator=(
        const PsBndryHldr<FLOATTYPE, NDIM>& vphh);

};

#endif // PS_BNDRY_HLDR_H
