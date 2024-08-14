/**
 *
 * @file    PsUniCartGrid.cpp
 *
 * @brief   Class containing info for uniform cartesian grid
 *
 * @version $Id: PsUniCartGrid.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// standard
//#include <math.h>

// psgrid includes
#include <PsUniCartGrid.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsUniCartGrid<FLOATTYPE, NDIM>::PsUniCartGrid() {
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsUniCartGrid<FLOATTYPE, NDIM>::~PsUniCartGrid() {
}

template <class FLOATTYPE, size_t NDIM>
void PsUniCartGrid<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Scoping call to base class
  PsGrid<FLOATTYPE, NDIM>::setAttrib(tas);

  // Physical size of uniform cells
  if (tas.hasPrmVec("cellSizes")) {
    std::vector<double> cellS;
    cellS = tas.getPrmVec("cellSizes");
    for (size_t i=0; i<cellS.size(); ++i) {
      cellSizes.push_back((FLOATTYPE)cellS[i]);
    }
  }
}

template <class FLOATTYPE, size_t NDIM>
void PsUniCartGrid<FLOATTYPE, NDIM>::buildData() {

  // Scoping call to base class
  PsGrid<FLOATTYPE, NDIM>::buildData();
}

template <class FLOATTYPE, size_t NDIM>
void PsUniCartGrid<FLOATTYPE, NDIM>::buildSolvers() {

  // Scoping call to base class
  PsGrid<FLOATTYPE, NDIM>::buildSolvers();

  for (size_t n=0; n<NDIM; ++n) {
    this->dbprt("PsUniCartGrid::buildSolvers ");
    this->dbprt("numCellsGlobal[n] = ", (int)this->numCellsGlobal[n]);
  }

}

//
// SWS: For now... in 2D (nz = 1) returns z_center = 0
// and does not yet check for odd even
//
template <class FLOATTYPE, size_t NDIM>
std::vector<int> PsUniCartGrid<FLOATTYPE, NDIM>::getCenterGlobal() {

  this->dbprt("PsUniCartGrid::getCenterGlobal() ");

  int nx2 = this->numCellsGlobal[0]/2;
  int ny2 = this->numCellsGlobal[1]/2;

  std::vector<int> cn;
  cn.push_back(nx2);
  cn.push_back(ny2);

  int nz2;

  // SWS: This will be generalized
  if (NDIM == 3) {
    int nz = this->numCellsGlobal[2];
    if (nz == 1) nz2 = 0;
    else         nz2 = nz/2;
    cn.push_back(nz2);
  }

  return cn;
}

//
// Note: if there are 100 cells in X-dir and the indices
// run from 0-->99, this will not return 99. Must have global
// random number generator set
//
// For now... in 2D (for nz = 1) this returns 0
//
template <class FLOATTYPE, size_t NDIM>
std::vector<int> PsUniCartGrid<FLOATTYPE, NDIM>::getRandomGlobalPt() {

  this->dbprt("PsUniCartGrid::getRandomGlobalPt() ");

  FLOATTYPE randNum = 0.0;
  FLOATTYPE fnum = 0.0;
  int inum = 0;
  std::vector<int> ranVec;

  for (size_t idim=0; idim<NDIM; ++idim) {
    randNum = psRandomGlobal<FLOATTYPE>();
    fnum = randNum*(FLOATTYPE)(this->numCellsGlobal[idim]);
    inum = (int)floor(fnum);
    ranVec.push_back(inum);
  }

  return ranVec;
}

//
// SWS: for now in 2D... z should be 0 and this returns
// z = 0
//
template <class FLOATTYPE, size_t NDIM>
void PsUniCartGrid<FLOATTYPE, NDIM>::mapPointToGrid(
            PsTinyVector<int, NDIM>& posvec) {

  this->dbprt("PsUniCartGrid::mapPointToGrid entered \n",
      "... assumes min values start at zero ");

  for (size_t idim=0; idim<NDIM; ++idim) {

    // Grid info
    int igridMax = (int)this->numCellsGlobal[idim];
    int iposMax = igridMax - 1;

    // Fold back using PBC
    int ipos = posvec[idim];
    if (ipos > iposMax) ipos = ipos - igridMax;
    if (ipos < 0)       ipos = ipos + igridMax;

    posvec[idim] = ipos;
  }

}

//
// SWS: for now in 2D... z should be 0 and this returns
// z = 0
//
template <class FLOATTYPE, size_t NDIM>
FLOATTYPE PsUniCartGrid<FLOATTYPE, NDIM>::mapDistToGrid(
    PsTinyVector<int, NDIM>& v1,
    PsTinyVector<int, NDIM>& v2) {

  // Initialize vector of coordinate differences
  PsTinyVector<int, NDIM> dr(v1-v2);

  // Apply absolute value to each component
  // SWS: should move an operator to PsTinyVector
  for (size_t idim=0; idim<NDIM; ++idim) {
    dr[idim] = std::abs(dr[idim]);
  }

  // Half-distance
  std::vector<int> cn = getCenterGlobal();

  for (size_t idim=0; idim<NDIM; ++idim) {
    if ( dr[idim] > cn[idim] )
      dr[idim] = std::abs(dr[idim] - (int)this->numCellsGlobal[idim]);
  }

  int distSqr = dr.lengthSqr();
  return (FLOATTYPE)std::sqrt((FLOATTYPE)distSqr);
}

//
template <class FLOATTYPE, size_t NDIM>
std::vector<FLOATTYPE> PsUniCartGrid<FLOATTYPE, NDIM>::getGlobalLengths() {

  std::vector<FLOATTYPE> lengths;
  FLOATTYPE val = 0.0;

  for (size_t idim=0; idim < NDIM; ++idim) {
    val = (FLOATTYPE)this->numCellsGlobal[idim]*cellSizes[idim];
    lengths.push_back(val);
  }

  return lengths;
}

//
// Take a position in global grid and return corresponding local position
//
template <class FLOATTYPE, size_t NDIM>
PsTinyVector<int, NDIM>& PsUniCartGrid<FLOATTYPE, NDIM>::mapToLocalVec(
    PsTinyVector<int, NDIM>& posvec) {

  std::vector<size_t> l2g = this->decompObjPtr->getLocalToGlobalShifts();

  size_t xloc = (size_t)posvec[0] - l2g[0];
  size_t yloc = (size_t)posvec[1] - l2g[1];
  size_t zloc = (size_t)posvec[2] - l2g[2];

  localVec[0] = (int)xloc;
  localVec[1] = (int)yloc;
  localVec[2] = (int)zloc;

  return localVec;
}

//
// Take a position in local grid and return corresponding global position
// SWS: Need to check for out-of-bounds
//
template <class FLOATTYPE, size_t NDIM>
PsTinyVector<int, NDIM>& PsUniCartGrid<FLOATTYPE, NDIM>::mapToGlobalVec(
    PsTinyVector<int, NDIM>& posvec) {

  std::vector<size_t> l2g = this->decompObjPtr->getLocalToGlobalShifts();

  size_t xloc = (size_t)posvec[0] + l2g[0];
  size_t yloc = (size_t)posvec[1] + l2g[1];
  size_t zloc = (size_t)posvec[2] + l2g[2];

  localVec[0] = (int)xloc;
  localVec[1] = (int)yloc;
  localVec[2] = (int)zloc;

  return localVec;
}

// Instantiate
template class PsUniCartGrid<float, 1>;
template class PsUniCartGrid<float, 2>;
template class PsUniCartGrid<float, 3>;

template class PsUniCartGrid<double, 1>;
template class PsUniCartGrid<double, 2>;
template class PsUniCartGrid<double, 3>;
