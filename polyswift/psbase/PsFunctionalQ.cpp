/**
 *
 * @file    PsFunctionalQ.cpp
 *
 * @brief   Holds the field pointers needed to calculate field-theory
 *
 * @version $Id: PsFunctionalQ.cpp 8257 2007-09-12 22:05:20Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsFunctionalQ.h>

// txbase includes
#include <TxTensor.h>
#include <TxMakerMap.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsFunctionalQ<FLOATTYPE, NDIM>::PsFunctionalQ() {

  monoDensPhysFldPtr = NULL;
  blockDensPhysFldPtr = NULL;
  chargeDensPhysFldPtr = NULL;
  constraintFieldPtr = NULL;

  hasChargeField = 0;
  hasBlockField = 0;
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsFunctionalQ<FLOATTYPE, NDIM>::~PsFunctionalQ() {

}

template <class FLOATTYPE, size_t NDIM>
void PsFunctionalQ<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Using base class to set attributes for base polymer class
  PsDynObjBase::setAttrib(tas);

  this->dbprt("PsFunctionalQ::setAttrib(): entered.");

  // Self-consistent field
  if (tas.hasString("scfield")) {
    scfieldName = tas.getString("scfield");
  }

  // Field for block dens field
  if (tas.hasString("blockfield")) {
    blockfieldName = tas.getString("blockfield");
    hasBlockField = 1;
  }

  // Charge field
  if (tas.hasString("chargefield")) {
    chargefieldName = tas.getString("chargefield");
    hasChargeField = 1;
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsFunctionalQ<FLOATTYPE, NDIM>::buildData() {

}

template <class FLOATTYPE, size_t NDIM>
void PsFunctionalQ<FLOATTYPE, NDIM>::buildSolvers() {

  this->dbprt("PsFunctionalQ::buildSolvers() entered");

  // Get the default constraint (pressure) field
  constraintFieldPtr =
    PsNamedObject::getObject<PsPhysField<FLOATTYPE, NDIM> >("defaultPressure");

  // Store monomer PhysField pointer
  monoDensPhysFldPtr =
    PsNamedObject::getObject<PsPhysField<FLOATTYPE, NDIM> >(scfieldName);

  //
  // Return pointers to a PsFieldBase object for workspace
  // Checking field type for one physical field
  // Must build here because its set on held physical fields
  //
  std::string fieldType = monoDensPhysFldPtr->getFieldType();
  tmpFieldPtr = TxMakerMap< PsFieldBase<FLOATTYPE> >::getNew(fieldType);
  PsGridBaseItr* gItr = &this->getGridBase();
  tmpFieldPtr->setGrid(gItr);

  // Store charge PhysField pointer
  if (hasChargeField) {

    chargeDensPhysFldPtr =
      PsNamedObject::getObject<PsPhysField<FLOATTYPE, NDIM> >(chargefieldName);
    if (!chargeDensPhysFldPtr) {
      TxDebugExcept tde("PsFunctionalQ::buildSolvers: charge dens ptr not set");
      tde << " in <PsFunctionalQ " << this->getName() << " >";
      throw tde;
    }

    this->dbprt("  storing physField in PsFunctionalQ: ", chargeDensPhysFldPtr->getName());
  } // has charge field

  // Store charge PhysField pointer
  if (hasBlockField) {

    //    blockDensPhysFldPtr = PsPhysField<FLOATTYPE, NDIM>::getPhysField(blockfieldName);
    blockDensPhysFldPtr =
      PsNamedObject::getObject<PsPhysField<FLOATTYPE, NDIM> >(blockfieldName);
    if (!blockDensPhysFldPtr) {
      TxDebugExcept tde("PsFunctionalQ::buildSolvers: separate monomer density pointer not set");
      tde << " in <PsFunctionalQ " << this->getName() << " >";
      throw tde;
    }

    this->dbprt("  storing physField in PsFunctionalQ: ", blockDensPhysFldPtr->getName());
  } // has block density field

}

template <class FLOATTYPE, size_t NDIM>
void PsFunctionalQ<FLOATTYPE, NDIM>::update(double t) { }

// Instantiation
template class PsFunctionalQ<float, 1>;
template class PsFunctionalQ<float, 2>;
template class PsFunctionalQ<float, 3>;

template class PsFunctionalQ<double, 1>;
template class PsFunctionalQ<double, 2>;
template class PsFunctionalQ<double, 3>;
