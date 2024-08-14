/**
 *
 * @file    PsInteraction.h
 *
 * @brief   Class for calculating energetics from appropriate fields
 *
 * @version $Id: PsInteraction.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_INTERACTION_H
#define PS_INTERACTION_H

// standard headers
#include <list>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#undef HAVE_CONFIG_H
#endif

// txbase headers
#include <TxDebugExcept.h>

// psstd includes
#include <PsSTFunc.h>

// psbase includes
#include <PsDynObj.h>
#include <PsPhysField.h>
#include <PsFieldBase.h>
#include <PsDecompBase.h>

/**
 * A PsInteraction object contains alogrithms for field operations
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 */

template <class FLOATTYPE, size_t NDIM>
class PsInteraction : public virtual PsDynObj<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 *
 */
    PsInteraction();

/**
 * Destructor must destroy blocks
 */
    virtual ~PsInteraction();

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
    virtual void initialize() = 0;

/**
 * Restore on restart
 */
    virtual void restore() = 0;

/**
 * Dump the object
 */
    virtual void dump() = 0;

/**
 * Calculate contribution to functional derivative
 * with respect to the input field
 *
 * @param wrtPhysField input field string
 * @param dField field reference for result
 */
    virtual void calcDfD(std::string wrtPhysField,
          PsFieldBase<FLOATTYPE>& dField) = 0;

/**
 * Calculate contribution to free-energy for held fields
 *
 * @param dField reference to base field result object
 * @param calcDisorder flag for including homogeneous contribution
 */
    virtual void calcFe(PsFieldBase<FLOATTYPE>& dField,
        bool calcDisorder=true) = 0;

/**
 * Update the interaction term
 *
 * @param t the time
 */
    virtual void update(double t) = 0;

/**
 * Return interaction paramter
 *
 * @return field base reference
*/
    virtual PsFieldBase<FLOATTYPE>& getParam() = 0;

/**
 * Return interaction paramter if constant in space
 *
 * @return constant interaction paramater
*/
    virtual FLOATTYPE getParamUniform() = 0;

/**
 * Check if field is contained in this interaction
 *
 * @param fieldName scField string name to check
 * @return boolean value
 */
    bool hasScField(std::string fieldName);

/**
 * Return pointer to the physical field in this interaction other than the
 * physical field name specified
 *
 * @param physFieldName scField string name to check
 * @return pointer to the physical field that the specified field interacts with
 */
    virtual PsPhysField<FLOATTYPE, NDIM>* getOtherPhysField(
        std::string physFieldName);

  protected:

    /** Free-energy contribution from interaction */
    FLOATTYPE freeEnergy;

    /** To shift fields with average density */
    bool shiftDensFlag;

    /** Self-consistent PhysField names */
    std::vector<std::string> scfieldNames;

    /** List of pointers to the physical fields */
    std::vector< PsPhysField<FLOATTYPE, NDIM>* > physFields;

    /** Constraint field (to find physical grid volume) */
    PsPhysField<FLOATTYPE, NDIM>* constraintFieldPtr;

    /** Workspace based on rank of held physical fields */
    PsFieldBase<FLOATTYPE>* tmpFieldPtr;

  private:

    /** Constructor private to prevent use */
    PsInteraction(const PsInteraction<FLOATTYPE, NDIM>& psb);

   /** Assignment private to prevent use */
    PsInteraction<FLOATTYPE, NDIM>& operator=(
       const PsInteraction<FLOATTYPE, NDIM>& psb);
};

#endif  // PS_INTERACTION_H
