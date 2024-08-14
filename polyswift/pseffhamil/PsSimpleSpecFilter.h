/**
 *
 * @file    PsSimpleSpecFilter.h
 *
 * @brief   Class for updating fields with spectral filter algorithm
 *           with single cutoff
 *
 * @version $Id: PsSimpleSpecFilter.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2010-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_SIMPLE_SPEC_FILTER_H
#define PS_SIMPLE_SPEC_FILTER_H

// standard headers
#include <list>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#undef HAVE_CONFIG_H
#endif

// txbase headers
#include <TxDebugExcept.h>

// psbase includes
#include <PsPhysField.h>
#include <PsFieldBase.h>
#include <PsFFTBase.h>

// pseffhamil includes
#include <PsSpecFilterUpdater.h>

/**
 * A PsSimpleSpecFilter object updates fields using spectral filter algorithm
 * using a single cutoff
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 */

template <class FLOATTYPE, size_t NDIM>
class PsSimpleSpecFilter : public virtual PsSpecFilterUpdater<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 *
 */
    PsSimpleSpecFilter();

/**
 * Destructor must destroy blocks
 */
    virtual ~PsSimpleSpecFilter();

/**
 * Set the parameters
 *
 * @param tas the parameters
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Build the data
 */
    virtual void buildData();

/**
 * Build the solvers structures
 */
    virtual void buildSolvers();

/**
 * Initialize object
 */
    virtual void initialize();

/**
 * Update the updater
 *
 * @param t the time
 */
    virtual void update(double t);

  protected:

  private:

    /** Helper for update to filter */
    void applyFilter(PsFieldBase<FLOATTYPE>& wField);

    /** Constructor private to prevent use */
    PsSimpleSpecFilter(const PsSimpleSpecFilter<FLOATTYPE, NDIM>& psb);

   /** Assignment private to prevent use */
    PsSimpleSpecFilter<FLOATTYPE, NDIM>& operator=(
       const PsSimpleSpecFilter<FLOATTYPE, NDIM>& psb);
};

#endif  // PS_SIMPLE_SPEC_FILTER_H
