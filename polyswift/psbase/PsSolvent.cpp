/**
 *
 * @file    PsSolvent.cpp
 *
 * @brief   Base class interface for solvent chains
 *
 * @version $Id: PsSolvent.cpp 8257 2007-09-12 22:05:20Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsSolvent.h>

// txbase includes
#include <TxTensor.h>
#include <TxMakerMap.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsSolvent<FLOATTYPE, NDIM>::PsSolvent() {

  hasVfSTFunc = false;
  vfSTFunc = NULL;
  volfrac = 0.0;
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsSolvent<FLOATTYPE, NDIM>::~PsSolvent() {}

template <class FLOATTYPE, size_t NDIM>
void PsSolvent<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Debug flag
  if (tas.hasString("printdebug")) {
    std::string dbPrint = tas.getString("printdebug");
    if (dbPrint == "on") this->dbStatus = PSDB_ON;
  }

  // Scoping call to base class
  PsFunctionalQ<FLOATTYPE, NDIM>::setAttrib(tas);
  this->dbprt("PsSolvent::setAttrib() ");

  // Volume fraction of solvent
  if (tas.hasParam("volfrac")) {
    volfrac = tas.getParam("volfrac");
  }

  // Parse input parameters for STFunc's, one for now
  funcNames = tas.getNamesOfType("STFunc");
  if (funcNames.size() > 0) {
    funcAttrib = tas.getAttrib(funcNames[0]);
    hasVfSTFunc = true;
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsSolvent<FLOATTYPE, NDIM>::buildData() {

  // Scoping call to base class
  PsFunctionalQ<FLOATTYPE, NDIM>::buildData();

  // Parse input parameters for STFunc's, one for now
  if (funcNames.size() > 0) {
    std::string kind = funcAttrib.getString("kind");
    vfSTFunc = TxMakerMap< PsSTFunc<FLOATTYPE, NDIM> >::getNew(kind);
    this->pprt("Found STFunc for PsPolymer setting the function...");
    vfSTFunc->setAttrib(funcAttrib);
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsSolvent<FLOATTYPE, NDIM>::buildSolvers() {

  // Scoping call to base class
  PsFunctionalQ<FLOATTYPE, NDIM>::buildSolvers();
}

template <class FLOATTYPE, size_t NDIM>
void PsSolvent<FLOATTYPE, NDIM>::update(double t) {

  FLOATTYPE simTime = (FLOATTYPE)t;
  FLOATTYPE x[NDIM];

  // Update any existing STFuncs
  if (hasVfSTFunc) {
    for (size_t j=0; j<NDIM; ++j) x[j] = 0.0;
    volfrac = (*this->vfSTFunc)(x, simTime);
    this->dbprt("solvent volume frac = ", volfrac);
  }

}

template <class FLOATTYPE, size_t NDIM>
std::vector<std::string> PsSolvent<FLOATTYPE, NDIM>::getSolventNames() {
  return allSolventNames;
}

template <class FLOATTYPE, size_t NDIM>
void PsSolvent<FLOATTYPE, NDIM>::setSolventName(std::string pName) {
  allSolventNames.push_back(pName);
}

// Declaration of static data
template <class FLOATTYPE, size_t NDIM>
std::vector<std::string> PsSolvent<FLOATTYPE, NDIM>::allSolventNames;

//
// Instantiate the templates
//
template class PsSolvent<float, 1>;
template class PsSolvent<float, 2>;
template class PsSolvent<float, 3>;

template class PsSolvent<double, 1>;
template class PsSolvent<double, 2>;
template class PsSolvent<double, 3>;

