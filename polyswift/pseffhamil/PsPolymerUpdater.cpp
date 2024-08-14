/**
 *
 * @file    PsPolymerUpdater.cpp
 *
 * @brief   Class for updating fields in effective Hamiltonian
 *
 * @version $Id: PsPolymerUpdater.cpp 8257 2007-09-12 22:05:20Z swsides $
 *
 * Copyright &copy; 2009-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsPolymerUpdater.h>

// txbase includes
#include <TxTensor.h>
#include <TxMakerMap.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsPolymerUpdater<FLOATTYPE, NDIM>::PsPolymerUpdater() {

  numInteractions = 0;
  numPolymers     = 0;
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsPolymerUpdater<FLOATTYPE, NDIM>::~PsPolymerUpdater() {

  // Delete local polymers vector,
  // no need to delete pointers as they arent owned.
  polymers.clear();

  // Delete local solvents vector,
  // no need to delete pointers as they arent owned.
  solvents.clear();

  // Clear local vector, pointers not owned
  interactionNames.clear();
  interactions.clear();
}

//
//
//
template <class FLOATTYPE, size_t NDIM>
void PsPolymerUpdater<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Scoping call to base class
  PsUpdater<FLOATTYPE, NDIM>::setAttrib(tas);

  // Names of interaction fields to point to
  if (tas.hasStrVec("interactions")) {
    interactionNames = tas.getStrVec("interactions");
    numInteractions = interactionNames.size();
  }
  else {
    TxDebugExcept tde("PsPolymerUpdater::setAttrib: no interactions found");
    tde << " for <Updater " << this->getName() << " >";
    throw tde;
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsPolymerUpdater<FLOATTYPE, NDIM>::buildData() {

  // Scoping call to base class
  PsUpdater<FLOATTYPE, NDIM>::buildData();

  this->dbprt("PsPolymerUpdater::buildData() ");

  // Return pointers to a PsFieldBase object
  feFieldPtr = TxMakerMap< PsFieldBase<FLOATTYPE> >::getNew(this->updateFieldType);
  if (!feFieldPtr) {
    TxDebugExcept tde("PsPolymerUpdater::buildData: no type set");
    tde << " in <Updater " << this->getName() << " >";
    throw tde;
  }

  // Return pointers to a PsFieldBase object
  feTotFieldPtr = TxMakerMap< PsFieldBase<FLOATTYPE> >::getNew(this->updateFieldType);
  if (!feTotFieldPtr) {
    TxDebugExcept tde("PsPolymerUpdater::buildData: no type set");
    tde << " in <Updater " << this->getName() << " >";
    throw tde;
  }

  // Return pointers to a PsFieldBase object
  feTotNoDisFieldPtr = TxMakerMap< PsFieldBase<FLOATTYPE> >::getNew(this->updateFieldType);
  if (!feTotFieldPtr) {
    TxDebugExcept tde("PsPolymerUpdater::buildData: no type set");
    tde << " in <Updater " << this->getName() << " >";
    throw tde;
  }

  // SWS: Set sizes... move this make safe....
  //  std::vector<size_t> local Dims =
  //    this->getGridBase().getDecomp().getNum CellsLocal();
  PsGridBaseItr* gItr = &this->getGridBase();
  feTotFieldPtr->setGrid(gItr);
  feFieldPtr->setGrid(gItr);
  feTotNoDisFieldPtr->setGrid(gItr);
}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsPolymerUpdater<FLOATTYPE, NDIM>::buildSolvers() {

  // Scoping call to base class
  PsConstraintUpdater<FLOATTYPE, NDIM>::buildSolvers();
  this->dbprt("PsPolymerUpdater::buildSolvers() ");

  // Push back pointers to polymer objects
  std::vector<std::string> polymerNames = PsPolymer<FLOATTYPE, NDIM>::getPolymerNames();
  numPolymers = polymerNames.size();

  for (size_t i=0; i<numPolymers; ++i) {

    PsPolymer<FLOATTYPE, NDIM>* polymerPtr =
      PsNamedObject::getObject<PsPolymer<FLOATTYPE, NDIM> >(polymerNames[i]);

    polymers.push_back(polymerPtr);
    this->dbprt("PsPolymerUpdater storing Polymer: ", polymers[i]->getName());
  } // numPolymers

  // Push back pointers to solvent objects
  std::vector<std::string> solventNames = PsSolvent<FLOATTYPE, NDIM>::getSolventNames();
  numSolvents = solventNames.size();

  for (size_t i=0; i<numSolvents; ++i) {

    PsSolvent<FLOATTYPE, NDIM>* solventPtr =
      PsNamedObject::getObject<PsSolvent<FLOATTYPE, NDIM> >(solventNames[i]);

    solvents.push_back(solventPtr);
    this->dbprt("PsPolymerUpdater storing Solvent: ", solvents[i]->getName());
  } // numSolvents

  // Push back pointers to interaction objects
  for (size_t i=0; i<numInteractions; ++i) {

    PsInteraction<FLOATTYPE, NDIM>* interactionPtr =
      PsNamedObject::getObject<PsInteraction<FLOATTYPE, NDIM> >(interactionNames[i]);

    this->dbprt("...storing interaction for updating ", interactionPtr->getName());
    this->dbprt("   for updater ", this->getName());
    interactions.push_back(interactionPtr);
  }

  // Status
  this->pprt("Note: mask field used in free-energy calc for product terms");
}

template <class FLOATTYPE, size_t NDIM>
void PsPolymerUpdater<FLOATTYPE, NDIM>::initialize() {

  // Scoping call to base class
  PsUpdater<FLOATTYPE, NDIM>::initialize();
}

//
// helper function to calculate free-energy
//
template <class FLOATTYPE, size_t NDIM>
void PsPolymerUpdater<FLOATTYPE, NDIM>::calcFeField() {

  this->dbprt("PsEffHamil::calcFeField() ");

  // Get field references
  PsFieldBase<FLOATTYPE>& feTot      = *feTotFieldPtr;
  PsFieldBase<FLOATTYPE>& feTotNoDis = *feTotNoDisFieldPtr;
  PsFieldBase<FLOATTYPE>& fe         = *feFieldPtr;
  FLOATTYPE disorderE = 0.0;

  // Reset total
  feTot.reset(0.0);
  feTotNoDis.reset(0.0);
  fe.reset(0.0);

  // Field product contribution
  calcUpdateFieldProd(fe);
  feTot += fe;
  feTotNoDis += fe;

  // Get contribs from interactions, Add to total freeE (feTot)
  // interactions->calcFe is where disorder contrib
  for (size_t n=0; n<this->numInteractions; ++n) {
    this->interactions[n]->calcFe(fe);
    feTot += fe;
    this->interactions[n]->calcFe(fe,false);
    feTotNoDis += fe;
  }

  // Free-energy contrib from single-chain partition functions
  FLOATTYPE sumFeTmp = 0.0;
  for (size_t i=0; i<numPolymers; ++i) {
    PsPolymer<FLOATTYPE, NDIM>* polymerPtr = polymers[i];
    FLOATTYPE logBigQ = (FLOATTYPE) polymerPtr->getLogBigQ();
    FLOATTYPE volfrac = polymerPtr->getVolfrac();
    FLOATTYPE lenRatio = polymerPtr->getLengthRatio();
    sumFeTmp = sumFeTmp - ((volfrac * logBigQ)/lenRatio);
  }

  // Free-energy contrib from single-chain partition functions
  for (size_t i=0; i<numSolvents; ++i) {
    PsSolvent<FLOATTYPE, NDIM>* solventPtr = solvents[i];
    FLOATTYPE logBigQ = solventPtr->getLogBigQ();
    FLOATTYPE volfrac = solventPtr->getVolfrac();
    FLOATTYPE lenRatio =
      1.0/((FLOATTYPE)PsPolymer<FLOATTYPE, NDIM>::getScaleLength());
    sumFeTmp = sumFeTmp - ((volfrac * logBigQ)/lenRatio);
  }

  // Add up all contributions
  // Track fe wo disorder correction
  sumFe      = sumFeTmp + feTot.getSumAll();
  sumFeNoDis = sumFeTmp + feTotNoDis.getSumAll();
}

template <class FLOATTYPE, size_t NDIM>
void PsPolymerUpdater<FLOATTYPE, NDIM>::update(double t) {

  // Scoping call to base class
  PsUpdater<FLOATTYPE, NDIM>::update(t);
  this->dbprt("PsPolymerUpdater::update() ");

  // If not updating this time step...exit
  if (!this->updateFlag) return;
}

//
// SWS: should possibly be moved to solution class
// SWS: ... eg derived PsSteepDUpdater
//
template <class FLOATTYPE, size_t NDIM>
void PsPolymerUpdater<FLOATTYPE, NDIM>::setConjgFieldsToPres() {

  // Get reference to pressure field
  PsFieldBase<FLOATTYPE>& presField = this->constraintFieldPtr->getConjgField();

  // Reset pressure values
  presField.reset(0.0);

  // Set to sum of w's
  for (size_t n=0; n<this->numUpdateFields; ++n) {
    PsFieldBase<FLOATTYPE>& wField = this->updateFields[n]->getConjgField();
    presField += wField;
  }

}

//
// protected section
//
template <class FLOATTYPE, size_t NDIM>
void PsPolymerUpdater<FLOATTYPE, NDIM>::addRandUpdateConjgFields(
        FLOATTYPE nstrength) {

  // Total constraint field
  PsFieldBase<FLOATTYPE>& wallField =
    this->constraintFieldPtr->getDensField();

  // Field workspace
  PsFieldBase<FLOATTYPE>& maskField  = *this->dFld;
  PsFieldBase<FLOATTYPE>& noiseField = *this->dFld2;

  // Construct mask field from constraint:  (1 - wallField)
  maskField.reset(0.0);
  maskField += wallField;
  maskField *= -1.0;
  maskField += 1.0;

  // SWS: turning off essentially. Remove??
  maskField.reset(1.0);

  for (size_t n=0; n<this->numUpdateFields; ++n) {

    PsFieldBase<FLOATTYPE>& wField = this->updateFields[n]->getConjgField();

    // Set noise values
    noiseField.reset(0.0);
    noiseField.addRandom(nstrength);
    noiseField *= maskField;
    wField += noiseField;
  }

}

//
// Free-energy contribution of field product sum (per cell)
// with volume (less the constraint)
//   [-phiA(r)*wA(r) + - phiB(r)*wB(r) +...... ]/vol_sim
//
template <class FLOATTYPE, size_t NDIM>
void PsPolymerUpdater<FLOATTYPE, NDIM>::calcUpdateFieldProd(
    PsFieldBase<FLOATTYPE>& dField) {

  // Get total volume (less constraint volume)
  size_t fieldSize = dField.getSize(); // SWS: volume
  FLOATTYPE localVol = FLOATTYPE(fieldSize)
    - this->constraintFieldPtr->calcLocalVolume();

  // Total constraint field   // SWS: not used??
  PsFieldBase<FLOATTYPE>& wallField =
    this->constraintFieldPtr->getDensField();

  // Product field workspace
  PsFieldBase<FLOATTYPE>& prodField = *(this->dFld);

  // Init fields
  prodField.reset(0.0);
  dField.reset(0.0);

  // Loop update fields get products, add to total
  for (size_t n=0; n<this->numUpdateFields; ++n) {
    this->updateFields[n]->calcFieldProd(prodField);
    prodField.scale(-1.0);
    dField += prodField;
  }

  // Construct mask field from constraint: dField*(1 - wallField)
  // This is commented out for now under the assumption that
  // the relaxation algorithm and grid resolution is such that
  // the monomer densities inside the constraint is ~ 0.0
  //
  PsFieldBase<FLOATTYPE>& maskField = *(this->dFld);
  maskField.reset(1.0);
  maskField -= wallField;
  dField *= maskField; // Apply mask

  // Volume scaling
  dField.scale(1.0/localVol);
}

// Instantiate classes
template class PsPolymerUpdater<float, 1>;
template class PsPolymerUpdater<float, 2>;
template class PsPolymerUpdater<float, 3>;

template class PsPolymerUpdater<double, 1>;
template class PsPolymerUpdater<double, 2>;
template class PsPolymerUpdater<double, 3>;

// std::cout << "bigQ free-energy = " << sumFe
//           << std::endl;
// std::cout << "free-energy from update fields ONLY = "
//           << feTot.getSumAll()
//           << std::endl;
// std::cout << "free-energy from update fields and interactions = "
//           << feTot.getSumAll()
//           << std::endl;

// ***************************************************************************
  // Adding up over all procs
  /*
  call mpi_allreduce(wphi ,wphi_tmp ,1,mpi_double_precision,
                     mpi_sum,mpi_comm_world,ierror)
  call mpi_allreduce(flory,flory_tmp,1,mpi_double_precision,
                     mpi_sum,mpi_comm_world,ierror)
  wphi  = wphi_tmp  / DBLE(nprocs)
  flory = flory_tmp / DBLE(nprocs)
  call mpi_allreduce(hwall,hwall_tmp,1,mpi_double_precision,
                     mpi_sum,mpi_comm_world,ierror)
  hwall = hwall_tmp / DBLE(nprocs)
  */
// ***************************************************************************
