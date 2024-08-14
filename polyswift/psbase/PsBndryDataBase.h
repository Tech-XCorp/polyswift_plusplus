/**
 *
 * @file    PsBndryDataBase.h
 *
 * @brief   Base class interface for boundary data
 *
 * @version $Id: PsBndryDataBase.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_BNDRY_DATA_BASE_H
#define PS_BNDRY_DATA_BASE_H

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

// psbase includes and forward references
#include <PsDynObj.h>
#include <PsGridField.h>
template <class FLOATTYPE, size_t NDIM> class PsSphereData;
template <class FLOATTYPE, size_t NDIM> class PsWallData;

/**
 * A PsBndryDataBase object contains the spatial part boundary data.
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 */

template <class FLOATTYPE, size_t NDIM>
class PsBndryDataBase : public virtual PsDynObj<FLOATTYPE, NDIM> {

  public:

  /** Type for tiny vector of position */
  typedef PsTinyVector<int, NDIM> posVecType;

/**
 * Constructor
 *
 */
    PsBndryDataBase();

/**
 * Destructor must destroy boundary data
 */
    virtual ~PsBndryDataBase();

//
// Accessors
//

/**
 * Set the parameters
 *
 * @param tas the parameters
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas) = 0;

/**
 * Build the data
 */
    virtual void buildData() = 0;

/**
 * Build the solvers
 */
    virtual void buildSolvers() = 0;

/**
 * Access the number of field elements comprising boundary
 *
 * @return number of field elements (length of field vector)
 */
    virtual size_t getNumFieldElements() = 0;

/**
 * Set the number of field elements comprising boundary
 *
 * @param s number of field elements (length of field vector)
 */
    virtual void setNumFieldElements(size_t s) = 0;

/**
 * Access location of boundary field element
 *
 * @param  index  index of the position vector list
 * @return vector of field position
 */
    virtual PsTinyVector<int, NDIM>& getFieldPos(size_t index) = 0;

/**
 * Set location of boundary field element
 *
 * @param vec   vector of field position
 * @param index index of the position vector list
 */
    virtual void setFieldPos(posVecType& vec, size_t index) = 0;

/**
 * Set field position vector in list. This adds (push back) to
 * the list because this should only happen at build
 *
 * @param posVec position vector
 */
    virtual void addFieldPos(posVecType& posVec) = 0;

/**
 * Set field value in list. This adds (push back) to
 * the list because this should only happen at build
 *
 * @param fval field value
 */
    virtual void addFieldVal(FLOATTYPE fval) = 0;

/**
 * Access value of nanoparticle field element
 *
 * @param  index  index of the position vector list
 * @return value of field at index location
 */
    virtual FLOATTYPE& getFieldVal(size_t index) = 0;

/**
 * Set the boundary index as a label
 *
 * @param idx boundary index
 */
    virtual void setBndryIndex(int idx) = 0;

/**
 * Get the boundary index as a label
 *
 * @return boundary index
 */
    virtual int getBndryIndex() const = 0;

/**
 * Get the entire field positions vector
 *
 * @return std::vector for entire field positions
 */
    virtual std::vector<posVecType> getFieldPos() = 0;

/**
 * Get the entire field value vector
 *
 * @return std::vector for entire field values
 */
    virtual std::vector<FLOATTYPE> getFieldVals() = 0;

/**
 * Set base particle data from another PsBndryDataBase object
 *
 * @param bndry reference to a PsBndryDataBase
 */
    virtual void setBndryData(PsBndryDataBase<FLOATTYPE, NDIM>& bndry) = 0;

/**
 * Interface function to check if this boundary is in contact
 * with any other boundary
 *
 * @param bndry pointer to data interface
 * @return      boolean for in contact
 */
    virtual bool inContact(PsBndryDataBase<FLOATTYPE, NDIM>* bndry) = 0;

/**
 * Check if this boundary is contact with a spherical particle
 *
 * @param spherePtcl pointer to particle data interface
 * @return boolean for in contact
 */
    virtual bool inContact(PsSphereData<FLOATTYPE, NDIM>* spherePtcl) = 0;

/**
 * Check this boundary is contact with another boundary
 *
 * @param wallBndry pointer to particle data interface
 * @return          boolean for in contact
 */
    virtual bool inContact(PsWallData<FLOATTYPE, NDIM>* wallBndry) = 0;

/**
 * Set debug flag
 */
    virtual void setDebugFlag(DebugPrint db) {
      this->dbStatus = db;
    }

/**
 * Increment move counter
 */
    virtual void bumpMoveAttempts() = 0;

/**
 * Set move counter to zero
 */
    virtual void resetMoveAttempts() = 0;

/**
 * Set move counter to zero
 */
    virtual size_t getMoveAttempts() = 0;

  protected:

  private:

    /** Make private to prevent use */
    PsBndryDataBase(const PsBndryDataBase<FLOATTYPE, NDIM>& vphh);

    /** Make private to prevent use */
    PsBndryDataBase<FLOATTYPE, NDIM>& operator=(
        const PsBndryDataBase<FLOATTYPE, NDIM>& vphh);

};

#endif  // PS_BNDRY_DATA_BASE_H
