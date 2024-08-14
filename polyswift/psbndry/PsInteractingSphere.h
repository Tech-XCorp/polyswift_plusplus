/**
 * @file    PsInteractingSphere.h
 *
 * @brief   Class for purely translate-able nanoparticles with interactions
 *
 * @version $Id: PsInteractingSphere.h 6319 2006-11-14 22:39:46Z swsides $
 *
 * Copyright &copy; 2009-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_INTERACTING_SPHERE_H
#define PS_INTERACTING_SPHERE_H

// std includes
#include <vector>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsFieldBase.h>
#include <PsInteraction.h>

// psptcl includes
#include <PsSphere.h>
#include <PsSphereData.h>

/**
 * Holds methods that perform operations on nanoparticles whose
 * spatial position is updated by pure translation. Dynamic casts
 * to appropriate data class (ie PsSphereData) for updating specific
 * nanoparticle data. Holds physical field that defines the interaction
 * of nanoparticle surface with enviroment
 *
 * @param FLOATTYPE the data type of simulation
 * @param NDIM the dimensionality of simulation
 */
template <class FLOATTYPE, size_t NDIM>
class PsInteractingSphere : public PsSphere<FLOATTYPE, NDIM> {

  public:

/**
 * constructor
 */
  PsInteractingSphere();

/**
 * Destructor
 */
  virtual ~PsInteractingSphere();

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
 * Build the partices... obtain pointers to PsBndryDataBase interface
 * class for spherical data class and put into ptclGroup list
 */
    virtual void buildSolvers();

/**
 * Initialize object before update steps
 */
    virtual void initialize();

/**
 * Update the purely translatable particles in ptclGroup
 *
 * @param t update time
 */
    virtual void update(double t);

  protected:

    /** Force-field workspace */
    PsFieldBase<FLOATTYPE>* forceField;

    /**
     * Component-wise force-field whole domain
     * Forces on particles obtained from the collocation points coinciding
     *  with particle centers
     */
    std::vector< PsFieldBase<FLOATTYPE>* > forceFieldVec;

    /**
     * Calculate forces for translation on interacting particles
     * Will be added to other forces on particle
     */
    void calculateForces();

    /** Result data pointer for FFT calcs */
    FLOATTYPE* resPtr;

    /** Number of domain update iterations between particle moves steps */
    size_t updateMovePeriod;

  private:

    /** Constructor private to prevent use */
    PsInteractingSphere(const PsInteractingSphere<FLOATTYPE, NDIM>& psbcp);

    /** Assignment private to prevent use */
    PsInteractingSphere<FLOATTYPE, NDIM>& operator=(
      const PsInteractingSphere<FLOATTYPE, NDIM>& psbcp);

};

#endif // PS_INTERACTING_SPHERE_H
