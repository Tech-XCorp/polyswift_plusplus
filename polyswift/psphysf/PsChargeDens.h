/**
 *
 * @file    PsChargeDens.h
 *
 * @brief   Class that holds electrostatic charge density fields
 *          and associated potential
 *
 * @version $Id: PsChargeDens.h 8401 2007-09-26 20:17:05Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_CHARGE_DENS_H
#define PS_CHARGE_DENS_H

#include <stdio.h>

// psphysf includes
#include <PsPhysFldFuncs.h>

/**
 * A PsChargeDens contains PsFields of electrostatic charge density fields
 *
 */
template <class FLOATTYPE, size_t NDIM>
class PsChargeDens : public PsPhysFldFuncs<FLOATTYPE, NDIM> {

 public:

/**
 * A PsChargeDens
 */
  PsChargeDens();

/**
 * Destructor
 */
  virtual ~PsChargeDens();

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
 *
 */
  virtual void buildData();

/**
 * Initializes the field values
 */
  virtual void initialize();

/**
 * Dump the field values
 *
 * @param txIoPtr the IO object
 */
  virtual void dump(TxIoBase* txIoPtr);

/**
 * Restore the field values on restart
 *
 * @param txIoPtr the IO object
 */
  virtual void restore(TxIoBase* txIoPtr);

/**
 * Update object
 */
  virtual void update(double t);

/**
 * Return reference to density field object
 */
    virtual PsFieldBase<FLOATTYPE>& getDensField() {
      return (*chargeDensFieldPtr);
    };

/**
 * Return reference to Del[density] field object

    virtual PsFieldBase<FLOATTYPE>& getDelDensField() {

      PsFieldBase<FLOATTYPE>& delChargeDens = *delChargeDensFieldPtr;
      PsFieldBase<FLOATTYPE>& chargeDens0 = *chargeDensFieldPtr0;
      PsFieldBase<FLOATTYPE>& chargeDens  = *chargeDensFieldPtr;

      // delPhi = Phi_(n+1) - Phi_(n)
      delChargeDens.reset(0.0);
      delChargeDens += chargeDens0;
      delChargeDens.scale(-1.0);
      delChargeDens += chargeDens;

      return (*delChargeDensFieldPtr);
    };
 */

/**
 * Return reference to electric potential field object
 */
    virtual PsFieldBase<FLOATTYPE>& getConjgField() {
      return (*ePotentialFieldPtr);
    };

  protected:

    /** Object holding charge density */
    PsFieldBase<FLOATTYPE>* chargeDensFieldPtr;

    /** Object holding charge density from previous update */
    //    PsFieldBase<FLOATTYPE>* chargeDensFieldPtr0;

    /** Object holding del[charge density] from previous update */
    //    PsFieldBase<FLOATTYPE>* delChargeDensFieldPtr;

    /** Object holding elec poten conjugate to charge dens */
    PsFieldBase<FLOATTYPE>* ePotentialFieldPtr;

  private:

    /* String name for dumps */
    std::string chargeDensDataName;

    /* String name for dumps */
    std::string ePotentialDataName;

    /**
     * Reset the field values
     */
    virtual void resetDensField();

    // Test function for charge distribution
    void testCharges();

    /**
     * Test function for initializing charge density
     */
    FLOATTYPE ptclFunc(PsTinyVector<int, NDIM>& rvec,
        PsTinyVector<int, NDIM>& r0);

    /** private to prevent use */
    PsChargeDens(const PsChargeDens<FLOATTYPE, NDIM>&);

    /** private to prevent use */
    PsChargeDens& operator=(const PsChargeDens<FLOATTYPE, NDIM>&);
};

#endif // PS_CHARGE_DENS_H

