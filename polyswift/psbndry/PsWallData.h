/**
 * @file    PsWallData.h
 *
 * @brief   Class containing field data for a wall/surface
 *
 * @version $Id: PsWallData.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2009-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_WALL_DATA_H
#define PS_WALL_DATA_H

// standard headers
#include <set>
#include <vector>
#include <map>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#undef HAVE_CONFIG_H
#endif

// txbase headers
#include <TxDebugExcept.h>

// psstd includes
#include <PsSTFunc.h>
#include <PsTinyVector.h>

// psbase includes
#include <PsDynObj.h>

// psptcl includes
#include <PsBndryData.h>
#include <PsSphereData.h>

/**
 * A PsWallData object contains the spatial data specific to
 * an arbitrary boundary... (eg walls)
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 */

template <class FLOATTYPE, size_t NDIM>
class PsWallData : public virtual PsBndryData<FLOATTYPE, NDIM> {

  public:

  /** Type for tiny vector of position */
  typedef PsTinyVector<int, NDIM>  posVecType;

/**
 * Constructor
 *
 */
    PsWallData();

/**
 * Destructor must destroy nanoptcls
 */
    virtual ~PsWallData();

//
// Accessors
//

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
 * Check if this arbitrary boundary is in contact with
 *  another boundary
 *
 * @param bndry pointer to base boundary data interface
 * @return boolean for in contact
 */
    virtual bool inContact(PsBndryDataBase<FLOATTYPE, NDIM>* bndry) {
      bool ic = bndry->inContact(this);
      return ic;
    }

/**
 * Check if this boundary is in contact with another boundary particle
 * This method implements distance check
 *
 * @param wallBndry pointer to data interface
 * @return bool for in contact
 */
    virtual bool inContact(PsWallData<FLOATTYPE, NDIM>* wallBndry);

/**
 * Check if this boundary is in contact with a spherical particle
 * Note: makes use of the spherical particle class check for walls
 *
 * @param spherePtcl pointer to particle data interface
 * @return bool for in contact
 */
    virtual bool inContact(PsSphereData<FLOATTYPE, NDIM>* spherePtcl) {
      bool inCont=false;
      inCont = spherePtcl->inContact(this);
      return inCont;
    }

/**
 * Get wall inside points
 *
 * @return vector of points inside wall
 */
    virtual std::vector<posVecType> getWallInside() {
      return wallInsidePts;
    }

/**
 * Set wall edge points
 *
 * @param weVec vector of positions for wall edge.
 */
    virtual void setWallInside(std::vector<posVecType>& wVec) {
      wallInsidePts = wVec;
    }

/**
 * Get local bndry wall points
 *
 * @return vector of points inside wall
 */
    virtual std::vector<posVecType> getLocalWall() {
      return localBndryPts;
    }

/**
 * Set local bndry wall points
 *
 * @param lwVec vector of local positions for wall edge.
 */
    virtual void setLocalWall(std::vector<posVecType>& lwVec) {
      localBndryPts = lwVec;
    }

  protected:

    /** List of points belonging to inside of wall (global) */
    std::vector<posVecType> wallInsidePts;

    /** List of points belonging to inside of wall (local) */
    std::vector<posVecType> localBndryPts;

  private:

    /** Make private to prevent use */
    PsWallData(const PsWallData<FLOATTYPE, NDIM>& vphh);

    /** Make private to prevent use */
    PsWallData<FLOATTYPE, NDIM>& operator=(
      const PsWallData<FLOATTYPE, NDIM>& vphh);
};

#endif  // PS_WALL_DATA_H
