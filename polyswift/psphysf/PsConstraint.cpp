/**
 *
 * @file    PsConstraint.cpp
 *
 * @brief   Class that holds constraint (walls/particles) fields
 *
 * @version $Id: PsConstraint.cpp 8401 2007-09-26 20:17:05Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psphysf includes
#include <PsConstraint.h>

// txbase includes
#include <TxMakerMap.h>

template <class FLOATTYPE, size_t NDIM>
PsConstraint<FLOATTYPE, NDIM>::PsConstraint() {

  constraintDataName  = "MonomerDensity";
  presDataName        = "PressureField";

  this->densFieldName = constraintDataName;
  this->conjFieldName = presDataName;

  preconditionFactor = 1.0;
  //  maxThreshold = 1.01; // Too stringent?
  maxThreshold = 1.05;
  volumeThreshold = 0.5;

  constraintFieldPtr = NULL;
  presFieldPtr = NULL;
  presCorrectFieldPtr = NULL;
  dField = NULL;
}

template <class FLOATTYPE, size_t NDIM>
PsConstraint<FLOATTYPE, NDIM>::~PsConstraint() {

  // Clear field pointers
  delete constraintFieldPtr;
  delete presFieldPtr;
  delete presCorrectFieldPtr;
  delete dField;
}

//
// set attributes
//
template <class FLOATTYPE, size_t NDIM>
void PsConstraint<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Using base class to set attributes for base physical field class
  PsPhysFldFuncs<FLOATTYPE, NDIM>::setAttrib(tas);
  this->dbprt("PsConstraint::setAttrib(): ");

  // Names of self-consistent fields to which constraint applies
  if (tas.hasStrVec("excludefields")) {
    excludeFieldNames = tas.getStrVec("excludefields");
  }

  // Value of precondition factor for constraints
  if (tas.hasParam("preconditionFactor")) {
    preconditionFactor = tas.getParam("preconditionFactor");
    this->dbprt("reset preconditionFactor = ", preconditionFactor);
  }

}

//
// build Data
//
template <class FLOATTYPE, size_t NDIM>
void PsConstraint<FLOATTYPE, NDIM>::buildData() {

  this->dbprt("PsConstraint::buildData() ");

  // Build base class data and notify
  PsPhysFldFuncs<FLOATTYPE, NDIM>::buildData();

  // Return pointers to a PsFieldBase objects for constraint
  constraintFieldPtr  = TxMakerMap< PsFieldBase<FLOATTYPE> >::getNew(this->fieldType);
  presFieldPtr        = TxMakerMap< PsFieldBase<FLOATTYPE> >::getNew(this->fieldType);
  presCorrectFieldPtr = TxMakerMap< PsFieldBase<FLOATTYPE> >::getNew(this->fieldType);
  dField              = TxMakerMap< PsFieldBase<FLOATTYPE> >::getNew(this->fieldType);

  constraintFieldPtr->setFieldType(this->fieldType);
  presFieldPtr->setFieldType(this->fieldType);
  presCorrectFieldPtr->setFieldType(this->fieldType);
  dField->setFieldType(this->fieldType);

  // Set field sizes
  PsGridBaseItr* gItr = &this->getGridBase();
  constraintFieldPtr->setGrid(gItr);
  presFieldPtr->setGrid(gItr);
  presCorrectFieldPtr->setGrid(gItr);
  dField->setGrid(gItr);
}

template <class FLOATTYPE, size_t NDIM>
void PsConstraint<FLOATTYPE, NDIM>::buildSolvers() {

  this->dbprt("PsConstraint::buildSolvers() ");

  // Build base class data and notify
  PsPhysFldFuncs<FLOATTYPE, NDIM>::buildSolvers();

  // Store PsPhysField pointers to be excluded
  for (size_t i=0; i<excludeFieldNames.size(); ++i) {

    PsPhysField<FLOATTYPE, NDIM>* physFieldPtr =
      PsNamedObject::getObject<PsPhysField<FLOATTYPE, NDIM> > (excludeFieldNames[i]);

    excludeFields.push_back(physFieldPtr);
    this->dbprt("..storing PhysField for excluding ", excludeFields[i]->getName());
  }

  // Initial values SWS: move for initialize/restore?
  constraintFieldPtr->reset(0.0);
  presCorrectFieldPtr->reset(0.0);
}

//
// Reset constraint fields... to be recalculated at update step
//
template <class FLOATTYPE, size_t NDIM>
void PsConstraint<FLOATTYPE, NDIM>::resetDensField() {
  this->dbprt("PsConstraint::resetDensField() ");
  constraintFieldPtr->reset(0.0);
}

//
// Update pressure field. subtracting out global pressure
// average accounting for wall/particle constraints
//
template <class FLOATTYPE, size_t NDIM>
void PsConstraint<FLOATTYPE, NDIM>::update(double t) {
  resetDensField();
}

//
// Initialize constraint fields... on first start-up
//
template <class FLOATTYPE, size_t NDIM>
void PsConstraint<FLOATTYPE, NDIM>::initialize() {

  this->dbprt("PsConstraint::initialize() ");

  // Initialize pressure values (so restore can work on restart)
  presFieldPtr->reset(0.0);

  //
  // Conditioning field values inside wall to aid relaxation
  // Arbitrary value for pre-conditioning constraint regions
  // SWS: this should be moved/refactored and check the initialization
  // SWS: exclude code run on restart as well....?
  //
  PsFieldBase<FLOATTYPE>& xField = *dField;
  PsFieldBase<FLOATTYPE>& phi0Field = *constraintFieldPtr;

  for (size_t i=0; i<excludeFieldNames.size(); ++i) {

    PsFieldBase<FLOATTYPE>& excludeField =
      excludeFields[i]->getConjgField();

    xField.reset(0.0);
    xField += phi0Field;
    xField *= preconditionFactor;
    excludeField += xField;
  }

  // After constraint values set, set the static volume
  // SWS: need to refactor/move this (why? 02/13/09)
  // calcLocalVolume();
}

//
// Update pressure field. subtracting out global pressure
// average accounting for wall/particle constraints
//
template <class FLOATTYPE, size_t NDIM>
void PsConstraint<FLOATTYPE, NDIM>::updatePres() {

  // Set references to local data
  PsFieldBase<FLOATTYPE>& xField = *dField;
  PsFieldBase<FLOATTYPE>& phi0Field = *constraintFieldPtr;
  PsFieldBase<FLOATTYPE>& presField = *presFieldPtr;
  PsFieldBase<FLOATTYPE>& corrField = *presCorrectFieldPtr;

  // "Saving" the pressure field in wall
  xField.reset(0.0);
  corrField.reset(0.0);
  xField += presField;
  xField *= phi0Field;
  corrField += xField;

  // Correction for constaint to the averaging process
  // xField is now the mask function for interior
  //  xField.reset(0.0);
  //  xField += phi0Field;
  //  xField *= -1.0;
  //  xField += 1.0;
  xField.reset(1.0);
  xField -= phi0Field;

  // Mask pressure field
  presField *= xField;

  // Pressure scale factor for number of components
  // and subtract out average
  shiftPresField();

  // Mask pressure field again to mask out shifted value inside
  // walls.... *** NEW ***
  presField *= xField;
  //  std::cout << " " << std::endl;
  //  std::cout << "NOT shifting pressure" << std::endl;
  //  std::cout << " " << std::endl;

  // Post-correction
  presField += corrField;

  // Check for out-of-bounds in constraint field.
  bool maxFound = phi0Field.checkMaxClip(maxThreshold,1.0);
  if (maxFound) {
    TxDebugExcept tde("PsConstraint::update: phi0");
    tde << " > " << maxThreshold << "in <PsNanoPtcl " << this->getName() << " >";
    tde << "\n Possible problem w/ interface widths";
    tde << "\n (possibly adjust boundary thresholds";
    throw tde;
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsConstraint<FLOATTYPE, NDIM>::shiftPresField() {

  this->dbprt("PsConstraint::shiftPresField() ");

  // Get pressure field
  PsFieldBase<FLOATTYPE>& presField = *presFieldPtr;
  size_t presSize = presField.getSize(); // SWS: volume
  //  std::cout << "presSize = " << presSize << std::endl;
  //  this->dbprt("presSize = ", (FLOATTYPE)presSize);
  //  this->dbprt("presSize = ", (FLOATTYPE)presSize);

  // Volume correction
  FLOATTYPE localVol = FLOATTYPE(presSize)-calcLocalVolume();
  // this->dbprt("localVol = ", localVol);

  size_t nprocs = this->getCommBase().getSize();
  FLOATTYPE localPresAvg = presField.getSumAll()/( (FLOATTYPE) (localVol*nprocs) );
  //  this->dbprt("localPresAvg = ", localPresAvg);
  FLOATTYPE globalPresAvg = this->getCommBase().allReduceSum(localPresAvg);
  //FLOATTYPE globalPresAvg = localPresAvg;
  presField -= globalPresAvg;

  // SWS: check dbprt call
  this->dbprt("globalPresAvg = ", globalPresAvg);
}

//
// Restore fields... on restart
// Only conjugate field because densities are reset by domain
//
template <class FLOATTYPE, size_t NDIM>
void PsConstraint<FLOATTYPE, NDIM>::restore(TxIoBase* txIoPtr) {

  // Scoping call to base class
  PsPhysFldFuncs<FLOATTYPE, NDIM>::restore(txIoPtr);
}

//
// Dump the constraint values
// leaving in case special dump functions arise
//
template <class FLOATTYPE, size_t NDIM>
void PsConstraint<FLOATTYPE, NDIM>::dump(TxIoBase* txIoPtr) {

  // Scoping call to base class
  PsPhysFldFuncs<FLOATTYPE, NDIM>::dump(txIoPtr);
}

//
//
template <class FLOATTYPE, size_t NDIM>
FLOATTYPE PsConstraint<FLOATTYPE, NDIM>::calcLocalVolume() {

  FLOATTYPE tmpVol=0.0;
  tmpVol = constraintFieldPtr->getSumAll();
  return tmpVol;

  if (tmpVol < volumeThreshold) {
    TxDebugExcept tde("PsConstraint::calcLocalVolume: tmpVol < volumeThreshold");
    tde << " in <PsPhysField " << this->getName() << " >";
    throw tde;
  }
}

//
// Instantiations
//
template class PsConstraint<float, 1>;
template class PsConstraint<float, 2>;
template class PsConstraint<float, 3>;

template class PsConstraint<double, 1>;
template class PsConstraint<double, 2>;
template class PsConstraint<double, 3>;
