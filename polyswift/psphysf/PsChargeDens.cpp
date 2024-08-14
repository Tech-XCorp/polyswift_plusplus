/**
 *
 * @file    PsChargeDens.cpp
 *
 * @brief   Class that holds electrostatic charge density fields
 *
 * @version $Id: PsChargeDens.cpp 8401 2007-09-26 20:17:05Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psphysf includes
#include <PsChargeDens.h>

// txbase includes
#include <TxMakerMap.h>

template <class FLOATTYPE, size_t NDIM>
PsChargeDens<FLOATTYPE, NDIM>::PsChargeDens() {

  chargeDensDataName  = "ChargeDensity";
  ePotentialDataName  = "ElectricPotential";
  this->densFieldName = chargeDensDataName;
  this->conjFieldName = ePotentialDataName;

  chargeDensFieldPtr = NULL;
  ePotentialFieldPtr = NULL;
}

template <class FLOATTYPE, size_t NDIM>
PsChargeDens<FLOATTYPE, NDIM>::~PsChargeDens() {

  // Clear field pointers
  delete chargeDensFieldPtr;
  //  delete chargeDensFieldPtr0;
  //  delete delChargeDensFieldPtr;
  delete ePotentialFieldPtr;
}

//
// set attributes
//
template <class FLOATTYPE, size_t NDIM>
void PsChargeDens<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Using base class to set attributes for base physical field class
  PsPhysFldFuncs<FLOATTYPE, NDIM>::setAttrib(tas);
}

template <class FLOATTYPE, size_t NDIM>
void PsChargeDens<FLOATTYPE, NDIM>::buildData() {

  // Build base class data and notify
  PsPhysFldFuncs<FLOATTYPE, NDIM>::buildData();

  this->dbprt("PsChargeDens::buildData() ");

  // Return pointers to a PsFieldBase object
  chargeDensFieldPtr = TxMakerMap< PsFieldBase<FLOATTYPE> >::getNew(this->fieldType);
  ePotentialFieldPtr = TxMakerMap< PsFieldBase<FLOATTYPE> >::getNew(this->fieldType);

  chargeDensFieldPtr->setFieldType(this->fieldType);
  ePotentialFieldPtr->setFieldType(this->fieldType);

  // Set field sizes
  PsGridBaseItr* gItr = &this->getGridBase();
  chargeDensFieldPtr->setGrid(gItr);
  ePotentialFieldPtr->setGrid(gItr);
}

template <class FLOATTYPE, size_t NDIM>
void PsChargeDens<FLOATTYPE, NDIM>::buildSolvers() {

  // Build base class data and notify
  PsPhysFldFuncs<FLOATTYPE, NDIM>::buildSolvers();
}

//
// Initialize the field values
//
template <class FLOATTYPE, size_t NDIM>
void PsChargeDens<FLOATTYPE, NDIM>::initialize() {

  this->dbprt("PsChargeDens::initialize() ");

  // Moved for init/restore
  //  FLOATTYPE scale =  0.00001;
  //  FLOATTYPE shift = -0.000005;
  // chargeDensFieldPtr->resetRandom(scale, shift);

  chargeDensFieldPtr->reset(0.0);
  ePotentialFieldPtr->reset(0.0);

  // chargeDensFieldPtr0->reset(0.0);
  // delChargeDensFieldPtr->reset(0.0);
  //  ePotentialFieldPtr->resetRandom(scale, shift);
  //  testCharges();
}

//
// Initialize the field values
//
template <class FLOATTYPE, size_t NDIM>
void PsChargeDens<FLOATTYPE, NDIM>::testCharges() {

  // Global center location, used to shift cavity
  // function to 0 position so grad calc correct
  PsTinyVector<int, NDIM> center(this->getGridBase().getCenterGlobal());
  PsTinyVector<int, NDIM> r01(this->getGridBase().getRandomGlobalPt());

  // Global dimensions
  std::vector<size_t> globalDims = this->getGridBase().getNumCellsGlobal();

  PsGridField<FLOATTYPE, NDIM> initLocalField;
  PsGridBaseItr* gItr = &this->getGridBase();
  initLocalField.setGrid(gItr);

  FLOATTYPE val;

// Load cavity values
  for (size_t x=0; x<globalDims[0]; ++x) {
    for (size_t y=0; y<globalDims[1]; ++y) {
      for (size_t z=0; z<globalDims[2]; ++z) {
        PsTinyVector<int, NDIM> posVec(x,y,z);
        val = ptclFunc(posVec, center);
        initLocalField.mapToLocalField(posVec, val, "add");
        val = ptclFunc(posVec, r01);
        initLocalField.mapToLocalField(posVec, val, "add");
      }
    }
  }
// ***************************************************************

// Assign charge field values
  PsFieldBase<FLOATTYPE>& chargeField = *chargeDensFieldPtr;
  chargeDensFieldPtr->reset(0.0);

  PsFieldBase<FLOATTYPE>& initField = *(initLocalField.getBasePtr());
  chargeField += initField;
}

//
// Generates a spherical particle cavity with a tanh()
// profile at the interface. Interface width controlled by
// interfaceWidth parameter.
//
template <class FLOATTYPE, size_t NDIM>
FLOATTYPE PsChargeDens<FLOATTYPE, NDIM>::ptclFunc(
      PsTinyVector<int, NDIM>& rvec,
      PsTinyVector<int, NDIM>& r0) {

  FLOATTYPE radius = 4.0;
  FLOATTYPE interfaceWidth = 1.0;

  // Sphere function
  FLOATTYPE rdis = this->getGridBase().mapDistToGrid(rvec, r0);
  FLOATTYPE tmp = (rdis-radius)/interfaceWidth;
  FLOATTYPE tanhVal = (FLOATTYPE) std::tanh(tmp);
  tmp = (tanhVal+1.0)/2.0;
  return (1.0 - tmp);
}

//
// Dump the constraint values
// leaving in case special dump functions arise
//
template <class FLOATTYPE, size_t NDIM>
void PsChargeDens<FLOATTYPE, NDIM>::dump(TxIoBase* txIoPtr) {

  // Scoping call to base class
  PsPhysFldFuncs<FLOATTYPE, NDIM>::dump(txIoPtr);
}

//
// Restore the field values on restart
//
template <class FLOATTYPE, size_t NDIM>
void PsChargeDens<FLOATTYPE, NDIM>::restore(TxIoBase* txIoPtr) {

  this->dbprt("PsChargeDens::restore() ");

  // Base class initialization
  PsPhysFldFuncs<FLOATTYPE, NDIM>::restore(txIoPtr);
}

//
template <class FLOATTYPE, size_t NDIM>
void PsChargeDens<FLOATTYPE, NDIM>::update(double t) {

  this->dbprt("PsChargeDens::update(t)");

  resetDensField();
}

//
// Reset the field values and save
// charge density from last update
//
template <class FLOATTYPE, size_t NDIM>
void PsChargeDens<FLOATTYPE, NDIM>::resetDensField() {

  this->dbprt("PsChargeDens::resetDensField() called");
  //  delChargeDensFieldPtr->reset(0.0); // SWS: This may be dangerous
  //  chargeDensFieldPtr0->reset(0.0);
  //  PsFieldBase<FLOATTYPE>& chargeDens0 = *chargeDensFieldPtr0;
  //  PsFieldBase<FLOATTYPE>& chargeDens  = *chargeDensFieldPtr;
  //  chargeDens0 += chargeDens;

  chargeDensFieldPtr->reset(0.0);
}

//
// Instantiations
//
template class PsChargeDens<float, 1>;
template class PsChargeDens<float, 2>;
template class PsChargeDens<float, 3>;

template class PsChargeDens<double, 1>;
template class PsChargeDens<double, 2>;
template class PsChargeDens<double, 3>;
