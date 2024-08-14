/**
 * @file    PsProbeSphere.cpp
 *
 * @brief   Class for purely translate-able nanoparticles with interactions
 *
 * @version $Id: PsProbeSphere.cpp 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psbndry includes
#include <PsProbeSphere.h>

// psstd includes
#include <PsRandom.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsProbeSphere<FLOATTYPE, NDIM>::PsProbeSphere() {
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsProbeSphere<FLOATTYPE, NDIM>::~PsProbeSphere() {
}

template <class FLOATTYPE, size_t NDIM>
void PsProbeSphere<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

// Scoping call to base class
  PsInteractingSphere<FLOATTYPE, NDIM>::setAttrib(tas);
}

template <class FLOATTYPE, size_t NDIM>
void PsProbeSphere<FLOATTYPE, NDIM>::buildData() {

// Scoping call to base class
  PsInteractingSphere<FLOATTYPE, NDIM>::buildData();
}

template <class FLOATTYPE, size_t NDIM>
void PsProbeSphere<FLOATTYPE, NDIM>::buildSolvers() {

// Scoping call to base class...
  PsInteractingSphere<FLOATTYPE, NDIM>::buildSolvers();

  this->dbprt("PsProbeSphere::buildSolvers() ");
}

template <class FLOATTYPE, size_t NDIM>
void PsProbeSphere<FLOATTYPE, NDIM>::initialize() {

// Scoping call to base class
  PsInteractingSphere<FLOATTYPE, NDIM>::initialize();
}

//
// Update steps include:
//
//    1. calculate forces on all pure-translatable particles
//    2. make trial translations based on forces
//    3. and makes movePtcl calls
//
template <class FLOATTYPE, size_t NDIM>
void PsProbeSphere<FLOATTYPE, NDIM>::update(double t) {

  this->dbprt("PsProbeSphere::update(t)" );

// Get simulation timestep and present domain step
  size_t timeStep = PsDynObjBase::getCurrDomainStep();
  double tpresent = (double)timeStep;

// Update present timestep --> "next" timestep:
  if (t > tpresent) {

// Only update for specified interval
    if (timeStep % this->updateMovePeriod == 0) {

      this->dbprt("particle update at timestep = ", (int)timeStep);

      this->calculateForces();             // Find forces,
      this->moveCheckAllPtcls();           // Move all particles, check overlaps
    }

// SWS: checking for safety
    this->getCommBase().barrier();
  }

// Scoping call to base class
  PsSphere<FLOATTYPE, NDIM>::update(t);
}

// Instantiate classes
template class PsProbeSphere<float, 1>;
template class PsProbeSphere<float, 2>;
template class PsProbeSphere<float, 3>;

template class PsProbeSphere<double, 1>;
template class PsProbeSphere<double, 2>;
template class PsProbeSphere<double, 3>;

