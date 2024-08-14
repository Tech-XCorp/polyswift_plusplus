/**
 *
 * @file    PsDecompFFTW.h
 *
 * @brief   Class containing info for slab decomposition from FFTW
 *
 * @version $Id: PsDecompFFTW.h 6319 2006-11-14 22:39:46Z swsides $
 *
 * Copyright &copy; 2009-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_DECOMP_FFTW_H
#define PS_DECOMP_FFTW_H

// std includes
#include <vector>
#include <map>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_MPI
#define MPICH_IGNORE_CXX_SEEK
#include <mpi.h>
#include <fftw_mpi.h>
#else
#include <fftw.h>
#endif

// psdecomp includes
#include <PsDecomp.h>

// psbase includes
#include <PsFFTBase.h>

/**
 * Class containing info for a slab (FFTW) decomposition
 *
 * @param FLOATTYPE the data type of simulation
 * @param NDIM the dimensionality of simulation
 */
template <class FLOATTYPE, size_t NDIM>
class PsDecompFFTW : public PsDecomp<FLOATTYPE, NDIM> {

  public:

/**
 * constructor
 */
  PsDecompFFTW();

/**
 * Destructor
 */
  virtual ~PsDecompFFTW();

/**
 * Store the data needed to build this object.
 *
 * @param tas the attribute set containing the initial conditions
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Single build method for special decomp object
 */
    virtual void build();

/**
 * Get shift values to map local indices to global indices
 * for a slab decomposition on Cartesian grid
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

  private:

    /** Vector of values mapping local grid indices to global */
    std::vector<size_t> localToGlobalShifts;

    /** Flag to grab transpose plan results from FFTW */
    bool transposeFlag;

    /** Constructor private to prevent use */
    PsDecompFFTW(const PsDecompFFTW<FLOATTYPE, NDIM>& psbcp);

   /** Assignment private to prevent use */
    PsDecompFFTW<FLOATTYPE, NDIM>& operator=(
      const PsDecompFFTW<FLOATTYPE, NDIM>& psbcp);

};

#endif // PS_DECOMP_FFTW_H
