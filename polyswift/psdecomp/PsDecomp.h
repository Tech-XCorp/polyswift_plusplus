/**
 *
 * @file    PsDecomp.h
 *
 * @brief   Base class for decomposition
 *
 * @version $Id: PsDecomp.h 6319 2006-11-14 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_DECOMP_H
#define PS_DECOMP_H

// std includes
#include <vector>
#include <map>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsDecompBase.h>

/**
 * Base class for decomposition
 *
 * @param FLOATTYPE the data type of simulation
 * @param NDIM the dimensionality of simulation
 */
template <class FLOATTYPE, size_t NDIM>
class PsDecomp : public PsDecompBase<FLOATTYPE, NDIM> {

  public:

/**
 * constructor
 */
  PsDecomp();

/**
 * Destructor
 */
  virtual ~PsDecomp();

/**
 * Store the data needed to build this object.
 *
 * @param tas the attribute set containing the initial conditions
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Build the data for this object
 */
    virtual void build();

/**
 * Get number of physical cells in each local dimension
 *
 * @return vector of cell numbers
 */
    virtual std::vector<size_t> getNumCellsLocal() {
      return numCellsLocal;
    }

/**
 * Get total local number of physical cells
 *
 * @return total number of cells
 */
    virtual size_t getTotalCellsLocal() {
      size_t totalCells = 1;
      for (size_t i=0; i<numCellsLocal.size(); ++i) {
        totalCells = totalCells * numCellsLocal[i];
      }
      return totalCells;
    }

  protected:

    /** Number of physical cells in each local dimension */
    std::vector<size_t> numCellsLocal;

  private:

    /** Constructor private to prevent use */
    PsDecomp(const PsDecomp<FLOATTYPE, NDIM>& psbcp);

   /** Assignment private to prevent use */
    PsDecomp<FLOATTYPE, NDIM>& operator=(
      const PsDecomp<FLOATTYPE, NDIM>& psbcp);

};

#endif // PS_DECOMP_H
