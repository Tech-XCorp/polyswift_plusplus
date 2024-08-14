/**
 *
 * @file    PsFunctionalQ.h
 *
 * @brief   Holds the field pointers needed to calculate field-theory
 *
 * @version $Id: PsFunctionalQ.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_FUNCTIONAL_Q_H
#define PS_FUNCTIONAL_Q_H

// standard headers
#include <set>

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
#include <PsDecompBase.h>
#include <PsGridBase.h>
#include <PsCommBase.h>

/**
 * A PsFunctionalQ object holds the field pointers needed to
 * define and calculate terms in the field-transformed Hamiltonian
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 */

template <class FLOATTYPE, size_t NDIM>
class PsFunctionalQ : public virtual PsDynObj<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 *
 */
    PsFunctionalQ();

/**
 * Destructor must destroy polymers
 */
    virtual ~PsFunctionalQ();

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
 * Build the solvers structures, fields etc.
 */
    virtual void buildSolvers();

/**
 * Update the object
 *
 * @param t update time
 */
    virtual void update(double t);

/**
 * Get the overall volume fraction for this component
 *
 * @return volume fraction
 */
//    virtual FLOATTYPE getVolfrac() {
//      return volfrac;
//    }

  protected:

    /** Volume fraction of component (polymer/solvent) */
    //    FLOATTYPE volfrac;

    /** Pointer to physical field for constraint */
    PsPhysField<FLOATTYPE, NDIM>* constraintFieldPtr;

    /** Pointer to self-consistent PhysField for monomer dens */
    PsPhysField<FLOATTYPE, NDIM>* monoDensPhysFldPtr;

    // SWS: should refactor
    /** Pointer to self-consistent PhysField for block densities */
    PsPhysField<FLOATTYPE, NDIM>* blockDensPhysFldPtr;

    /** Pointer to self-consistent PhysField for charge densities */
    PsPhysField<FLOATTYPE, NDIM>* chargeDensPhysFldPtr;

    /** Workspace based on rank of held physical fields */
    PsFieldBase<FLOATTYPE>* tmpFieldPtr;

    /** Charge PhysField name */
    bool hasChargeField;

    /** Charge PhysField name */
    bool hasBlockField;

  private:

    /** Monomer PhysField name */
    std::string scfieldName;

    /** Monomer block PhysField name */
    std::string blockfieldName;

    /** Charge PhysField name */
    std::string chargefieldName;

    /** Make private to prevent use */
    PsFunctionalQ(const PsFunctionalQ<FLOATTYPE, NDIM>& vphh);

    /** Make private to prevent use */
    PsFunctionalQ<FLOATTYPE, NDIM>& operator=(
       const PsFunctionalQ<FLOATTYPE, NDIM>& vphh);
};

#endif // PS_FUNCTIONAL_Q_H
