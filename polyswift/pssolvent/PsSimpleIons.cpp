/**
 *
 * @file    PsSimpleIons.cpp
 *
 * @brief   Class for simple solvents whose interactions depend
 *          only on monomer separation
 *
 * @version $Id: PsSimpleIons.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2010-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// pspolymer includes
#include <PsSimpleIons.h>

// txbase includes
#include <TxTensor.h>
#include <TxMakerMap.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsSimpleIons<FLOATTYPE, NDIM>::PsSimpleIons() {
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsSimpleIons<FLOATTYPE, NDIM>::~PsSimpleIons() {
}

//
// set attributes
//
template <class FLOATTYPE, size_t NDIM>
void PsSimpleIons<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Scoping call to base class
  PsSolvent<FLOATTYPE, NDIM>::setAttrib(tas);

  this->dbprt("PsSimpleIons::setAttrib() ");

  // Charge strength Z_c
  if (tas.hasParam("zc")) {
    zc = tas.getParam("zc");
  }
  else {
    TxDebugExcept tde("PsSimpleIons::setAttrib");
    tde << " in <PsSimpleIons " << this->getName() << " >";
    tde << " charge strength zc is not set";
    std::cout << tde.getMessage() << std::endl;
    throw tde;
  }

  // Charge strength Z_c
  if (tas.hasParam("neutralFactor")) {
    neutralFactor = tas.getParam("neutralFactor");
  }
  else {
    TxDebugExcept tde("PsSimpleIons::setAttrib");
    tde << " in <PsSimpleIons " << this->getName() << " >";
    tde << " charge neutral factor not set";
    std::cout << tde.getMessage() << std::endl;
    throw tde;
  }

}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsSimpleIons<FLOATTYPE, NDIM>::buildData() {

  // Scoping call to base class
  PsSolvent<FLOATTYPE, NDIM>::buildData();
}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsSimpleIons<FLOATTYPE, NDIM>::buildSolvers() {

  // Scoping call to base class
  PsSolvent<FLOATTYPE, NDIM>::buildSolvers();

  this->dbprt("PsSimpleIons::buildSolvers() ");

  // Return pointers to a PsFieldBase object
  std::string fieldType = this->chargeDensPhysFldPtr->getFieldType();
  dField = TxMakerMap< PsFieldBase<FLOATTYPE> >::getNew(fieldType);
  PsGridBaseItr* gItr = &this->getGridBase();
  dField->setGrid(gItr);

  // Set scale length from polymers
  //  This depends on order (from PsDomain) and should
  //  probably be re-factored to reside in PsFunctionalQ
  scaleLength = PsPolymer<FLOATTYPE, NDIM>::getScaleLength();
  this->dbprt("... scaleLength = ", (int)scaleLength);
}

//
template <class FLOATTYPE, size_t NDIM>
void PsSimpleIons<FLOATTYPE, NDIM>::setPhysFields() {

  this->dbprt("PsSimpleIons::setPhysFields() ");

  // Get electric potential
  PsFieldBase<FLOATTYPE>& eFactor = *dField;
  PsFieldBase<FLOATTYPE>& potentialField  =
    this->chargeDensPhysFldPtr->getConjgField();

  // Initialize temp space
  eFactor.reset(0.0);
  eFactor += potentialField;
  FLOATTYPE negFac = -1.0*zc;
  eFactor.scale(negFac);
  eFactor.apply_exp();

  // Adjusting volume for constraints
  size_t fieldSize = eFactor.getSize();
  FLOATTYPE localVol =
    FLOATTYPE(fieldSize) - this->constraintFieldPtr->calcLocalVolume();

  // Sum localbQc and Sum bQ over all ranks
  FLOATTYPE localbQc = eFactor.getSumAll();
  size_t nprocs = this->getCommBase().getSize();
  localbQc = localbQc / ( (FLOATTYPE)nprocs*localVol);
  FLOATTYPE globalbQc = this->getCommBase().allReduceSum(localbQc);

  // Initialize temp space
  eFactor.reset(0.0);
  eFactor += potentialField;
  FLOATTYPE zFac = -1.0*zc;
  eFactor.scale(zFac);
  eFactor.apply_exp();

  FLOATTYPE cFac = -neutralFactor/(globalbQc*zc);
  //  FLOATTYPE cFac = -1.0;
  eFactor.scale(cFac);

  // Set base data member
  this->bigQ = globalbQc;

  this->dbprt("globalQc = ", globalbQc);
  this->dbprt(" from solvent ", this->getName());

  //
  // Assumes that the density fields have been appropriately
  // initialized to zero, so they may act as counters.
  //
  this->dbprt("inside PsSimpleIons::setPhysFields()");
  this->dbprt("... updating the charge densities in physField ",
      this->chargeDensPhysFldPtr->getName() );

  // If tracking single block density then set field
  if (this->hasBlockField) {
    PsFieldBase<FLOATTYPE>& blockField = this->blockDensPhysFldPtr->getDensField();
    blockField.reset(0.0);
    blockField += eFactor;
  }

  // Set charge field with updated value
  PsFieldBase<FLOATTYPE>& densField = this->chargeDensPhysFldPtr->getDensField();

  // Scaling for density---> charge density
  eFactor.scale(zc);
  densField += eFactor;
}

// Initializes
template <class FLOATTYPE, size_t NDIM>
void PsSimpleIons<FLOATTYPE, NDIM>::initialize() {
} // end initialize

//
// Update simple solvent
//
template <class FLOATTYPE, size_t NDIM>
void PsSimpleIons<FLOATTYPE, NDIM>::update(double t) {

  // Base class update
  PsSolvent<FLOATTYPE, NDIM>::update(t);

  this->dbprt("PsSimpleIons::update() ");

  setPhysFields();
}

// Instantiate
template class PsSimpleIons<float, 1>;
template class PsSimpleIons<float, 2>;
template class PsSimpleIons<float, 3>;

template class PsSimpleIons<double, 1>;
template class PsSimpleIons<double, 2>;
template class PsSimpleIons<double, 3>;

