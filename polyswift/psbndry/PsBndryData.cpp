/**
 * @file    PsBndryData.cpp
 *
 * @brief   Class containing field data structures for boundaries
 *
 * @version $Id: PsBndryData.cpp 8257 2007-09-12 22:05:20Z swsides $
 *
 * Copyright &copy; 2009-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsBndryData.h>

//
// Constructor
//
template <class FLOATTYPE, size_t NDIM>
PsBndryData<FLOATTYPE, NDIM>::PsBndryData() {

  this->dbprt("PsBndryData() 0-arg constructor called");

  // Grid pointer (needed by inContact functions
  gridObjPtr = NULL;

  // Size of field space
  numFieldElements = 0;

  // Set default particle index negative for non-membership in ptclGroup
  bndryIndex = -1;

  // Move attempts
  moveAttempts = 0;
}

//
// Destructor
//
template <class FLOATTYPE, size_t NDIM>
PsBndryData<FLOATTYPE, NDIM>::~PsBndryData() {

  // Clear std::vectors
  fieldPos.clear();
  fieldVals.clear();
}

//
// Set the attributes: dont need?
//
template <class FLOATTYPE, size_t NDIM>
void PsBndryData<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Using base class to set attributes for base polymer class
  PsDynObjBase::setAttrib(tas);
}

// Initialize basic field data etc.
template <class FLOATTYPE, size_t NDIM>
void PsBndryData<FLOATTYPE, NDIM>::setBndryData(
       PsBndryDataBase<FLOATTYPE, NDIM>& bdata) {

  this->dbprt("PsBndryData::setBndryData() ");

  bndryIndex       = bdata.getBndryIndex();
  numFieldElements = bdata.getNumFieldElements();
  fieldPos         = bdata.getFieldPos();
  fieldVals        = bdata.getFieldVals();
}

// Build data
template <class FLOATTYPE, size_t NDIM>
void PsBndryData<FLOATTYPE, NDIM>::buildData() {
}

// Build solvers
template <class FLOATTYPE, size_t NDIM>
void PsBndryData<FLOATTYPE, NDIM>::buildSolvers() {

  this->dbprt("PsBndryData::buildSolvers() ");

  // Get grid method pointer
  // SWS: this may very well conflict with the grid set in singletons
  // SWS: Setup in constructor and do away with buildData/Solvers calls
  // SWS:      and inheirtance from DynObj????
  gridObjPtr = &this->getGridBase();
  if (!gridObjPtr) {
    TxDebugExcept tde("PsBndryData::buildSolvers: Grid ptr not set");
    tde << " in <PsBndryData " << this->getName() << " >";
    throw tde;
  }

}

// Instantiation
template class PsBndryData<float, 1>;
template class PsBndryData<float, 2>;
template class PsBndryData<float, 3>;

template class PsBndryData<double, 1>;
template class PsBndryData<double, 2>;
template class PsBndryData<double, 3>;
