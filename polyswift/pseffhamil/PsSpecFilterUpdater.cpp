/**
 *
 * @file    PsSpecFilterUpdater.cpp
 *
 * @brief   Class for updating fields with spectral filter algorithm
 *
 * @version $Id: PsSpecFilterUpdater.cpp 8257 2007-09-12 22:05:20Z swsides $
 *
 * Copyright &copy; 2009-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// pseffhamil includes
#include <PsSpecFilterUpdater.h>

// txbase includes
#include <TxTensor.h>
#include <TxMakerMap.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsSpecFilterUpdater<FLOATTYPE, NDIM>::PsSpecFilterUpdater() {

  cutoffFactor = 0.0;
  filterStrength = 0.0;
  hasCutSTFunc = false;

  fftObjPtr = NULL;

  // Owned memory
  resPtr     = NULL;
  maskCutPtr = NULL;
  cutSTFunc = NULL;

}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsSpecFilterUpdater<FLOATTYPE, NDIM>::~PsSpecFilterUpdater() {

  // Owned memory
  delete cutSTFunc;
  delete[] resPtr;
  delete[] maskCutPtr;
}

template <class FLOATTYPE, size_t NDIM>
void PsSpecFilterUpdater<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Scoping call to base class
  PsUpdater<FLOATTYPE, NDIM>::setAttrib(tas);

  this->dbprt("PsSpecFilterUpdater::setAttrib() ");

  // Cutoff strength
  if (tas.hasParam("cutoffFactor")) {
    double tmp = tas.getParam("cutoffFactor");
    cutoffFactor = (FLOATTYPE) tmp;
  }
  else {
    this->dbprt("cutoffFactor default to 0.0 (ie...no filter)" );
  }

  // Cutoff strength
  if (tas.hasParam("filterStrength")) {
    double tmp = tas.getParam("filterStrength");
    filterStrength = (FLOATTYPE) tmp;
  }
  else {
    this->pprt("filterStrength default set to 0.0");
  }

  // Parse input parameters for STFunc's only one for now
  funcNames = tas.getNamesOfType("STFunc");
  if (funcNames.size() > 0) {
    funcAttrib = tas.getAttrib(funcNames[0]);
  }

  // Object name of FFT object
  if (tas.hasString("fftKind")) {
    fftKind = tas.getString("fftKind");
   }
  else {
    TxDebugExcept tde("PsSpecFilterUpdater::setAttrib: fftKind not set");
    tde << " in <PsSpecFilterUpdater " << this->getName() << " >";
    throw tde;
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsSpecFilterUpdater<FLOATTYPE, NDIM>::buildData() {

  // Scoping call to base class
  PsUpdater<FLOATTYPE, NDIM>::buildData();
  this->dbprt("PsSpecFilterUpdater::buildData() ");

  // Parse input parameters for STFunc's only one for now
  if (funcNames.size() > 0) {
    std::string kind = funcAttrib.getString("kind");
    cutSTFunc = TxMakerMap< PsSTFunc<FLOATTYPE, NDIM> >::getNew(kind);
  }
  if (!cutSTFunc) {
    this->dbprt("No STFunc found for PsSpecFilterUpdater: using fixed cutoff");
  }
  else {
    this->dbprt("Found STFunc for PsSpecFilterUpdater: setting the function...");
    cutSTFunc->setAttrib(funcAttrib);
    hasCutSTFunc = true;
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsSpecFilterUpdater<FLOATTYPE, NDIM>::buildSolvers() {

  // Scoping call to base class
  PsConstraintUpdater<FLOATTYPE, NDIM>::buildSolvers();
  this->dbprt("PsSpecFilterUpdater::buildSolvers() ");

  // Get default FFTW method pointer
  fftObjPtr = PsNamedObject::getObject<PsFFTBase<FLOATTYPE, NDIM> >(fftKind);

  // SWS: should be moved to buildData(), returning "total_local_size"
  fftSize = fftObjPtr->getFFTSize();

  // Local data pointer for results and
  // Local data pointer for applying cutoffr
  resPtr     = new FLOATTYPE[fftSize];
  maskCutPtr = new FLOATTYPE[fftSize];

  // Set FFT scaling
  scaleFFT = 1.0 / ( (FLOATTYPE)this->getGridBase().getTotalCellsGlobal() );

  // This is particular to slab layout (grid)
  PsGridBase<FLOATTYPE, NDIM>* fftGridPtr = &fftObjPtr->getGrid();
  std::vector<size_t> kDims  = fftGridPtr->getDecomp().getNumCellsLocal();
  std::vector<size_t> shifts = fftGridPtr->getDecomp().getLocalToGlobalShifts();
  ny_trans      = kDims[1];
  y_start_trans = shifts[1];
}

//
// Initialize before update step
// SWS: reorder this so initialize is only called at startup.....
template <class FLOATTYPE, size_t NDIM>
void PsSpecFilterUpdater<FLOATTYPE, NDIM>::initialize() {

  // Scoping call to base class
  PsUpdater<FLOATTYPE, NDIM>::initialize();
}

template <class FLOATTYPE, size_t NDIM>
void PsSpecFilterUpdater<FLOATTYPE, NDIM>::update(double t) {

  // Scoping call to base class
  PsUpdater<FLOATTYPE, NDIM>::update(t);

  this->dbprt("PsSpecFilterUpdater::update() ");

  // If not updating this time step...exit
  if (!this->updateFlag) return;

  // Initialize and calculate correct parameters
  // for updating the owned STFunc
  FLOATTYPE simTime = (FLOATTYPE)t;
  FLOATTYPE x[NDIM];
  for (size_t j=0; j<NDIM; ++j) { x[j] = 0.0; }

  // Update any existing STFuncs
  if (hasCutSTFunc) {
    cutoffFactor = (*cutSTFunc)(x, simTime);
    this->pprt("... filtering with factor = ", cutoffFactor);
  }

} // end update

//
// Find maximum value in array
//
template <class FLOATTYPE, size_t NDIM>
size_t PsSpecFilterUpdater<FLOATTYPE, NDIM>::findMaxPos(
       FLOATTYPE* data, size_t dataSize) {

  // SWS: this is a hack and needs changing
  FLOATTYPE maxVal = -1000000.00;
  size_t maxPos = 0;

  for (size_t n=0; n<dataSize; ++n) {
    if (data[n] > maxVal) {
      maxVal = data[n];
      maxPos = n;
    }
  }

  return maxPos;
}

//
// Find maximum value in array
//
template <class FLOATTYPE, size_t NDIM>
FLOATTYPE PsSpecFilterUpdater<FLOATTYPE, NDIM>::findMaxVal(
      FLOATTYPE* data, size_t dataSize) {

  // SWS: this is a hack and needs changing
  FLOATTYPE maxVal = -1000000.00;

  for (size_t n=0; n<dataSize; ++n) {
    if (data[n] > maxVal) {
      maxVal = data[n];
    }
  }

  return maxVal;
}

//
// Helper method to subtract out S(0) component of chemical
// potential fields
//
template <class FLOATTYPE, size_t NDIM>
void PsSpecFilterUpdater<FLOATTYPE, NDIM>::subtractAverage(PsFieldBase<FLOATTYPE>& wf) {

  this->dbprt("PsSpecFilterUpdater::subtractAverage ");

  size_t fieldSize = wf.getSize();
  FLOATTYPE localVol =
    FLOATTYPE(fieldSize) - this->constraintFieldPtr->calcLocalVolume();

  this->dbprt("fieldSize = ", (int)fieldSize);
  this->dbprt("localVol  = ", localVol);

  // Sum field averages to damp out S(0) components
  size_t nprocs = this->getCommBase().getSize();
  FLOATTYPE localwsum = wf.getSumAll()/( (FLOATTYPE) (nprocs*localVol) );
  FLOATTYPE wtot = -1.0*this->getCommBase().allReduceSum(localwsum);

  // SWS: careful... this changes actual field values
  wf += wtot;
}

// Instantiate classes
template class PsSpecFilterUpdater<float, 1>;
template class PsSpecFilterUpdater<float, 2>;
template class PsSpecFilterUpdater<float, 3>;

template class PsSpecFilterUpdater<double, 1>;
template class PsSpecFilterUpdater<double, 2>;
template class PsSpecFilterUpdater<double, 3>;
