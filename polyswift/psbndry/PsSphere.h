/**
 * @file    PsSphere.h
 *
 * @brief   Class for purely translate-able nanoparticles
 *
 * @version $Id: PsSphere.h 6319 2006-11-14 22:39:46Z swsides $
 *
 * Copyright &copy; 2009-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_SPHERE_H
#define PS_SPHERE_H

#include <cmath> // for round()

// std includes
#include <vector>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psstd includes
#include <PsSTFunc.h>
#include <PsTinyVector.h>
#include <PsTinyMatrix.h>

// psbase includes
#include <PsFieldBase.h>

// psptcl includes
#include <PsNanoPtcl.h>
#include <PsSphereData.h>

/**
 * Holds methods that perform operations on nanoparticles whose
 * spatial position is updated by pure translation. Dynamic casts
 * to appropriate data class (ie PsSphereData) for updating specific
 * nanoparticle data.
 *
 *
 * @param FLOATTYPE the data type of simulation
 * @param NDIM the dimensionality of simulation
 */
template <class FLOATTYPE, size_t NDIM>
class PsSphere : public PsNanoPtcl<FLOATTYPE, NDIM> {

  public:

/**
 * constructor
 */
  PsSphere();

/**
 * Destructor
 */
  virtual ~PsSphere();

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
 * Build the particles... obtain pointers to PsBndryDataBase interface
 * class for spherical data class and put into ptclGroup list
 */
    virtual void buildSolvers();

/**
 * Initialize object at start of sim
 */
    virtual void initialize();

/**
 * Restore object on re-starts
 *
 * @param txIoPtr IO object pointer
 */
    virtual void restore(TxIoBase* txIoPtr);

/**
 * Dump particle center positions, needed for
 * analysis and restart
 *
 * @param txIoPtr IO object pointer
 */
    virtual void dump(TxIoBase* txIoPtr);

/**
 * Dump particle center positions, this is temporary
 * method needed for Visit database files to be sync-ed
 *
 * @param txIoPtr IO object pointer
 */
    virtual void dumpEmpty(TxIoBase* txIoPtr);

/**
 * Update the purely translatable particles in ptclGroup
 * Checks on updating the number of particles in group
 *
 * @param t update time
 */
    virtual void update(double t);

  protected:

    /**
     * Returns value of particle field
     *
     * @param rvec global position vector
     */
    virtual FLOATTYPE ptclFunc(PsTinyVector<int, NDIM>& rvec);

    /**
     * Setting template sphere particle data. For now, this
     * uses data members in parent class
     */
    virtual void setSphereData();

    /**
     * Uses the template particle data field to construct new particle,
     * creates data structures necessary for nanoparticle, randomly
     * moves to a trial position, and checks for overlaps with all
     * other nanoparticles present. Attempts this until no overlaps are found
     */
    virtual void buildPtcl();

    /**
     * Uses the templatePtclData field to construct new particle,
     * creates data structures necessary for nanoparticle, uses the
     * center position provided, and checks for overlaps with all
     * other nanoparticles present. Throws exception if overlaps found
     * (because this is used primarily for restores and no overlaps should
     * be present)
     */
    virtual void buildPtcl(PsTinyVector<int, NDIM>& ptclPos);

    /**
     * A driver for movePtcl method: Takes specified displacements,
     * makes trial movements and checks for overlaps and accepts/rejects
     */
    virtual void moveCheckAllPtcls();

    /**
     * Effective dynamics implementation: includes force scaling
     * to estimate displacement and checks for maximum displacement
     * in order to enforce stability. Includes fluctuating noise
     * contribution, and rounds to estimate position on the grid
     *
     * @param forceFld force field components for translation
     */
    virtual void calculateMoves(std::vector< PsFieldBase<FLOATTYPE>* >& forceFld);

    /**
     * Stores particle field info set initially, ostensibly from STFunc
     * This is stored as class data because particles are added throughout
     * simulation
     */
    PsSphereData<FLOATTYPE, NDIM> tplSphereData;

