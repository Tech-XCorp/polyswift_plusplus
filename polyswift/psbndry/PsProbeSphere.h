/**
 * @file    PsProbeSphere.h
 *
 * @brief   Class for purely translate-able nanoparticles with interactions
 *          and measuring forces on probe particle
 *
 * @version $Id: PsProbeSphere.h 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_PROBE_SPHERE_H
#define PS_PROBE_SPHERE_H

// std includes
#include <vector>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psptcl includes
#include <PsInteractingSphere.h>
#include <PsSphereData.h>

/**
 * Measuring forces on probe particle. Position updates are
 * pre-determined and forces dumped to file
 *
 * @param FLOATTYPE the data type of simulation
 * @param NDIM the dimensionality of simulation
 */
template <class FLOATTYPE, size_t NDIM>
class PsProbeSphere : public PsInteractingSphere<FLOATTYPE, NDIM> {

  public:

/**
 * constructor
 */
  PsProbeSphere();

/**
 * Destructor
 */
  virtual ~PsProbeSphere();

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

  private:

    /** Result data pointer for FFT calcs */
    FLOATTYPE* resPtr;

    /** Constructor private to prevent use */
    PsProbeSphere(const PsProbeSphere<FLOATTYPE, NDIM>& psbcp);

    /** Assignment private to prevent use */
    PsProbeSphere<FLOATTYPE, NDIM>& operator=(
      const PsProbeSphere<FLOATTYPE, NDIM>& psbcp);

};

#endif // PS_PROBE_SPHERE_H
