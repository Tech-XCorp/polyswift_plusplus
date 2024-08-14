/**
 * @file    PsFloryConstChi.h
 *
 * @brief   Tracks uniform chi paramater
 *
 * @version $Id: PsFloryConstChi.h 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_FLORY_CONST_CHI_H
#define PS_FLORY_CONST_CHI_H

// std includes
#include <vector>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include<PsInteraction.h>

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
class PsFloryConstChi : public PsCommHistory<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE> {

  public:

/**
 * constructor
 */
  PsFloryConstChi() {}

/**
 * Destructor
 */
  virtual ~PsFloryConstChi() { }

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

    /** Pointer to interaction object */
    PsInteraction<FLOATTYPE, NDIM>* interactPtr;

  private:

    /** Name of interaction object */
    std::string interactionName;

/**
 * Constructor private to prevent use
 */
    PsFloryConstChi(const PsFloryConstChi<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>& psfe);

/**
 * Assignment private to prevent use
 */
    PsFloryConstChi<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>& operator=(
       const PsFloryConstChi<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>& psfe);

};

#endif  // PS_FLORY_CONST_CHI_H
