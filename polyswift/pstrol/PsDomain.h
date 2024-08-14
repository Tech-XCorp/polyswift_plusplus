/**
 * @file    PsDomain.h
 *
 * @brief   To setup the simulation on individual processors.
 *
 * @version $Id: PsDomain.h 7984 2007-08-09 22:22:54Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_DOMAIN_H
#define PS_DOMAIN_H

// pstrol includes
#include <PsDomainBase.h>
#include <PsDomMakerMaps.h>

// includes for default singletons
#include <PsDomainSingletons.h>
#include <PsGridBase.h>
#include <PsDecompBase.h>
#include <PsCommBase.h>

// polyswift includes
#include <PsCommHldr.h>
#include <PsBndryHldr.h>
#include <PsFFTHldr.h>
#include <PsGridHldr.h>
#include <PsDecompHldr.h>
#include <PsPolymerHldr.h>
#include <PsSolventHldr.h>
#include <PsEffHamilHldr.h>
#include <PsPhysFieldHldr.h>
#include <PsHistHldr.h>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// std includes
#include <string>
#include <vector>

//  txio includes
#include <TxIoBase.h>
#include <TxHdf5Base.h>
#ifdef HAVE_MPI
#include <TxMpiBase.h>
#else
#include <TxSelfBase.h>
#endif

/**
 *  Templated class that contains all the information
 *  for the domains that live on each processor
 */
template <class FLOATTYPE, size_t NDIM>
class PsDomain : public PsDomainBase {

  public:

/**
 * Constructor
 */
    PsDomain();

/**
 * Destructor
 */
    virtual ~PsDomain();

/**
 * Set the attributes of this domain and create the bulk
 * of the domain singletons
 *
 * @param tha the hierarchical attribute set describing domain
 * @param restoreNum index of the files to use to restore
 * @param runName string of job name from input file
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tha,
                           size_t restoreNum,
                           std::string runName);

/**
 * Build the solvers for this domain.
 */
    virtual void buildSolvers();

/**
 * Build the data for this domain.
 */
    virtual void buildData();

/**
 * Initialize domain on first start-up.
 */
    virtual void initialize();

/**
 * Restore domain on restart
 */
    virtual void restore();

/**
 * Update the domain
 *
 * @param t the time to update all to.
 */
    virtual void update(double t);

/**
 * Dump data to file
 */
    virtual void dump();

  protected:

    /** Local debug flag */
    DebugPrint dbStatus;

    /** The domain singletons contain all info needed to connect an object */
    PsDomainSingletons<FLOATTYPE, NDIM> domSings;

    /* ******************************************************* */
    // The Base Class Holders

    /** The grid object holder */
    PsGridHldr<FLOATTYPE, NDIM> gridHldr;

    /** The communication object holder */
    PsCommHldr<FLOATTYPE, NDIM> commHldr;

    /** The decomp object holder */
    PsDecompHldr<FLOATTYPE, NDIM> decompHldr;

    /** The grid object holder */
    PsFFTHldr<FLOATTYPE, NDIM> fftHldr;

    /** The polymers holder */
    PsPolymerHldr<FLOATTYPE, NDIM> polymerHldr;

    /** The solvents holder */
    PsSolventHldr<FLOATTYPE, NDIM> solventHldr;

    /** The physical observable field holder */
    PsPhysFieldHldr<FLOATTYPE, NDIM> physFieldHldr;

    /** The effective Hamiltonian holder */
    PsEffHamilHldr<FLOATTYPE, NDIM> effHamilHldr;

    /** The boundary holder */
    PsBndryHldr<FLOATTYPE, NDIM> bndryHldr;

    /** The diagnostics/history holder */
    PsHistHldr<FLOATTYPE, NDIM> histHldr;

    /** Random number seed */
    size_t randomSeed;

    /** Number of saddle-point updates */
    size_t nsteps;

    /** Number of steps between "big" data output dumps */
    size_t dumpPeriodicity;

    /** The current time step */
    size_t currentStep;

    /** Sequence tracking: times data written to file */
    // Might make this same of currentStep
    size_t seqNumber;

    /** The actual time step from the last update time */
    double timeStep;

  private:

/**
 * Increase the dump number by one.
 */
    virtual void bumpDumpNum() {seqNumber++;}

/// comm for IO
    TxCommBase* comm;

/// IO object
    TxIoBase* txIoPtr;

/// Default grid for domain singletons
    std::string defaultGrid;

/// Base name for run
    std::string baseName;

/// Process rank of domain
    int thisRank;

/// Make private to prevent use
    PsDomain(const PsDomain<FLOATTYPE, NDIM>& vpd);

/// Make private to prevent use
    PsDomain<FLOATTYPE, NDIM>& operator=(
        const PsDomain<FLOATTYPE, NDIM>& vpd);

/// The stored settings for this domain
    TxHierAttribSetIntDbl domainSettings;

};

#endif // PS_DOMAIN_H
