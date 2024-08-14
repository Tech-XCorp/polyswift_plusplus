/**
 *
 * @file    PsFloryInteraction.h
 *
 * @brief   Class for calculating energy from Flory interaction model
 *
 * @version $Id: PsFloryInteraction.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_FLORY_INTERACTION_H
#define PS_FLORY_INTERACTION_H

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
#include <PsGridField.h>
#include <PsPolymer.h>

// pseffhamil includes
#include <PsInteraction.h>

/**
 * A PsFloryInteraction object calculates energy from Flory interaction model
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 */

template <class FLOATTYPE, size_t NDIM>
class PsFloryInteraction : public virtual PsInteraction<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 *
 */
    PsFloryInteraction();

/**
 * Destructor must destroy blocks
 */
    virtual ~PsFloryInteraction();

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
    virtual void restore() { }

/**
 * Dump the object:
 */
    virtual void dump();

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
 * Calculate contribution to free-energy for held fields
 *
 * @param dField reference to base field result object
 * @param calcDisorder flag for including homogeneous contribution
 */
    virtual void calcFe(PsFieldBase<FLOATTYPE>& dField,
        bool calcDisorder=true);
/**
 * Update the interaction term
 *
 * @param t the time
 */
    virtual void update(double t);

/**
 * Return interaction paramter if constant in space
 *
 * @return constant interaction paramater
*/
    virtual FLOATTYPE getParamUniform() {
      if (hasChiNrSTFunc) {
        TxDebugExcept tde("PsFloryInteraction::getParamUniform cannot "
            "be used with non-uniform STFunc");
        tde << " in <PsInteraction " << this->getName() << " >";
        throw tde;
      }
// Assumes chiN used to set unform, but (t) varying chi
      return chiN;
    }

/**
 * Return interaction paramter
 *
 * @return chiN
*/
    virtual PsFieldBase<FLOATTYPE>& getParam() {
      return (*chiNFieldPtr);
    }

  protected:

    /** Flory "chi" parameter */
    FLOATTYPE chi;

    /** Flory "chi*N" parameter */
    FLOATTYPE chiN;

    /** Spatially dependent chiN field */
    PsFieldBase<FLOATTYPE>* chiNFieldPtr;

    /** Local work space for dens at wall field */
    PsFieldBase<FLOATTYPE>* densAtWallPtr;

    /** Local dimensions (for chiN field) */
    std::vector<size_t> localDims;

    /** Local workspace for chi field */
    PsGridField<FLOATTYPE, NDIM> initLocalField;

    /** Scaling length... the N in chiN */
    FLOATTYPE Nlen;

    /** Input hierarchy set for STFunc */
    std::vector<TxHierAttribSetIntDbl> funcAttribs;

    /** Names of STFuncs */
    std::vector<std::string> funcNames;

    // SWS: combine these two STFunc's???
    /** Function for updating chi parameter */
    PsSTFunc<FLOATTYPE, NDIM>* chiSTFunc;

    /** Function for updating chi parameter */
    //    PsSTFunc<FLOATTYPE, NDIM>* chiNrSTFunc;

    /** Holder for all STFuncs */
    std::vector<PsSTFunc<FLOATTYPE, NDIM>* > stFuncVec;

    /** Flag for simple chi */
    bool hasConstChi;

    /** Flag for chiSTFunc */
    bool hasConstChiRamp;

    /** Flag for chiSTFunc */
    bool hasChiNrSTFunc;

  private:

    // SWS: Are these updated for varying vol frac calcs?
    /** Average density */
    FLOATTYPE densAvg0;

    /** Average density */
    FLOATTYPE densAvg1;

    /** Constructor private to prevent use */
    PsFloryInteraction(const PsFloryInteraction<FLOATTYPE, NDIM>& psb);

   /** Assignment private to prevent use */
    PsFloryInteraction<FLOATTYPE, NDIM>& operator=(
       const PsFloryInteraction<FLOATTYPE, NDIM>& psb);
};

#endif  // PS_FLORY_INTERACTION_H
