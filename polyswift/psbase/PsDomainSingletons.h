/**
 *
 * @file    PsDomainSingletons.h
 *
 * @brief   Interface for the PsDomainSingletons class.
 *
 * @version $Id: PsDomainSingletons.h 6561 2007-02-05 03:47:11Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_DOMAIN_SINGLETONS_H
#define PS_DOMAIN_SINGLETONS_H

// std includes
#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

//#include <PsCommBase.h>

/*
 * Forward reference all pointees
 * PsField (and others) must be included as forward
 * references to avoid circular includes from
 * PsDomainSingleton inclusion in PsDynObj.
 * Forward reference as possible all pointees, for type defs below
 */
template <class FLOATTYPE, size_t NDIM> class PsGridBase;
template <class FLOATTYPE, size_t NDIM> class PsCommBase;

/**
 * A PsDomainSingletons object holds pointers to all of
 * the global first-rank objects that must exist.  See headers.
 */
template <class FLOATTYPE, size_t NDIM>
class PsDomainSingletons {

  public:

/**
 * Default constructor
 */
    PsDomainSingletons();

/**
 * Destructor
 */
    virtual ~PsDomainSingletons();

/**
 * Return the current simulation time step
 *
 * @return the current simulation time step
 */
    virtual size_t getSimulationTimeStep() const {
      return simulationTimeStep;
    }

/**
 * Set the current simulation time step
 *
 */
    virtual void setSimulationTimeStep(size_t ts) {
      simulationTimeStep = ts;
    }

/**
 * Set the grid object
 *
 * @param dsPtr the grid object
 */
    virtual void setGridBase(PsGridBase<FLOATTYPE, NDIM>* dsPtr) {
      gridObjPtr = dsPtr;
    }

/**
 * Get a reference to the grid.
 *
 * @return a reference to the grid
 */
    virtual PsGridBase<FLOATTYPE, NDIM>& getGridBase() {
      return *gridObjPtr;
    }

/**
 * Set the communicator and rank
 *
 * @param dsPtr the communicator
 */
     virtual void setCommBase(PsCommBase<FLOATTYPE, NDIM>* dsPtr) {
       commPtr = dsPtr;
    }

     // For DynObj forward ref problem
     // SWS: simplify this...?
/**
 * Get proc rank
 *
 * @return rank
 */
     virtual size_t getDomRank() {
       return domRank;
     }

/**
 * Set proc rank in dom sings
 *
 * @param drnk domain rank
 */
     virtual void setDomRank(size_t drnk) {
       domRank = drnk;
     }

/**
 * Get a reference to the communicator.
 *
 * return a reference to the communicator
 */
     virtual PsCommBase<FLOATTYPE, NDIM>& getCommBase() {
       return *commPtr;
     }

/**
 * Set the output file prefix
 *
 * @param fname the file prefix string
 */
     virtual void setOutputFilePrefix(std::string fname) {
       outputFilePrefix = fname;
     }

/**
 * Get the output file prefix
 *
 * @return file prefix name string
 */
     virtual std::string& getOutputFilePrefix() {
       return outputFilePrefix;
     }

  protected:

     /** The simulation time step */
     size_t simulationTimeStep;

  private:

     /** Rank for print methods */
     size_t domRank;

     /** Prefix for all dumpfile names */
     std::string outputFilePrefix;

     /** Grid object pointer */
     PsGridBase<FLOATTYPE, NDIM>* gridObjPtr;

     /** Communicator object pointer */
     PsCommBase<FLOATTYPE, NDIM>* commPtr;

     /** Private to prevent use */
     PsDomainSingletons(const PsDomainSingletons<FLOATTYPE, NDIM>& vds);

     /** Private to prevent use */
     PsDomainSingletons<FLOATTYPE, NDIM>& operator=(
         const PsDomainSingletons<FLOATTYPE, NDIM>& vds);

};

#endif // PS_DOMAIN_SINGLETONS_H

