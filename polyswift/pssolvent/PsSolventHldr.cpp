/**
 *
 * @file    PsSolventHldr.cpp
 *
 * @brief   Base class owning instances of base class PsSolvent
 *
 * @version $Id: PsSolventHldr.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// pssolvent includes
#include <PsSolventHldr.h>

// txbase includes
#include <TxMakerMap.h>

template <class FLOATTYPE, size_t NDIM>
void PsSolventHldr<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Look for Solvent blocks in input file
  solventNames = tas.getNamesOfType("Solvent");

  // Set the number of Solvent blocks found
  numSolvents = solventNames.size();

  // Put each solvent attribute set into local list
  for (size_t i=0; i<numSolvents; ++i) {
    this->pprt("Found <Solvent> block ", solventNames[i]);
    TxHierAttribSetIntDbl attribs = tas.getAttrib(solventNames[i]);
    solventAttribs.push_back(attribs);
  }
}

template <class FLOATTYPE, size_t  NDIM>
void PsSolventHldr<FLOATTYPE, NDIM>::buildData(PsNamedObject* inOwner) {

  this->dbprt("PsSolventHldr::buildData() ");

  for (size_t i=0; i<solventNames.size(); ++i) {

    // For each Solvent look for kind
    std::string kind = solventAttribs[i].getString("kind");

    // Makermap returns pointer to a derived object of type PsSolvent
    PsSolvent<FLOATTYPE, NDIM>* solventPtr =
      TxMakerMap< PsSolvent<FLOATTYPE, NDIM> >::getNew(kind);
    if (!solventPtr) {
      TxDebugExcept tde("PsSolventHldr::buildData: Solvent pointer not set");
      throw tde;
    }

    // Set parameters and attributes for particular solvent and build
    solventPtr->setName(solventNames[i]);
    solventPtr->setSolventName(solventNames[i]);
    solventPtr->setDomainSingletons( this->getDomainSingletons() );
    solventPtr->setAttrib(solventAttribs[i]);

    // New name registration and build
    solventPtr->setOwner(inOwner);
    inOwner->makeAvail(solventPtr, solventNames[i]);
    solventPtr->buildData();

    // Push pointer into solvents structure
    solvents.push_back(solventPtr);
  }

}

template <class FLOATTYPE, size_t  NDIM>
void PsSolventHldr<FLOATTYPE, NDIM>::buildSolvers() {

  this->dbprt("PsSolventHldr::buildSolvers() ");

  // Loop over vector of solvent pointers and call buildSolvers()
  for (isolvent = solvents.begin(); isolvent != solvents.end(); ++isolvent) {
    (*isolvent)->buildSolvers();
  }

}

//
//
//
template <class FLOATTYPE, size_t  NDIM>
void PsSolventHldr<FLOATTYPE, NDIM>::update(double t) {

  // Update the holder
  PsDynObj<FLOATTYPE, NDIM>::update(t);

  this->dbprt("PsSolventHldr::update() ");

  // Loop over vector of solvent pointers and call update(t)
  for (isolvent = solvents.begin(); isolvent != solvents.end(); ++isolvent) {
    this->dbprt("Updating solvent ", (*isolvent)->getName());
    (*isolvent)->update(t);
  }

}

//
//
//
template <class FLOATTYPE, size_t  NDIM>
void PsSolventHldr<FLOATTYPE, NDIM>::dump() {

  // Update the holder
  PsDynObj<FLOATTYPE, NDIM>::dump();

  this->dbprt("PsSolventHldr::dump() ");

  // Loop over vector of solvent pointers and call dump
  for (isolvent = solvents.begin(); isolvent != solvents.end(); ++isolvent) {
    // this->dbprt("Dumping solvent ", (*isolvent)->getName() );
    // (*isolvent)->dump();
  }

}

// Instantiate base solvent holder classes
template class PsSolventHldr<float, 1>;
template class PsSolventHldr<float, 2>;
template class PsSolventHldr<float, 3>;

template class PsSolventHldr<double, 1>;
template class PsSolventHldr<double, 2>;
template class PsSolventHldr<double, 3>;
