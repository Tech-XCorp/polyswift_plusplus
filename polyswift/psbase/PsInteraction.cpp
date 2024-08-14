/**
 *
 * @file    PsInteraction.cpp
 *
 * @brief   Class for calculating energetics from appropriate fields
 *
 * @version $Id: PsInteraction.cpp 8257 2007-09-12 22:05:20Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsInteraction.h>

// txbase includes
#include <TxTensor.h>
#include <TxMakerMap.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsInteraction<FLOATTYPE, NDIM>::PsInteraction() {
  freeEnergy = 0.0;
  constraintFieldPtr = NULL;
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsInteraction<FLOATTYPE, NDIM>::~PsInteraction() {

  // Clear local vector, pointers not owned
  physFields.clear();

  // Clear names
  scfieldNames.clear();
}

//
//
//
template <class FLOATTYPE, size_t NDIM>
void PsInteraction<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Debug flag
  if (tas.hasString("printdebug")) {
    std::string dbPrint = tas.getString("printdebug");
    if (dbPrint == "on") this->dbStatus = PSDB_ON;
  }

  this->dbprt("PsInteraction::setAttrib() ");

  // Names of Self-consistent fields to point to
  if (tas.hasStrVec("scfields")) {
    scfieldNames = tas.getStrVec("scfields");
  }

  // SWS: Shift fields with average density values
  // SWS: this should be refactored because it
  // SWS: will be tricky for users
  if (tas.hasOption("shiftDensFlag")) {
    shiftDensFlag = tas.getOption("shiftDensFlag");
  }
  else {
    shiftDensFlag = true; // Default behavior
  }

}

//
template <class FLOATTYPE, size_t NDIM>
void PsInteraction<FLOATTYPE, NDIM>::buildData() {
}

template <class FLOATTYPE, size_t NDIM>
void PsInteraction<FLOATTYPE, NDIM>::buildSolvers() {

  this->dbprt("PsInteraction::buildSolvers() ");

  //
  // Store PsPhysField pointers
  //
  for (size_t iphys=0; iphys<scfieldNames.size(); ++iphys) {

    PsPhysField<FLOATTYPE, NDIM>* physFieldPtr =
      PsNamedObject::getObject<PsPhysField<FLOATTYPE, NDIM> >(scfieldNames[iphys]);

    physFieldPtr->registerInteraction(this->getName() );
    physFields.push_back(physFieldPtr);

    this->dbprt("storing physField: ", physFields[iphys]->getName());
    this->dbprt("  for interaction ", this->getName());
  }

  //
  // Check rank for all fields same
  //
  size_t irank = physFields[0]->getFieldRank();
  for (size_t j=1; j<physFields.size(); ++j) {
    size_t jrank = physFields[j]->getFieldRank();
    if (jrank != irank) {
      TxDebugExcept tde("PsInteraction::buildSolvers: rank of fields not equal");
      throw tde;
    }
  }

  // Get the default constraint (pressure) field & set related field pointers
  constraintFieldPtr =
    PsNamedObject::getObject<PsPhysField<FLOATTYPE, NDIM> >("defaultPressure");

  //
  // Return pointers to a PsFieldBase object for workspace
  // Checking field type for one physical field and using to set
  // This is in buildSolvers rather than buildData because its
  // using sizes from the field variables set in the held physical
  // field objects
  //
  std::string fieldType = physFields[0]->getFieldType();
  tmpFieldPtr = TxMakerMap< PsFieldBase<FLOATTYPE> >::getNew(fieldType);
  if (!tmpFieldPtr) {
    TxDebugExcept tde("PsInteraction::buildSolvers: no field type set for temp space");
    tde << " in <Interaction " << this->getName() << " >";
    throw tde;
  }

  PsGridBaseItr* gItr = &this->getGridBase();
  tmpFieldPtr->setGrid(gItr);

}

template <class FLOATTYPE, size_t NDIM>
PsPhysField<FLOATTYPE, NDIM>* PsInteraction<FLOATTYPE, NDIM>::getOtherPhysField(
    std::string physFieldName) {

  if (!hasScField(physFieldName) ) {
    TxDebugExcept tde("PsInteraction::getOtherPhysField: specified phys fld not a member");
    tde << " in <Interaction " << this->getName() << " >";
    throw tde;
  }

  if (physFields.size()>2) {
    TxDebugExcept tde("PsInteraction::getOtherPhysField: more than 2 contained phys fields");
    tde << " in <Interaction " << this->getName() << " > not supported yet";
    throw tde;
  }

  //
  // Return pointer for physField
  // Look for and return "other" physical field pointer
  //
  PsPhysField<FLOATTYPE, NDIM>* pfPtr = 0;
  for (size_t n=0; n<scfieldNames.size(); ++n) {
    if (physFieldName != scfieldNames[n]) {
      pfPtr = physFields[n];
    }
  }

  return pfPtr;
}

//
//
template <class FLOATTYPE, size_t NDIM>
bool PsInteraction<FLOATTYPE, NDIM>::hasScField(std::string fieldName) {

  bool hasField = 0;

  for (size_t n=0; n<scfieldNames.size(); ++n) {
    if (fieldName == scfieldNames[n]) {
      hasField = 1;
      return hasField;
    }
  }

  return hasField;
}

//
// Instantiate classes
//
template class PsInteraction<float, 1>;
template class PsInteraction<float, 2>;
template class PsInteraction<float, 3>;

template class PsInteraction<double, 1>;
template class PsInteraction<double, 2>;
template class PsInteraction<double, 3>;
