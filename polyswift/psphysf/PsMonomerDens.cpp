/**
 * @file    PsMonomerDens.cpp
 *
 * @brief   Class that holds a monomer density field
 *
 * @version $Id: PsMonomerDens.cpp 8401 2007-09-26 20:17:05Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psphysf includes
#include <PsMonomerDens.h>

#include <PsGridField.h>

// txbase includes
#include <TxMakerMap.h>

template <class FLOATTYPE, size_t NDIM>
PsMonomerDens<FLOATTYPE, NDIM>::PsMonomerDens() {

  monoDensDataName  = "MonomerDensity";
  wFieldDataName  = "ConjugateDensity";
  this->densFieldName = monoDensDataName;
  this->conjFieldName = wFieldDataName;

  monoDensAverage = 0.0;
  initOption = "random";

  monoDensFieldPtr = NULL;
  chemWFieldPtr = NULL;
}

template <class FLOATTYPE, size_t NDIM>
PsMonomerDens<FLOATTYPE, NDIM>::~PsMonomerDens() {

  // Clear field pointers
  delete monoDensFieldPtr;
  delete chemWFieldPtr;
}

//
// set attributes
//
template <class FLOATTYPE, size_t NDIM>
void PsMonomerDens<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Using base class to set attributes for base physical field class
  PsPhysFldFuncs<FLOATTYPE, NDIM>::setAttrib(tas);
  this->dbprt("PsMonomerDens::setAttrib() ");

  // Flag for dumping field
  if (tas.hasParam("ifreqSize")) {
    ifreqSize = tas.getParam("ifreqSize");
  }

  // String flag for setting initial chem field
  if (tas.hasString("initOption")) {
    initOption = tas.getString("initOption");
  }

}

//
// build Data
//
template <class FLOATTYPE, size_t NDIM>
void PsMonomerDens<FLOATTYPE, NDIM>::buildData() {

  // Build base class data and notify
  PsPhysFldFuncs<FLOATTYPE, NDIM>::buildData();
  this->dbprt("PsMonomerDens::buildData() ");

  // Return pointers to a PsFieldBase objects
  monoDensFieldPtr = TxMakerMap< PsFieldBase<FLOATTYPE> >::getNew(this->fieldType);
  chemWFieldPtr    = TxMakerMap< PsFieldBase<FLOATTYPE> >::getNew(this->fieldType);
  dField           = TxMakerMap< PsFieldBase<FLOATTYPE> >::getNew(this->fieldType);

  monoDensFieldPtr->setFieldType(this->fieldType);
  chemWFieldPtr->setFieldType(this->fieldType);
  dField->setFieldType(this->fieldType);

  // Set field sizes
  PsGridBaseItr* gItr = &this->getGridBase();
  monoDensFieldPtr->setGrid(gItr);
  chemWFieldPtr->setGrid(gItr);
  dField->setGrid(gItr);
}

//
// next part of build cycle
//
template <class FLOATTYPE, size_t NDIM>
void PsMonomerDens<FLOATTYPE, NDIM>::buildSolvers() {

  // Build base class data and notify
  PsPhysFldFuncs<FLOATTYPE, NDIM>::buildSolvers();
  this->dbprt("PsMonomerDens::buildSolvers() ");
}

//
// Reset density fields... to be recalculated at update step
//
template <class FLOATTYPE, size_t NDIM>
void PsMonomerDens<FLOATTYPE, NDIM>::resetDensField() {

  this->dbprt("PsMonomerDens::resetDensField() ");
  monoDensFieldPtr->reset(0.0);
}

//
// Initialize conjugate density fields... on first start-up
//
template <class FLOATTYPE, size_t NDIM>
void PsMonomerDens<FLOATTYPE, NDIM>::initialize() {

  this->dbprt("PsMonomerDens::initialize() ");

  // Random initial values
  FLOATTYPE scale = 1.0;
  FLOATTYPE shift = -0.5;

  if (initOption == "random") {
    chemWFieldPtr->resetRandom(scale, shift);
    this->pprt("PsMonomerDens::initialize random option for ", this->getName());
  }
  else if (initOption == "centerSlabPos") {
    testSlab(1.0);
    this->pprt("PsMonomerDens::initialize center (+)slab option ", this->getName());
  }
  else if (initOption == "centerSlabNeg") {
    testSlab(-1.0);
    this->pprt("PsMonomerDens::initialize center (-)slab option ", this->getName());
  }

  else if (initOption == "sineXInitPos") {
    sineSlab(1.0, 4.0);
    this->pprt("PsMonomerDens::initialize 0<Nx/2 with sin(x) ", this->getName());
  }
  else if (initOption == "sineXInitNeg") {
    sineSlab(-1.0, 4.0);
    this->pprt("PsMonomerDens::initialize 0<Nx/2 with sin(x) ", this->getName());
  }

  else if (initOption == "sineXInitPosOld") {
    sineSlab(1.0, 4.0);
    this->pprt("PsMonomerDens::initialize 0<Nx/2 with sin(x) ", this->getName());
  }
  else if (initOption == "sineXInitNegOld") {
    sineSlab(-1.0, 4.0);
    this->pprt("PsMonomerDens::initialize 0<Nx/2 with sin(x) ", this->getName());
  }
  else {
    TxDebugExcept tde("PsMonomerDens::initialize: initOption not recognized");
    tde << "initOption = " << initOption;
    tde << " in <PsPhysField " << this->getName() << " >";
    throw tde;
  }

}

//
// Update member data associated w/this physical field
//
template <class FLOATTYPE, size_t NDIM>
void PsMonomerDens<FLOATTYPE, NDIM>::update(double t) {

  resetDensField();   // Reset counters for densities
  updateMonoAvg();    // Update monomer averages
}

//
template <class FLOATTYPE, size_t NDIM>
PsFieldBase<FLOATTYPE>& PsMonomerDens<FLOATTYPE, NDIM>::getShiftedDensField() {

  // Workspace
  PsFieldBase<FLOATTYPE>& xField = *(dField);
  xField.reset(0.0);

  // Bare density field
  PsFieldBase<FLOATTYPE>& densField = *monoDensFieldPtr;
  xField += densField;
  xField -= monoDensAverage;

  return xField;
}

//
// Private helper method:
// Update member data associated w/this physical field
//
template <class FLOATTYPE, size_t NDIM>
void PsMonomerDens<FLOATTYPE, NDIM>::updateMonoAvg() {

  std::vector<std::string> blockNames   = this->blockMembers;
  std::vector<std::string> solventNames = this->solventMembers;
  PsBlockBase<FLOATTYPE, NDIM>* blockObjPtr;
  PsPolymer<FLOATTYPE, NDIM>*   polymerObjPtr;
  PsSolvent<FLOATTYPE, NDIM>*   solventObjPtr;
  FLOATTYPE totMonoAvg = 0.0;

  // Find solvent contributions
  for (size_t n=0; n<solventNames.size(); ++n) {
    solventObjPtr = PsNamedObject::getObject<PsSolvent<FLOATTYPE, NDIM> >(solventNames[n]);
    FLOATTYPE volfrac = solventObjPtr->getVolfrac();
    totMonoAvg += volfrac;
  }

  // Find polymer block contributions
  for (size_t n=0; n<blockNames.size(); ++n) {
    blockObjPtr = PsNamedObject::getObject<PsBlockBase<FLOATTYPE, NDIM> >(blockNames[n]);
    std::string polyName = blockObjPtr->getPolymerName();
    polymerObjPtr = PsNamedObject::getObject<PsPolymer<FLOATTYPE, NDIM> >(polyName);
    FLOATTYPE volfrac = polymerObjPtr->getVolfrac();
    FLOATTYPE lenfrac = blockObjPtr->getLengthFrac();
    FLOATTYPE lenWt   = blockObjPtr->getLengthWeight();
    // FLOATTYPE monoAvg = lenfrac*volfrac;
    FLOATTYPE monoAvg = lenWt*lenfrac*volfrac;
    totMonoAvg += monoAvg;
  }

  // Set local monomer density average
  monoDensAverage = totMonoAvg;
}

//
// Restore fields... on restart
// leaving this method in case special restore functions arise
//
template <class FLOATTYPE, size_t NDIM>
void PsMonomerDens<FLOATTYPE, NDIM>::restore(TxIoBase* txIoPtr) {

  this->dbprt("PsMonomerDens::restore() ");

  // Scoping call to base class
  PsPhysFldFuncs<FLOATTYPE, NDIM>::restore(txIoPtr);
}

//
// Initialize the field values
//
template <class FLOATTYPE, size_t NDIM>
void PsMonomerDens<FLOATTYPE, NDIM>::sineSlab(int fsign, FLOATTYPE omega) {

  // Global center location, used to shift cavity
  PsTinyVector<int, NDIM> center(this->getGridBase().getCenterGlobal());

  // Global dimensions
  std::vector<size_t> globalDims = this->getGridBase().getNumCellsGlobal();

  PsGridField<FLOATTYPE, NDIM> initLocalField;
  PsGridBaseItr* gItr = &this->getGridBase();
  initLocalField.setGrid(gItr);

  FLOATTYPE val;
  FLOATTYPE pii = 3.1415;

  // Load cavity values
  for (size_t x=0; x<globalDims[0]; ++x) {
    for (size_t y=0; y<globalDims[1]; ++y) {
      for (size_t z=0; z<globalDims[2]; ++z) {

        // Setup tiny vector
        PsTinyVector<int, NDIM> posVec(x,y,z);

        FLOATTYPE sinArg =
          omega*2.0*pii*( (FLOATTYPE)x/(FLOATTYPE)globalDims[0] );
        FLOATTYPE wVal = 4.0*std::sin(sinArg);
        if (x > globalDims[0]/2) wVal = 0.0;
        val = FLOATTYPE(fsign)*wVal;

        initLocalField.mapToLocalField(posVec, val, "add");

      }}}
  // ***************************************************************

  // Assign charge field values
  PsFieldBase<FLOATTYPE>& chemField = *chemWFieldPtr;
  chemWFieldPtr->reset(0.0);

  PsFieldBase<FLOATTYPE>& initField = *(initLocalField.getBasePtr());
  chemField += initField;
}

//
// Initialize the field values
//
template <class FLOATTYPE, size_t NDIM>
void PsMonomerDens<FLOATTYPE, NDIM>::testSlab(int fsign) {

  // Global center location, used to shift cavity
  PsTinyVector<int, NDIM> center(this->getGridBase().getCenterGlobal());

  // Global dimensions
  std::vector<size_t> globalDims = this->getGridBase().getNumCellsGlobal();

  // Setup temp field
  PsGridField<FLOATTYPE, NDIM> initLocalField;
  PsGridBaseItr* gItr = &this->getGridBase();
  initLocalField.setGrid(gItr);

  FLOATTYPE val;

  // Load cavity values
  for (size_t x=0; x<globalDims[0]; ++x) {
    for (size_t y=0; y<globalDims[1]; ++y) {
      for (size_t z=0; z<globalDims[2]; ++z) {

        // Setup tiny vector
        PsTinyVector<int, NDIM> posVec(x,y,z);

        val = FLOATTYPE(fsign)*slabFunc(posVec, center);
        initLocalField.mapToLocalField(posVec, val, "add");

      }}}
  // ***************************************************************

  // Assign charge field values
  PsFieldBase<FLOATTYPE>& chemField = *chemWFieldPtr;
  chemWFieldPtr->reset(0.0);

  PsFieldBase<FLOATTYPE>& initField = *(initLocalField.getBasePtr());
  chemField += initField;
}

//
// Generates a spherical particle cavity with a tanh()
// profile at the interface. Interface width controlled by
// interfaceWidth parameter.
//
template <class FLOATTYPE, size_t NDIM>
FLOATTYPE PsMonomerDens<FLOATTYPE, NDIM>::slabFunc(
      PsTinyVector<int, NDIM>& rvec,
      PsTinyVector<int, NDIM>& r0) {

  FLOATTYPE interfaceWidth = 2.0;
  FLOATTYPE wMax = 0.25;
  FLOATTYPE wMin = -0.25;
  FLOATTYPE tmpLeft, tmpRight, tanhLeft, tanhRight;

  // Global dimensions
  std::vector<size_t> globalDims = this->getGridBase().getNumCellsGlobal();
  int zoneSize = (int)(globalDims[0]/4);
  //  int zoneSize = 10; // Number of cells

  // Find distance to left side and/or right side
  int x = rvec[0];
  int cpos = r0[0];
  int ll = x - (cpos - zoneSize);
  int rr = (cpos + zoneSize) - x;

  //  # Tanh interface on left and right sides of zone
  tmpLeft  = (FLOATTYPE)ll/interfaceWidth;
  tmpRight = (FLOATTYPE)rr/interfaceWidth;
  tanhLeft  = (FLOATTYPE) std::tanh(tmpLeft);
  tanhRight = (FLOATTYPE) std::tanh(tmpRight);

  //  # Zone value and scale for max/min chiN
  FLOATTYPE tmp = 0.5*(tanhLeft + tanhRight);
  FLOATTYPE val = (tmp*(wMax-wMin))+wMin;
  return val;
}

//
// Dump the field values (monomer density/conjugate density)
// leaving this method in case special dump functions arise
//
template <class FLOATTYPE, size_t NDIM>
void PsMonomerDens<FLOATTYPE, NDIM>::dump(TxIoBase* txIoPtr) {

  // Scoping call to base class
  PsPhysFldFuncs<FLOATTYPE, NDIM>::dump(txIoPtr);
}

//
// Change input field by reference to the elementwise product of
// density field and chemical potential field
//
template <class FLOATTYPE, size_t NDIM>
void PsMonomerDens<FLOATTYPE, NDIM>::calcFieldProd(PsFieldBase<FLOATTYPE>& dField) {

  // References to density fields held in base class
  PsFieldBase<FLOATTYPE>& densField = *monoDensFieldPtr;
  PsFieldBase<FLOATTYPE>& chemField = *chemWFieldPtr;

  // Calculate: densField*chemField
  dField.reset(0.0);
  dField += densField;
  dField *= chemField;
}

//
// Instantiations
//
template class PsMonomerDens<float, 1>;
template class PsMonomerDens<float, 2>;
template class PsMonomerDens<float, 3>;

template class PsMonomerDens<double, 1>;
template class PsMonomerDens<double, 2>;
template class PsMonomerDens<double, 3>;
