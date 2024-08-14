/**
 *
 * @file    PsGrid.cpp
 *
 * @brief   Base interface for grid
 *
 * @version $Id: PsGrid.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// pseffhamil includes
#include <PsGrid.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsGrid<FLOATTYPE, NDIM>::PsGrid() {
  decompObjPtr = NULL;
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsGrid<FLOATTYPE, NDIM>::~PsGrid() {
  delete decompObjPtr;
}

template <class FLOATTYPE, size_t NDIM>
void PsGrid<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Scoping call to base class
  // PsGridBase<FLOATTYPE, NDIM>::setAttrib(tas);

  // Using base class to set attributes for base polymer class
  PsDynObjBase::setAttrib(tas);

  // Debug flag
  if (tas.hasString("printdebug")) {
    std::string dbPrint = tas.getString("printdebug");
    if (dbPrint == "on") this->dbStatus = PSDB_ON;
  }

  // Number of simulation cells
  if (tas.hasOptVec("numCellsGlobal")) {

    std::vector<int> cells;
    cells = tas.getOptVec("numCellsGlobal");

    for (size_t n=0; n<cells.size(); ++n) {
      numCellsGlobal.push_back(static_cast<size_t>(cells[n]));
    }
  }

  // Name of decomp
  if (tas.hasString("decomp")) {
    decompName = tas.getString("decomp");
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsGrid<FLOATTYPE, NDIM>::buildData() {
}

template <class FLOATTYPE, size_t NDIM>
void PsGrid<FLOATTYPE, NDIM>::buildSolvers() {

  // Store decomp pointer
  decompObjPtr =
    PsNamedObject::getObject<PsDecompBase<FLOATTYPE, NDIM> >(decompName);
  if (!decompObjPtr) {
    TxDebugExcept tde("PsGrid::buildSolvers: decomp pointer not set");
    tde << " in <PsGrid " << this->getName() << " >";
    throw tde;
  }

}

// top level global cells
template <class FLOATTYPE, size_t NDIM>
size_t PsGrid<FLOATTYPE, NDIM>::getTotalCellsGlobal() {

  size_t totalCells = 1;
  for (size_t i=0; i<numCellsGlobal.size(); ++i) {
    totalCells = totalCells * numCellsGlobal[i];
  }
  return totalCells;
}

// top level global cells
template <class FLOATTYPE, size_t NDIM>
std::vector<size_t> PsGrid<FLOATTYPE, NDIM>::getNumNodesGlobal() {

  std::vector<size_t> nodes;
  size_t num;
  for (size_t idim=0; idim < NDIM; ++idim) {
    num = numCellsGlobal[idim] - 1;
    nodes.push_back(num);
  }
  return nodes;
}

// top level global cells
template <class FLOATTYPE, size_t NDIM>
std::vector<FLOATTYPE> PsGrid<FLOATTYPE, NDIM>::getOrigin() {

  std::vector<FLOATTYPE> lengths;
  FLOATTYPE val = 0.0;
  for (size_t idim=0; idim < NDIM; ++idim) {
    lengths.push_back(val);
  }
  return lengths;
}

// Instantiate
template class PsGrid<float, 1>;
template class PsGrid<float, 2>;
template class PsGrid<float, 3>;

template class PsGrid<double, 1>;
template class PsGrid<double, 2>;
template class PsGrid<double, 3>;

