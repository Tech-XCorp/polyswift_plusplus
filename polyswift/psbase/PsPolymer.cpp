/**
 * @file    PsPolymer.cpp
 *
 * @brief   Base class interface for polymer chains
 *
 * @version $Id: PsPolymer.cpp 8257 2007-09-12 22:05:20Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsPolymer.h>

// txbase includes
#include <TxTensor.h>
#include <TxMakerMap.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsPolymer<FLOATTYPE, NDIM>::PsPolymer() {

  volfrac = 0.0;
  lengthRatio = 0.0;
  length = 0;
  scaleLength = 0;

  hasVfSTFunc = false;
  vfSTFunc = NULL;
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsPolymer<FLOATTYPE, NDIM>::~PsPolymer() {
  delete vfSTFunc;
}

template <class FLOATTYPE, size_t NDIM>
void PsPolymer<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Using base class to set attributes for base polymer class
  PsDynObjBase::setAttrib(tas);

  // Debug flag
  if (tas.hasString("printdebug")) {
    std::string dbPrint = tas.getString("printdebug");
    if (dbPrint == "on") this->dbStatus = PSDB_ON;
  }

  this->dbprt("PsPolymer::setAttrib(): entered.");

  // Volume fraction of polymer
  if (tas.hasParam("volfrac")) {
    volfrac = tas.getParam("volfrac");
   }

  // Number of statistical units in polymer
  if (tas.hasOption("length")) {
    length = tas.getOption("length");
   }

  // Parse input parameters for STFunc's, one for now
  funcNames = tas.getNamesOfType("STFunc");
  if (funcNames.size() > 0) {
    funcAttrib = tas.getAttrib(funcNames[0]);
    hasVfSTFunc = true;
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsPolymer<FLOATTYPE, NDIM>::buildData() {

  // Parse input parameters for STFunc's, one for now
  if (funcNames.size() > 0) {
    std::string kind = funcAttrib.getString("kind");
    vfSTFunc = TxMakerMap< PsSTFunc<FLOATTYPE, NDIM> >::getNew(kind);
    this->pprt("Found STFunc for PsPolymer setting the function...");
    vfSTFunc->setAttrib(funcAttrib);
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsPolymer<FLOATTYPE, NDIM>::buildSolvers() {}

template <class FLOATTYPE, size_t NDIM>
void PsPolymer<FLOATTYPE, NDIM>::update(double t) {

  FLOATTYPE simTime = (FLOATTYPE)t;
  FLOATTYPE x[NDIM];

  // Update any existing STFuncs
  if (hasVfSTFunc) {
    for (size_t j=0; j<NDIM; ++j) x[j] = 0.0;
    volfrac = (*this->vfSTFunc)(x, simTime);
    this->dbprt("polymer volume frac = ", volfrac);
  }

}

template <class FLOATTYPE, size_t NDIM>
std::vector<std::string> PsPolymer<FLOATTYPE, NDIM>::getPolymerNames() {
  return allPolymerNames;
}

template <class FLOATTYPE, size_t NDIM>
size_t PsPolymer<FLOATTYPE, NDIM>::getScaleLength() {
  return scaleLength;
}

template <class FLOATTYPE, size_t NDIM>
void PsPolymer<FLOATTYPE, NDIM>::setScaleLength(size_t scalelen) {
  scaleLength = scalelen;
}

template <class FLOATTYPE, size_t NDIM>
void PsPolymer<FLOATTYPE, NDIM>::setPolymerName(std::string pName) {
  allPolymerNames.push_back(pName);
}

//
// Declaration of static data
//
template <class FLOATTYPE, size_t NDIM>
std::vector<std::string> PsPolymer<FLOATTYPE, NDIM>::allPolymerNames;

template <class FLOATTYPE, size_t NDIM>
size_t PsPolymer<FLOATTYPE, NDIM>::scaleLength;

//
// Instantiate the templates
//
template class PsPolymer<float, 1>;
template class PsPolymer<float, 2>;
template class PsPolymer<float, 3>;

template class PsPolymer<double, 1>;
template class PsPolymer<double, 2>;
template class PsPolymer<double, 3>;
