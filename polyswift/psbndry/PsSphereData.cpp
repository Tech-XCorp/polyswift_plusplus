/**
 * @file    PsSphereData.cpp
 *
 * @brief   Class containing field data structures for spherical nanoparticles
 *
 * @version $Id: PsSphereData.cpp 8257 2007-09-12 22:05:20Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsSphereData.h>

//
// Constructor
//
template <class FLOATTYPE, size_t NDIM>
PsSphereData<FLOATTYPE, NDIM>::PsSphereData() {

  this->dbprt("PsSphereData() 0-arg constructor called");

  // Initialize
  radius = 0.0;
  center = 0;
  dr = 0;
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsSphereData<FLOATTYPE, NDIM>::~PsSphereData() {}

//
// Set the attributes: dont need?
//
template <class FLOATTYPE, size_t NDIM>
void PsSphereData<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Using base class to set attributes for base polymer class
  PsDynObjBase::setAttrib(tas);
}

//
// Build data
//
template <class FLOATTYPE, size_t NDIM>
void PsSphereData<FLOATTYPE, NDIM>::buildData() {

  // Scoping call to parent class
  PsBndryData<FLOATTYPE, NDIM>::buildData();
}

//
// Build solvers
//
template <class FLOATTYPE, size_t NDIM>
void PsSphereData<FLOATTYPE, NDIM>::buildSolvers() {

  // Scoping call to parent class
  PsBndryData<FLOATTYPE, NDIM>::buildSolvers();
}

//
// check contact between "this" spherical particle and
// another spherical particle
//
template <class FLOATTYPE, size_t NDIM>
bool PsSphereData<FLOATTYPE, NDIM>::inContact(
         PsSphereData<FLOATTYPE, NDIM>* spherePtcl) {

  // Contact condition
  //  FLOATTYPE r1 = spherePtcl->getRadius();
  //  FLOATTYPE r2 = this->getRadius();
  FLOATTYPE r1 = spherePtcl->getDynamicRadius();
  FLOATTYPE r2 = this->getDynamicRadius();
  FLOATTYPE distContact = r1 + r2;

  // Find distance between spherical particle centers
  PsTinyVector<int, NDIM> center1 = spherePtcl->getCenter();
  PsTinyVector<int, NDIM> center2 = this->getCenter();
  FLOATTYPE dist = this->gridObjPtr->mapDistToGrid(center1, center2);
  bool inCont=false;

  if (dist <= distContact) {
    inCont = true;
    this->dbprt("Particles in contact");
    this->dbprt("center 1 = ", center1[0]);
    this->dbprt("           ", center1[1]);
    this->dbprt("           ", center1[2]);
    this->dbprt("center 2 = ", center2[0]);
    this->dbprt("           ", center2[1]);
    this->dbprt("           ", center2[2]);
    this->dbprt("dist = ", dist);
  }

  return inCont;
}

//
// check contact between "this" spherical particle and
// a wall boundary
//
template <class FLOATTYPE, size_t NDIM>
bool PsSphereData<FLOATTYPE, NDIM>::inContact(
         PsWallData<FLOATTYPE, NDIM>* wallBndry) {

  this->dbprt("PsSphereData::inContact for wall boundary");
  bool inCont=false;

  // Sphere info, Contact criteria correct
  // Sphere val is an assumption
  FLOATTYPE dist;
  posVecType center0 = this->getCenter();
  FLOATTYPE distContact = this->getRadius();
  FLOATTYPE sphereVal = 0.5;

  // List of wall edge inside points
  std::vector<posVecType> wallInside = wallBndry->getWallInside();

  // Check on wallEdge exists
  if (wallInside.size() == 0) {
    TxDebugExcept tde("PsSphereData::inContact # of wall inside elements = 0");
    tde << " in <PsFixedWall " << this->getName() << " >";
    throw tde;
  }

  // Loop on field elements
  for (size_t ff=0; ff<wallInside.size(); ++ff) {

    // Find distance between sphere particle center and wall inside element
    PsTinyVector<int, NDIM> wallInsidePos = wallInside[ff];
    dist = this->gridObjPtr->mapDistToGrid(center0, wallInsidePos);

    if (dist <= distContact) {
      this->dbprt("Particle in contact w/wall dist = ", dist);
      this->dbprt("center 0 = ", center0[0]);
      this->dbprt("           ", center0[1]);
      this->dbprt("           ", center0[2]);
      this->dbprt("wallPos  = ", wallInsidePos[0]);
      this->dbprt("           ", wallInsidePos[1]);
      this->dbprt("           ", wallInsidePos[2]);
      inCont = true; break;
    } // dist check

  }  // loop on wall inside elements

  return inCont;
}

// Instantiation
template class PsSphereData<float, 1>;
template class PsSphereData<float, 2>;
template class PsSphereData<float, 3>;

template class PsSphereData<double, 1>;
template class PsSphereData<double, 2>;
template class PsSphereData<double, 3>;
