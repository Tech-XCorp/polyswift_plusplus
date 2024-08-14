/**
 * @file    PsPolymerHldr.cpp
 *
 * @brief   Base class owning instances of base class PsPolymer
 *
 * @version $Id: PsPolymerHldr.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// pspolymer includes
#include <PsPolymerHldr.h>

#include <TxMakerMap.h>

template <class FLOATTYPE, size_t NDIM>
void PsPolymerHldr<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Look for Polymer blocks in input file
  polymerNames = tas.getNamesOfType("Polymer");

  // Set the number of Polymer blocks found
  numPolymers = polymerNames.size();

  // Put each polymer attribute set into local list
  for (size_t i=0; i<numPolymers; ++i) {

    this->pprt("Found <Polymer> block: ", polymerNames[i]);
    TxHierAttribSetIntDbl attribs = tas.getAttrib(polymerNames[i]);
    polymerAttribs.push_back(attribs);
  }

}

template <class FLOATTYPE, size_t  NDIM>
void PsPolymerHldr<FLOATTYPE, NDIM>::buildData(PsNamedObject* inOwner) {

  for (size_t i=0; i<polymerNames.size(); ++i) {

    // For each Polymer look for kind
    std::string kind = polymerAttribs[i].getString("kind");

    // Makermap returns pointer to a derived object of type PsPolymer
    PsPolymer<FLOATTYPE, NDIM>* polymerPtr =
      TxMakerMap< PsPolymer<FLOATTYPE, NDIM> >::getNew(kind);
    if (!polymerPtr) {
      TxDebugExcept tde("PsPolymerHldr::buildData: Polymer pointer not set");
      throw tde;
    }

    // Set parameters and attributes for particular polymer and build
    polymerPtr->setName(polymerNames[i]);
    polymerPtr->setPolymerName(polymerNames[i]); // SWS: Needed?
    polymerPtr->setDomainSingletons( this->getDomainSingletons() );
    polymerPtr->setAttrib(polymerAttribs[i]);

    // New name registration and build
    polymerPtr->setOwner(inOwner);
    inOwner->makeAvail(polymerPtr, polymerNames[i]);
    polymerPtr->buildData();

    // Push pointer into polymers structure
    polymers.push_back(polymerPtr);
  }

  // Set polymer length scale from "first" polymer
  // SWS: make selection easier??
  size_t len = polymers[0]->getPolymerLength();
  PsPolymer<FLOATTYPE, NDIM>::setScaleLength(len);

  this->dbprt("Polymer lengths scaled by = ",
              (int)PsPolymer<FLOATTYPE, NDIM>::getScaleLength());

  // Set length ratio for each polymer
  FLOATTYPE lratio, length, slen;

  for (size_t i=0; i<polymerNames.size(); ++i) {
    slen = (FLOATTYPE)PsPolymer<FLOATTYPE, NDIM>::getScaleLength();
    length = (FLOATTYPE) polymers[i]->getPolymerLength();
    lratio = length/slen;
    polymers[i]->setLengthRatio(lratio);
  }

}

template <class FLOATTYPE, size_t  NDIM>
void PsPolymerHldr<FLOATTYPE, NDIM>::buildSolvers() {

  this->dbprt("PsPolymerHldr::buildSolvers()" );

  // Loop over vector of polymer pointers and call buildSolvers()
  for (ipoly = polymers.begin(); ipoly != polymers.end(); ++ipoly) {
    (*ipoly)->buildSolvers();
  }

}

template <class FLOATTYPE, size_t  NDIM>
void PsPolymerHldr<FLOATTYPE, NDIM>::update(double t) {

  // Update the holder
  PsDynObj<FLOATTYPE, NDIM>::update(t);

  this->dbprt("PsPolymerHldr::update()" );

  // Loop over vector of polymer pointers and call update(t)
  for (ipoly = polymers.begin(); ipoly != polymers.end(); ++ipoly) {
    this->dbprt("Updating polymer ", (*ipoly)->getName());
    (*ipoly)->update(t);
  }
}

template <class FLOATTYPE, size_t  NDIM>
void PsPolymerHldr<FLOATTYPE, NDIM>::dump() {

  // Update the holder
  PsDynObj<FLOATTYPE, NDIM>::dump();

  // Loop over vector of polymer pointers and call dump
  for (ipoly = polymers.begin(); ipoly != polymers.end(); ++ipoly) {
    //      std::cout << "Dumping polymer " << (*ipoly)->getName() << std::endl;
    //    }
    //    (*ipoly)->dump();
  }
}

// Instantiate base polymer holder classes
template class PsPolymerHldr<float, 1>;
template class PsPolymerHldr<float, 2>;
template class PsPolymerHldr<float, 3>;

template class PsPolymerHldr<double, 1>;
template class PsPolymerHldr<double, 2>;
template class PsPolymerHldr<double, 3>;
