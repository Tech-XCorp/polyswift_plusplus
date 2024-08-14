/**
 *
 * @file    PsDecompRegular.h
 *
 * @brief   Class containing info for a regular decomposition
 *
 * @version $Id: PsDecompRegular.h 6319 2006-11-14 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_DECOMP_REGULAR_H
#define PS_DECOMP_REGULAR_H

// std includes
#include <vector>
#include <map>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psgrid includes
#include <PsDecomp.h>

/**
 * Class containing info for a regular decomposition
 *
 * @param FLOATTYPE the data type of simulation
 * @param NDIM the dimensionality of simulation
 */
template <class FLOATTYPE, size_t NDIM>
class PsDecompRegular : public PsDecomp<FLOATTYPE, NDIM> {

  public:

/**
 * constructor
 */
  PsDecompRegular();

/**
 * Destructor
 */
  virtual ~PsDecompRegular();

/**
 * Store the data needed to build this object.
 *
 * @param tas the attribute set containing the initial conditions
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Build the solvers for this object
 */
    virtual void buildSolvers();

/**
 * Build the data for this object
 */
    virtual void buildData();

/**
 * Get shift values to map local indices to global indices
 * for a regular decomposition on Cartesian grid
 *
 * @return vector of local to global shifts
 */
    virtual std::vector<size_t> getLocalToGlobalShifts() {
      return localToGlobalShifts;
    }

/**
 * Check if decomp owned point specified in globalPos
 *
 * @param globalPos std::vector of global position
 */
    virtual bool hasPosition(std::vector<int> globalPos);

/**
 * Check if decomp owned point specified in globalPos
 *
 * @param globalPos PsTinyVector of global position
 */
    virtual bool hasPosition(PsTinyVector<int, NDIM> globalPos);

  protected:

    /** Vector of shift values mapping local grid indices to global */
    std::vector<size_t> localToGlobalShifts;

  private:

    /** Constructor private to prevent use */
    PsDecompRegular(const PsDecompRegular<FLOATTYPE, NDIM>& psbcp);

   /** Assignment private to prevent use */
    PsDecompRegular<FLOATTYPE, NDIM>& operator=(
      const PsDecompRegular<FLOATTYPE, NDIM>& psbcp);

};

#endif // PS_DECOMP_REGULAR_H
