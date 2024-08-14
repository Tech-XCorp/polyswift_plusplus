/**
 * @file    PsChargeFlexPseudoSpec.cpp
 *
 * @brief   Class for solving propagators for flexible Gaussian blocks
 *          with pseudo-spectral algorithm
 *
 * @version $Id: PsChargeFlexPseudoSpec.cpp 8257 2007-09-12 22:05:20Z swsides $
 *
 * Copyright &copy; 2010-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsFieldBase.h>

// pspolymer includes
#include <PsChargeFlexPseudoSpec.h>

// psstd includes
#include <PsRandom.h>
#include <PsPhysConsts.h>

// Constructor
template <class FLOATTYPE, size_t NDIM, class QTYPE>
PsChargeFlexPseudoSpec<FLOATTYPE, NDIM, QTYPE>::PsChargeFlexPseudoSpec() {

  //  this->blockTypeStr = "chargedFlexibleBlock";
  // This would be used in convertQ in PsBlockBase because the same
  // q(r,s) rank is used.
  this->blockTypeStr = "flexibleBlock";
}

// Destructor
template <class FLOATTYPE, size_t NDIM, class QTYPE>
PsChargeFlexPseudoSpec<FLOATTYPE, NDIM, QTYPE>::~PsChargeFlexPseudoSpec() {
}

template <class FLOATTYPE, size_t NDIM, class QTYPE>
void PsChargeFlexPseudoSpec<FLOATTYPE, NDIM, QTYPE>::setAttrib(
     const TxHierAttribSetIntDbl& tas) {

  // Scoping call to base class
  PsFlexPseudoSpec<FLOATTYPE, NDIM, QTYPE>::setAttrib(tas);

  this->dbprt("PsChargeFlexPseudoSpec::setAttrib() ");

  // Charge strength Z_a
  if (tas.hasParam("za")) {
    za = tas.getParam("za");
  }
  else {
    TxDebugExcept tde("PsChargeFlexPseudoSpec::setAttrib");
    tde << " in <PsFlexPseudoSpec " << this->getName() << " >";
    tde << " charge strength za is not set";
    std::cout << tde.getMessage() << std::endl;
    throw tde;
  }

  // Charge distribution alpha
  if (tas.hasParam("alpha")) {
    alpha = tas.getParam("alpha");
  }
  else {
    TxDebugExcept tde("PsChargeFlexPseudoSpec::setAttrib");
    tde << " in <PsFlexPseudoSpec " << this->getName() << " >";
    tde << " charge distribution parameter alpha is not set";
    std::cout << tde.getMessage() << std::endl;
    throw tde;
  }

}

//
// build data
//
template <class FLOATTYPE, size_t NDIM, class QTYPE>
void PsChargeFlexPseudoSpec<FLOATTYPE, NDIM, QTYPE>::buildData() {

  // Scoping call to base class, sets the spatial part of qDims
  PsFlexPseudoSpec<FLOATTYPE, NDIM, QTYPE>::buildData();
}

template <class FLOATTYPE, size_t NDIM, class QTYPE>
void PsChargeFlexPseudoSpec<FLOATTYPE, NDIM, QTYPE>::buildSolvers() {

  // Scoping call to base class
  PsFlexPseudoSpec<FLOATTYPE, NDIM, QTYPE>::buildSolvers();
}

//
// Reset blocks at beginning of new solve for w-fields
//
template <class FLOATTYPE, size_t NDIM, class QTYPE>
void PsChargeFlexPseudoSpec<FLOATTYPE, NDIM, QTYPE>::reset() {

  // Scoping call to base class, this sets the monomer field factor
  PsFlexPseudoSpec<FLOATTYPE, NDIM, QTYPE>::reset();

  this->dbprt("PsChargeFlexPseudoSpec::reset() ");
  this->dbprt("... setting w-fields with physField ",
              this->chargeDensPhysFldPtr->getName() );

  // Factor for Z_a*alpha*N
  FLOATTYPE nLen = (FLOATTYPE)PsPolymer<FLOATTYPE, NDIM>::getScaleLength();
  FLOATTYPE facZ = za*alpha*nLen;

  // Get physField and the associated conjugate field
  // Obtain data pointer from fieldBase
  //  FLOATTYPE ds2 = -0.5*this->ds; double counting negative sign
  FLOATTYPE ds2 = -0.5*this->ds;
  PsFieldBase<FLOATTYPE>& wField = this->chargeDensPhysFldPtr->getConjgField();
  const FLOATTYPE* wDataPtr = wField.getConstDataPtr();

  // Set w fields and scale by ds factor
  // adding in the contribution from the charge field
  for (size_t n=0; n<this->fftSize; ++n) {
    this->wfac[n] = this->wfac[n]* (FLOATTYPE)std::exp(facZ*ds2*wDataPtr[n]);
  }

  // Debug field values
  size_t fieldNum = 5;
  for (size_t nn=0; nn<fieldNum; ++nn) {
    this->dbprt("wfac[0...5] = ", this->wfac[nn]);
  }
}

//
// This updates specific charge density using momomer density
/// from scoping call
//
template <class FLOATTYPE, size_t NDIM, class QTYPE>
void PsChargeFlexPseudoSpec<FLOATTYPE, NDIM, QTYPE>::setPhysFields() {

  // Scoping call to base class
  // After this call ... the local block monomer density is updated
  PsBlock<FLOATTYPE, NDIM, QTYPE>::setPhysFields();

  // Update charge densities
  if (this->hasChargeField)
    this->dbprt("... updating the densities in physField ",
                this->chargeDensPhysFldPtr->getName() );

  PsFieldBase<FLOATTYPE>& densField = this->monoDensPhysFldPtr->getDensField();

  PsGridField<FLOATTYPE, NDIM> tmpChargeField;
  PsGridBaseItr* gItr = &this->getGridBase();
  tmpChargeField.setGrid(gItr);

  // Add to appropriate density field
  tmpChargeField += densField;
  tmpChargeField.scale(alpha);
  tmpChargeField.scale(za);

  // Update charge density based on monomer density
  PsFieldBase<FLOATTYPE>& chargeField =
    this->chargeDensPhysFldPtr->getDensField();

  // Assign charge field values
  PsFieldBase<FLOATTYPE>& setChargeField = *(tmpChargeField.getBasePtr());
  chargeField += setChargeField;
}

// Instantiate classes (for flexible block model)
template class PsChargeFlexPseudoSpec<float, 1, PsBlockTypes<float, 1>::flexQType >;
template class PsChargeFlexPseudoSpec<float, 2, PsBlockTypes<float, 2>::flexQType >;
template class PsChargeFlexPseudoSpec<float, 3, PsBlockTypes<float, 3>::flexQType >;

template class PsChargeFlexPseudoSpec<double, 1, PsBlockTypes<double, 1>::flexQType >;
template class PsChargeFlexPseudoSpec<double, 2, PsBlockTypes<double, 2>::flexQType >;
template class PsChargeFlexPseudoSpec<double, 3, PsBlockTypes<double, 3>::flexQType >;
