/**
 * @file    PsFloryChiAtPoint.h
 *
 * @brief   Tracks uniform chi paramater
 *
 * @version $Id: PsFloryChiAtPoint.h 1346 2011-08-18 19:17:57Z swsides $
 *
 * Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_FLORY_CHI_AT_POINT_H
#define PS_FLORY_CHI_AT_POINT_H

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
class PsFloryChiAtPoint : public PsCommHistory<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE> {

  public:

/**
 * constructor
 */
  PsFloryChiAtPoint() {}

/**
 * Destructor
 */
  virtual ~PsFloryChiAtPoint() { }

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

/** List of sizes for each tensor dimension */
    size_t histDataDims[1];

/** DATATYPE for history datum */
    TxTensor<ELEMENTTYPE, 1> histDatum;

/** Number of dump field components */
    size_t numDumpComp;

/** Number of grid cells/per spec cells */
    std::vector<size_t> chiGridPoint;

/** Vector of max values vs proc */
    std::vector<FLOATTYPE> maxLocalVec;

/** Vector of min values vs proc */
    std::vector<FLOATTYPE> minLocalVec;

  private:

/**
 * Helper method to find maximum value of chi
 *
 * @param fld field of chi values
 * @return max value in chi across all procs
 */
    FLOATTYPE findMaxChi(PsGridField<FLOATTYPE, NDIM>& fld);

/**
 * Helper method to find minimum value of chi
 *
 * @param fld field of chi values
 * @return min value in chi across all procs
 */
    FLOATTYPE findMinChi(PsGridField<FLOATTYPE, NDIM>& fld);

/**
 * Helper method to find chi at grid point. If point not
 * owned its set to zero and communication between procs
 * accounts for this correctly.
 *
 * @param fld field of chi values
 * @return max val chi across all procs
 */
    FLOATTYPE findChiAtPoint(PsGridField<FLOATTYPE, NDIM>& fld);

/** Name of interaction object */
    std::string interactionName;

/** Constructor private to prevent use */
    PsFloryChiAtPoint(const PsFloryChiAtPoint<FLOATTYPE, NDIM,
                      ELEMENTTYPE, DATATYPE>& psfe);

/** Assignment private to prevent use */
    PsFloryChiAtPoint<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>& operator=(
       const PsFloryChiAtPoint<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>& psfe);

};

#endif // PS_FLORY_CHI_AT_POINT_H
