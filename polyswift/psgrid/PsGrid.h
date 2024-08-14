/**
 * @file    PsGrid.h
 *
 * @brief   Base class for grid
 *
 * @version $Id: PsGrid.h 6319 2006-11-14 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_GRID_H
#define PS_GRID_H

// std includes
#include <vector>
#include <map>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsGridBase.h>

/**
 * Base class for grid
 *
 * @param FLOATTYPE the data type of simulation
 * @param NDIM the dimensionality of simulation
 */
template <class FLOATTYPE, size_t NDIM>
class PsGrid : public PsGridBase<FLOATTYPE, NDIM> {

  public:

/**
 * constructor
 */
  PsGrid();

/**
 * Destructor
 */
  virtual ~PsGrid();

/**
 * Store the data needed to build this object.
 *
 * @param tas the attribute set containing the initial conditions
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Build the data for this object
 */
    virtual void buildData();

/**
 * Build the solvers for this object
 */
    virtual void buildSolvers();

/**
 * Get a reference to the decomposition. Access to decomp through
 * grid as they as strongly related to one another
 *
 * return a reference to the decomposition
 */
     virtual PsDecompBase<FLOATTYPE, NDIM>& getDecomp() {
       return *decompObjPtr;
     }

/**
 * Get number of physical cells in each global dimension
 *
 * @return vector of cell numbers
 */
    virtual std::vector<size_t> getNumCellsGlobal() {
      return numCellsGlobal;
    }

/**
 * Get number of physical cells in a direction
 *
 * @param indx index of direction
 */
    virtual size_t getNumCellsGlobalInDir(size_t indx) {
      return numCellsGlobal[indx];
    }

/**
 * Get total global number of physical cells (top-level)
 *
 * @return total number of cells
 */
    virtual size_t getTotalCellsGlobal();

/**
 * Get number of nodes in each global dimension
 * (default Cartesian)
 *
 * @return vector of nodes
 */
    virtual std::vector<size_t> getNumNodesGlobal();

/**
 * Get global real location of origin (hardwired to 0.0 for now)
 *
 * @return vector of real locations
 */
    virtual std::vector<FLOATTYPE> getOrigin();

// ****************************************************************
//     This section is for methods that are implemented
//     in only some derived classes. Provides a single
//     place to provide error catching implementations.
// ****************************************************************

/**
 * Get location of center of global grid
 *
 * @return vector of cell numbers
 */
    virtual std::vector<int> getCenterGlobal() {
      TxDebugExcept tde("PsGrid::getCenterGlobal() not implemented");
      throw tde;
    }

/**
 * Get a random location in the global grid
 *
 * @return vector of cell indices
 */
    virtual std::vector<int> getRandomGlobalPt() {
      TxDebugExcept tde("PsGrid::getRandomGlobalPt() not implemented");
      throw tde;
    }

/**
 * Take a position in global grid and if outside bounds return
 * a vector after "folding" position back into domain for periodic
 * boundary conditions (PBC). If inside box... no change
 *
 * @param posvec position vector
 */
    virtual void mapPointToGrid(PsTinyVector<int, NDIM>& posvec) {
      TxDebugExcept tde("PsGrid::mapPointToGrid() not implemented");
      throw tde;
    }

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
                                    PsTinyVector<int, NDIM>& v2) {
      TxDebugExcept tde("PsGrid::mapDistToGrid() not implemented");
      throw tde;
    }

/**
 * Take a position in global grid and return associated local point
 * on this grid
 *
 * @param posvec position vector
 * @return local position
 */
    virtual PsTinyVector<int, NDIM>& mapToLocalVec(PsTinyVector<int, NDIM>& posvec) {
      TxDebugExcept tde("PsGrid::mapToLocalVec() not implemented");
      throw tde;
    }

/**
 * Take a position in local grid and return associated global point
 *
 * @param posvec position vector
 * @return global position
 */
    virtual PsTinyVector<int, NDIM>& mapToGlobalVec(PsTinyVector<int, NDIM>& posvec) {
      TxDebugExcept tde("PsGrid::mapToGlobalVec() not implemented");
      throw tde;
    }

/**
 * Get size of cells in each dimension
 *
 * @return vector of cell sizes
 */
    virtual std::vector<FLOATTYPE> getCellSizes() {
      TxDebugExcept tde("PsGrid::getCellSizes() not implemented");
      throw tde;
    }

/**
 * Get global real lengths for each dimension of uniform Cartesian
 *
 * @return vector of lengths
 */
    virtual std::vector<FLOATTYPE> getGlobalLengths() {
      TxDebugExcept tde("PsGrid::getGlobalLengths() not implemented");
      throw tde;
    }

  protected:

    /** Number of physical cells in each global dimension */
    std::vector<size_t> numCellsGlobal;

    /** Decomp object pointer */
    PsDecompBase<FLOATTYPE, NDIM>* decompObjPtr;

  private:

    /** Name of decomposition object */
    std::string decompName;

    /** Constructor private to prevent use */
    PsGrid(const PsGrid<FLOATTYPE, NDIM>& psbcp);

   /** Assignment private to prevent use */
    PsGrid<FLOATTYPE, NDIM>& operator=(
      const PsGrid<FLOATTYPE, NDIM>& psbcp);

};

#endif // PS_GRID_H
