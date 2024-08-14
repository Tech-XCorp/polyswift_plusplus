/**
 *
 * @file    PsPoissonUpdater.cpp
 *
 * @brief   Class for updating fields with Poisson solve
 *
 * @version $Id: PsPoissonUpdater.cpp 8257 2007-09-12 22:05:20Z swsides $
 *
 * Copyright &copy; 2010-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// pseffhamil includes
#include <PsPoissonUpdater.h>

// txbase includes
#include <TxTensor.h>
#include <TxMakerMap.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsPoissonUpdater<FLOATTYPE, NDIM>::PsPoissonUpdater() {

  fftObjPtr = NULL;
  k2     = NULL;
  resPtr = NULL;
  cutSTFunc = NULL;

  fftSize = 0;
  scaleFFT = 0.0;

  hasCutSTFunc = false;
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsPoissonUpdater<FLOATTYPE, NDIM>::~PsPoissonUpdater() {

  // Owned memory
  delete cutSTFunc;
  delete[] k2;
  delete[] resPtr;
}

template <class FLOATTYPE, size_t NDIM>
void PsPoissonUpdater<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Scoping call to base class
  PsUpdater<FLOATTYPE, NDIM>::setAttrib(tas);

  this->dbprt("PsPoissonUpdater::setAttrib() ");

  // Relaxation step size
  if (tas.hasParam("relaxlambda")) {
    relaxlambda = tas.getParam("relaxlambda");
  }
  else {
    TxDebugExcept tde("PsPoissonUpdater::setAttrib: relaxlambda not set");
    tde << " in <Updater " << this->getName() << " >";
    std::cout << tde.getMessage() << std::endl;
    throw tde;
  }

  // Bjerrum length
  if (tas.hasParam("bjerrumLen")) {
    bjerrumLen = tas.getParam("bjerrumLen");
  }
  else {
    TxDebugExcept tde("PsPoissonUpdater::setAttrib: bjerrumLen not set");
    tde << " in <Updater " << this->getName() << " >";
    std::cout << tde.getMessage() << std::endl;
    throw tde;
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
    TxDebugExcept tde("PsPoissonUpdater::setAttrib: fftKind not set");
    tde << " in <PsPoissonUpdater " << this->getName() << " >";
    throw tde;
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsPoissonUpdater<FLOATTYPE, NDIM>::buildData() {

  // Scoping call to base class
  PsUpdater<FLOATTYPE, NDIM>::buildData();
  this->dbprt("PsPoissonUpdater::buildData() ");

  // Parse input parameters for STFunc's only one for now
  if (funcNames.size() > 0) {
    std::string kind = funcAttrib.getString("kind");
    cutSTFunc = TxMakerMap< PsSTFunc<FLOATTYPE, NDIM> >::getNew(kind);
  }
  if (!cutSTFunc) {
    this->dbprt("No STFunc found for PsPoissonUpdater: using fixed cutoff");
  }
  else {
    this->dbprt("Found STFunc for PsPoissonUpdater: setting the function...");
    cutSTFunc->setAttrib(funcAttrib);
    hasCutSTFunc = true;
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsPoissonUpdater<FLOATTYPE, NDIM>::buildSolvers() {

  // Scoping call to base class
  PsUpdater<FLOATTYPE, NDIM>::buildSolvers();

  this->dbprt("PsPoissonUpdater::buildSolvers() ");

  // Get default FFTW method pointer
  fftObjPtr = PsNamedObject::getObject<PsFFTBase<FLOATTYPE, NDIM> >(fftKind);

  // SWS: should be moved to buildData()....
  // This is returning "total_local_size"
  fftSize = fftObjPtr->getFFTSize();
  this->dbprt("fftSize = ", (int)fftSize);

  // Allocate space for transform lists
  k2     = new FLOATTYPE[fftSize];
  resPtr = new FLOATTYPE[fftSize];

  // Set FFT scaling
  scaleFFT =
    1.0 / ( (FLOATTYPE)this->getGridBase().getTotalCellsGlobal() );

  // Build the k2 list needed for Poisson solve
  build_k2();
}

//
// helper method to calck2 list for calculating Laplacian
// in Poisson update
//
template <class FLOATTYPE, size_t NDIM>
void PsPoissonUpdater<FLOATTYPE, NDIM>::build_k2() {

  // Local holder for k2 values
  PsGridField<FLOATTYPE, NDIM> k2Field;
  PsGridBaseItr* gItr = &this->getGridBase();
  k2Field.setGrid(gItr);

  std::vector<size_t> updateLengths = this->getGridBase().getDecomp().getNumCellsLocal();

  // Get k2 values from default grid
  k2Field.calck2();
  this->dbprt("From PoissonUpdater:build_k2 k2Field(0, 0, 0, 0) ", k2Field(0, 0, 0, 0));

  // Map k2 field values to normal order layout for Laplacian in Poisson solve
  size_t n=0;
  for (size_t i = 0; i < updateLengths[0]; ++i) {
    for (size_t j = 0; j< updateLengths[1]; ++j) {
      for (size_t k = 0; k < updateLengths[2]; ++k) {
        if (k2Field(i, j, k, 0) < 0.000000001) {
          k2[n] = 0.0;
        } else {
          k2[n] = (4.0*3.1415*bjerrumLen)/k2Field(i, j, k, 0);
        }
        n++;
      }
    }
  }

// Set FFT scaling
  scaleFFT = 1.0 / ((FLOATTYPE) this->getGridBase().getTotalCellsGlobal() );
}

// Initialize
template <class FLOATTYPE, size_t NDIM>
void PsPoissonUpdater<FLOATTYPE, NDIM>::initialize() {

  // Scoping call to base class
  PsUpdater<FLOATTYPE, NDIM>::initialize();
}

//
template <class FLOATTYPE, size_t NDIM>
void PsPoissonUpdater<FLOATTYPE, NDIM>::update(double t) {

  // Scoping call to base class
  PsUpdater<FLOATTYPE, NDIM>::update(t);

  this->dbprt("PsPoissonUpdater::update() ");

  // If not updating this time step...exit
  if (!this->updateFlag) return;

  // Initialize and calculate correct parameters
  // for updating the owned STFunc
  FLOATTYPE simTime = (FLOATTYPE)t;
  FLOATTYPE x[NDIM];
  for (size_t j=0; j<NDIM; ++j) { x[j] = 0.0; }

  // Loop on interactions... obtain charge density
  // per each electrostatic interaction
  for (size_t n=0; n<this->numUpdateFields; ++n) {

    this->dbprt("Poisson updater accessing field ", this->updateFields[n]->getName());
    this->dbprt("... will use fftObj");

    PsFieldBase<FLOATTYPE>& psiField = this->updateFields[n]->getConjgField();
    PsFieldBase<FLOATTYPE>& chargeField = this->updateFields[n]->getDensField();

    // ************************************************************
    // Find average charge density

    // SWS: bulk volume only
    size_t fieldSize = chargeField.getSize(); // SWS: volume
    FLOATTYPE localVol = FLOATTYPE(fieldSize);

    FLOATTYPE localC = chargeField.getSumAll();
    size_t nprocs = this->getCommBase().getSize();
    localC = localC / ( (FLOATTYPE)nprocs*localVol);
    FLOATTYPE globalChargeDensity =
      this->getCommBase().allReduceSum(localC);

    if ( std::fmod(FLOATTYPE(t),FLOATTYPE(100) ) == 0) {
      this->pprt("total charge density frac = ", globalChargeDensity);
    }
    // ************************************************************

    // ************************************************************
    // Find average charge density

    // SWS: bulk volume only
    fieldSize = chargeField.getSize();
    localVol = FLOATTYPE(fieldSize);

    localC = chargeField.getSumAll();
    FLOATTYPE chargeTotal = this->getCommBase().allReduceSum(localC);

    if ( std::fmod(FLOATTYPE(t),FLOATTYPE(100) ) == 0) {
      this->pprt("total charge = ", chargeTotal);
    }
    // ************************************************************

    // Shift charge
    FLOATTYPE totalC = -1.0*globalChargeDensity;
    chargeField += totalC;

    // Perform pseudo-spectral transform pair:  F^-1[k2*F[rho]]
    fftObjPtr->scaledFFTPair(chargeField.getDataPtr(), k2, resPtr);

    // SWS: this logic should be abstracted out
    FLOATTYPE* psiPtr = 0;
    psiPtr = psiField.getDataPtr();                   // Get the psField data
    size_t fftMemSize = fftSize * sizeof(FLOATTYPE);  // needed for memcpy

    FLOATTYPE psi0 = 0.0;
    FLOATTYPE psi1 = 0.0;
    FLOATTYPE fracChange = 0.0;
    for (size_t n=0; n<fftSize; ++n) {
      psi0 = psiPtr[n];
      psi1 = resPtr[n]*scaleFFT;
      fracChange = std::fabs((psi1-psi0)/psi0);

      if (fracChange > 0.2) {
// std::cout << "fracChange = " << fracChange << std::endl;
        resPtr[n] = psi0*1.2;
      }
    }

    memcpy(psiPtr, resPtr, fftMemSize);    // Copy result into psiField ptr
    psiField.scale(1.0*scaleFFT);          // Scale factor for FFT

  }

} // end update

// Instantiate classes
template class PsPoissonUpdater<float, 1>;
template class PsPoissonUpdater<float, 2>;
template class PsPoissonUpdater<float, 3>;

template class PsPoissonUpdater<double, 1>;
template class PsPoissonUpdater<double, 2>;
template class PsPoissonUpdater<double, 3>;
