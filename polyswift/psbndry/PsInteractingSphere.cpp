/**
 * @file    PsInteractingSphere.cpp
 *
 * @brief   Class for purely translate-able nanoparticles with interactions
 *
 * @version $Id: PsInteractingSphere.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2009-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psptcl includes
#include <PsInteractingSphere.h>

// psstd includes
#include <PsRandom.h>

// txbase includes
#include <TxMakerMap.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsInteractingSphere<FLOATTYPE, NDIM>::PsInteractingSphere() {

  // Default move period
  updateMovePeriod = 500;

  // Initialize pointers etc.
  forceField = 0;
  resPtr = 0;

  forceField = NULL;
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsInteractingSphere<FLOATTYPE, NDIM>::~PsInteractingSphere() {

  for (size_t n=0; n<forceFieldVec.size(); ++n) {
    delete forceFieldVec[n];
  }
  forceFieldVec.clear();

  // FFT convolution data space
  delete[] resPtr;

  delete forceField;
}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsInteractingSphere<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Scoping call to base class
  PsSphere<FLOATTYPE, NDIM>::setAttrib(tas);

  // Number of timesteps between moves
  if (tas.hasOption("updateMovePeriod")) {
    updateMovePeriod = tas.getOption("updateMovePeriod");
   }
  else {
    TxDebugExcept tde("PsSphere::setAttrib: updateMovePeriod not set");
    tde << " in <PsNanoPtcl " << this->getName() << " >";
    throw tde;
  }
}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsInteractingSphere<FLOATTYPE, NDIM>::buildData() {

  // Scoping call to base class
  PsSphere<FLOATTYPE, NDIM>::buildData();
}

//
//
//
template <class FLOATTYPE, size_t NDIM>
void PsInteractingSphere<FLOATTYPE, NDIM>::buildSolvers() {

  // Scoping call to base class...
  PsSphere<FLOATTYPE, NDIM>::buildSolvers();

  this->dbprt("PsInteractingSphere::buildSolvers() ");

  // Field type for force fields
  // SWS: this breaks build cycle because physFldPtr only set in buildSolvers
  std::string fieldType = this->bndryPhysFldPtr->getFieldType();

  // Build temp space
  forceField = TxMakerMap< PsFieldBase<FLOATTYPE> >::getNew(fieldType);
  PsGridBaseItr* gItr = &this->getGridBase();
  forceField->setGrid(gItr);

  // Build force field object based on physFields and
  // to initialize forceFieldVec
  for (size_t ic=0; ic<NDIM; ++ic) {
    PsFieldBase<FLOATTYPE>* ffPtr = TxMakerMap< PsFieldBase<FLOATTYPE> >::getNew(fieldType);
    ffPtr->setGrid(gItr);
    forceFieldVec.push_back(ffPtr);
  }

  // Allocate result pointer
  resPtr = new FLOATTYPE[this->fftSize];
}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsInteractingSphere<FLOATTYPE, NDIM>::initialize() {

  // Scoping call to base class
  PsSphere<FLOATTYPE, NDIM>::initialize();
}

//
// Calculate forces due to interacting terms
// Set values in forceFieldVec data member
//
template <class FLOATTYPE, size_t NDIM>
void PsInteractingSphere<FLOATTYPE, NDIM>::calculateForces() {

  this->dbprt("PsInteractingSphere::calculateForces " );

  // Reset forces
  for (size_t ic=0; ic<NDIM; ++ic) {
    forceFieldVec[ic]->reset(0.0);
  }
  forceField->reset(0.0);

  // Volume factor (corrected for total constraint volume)
  FLOATTYPE localVol = FLOATTYPE(this->fftSize) -
    this->constraintPhysFldPtr->calcLocalVolume();

  // SWS: these could be separated into helpers....

  // Pressure force contribution...
  FLOATTYPE* presPtr =
    this->constraintPhysFldPtr->getConjgField().getDataPtr();

  //
  // Loop on spatial components
  //
  for (size_t ic=0; ic<NDIM; ++ic) {

    // Find convolutaion integral for ic component of the
    // particle gradient function
    FLOATTYPE* gradPtr = this->gradFieldVec[ic].getDataPtr();
    this->fftObjPtr->convolveRe(presPtr, gradPtr, resPtr);

    // Assign to force-field component
    FLOATTYPE* forcePtr = forceFieldVec[ic]->getDataPtr();
    memcpy(forcePtr, resPtr, this->fftMemSize);
    forceFieldVec[ic]->scale(-1.0*this->scaleFFT/localVol);
  }

  // Find interactions names for which the physical field
  // associated with this nanoparticle type takes part.
  std::vector<std::string> interactNames = this->bndryPhysFldPtr->getInteractionNames();

  for (size_t n=0; n<interactNames.size(); ++n) {

    this->dbprt("Nanoparticle type: ", this->getName());
    this->dbprt("   accessing interaction ", interactNames[n]);

    // Get interaction that name is registered to... then get the
    // "other" field that the particle field interacts with and its data
    PsInteraction<FLOATTYPE, NDIM>* interPtr =
      PsNamedObject::getObject<PsInteraction<FLOATTYPE, NDIM> >(interactNames[n]);
    if (!interPtr) {
      TxDebugExcept tde("PsInteractionSphere::calculateForces: interPtr not set");
      tde << " in <NanoPtcl " << this->getName() << " >";
      throw tde;
    }

    PsPhysField<FLOATTYPE, NDIM>* otherPhysField =
      interPtr->getOtherPhysField(this->bndryFieldName);
    if (!otherPhysField) {
      TxDebugExcept tde("PsInteractionSphere::calculateForces: otherPhysField not set");
      tde << " in <NanoPtcl " << this->getName() << " >";
      throw tde;
    }

    FLOATTYPE* physPtr = otherPhysField->getDensField().getDataPtr();

    // Get chiN parameter for this interaction
    // FLOATTYPE chiN = interPtr->getParam();
    PsFieldBase<FLOATTYPE>& chiN = interPtr->getParam();

    this->dbprt("accessing 'other' physical field ", otherPhysField->getName());
    this->dbprt("  for particle field ", this->bndryFieldName);

    // Loop on spatial components
    for (size_t ic=0; ic<NDIM; ++ic) {

      // Reset for safety
      forceField->reset(0.0);

      // Convolution integral for forces
      FLOATTYPE* gradPtr = this->gradFieldVec[ic].getDataPtr();
      this->fftObjPtr->convolveRe(physPtr, gradPtr, resPtr);

      // Assign to force-field component
      FLOATTYPE* forcePtr = forceField->getDataPtr();
      memcpy(forcePtr, resPtr, this->fftMemSize);

      // Scale factor for convolution, and volume factor from force derivation
      // SWS: localVol should be global volume
      //      forceField->scale( (-1.0*chiN*this->scaleFFT) / localVol);
      forceField->scale( (-1.0*this->scaleFFT) / localVol);
      // This should be moved inside convolution integral???
      PsFieldBase<FLOATTYPE>& fFld = *forceField;
      fFld *= chiN;

      // Add to total force
      PsFieldBase<FLOATTYPE>& ffVecRef = *(forceFieldVec[ic]);
      PsFieldBase<FLOATTYPE>& ffRef    = *forceField;
      ffVecRef += ffRef;

    } //loop on comp

  } // loop on interactions
}

//
// Update steps include:
//
//    1. calculate forces on all pure-translatable particles
//    2. make trial translations based on forces
//    3. and makes movePtcl calls
//
template <class FLOATTYPE, size_t NDIM>
void PsInteractingSphere<FLOATTYPE, NDIM>::update(double t) {

  this->dbprt("PsInteractingSphere::update(t)" );

  // Get simulation timestep and present domain step
  size_t timeStep = PsDynObjBase::getCurrDomainStep();
  double tpresent = (double)timeStep;

  // Update present timestep --> "next" timestep:
  if (t > tpresent) {

    // Only update for specified interval
    if (timeStep % this->updateMovePeriod == 0) {

      this->dbprt("particle update at timestep = ", (int)timeStep);

      calculateForces();                   // Find forces,
      this->calculateMoves(forceFieldVec); // Find dr's from forces
      this->moveCheckAllPtcls();           // Move all particles, check overlaps
    }

    // SWS: checking for safety
    this->getCommBase().barrier();
  }

  // Scoping call to base class
  PsSphere<FLOATTYPE, NDIM>::update(t);
}

// Instantiate classes
template class PsInteractingSphere<float, 1>;
template class PsInteractingSphere<float, 2>;
template class PsInteractingSphere<float, 3>;

template class PsInteractingSphere<double, 1>;
template class PsInteractingSphere<double, 2>;
template class PsInteractingSphere<double, 3>;
