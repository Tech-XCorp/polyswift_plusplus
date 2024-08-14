/**
 * @file    PsBndryHldr.cpp
 *
 * @brief   Base class owning instances of grid object(s)
 *
 * @version $Id: PsBndryHldr.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psbndry includes
#include <PsBndryHldr.h>

// txbase includes
#include <TxMakerMap.h>

template <class FLOATTYPE, size_t NDIM>
void PsBndryHldr<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Look for Bndry blocks in input file
  boundaryNames = tas.getNamesOfType("Boundary");

  // Set the number of Bndry blocks found
  numBoundaries = boundaryNames.size();

  // Put each bndry attribute set into local list
  for (size_t i=0; i<numBoundaries; ++i) {

    this->pprt("Found <Boundary> block: ", boundaryNames[i]);
    TxHierAttribSetIntDbl attribs = tas.getAttrib(boundaryNames[i]);
    boundaryAttribs.push_back(attribs);
  }
}

template <class FLOATTYPE, size_t  NDIM>
void PsBndryHldr<FLOATTYPE, NDIM>::buildData(PsNamedObject* inOwner) {

  this->dbprt("PsBndryHldr::buildData() ");

  for (size_t i=0; i<boundaryNames.size(); ++i) {

    // For each Bndry look for kind
    std::string kind = boundaryAttribs[i].getString("kind");

    // Makermap returns pointer to a derived object of type PsBndry
    PsBndryBase<FLOATTYPE, NDIM>* boundaryPtr =
        TxMakerMap< PsBndryBase<FLOATTYPE, NDIM> >::getNew(kind);
    if (!boundaryPtr) {
      TxDebugExcept tde("PsBndryHldr::buildData: Boundary pointer not set");
      throw tde;
    }

    // Set parameters and attributes for particular boundary and build
    boundaryPtr->setName(boundaryNames[i]);
    boundaryPtr->setDomainSingletons( this->getDomainSingletons() );
    boundaryPtr->setAttrib(boundaryAttribs[i]);

    // New name registration and build
    boundaryPtr->setOwner(inOwner);
    inOwner->makeAvail(boundaryPtr, boundaryNames[i]);
    boundaryPtr->buildData();

    // Push pointer into boundaries structure
    boundaries.push_back(boundaryPtr);
  }

}

template <class FLOATTYPE, size_t  NDIM>
void PsBndryHldr<FLOATTYPE, NDIM>::buildSolvers() {

  this->dbprt("PsBndryHldr::buildSolvers() ");

  // Loop over vector of bndry pointers and call buildSolvers()
  for (ibndry = boundaries.begin(); ibndry != boundaries.end(); ++ibndry) {
    (*ibndry)->buildSolvers();
  }
}

template <class FLOATTYPE, size_t  NDIM>
void PsBndryHldr<FLOATTYPE, NDIM>::initialize() {

  this->dbprt("PsBndryHldr::initialize() ");

  // Loop over vector of bndry pointers and call initialize
  for (ibndry = boundaries.begin(); ibndry != boundaries.end(); ++ibndry) {
    (*ibndry)->initialize();
  }
}

template <class FLOATTYPE, size_t  NDIM>
void PsBndryHldr<FLOATTYPE, NDIM>::restore(TxIoBase* txIoPtr) {

  this->dbprt("PsBndryHldr::restore() ");

  // Loop over vector of bndry pointers and call initialize
  for (ibndry = boundaries.begin(); ibndry != boundaries.end(); ++ibndry) {
    (*ibndry)->restore(txIoPtr);
  }
}

template <class FLOATTYPE, size_t  NDIM>
void PsBndryHldr<FLOATTYPE, NDIM>::update(double t) {

  // Scoping call to base class
  PsDynObj<FLOATTYPE, NDIM>::update(t);

  this->dbprt("PsBndryHldr::update() ");

  // Loop over vector of polymer boundary and reset density fields
  //   of all held physical fields.
  for (ibndry = boundaries.begin(); ibndry != boundaries.end(); ++ibndry) {
    (*ibndry)->resetDensFields();
  }

  // Loop over vector of polymer boundary and call update(t)
  for (ibndry = boundaries.begin(); ibndry != boundaries.end(); ++ibndry) {
    (*ibndry)->update(t);
  }
}

template <class FLOATTYPE, size_t  NDIM>
void PsBndryHldr<FLOATTYPE, NDIM>::dump(TxIoBase* txIoPtr) {

  // Scoping call to base class
  PsDynObj<FLOATTYPE, NDIM>::dump();

  this->dbprt("PsBndryHldr::dump() ");

  // Loop over vector of polymer boundary and call update(t)
  for (ibndry = boundaries.begin(); ibndry != boundaries.end(); ++ibndry) {
    (*ibndry)->dump(txIoPtr);
  }

}

// Instantiate base boundary holder classes
template class PsBndryHldr<float, 1>;
template class PsBndryHldr<float, 2>;
template class PsBndryHldr<float, 3>;

template class PsBndryHldr<double, 1>;
template class PsBndryHldr<double, 2>;
template class PsBndryHldr<double, 3>;
