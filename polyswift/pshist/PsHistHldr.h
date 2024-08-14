/**
 * @file    PsHistHldr.h
 *
 * @brief   Base class owning instances of PsHistoryBase
 *
 * @version $Id: PsHistHldr.h 6532 2007-02-01 18:37:00Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_HIST_HLDR_H
#define PS_HIST_HLDR_H

// std includes
#include <vector>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsDynObj.h>
#include <PsHistoryBase.h>
#include <PsCommHistory.h>

/**
 * HistHldr contains a vector of pointers to PsHistory objects
 *
 * @param FLOATTYPE the data type of this history
 * @param NDIM the dimensionality of this history
 */
template <class FLOATTYPE, size_t NDIM>
class PsHistHldr : public PsDynObj<FLOATTYPE, NDIM> {

  public:

    typedef typename std::vector< PsHistoryBase<FLOATTYPE, NDIM>* >::iterator HistIter;

/**
 * Constructor
 */
    PsHistHldr();

/**
 * Destructor
 */
    virtual ~PsHistHldr();

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
 * Creates history object
 */
    virtual void buildData(PsNamedObject* inOwner);

/**
 * Finishes build for history object
 */
    virtual void buildSolvers();

/**
 * Initializes diagnostics
 */
    virtual void initialize();

/**
 * Updates all PsHistory's
 *
 * @param t simulation time
 */
    virtual void update(double t);

/**
 * Dumps PsHistory data
 */
    virtual void dump(TxIoBase* txIoPtr);

/**
 * Gets the length of the histories vector
 */
    virtual size_t getNumHistories() {
      return numHistories;
    };

/**
 * Sets timestep for histories to compare for update step
 *
 * @param tstep simulation time step
 */
    virtual void setHistTimeStep(size_t tstep) {
      simTimeStep = tstep;
    };

  protected:

    /** Global simulation timestep */
    size_t simTimeStep;

    /** Number of histories, length of histories vector */
    size_t numHistories;

    /** Vector of pointers to the base PsHistory objects */
    std::vector< PsHistoryBase<FLOATTYPE, NDIM>* > histories;

  private:

    /** List of names for the histories */
    std::vector< std::string > historyNames;

    /** List of attrib sets for the histories */
    std::vector< TxHierAttribSetIntDbl > historyAttribs;

    /** Make private to prevent use */
    PsHistHldr(const PsHistHldr<FLOATTYPE, NDIM>& vphh);

    /** Make private to prevent use */
    PsHistHldr<FLOATTYPE, NDIM>& operator=(
        const PsHistHldr<FLOATTYPE, NDIM>& vphh);

};
#endif // PS_HIST_HLDR_H
