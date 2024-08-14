/**
 *
 * @file    PsEffHamil.h
 *
 * @brief   Specifies energetics of field-theoretic model
 *
 * @version $Id: PsEffHamil.h 6319 2006-11-14 22:39:46Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_EFF_HAMIL_H
#define PS_EFF_HAMIL_H

// std includes
#include <vector>
#include <map>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// txbase includes
#include <TxTensor.h>

// psbase includes
#include <PsPhysField.h>
#include <PsEffHamilBase.h>
#include <PsCommBase.h>

// pseffhamil includes
#include <PsInteraction.h>
#include <PsUpdater.h>

/**
 * Specifies energetics of field-theoretic model
 * Also holds other parts of update model...
 *   - stores pointers to physical pointers
 *   - parses Interaction blocks
 *   - parses Updater blocks
 *
 * @param FLOATTYPE the data type of simulation
 * @param NDIM the dimensionality of simulation
 */
template <class FLOATTYPE, size_t NDIM>
class PsEffHamil : public PsEffHamilBase<FLOATTYPE, NDIM> {

  public:

/**
 * constructor
 */
  PsEffHamil();

/**
 * Destructor
 */
  virtual ~PsEffHamil();

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
 * Update effective Hamiltonian
 *
 * @param t simulation time
 */
    virtual void update(double t);

/**
 * Dump the object
 */
    virtual void dump();

  protected:

    //
    //  Interaction data
    //

    /** Number of interactions */
    size_t numInteractions;

    /** List of interaction model pointers */
    std::vector< PsInteraction<FLOATTYPE, NDIM>* > interactions;

    //
    // Updater SCFT scheme/ and others
    //

    /** Number of interactions */
    size_t numUpdaters;

    /** Updater pointer */
    std::vector< PsUpdater<FLOATTYPE, NDIM>* >  updaters;

  private:

    /** Flag for incompressible pressure field */
    bool hasPresField;

    /** Sequence order of updaters */
    std::vector< std::string > updaterSequence;

    /** Updater name */
    std::vector< std::string > updaterNames;

    /** List of attrib sets for updaters */
    std::vector< TxHierAttribSetIntDbl > updaterAttribs;

    /** List of interaction names */
    std::vector< std::string > interactionNames;

    /** List of attrib sets for interactions */
    std::vector< TxHierAttribSetIntDbl > interactionAttribs;

    /** Name of free-energy field to be made available */
    std::string availFeName;

    /** Constructor private to prevent use */
    PsEffHamil(const PsEffHamil<FLOATTYPE, NDIM>& psbcp);

   /** Assignment private to prevent use */
    PsEffHamil<FLOATTYPE, NDIM>& operator=(
      const PsEffHamil<FLOATTYPE, NDIM>& psbcp);

};

#endif // PS_EFF_HAMIL_H
