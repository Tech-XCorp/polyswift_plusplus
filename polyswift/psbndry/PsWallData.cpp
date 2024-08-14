/**
 * @file    PsWallData.cpp
 *
 * @brief   Class containing field data for an arbitrary boundary
 *
 * @version $Id: PsWallData.cpp 8257 2007-09-12 22:05:20Z swsides $
 *
 * Copyright &copy; 2009-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsWallData.h>

//
// Constructor
//
template <class FLOATTYPE, size_t NDIM>
PsWallData<FLOATTYPE, NDIM>::PsWallData() {
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsWallData<FLOATTYPE, NDIM>::~PsWallData() {
}

//
// Set the attributes: dont need?
//
template <class FLOATTYPE, size_t NDIM>
void PsWallData<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Using base class to set attributes for base polymer class
  PsDynObjBase::setAttrib(tas);
}

//
// Build data
//
template <class FLOATTYPE, size_t NDIM>
void PsWallData<FLOATTYPE, NDIM>::buildData() {

  // Scoping call to parent class
  PsBndryData<FLOATTYPE, NDIM>::buildData();
}

//
// Build solvers
//
template <class FLOATTYPE, size_t NDIM>
void PsWallData<FLOATTYPE, NDIM>::buildSolvers() {

  // Scoping call to parent class
  PsBndryData<FLOATTYPE, NDIM>::buildSolvers();
}

//
// check contact... for now fixed walls shouldnt overlap!
//
template <class FLOATTYPE, size_t NDIM>
bool PsWallData<FLOATTYPE, NDIM>::inContact(
         PsWallData<FLOATTYPE, NDIM>* wallBndry) {

  bool inCont=false;
  return inCont;
}

// Instantiation
template class PsWallData<float, 1>;
template class PsWallData<float, 2>;
template class PsWallData<float, 3>;

template class PsWallData<double, 1>;
template class PsWallData<double, 2>;
template class PsWallData<double, 3>;
