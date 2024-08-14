/**
 *
 * @file    PsSimpleSolvent.cpp
 *
 * @brief   Class for simple solvents whose interactions depend
 *          only on monomer separation
 *
 * @version $Id: PsSimpleSolvent.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// pspolymer includes
#include <PsSimpleSolvent.h>

// txbase includes
#include <TxTensor.h>
#include <TxMakerMap.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsSimpleSolvent<FLOATTYPE, NDIM>::PsSimpleSolvent() {
  dField = NULL;
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsSimpleSolvent<FLOATTYPE, NDIM>::~PsSimpleSolvent() {
  delete dField;
}

//
// set attributes
//
template <class FLOATTYPE, size_t NDIM>
void PsSimpleSolvent<FLOATTYPE, NDIM>::setAttrib(
     const TxHierAttribSetIntDbl& tas) {

  // Scoping call to base class
  PsSolvent<FLOATTYPE, NDIM>::setAttrib(tas);

}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsSimpleSolvent<FLOATTYPE, NDIM>::buildData() {

  // Scoping call to base class
  PsSolvent<FLOATTYPE, NDIM>::buildData();
}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsSimpleSolvent<FLOATTYPE, NDIM>::buildSolvers() {

  // Scoping call to base class
  PsSolvent<FLOATTYPE, NDIM>::buildSolvers();

  this->dbprt("PsSimpleSolvent::buildSolvers() ");

  // register name of solvent in physField list
  this->monoDensPhysFldPtr->registerSolvent(this->getName());

  // Add contribution to density scaling factor for physField
  this->monoDensPhysFldPtr->addToDensAverage(this->volfrac);

  // Field types of held monomer density fields
  // Return pointers to a PsFieldBase object
  std::string fieldType = this->monoDensPhysFldPtr->getFieldType();
  dField = TxMakerMap< PsFieldBase<FLOATTYPE> >::getNew(fieldType);
  PsGridBaseItr* gItr = &this->getGridBase();
  dField->setGrid(gItr);

  // Set scale length from polymers
  // This depends on order (from PsDomain) and should
  // probably be re-factored to reside in PsFunctionalQ
  scaleLength = PsPolymer<FLOATTYPE, NDIM>::getScaleLength();
  this->dbprt("... scaleLength = ", (int)scaleLength);
}

//
//
//
template <class FLOATTYPE, size_t NDIM>
void PsSimpleSolvent<FLOATTYPE, NDIM>::setPhysFields() {

  this->dbprt("SWS: check order of solvent scaling and integration");
  this->dbprt("PsSimpleSolvent::setPhysFields() ");

  FLOATTYPE vf = this->getVolfrac();
  this->dbprt("solvent vf = ", vf);

  // Calculate big Q directly
  PsFieldBase<FLOATTYPE>& wFactor = *dField;
  PsFieldBase<FLOATTYPE>& wField  = this->monoDensPhysFldPtr->getConjgField();

  wFactor.reset(0.0);
  wFactor += wField;
  FLOATTYPE negscaleLen = -1.0/((FLOATTYPE)scaleLength);
  wFactor *= negscaleLen;
  wFactor.apply_exp();

  this->dbprt("solvent scaleLength = ", (int)scaleLength);

  // Adjusting volume for constraints
  size_t fieldSize = wFactor.getSize();
  FLOATTYPE localVol =
    FLOATTYPE(fieldSize) - this->constraintFieldPtr->calcLocalVolume();

  // Sum localbQ
  FLOATTYPE localbQ = wFactor.getSumAll();
  size_t nprocs = this->getCommBase().getSize();
  localbQ = localbQ / ( (FLOATTYPE)nprocs*localVol);

  // Sum bQ over all ranks
  FLOATTYPE globalbQ = this->getCommBase().allReduceSum(localbQ);
  wFactor *= (vf/globalbQ);

  // Set base data member
  this->bigQ = globalbQ;

  this->dbprt("bigQ = ", globalbQ);
  this->dbprt(" from solvent ", this->getName());

  //
  // Assumes that the density fields have been appropriately
  // initialized to zero, so they may act as counters.
  //
  this->dbprt("inside PsSimpleSolvent::setPhysFields()" );
  this->dbprt("... updating the densities in physField ",
      this->monoDensPhysFldPtr->getName() );

  PsFieldBase<FLOATTYPE>& densField  =
    this->monoDensPhysFldPtr->getDensField();

  densField += wFactor;
}

// Initializes
template <class FLOATTYPE, size_t NDIM>
void PsSimpleSolvent<FLOATTYPE, NDIM>::initialize() {
} // end initialize

//
// Update simple solvent
//
template <class FLOATTYPE, size_t NDIM>
void PsSimpleSolvent<FLOATTYPE, NDIM>::update(double t) {

  // Base class update
  PsSolvent<FLOATTYPE, NDIM>::update(t);

  this->dbprt("PsSimpleSolvent::update() ");

  setPhysFields();
}

// Instantiate
template class PsSimpleSolvent<float, 1>;
template class PsSimpleSolvent<float, 2>;
template class PsSimpleSolvent<float, 3>;

template class PsSimpleSolvent<double, 1>;
template class PsSimpleSolvent<double, 2>;
template class PsSimpleSolvent<double, 3>;
