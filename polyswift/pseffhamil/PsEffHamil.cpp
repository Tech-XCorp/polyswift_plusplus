/**
 *
 * @file    PsEffHamil.cpp
 *
 * @brief   Specifies energetics of field-theoretic model
 *
 * @version $Id: PsEffHamil.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// pseffhamil includes
#include <PsEffHamil.h>

// txbase includes
#include <TxMakerMap.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsEffHamil<FLOATTYPE, NDIM>::PsEffHamil() {

  numInteractions = 0;
  numUpdaters     = 0;
}

//
// Destructor
//
template <class FLOATTYPE, size_t NDIM>
PsEffHamil<FLOATTYPE, NDIM>::~PsEffHamil() {

  // Clear
  interactionNames.clear();
  interactionAttribs.clear();

  // "Full" delete as PsEffHamil is the container for interactions
  for (size_t i=0; i<interactions.size(); ++i) {
    delete interactions[i];
  }
  interactions.clear();

  // Clear
  updaterNames.clear();
  updaterAttribs.clear();
  updaterSequence.clear();

  // "Full" delete as PsEffHamil is the container for updaters
  for (size_t i=0; i<updaters.size(); ++i) {
    delete updaters[i];
  }
  updaters.clear();

}

//
//
//
template <class FLOATTYPE, size_t NDIM>
void PsEffHamil<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Scoping call to base class
  PsEffHamilBase<FLOATTYPE, NDIM>::setAttrib(tas);

  this->dbprt("PsEffHamil::setAttrib() ");

  //
  // Find Interaction blocks
  //
  interactionNames = tas.getNamesOfType("Interaction");
  numInteractions = interactionNames.size();
  for (size_t n=0; n<numInteractions; ++n) {
    this->pprt("       PsEffHamil found <Interaction ", interactionNames[n]);
    TxHierAttribSetIntDbl attribs = tas.getAttrib(interactionNames[n]);
    interactionAttribs.push_back(attribs);
  }

  //
  // Find Updater blocks
  //
  updaterNames = tas.getNamesOfType("Updater");
  numUpdaters = updaterNames.size();
  for (size_t n=0; n<numUpdaters; ++n) {
    this->pprt("       PsEffHamil found <Updater ", updaterNames[n]);
    TxHierAttribSetIntDbl attribs = tas.getAttrib(updaterNames[n]);
    updaterAttribs.push_back(attribs);
  }

  // Sequence of updaters
  if (tas.hasStrVec("updaterSequence")) {
    updaterSequence = tas.getStrVec("updaterSequence");
  }
  else {
    TxDebugExcept tde("PsEffHamil::setAttrib: no updater sequence defined");
    tde << " for <EffHamil " << this->getName() << " >";
    throw tde;
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsEffHamil<FLOATTYPE, NDIM>::buildData() {

  // Scoping call to base class
  PsEffHamilBase<FLOATTYPE, NDIM>::buildData();

  this->dbprt("PsEffHamil::buildData() ");

  // Create Interaction objects from attrib lists
  for (size_t i=0; i<numInteractions; ++i) {

    // For each interaction look for kind
    std::string kind = interactionAttribs[i].getString("kind");

    // Have makermap return a pointer to a PsInteraction object
    PsInteraction<FLOATTYPE, NDIM>* interactionPtr =
        TxMakerMap< PsInteraction<FLOATTYPE, NDIM> >::getNew(kind);

    // Set parameters and attributes for particular interaction
    // and make availible for lookup by name
    interactionPtr->setName(interactionNames[i]);
    interactionPtr->setDomainSingletons( this->getDomainSingletons() );
    interactionPtr->setAttrib(interactionAttribs[i]);

    // New name/regis and build
    interactionPtr->setOwner(this->getOwner());
    this->getNonConstOwner()->makeAvail(interactionPtr, interactionNames[i]);
    interactionPtr->buildData();

    // Push pointer into interactions structure
    interactions.push_back(interactionPtr);
  }

  // Create Updater objects from attrib lists
  for (size_t i=0; i<numUpdaters; ++i) {

    // For each interaction look for kind
    std::string kind = updaterAttribs[i].getString("kind");

    // Have makermap return a pointer to a PsUpdater object
    PsUpdater<FLOATTYPE, NDIM>* updaterPtr =
        TxMakerMap< PsUpdater<FLOATTYPE, NDIM> >::getNew(kind);

    // Set parameters and attributes for particular updater
    // and make availible for lookup by name
    updaterPtr->setName(updaterNames[i]);
    updaterPtr->setDomainSingletons( this->getDomainSingletons() );
    updaterPtr->setAttrib(updaterAttribs[i]);

    // New name/regis and build
    updaterPtr->setOwner(this->getOwner());
    this->getNonConstOwner()->makeAvail(updaterPtr, updaterNames[i]);
    updaterPtr->buildData();

    // Push pointer into updater structure
    updaters.push_back(updaterPtr);
  }

}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsEffHamil<FLOATTYPE, NDIM>::buildSolvers() {

  // Scoping call to base class
  PsEffHamilBase<FLOATTYPE, NDIM>::buildSolvers();

  this->dbprt("PsEffHamil::buildSolvers() ");

  // buildSolvers for Interactions/Updaters
  for (size_t i=0; i<numInteractions; ++i)  interactions[i]->buildSolvers();
  for (size_t i=0; i<numUpdaters; ++i)      updaters[i]->buildSolvers();
}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsEffHamil<FLOATTYPE, NDIM>::update(double t) {

  this->dbprt("PsEffHamil::update() ");

  // Initialize all individual interaction objects
  for (size_t i=0; i<numInteractions; ++i) interactions[i]->initialize();

  // Update all individual interaction/updater objects
  for (size_t i=0; i<numInteractions; ++i) interactions[i]->update(t);
  for (size_t i=0; i<numUpdaters; ++i)     updaters[i]->initialize();

  // Update the updaters ;-)
  for (size_t i=0; i<updaterSequence.size(); ++i) {

    PsUpdater<FLOATTYPE, NDIM>* updaterPtr =
      PsNamedObject::getObject<PsUpdater<FLOATTYPE, NDIM> >(updaterSequence[i]);
    if (!updaterPtr) {
      TxDebugExcept tde("PsEffHamil::element in updaterSequence not defined");
      tde << " for <EffHamil " << this->getName() << " >";
      throw tde;
    }

    this->dbprt("Performing updater ", updaterSequence[i]);
    updaterPtr->update(t);
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsEffHamil<FLOATTYPE, NDIM>::dump() {

  this->dbprt("PsEffHamil::dump() ");

  // Dump all interaction info
  for (size_t i=0; i<numInteractions; ++i) interactions[i]->dump();
}

// Instantiate
template class PsEffHamil<float, 1>;
template class PsEffHamil<float, 2>;
template class PsEffHamil<float, 3>;

template class PsEffHamil<double, 1>;
template class PsEffHamil<double, 2>;
template class PsEffHamil<double, 3>;
