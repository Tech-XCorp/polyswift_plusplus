/**
 * @file    PsDynObjBase.h
 *
 * @brief   Control methods - base class for all domains
 *
 * @version $Id: PsDynObjBase.h 6319 2006-11-14 22:39:46Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_DYN_OBJ_BASE_H
#define PS_DYN_OBJ_BASE_H

// std includes
#include <string>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// txbase
#include <TxHierAttribSetTmpl.h>
#include <TxMaker.h>

#include <PsGridBaseItr.h>

enum DebugPrint {PSDB_OFF, PSDB_ON};

/**
 * Base class for dynamic objects
 *
 */
class PsDynObjBase {

  public:

  // SWS: not needed?
  // enum DebugPrint {PSDB_OFF, PSDB_ON};

/**
 * Constructor - does nothing.
 */
    PsDynObjBase();

/**
 * Constructor to set name.
 */
    PsDynObjBase(const std::string& nm);

/**
 * Copy Constructor
 */
    PsDynObjBase(const PsDynObjBase& dob);

/**
 * Destructor
 */
    virtual ~PsDynObjBase();

/**
 * assignment operator
 */
    PsDynObjBase& operator=(const PsDynObjBase& dob);

/**
 * Set from an attribute set.
 *
 * Stores attribute set and sets name.  By storing the settings
 * in the base class, any derived class has the information it
 * needs to rebuild itself.
 *
 * @param tas the attribute set
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Get a reference to the attribute set.
 *
 * @return a reference to the attribute set
 */
    virtual const TxHierAttribSetIntDbl& getAttrib() const {
      return settings;
    }

/**
 * Build the data for this object
 */
    virtual void buildData() = 0;

/**
 * Build the solvers structures for this object
 */
    virtual void buildSolvers() = 0;

/**
 * update system
 *
 * @param time the new time to update to
 */
    virtual void update(double time) = 0;

/**
 * dump system
 */
    virtual void dump() = 0;

/**
 * Get the domain update step
 */
    static size_t getCurrDomainStep() {
      return currDomainStep;
    }

/**
 * Set the domain update step
 */
    static void setCurrDomainStep(size_t tstep) {
      currDomainStep = tstep;
    }

/**
 * Get nsteps
 */
    static size_t getNsteps() {
      return totNumDomainStep;
    }

/**
 * Set nsteps
 */
    static void setNsteps(size_t ns) {
      totNumDomainStep = ns;
    }

/**
 * Increment domain update step by one
 */
    static void bumpCurrDomainStep() {
      currDomainStep = currDomainStep + 1;
    }

  protected:

  private:

/// The name for this object
    std::string name;

/// The current update step for the domain
    static size_t currDomainStep;

/// The total number of update steps for the domain during this sim
    static size_t totNumDomainStep;

/// The settings for this object
    TxHierAttribSetIntDbl settings;

};

#endif // PS_DYN_OBJ_BASE_H
