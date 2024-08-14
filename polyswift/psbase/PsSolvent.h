/**
 *
 * @file    PsSolvent.h
 *
 * @brief   Base class interface for solvent
 *
 * @version $Id: PsSolvent.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_SOLVENT_H
#define PS_SOLVENT_H

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
#include <PsFunctionalQ.h> // needed?

/**
 * A PsSolvent object contains the spatially dependent
 * solvent observables. These include, density fields,
 * orientation fields, stress fields, electric potential etc.
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 */

template <class FLOATTYPE, size_t NDIM>
class PsSolvent : public virtual PsFunctionalQ<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 */
    PsSolvent();

/**
 * Destructor must destroy solvents
 */
    virtual ~PsSolvent();

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
 * Get the overall volume fraction for this solvent
 *
 * @return volume fraction
 */
    virtual FLOATTYPE getVolfrac() {
      return volfrac;
    }

/**
 * Calculate solvent observables
 */
    virtual void setPhysFields() = 0;

/**
 * Get the natural-log of the single-chain partition function
 * normalization value
 *
 * @return log(bigQ)
 */
    virtual FLOATTYPE getLogBigQ() {
      return (FLOATTYPE) std::log(bigQ);
    }

/**
  * Access all strings for created objects
  *
  * @return static vector of name strings
  */
    static std::vector<std::string> getSolventNames();

/**
  * Access all strings for created objects
  *
  * @return static vector of name strings
  */
    static void setSolventName(std::string pName);

  protected:

    /** Volume fraction of solvent */
    FLOATTYPE volfrac;

    // SWS: should possibly be consolidated w/PsPolymer
    // SWS: in PsFunctionalQ
    /** Solvent partition function */
    FLOATTYPE bigQ;

    /** Input hierarchy set for STFunc-s */
    TxHierAttribSetIntDbl funcAttrib;

    /** Names of STFuncs */
    std::vector<std::string> funcNames;

    /** Function for updating volume frac */
    PsSTFunc<FLOATTYPE, NDIM>* vfSTFunc;

    /** Flag for vfSTFunc */
    bool hasVfSTFunc;

  private:

    /** Make private to prevent use */
    PsSolvent(const PsSolvent<FLOATTYPE, NDIM>& vphh);

    /** Make private to prevent use */
    PsSolvent<FLOATTYPE, NDIM>& operator=(
       const PsSolvent<FLOATTYPE, NDIM>& vphh);

    /** All strings for these objects */
    static std::vector<std::string> allSolventNames;
};

#endif // PS_SOLVENT_H
