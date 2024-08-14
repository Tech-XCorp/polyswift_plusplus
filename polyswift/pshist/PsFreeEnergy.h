/**
 * @file    PsFreeEnergy.h
 *
 * @brief   Tracks free-energy from Hamiltonian
 *
 * @version $Id: PsFreeEnergy.h 11989 2009-03-25 05:14:55Z dws $
 *
 * Copyright &copy; 2009-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_FREE_ENERGY_H
#define PS_FREE_ENERGY_H

// std includes
#include <vector>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include<PsUpdater.h>

// pshist includes
#include <PsCommHistory.h>

/**
 * Calculates free energy from some simulation object
 *
 * @param FLOATTYPE the data type of simulation
 * @param NDIM the dimensionality of simulation
 * @param ELEMENTTYPE data type of elements in DATATYPE
 * @param DATATYPE data type history structure (eg scalar, TxTensor<>...)
 */
template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
class PsFreeEnergy : public PsCommHistory<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE> {

  public:

/**
 * constructor
 */
    PsFreeEnergy() {
      bool calcDisorder = true;
      // std::cout << "PsFreeEnergy created" << std::endl;
    }

/**
 * Destructor
 */
    virtual ~PsFreeEnergy() {
    }

/**
 * Store the data needed to build this object.
 *
 * @param tas the attribute set containing the initial conditions
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Build the data for this object
 */
    virtual void buildData();

/**
 * Build the solvers for this object
 */
    virtual void buildSolvers();

/**
 * History updater
 */
    virtual void update(double t);

/**
 * Dump attribute method
 */
    virtual void setHistAttributes();

  protected:

    /** Pointer to hamiltonian object */
    PsUpdater<FLOATTYPE, NDIM>* updaterPtr;

  private:

    /** Flag to exclude disorder free-energy */
    bool calcDisorder;

    /** Name of updater object */
    std::string updaterName;

/**
 * Constructor private to prevent use
 */
    PsFreeEnergy(const PsFreeEnergy<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>& psfe);

/**
 * Assignment private to prevent use
 */
    PsFreeEnergy<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>& operator=(
       const PsFreeEnergy<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>& psfe);

};

#endif  // PS_FREE_ENERGY_H
