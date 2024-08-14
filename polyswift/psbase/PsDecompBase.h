/**
 * @file    PsDecompBase.h
 *
 * @brief   Base interface for decompositions
 *
 * @version $Id: PsDecompBase.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_DECOMP_BASE_H
#define PS_DECOMP_BASE_H

// standard headers
#include <set>
#include <vector>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#undef HAVE_CONFIG_H
#endif

// txbase headers
#include <TxDebugExcept.h>

// psstd includes
#include <PsTinyVector.h>

// psbase includes
#include <PsDynObj.h>

/**
 * A PsDecompBase object contains base Decomp methods interface
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 */
template <class FLOATTYPE, size_t NDIM>
class PsDecompBase : public virtual PsDynObj<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 */
    PsDecompBase();

/**
 * Destructor
 */
    virtual ~PsDecompBase();

/**
 * Set the parameters
 *
 * @param tas the parameters
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Single build method for special decomp object
 */
    virtual void build() = 0;

/**
 * Get number of physical cells in each local domain dimension
 *
 * @return vector of cell numbers
 */
    virtual std::vector<size_t> getNumCellsLocal() = 0;

/**
 * Get total local number of physical cells
 *
 * @return total number of cells
 */
    virtual size_t getTotalCellsLocal() = 0;

/**
 * Get shift values to map local indices to global indices
 *
 * @return vector of local to global shifts
 */
    virtual std::vector<size_t> getLocalToGlobalShifts() = 0;

/**
 * Check if decomp owned point specified in globalPos
 *
 * @param globalPos vector of global position
 */
    virtual bool hasPosition(std::vector<int> globalPos) = 0;

/**
 * Check if decomp owned point specified in globalPos
 *
 * @param globalPos PsTinyVector of global position
 */
    virtual bool hasPosition(PsTinyVector<int, NDIM> globalPos) = 0;

  protected:

  private:

    /** Make private to prevent use */
    PsDecompBase(const PsDecompBase<FLOATTYPE, NDIM>& vphh);

    /** Make private to prevent use */
    PsDecompBase<FLOATTYPE, NDIM>& operator=(
        const PsDecompBase<FLOATTYPE, NDIM>& vphh);
};

#endif // PS_DECOMP_BASE_H
