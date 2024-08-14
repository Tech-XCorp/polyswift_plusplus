/**
 * @file    PsFloryWallInteraction.cpp
 *
 * @brief   Class for calculating energy from FloryWall interaction model
 *
 * @version $Id: PsFloryWallInteraction.cpp 8257 2007-09-12 22:05:20Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// txbase includes
#include <TxTensor.h>

// psbase includes
#include <PsPhysField.h>

// pseffhamil includes
#include <PsFloryWallInteraction.h>

//
// Constructor
template <class FLOATTYPE, size_t NDIM>
PsFloryWallInteraction<FLOATTYPE, NDIM>::PsFloryWallInteraction() {

  densAvg = 0.0;
  wallFieldPtr = NULL;
  densFieldPtr = NULL;
}

//
// Destructor
template <class FLOATTYPE, size_t NDIM>
PsFloryWallInteraction<FLOATTYPE, NDIM>::~PsFloryWallInteraction() {
  delete wallFieldPtr;
  delete densFieldPtr;
}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsFloryWallInteraction<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  this->dbprt("PsFloryWallInteraction::setAttrib() ");

  // Scoping call to base class
  PsFloryInteraction<FLOATTYPE, NDIM>::setAttrib(tas);

  if (tas.hasString("wallField")) {
    wallFieldName = tas.getString("wallField");
  }
  else {
    TxDebugExcept tde("PsFloryWallInteraction::setAttrib: wall field not specified");
    tde << " in <Interaction " << this->getName() << " >";
    throw tde;
  }

}

//
// build data
//
template <class FLOATTYPE, size_t NDIM>
void PsFloryWallInteraction<FLOATTYPE, NDIM>::buildData() {

  // Scoping call to base class
  PsFloryInteraction<FLOATTYPE, NDIM>::buildData();
}

//
// buildSolvers
//
template <class FLOATTYPE, size_t NDIM>
void PsFloryWallInteraction<FLOATTYPE, NDIM>::buildSolvers() {

  // Scoping call to base class
  PsFloryInteraction<FLOATTYPE, NDIM>::buildSolvers();
  this->dbprt("PsFloryWallInteraction::buildSolvers() ");

  // Set local wall field pointer
  std::string physFieldName;
  for (size_t n=0; n<this->physFields.size(); ++n) {

    physFieldName = this->physFields[n]->getName();

    if ( wallFieldName == physFieldName) {
      wallFieldPtr = this->physFields[n];
      this->dbprt("wallField set to ", physFieldName);
    }
  }

  // Set density field
  densFieldPtr = this->getOtherPhysField(wallFieldName);
  if (this->shiftDensFlag)
    densAvg = densFieldPtr->getDensAverage();

  this->dbprt("other density field     = ", densFieldPtr->getName() );
  this->dbprt("other density field avg = ", densAvg);
}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsFloryWallInteraction<FLOATTYPE, NDIM>::initialize() {

  // Scoping call to base class
  PsFloryInteraction<FLOATTYPE, NDIM>::initialize();
}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsFloryWallInteraction<FLOATTYPE, NDIM>::calcDfD(
         std::string wrtPhysField, PsFieldBase<FLOATTYPE>& dField) {

  this->dbprt("PsFloryWallInteraction::calcDfD() wrt to ", wrtPhysField);

  PsFieldBase<FLOATTYPE>& chiNr = *(this->chiNFieldPtr);

  // Reference to result holder in base class
  dField.reset(0.0);

  // Switch for functional derivative contribution
  if (wrtPhysField == this->scfieldNames[0]) {

    this->dbprt("..return fld qnt from ", this->scfieldNames[1]);

    // Reference to density field held in base class
    PsFieldBase<FLOATTYPE>& densField1 = this->physFields[1]->getDensField();
    // dField = chiwall * dn0 * densField1
    dField += densField1;
    dField *= chiNr;
  }

  else {

    this->dbprt(".... return fld qnt from ", this->scfieldNames[0]);

    // Reference to density field held in base class
    PsFieldBase<FLOATTYPE>& densField0 = this->physFields[0]->getDensField();
    // dField = chiwall * dn0 * densField1
    dField += densField0;
    dField *= chiNr;
  }

}

//
//
//
template <class FLOATTYPE, size_t NDIM>
void PsFloryWallInteraction<FLOATTYPE, NDIM>::calcFe(
      PsFieldBase<FLOATTYPE>& dField,
      bool calcDisorder) {

  this->dbprt("PsFloryWallInteraction::calcFe ");

  // Get total volume (less constraint volume)
  // Recomputed in case constraint volume changes during calculation
  size_t fieldSize = dField.getSize();
  FLOATTYPE localVol = FLOATTYPE(fieldSize) -
    this->constraintFieldPtr->calcLocalVolume();

  // References to density fields held in base class
  PsFieldBase<FLOATTYPE>& wallField = wallFieldPtr->getDensField();
  PsFieldBase<FLOATTYPE>& densField = densFieldPtr->getDensField();

  // Reference to interaction field
  PsFieldBase<FLOATTYPE>& chiNr = *(this->chiNFieldPtr);

  // Get average monomer density
  FLOATTYPE monoDensAvg = densFieldPtr->getDensAverage();

  // Approximate dens field in disordered state by 'mask' field
  //  phiavg*chiN_WA*wallField*(1 - wallField)
  PsFieldBase<FLOATTYPE>& densDisorderField  = *(this->tmpFieldPtr);
  densDisorderField.reset(1.0);
  densDisorderField -= wallField;
  densDisorderField *= wallField;
  densDisorderField *= monoDensAvg;
  densDisorderField *= chiNr;

  // Calculate: chiN * (densField * wallField)
  dField.reset(0.0);
  dField += wallField;
  dField *= densField;
  dField *= chiNr;

  // [chiN * (densField * wallField)] - disOrderFe
  if (calcDisorder)
    dField -= densDisorderField;

  // Volume scaling
  dField.scale(1.0/localVol);
}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsFloryWallInteraction<FLOATTYPE, NDIM>::update(double t) {

  // Scoping call to base class
  PsFloryInteraction<FLOATTYPE, NDIM>::update(t);
}

//
// Instantiate classes
//
template class PsFloryWallInteraction<float, 1>;
template class PsFloryWallInteraction<float, 2>;
template class PsFloryWallInteraction<float, 3>;

template class PsFloryWallInteraction<double, 1>;
template class PsFloryWallInteraction<double, 2>;
template class PsFloryWallInteraction<double, 3>;
