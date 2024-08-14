/**
 * @file    PsGridBase.h
 *
 * @brief   Base interface for the grid
 *
 * @version $Id: PsGridBase.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_GRID_BASE_H
#define PS_GRID_BASE_H

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
#include <PsRandom.h>
#include <PsTinyVector.h>
#include <PsPhysConsts.h>

// psbase includes
#include <PsGridBaseItr.h>
#include <PsDynObj.h>
#include <PsDecompBase.h>

/**
 * A PsGridBase object contains base Grid methods interface
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 */

template <class FLOATTYPE, size_t NDIM>
class PsGridBase : public virtual PsDynObj<FLOATTYPE, NDIM>, public PsGridBaseItr {

  public:

/**
 * Constructor
 *
 */
  PsGridBase() {};

/**
 * Destructor
 */
  virtual ~PsGridBase() {};

/**
 * Set the parameters
 *
 * @param tas the parameters
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas) = 0;

/**
 * Build the data, such as updaters, parses input etc.
 */
    virtual void buildData() = 0;

/**
 * Build the solvers structures
 */
    virtual void buildSolvers() = 0;

/**
 * Get a reference to the decomposition. Access to decomp through
 * grid as they as strongly related to one another
 *
 * return a reference to the decomposition
 */
    virtual PsDecompBase<FLOATTYPE, NDIM>& getDecomp() = 0;

/**
 * Get number of physical cells in each global dimension
 *
 * @return vector of cell numbers
 */
    virtual std::vector<size_t> getNumCellsGlobal() = 0;

/**
 * Get number of physical cells in a direction
 *
 * @param indx index of direction
 */
    virtual size_t getNumCellsGlobalInDir(size_t indx) = 0;

/**
 * Get total global number of physical cells
 *
 * @return total number of cells
 */
    virtual size_t getTotalCellsGlobal() = 0;

/**
 * Get number of nodes in each global dimension
 * (default Cartesian)
 *
 * @return vector of nodes
 */
    virtual std::vector<size_t> getNumNodesGlobal() = 0;

/**
 * Get global real location of origin (hardwired to 0.0 for now)
 *
 * @return vector of real locations
 */
    virtual std::vector<FLOATTYPE> getOrigin() = 0;

    // ***************************

/**
 * Get location of center of global grid
 *
 * @return vector of cell numbers
 */
    virtual std::vector<int> getCenterGlobal() = 0;

/**
 * Get a random location in the global grid
 *
 * @return vector of cell indices
 */
    virtual std::vector<int> getRandomGlobalPt() = 0;

/**
 * Take a position in global grid and if outside bounds return
 * a vector after "folding" position back into domain for periodic
 * boundary conditions (PBC). If inside box... no change
 *
 * @param posvec position vector
 */
    virtual void mapPointToGrid(PsTinyVector<int, NDIM>& posvec) = 0;

/**
 * Take the distance between two points in the grid and
 * return the shortest distance given periodic
 * boundary conditions (PBC).
 *
 * @param v1 position vector
 * @param v2 position vector
 * @return distance between two points
 */
    virtual FLOATTYPE mapDistToGrid(PsTinyVector<int, NDIM>& v1,
                                    PsTinyVector<int, NDIM>& v2) = 0;

/**
 * Take a position in global grid and return associated local point
 * on this grid
 *
 * @param posvec position vector
 * @return local position
 */
    virtual PsTinyVector<int, NDIM>& mapToLocalVec(
                   PsTinyVector<int, NDIM>& posvec) = 0;

/**
 * Take a position in local grid and return associated global point
 *
 * @param posvec position vector
 * @return global position
 */
    virtual PsTinyVector<int, NDIM>& mapToGlobalVec(
                    PsTinyVector<int, NDIM>& posvec) = 0;

/**
 * Get size of cells in each dimension
 *
 * @return vector of cell sizes
 */
    virtual std::vector<FLOATTYPE> getCellSizes() = 0;

/**
 * Get global real lengths for each dimension of uniform Cartesian
 *
 * @return vector of lengths
 */
    virtual std::vector<FLOATTYPE> getGlobalLengths() = 0;

  protected:

  private:

    /** Make private to prevent use */
    PsGridBase(const PsGridBase<FLOATTYPE, NDIM>& vphh);

    /** Make private to prevent use */
    PsGridBase<FLOATTYPE, NDIM>& operator=(
        const PsGridBase<FLOATTYPE, NDIM>& vphh);
};

#endif // PS_GRID_BASE_H
