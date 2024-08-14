/**
 *
 * @file    PsSteepDUpdater.cpp
 *
 * @brief   Class for updating fields with steepest descent algorithm
 *
 * @version $Id: PsSteepDUpdater.cpp 8257 2007-09-12 22:05:20Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// pseffhamil includes
#include <PsSteepDUpdater.h>

// txbase includes
#include <TxTensor.h>
#include <TxMakerMap.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsSteepDUpdater<FLOATTYPE, NDIM>::PsSteepDUpdater() {
  //  numInteractions = 0;
  noiseStrength = 0.0000;
  numCInteractions = 0;
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsSteepDUpdater<FLOATTYPE, NDIM>::~PsSteepDUpdater() {

  // Delete dHTotals
  for (size_t i=0; i<dHTotals.size(); ++i) {
    delete dHTotals[i];
  }
  dHTotals.clear();

  relaxlambdas.clear();
  cInteractionNames.clear();
  constraintInteractions.clear();
}

template <class FLOATTYPE, size_t NDIM>
void PsSteepDUpdater<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Scoping call to base class
  PsPolymerUpdater<FLOATTYPE, NDIM>::setAttrib(tas);

  this->dbprt("PsSteepDUpdater::setAttrib() ");

  // Relaxation "timesteps"
  if (tas.hasPrmVec("relaxlambdas")) {
    std::vector<double> rlam;
    rlam = tas.getPrmVec("relaxlambdas");
    for (size_t n=0; n<rlam.size(); ++n) {
      relaxlambdas.push_back((FLOATTYPE)rlam[n]);
    }
    rlam.clear();
  }

  // Names of constraint interaction fields to point to
  if (tas.hasStrVec("constraints")) {
    cInteractionNames = tas.getStrVec("constraints");
    numCInteractions = cInteractionNames.size();
  }

  // Noise strength
  if (tas.hasParam("noise")) {
    double tmp = tas.getParam("noise");
    noiseStrength = (FLOATTYPE) tmp;
  }
}

template <class FLOATTYPE, size_t NDIM>
void PsSteepDUpdater<FLOATTYPE, NDIM>::buildData() {

  // Scoping call to base class
  PsPolymerUpdater<FLOATTYPE, NDIM>::buildData();
  this->dbprt("PsSteepDUpdater::buildData() ");

  // Check for more than 3 fields
  if (this->numUpdateFields > 3 ) {
    TxDebugExcept tde("PsSteepDUpdater::buildData");
    tde << "numUpdateFields > 3 not implemented yet";
    throw tde;
  }

  // Push back pointers to interaction objects
  for (size_t i=0; i<numCInteractions; ++i) {

    PsInteraction<FLOATTYPE, NDIM>* interactionPtr =
      PsNamedObject::getObject<PsInteraction<FLOATTYPE, NDIM> >(cInteractionNames[i]);

    this->dbprt("...storing constraint interactions", interactionPtr->getName());
    this->dbprt("   for updater ", this->getName());
    constraintInteractions.push_back(interactionPtr);
  }

  // Check for more than 2 fields if interacting constraints present
  if ( (numCInteractions > 0) && (this->numUpdateFields >= 3) ) {
    TxDebugExcept tde("PsSteepDUpdater::buildData");
    tde << "More than 2-component updates not yet implemeneted ";
    tde << "for interacting nanoparticles/walls";
    throw tde;
  }

  //
  // Functional derivative fields built here to enforce the
  // ordering relationship between updateFields <--> dHTotals
  //
  for (size_t i=0; i<this->numUpdateFields; ++i) {
    PsFieldBase<FLOATTYPE>* dFieldPtr =
      TxMakerMap< PsFieldBase<FLOATTYPE> >::getNew(this->updateFieldType);
    dHTotals.push_back(dFieldPtr);
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsSteepDUpdater<FLOATTYPE, NDIM>::buildSolvers() {

  // Scoping call to base class
  PsPolymerUpdater<FLOATTYPE, NDIM>::buildSolvers();
  this->dbprt("PsSteepDUpdater::buildSolvers() ");

  // Setting dHFields sizes/values
  PsGridBaseItr* gItr = &this->getGridBase();
  for (size_t n=0; n<this->numUpdateFields; ++n) {
    dHTotals[n]->setGrid(gItr);
  }

  // Return pointers to a PsFieldBase object
  resField = TxMakerMap< PsFieldBase<FLOATTYPE> >::getNew(this->updateFieldType);
  resField->setGrid(gItr);
}

//
// Initialize before update step
// SWS: reorder this so initialize is only called at startup.....
//
template <class FLOATTYPE, size_t NDIM>
void PsSteepDUpdater<FLOATTYPE, NDIM>::initialize() {

  // Scoping call to base class
  PsPolymerUpdater<FLOATTYPE, NDIM>::initialize();
  this->dbprt("PsSteepDUpdater::initialize() ");

  // Reset total fields and holder field
  for (size_t n=0; n<this->numUpdateFields; ++n)
    dHTotals[n]->reset(0.0);
}

//
template <class FLOATTYPE, size_t NDIM>
void PsSteepDUpdater<FLOATTYPE, NDIM>::update(double t) {

  // Scoping call to base class
  PsPolymerUpdater<FLOATTYPE, NDIM>::update(t);
  this->dbprt("PsSteepDUpdater::update() ");

  // If not updating this time step...exit
  if (!this->updateFlag) return;

  // **************************************************************************
  // Get reference to pressure field
  PsFieldBase<FLOATTYPE>& presField = this->constraintFieldPtr->getConjgField();

  // Loop over update fields and initialize with chemical
  // potential fields and pressure [ie dHTotals_n = -w()_n + p() ]
  for (size_t n=0; n<this->numUpdateFields; ++n) {

    PsFieldBase<FLOATTYPE>& dH0 = *dHTotals[n];
    PsFieldBase<FLOATTYPE>& wField = this->updateFields[n]->getConjgField();

    dH0.reset(0.0);
    dH0 += presField;
    dH0 -= wField;
    //    dH0 += (presField - wField);
    //    dH0 -= wField;
  }
  // **************************************************************************

  // Calc tot contrib to dHTotals for all updateFields
  // Loops over over interactions for *each* update field
  update_dHTotals();

  // Replace new values in update fields
  if (this->numUpdateFields == 2) update_set2Fields();
  if (this->numUpdateFields == 3) update_set3Fields();

  // Update constraint physical field class specifics
  // dynamic cast to specific interface
  // this->constraintFieldPtr->update(t);
  this->constraintFieldPtr->updatePres();

  // SWS: here to match regression, need to sort order
  this->calcFeField();

} // end update

//
//
template <class FLOATTYPE, size_t NDIM>
void PsSteepDUpdater<FLOATTYPE, NDIM>::update_dHTotals() {

  this->dbprt("PsSteepDUpdater::update_dHTotals() ");

  std::string upFieldStr;

  // Loop on all updateFields
  for (size_t n=0; n<this->numUpdateFields; ++n) {

    // Get reference to dHTotal for updateField
    // dHTotals[n] should already be initialized with -w[n] + pres
    PsFieldBase<FLOATTYPE>& dHTot = *dHTotals[n];
    PsFieldBase<FLOATTYPE>& dH = *(resField);

    upFieldStr = this->updateFields[n]->getName();
    this->dbprt("...updating dHTotals for ", upFieldStr);
    this->dbprt("... with numInteractions = ",(int)this->numInteractions);

    // Loop on interactions for each updateField[n]
    for (size_t i=0; i<this->numInteractions; ++i) {
      if ( this->interactions[i]->hasScField(upFieldStr) ) {
        this->interactions[i]->calcDfD(upFieldStr, dH);
        dHTot += dH;
      }
    } // loop interactions

  } // loop on update fields

}

//
// helper method for update()
//   Update 2-component updateFields
//   Add noise to all update fields and
//   update pressure with sum of conjugate fields
//
// pres = 0.5*( w(1,:,:,:)+w(2,:,:,:)          + &
//        ( phi0*(      chiN(1,2)           )) + &
//        ( phi0_wall*( chiwN(1) + chiwN(2) )) + &
//        ( phi0_ptcl*( chipN(1) + chipN(2) )) )
//
template <class FLOATTYPE, size_t NDIM>
void PsSteepDUpdater<FLOATTYPE, NDIM>::update_set2Fields() {

  // Workspace
  PsFieldBase<FLOATTYPE>& xField = *(resField);

  // Explicitly list update/constraint fields
  PsFieldBase<FLOATTYPE>& updateField0 = this->updateFields[0]->getConjgField();
  PsFieldBase<FLOATTYPE>& updateField1 = this->updateFields[1]->getConjgField();
  PsFieldBase<FLOATTYPE>& presField = this->constraintFieldPtr->getConjgField();
  PsFieldBase<FLOATTYPE>& phi0Field = this->constraintFieldPtr->getDensField();

  PsFieldBase<FLOATTYPE>& dHTotal0 = *dHTotals[0];
  PsFieldBase<FLOATTYPE>& dHTotal1 = *dHTotals[1];

  // Hardwired updateField[0]
  xField.reset(0.0);
  xField += dHTotal1;
  xField *= -1.0*relaxlambdas[1];
  xField += dHTotal0;
  xField *= relaxlambdas[0];
  updateField0 += xField;

  // Hardwired updateField[1]
  xField.reset(0.0);
  xField += dHTotal0;
  xField *= -1.0*relaxlambdas[1];
  xField += dHTotal1;
  xField *= relaxlambdas[0];
  updateField1 += xField;

  //
  // Hardwired pressure update for 2-components
  //
  this->addRandUpdateConjgFields(noiseStrength);
  this->setConjgFieldsToPres();

  // Contributions from 'mixed' terms from polymer interactions
  //  p(r) = phi0*(chiN_12)
  for (size_t i=0; i<this->numInteractions; ++i) {

    std::string upFieldStr1 = this->updateFields[0]->getName();
    std::string upFieldStr2 = this->updateFields[1]->getName();
    bool hasField1 = this->interactions[i]->hasScField(upFieldStr1);
    bool hasField2 = this->interactions[i]->hasScField(upFieldStr2);

    // This logic limits terms to polymer chi's
    if ( hasField1 && hasField2 ) {
      PsFieldBase<FLOATTYPE>& chiN = this->interactions[i]->getParam();
      xField.reset(0.0);
      xField += phi0Field;
      xField *= chiN;
      presField += xField;
      //      presField += phi0Field * chiN;
    }
  } // loop interactions

  // Constraint interaction pressure contribution
  // SWS: sign correction for constraint chi-s added 02/01/2013
  //  p(r) = ( p0_wall*( chiwN(1) + chiwN(2) )) +
  //         ( p0_ptcl*( chipN(1) + chipN(2) )) )
  for (size_t i=0; i<numCInteractions; ++i) {
    for (size_t n=0; n<this->numUpdateFields; ++n) {

      std::string upFieldStr = this->updateFields[n]->getName();
      bool hasField = constraintInteractions[i]->hasScField(upFieldStr);

      // This logic picks out wall physical fields
      if (hasField ) {
        PsFieldBase<FLOATTYPE>& chiN = constraintInteractions[i]->getParam();
        PsPhysField<FLOATTYPE, NDIM>* physPtr =
            constraintInteractions[i]->getOtherPhysField(upFieldStr);
        PsFieldBase<FLOATTYPE>& wallField = physPtr->getDensField();
        xField.reset(0.0);
        xField += wallField;
        xField *= chiN;
        presField -= xField;
      }

    }
  }

  // Scaling w/#of components
  presField.scale(0.5);
}

//
// helper method for update()
//   Update 3-component updateFields
//   Add noise to all update fields and
//   update pressure with sum of conjugate fields
//
//  pres = (1./3.)*( w(1,:,:,:)+w(2,:,:,:)+w(3,:,:,:)      +      &
//     chiN(1,2)*(phi_tot(3,:,:,:)-phi_avg(3))        +      &
//     chiN(1,3)*(phi_tot(2,:,:,:)-phi_avg(2))        +      &
//     chiN(2,3)*(phi_tot(1,:,:,:)-phi_avg(1))        +      &
//     (     phi0*( chiN(1,2) + chiN(1,3) + chiN(2,3) ) ) +  &
//     ( dn0*phi0_wall  *( chiw(1)   + chiw(2)   + chiw(3) ) ) + &
//     ( dn0*phi0_multip*( chip(1)   + chip(2)   + chip(3) ) ) )
//
template <class FLOATTYPE, size_t NDIM>
void PsSteepDUpdater<FLOATTYPE, NDIM>::update_set3Fields() {

  PsFieldBase<FLOATTYPE>& xField = *(resField);

  // Explicitly list update/constraint fields
  PsFieldBase<FLOATTYPE>& dHTotal0     = *dHTotals[0];

  //  Ps Field<FLOATTYPE, NDIM> dHTotalf(dHTotal0);

  PsFieldBase<FLOATTYPE>& dHTotal1     = *dHTotals[1];
  PsFieldBase<FLOATTYPE>& dHTotal2     = *dHTotals[2];
  PsFieldBase<FLOATTYPE>& updateField0 = this->updateFields[0]->getConjgField();
  PsFieldBase<FLOATTYPE>& updateField1 = this->updateFields[1]->getConjgField();
  PsFieldBase<FLOATTYPE>& updateField2 = this->updateFields[2]->getConjgField();

  PsFieldBase<FLOATTYPE>& presField = this->constraintFieldPtr->getConjgField();
  PsFieldBase<FLOATTYPE>& phi0Field = this->constraintFieldPtr->getDensField();

  // Hardwired updateField[0]
  xField.reset(0.0);
  xField += dHTotal1;
  xField += dHTotal2;
  xField *= -1.0*relaxlambdas[1];
  xField += dHTotal0;
  xField *= relaxlambdas[0];
  updateField0 += xField;

  // Hardwired updateField[1]
  xField.reset(0.0);
  xField += dHTotal0;
  xField += dHTotal2;
  xField *= -1.0*relaxlambdas[1];
  xField += dHTotal1;
  xField *= relaxlambdas[0];
  updateField1 += xField;

  // Hardwired updateField[2]
  xField.reset(0.0);
  xField += dHTotal0;
  xField += dHTotal1;
  xField *= -1.0*relaxlambdas[1];
  xField += dHTotal2;
  xField *= relaxlambdas[0];
  updateField2 += xField;

  //
  // Hardwired pressure update for 3-components
  //
  this->addRandUpdateConjgFields(noiseStrength);
  this->setConjgFieldsToPres();

  // Contributions from 'mixed' terms from polymer interactions
  //  p(r) = chiN_12*(phi3-phi3ave) + chiN_13*(phi2-phi2ave) + chiN_23*(phi1-phi1ave)
  //  p(r) = phi0*( chiN_12 + chiN_13 + chiN_23 )
  for (size_t i=0; i<this->numInteractions; ++i) {
    for (size_t n=0; n<this->numUpdateFields; ++n) {

      // This logic limits terms to polymer chi's
      std::string upFieldStr = this->updateFields[n]->getName();
      bool hasField = this->interactions[i]->hasScField(upFieldStr);
      PsFieldBase<FLOATTYPE>& chiN = this->interactions[i]->getParam();

      if ( !hasField ) {
        PsFieldBase<FLOATTYPE>& densField =
            this->updateFields[n]->getShiftedDensField();
        xField.reset(0.0);
        xField += densField;
        xField += phi0Field;
        xField *= chiN;
        presField += xField;
      }

    } // loop field check
  } // loop interactions

  // Constraint interaction pressure contribution
  // SWS: sign correction added 02/01/2013
  //  p(r) = ( p0_wall*( chiwN(1) + chiwN(2) + chiwN(3) )) +
  //         ( p0_ptcl*( chipN(1) + chipN(2) + chiwN(3) )) )
  for (size_t i=0; i<numCInteractions; ++i) {
    for (size_t n=0; n<this->numUpdateFields; ++n) {

      std::string upFieldStr = this->updateFields[n]->getName();
      bool hasField = constraintInteractions[i]->hasScField(upFieldStr);

      // This logic picks out wall physical fields
      if (hasField ) {
        PsFieldBase<FLOATTYPE>& chiN =
            constraintInteractions[i]->getParam();
        PsPhysField<FLOATTYPE, NDIM>* physPtr =
            constraintInteractions[i]->getOtherPhysField(upFieldStr);
        PsFieldBase<FLOATTYPE>& wallField = physPtr->getDensField();
        xField.reset(0.0);
        xField += wallField;
        xField *= chiN;
        presField -= xField;
      }
    }
  }

  // Pressure scale factor for number of components
  FLOATTYPE invNumFields = 1.0/3.0;
  presField.scale(invNumFields);
}

// Instantiate classes
template class PsSteepDUpdater<float, 1>;
template class PsSteepDUpdater<float, 2>;
template class PsSteepDUpdater<float, 3>;

template class PsSteepDUpdater<double, 1>;
template class PsSteepDUpdater<double, 2>;
template class PsSteepDUpdater<double, 3>;
