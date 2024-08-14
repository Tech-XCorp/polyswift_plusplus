/**
 *
 * @file    PsBndryBase.h
 *
 * @brief   Base interface for the boundaries
 *
 * @version $Id: PsBndryBase.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_BNDRY_BASE_H
#define PS_BNDRY_BASE_H

// standard headers
#include <set>
#include <vector>
#include <algorithm> // Needed for find()

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#undef HAVE_CONFIG_H
#endif

// txbase headers
#include <TxDebugExcept.h>

// psbase includes
#include <PsDynObj.h>
#include <PsPhysField.h>
#include <PsGridField.h>
#include <PsBndryDataBase.h>

/**
 * A PsBndryBase object contains base boundary methods interface
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 */
template <class FLOATTYPE, size_t NDIM>
class PsBndryBase : public virtual PsDynObj<FLOATTYPE, NDIM> {

  public:

/** Constructor */
    PsBndryBase();

/** Destructor */
    virtual ~PsBndryBase();

/**
 * Set the parameters
 *
 * @param tas the parameters
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Build the data, such as updaters, parses input etc.
 */
    virtual void buildData();

/**
 * Build the solvers structures
 */
    virtual void buildSolvers();

/**
 * Initialize object before update steps
 */
    virtual void initialize() = 0;

/**
 * Dump object before update steps
 *
 * @param txIoPtr IO object poiner
 */
    virtual void dump(TxIoBase* txIoPtr) = 0;

/**
 * Restore object before update steps
 *
 * @param txIoPtr IO object poiner
 */
    virtual void restore(TxIoBase* txIoPtr) = 0;

/**
 * Update the object:
 *   Uses depFields to set constraint and interaction fields
 *
 * @param t update time
 */
    virtual void update(double t);

/**
 * Reset density fields only of held physical fields
 */
    virtual void resetDensFields() {
      constraintPhysFldPtr->resetDensField();
      bndryPhysFldPtr->resetDensField();
    }

/**
 * Get number of all boundaries
 *
 * @return number of particle interfaces held by the allBndrys list
 */
    static size_t getNumAllBndrys();
// {
//      return allBndrys.size();
//    }

  protected:

    /**
     * Check overlaps for boundary global static list allBndrys
     *
     * @param bndryPtr pointer to boundary data interface
     */
    static bool doesBndryOverlap(PsBndryDataBase<FLOATTYPE, NDIM>* bndryPtr);

    /**
     * Add boundary to static group list and update boundary index
     * Sets the boundary index
     *
     * @param bndryPtr pointer to data interface
     */
    static void addBndry(PsBndryDataBase<FLOATTYPE, NDIM>* bndryPtr);

    /**
     * Remove particle from group list... this is untested, only
     * removes pointer copy from list
     *
     * @param bndryPtr pointer to particle data interface
     */
    static void removeBndry(PsBndryDataBase<FLOATTYPE, NDIM>* bndryPtr);

    /** Arbitrary value of constraint field threshold */
    FLOATTYPE bndryFieldThreshold;

    /** Boundary PhysField name */
    std::string bndryFieldName;

    /** Dimensions of wall field */
    //    std::vector<size_t> bndry Dims;

    /** Boundary deposition field */
    PsGridField<FLOATTYPE, NDIM> bndryDepField;

    /** Pointer to self-consistent PhysField for constraints */
    PsPhysField<FLOATTYPE, NDIM>* constraintPhysFldPtr;

    /** Pointer to self-consistent PhysField for monomer densities */
    PsPhysField<FLOATTYPE, NDIM>* bndryPhysFldPtr;

    /** Vector of pointers to particle base interfaces */
    static std::vector< PsBndryDataBase<FLOATTYPE, NDIM>* > allBndrys;

  private:

    /** Largest particle index... across all particles */
    static int maxAllBndryIndex;

    /** Make private to prevent use */
    PsBndryBase(const PsBndryBase<FLOATTYPE, NDIM>& vphh);

    /** Make private to prevent use */
    PsBndryBase<FLOATTYPE, NDIM>& operator=(
        const PsBndryBase<FLOATTYPE, NDIM>& vphh);
};

#endif // PS_BNDRY_BASE_H
