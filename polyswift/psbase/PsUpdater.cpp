/**
 *
 * @file    PsUpdater.cpp
 *
 * @brief   Class for updating physical fields in simulation model
 *
 * @version $Id: PsUpdater.cpp 8257 2007-09-12 22:05:20Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsUpdater.h>

// txbase includes
#include <TxTensor.h>
#include <TxMakerMap.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsUpdater<FLOATTYPE, NDIM>::PsUpdater() {

  numUpdateFields = 0;
  //  constraintFieldPtr = NULL;
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsUpdater<FLOATTYPE, NDIM>::~PsUpdater() {

  // Clear names
  updateFieldNames.clear();

  // Only holding local pointers
  updateFields.clear();
}

//
//
//
template <class FLOATTYPE, size_t NDIM>
void PsUpdater<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Debug flag
  if (tas.hasString("printdebug")) {
    std::string dbPrint = tas.getString("printdebug");
    if (dbPrint == "on") this->dbStatus = PSDB_ON;
  }

  this->dbprt("PsUpdater::setAttrib() ");

  // Names of Self-consistent fields to point to
  if (tas.hasStrVec("updatefields")) {
    updateFieldNames = tas.getStrVec("updatefields");
    numUpdateFields =  updateFieldNames.size();
  }
  else {
    TxDebugExcept tde("PsUpdater::setAttrib: no updatefields found");
    tde << " for <Updater " << this->getName() << " >";
    throw tde;
  }

  // Apply frequency
  if (tas.hasOption("applyFrequency")) {
    applyFrequency = tas.getOption("applyFrequency");
  }
  else {
    applyFrequency = 1; // Defaults to application at every timestep
    this->dbprt("PsUpdater::setAttrib(): applyFrequency set to = 1");
  }

  // Apply start number of timesteps
  if (tas.hasOption("applyStart")) {
    applyStart = tas.getOption("applyStart");
  }
  else {
    // Defaults to application at beginning
    applyStart = PsDynObjBase::getCurrDomainStep();
    this->dbprt("PsUpdater::setAttrib(): applyStart set to = ", (int)applyStart);
  }

  // Apply end number of timesteps
  if (tas.hasOption("applyEnd")) {
    applyEnd = tas.getOption("applyEnd");
  }
  else {
    // Defaults to application until end of present run
    applyEnd = PsDynObjBase::getNsteps() +
               PsDynObjBase::getCurrDomainStep();
    this->dbprt("PsUpdater::setAttrib(): applyEnd set to = ", (int)applyEnd);
  }

  // Checks for apply times
  if (applyStart >= applyEnd) {
    TxDebugExcept tde("PsUpdater::setAttrib: applyStart >= applyEnd");
    tde << " for <Updater " << this->getName() << " >\n";
    tde << " applyEnd   = " << applyEnd << " \n";
    tde << " applyStart = " << applyEnd << " \n";
    throw tde;
  }

  // Checks for apply times
  if ( (applyEnd - applyStart) <= applyFrequency )  {
    TxDebugExcept tde("PsUpdater::setAttrib: applyFrequency too large");
    tde << " for <Updater " << this->getName() << " >\n";
    tde << " applyEnd   = " << applyEnd << " \n";
    tde << " applyStart = " << applyEnd << " \n";
    throw tde;
  }

  // Shift fields with average density values
  // SWS: this should be refactored because it
  // will be tricky for users
  if (tas.hasOption("shiftDensFlag")) {
    shiftDensFlag = tas.getOption("shiftDensFlag");
  }
  else {
    shiftDensFlag = true; // Default behavior
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsUpdater<FLOATTYPE, NDIM>::buildData() {

  this->dbprt("PsUpdater::buildData() ");

  // Store PsPhysField pointers to be updated
  for (size_t i=0; i<numUpdateFields; ++i) {

    PsPhysField<FLOATTYPE, NDIM>* physFieldPtr =
      PsNamedObject::getObject<PsPhysField<FLOATTYPE, NDIM> >(updateFieldNames[i]);
    if (!physFieldPtr) {
      TxDebugExcept tde("PsUpdater::buildData: physField not found");
      tde << " in <Updater " << this->getName() << " >";
      throw tde;
    }
    else {
      this->dbprt("...storing PhysField TO BE updated: ", physFieldPtr->getName());
      this->dbprt("   for updater ", this->getName());
    }

    // Store pointers
    updateFields.push_back(physFieldPtr);
  }

  // Local helper to check consistency of field
  // types/sizes and set relavent local member info
  checkSetUpdateFields();

  // Allocate workspace
  dFld  = TxMakerMap< PsFieldBase<FLOATTYPE> >::getNew(updateFieldType);
  dFld2 = TxMakerMap< PsFieldBase<FLOATTYPE> >::getNew(updateFieldType);
}

template <class FLOATTYPE, size_t NDIM>
void PsUpdater<FLOATTYPE, NDIM>::buildSolvers() {

  this->dbprt("PsUpdater::buildSolvers() ");

  // Setting dFld sizes/values
  PsGridBaseItr* gItr = &this->getGridBase();
  dFld->setGrid(gItr);
  dFld2->setGrid(gItr);
}

template <class FLOATTYPE, size_t NDIM>
void PsUpdater<FLOATTYPE, NDIM>::initialize() {
}

//
//
//
template <class FLOATTYPE, size_t NDIM>
void PsUpdater<FLOATTYPE, NDIM>::update(double t) {

  this->dbprt("PsUpdater::update() ");

  // Reset flag and cast types
  updateFlag = false;
  size_t ts  = (size_t)t;
  size_t tsSinceStart = ts - applyStart;

  // Do not update at all if before applyStart or after applyEnd
  if ( (ts < applyStart) || (ts > applyEnd) ) {
    updateFlag = false;
  }
  else {

    this->dbprt("tsSinceStart   = ", (int)tsSinceStart);
    this->dbprt("applyFrequency = ", (int)applyFrequency);

    if ( (tsSinceStart % applyFrequency) == 0) {
      updateFlag = true;
      this->dbprt(" applying Updater ", this->getName());
      this->dbprt(" at t =  ", (int)ts);
    }
    else { updateFlag = false; }
  }

}

//
// private helper method
//
template <class FLOATTYPE, size_t NDIM>
void PsUpdater<FLOATTYPE, NDIM>::checkSetUpdateFields() {

  if ( updateFields.size() == 0 ) {
    TxDebugExcept tde("PsUpdater::checkSetUpdateFields: no update fields set");
    tde << " in <Updater " << this->getName() << " >";
    throw tde;
  }

// Set first field type/sizes
  PsPhysField<FLOATTYPE, NDIM>* physFieldPtr = updateFields[0];
  std::string ftype0          = physFieldPtr->getFieldType();

  if ( updateFields.size() > 1 ) {

    for (size_t i=1; i<numUpdateFields; ++i) {

      PsPhysField<FLOATTYPE, NDIM>* physFieldPtr = updateFields[i];
      std::string ftype          = physFieldPtr->getFieldType();
      // std::vector<size_t> fsizes = physFieldPtr->getField Lengths();

      if ( ftype != ftype0 ) {
        TxDebugExcept tde("PsUpdater::checkSetUpdateFields: update field types !=");
        tde << " in <Updater " << this->getName() << " >";
        throw tde;
      }
    } // loop on update fields
  }

// If exception not thrown or only one fields then set type/sizes
  updateFieldType = ftype0;
}

// Instantiate classes
template class PsUpdater<float, 1>;
template class PsUpdater<float, 2>;
template class PsUpdater<float, 3>;

template class PsUpdater<double, 1>;
template class PsUpdater<double, 2>;
template class PsUpdater<double, 3>;
