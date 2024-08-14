/**
 * @file    PsMonomerDens.h
 *
 * @brief   Class that holds monomer density fields
 *
 * @version $Id: PsMonomerDens.h 8401 2007-09-26 20:17:05Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_MONOMER_DENS_H
#define PS_MONOMER_DENS_H

#include <vector>
#include <map>
#include <set>

// system includes
#include <stdio.h>

// psphysf includes
#include <PsPhysFldFuncs.h>

// psbase includes
#include <PsBlockBase.h>
#include <PsPolymer.h>
#include <PsSolvent.h>

/**
 * A PsMonomerDens contains PsFields of monomer densities
 *
 */
template <class FLOATTYPE, size_t NDIM>
class PsMonomerDens : public PsPhysFldFuncs<FLOATTYPE, NDIM> {

   public:

/**
 * A PsMonomerDens
 */
    PsMonomerDens();

/**
 * Destructor
 */
    virtual ~PsMonomerDens();

/**
 * Store the data needed to build this object.
 *
 * @param tas the attribute set containing the initial conditions
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Build the data, such as updaters, initial condition setters, ...
 */
    virtual void buildData();

/**
 * Build the solvers, resize, ...
 */
    virtual void buildSolvers();

/**
 * Initializes the field values from the initial conditions...
 */
    virtual void initialize();

/**
 * Restores the field values on restart
 *
 * @param txIoPtr the IO object
 */
    virtual void restore(TxIoBase* txIoPtr);

/**
 * Dump the field data
 *
 * @param txIoPtr the IO object
 */
    virtual void dump(TxIoBase* txIoPtr);

/**
 * Update object
 */
    virtual void update(double t);

/**
 * Return reference to density field object
 */
    virtual PsFieldBase<FLOATTYPE>& getDensField() {
      return (*monoDensFieldPtr);
    };

/**
 * Return reference to density field object shifted by monomer average
 */
    virtual PsFieldBase<FLOATTYPE>& getShiftedDensField();

/**
 * Return reference to conjugate density field object
 */
    virtual PsFieldBase<FLOATTYPE>& getConjgField() {
      return (*chemWFieldPtr);
    };

/**
 * Get average monomer density
 *
 * @return average monomer density
 */
    virtual FLOATTYPE getDensAverage() {
      return monoDensAverage;
    }

/**
 * Set average monomer density to zero
 */
    virtual void resetDensAverage() {
      monoDensAverage = 0.0;
    }

/**
 * Add to average monomer density
 *
 * @param avg average monomer density contribution
 */
    virtual void addToDensAverage(FLOATTYPE avg) {
      monoDensAverage = monoDensAverage + avg;
    }

/**
 * Calculate product of held physical fields
 *
 * @param dField reference for result field
 */
    virtual void calcFieldProd(PsFieldBase<FLOATTYPE>& dField);

  protected:

    /** PsFieldBase object holding monomer densities */
    PsFieldBase<FLOATTYPE>* monoDensFieldPtr;

    /** PsFieldBase object holding conjugate monomer densities */
    PsFieldBase<FLOATTYPE>* chemWFieldPtr;

    /** String init flag */
    std::string initOption;

   private:

/**
 * Private helper method: reset the field class values
 */
    virtual void resetDensField();

/**
 * Private helper method. Uses current
 * volume fractions and length fractions of all components
 */
    virtual void updateMonoAvg();

/**
 * Test function for initializing charge density
 */
    FLOATTYPE slabFunc(PsTinyVector<int, NDIM>& rvec,
                       PsTinyVector<int, NDIM>& r0);

    /** Field quantity (a placeholder) */
    PsFieldBase<FLOATTYPE>* dField;

    // Test function for charge distribution
    void testSlab(int fsign);

    // Test function for charge distribution
    void sineSlab(int fsign, FLOATTYPE omega);

    /** Sign of init pattern */
    FLOATTYPE ifreqSize;

    /* String name for dumps */
    std::string monoDensDataName;

    /* String name for dumps */
    std::string wFieldDataName;

    /** Field average */
    FLOATTYPE monoDensAverage;

    /** private to prevent use */
    PsMonomerDens(const PsMonomerDens<FLOATTYPE, NDIM>&);

    /** private to prevent use */
    PsMonomerDens& operator=(const PsMonomerDens<FLOATTYPE, NDIM>&);
};

#endif // PS_MONOMER_DENS_H
