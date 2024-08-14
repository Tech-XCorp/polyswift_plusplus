/**
 * @file    PsFloryWallInteraction.h
 *
 * @brief   Class for calculating energy from FloryWall interaction model
 *
 * @version $Id: PsFloryWallInteraction.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_FLORY_WALL_INTERACTION_H
#define PS_FLORY_WALL_INTERACTION_H

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
#include <PsFloryInteraction.h>

/**
 * A PsFloryWallInteraction object calculates energy from FloryWall interaction model
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 */

template <class FLOATTYPE, size_t NDIM>
class PsFloryWallInteraction : public virtual PsFloryInteraction<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 *
 */
    PsFloryWallInteraction();

/**
 * Destructor must destroy blocks
 */
    virtual ~PsFloryWallInteraction();

/**
 * Set the parameters
 *
 * @param tas the parameters
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Build the solvers structures
 */
    virtual void buildSolvers();

/**
 * Build the data
 */
    virtual void buildData();

/**
 * Initialize
 */
    virtual void initialize();

/**
 * Restore on restart
 */
    virtual void restore() {}

/**
 * Dump the object:
 */
    virtual void dump() {}

/**
 * Calculate contribution to functional derivative
 * with respect to the input field
 *
 * @param wrtPhysField input field string
 * @param dField field reference for result
 */
    virtual void calcDfD(std::string wrtPhysField,
         PsFieldBase<FLOATTYPE>& dField);

/**
 * Calculate contribution to free-energy for input field
 *
 * @param dField reference for field result
 * @param calcDisorder flag for including homogeneous contribution
 */
    virtual void calcFe(PsFieldBase<FLOATTYPE>& dField,
                        bool calcDisorder=true);

/**
 * Update the interaction term: energy and dEnergy
 *
 * @param t the time
 */
    virtual void update(double t);

  protected:

    /** Name of physical field associated with wall */
    std::string wallFieldName;

    /** Local place holder for pointer to wall field */
    PsPhysField<FLOATTYPE, NDIM>* wallFieldPtr;

    /** Local place holder density field */
    PsPhysField<FLOATTYPE, NDIM>* densFieldPtr;

  private:

    /** Average of density field that couples to wall */
    FLOATTYPE densAvg;

    /** Constructor private to prevent use */
    PsFloryWallInteraction(const PsFloryWallInteraction<FLOATTYPE, NDIM>& psb);

   /** Assignment private to prevent use */
    PsFloryWallInteraction<FLOATTYPE, NDIM>& operator=(
       const PsFloryWallInteraction<FLOATTYPE, NDIM>& psb);
};

#endif  // PS_FLORY_WALL_INTERACTION_H
