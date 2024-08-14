/**
 *
 * @file    PsPolymerUpdater.h
 *
 * @brief   Class for updating fields for polymer models w/interactions
 *
 * @version $Id: PsPolymerUpdater.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2009-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_POLYMER_UPDATER_H
#define PS_POLYMER_UPDATER_H

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#undef HAVE_CONFIG_H
#endif

// psbase includes
#include <PsPolymer.h>
#include <PsSolvent.h>
#include <PsInteraction.h>

// pseffhamil includes
#include <PsConstraintUpdater.h>

/**
 * A PsPolymerUpdater object contains alogrithms for updating
 * Polymer objects and obtains access to polymer object
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 */

template <class FLOATTYPE, size_t NDIM>
class PsPolymerUpdater : public virtual PsConstraintUpdater<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 *
 */
    PsPolymerUpdater();

/**
 * Destructor must destroy blocks
 */
    virtual ~PsPolymerUpdater();

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
 * Update the updater
 *
 * @param t the time
 */
    virtual void update(double t);

  protected:

    //
    // Interactions
    //

    /** Number of interactions */
    size_t numInteractions;

    /** List of interaction names */
    std::vector< std::string > interactionNames;

    /** List of interaction model pointers */
    std::vector< PsInteraction<FLOATTYPE, NDIM>* > interactions;

    //
    // Polymer data
    //

    /** Number of polymers */
    size_t numPolymers;

    /** Number of solvents */
    size_t numSolvents;

    /** List of polymer data pointers */
    std::vector< PsPolymer<FLOATTYPE, NDIM>* > polymers;

    /** List of polymer data pointers */
    std::vector< PsSolvent<FLOATTYPE, NDIM>* > solvents;

    //
    // Free-energy
    //

/**
  * Get free-energy value
  *
  * @param calcDisorder flag for including disorder freeE calc
  * @return free energy value
 */
    virtual FLOATTYPE getFreeE(bool calcDisorder=true) {
      if (calcDisorder)
        return sumFe;
      else
        return sumFeNoDis;
    }

    /** Local free-energy w/disorder removed */
    FLOATTYPE sumFe;

    /** Local free-energy wo/disorder removed */
    FLOATTYPE sumFeNoDis;

    /** Total Free-energy field */
    PsFieldBase<FLOATTYPE>* feTotFieldPtr;

    /** Total Free-energy field */
    PsFieldBase<FLOATTYPE>* feTotNoDisFieldPtr;

    /** Free-energy field counter */
    PsFieldBase<FLOATTYPE>* feFieldPtr;

    /** Calculate the free-energy/chain over space */
    virtual void calcFeField();

    // SWS: should be moved to PsSteepDUpdater???
    /**
     * Update pressure field by adding conjugate fields together
     * When used... pressure values assumed to
     * have already been used to update dHTotals
     */
    void setConjgFieldsToPres();

    /**
     * Add random numbers to conjugate fields for all
     * updateFields (ie  PsPhysField-s)
     *
     * @param nstrength size factor of random numbers
     */
    void addRandUpdateConjgFields(FLOATTYPE nstrength);

  private:

    /** Sum the prod of all physfields held as update fields */
    virtual void calcUpdateFieldProd(PsFieldBase<FLOATTYPE>& dField);

    /** Constructor private to prevent use */
    PsPolymerUpdater(const PsPolymerUpdater<FLOATTYPE, NDIM>& psb);

   /** Assignment private to prevent use */
    PsPolymerUpdater<FLOATTYPE, NDIM>& operator=(
       const PsPolymerUpdater<FLOATTYPE, NDIM>& psb);
};

#endif  // PS_POLYMER_UPDATER_H
