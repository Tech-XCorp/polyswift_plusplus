/**
 *
 * @file    PsUniCartGrid.h
 *
 * @brief   Class containing info for uniform cartesian grid
 *
 * @version $Id: PsUniCartGrid.h 6319 2006-11-14 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_UNI_CART_GRID_H
#define PS_UNI_CART_GRID_H

// std includes
#include <vector>
#include <map>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psgrid includes
#include <PsGrid.h>

// psbase includes
#include <PsPhysField.h>

/**
 * Class for uniform cartesian grid
 *
 * @param FLOATTYPE the data type of simulation
 * @param NDIM the dimensionality of simulation
 */
template <class FLOATTYPE, size_t NDIM>
class PsUniCartGrid : public PsGrid<FLOATTYPE, NDIM> {

  public:

/**
 * constructor
 */
  PsUniCartGrid();

/**
 * Destructor
 */
  virtual ~PsUniCartGrid();

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
 * Get location of center of global grid
 *
 * @return vector of cell numbers
 */
    virtual std::vector<int> getCenterGlobal();

/**
 * Get a random location in the global grid
 *
 * @return vector of cell indices
 */
    virtual std::vector<int> getRandomGlobalPt();

/**
 * Take a position in global grid and if outside bounds return
 * a vector after "folding" position back into domain for periodic
 * boundary conditions (PBC). If inside box... no change
 *
 * @param posvec position vector
 */
    virtual void mapPointToGrid(PsTinyVector<int, NDIM>& posvec);

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
        PsTinyVector<int, NDIM>& v2);

/**
 * Take a position in global grid and return associated local point
 * on this grid
 *
 * @param posvec position vector
 * @return local position
 */
    virtual PsTinyVector<int, NDIM>& mapToLocalVec(PsTinyVector<int, NDIM>& posvec);

/**
 * Take a position in local grid and return associated global point
 *
 * @param posvec position vector
 * @return global position
 */
    virtual PsTinyVector<int, NDIM>& mapToGlobalVec(PsTinyVector<int, NDIM>& posvec);

/**
 * Get size of cells in each dimension for uniform Cartesian
 *
 * @return vector of cell sizes
 */
    virtual std::vector<FLOATTYPE> getCellSizes() {
      return cellSizes;
    }

/**
 * Get global real lengths for each dimension of uniform Cartesian
 *
 * @return vector of lengths
 */
    virtual std::vector<FLOATTYPE> getGlobalLengths();

  protected:

    /** Uniform size of cell in x,y,z directions (dx,dy,dz) */
    std::vector<FLOATTYPE> cellSizes;

  private:

    /** Local return result holder */
    PsTinyVector<int, NDIM> localVec;

    /** Constructor private to prevent use */
    PsUniCartGrid(const PsUniCartGrid<FLOATTYPE, NDIM>& psbcp);

   /** Assignment private to prevent use */
    PsUniCartGrid<FLOATTYPE, NDIM>& operator=(
      const PsUniCartGrid<FLOATTYPE, NDIM>& psbcp);
};

#endif // PS_UNI_CART_GRID_H
