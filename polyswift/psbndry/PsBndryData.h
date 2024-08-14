/**
 * @file    PsBndryData.h
 *
 * @brief   Intermediate base class for boundary data
 *
 * @version $Id: PsBndryData.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2009-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_BNDRY_DATA_H
#define PS_BNDRY_DATA_H

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
#include <PsGridField.h>
#include <PsGridBase.h>
#include <PsCommBase.h>
#include <PsBndryDataBase.h>

template <class FLOATTYPE, size_t NDIM> class PsBndryDataBase;

/**
 * A PsBndryData object contains the fundamental spatial field
 * data for any nanoparticle
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 */
template <class FLOATTYPE, size_t NDIM>
class PsBndryData : public PsBndryDataBase<FLOATTYPE, NDIM> {

  public:

  /** Type for tiny vector of position */
  typedef PsTinyVector<int, NDIM> posVecType;

/**
 * Constructor
 *
 */
    PsBndryData();

/**
 * Destructor must destroy nanoptcls
 */
    virtual ~PsBndryData();

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
 * Build the solvers
 */
    virtual void buildSolvers();

/**
 * Set base particle data from another PsBndryData object
 *
 * @param bdata reference to a PsBndryDataBase interface
 */
    virtual void setBndryData(PsBndryDataBase<FLOATTYPE, NDIM>& bdata);

/**
 * Access the number of field elements comprising nanoparticle
 *
 * @return number of field elements (length of field vector)
 */
    virtual size_t getNumFieldElements() { return numFieldElements; }

/**
 * Set the number of field elements comprising nanoparticle
 *
 * @param s number of field elements (length of field vector)
 */
    virtual void setNumFieldElements(size_t s) { numFieldElements = s; }

/**
 * Access location of nanoparticle field element
 *
 * @param  index  index of the position vector list
 * @return vector of field position
 */
    virtual PsTinyVector<int, NDIM>& getFieldPos(size_t index) {
      return fieldPos[index];
    }

/**
 * Set location of nanoparticle field element
 *
 * @param vec   vector of field position
 * @param index index of the position vector list
 */
    virtual void setFieldPos(posVecType& vec, size_t index) {
      fieldPos[index] = vec;
    }

/**
 * Set field position vector and value in list,
 * should only happen at build. This automatically tracks number of members
 *
 * @param posVec position vector
 * @param val    field value at posVec
 */
    virtual void addFieldData(posVecType& posVec, FLOATTYPE val) {
      fieldPos.push_back(posVec);
      fieldVals.push_back(val);
      ++numFieldElements;
    }

/**
 * Set field position vector in list. This adds (push back) to
 * the list because this should only happen at build
 *
 * @param posVec position vector
 */
    virtual void addFieldPos(posVecType& posVec) {
      fieldPos.push_back(posVec);
    }

/**
 * Set field value in list. This adds (push back) to
 * the list because this should only happen at build
 *
 * @param fval field value
 */
    virtual void addFieldVal(FLOATTYPE fval) {
      fieldVals.push_back(fval);
    }

/**
 * Access value of nanoparticle field element
 *
 * @param  index  index of the position vector list
 * @return value of field at index location
 */
    virtual FLOATTYPE& getFieldVal(size_t index) {
      return fieldVals[index];
    }

/**
 * Set the boundary index as a label
 *
 * @param idx particle index
 */
    virtual void setBndryIndex(int idx) { bndryIndex = idx;}

/**
 * Get the boundary index as a label
 *
 * @return boundary index
 */
    virtual int getBndryIndex() const { return bndryIndex;}

/**
 * Return copy of the entire particle field positions vector
 *
 * @return std::vector for entire particle field positions
 */
    virtual std::vector<posVecType> getFieldPos() {
      return fieldPos;
    }

/**
 * Return copy the entire particle field value vector
 *
 * @return std::vector for entire particle field positions
 */
    virtual std::vector<FLOATTYPE> getFieldVals() {
      return fieldVals;
    }

/**
 * Increment move counter
 */
    virtual void bumpMoveAttempts() { ++moveAttempts;}

/**
 * Set move counter to zero
 */
    virtual void resetMoveAttempts() { moveAttempts = 0;}

/**
 * Set move counter to zero
 */
    virtual size_t getMoveAttempts() { return moveAttempts;}

  protected:

    /**  Recent move attempts */
    size_t moveAttempts;

    /**  Index of particle (neg values reserved for non-membership) */
    int bndryIndex;

    /** Size of field space */
    size_t numFieldElements;

    // Grid object pointer
    PsGridBase<FLOATTYPE, NDIM>* gridObjPtr;

    // List of global positions for particle field
    std::vector< posVecType > fieldPos;

    // List of particle field values
    std::vector<FLOATTYPE> fieldVals;

  private:

    // Input hierarchy set
    TxHierAttribSetIntDbl funcAttrib;

    // Names of STFuncs
    std::vector<std::string> funcNames;

    /** Make private to prevent use */
    PsBndryData(const PsBndryData<FLOATTYPE, NDIM>& vphh);

    /** Make private to prevent use */
    PsBndryData<FLOATTYPE, NDIM>& operator=(
        const PsBndryData<FLOATTYPE, NDIM>& vphh);

};

#endif  // PS_BNDRY_DATA_H
