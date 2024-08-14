/**
 *
 * @file    PsBndryBase.cpp
 *
 * @brief   Base interface for the boundaries
 *
 * @version $Id: PsBndryBase.cpp 8257 2007-09-12 22:05:20Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsBndryBase.h>

// txbase includes
#include <TxTensor.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsBndryBase<FLOATTYPE, NDIM>::PsBndryBase() {

  // Initialize pointers
  constraintPhysFldPtr = NULL;
  bndryPhysFldPtr      = NULL;

  // Initialize boundry index
  maxAllBndryIndex = 0;

  bndryFieldThreshold = 0.005;
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsBndryBase<FLOATTYPE, NDIM>::~PsBndryBase() {
}

template <class FLOATTYPE, size_t NDIM>
void PsBndryBase<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Using base class to set attributes for base polymer class
  PsDynObjBase::setAttrib(tas);

  // Debug flag
  if (tas.hasString("printdebug")) {
    std::string dbPrint = tas.getString("printdebug");
    if (dbPrint == "on") this->dbStatus = PSDB_ON;
  }

  // Interaction wall field name
  if (tas.hasString("boundaryfield")) {
    bndryFieldName = tas.getString("boundaryfield");
    this->dbprt("PsBndryBase: ", this->getName(), " has boundary field ");
  }

  // Threshold value for constraint field to be considered
  // part of nanoparticle field
  if (tas.hasParam("bndryFieldThreshold")) {
    bndryFieldThreshold = tas.getParam("bndryFieldThreshold");
    if (!(this->getCommBase().getRank()) ) {
      std::cout << "<" << this->getName()
          << "> setting boundary field threshold --> "
          << bndryFieldThreshold << std::endl;
    }
  }
  else {
    this->dbprt(this->getName(),
        "bndryFieldThreshold not set: using default");
  }
}

//
// buildData
//
template <class FLOATTYPE, size_t NDIM>
void PsBndryBase<FLOATTYPE, NDIM>::buildData() {

  this->dbprt("PsBndryBase::buildData() ");

  // Setup local wall field values
  PsGridBaseItr* gItr = &this->getGridBase();
  bndryDepField.setGrid(gItr);
}

template <class FLOATTYPE, size_t NDIM>
void PsBndryBase<FLOATTYPE, NDIM>::buildSolvers() {

  this->dbprt("PsBndryBase::buildSolvers() ");

  // Store constraint PhysField pointer
  constraintPhysFldPtr =
    PsNamedObject::getObject<PsPhysField<FLOATTYPE, NDIM> >("defaultPressure");
  if (!constraintPhysFldPtr) {
    TxDebugExcept tde("PsBndryBase::buildSolvers: no default constraint defined");
    tde << " in <Boundary " << this->getName() << " >";
    throw tde;
  }

  // Store interaction PhysField pointer
  bndryPhysFldPtr =
    PsNamedObject::getObject<PsPhysField<FLOATTYPE, NDIM> >(bndryFieldName);
  if (!bndryPhysFldPtr) {
    TxDebugExcept tde("PsBndryBase::buildSolvers: bndry density pointer not set");
    tde << " in <PsBndryBase " << this->getName() << " >";
    throw tde;
  }
}

//
// Uses depField to set constraint and interaction fields
//
template <class FLOATTYPE, size_t NDIM>
void PsBndryBase<FLOATTYPE, NDIM>::update(double t) {

  this->dbprt("PsBndryBase:update() entered");

  // Set constraint and interaction physical fields
  PsFieldBase<FLOATTYPE>& depFld        = *(bndryDepField.getBasePtr());
  PsFieldBase<FLOATTYPE>& densField     = constraintPhysFldPtr->getDensField();
  PsFieldBase<FLOATTYPE>& interactField = bndryPhysFldPtr->getDensField();
  densField     += depFld;
  interactField += depFld;
}

//
// static helper function... because acts on static data
// member, allPtcls
//
template <class FLOATTYPE, size_t NDIM>
bool PsBndryBase<FLOATTYPE, NDIM>::doesBndryOverlap(
      PsBndryDataBase<FLOATTYPE, NDIM>* bndryPtr) {

  // Initialize overlap test
  // If no boundaries in list then no overlaps
  bool bndryOverlap = false;
  if (allBndrys.size() == 0) return bndryOverlap;

  // Index for test boundary
  int bndryIndx = bndryPtr->getBndryIndex();

  // Check all other particles
  for (size_t j=0; j<allBndrys.size(); ++j) {

    // "Other" boundaries in list
    PsBndryDataBase<FLOATTYPE, NDIM>* jbndryPtr = allBndrys[j];
    int jBndryIndx = jbndryPtr->getBndryIndex();

    // Skip self-particle
    if (jBndryIndx == bndryIndx) continue;

    // Check contact with base method
    bool bndrysInContact = bndryPtr->inContact(jbndryPtr);
    if (bndrysInContact) {
      bndryOverlap = true;
      break;
    }
  } // loop on bndryGroup

  return bndryOverlap;
}

//
// static helper function... because acts on static data
// member, allPtcls
//
template <class FLOATTYPE, size_t NDIM>
void PsBndryBase<FLOATTYPE, NDIM>::addBndry(
         PsBndryDataBase<FLOATTYPE, NDIM>* bndryPtr) {

  // Increment maximum index and add to global particle group
  bndryPtr->setBndryIndex(maxAllBndryIndex);
  allBndrys.push_back(bndryPtr);
  maxAllBndryIndex++;
}

//
// static helper function... because acts on static data
// member, allPtcls
//
template <class FLOATTYPE, size_t NDIM>
void PsBndryBase<FLOATTYPE, NDIM>::removeBndry(
          PsBndryDataBase<FLOATTYPE, NDIM>* bndryPtr) {

  // Find iterator position of particle pointer and erase
  typename std::vector< PsBndryDataBase<FLOATTYPE, NDIM>* >::iterator bndryPos;
  bndryPos = find(allBndrys.begin(), allBndrys.end(), bndryPtr);

  // Only deletes from allBndrys list... object pointed to must
  // be deleted but this is not responsible for that memory
  allBndrys.erase(bndryPos);
}

template <class FLOATTYPE, size_t NDIM>
size_t PsBndryBase<FLOATTYPE, NDIM>::getNumAllBndrys() {
   return allBndrys.size();
}

// Declaration of static vector of particle data pointers
template <class FLOATTYPE, size_t NDIM>
std::vector< PsBndryDataBase<FLOATTYPE, NDIM>* > PsBndryBase<FLOATTYPE, NDIM>::allBndrys;

// Declaration of static maximunm index on particle list
template <class FLOATTYPE, size_t NDIM>
int PsBndryBase<FLOATTYPE, NDIM>::maxAllBndryIndex;

//
// Instantiate the templates
//
template class PsBndryBase<float, 1>;
template class PsBndryBase<float, 2>;
template class PsBndryBase<float, 3>;

template class PsBndryBase<double, 1>;
template class PsBndryBase<double, 2>;
template class PsBndryBase<double, 3>;

