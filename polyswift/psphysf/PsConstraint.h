/**
 *
 * @file    PsConstraint.h
 *
 * @brief   Class that holds constraint (walls/particles) fields
 *
 * @version $Id: PsConstraint.h 8401 2007-09-26 20:17:05Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_CONSTRAINT_H
#define PS_CONSTRAINT_H

#include <vector>
#include <map>
#include <set>

// system includes
#include <stdio.h>

// psphysf includes
#include <PsPhysFldFuncs.h>

/**
 * A PsConstraint contains PsFields of monomer densities
 *
 */
template <class FLOATTYPE, size_t NDIM>
class PsConstraint : public virtual PsPhysFldFuncs<FLOATTYPE, NDIM> {

   public:

/**
 * A PsConstraint
 */
    PsConstraint();

/**
 * Destructor
 */
    virtual ~PsConstraint();

/**
 * Store the data needed to build this object.
 *
 * @param tas the attribute set containing the initial conditions
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Build the solvers, resize, ...
 */
    virtual void buildSolvers();

/**
 * Build the data, such as updaters, initial condition setters, ...
 */
    virtual void buildData();

/**
 * Initializes the field values from the initial conditions...
 */
    virtual void initialize();

/**
 * Dump the field data
 *
 * @param txIoPtr IO object
 */
    virtual void dump(TxIoBase* txIoPtr);

/**
 * Restores the field values on restart
 *
 * @param txIoPtr IO object
 */
    virtual void restore(TxIoBase* txIoPtr);

/**
 * Update object
 */
    virtual void update(double t);

/**
 * Update specific pressure field members
 * Note: not in PsPhysField interface
 */
    virtual void updatePres();

/**
 * Return pointer to density field object
 */
    virtual PsFieldBase<FLOATTYPE>& getDensField() {
      return *constraintFieldPtr;
    };

/**
 * Return pointer to conjugate density field object
 */
    virtual PsFieldBase<FLOATTYPE>& getConjgField() {
      return *presFieldPtr;
    };

/**
 * Get the volume of the constraint on a process
 * Checks for zero volume from 'bad' decomposition
 *
 * @return volume on a processor
 */
    virtual FLOATTYPE calcLocalVolume();

  protected:

    /** PsFieldBase object holding constraint density */
    PsFieldBase<FLOATTYPE>* constraintFieldPtr;

    /** PsFieldBase object holding conjugate incompressibility field */
    PsFieldBase<FLOATTYPE>* presFieldPtr;

    /** PsFieldBase object holding pressure correction for walls */
    PsFieldBase<FLOATTYPE>* presCorrectFieldPtr;

    /** Vector of pointers to PhysField for which constraint applies */
    std::vector<PsPhysField<FLOATTYPE, NDIM>* > excludeFields;

    /**
     * Subtract out the average pressure field value
     * from all elements
     */
    void shiftPresField();

   private:

    /** min threshold for local volume excluded by constraint */
    FLOATTYPE volumeThreshold;

    /** max threshold for constraint density */
    FLOATTYPE maxThreshold;

    /** Field quantity (a placeholder) */
    PsFieldBase<FLOATTYPE>* dField;

    /** Pre-condition factor setting chem fields in constraint regions */
    FLOATTYPE preconditionFactor;

    /** String name for dumps */
    std::string constraintDataName;

    /** String name for dumps */
    std::string presDataName;

    /** Self-consistent PhysField names on which to apply constraint */
    std::vector<std::string> excludeFieldNames;

    /**
     * Helper method: reset the field class values
     */
    virtual void resetDensField();

    /** private to prevent use */
    PsConstraint(const PsConstraint<FLOATTYPE, NDIM>&);

    /** private to prevent use */
    PsConstraint& operator=(const PsConstraint<FLOATTYPE, NDIM>&);
};

#endif // PS_CONSTRAINT_H

