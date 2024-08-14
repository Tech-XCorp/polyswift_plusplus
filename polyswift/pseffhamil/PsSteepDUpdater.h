/**
 *
 * @file    PsSteepDUpdater.h
 *
 * @brief   Class for updating fields with steepest descent algorithm
 *
 * @version $Id: PsSteepDUpdater.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_STEEP_D_UPDATER_H
#define PS_STEEP_D_UPDATER_H

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
#include <PsPhysField.h>
#include <PsFieldBase.h>

// pseffhamil includes
#include <PsPolymerUpdater.h>

/**
 * A PsSteepDUpdater object updates fields using steepest descent
 * algorithm
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 */

template <class FLOATTYPE, size_t NDIM>
class PsSteepDUpdater : public virtual PsPolymerUpdater<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 *
 */
    PsSteepDUpdater();

/**
 * Destructor must destroy blocks
 */
    virtual ~PsSteepDUpdater();

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
 * Initialize object
 */
    virtual void initialize();

/**
 * Update the updater
 *
 * @param t the time
 */
    virtual void update(double t);

  protected:

    /** Strength of noise term in relaxation algorithm */
    FLOATTYPE noiseStrength;

    /** Relaxation parameters */
    std::vector<FLOATTYPE> relaxlambdas;

    /** Total func. derivative contributions used to calc updateFields */
    std::vector< PsFieldBase<FLOATTYPE>* > dHTotals;

    /** Number of constraint interactions */
    size_t numCInteractions;

    /** List of interaction constraint names */
    std::vector< std::string > cInteractionNames;

    /** List of interaction constraint pointers */
    std::vector< PsInteraction<FLOATTYPE, NDIM>* > constraintInteractions;

  private:

    /** Field result holder */
    PsFieldBase<FLOATTYPE>* resField;

    /**
     * Helper method for update
     * Calculate total dH values for each physical update field
     */
    void update_dHTotals();

    /**
     * Helper method for update
     * Uses dHTotals to calculate change in updateFields
     */
    void update_set2Fields();

    /**
     * Helper method for update
     * Uses dHTotals to calculate change in updateFields
     */
    void update_set3Fields();

    /** Constructor private to prevent use */
    PsSteepDUpdater(const PsSteepDUpdater<FLOATTYPE, NDIM>& psb);

   /** Assignment private to prevent use */
    PsSteepDUpdater<FLOATTYPE, NDIM>& operator=(
       const PsSteepDUpdater<FLOATTYPE, NDIM>& psb);
};

#endif  // PS_STEEP_D_UPDATER_H
