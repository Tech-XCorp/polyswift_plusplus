/**
 *
 * @file    PsGridHldr.cpp
 *
 * @brief   Base class owning instances of grid object(s)
 *
 * @version $Id: PsGridHldr.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psgrid includes
#include <PsGridHldr.h>

#include <PsCommBase.h>

// txbase includes
#include <TxMakerMap.h>

template <class FLOATTYPE, size_t NDIM>
void PsGridHldr<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Look for Grid blocks in input file
  gridNames = tas.getNamesOfType("Grid");

  // Set the number of Grid blocks found
  numGrids = gridNames.size();

  // Put each grid attribute set into local list
  for (size_t i=0; i<numGrids; ++i) {
    this->pprt("Found <Grid> block:  ", gridNames[i]);
    TxHierAttribSetIntDbl attribs = tas.getAttrib(gridNames[i]);
    gridAttribs.push_back(attribs);
  }
}

template <class FLOATTYPE, size_t  NDIM>
void PsGridHldr<FLOATTYPE, NDIM>::buildData(PsNamedObject* inOwner) {

  this->dbprt("PsGridHldr::buildData()" );

  for (size_t i=0; i<gridNames.size(); ++i) {

    // For each Grid look for kind
    std::string kind = gridAttribs[i].getString("kind");

    // Makermap returns pointer to a derived object of type PsGrid
    PsGridBase<FLOATTYPE, NDIM>* gridPtr =
        TxMakerMap< PsGridBase<FLOATTYPE, NDIM> >::getNew(kind);

    // Set parameters and attributes for particular grid and build
    gridPtr->setName(gridNames[i]);
    gridPtr->setDomainSingletons( this->getDomainSingletons() );
    gridPtr->setAttrib(gridAttribs[i]);

    // New name registration and build
    gridPtr->setOwner(inOwner);
    inOwner->makeAvail(gridPtr, gridNames[i]);
    gridPtr->buildData();

    // Push pointer into grids structure
    grids.push_back(gridPtr);
  }

}

template <class FLOATTYPE, size_t  NDIM>
void PsGridHldr<FLOATTYPE, NDIM>::buildSolvers() {

  this->dbprt("PsGridHldr::buildSolvers() ");

  // Loop over vector of grid pointers and call buildSolvers()
  typename std::vector< PsGridBase<FLOATTYPE, NDIM>* >::iterator igrid;
  for (igrid = grids.begin(); igrid != grids.end(); ++igrid) {
    (*igrid)->buildSolvers();
  }

}

// Instantiate base grid holder classes
template class PsGridHldr<float, 1>;
template class PsGridHldr<float, 2>;
template class PsGridHldr<float, 3>;

template class PsGridHldr<double, 1>;
template class PsGridHldr<double, 2>;
template class PsGridHldr<double, 3>;

