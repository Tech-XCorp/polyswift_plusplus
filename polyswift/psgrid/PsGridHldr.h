/**
 *
 * @file    PsGridHldr.h
 *
 * @brief   Base class owning instances of grid object(s)
 *
 * @version $Id: PsGridHldr.h 6532 2007-02-01 18:37:00Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_GRID_HLDR_H
#define PS_GRID_HLDR_H

// std includes
#include <vector>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsDynObj.h>
#include <PsGridBase.h>

/**
 * PsGridHldr contains a (vector) of pointer(s) to PsGrid object(s)
 *
 * @param FLOATTYPE the data type
 * @param NDIM the dimensionality
 */
template <class FLOATTYPE, size_t NDIM>
class PsGridHldr : public PsDynObj<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 */
    PsGridHldr() {
      this->setName("GridHdlr");
    }

/**
 * Destructor
 */
    virtual ~PsGridHldr() { }

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
 * Creates data
 */
    virtual void buildSolvers();

/**
 * Creates solvers
 */
    virtual void buildData(PsNamedObject* inOwner);

  protected:

    /** Number of Grid objects */
    size_t numGrids;

    /** Pointers to the base PsGrid objects */
    std::vector< PsGridBase<FLOATTYPE, NDIM>* > grids;

  private:

    /** List of grid names */
    std::vector< std::string > gridNames;

    /** List of attrib sets for the grids */
    std::vector< TxHierAttribSetIntDbl > gridAttribs;

    /** Make private to prevent use */
    PsGridHldr(const PsGridHldr<FLOATTYPE, NDIM>& vphh);

    /** Make private to prevent use */
    PsGridHldr<FLOATTYPE, NDIM>& operator=(
        const PsGridHldr<FLOATTYPE, NDIM>& vphh);

};

#endif // PS_GRID_HLDR_H
