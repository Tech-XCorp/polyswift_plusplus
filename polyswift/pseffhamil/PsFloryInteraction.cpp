/**
 *
 * @file    PsFloryInteraction.cpp
 *
 * @brief   Class for calculating energy from Flory interaction model
 *
 * @version $Id: PsFloryInteraction.cpp 8257 2007-09-12 22:05:20Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsPhysField.h>

// pseffhamil includes
#include <PsFloryInteraction.h>

// txbase includes
#include <TxTensor.h>
#include <TxMakerMap.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsFloryInteraction<FLOATTYPE, NDIM>::PsFloryInteraction() {

  chi = 0.01;
  Nlen = 1;
  chiN = chi*Nlen;
  densAvg0 = 0.0;
  densAvg1 = 0.0;

  hasConstChi = false;
  hasConstChiRamp = false;
  hasChiNrSTFunc = false;

  chiSTFunc     = NULL;
  chiNFieldPtr  = NULL;
  densAtWallPtr = NULL;
  //  chiNrSTFunc = NULL;
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsFloryInteraction<FLOATTYPE, NDIM>::~PsFloryInteraction() {

  delete densAtWallPtr;
  delete chiNFieldPtr;
  delete chiSTFunc;
  //  delete chiNrSTFunc;
  funcNames.clear();
  for (size_t n=0; n<stFuncVec.size(); ++n) {
    chiSTFunc = stFuncVec[n];
    delete chiSTFunc;
  }
  stFuncVec.clear();

}

template <class FLOATTYPE, size_t NDIM>
void PsFloryInteraction<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Scoping call to base class
  PsInteraction<FLOATTYPE, NDIM>::setAttrib(tas);

  this->dbprt("PsFloryInteraction::setAttrib() ");

  // Flory "chi" parameter
  if (tas.hasParam("chi")) {
    chi = tas.getParam("chi");
    hasConstChi = true;
  }

  // Parse input parameters for STFunc's only one for now
  funcNames = tas.getNamesOfType("STFunc");
  if (funcNames.size() > 2) {
    TxDebugExcept tde("PsFloryInteraction::setAttrib: >2 STFunc per/interaction");
    tde << " in <PsInteraction " << this->getName() << " >";
    throw tde;
  }

  // Store STFunc attribs
  for (size_t n=0; n<funcNames.size(); ++n)
    funcAttribs.push_back(tas.getAttrib(funcNames[n]));
}

//
// build data
//
template <class FLOATTYPE, size_t NDIM>
void PsFloryInteraction<FLOATTYPE, NDIM>::buildData() {

  // Scoping call to base class
  PsInteraction<FLOATTYPE, NDIM>::buildData();
  this->dbprt("PsFloryInteraction::buildData() ");

  // Parse input parameters for STFunc's
  for (size_t n=0; n<funcNames.size(); ++n) {

    std::string kind = funcAttribs[n].getString("kind");

    if (kind == "cutExpression" || kind == "chiCutExpression") {
      chiSTFunc = TxMakerMap< PsSTFunc<FLOATTYPE, NDIM> >::getNew(kind);
      this->pprt("PsFloryInteraction:found STFunc chi(t)", kind);
      chiSTFunc->setAttrib(funcAttribs[n]);
      hasConstChiRamp = true;
      stFuncVec.push_back(chiSTFunc);
    }

    if ( kind == "movTanhSlab" ||
         kind == "switchMovTanhSlab" ||
         kind == "pyfunc") {
      chiSTFunc = TxMakerMap< PsSTFunc<FLOATTYPE, NDIM> >::getNew(kind);
      this->pprt("PsFloryInteraction:found STFunc for chiN(r)", kind);
      chiSTFunc->setAttrib(funcAttribs[n]);
      hasChiNrSTFunc = true;
      stFuncVec.push_back(chiSTFunc);
    }
  }

  if (hasConstChi && (hasConstChiRamp || hasChiNrSTFunc) ) {
    TxDebugExcept tde("PsFloryInteraction::buildData");
    tde << " constant chi parameter cannot be set w/STFunc for chi"
        << " in <PsInteraction " << this->getName() << " >"
        << "\n Remove either STFunc block or chi param";
    throw tde;
  }

}

//
// buildSolvers
//
template <class FLOATTYPE, size_t NDIM>
void PsFloryInteraction<FLOATTYPE, NDIM>::buildSolvers() {

  // Scoping call to base class
  PsInteraction<FLOATTYPE, NDIM>::buildSolvers();

  this->dbprt("PsFloryInteraction::buildSolvers() ");

  // Set average monomer density values
  densAvg0 = 0.0;
  densAvg1 = 0.0;
  if (this->shiftDensFlag) {
    densAvg0 = this->physFields[0]->getDensAverage();
    densAvg1 = this->physFields[1]->getDensAverage();
  }

  this->dbprt("Density average = ", densAvg0);
  this->dbprt(" for ", this->physFields[0]->getName());
  this->dbprt("Density average = ", densAvg1);
  this->dbprt(" for ", this->physFields[1]->getName());

  // Scale chi value by static polymer scaling length
  Nlen = (FLOATTYPE) PsPolymer<FLOATTYPE, NDIM>::getScaleLength();
  chiN = chi*Nlen;

  // Return pointers to a PsFieldBase object
  // Same rank as physical fields
  std::string fType = this->physFields[0]->getFieldType();
  chiNFieldPtr = TxMakerMap< PsFieldBase<FLOATTYPE> >::getNew(fType);

  // Spatially dependent chi field
  // SWS: move to buildData (need to move fieldLengths in base class)
  //  localDims = this->physFields[0]->get Field Lengths();
  localDims = this->getGridBase().getDecomp().getNumCellsLocal();

  PsGridBaseItr* gItr = &this->getGridBase();
  chiNFieldPtr->setGrid(gItr);
  chiNFieldPtr->reset(chiN);

  // Local work chiN field space
  initLocalField.setGrid(gItr);

  //
  // Temp space for disordered density at wall
  //
  densAtWallPtr =
    TxMakerMap< PsFieldBase<FLOATTYPE> >::getNew(this->physFields[0]->getFieldType());
  if (!densAtWallPtr) {
    TxDebugExcept tde("PsFloryInteraction::buildSolvers: no field type set for temp space");
    tde << " in <Interaction " << this->getName() << " >";
    throw tde;
  }
  // densAtWallPtr->setLengths(localDims);
  densAtWallPtr->setGrid(gItr);

}

//
//
//
template <class FLOATTYPE, size_t NDIM>
void PsFloryInteraction<FLOATTYPE, NDIM>::initialize() {}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsFloryInteraction<FLOATTYPE, NDIM>::calcDfD(
     std::string wrtPhysField, PsFieldBase<FLOATTYPE>& dField) {

  // Diagnostic output
  this->dbprt("PsFloryInteraction::calcDfD() wrt to ", wrtPhysField);
  if (wrtPhysField == this->scfieldNames[0]) {
    this->dbprt("..return fld ", this->scfieldNames[1]);
  }
  else {
    this->dbprt(".... return fld ", this->scfieldNames[0]);
  }

  // Constraint field
  PsFieldBase<FLOATTYPE>& wallField = this->constraintFieldPtr->getDensField();

  // Reference to interaction field
  PsFieldBase<FLOATTYPE>& chiNr = *chiNFieldPtr;

  //
  // Switch for functional derivative contribution
  //
  // SWS: note this maskfield can probably be removed
  //
  if (wrtPhysField == this->scfieldNames[0]) {

    // Reference to density field held in base class
    PsFieldBase<FLOATTYPE>& densField1 = this->physFields[1]->getDensField();

    // Construct mask field for average  -densAvg1(1-wallField)
    PsFieldBase<FLOATTYPE>& maskField  = *(this->tmpFieldPtr);
    maskField.reset(0.0);
    maskField += wallField;
    maskField *= -1.0;
    maskField += 1.0;
    maskField *= -1.0*densAvg1;

    // dField = chiN * (densField1 - densAvg1)
    dField.reset(0.0);
    dField += densField1;
    // dField += -1.0*densAvg1;
    dField += maskField;
    dField *= chiNr;
  }

  else {

    // Reference to density field held in base class
    PsFieldBase<FLOATTYPE>& densField0 = this->physFields[0]->getDensField();

    // Construct mask field for average  -densAvg0(1-wallField)
    PsFieldBase<FLOATTYPE>& maskField  = *(this->tmpFieldPtr);
    maskField.reset(0.0);
    maskField += wallField;
    maskField *= -1.0;
    maskField += 1.0;
    maskField *= -1.0*densAvg0;

    // dField = chiN * (densField0 - densAvg0)
    dField.reset(0.0);
    dField += densField0;
    // dField += -1.0*densAvg0;
    dField += maskField;
    dField *= chiNr;
  }

}

//
// Calculate contribution to free-energy per chain...
//
template <class FLOATTYPE, size_t NDIM>
void PsFloryInteraction<FLOATTYPE, NDIM>::calcFe(
      PsFieldBase<FLOATTYPE>& dField,
      bool calcDisorder) {

  this->dbprt("PsFloryInteraction::calcFe ");

  // References to density fields held in base class
  PsFieldBase<FLOATTYPE>& densField0 = this->physFields[0]->getDensField();
  PsFieldBase<FLOATTYPE>& densField1 = this->physFields[1]->getDensField();
  PsFieldBase<FLOATTYPE>& wallField  = this->constraintFieldPtr->getDensField();

  // Reference to interaction field
  PsFieldBase<FLOATTYPE>& chiNr = *chiNFieldPtr;

  // Density at wall approx (1 - wallField)
  PsFieldBase<FLOATTYPE>& densAtWallField = *(densAtWallPtr);
  densAtWallField.reset(1.0);
  densAtWallField -= wallField;

  // Disorder contribution
  // Approximate dens field in disordered state by 'mask' field
  //  phiavg0*phiavg1*chiN*(1- wallField)*(1 - wallField)
  PsFieldBase<FLOATTYPE>& densDisorderField  = *(this->tmpFieldPtr);
  densDisorderField.reset(0.0);
  densDisorderField += densAtWallField;
  densDisorderField *= densAtWallField;
  densDisorderField *= densAvg0;
  densDisorderField *= densAvg1;
  densDisorderField *= chiNr;

  // Calculate: chiN * (densField0 * densField1)
  dField.reset(0.0);
  dField += densField0;
  dField *= densField1;
  dField *= chiNr;

  if (calcDisorder)
    dField -= densDisorderField;

  // Volume scale
  // Get total volume (less constraint volume)
  // Recomputed in case constraint volume changes during calculation
  size_t fieldSize = dField.getSize();
  FLOATTYPE localVol =
    FLOATTYPE(fieldSize)-this->constraintFieldPtr->calcLocalVolume();
  dField.scale(1.0/localVol);
}

//
template <class FLOATTYPE, size_t NDIM>
void PsFloryInteraction<FLOATTYPE, NDIM>::update(double t) {

  this->dbprt("PsFloryInteraction::update(t = ", (int)t);

  // Initialize and calculate correct parameters for updating the owned STFunc
  FLOATTYPE simTime = (FLOATTYPE)t;
  FLOATTYPE x[NDIM];

  // Update any existing STFuncs
  if (hasConstChiRamp) {

    if (stFuncVec.size() > 1) {
      TxDebugExcept tde("PsFloryInteraction::update: > 1 STFuncs");
      tde << "only 1 STFunc for constant ramp chi expected ";
      tde << " in <PsInteraction " << this->getName() << " >";
      throw tde;
    }

    for (size_t j=0; j<NDIM; ++j) x[j] = 0.0;
    chiSTFunc = stFuncVec[0]; // only one constant Tfunc
    chi = chiSTFunc->operator()(x, simTime);
    // (*this->chiSTFunc)(x, simTime);
    chiN = chi*Nlen;
  }

  // Assign chiN(r) field values if STFunc owned
  if (hasChiNrSTFunc) {

    // Load cavity values
    for (size_t x=0; x<localDims[0]; ++x) {
    for (size_t y=0; y<localDims[1]; ++y) {
    for (size_t z=0; z<localDims[2]; ++z) {

      // Setup tiny vector
      PsTinyVector<int, NDIM> locPosVec(x,y,z);
      PsTinyVector<int, NDIM> posVec =
          this->getGridBase().mapToGlobalVec(locPosVec);

      // Evaluate all STFunc's and add
      FLOATTYPE val=0.0;
      for (size_t n=0; n<stFuncVec.size(); ++n) {
        chiSTFunc = stFuncVec[n];
        val = val + chiSTFunc->operator()(posVec, simTime);
      }
      // FLOATTYPE val = chiNrSTFunc->operator()(posVec, simTime);
      initLocalField(locPosVec[0], locPosVec[1], locPosVec[2], 0) = val;

    }}}

    // Assign chiN(r) field values
    PsFieldBase<FLOATTYPE>& chiNField = *chiNFieldPtr;
    PsFieldBase<FLOATTYPE>& initField = *(initLocalField.getBasePtr());
    chiNField.reset(0.0);
    chiNField += initField;
  }

  else {
    // Default assign uniform chiN(r) field values
    chiNFieldPtr->reset(chiN);
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsFloryInteraction<FLOATTYPE, NDIM>::dump() {}

//
// Instantiate classes
//
template class PsFloryInteraction<float, 1>;
template class PsFloryInteraction<float, 2>;
template class PsFloryInteraction<float, 3>;

template class PsFloryInteraction<double, 1>;
template class PsFloryInteraction<double, 2>;
template class PsFloryInteraction<double, 3>;
