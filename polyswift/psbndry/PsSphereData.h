/**
 * @file    PsSphereData.h
 *
 * @brief   Class containing field data structures for
 *          spherical nanoparticles
 *
 * @version $Id: PsSphereData.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_SPHERE_DATA_H
#define PS_SPHERE_DATA_H

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

// psbndry includes
#include <PsWallData.h>
#include <PsBndryData.h>

/**
 * A PsSphereData object contains the spatial data specific to
 * a nanoparticle (ie. center, radius...)
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 */
template <class FLOATTYPE, size_t NDIM>
class PsSphereData : public virtual PsBndryData<FLOATTYPE, NDIM> {

  public:

  // SWS: make common somewhere?
  /** Type for tiny vector of position */
  typedef PsTinyVector<int, NDIM>  posVecType;

/**
 * Constructor
 *
 */
    PsSphereData();

/**
 * Destructor must destroy nanoptcls
 */
    virtual ~PsSphereData();

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
 * Get the particle center value
 *
 * @return global center of particle
 */
    virtual const PsTinyVector<int, NDIM>& getCenter() const {
      return center;
    }

/**
 * Set the particle center value
 *
 * @param cen global center of particle
 */
    virtual void setCenter(posVecType& cen) {
      center = cen;
    }

/**
 * Get the particle radius
 *
 * @return particle radius
 */
    virtual FLOATTYPE getRadius() const {
      return radius;
    }

/**
 * Get the particle radius at which approach is disallowed
 *
 * @return particle radius
 */
    virtual FLOATTYPE getDynamicRadius() const {
      return dynRadius;
    }

/**
 * Set the particle radius
 *
 * @param r particle radius
 */
    virtual void setRadius(FLOATTYPE r) { radius = r; }

/**
 * Set the particle radius
 *
 * @param r particle radius
 */
    virtual void setDynamicRadius(FLOATTYPE r) { dynRadius = r; }

/**
 * Set the particle displacement
 *
 * @param idr particle displacement (on grid)
 */
    virtual void setDr(posVecType& idr) { dr = idr; }

/**
 * Get the particle displacement
 *
 * @return particle displacement (on grid)
 */
    virtual const posVecType& getDr() const {
      return dr;
    }

/**
 * Check if this particle is in contact with particle in argument list
 *
 * @param bndry pointer to base boundary data interface
 * @return boolean for in contact
 */
    virtual bool inContact(PsBndryDataBase<FLOATTYPE, NDIM>* bndry) {
      //      std::cout << "PsSphereData interface inContact" << std::endl;
      bool ic = bndry->inContact(this);
      return ic;
    }

/**
 * Check if this particle is in contact with a spherical particle
 * This method implements actual center distance check
 *
 * @param spherePtcl pointer to particle data interface
 * @return bool for in contact
 */
    virtual bool inContact(PsSphereData<FLOATTYPE, NDIM>* spherePtcl);

/**
 * Check if this particle is in contact with another boundary
 * This method implements distance check.
 *
 * When walls must check contact with a sphere...this method is
 * called.
 *
 * @param wallBndry pointer to data interface
 * @return bool for in contact
 */
    virtual bool inContact(PsWallData<FLOATTYPE, NDIM>* wallBndry);

  protected:

    /** Center of particle */
    posVecType center;

    /** Displacement for center, pure translation */
    posVecType dr;

    /** Radius of particle */
    FLOATTYPE radius;

    /** Radius of particle */
    FLOATTYPE dynRadius;

  private:

    /** Make private to prevent use */
    PsSphereData(const PsSphereData<FLOATTYPE, NDIM>& vphh);

    /** Make private to prevent use */
    PsSphereData<FLOATTYPE, NDIM>& operator=(
      const PsSphereData<FLOATTYPE, NDIM>& vphh);

};

#endif  // PS_SPHERE_DATA_H
