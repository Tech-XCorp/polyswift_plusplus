/**
 * @file    PsNanoPtcl.cpp
 *
 * @brief   Contains common particle data/methods common to
 *          particle "types".
 *
 * @version $Id: PsNanoPtcl.cpp 8257 2007-09-12 22:05:20Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// txbase includes
#include <TxNotImplExcept.h>

// psptcl includes
#include <PsNanoPtcl.h>

// psstd includes
#include <PsPhysConsts.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsNanoPtcl<FLOATTYPE, NDIM>::PsNanoPtcl() {

  maxNumPtcls = 0;
  fftObjPtr = 0;

  fftSize = 0;
  fftMemSize = 0;
  scaleFFT = 0.0;
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsNanoPtcl<FLOATTYPE, NDIM>::~PsNanoPtcl() {

  for (size_t n=0; n<ptclGroup.size(); ++n) {
    delete ptclGroup[n];
  }
  ptclGroup.clear();
}

// Set the attributes
template <class FLOATTYPE, size_t NDIM>
void PsNanoPtcl<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Scoping call to base class
  PsBndryBase<FLOATTYPE, NDIM>::setAttrib(tas);
  this->dbprt("PsNanoPtcl::setAttrib() ");

  // Number of particles in group type
  if (tas.hasOption("maxNumPtcls")) {
    maxNumPtcls = tas.getOption("maxNumPtcls");
   }
  else {
    TxDebugExcept tde("PsNanoPtcl::setAttrib: maxNumPtcls not set");
    tde << " in <PsNanoPtcl " << this->getName() << " >";
    throw tde;
  }

  // Object name of FFT object
  if (tas.hasString("fftKind")) {
    fftKind = tas.getString("fftKind");
   }
  else {
    TxDebugExcept tde("PsNanoPtcl::setAttrib: fftKind not set");
    tde << " in <PsNanoPtcl " << this->getName() << " >";
    throw tde;
  }

}

//
// Build data
//
template <class FLOATTYPE, size_t NDIM>
void PsNanoPtcl<FLOATTYPE, NDIM>::buildData() {

  // Scoping call to base class
  PsBndryBase<FLOATTYPE, NDIM>::buildData();
  this->dbprt("PsNanoPtcl::buildData() ");

  // Setup cavity field/
  PsGridBaseItr* gItr = &this->getGridBase();
  cavPtclField.setGrid(gItr);

  // Setup global fields space / global dims
  globalPtclDims = this->getGridBase().getNumCellsGlobal();
  globalCavField.setLengths(globalPtclDims);
  globalCavField.reset(0.0);
}

//
// build solvers
//
template <class FLOATTYPE, size_t NDIM>
void PsNanoPtcl<FLOATTYPE, NDIM>::buildSolvers() {

  // Scoping call to base class
  PsBndryBase<FLOATTYPE, NDIM>::buildSolvers();
  this->dbprt("PsNanoPtcl::buildSolvers() ");

  // Get default FFTW method pointer
  fftObjPtr = PsNamedObject::getObject<PsFFTBase<FLOATTYPE, NDIM> >(fftKind);

  // SWS: should be moved to buildData()....
  // Used to set the size of the transform ik etc lists
  // Note: This is dangerous and should be checked against this->bndryDims
  // This is returning "total_local_size"
  fftSize = fftObjPtr->getFFTSize();
  fftMemSize = fftSize * sizeof(FLOATTYPE);

  // Create initial particle data in the template
  // and set "global data" for gradient calc
  setPtclData();

  // Calculate gradient of particle cavity field
  // Needs setPtclData() to be called
  setGradFieldVec();
}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsNanoPtcl<FLOATTYPE, NDIM>::update(double t) {

  this->dbprt("PsNanoPtcl::update ");

  // Transfer all particle group info to depField
  sendPtclsToDepField();

  // Scoping call to base class
  // to use depFields to set physFields
  PsBndryBase<FLOATTYPE, NDIM>::update(t);
}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsNanoPtcl<FLOATTYPE, NDIM>::addPtcl(
             PsBndryDataBase<FLOATTYPE, NDIM>* ptclPtr) {

  // Add to local particle group
  ptclGroup.push_back(ptclPtr);

  // Call static member to add to base static list for all boundaries
  PsBndryBase<FLOATTYPE, NDIM>::addBndry(ptclPtr);

  this->dbprt("addPtcl called. New particle number is = ", (int)ptclGroup.size());
  //this->dbprt("with allBndrys.size = ", (int)this->allBndrys.size());
  int sz = (int)(PsBndryBase<FLOATTYPE, NDIM>::getNumAllBndrys());
  //this->dbprt("with allBndrys.size = ", sz);
  this->dbprt("with index = ", (int)ptclPtr->getBndryIndex());
}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsNanoPtcl<FLOATTYPE, NDIM>::removePtcl(
       PsBndryDataBase<FLOATTYPE, NDIM>* ptclPtr) {

  this->pprt("removePtcl called. Present particle number is = ",
             (int)ptclGroup.size());

  // Call static member to remove from base static list
  // for all particles, calling this first so the object itself
  // can finally be deleted below
  PsBndryBase<FLOATTYPE, NDIM>::removeBndry(ptclPtr);

  // Find iterator position of particle pointer and erase
  typename std::vector< PsBndryDataBase<FLOATTYPE, NDIM>* >::iterator ptclPos;
  ptclPos = find(ptclGroup.begin(), ptclGroup.end(), ptclPtr);

  // SWS: must delete object pointed too, careful... is this right?
  delete ptclPtr;
  ptclGroup.erase(ptclPos);

  this->dbprt("removePtcl finished, new ptcl # = ", (int)ptclGroup.size());
}

//
// Enforces use of global random number generator so all particle
//   data is sync-ed across all procs. Scaled to [-1, 1]
//
template <class FLOATTYPE, size_t NDIM>
FLOATTYPE PsNanoPtcl<FLOATTYPE, NDIM>::getRand() {

  FLOATTYPE rand = psRandomGlobal<FLOATTYPE>();
  return 2.0*(rand - 0.5);
}

//
// Get random vector with norm scaled between [-max, max]
// Uses local getRand that returns globally sync-ed random values
//
template <class FLOATTYPE, size_t NDIM>
PsTinyVector<FLOATTYPE, NDIM> PsNanoPtcl<FLOATTYPE, NDIM>::getRandVec(FLOATTYPE maxVal) {

  // Local variables
  PsTinyVector<FLOATTYPE, NDIM> dr;
  FLOATTYPE drNorm = 0.0;

  // Field cell sizes (dx,dy,dz)
  std::vector<FLOATTYPE> cellSizes = this->getGridBase().getCellSizes();
  std::vector<size_t>    bndryDimens = this->getGridBase().getNumCellsGlobal();

  do {

    for (size_t n=0; n<NDIM; ++n) {
      dr[n] = maxVal*this->getRand();
    }
    // ********************************************
    // SWS: temp kludge for 3D
    if (NDIM==3) {
      // if (this->bndry Dims[NDIM-1] == 1) dr[NDIM-1] = 0;
      if (bndryDimens[NDIM-1] == 1) dr[NDIM-1] = 0;
    }
    // ********************************************

    drNorm = dr.length();
    this->dbprt("random vector with norm maxVal = ", maxVal);
    this->dbprt("calculated |dr| = ", drNorm);

  } while (drNorm > maxVal); // Check for overall length

  return dr;
}

//
// local helper function to set particle field
// directly in particle template and global particle
// cavity field. The global cavity field is used
// to calculate the gradient g(r) function...while
// the template data in PsBndryDataBase is used to
// set the data is the particle objects held by the
// ptclGroup data member
//
template <class FLOATTYPE, size_t NDIM>
void PsNanoPtcl<FLOATTYPE, NDIM>::setPtclData() {

  this->dbprt("PsNanoPtcl::setPtclData entered ");

  // Local coordinate variables
  FLOATTYPE time=1.0;
  FLOATTYPE val=0.0;
  FLOATTYPE valPlus=0.0;
  FLOATTYPE valNeg=0.0;

  // Reset global fields space
  globalCavField.reset(0.0);

  // Global center location, used to shift cavity
  // function to 0 position so grad calc correct
  PsTinyVector<int, NDIM> center(this->getGridBase().getCenterGlobal());

  // Load cavity values
  for (size_t x=0; x<globalPtclDims[0]; ++x) {
    for (size_t y=0; y<globalPtclDims[1]; ++y) {
      for (size_t z=0; z<globalPtclDims[2]; ++z) {

        // Setup tiny vector
        PsTinyVector<int, NDIM> posVec(x,y,z);
        PsTinyVector<int, NDIM> posVecTmp(0);

        // Calculates value at global position
        val = ptclFunc(posVec);

        // Shift to origin 0 position
        posVec = posVec - center;
        this->getGridBase().mapPointToGrid(posVec);

        // Initialize cavity field
        globalCavField(posVec[0], posVec[1], posVec[2], 0) = val;

      } // loop z
    } // loop y
  } // loop x

  // ***************************************************************
  // Initialize local cavPtclField

  for (size_t x=0; x<globalPtclDims[0]; ++x) {
    for (size_t y=0; y<globalPtclDims[1]; ++y) {
      for (size_t z=0; z<globalPtclDims[2]; ++z) {

        PsTinyVector<int, NDIM> posVec(x,y,z);
        FLOATTYPE val = globalCavField(posVec[0], posVec[1], posVec[2], 0);

        cavPtclField.mapToLocalField(posVec, val, "set");
      }}}
  // ***************************************************************

}

//
// dump method for the particle template fields
// mostly for development, testing
//
template <class FLOATTYPE, size_t NDIM>
void PsNanoPtcl<FLOATTYPE, NDIM>::dump(TxIoBase* txIoPtr) {

  this->dbprt("PsNanoPtcl::dump() ");

  // **************************************************
  // txbase-IO data members

  // Global/local sizes
  std::vector<size_t> dataSize, dataLen, dataBeg;
  dataSize = this->getGridBase().getNumCellsGlobal();
  dataSize.push_back(1); // scalar data
  // SWS: need to obtain info from field grid
  dataLen = this->getGridBase().getDecomp().getNumCellsLocal(); // dump
  dataLen.push_back(1);

  // SWS: Depends on data decomp... assuming slab  (refactor)
  size_t offset = dataLen[0]*this->getCommBase().getRank();
  dataBeg.push_back(offset);
  dataBeg.push_back(0);
  dataBeg.push_back(0);
  dataBeg.push_back(0);
  // **************************************************

  size_t dumpDims = 2; // SWS: careful to change this
  std::string ptclDataName = "PtclTemplateField";
  std::vector<std::string> gradDataNames;
  gradDataNames.push_back("gradientX");
  gradDataNames.push_back("gradientY");
  gradDataNames.push_back("gradientZ");

  //
  // Create/open filename
  // this has no dump number suffix so there's one file for now
  //
  std::string datName = this->getName() + "BaseAuxillary";
  std::string fileName = txIoPtr->getDumpFileName(datName,false);

  // Check if filename already exists
  bool densFileExists = txIoPtr->fileExists(fileName);

  if (!densFileExists) {

    this->dbprt("The file ", fileName, " doesnt exist");

    // create a file
    TxIoNodeType fn = txIoPtr->createFile(fileName);
    FLOATTYPE* data = cavPtclField.getDataPtr();
    TxIoNodeType ds = txIoPtr->writeDataSet(fn, ptclDataName,
                                            dataSize, dataBeg, dataLen,
                                            data);
    // close nodes
    txIoPtr->closeDataSet(ds);

    // ******************************************************************
    // Format monomer density data for IO method write

    for (size_t icomp=0; icomp<dumpDims; ++icomp) {
      data = gradFieldVec[icomp].getDataPtr();
      TxIoNodeType ds2 = txIoPtr->writeDataSet(fn, gradDataNames[icomp],
                                               dataSize, dataBeg, dataLen,
                                               data);
      // close nodes
      txIoPtr->closeDataSet(ds2);
    }
    // ******************************************************************

    // Append the vizschema attributes
    appendMetaDataset(txIoPtr, fn, ptclDataName);
    for (size_t icomp=0; icomp<dumpDims; ++icomp) {
      appendMetaDataset(txIoPtr, fn, gradDataNames[icomp]);
    }
    appendMetaGroup(txIoPtr, fn);
    txIoPtr->closeFile(fn);

  } // file exists
}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsNanoPtcl<FLOATTYPE, NDIM>::appendMetaDataset(TxIoBase* txIoPtr,
                                                    TxIoNodeType fn,
                                                    std::string datasetName) {

  TxIoNodeType dn = txIoPtr->openDataSet(fn, datasetName);

  txIoPtr->writeAttribute(dn, "vsType", "variable");
  txIoPtr->writeAttribute(dn, "vsMesh", "mesh");
  txIoPtr->writeAttribute(dn, "vsLimits", "limits");

  txIoPtr->closeDataSet(dn);

  // For safety
  //  this->getCommBase().barrier();
}

template <class FLOATTYPE, size_t NDIM>
void PsNanoPtcl<FLOATTYPE, NDIM>::appendMetaGroup(TxIoBase* txIoPtr,
                                                  TxIoNodeType fn) {

  // appendLimits
  TxIoNodeType lgn = txIoPtr->createGroup(fn, "limits");
  txIoPtr->writeAttribute(lgn, "vsKind", "Cartesian");
  txIoPtr->writeAttribute(lgn, "vsType", "limits");
  std::vector<FLOATTYPE> lowerB, upperB;
  lowerB = this->getGridBase().getOrigin();
  upperB = this->getGridBase().getGlobalLengths();
  txIoPtr->writeAttribute(lgn, "vsLowerBounds", lowerB);
  txIoPtr->writeAttribute(lgn, "vsUpperBounds", upperB);

  // appendGrid
  TxIoNodeType ggn = txIoPtr->createGroup(fn, "mesh");
  txIoPtr->writeAttribute(ggn, "vsKind", "uniform");
  txIoPtr->writeAttribute(ggn, "vsType", "mesh");
  txIoPtr->writeAttribute(ggn, "vsLowerBounds", lowerB);
  txIoPtr->writeAttribute(ggn, "vsUpperBounds", upperB);
  std::vector<size_t> numCells, startCell;
  numCells = this->getGridBase().getNumNodesGlobal();
  startCell.push_back(0);
  startCell.push_back(0);
  startCell.push_back(0);
  txIoPtr->writeAttribute(ggn, "vsNumCells", numCells);
  txIoPtr->writeAttribute(ggn, "vsStartCell", startCell);

  // For safety
  //  this->getCommBase().barrier();
}

//
// private helper method to calculate gradient
// of cavity function
//
template <class FLOATTYPE, size_t NDIM>
void PsNanoPtcl<FLOATTYPE, NDIM>::setGradFieldVec() {

  // *********************************************************
  // Build local k-vector fields

  PsGridBaseItr* gItr = &this->getGridBase();

  // Populate global k vectors
  PsGridField<FLOATTYPE, NDIM> kRLocal;
  kRLocal.setGrid(gItr);
  kRLocal.setNumElements(3);
  kRLocal.calckVec();

  // Map to 1-component fields
  PsGridField<FLOATTYPE, NDIM> kxLocal;
  PsGridField<FLOATTYPE, NDIM> kyLocal;
  PsGridField<FLOATTYPE, NDIM> kzLocal;
  kxLocal.setGrid(gItr);
  kyLocal.setGrid(gItr);
  kzLocal.setGrid(gItr);

  std::vector<size_t> localDims =
    this->getGridBase().getDecomp().getNumCellsLocal();
  for (size_t x=0; x<localDims[0]; ++x) {
  for (size_t y=0; y<localDims[1]; ++y) {
  for (size_t z=0; z<localDims[2]; ++z) {
    kxLocal(x, y, z, 0) = kRLocal(x, y, z, 0);
    kyLocal(x, y, z, 0) = kRLocal(x, y, z, 1);
    kzLocal(x, y, z, 0) = kRLocal(x, y, z, 2);
  }}}

  // Populate vector of 1-component fields
  std::vector<PsGridField<FLOATTYPE, NDIM> > ikVecField;
  ikVecField.clear();
  ikVecField.push_back(kxLocal);
  ikVecField.push_back(kyLocal);
  ikVecField.push_back(kzLocal);
  // *********************************************************

  // *******************************************************
  // Set transform scale factor
  std::vector<size_t> globalSize = globalPtclDims;
  size_t gsize = 1;
  for (size_t n=0; n<globalSize.size(); ++n)
    gsize = gsize * globalSize[n];
  scaleFFT = 1.0 / ((FLOATTYPE) gsize);
  this->dbprt("scaleFFT in from grad calc = ", scaleFFT);
  // *******************************************************

  // ***********************************************************************
  // Calculate gradient

  // Setup single-component, local gradient field
  PsGridField<FLOATTYPE, NDIM> gradPtclField;
  gradPtclField.setGrid(gItr);

  // Local data pointers
  FLOATTYPE* resPtr = new FLOATTYPE[fftSize];
  FLOATTYPE* gradPtr = 0;

  // Loop on spatial components
  for (size_t ic=0; ic<NDIM; ++ic) {

    fftObjPtr->scaledFFTPairIm(cavPtclField.getDataPtr(),
                               ikVecField[ic].getDataPtr(),
                               resPtr);

    gradPtr = gradPtclField.getDataPtr();  // Get gradPtclField data
    memcpy(gradPtr, resPtr, fftMemSize);   // Copy result into gradField ptr
    gradPtclField.scale(1.0*scaleFFT);     // Scale factor for FFT
    gradFieldVec.push_back(gradPtclField); // Set in main list
  }
  delete[] resPtr;
  // ***********************************************************************

  // Clean
  globalSize.clear();
}

// ***********************************************************************
//                        Private helper methods
// ***********************************************************************
//
// Takes ptclGroup info and deposits into local depField
// (now private after refactor in Hldr)
//
template <class FLOATTYPE, size_t NDIM>
void PsNanoPtcl<FLOATTYPE, NDIM>::sendPtclsToDepField() {

  // Resetting local ptclGroup dep field
  this->bndryDepField.reset(0.0);

  // Set ptclField with data from ptclGroup
  for (size_t n=0; n<ptclGroup.size(); ++n) {

    // Get pointer to particle interface
    PsBndryDataBase<FLOATTYPE, NDIM>* ptcl = ptclGroup[n];

    // Loop on field elements
    for (size_t ff=0; ff<ptcl->getNumFieldElements(); ++ff) {

      PsTinyVector<int, NDIM> pos = ptcl->getFieldPos(ff);
      FLOATTYPE val = ptcl->getFieldVal(ff);
      this->bndryDepField.mapToLocalField(pos, val, "add");
    }

  }   // particle loop
}

// Instantiation
template class PsNanoPtcl<float, 1>;
template class PsNanoPtcl<float, 2>;
template class PsNanoPtcl<float, 3>;

template class PsNanoPtcl<double, 1>;
template class PsNanoPtcl<double, 2>;
template class PsNanoPtcl<double, 3>;
