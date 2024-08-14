/**
 *
 * @file    PsUpdater.h
 *
 * @brief   Class for updating physical fields in simulation model
 *
 * @version $Id: PsUpdater.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_UPDATER_H
#define PS_UPDATER_H

// standard headers
#include <list>

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
#include <PsFieldBase.h>
#include <PsCommBase.h>

/**
 * A PsUpdater object contains alogrithms for field operations
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 */

template <class FLOATTYPE, size_t NDIM>
class PsUpdater : public virtual PsDynObj<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 */
    PsUpdater();

/**
 * Destructor must destroy blocks
 */
    virtual ~PsUpdater();

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
 * Initialize
 */
    virtual void initialize();

/**
 * Update the updater and set flag for performing update step
 *
 * @param t the time
 */
    virtual void update(double t);

/**
 * Get names of held update fields
 */
    std::vector<std::string> getUpdateFieldNames() {
      return updateFieldNames;
    }

    // SWS: make friend func?
/**
 * Get free-energy value
 */
    virtual FLOATTYPE getFreeE(bool calcDisorder=true) = 0;

  protected:

    /** Flag for performing update methods */
    bool updateFlag;

    /** Number of update fields */
    size_t numUpdateFields;

    /** Pointers to phys fields to be updated (same types assumed) */
    std::vector< PsPhysField<FLOATTYPE, NDIM>* > updateFields;

    /** Type of the held update physical fields */
    std::string updateFieldType;

    /** Update field lengths */
    //    std::vector<size_t> update Field Lengths;

    /** Self-consistent PhysField names */
    std::vector<std::string> updateFieldNames;

    /** To shift fields with average density */
    bool shiftDensFlag;

    /**
     * Number of simulation steps between successive
     * applications of this updater... AFTER the first
     * update is applied
     */
    size_t applyFrequency;

    /** # of sim steps before first application of updater */
    size_t applyStart;

    /** # of simulation steps before last application of updater */
    size_t applyEnd;

    /** Field workspace */
    PsFieldBase<FLOATTYPE>* dFld;

    /** Field workspace */
    PsFieldBase<FLOATTYPE>* dFld2;

  private:

    /** Check field types/sizes */
    void checkSetUpdateFields();

    /** Constructor private to prevent use */
    PsUpdater(const PsUpdater<FLOATTYPE, NDIM>& psb);

   /** Assignment private to prevent use */
    PsUpdater<FLOATTYPE, NDIM>& operator=(
       const PsUpdater<FLOATTYPE, NDIM>& psb);
};

#endif  // PS_UPDATER_H