    /** Number of domain update iterations between particle moves steps */
    //    size_t updateMovePeriod;

    /** Number of domain update iterations between particle insertion */
    size_t updateAddPeriod;

    /** Number of domain update steps before first particle insertion */
    size_t tstepBeforeFirstAdd;

    /** Radius of spherical particle in terms of grid cells */
    size_t radius;

    /** Dynamic Radius of spherical ptcl in grid cells */
    size_t dynRadius;

    /** Width of interface */
    FLOATTYPE interfaceWidth;

  private:

    /** Input hierarchy set for STFunc-s */
    TxHierAttribSetIntDbl funcAttrib;

    /** Names of STFuncs */
    std::vector<std::string> funcNames;

    /** Function for setting boundary field */
    PsSTFunc<FLOATTYPE, NDIM>* insertSTFunc;

    /** Initial number of particles */
    size_t initialNumPtcls;

    /** Flag for skipping rotate moves */
    bool willSkipRotate;

    /** Fraction of Rg to set max displacement from force calc */
    FLOATTYPE rgFraction;

    /** Maximum allowed displacement in units of Rg */
    FLOATTYPE maxdr;

    /** Noise factor on displacment */
    FLOATTYPE ptclNoiseFactor;

    /** Effective Brownian dynamics scale factor */
    FLOATTYPE scaleForceFactor;

    /** Dataset name for ptcl IO */
    std::string ptclDataSetName;

    /**
     * Driver for movePtcl method. Attempt a move with dr
     * check for overlaps... keep move if no overlap, reverse
     * move if overlaps and return check.
     * Also, tracks ptclData move info.
     *
     * @param  ptclPtr pointer to partcle data interface to move
     * @param  dr      displacment vector for particle center
     * @return boolean for successful particle move
     */
    virtual bool movePtclwCheck(PsBndryDataBase<FLOATTYPE, NDIM>* ptclPtr,
                                PsTinyVector<int, NDIM> dr);

    /**
     * Translate nano-particle in space
     * This will be part of scoping call to update for derived
     * classes of PsSphere for non-spherical particles
     *
     * @param  ptclPtr pointer to partcle data interface to move
     * @param  dr      displacment vector for particle center
     */
    virtual void movePtcl(PsBndryDataBase<FLOATTYPE, NDIM>* ptclPtr,
                          PsTinyVector<int, NDIM> dr);

    /**
     * Append metadata to a dataset common to all particle files
     *
     * @param txIoPtr IO object pointer
     * @param fn file node
     * @param datasetName name string for dataset to append metadata
     */
    void appendMetaDataset(TxIoBase* txIoPtr,
                           TxIoNodeType fn,
                           std::string datasetName);

    /**
     * Append vizschema GROUP attributes
     *
     * @param txIoPtr IO object pointer
     * @param fn file node
     */
    void appendMetaGroup(TxIoBase* txIoPtr, TxIoNodeType fn);

    /** Rotation matrix around z-axis */
    PsTinyMatrix<int, NDIM, NDIM> rotZ;

    /** Rotation matrix around y-axis */
    PsTinyMatrix<int, NDIM, NDIM> rotY;

    /** Rotation matrix around x-axis */
    PsTinyMatrix<int, NDIM, NDIM> rotX;

    /** Rotation matrix around z-axis */
    PsTinyMatrix<int, NDIM, NDIM> rotNegZ;

    /** Rotation matrix around y-axis */
    PsTinyMatrix<int, NDIM, NDIM> rotNegY;

    /** Rotation matrix around x-axis */
    PsTinyMatrix<int, NDIM, NDIM> rotNegX;

    /** Rotation matrix list */
    std::vector< PsTinyMatrix<int, NDIM, NDIM> > rotMatrices;

    /** Constructor private to prevent use */
    PsSphere(const PsSphere<FLOATTYPE, NDIM>& psbcp);

    /** Assignment private to prevent use */
    PsSphere<FLOATTYPE, NDIM>& operator=(
      const PsSphere<FLOATTYPE, NDIM>& psbcp);

};

#endif // PS_SPHERE_H
