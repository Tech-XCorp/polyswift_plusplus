/**
 *
 * @file    PsConstraintUpdater.cpp
 *
 * @brief   Class for updating physical fields in simulation model
 *
 * @version $Id: PsConstraintUpdater.cpp 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// txbase includes
#include <TxTensor.h>

// psbase includes
#include <PsConstraintUpdater.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsConstraintUpdater<FLOATTYPE, NDIM>::PsConstraintUpdater() {

  constraintFieldPtr = NULL;
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsConstraintUpdater<FLOATTYPE, NDIM>::~PsConstraintUpdater() {
}

//
//
//
template <class FLOATTYPE, size_t NDIM>
void PsConstraintUpdater<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  this->dbprt("PsConstraintUpdater::setAttrib() ");
}

template <class FLOATTYPE, size_t NDIM>
void PsConstraintUpdater<FLOATTYPE, NDIM>::buildData() {

  this->dbprt("PsConstraintUpdater::buildData() ");
}

template <class FLOATTYPE, size_t NDIM>
void PsConstraintUpdater<FLOATTYPE, NDIM>::buildSolvers() {

  // Scoping call to base class
  PsUpdater<FLOATTYPE, NDIM>::buildSolvers();

  this->dbprt("PsConstraintUpdater::buildSolvers() ");

  // Get default constraint field, set related field pointers
  constraintFieldPtr =
    PsNamedObject::getObject<PsPhysField<FLOATTYPE, NDIM> >("defaultPressure");
}

template <class FLOATTYPE, size_t NDIM>
void PsConstraintUpdater<FLOATTYPE, NDIM>::initialize() {
}

//
//
//
template <class FLOATTYPE, size_t NDIM>
void PsConstraintUpdater<FLOATTYPE, NDIM>::update(double t) {

  this->dbprt("PsConstraintUpdater::update() ");
}

// Instantiate classes
template class PsConstraintUpdater<float, 1>;
template class PsConstraintUpdater<float, 2>;
template class PsConstraintUpdater<float, 3>;

template class PsConstraintUpdater<double, 1>;
template class PsConstraintUpdater<double, 2>;
template class PsConstraintUpdater<double, 3>;
