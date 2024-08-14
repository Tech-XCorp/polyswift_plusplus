/**
 * @file    PsSphere.cpp
 *
 * @brief   Class for purely translate-able nanoparticles
 *
 * @version $Id: PsSphere.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2009-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psptcl includes
#include <PsSphere.h>

// psstd includes
#include <PsRandom.h>

// txbase includes
#include <TxNotImplExcept.h>
#include <TxTensor.h>
#include <TxMakerMap.h>

// std include
#include <random>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsSphere<FLOATTYPE, NDIM>::PsSphere() {

  // Dataset name for IO
  ptclDataSetName = "ParticleCenters";

  // Maximum displacement --> half Rg
  maxdr = 0.5;

  // Max noise factor set --> maxdr
  ptclNoiseFactor = 1.0;

  initialNumPtcls = 0;
  scaleForceFactor = 1.0;
  tstepBeforeFirstAdd = 10;
  //  updateMovePeriod = 500;
  updateAddPeriod = 1000;
  willSkipRotate = true;
  insertSTFunc = NULL;
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsSphere<FLOATTYPE, NDIM>::~PsSphere() {

  delete insertSTFunc;
  rotMatrices.clear();
}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsSphere<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Scoping call to base class
  PsNanoPtcl<FLOATTYPE, NDIM>::setAttrib(tas);

  this->dbprt("PsSphere::setAttrib() ");

  // Radius of particle
  if (tas.hasOption("radius")) {
    radius = tas.getOption("radius");
  }
  else {
    TxDebugExcept tde("PsSphere::setAttrib: radius not set");
    tde << " in <PsNanoPtcl " << this->getName() << " >";
    throw tde;
  }

  // 'Dynamic' radius of particle
  if (tas.hasOption("dynRadius")) {
    dynRadius = tas.getOption("dynRadius");
  }
  else {
    dynRadius = radius;
  }

  /*
  // Number of timesteps between moves
  if (tas.hasOption("updateMovePeriod")) {
    updateMovePeriod = tas.getOption("updateMovePeriod");
   }
  else {
    TxDebugExcept tde("PsSphere::setAttrib: updateMovePeriod not set");
    tde << " in <PsNanoPtcl " << this->getName() << " >";
    throw tde;
  }
  */

  // Number of timesteps between moves
  if (tas.hasOption("updateAddPeriod")) {
    updateAddPeriod = tas.getOption("updateAddPeriod");
   }
  else {
    TxDebugExcept tde("PsSphere::setAttrib: updateAddPeriod not set");
    tde << " in <PsNanoPtcl " << this->getName() << " >";
    throw tde;
  }

  // Number of timesteps before first particle is inserted
  if (tas.hasOption("tstepBeforeFirstAdd")) {
    tstepBeforeFirstAdd = tas.getOption("tstepBeforeFirstAdd");
   }
  else {
    TxDebugExcept tde("PsSphere::setAttrib: tstepBeforeFirstAdd not set");
    tde << " in <PsNanoPtcl " << this->getName() << " >";
    throw tde;
  }

  // Width of particle interface
  if (tas.hasParam("interfaceWidth")) {
    interfaceWidth = tas.getParam("interfaceWidth");
   }
  else {
    TxDebugExcept tde("PsSphere::setAttrib: interfaceWidth not set");
    tde << " in <PsNanoPtcl " << this->getName() << " >";
    throw tde;
  }

  // Maximum allowed particle displacement
  if (tas.hasParam("maxdr")) {
    maxdr = tas.getParam("maxdr");
   }
  else {
    this->dbprt("PsSphere::setAttrib maxdr set to default");
  }

  // Width check
  if (interfaceWidth <= 0.0) {
    TxDebugExcept tde("PsSphere::setAttrib: interfaceWidth must be >0");
    tde << " in <PsNanoPtcl " << this->getName() << " >";
    throw tde;
  }

  // Number of timesteps before first particle is inserted
  // Move to PsInteractingSphere???
  if (tas.hasParam("ptclNoiseFactor")) {
    ptclNoiseFactor = tas.getParam("ptclNoiseFactor");
   }
  else {
    this->dbprt("PsSphere::setAttrib ptclNoiseFactor set to default");
  }

  // Scale force factor for dynamics
  if (tas.hasParam("scaleForceFactor")) {
    scaleForceFactor = tas.getParam("scaleForceFactor");
  }
  else {
    this->pprt("PsSphere::setAttrib: default scaleForceFactor=1.0 set");
  }

  // Parse input parameters for STFunc's, one for now
  funcNames = tas.getNamesOfType("STFunc");
  if (funcNames.size() > 0) {
    funcAttrib = tas.getAttrib(funcNames[0]);
  }

  // Flag for skipping sliding moves
  if (tas.hasString("willSkipRotate")) {
    std::string skipFlag = tas.getString("willSkipRotate");
    if (skipFlag == "on") willSkipRotate = true;
  }

  // Radius check
  if (radius < 2) {
    TxDebugExcept tde("PsSphere::setAttrib: radius < 2, try decrease in dx");
    tde << " in <PsNanoPtcl " << this->getName() << " >";
    throw tde;
  }

}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsSphere<FLOATTYPE, NDIM>::buildData() {

  // Scoping call to base class
  PsNanoPtcl<FLOATTYPE, NDIM>::buildData();

  // Parse input parameters for STFunc's, one for now
  if (funcNames.size() > 0) {
    std::string kind = funcAttrib.getString("kind");
    insertSTFunc =
      TxMakerMap< PsSTFunc<FLOATTYPE, NDIM> >::getNew(kind);
    insertSTFunc->setAttrib(funcAttrib);
  }
  else {
    this->dbprt("No STFunc for PsSphere: insert region is system grid");
  }

}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsSphere<FLOATTYPE, NDIM>::buildSolvers() {

  // Scoping call to base class
  //  ...sets field values in setPtclField
  PsNanoPtcl<FLOATTYPE, NDIM>::buildSolvers();

  this->dbprt("PsSphere::buildSolvers() ");

  // Set the template sphere data from global list in base class
  setSphereData();

  // +90z rotation
  rotZ[0] = 0; rotZ[1] = -1; rotZ[2] =  0;
  rotZ[3] = 1; rotZ[4] =  0; rotZ[5] =  0;
  rotZ[6] = 0; rotZ[7] =  0; rotZ[8] =  1;

  // +90y rotation
  rotY[0] = 0; rotY[1] =  0; rotY[2] = -1;
  rotY[3] = 0; rotY[4] =  1; rotY[5] =  0;
  rotY[6] = 1; rotY[7] =  0; rotY[8] =  0;

  // +90x rotation
  rotX[0] = 1; rotX[1] =  0; rotX[2] =  0;
  rotX[3] = 0; rotX[4] =  0; rotX[5] = -1;
  rotX[6] = 0; rotX[7] =  1; rotX[8] =  0;

  // *********************************************

  // -90z rotation
  rotNegZ[0] =  0; rotNegZ[1] =  1; rotNegZ[2] = 0;
  rotNegZ[3] = -1; rotNegZ[4] =  0; rotNegZ[5] = 0;
  rotNegZ[6] =  0; rotNegZ[7] =  0; rotNegZ[8] = 1;

  // -90y rotation
  rotNegY[0] =  0; rotNegY[1] =  0; rotNegY[2] = 1;
  rotNegY[3] =  0; rotNegY[4] =  1; rotNegY[5] = 0;
  rotNegY[6] = -1; rotNegY[7] =  0; rotNegY[8] = 0;

  // -90x rotation
  rotNegX[0] =  1; rotNegX[1] =  0; rotNegX[2] = 0;
  rotNegX[3] =  0; rotNegX[4] =  0; rotNegX[5] = 1;
  rotNegX[6] =  0; rotNegX[7] = -1; rotNegX[8] = 0;

  // Kludge for dimensions
  std::vector<size_t> globalDims =
    this->getGridBase().getNumCellsGlobal();

  // Set all rotation matrices in list
  // These separate lists are temporary
  if (globalDims[2] == 1) {
    this->dbprt("Setting rotMatrices for 2D ");
    rotMatrices.push_back(rotZ);
    rotMatrices.push_back(rotNegZ);
  }
  else {
    this->dbprt("Setting rotMatrices for 3D ");
    rotMatrices.push_back(rotZ);
    rotMatrices.push_back(rotNegZ);
    rotMatrices.push_back(rotY);
    rotMatrices.push_back(rotNegY);
    rotMatrices.push_back(rotX);
    rotMatrices.push_back(rotNegX);
  }

  /*
  // Test vectors
  PsTinyVector<int, NDIM>    rvec;
  PsTinyVector<int, NDIM>    rnewvec;

    rvec[0] = 1;
    rvec[1] = 0;
    rvec[2] = 0;
    rnewvec = rotZ*rvec;
    if (!(this->getCommBase().getRank()) ) {
      std::cout << "rnewvec[0] = " << rnewvec[0] << std::endl;
      std::cout << "rnewvec[1] = " << rnewvec[1] << std::endl;
      std::cout << "rnewvec[2] = " << rnewvec[2] << std::endl;
    }
  */

}

//
// Generates a spherical particle cavity with a tanh()
// profile at the interface. Interface width controlled by
// interfaceWidth parameter.
//
template <class FLOATTYPE, size_t NDIM>
FLOATTYPE PsSphere<FLOATTYPE, NDIM>::ptclFunc(PsTinyVector<int, NDIM>& rvec) {

  // Global center location
  PsTinyVector<int, NDIM> center(this->getGridBase().getCenterGlobal());

  // Sphere function
  FLOATTYPE rdis = this->getGridBase().mapDistToGrid(rvec, center);
  FLOATTYPE tmp = (rdis-radius)/interfaceWidth;
  FLOATTYPE tanhVal = (FLOATTYPE) std::tanh(tmp);
  tmp = (tanhVal+1.0)/2.0;
  return (1.0 - tmp);
}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsSphere<FLOATTYPE, NDIM>::initialize() {

  this->dbprt("PsSphere::initialize() ");

  // Build initial particle fields
  while (this->ptclGroup.size() != initialNumPtcls) {
    buildPtcl();
  }
}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsSphere<FLOATTYPE, NDIM>::restore(TxIoBase* txIoPtr) {

  this->dbprt("PsSphere::restore() ");

  // ****************************************************************
  // IO section:  get particle centers

  // Create/open filename
  std::string fileName = txIoPtr->getDumpFileName(this->getName());

  this->dbprt("File ", fileName, " will be searched for");

  // Check if filename already exists
  bool ptclFileExists = txIoPtr->fileExists(fileName);

  if (ptclFileExists) {

    TxIoNodeType fn = txIoPtr->openFile(fileName, "r");

    std::vector<size_t> shapeList =
      txIoPtr->getDataSetSize(fn, ptclDataSetName);

    size_t ptclGroupNum = shapeList[0];
    size_t ptclInfoDims = shapeList[1];
    size_t dataSizeInfo = ptclGroupNum*ptclInfoDims;

    // Setup data buffer
    FLOATTYPE* data = new FLOATTYPE[dataSizeInfo];

    // **************************************************
    // txbase-IO data members

    // Global/local sizes
    std::vector<size_t> dataSize, dataLen, dataBeg;
    dataSize.push_back(ptclGroupNum);
    dataSize.push_back(ptclInfoDims);
    dataLen.push_back(ptclGroupNum);
    dataLen.push_back(ptclInfoDims);
    dataBeg.push_back(0);
    dataBeg.push_back(0);
    // **************************************************

    // Access data in field
    TxIoNodeType dw = txIoPtr->readDataSet(fn, ptclDataSetName,
                                           dataBeg, dataLen,
                                           data);
    // close nodes
    txIoPtr->closeDataSet(dw);
    shapeList.clear();
    txIoPtr->closeFile(fn);

    // *****************************************************
    // Format data for particle centers

    // Cell sizes for conversion to grid position
    std::vector<FLOATTYPE> cellSizes =
      this->getGridBase().getCellSizes();

    // List for holding all centers
    std::vector< PsTinyVector<int, NDIM> > allPtclCenters;

    for (size_t n=0; n<ptclGroupNum; ++n) {

      PsTinyVector<int, NDIM> ptclCenter(0);
      size_t ptclIndx = n*ptclInfoDims;
      ptclCenter[0] = (int) (data[ptclIndx+0] / cellSizes[0]);
      ptclCenter[1] = (int) (data[ptclIndx+1] / cellSizes[1]);
      ptclCenter[2] = (int) (data[ptclIndx+2] / cellSizes[2]);

      allPtclCenters.push_back(ptclCenter);
    }

    delete[] data;
    /* ***************************************************** */

    // Build initial particle fields
    for (size_t n=0; n<allPtclCenters.size(); ++n) {
      if (!this->getCommBase().getRank())
        std::cout << "restoring particle at --> " << allPtclCenters[n] << std::endl;
      PsTinyVector<int, NDIM>& ptclPos = allPtclCenters[n];
      buildPtcl(ptclPos);
    }

  } // ***** file exists ******
}

//
// local helper function
//
// Create data structures necessary for nanoparticle, randomly
//   place particle until no overlaps are found
//
// Uses the templatePtclData field to construct new particles,
//   moves to a trial position, and checks for overlaps with all
//   other nanoparticles present
//
template <class FLOATTYPE, size_t NDIM>
void PsSphere<FLOATTYPE, NDIM>::buildPtcl() {

  // Current number of particle types
  size_t currentPtclNum = this->ptclGroup.size();

  // Attempt insert until successful
  while (this->ptclGroup.size() != currentPtclNum+1) {

    // Have makermap return a pointer to a boundary data object
    PsBndryDataBase<FLOATTYPE, NDIM>* ptclPtr =
      TxMakerMap< PsBndryDataBase<FLOATTYPE, NDIM> >::getNew("sphereData");
    if (!ptclPtr) {
      TxDebugExcept tde("PsSphere::buildSolvers: Particle pointer not set");
      throw tde;
    }

    // Call build methods for data class and field values
    // with template object, (note: the field has been shift to [0,0])
    ptclPtr->setDomainSingletons(this->getDomainSingletons() );
    ptclPtr->buildData();
    ptclPtr->buildSolvers();
    ptclPtr->setBndryData(tplSphereData);

    // Cast to spherical data class for specific data access
    PsSphereData<FLOATTYPE, NDIM>* spherePtr =
      dynamic_cast< PsSphereData<FLOATTYPE, NDIM>* >(ptclPtr);

    // Set specific sphere values
    PsTinyVector<int, NDIM> origin(0);
    spherePtr->setCenter(origin);               // Set particle center
    spherePtr->setRadius((FLOATTYPE)radius);    // Set particle radius
    spherePtr->setDynamicRadius((FLOATTYPE)dynRadius); // Set particle radius

    // ********************************************************************
    // Get a random global position and make trial move
    PsTinyVector<int, NDIM> dr0(this->getGridBase().getRandomGlobalPt() );
    movePtcl(ptclPtr, dr0);
    // ********************************************************************

    // Select insert region
    FLOATTYPE insertFlag; FLOATTYPE time=1.0;
    if (insertSTFunc) {
      insertFlag = insertSTFunc->operator()(dr0, time);
      if (insertFlag < 1.0) continue;
    }

    // Check overlaps to list
    if ( this->doesBndryOverlap(ptclPtr) ) {
      this->dbprt("insert made overlap");
      this->dbprt("... deleting ptclPtr, trying again");
      // Free memory created by call to map
      delete ptclPtr; continue;
    }
    else {
      this->addPtcl(ptclPtr);
    }

  } // loop to add one particle
}

//
// local helper function
// Create data structures necessary for nanoparticle,
// place particle at specified ptclPos and quit if overlaps found
// Uses the template field to construct new particles,
//
template <class FLOATTYPE, size_t NDIM>
void PsSphere<FLOATTYPE, NDIM>::buildPtcl(PsTinyVector<int, NDIM>& ptclPos) {

  // Current number of particle types
  size_t currentPtclNum = this->ptclGroup.size();

  // Have makermap return a pointer to a boundary data object
  PsBndryDataBase<FLOATTYPE, NDIM>* ptclPtr =
    TxMakerMap< PsBndryDataBase<FLOATTYPE, NDIM> >::getNew("sphereData");
  if (!ptclPtr) {
    TxDebugExcept tde("PsSphere::buildSolvers: Particle pointer not set");
    throw tde;
  }

  // Call build methods for data class and field values with template object
  // (note: the field has been shift to [0,0])
  ptclPtr->setDomainSingletons(this->getDomainSingletons() );
  ptclPtr->buildData();
  ptclPtr->buildSolvers();
  ptclPtr->setBndryData(tplSphereData);

  // Cast to spherical data class for specific data
  // access and set sphere values
  PsSphereData<FLOATTYPE, NDIM>* spherePtr =
    dynamic_cast< PsSphereData<FLOATTYPE, NDIM>* >(ptclPtr);
  PsTinyVector<int, NDIM> origin(0);
  spherePtr->setCenter(origin);                      // Set particle center
  spherePtr->setRadius((FLOATTYPE)radius);           // Set particle radius
  spherePtr->setDynamicRadius((FLOATTYPE)dynRadius); // Set particle radius

  // Move to specified location
  movePtcl(ptclPtr, ptclPos);

  // Check overlaps to list
  if ( this->doesBndryOverlap(ptclPtr) ) {
    // Free memory created by call to map
    delete ptclPtr;
    TxDebugExcept tde("PsSphere::buildPtcl(ptclPos): overlaps found");
    throw tde;
  }
  else {
    this->addPtcl(ptclPtr);
  }
}

//
// Manage inserting particles... for now deleting elsewhere
//
template <class FLOATTYPE, size_t NDIM>
void PsSphere<FLOATTYPE, NDIM>::update(double t) {

  this->dbprt("PsSphere::update() ");

  // Get simulation timestep
  size_t timeStep = PsDynObjBase::getCurrDomainStep();
  double tpresent = (double)timeStep;

  // Update present timestep --> "next" timestep:
  if (t > tpresent) {

    // Check for start add interval
    if (timeStep >= tstepBeforeFirstAdd) {

      // Check for maximum number of particles
      if (this->ptclGroup.size() < this->maxNumPtcls) {
        if (timeStep % updateAddPeriod == 0) buildPtcl();
      }
    }

  }

  // Scoping call
  PsNanoPtcl<FLOATTYPE, NDIM>::update(t);
}

//
// Effective dynamics implementation: includes force scaling
// to estimate displacement and checks for maximum displacement
// in order to enforce stability
//
// Includes fluctuating noise contribution, and rounds to
//   estimate position on the grid
//
template <class FLOATTYPE, size_t NDIM>
void PsSphere<FLOATTYPE, NDIM>::calculateMoves(
      std::vector< PsFieldBase<FLOATTYPE>* >& forceFld) {

  // Temp holders for force vector and displacements
  PsTinyVector<FLOATTYPE, NDIM> pfvec;
  std::vector< PsTinyVector<int, NDIM> > drGlobalVec;
  std::vector< PsTinyVector<int, NDIM> > drVec;

  // *************************************************
  // Loop on particles
  // *************************************************
  for (size_t n=0; n<this->ptclGroup.size(); ++n) {

    // Get pointer to particle object and cast to spherical data
    // class for specific data access and get sphere centers
    PsBndryDataBase<FLOATTYPE, NDIM>* ptclPtr = this->ptclGroup[n];
    PsSphereData<FLOATTYPE, NDIM>* spherePtr =
        dynamic_cast< PsSphereData<FLOATTYPE, NDIM>* >(ptclPtr);

    // Displacement vectors
    PsTinyVector<FLOATTYPE, NDIM> dr(0.0);
    PsTinyVector<int, NDIM>       idr(0);

    // *************************************************************
    // If center owned by proc, obtain force... then add
    // random displacement and set dr values in particle
    // data class. Then sync all dr values

    // Global center position of particle, does local own
    PsTinyVector<int, NDIM> gc = spherePtr->getCenter();
    bool centerOwned = this->getGridBase().getDecomp().hasPosition(gc);

    // Local center vector
    PsTinyVector<int, NDIM> localgc;

    // Note: no rank dependent calls inside decision and force calc
    if (centerOwned) {

      localgc = this->getGridBase().mapToLocalVec(gc);

      for (size_t ic=0; ic<NDIM; ++ic) {

        // SWS: this casting logic needs revising...
        PsFieldBase<FLOATTYPE>* fcompbase = forceFld[ic];
        PsGridField<FLOATTYPE, NDIM>* fcompPtr =
          dynamic_cast< PsGridField<FLOATTYPE, NDIM>* >(fcompbase);

        // Pick off force-component
        // dr should be proportional to real distance in [Rg]
        PsGridField<FLOATTYPE, NDIM>& fcomp = *fcompPtr;
        pfvec[ic] = fcomp((size_t)localgc[0],
                          (size_t)localgc[1],
                          (size_t)localgc[2], 0);
        dr[ic] = scaleForceFactor * pfvec[ic];
      }

    } // center owned

    // Generate random global displacment in
    // order to keep ranGenerator sync-ed
    PsTinyVector<FLOATTYPE, NDIM> randVec = this->getRandVec(ptclNoiseFactor*maxdr);

    // Only domain that owns center will be setting dr
    // Set to zero if center not owned
    if (centerOwned) {

      dr = dr + randVec;
      if (dr.length() > maxdr) {
        FLOATTYPE scaleDownFac = maxdr/dr.length();
        dr *= scaleDownFac;
      }

      // Cell sizes for convert to 'real' position
      std::vector<FLOATTYPE> cellSizes = this->getGridBase().getCellSizes();

      // Apply cellSizes by component to convert to cell displacement
      // SWS: should move this elsewhere...?
      for (size_t i=0; i<NDIM; ++i) dr[i] = dr[i]/cellSizes[i];

      idr = round(dr);
      drVec.push_back(idr);
    }

    else {
      PsTinyVector<int, NDIM> idr(0);
      drVec.push_back(idr);
    }

  } // loop on particles

  // ****************************************************************
  // Allreduce for dr list so all procs are sync-ed
  // drGlobalVec assumes position in list is sync-ed
  // for ptclGroup position
  //
  this->getCommBase().barrier();
  size_t thisRank = this->getCommBase().getRank();
  this->getCommBase().allReduceSumVec(drVec, drGlobalVec);
  this->getCommBase().barrier();

  // Set dr values from sync-ed global list
  for (size_t n=0; n<this->ptclGroup.size(); ++n) {

    PsBndryDataBase<FLOATTYPE, NDIM>* ptclPtr = this->ptclGroup[n];

    PsSphereData<FLOATTYPE, NDIM>* spherePtr =
      dynamic_cast< PsSphereData<FLOATTYPE, NDIM>* >(ptclPtr);

    spherePtr->setDr(drGlobalVec[n]);
  }
  // ****************************************************************

}

//
// local helper function to set particle field
// template from the global particle data in base class
//
template <class FLOATTYPE, size_t NDIM>
void PsSphere<FLOATTYPE, NDIM>::setSphereData() {

  this->dbprt("PsSphere::setSphereData ");

  // Local coordinate variables
  FLOATTYPE val=0.0;

  // Load cavity values
  for (size_t x=0; x<this->globalPtclDims[0]; ++x) {
    for (size_t y=0; y<this->globalPtclDims[1]; ++y) {
      for (size_t z=0; z<this->globalPtclDims[2]; ++z) {

        // Setup tiny vector
        //      int ix = (int)x; int iy = (int)y; int iz = (int)z;
        PsTinyVector<int, NDIM> posVec(x,y,z);

        // Initialize cavity field
        val = this->globalCavField(posVec[0], posVec[1], posVec[2], 0);

        // If above threshold, then set particle values
        // appropriately in the PtclData object
        if (val > this->bndryFieldThreshold) {
          tplSphereData.addFieldData(posVec,val);
        }

      } // loop z
    } // loop y
  } // loop x

  // Check
  if (tplSphereData.getNumFieldElements() == 0) {
    TxDebugExcept tde("PsSphere::setSphereData:: # field elements = 0");
    tde << " in <PsNanoPtcl " << this->getName() << " >";
    throw tde;
  }

  // Setting debug flag
  tplSphereData.setDebugFlag(this->dbStatus);
}

//
// A driver for movePtcl method.
//    Takes specified displacements,
//    makes trial movements
//    checks for overlaps and accepts/rejects
//
template <class FLOATTYPE, size_t NDIM>
void PsSphere<FLOATTYPE, NDIM>::moveCheckAllPtcls() {

  this->dbprt("PsSphere::moveCheckAllPtcls ");

  // Local flag for tracking if a particle is removed
  //  bool removedParticleThisUpdate = false;

  //
  // Reverse order of particles in vector (primitive shuffle)
  // to mix up move update order
  //
#ifdef HAVE_MPI
  reverse(this->ptclGroup.begin(), this->ptclGroup.end() );
#else
  // This will be problematic in parallel
  std::random_device rd;
// Need to get global random seed here and using it in the next
// line instead of the random_device
  std::mt19937 g(rd());
// std::mt19937 g(randomSeed);
  std::shuffle(this->ptclGroup.begin(), this->ptclGroup.end(), g);
#endif

  // Local list to tag particle for removal, AFTER all
  // displacments are applied
  std::vector< PsBndryDataBase<FLOATTYPE, NDIM>* > taggedForRemoval;

  //
  // Move particles in local group
  //
  for (size_t n=0; n<this->ptclGroup.size(); ++n) {

    // Get particle ptr and get displacment from local list
    PsBndryDataBase<FLOATTYPE, NDIM>* ptclPtr = this->ptclGroup[n];

    PsSphereData<FLOATTYPE, NDIM>* spherePtr =
      dynamic_cast< PsSphereData<FLOATTYPE, NDIM>* >(ptclPtr);
    PsTinyVector<int, NDIM> idr = spherePtr->getDr();

    // ********************************************************
    // Move algorithm

    // Pick next rotate move as first attempt
    PsTinyVector<int, NDIM> rotdr(0);
    rotate(rotMatrices.begin(),
           rotMatrices.begin()+1,
           rotMatrices.end());

    // Move particle with check
    bool moveOk = movePtclwCheck(ptclPtr, idr);

      // Accepts 1st move above then --> next particle
    if (moveOk) { continue; }
    else {

      // SWS: should skip sliding moves...
      if (willSkipRotate) continue;

      // Apply all "rotation moves" as needed
      for (size_t nn=0; nn<rotMatrices.size(); ++nn) {
        if (!(this->getCommBase().getRank()) )
          std::cout << "trying rot for rotMatrices[nn] = " << nn << std::endl;
        rotdr = (rotMatrices[nn])*idr;
        moveOk = movePtclwCheck(ptclPtr, rotdr);
        if (moveOk) {
          if (!(this->getCommBase().getRank()) )
            std::cout << "rot OK for rotMatrices[n] = " << nn << std::endl;
        }
        if (moveOk) break;
      }

    }

    // This tags a particle for removal after all moves in
    // loop above arent successful
    if (!moveOk) {
      taggedForRemoval.push_back(ptclPtr);
      this->pprt("tagging for removal ");
    }
    else {
      this->pprt("ACCEPTING move ", (int)idr[0]);
      this->pprt("               ", (int)idr[1]);
      this->pprt("               ", (int)idr[2]);
      this->pprt(" for particle n = ", (int)n);
    }
    // ********************************************************

    // For safety
    this->getCommBase().barrier();

  } // particle loop

  //
  // Remove some subset of the tagged particles
  //
  size_t maxRemovedPtcls = 1;
  if (taggedForRemoval.size() > maxRemovedPtcls) {
    taggedForRemoval.resize(maxRemovedPtcls);
  }
  for (size_t n=0; n<taggedForRemoval.size(); ++n) {
    PsBndryDataBase<FLOATTYPE, NDIM>* ptclPtr = taggedForRemoval[n];
    this->removePtcl(ptclPtr);
    this->pprt(" removing particle after all updates ");
  }
  taggedForRemoval.clear();

  // For safety
  this->getCommBase().barrier();
}

//
// This move method is local to this class which is only
// for non-orientable (spherical) particles whose only degree
// of freedom in the system is that of translational movement.
//
template <class FLOATTYPE, size_t NDIM>
void PsSphere<FLOATTYPE, NDIM>::movePtcl(
    PsBndryDataBase<FLOATTYPE, NDIM>* ptclPtr, PsTinyVector<int, NDIM> dr) {

  this->dbprt("movePtcl entered ");

  // Position vec
  PsTinyVector<int, NDIM> posvec;

  // Move field elements
  for (size_t ff=0; ff<ptclPtr->getNumFieldElements(); ++ff) {
    posvec = ptclPtr->getFieldPos(ff);        // get element
    posvec = posvec + dr;                     // advance element
    this->getGridBase().mapPointToGrid(posvec); // fold into box
    ptclPtr->setFieldPos(posvec,ff);          // set element
  }

  // Cast to spherical data class
  PsSphereData<FLOATTYPE, NDIM>* spherePtr =
    dynamic_cast< PsSphereData<FLOATTYPE, NDIM>* >(ptclPtr);

  // Update center value
  posvec = spherePtr->getCenter();
  posvec = posvec + dr;
  this->getGridBase().mapPointToGrid(posvec);
  spherePtr->setCenter(posvec);
}

//
// Driver for movePtcl method. Attempt a move with dr
// check for overlaps... keep move if no overlap, reverse
// move if overlaps and return check.
// Also, tracks ptclData move info.
//
template <class FLOATTYPE, size_t NDIM>
bool PsSphere<FLOATTYPE, NDIM>::movePtclwCheck(
    PsBndryDataBase<FLOATTYPE, NDIM>* ptclPtr,
    PsTinyVector<int, NDIM> dr) {

  bool moveOK = true;

  // Move particle in ptclGroup
  movePtcl(ptclPtr, dr);

  // Check for overlaps...
  if (this->doesBndryOverlap(ptclPtr) ) {
    this->dbprt("Particle with index = ", (int)ptclPtr->getBndryIndex());
    this->dbprt("  made overlap, ");
    this->dbprt("  movePtclwCheck will reverse move");
    moveOK = false;
  }

  // If overlaps reverse particle move and bump counter
  if (moveOK) {
    return moveOK;
  }
  else {
    movePtcl(ptclPtr, -dr);
    return moveOK;
  }
}

//
// Dump particle center positions, needed for
// analysis and restart
//
template <class FLOATTYPE, size_t NDIM>
void PsSphere<FLOATTYPE, NDIM>::dump(TxIoBase* txIoPtr) {

  // Scoping call to base dump
  PsNanoPtcl<FLOATTYPE, NDIM>::dump(txIoPtr);

  this->dbprt("PsSphere::dump() ");

  // SWS: kludge for dump with no particles
  if ( this->ptclGroup.size() == 0) {
    dumpEmpty(txIoPtr); // Used to be called by node 0?
  }

  else {

    // Cell sizes for convert to 'real' position
    std::vector<FLOATTYPE> cellSizes = this->getGridBase().getCellSizes();

    // System sizes for convert to 'real' position
    std::vector<size_t> globalSizes =
      this->getGridBase().getNumCellsGlobal();
    size_t nz = globalSizes[2];

    // Data info
    size_t ptclGroupNum = this->ptclGroup.size();
    size_t ptclInfoDims = 6; // SWS: careful to change this
    FLOATTYPE* data = 0;
    data = new FLOATTYPE[ptclGroupNum*ptclInfoDims];

    // Format sphere center data
    for (size_t n=0; n<ptclGroupNum; ++n) {

      PsSphereData<FLOATTYPE, NDIM>* spherePtr =
        dynamic_cast< PsSphereData<FLOATTYPE, NDIM>* >(this->ptclGroup[n]);
      PsTinyVector<int, NDIM> centerVec = spherePtr->getCenter();
      size_t ptclIndx = n*ptclInfoDims;

      data[ptclIndx+0] = (FLOATTYPE)centerVec[0]*cellSizes[0];
      data[ptclIndx+1] = (FLOATTYPE)centerVec[1]*cellSizes[1];
      // SWS: this is kludge for 2D
      if (nz == 1)
        data[ptclIndx+2] = 1.0*cellSizes[2];
      else
        data[ptclIndx+2] = (FLOATTYPE)centerVec[2]*cellSizes[2];

      data[ptclIndx+3] = (FLOATTYPE)radius * cellSizes[0];
      data[ptclIndx+4] = (FLOATTYPE)radius * cellSizes[1];
      data[ptclIndx+5] = (FLOATTYPE)radius * cellSizes[2];
    }

    // Create/open filename
    std::string fileName = txIoPtr->getDumpFileName(this->getName());

    bool ptclFileExists = txIoPtr->fileExists(fileName);
    if (!ptclFileExists) {

      // **************************************************
      // txbase-IO data members

      // Global/local sizes
      std::vector<size_t> dataSize, dataLen, dataBeg;
      dataSize.push_back(ptclGroupNum);
      dataSize.push_back(ptclInfoDims);
      dataLen.push_back(ptclGroupNum);
      dataLen.push_back(ptclInfoDims);
      dataBeg.push_back(0);
      dataBeg.push_back(0);
      // **************************************************

      TxIoNodeType fn = txIoPtr->createFile(fileName);
      TxIoNodeType ds = txIoPtr->writeDataSet(fn, ptclDataSetName,
                                              dataSize, dataBeg, dataLen,
                                              data);
      txIoPtr->closeDataSet(ds);
      delete[] data;

      appendMetaDataset(txIoPtr, fn, ptclDataSetName);
      appendMetaGroup(txIoPtr, fn);
      txIoPtr->closeFile(fn);

    } // file exists
  } // else

  // For safety
  this->getCommBase().barrier();
}

//
// Dump particle center positions, this is temporary
//  method needed for Visit database files to be sync-ed
//
template <class FLOATTYPE, size_t NDIM>
void PsSphere<FLOATTYPE, NDIM>::dumpEmpty(TxIoBase* txIoPtr) {

  // Scoping call to base dump
  PsNanoPtcl<FLOATTYPE, NDIM>::dump(txIoPtr);

  this->dbprt("PsSphere::dump() ");

  // Cell sizes for convert to 'real' position
  std::vector<FLOATTYPE> cellSizes = this->getGridBase().getCellSizes();

  // Data info
  size_t ptclGroupNum = 1;
  size_t ptclInfoDims = 6; // SWS: careful to change this
  FLOATTYPE* data = 0;
  data = new FLOATTYPE[ptclGroupNum*ptclInfoDims];

  // Load "blank" particle
  data[0] = cellSizes[0];
  data[1] = cellSizes[1];
  data[2] = cellSizes[2];
  data[3] = 1.0 * cellSizes[0];
  data[4] = 0.0;
  data[5] = 0.0;

  // Create/open filename
  std::string fileName = txIoPtr->getDumpFileName(this->getName());

  // Check if filename already exists
  bool ptclFileExists = txIoPtr->fileExists(fileName);
  TxIoNodeType fn;

  if (!ptclFileExists) {

    // **************************************************
    // txbase-IO data members

    // Global/local sizes
    std::vector<size_t> dataSize, dataLen, dataBeg;
    dataSize.push_back(ptclGroupNum);
    dataSize.push_back(ptclInfoDims);
    dataLen.push_back(ptclGroupNum);
    dataLen.push_back(ptclInfoDims);
    dataBeg.push_back(0);
    dataBeg.push_back(0);
    // **************************************************

    fn = txIoPtr->createFile(fileName);
    TxIoNodeType ds = txIoPtr->writeDataSet(fn, ptclDataSetName,
                                            dataSize, dataBeg, dataLen,
                                            data);
    txIoPtr->closeDataSet(ds);
    delete[] data;

    // Append the vizschema attributes
    appendMetaDataset(txIoPtr, fn, ptclDataSetName);
    appendMetaGroup(txIoPtr, fn);
    txIoPtr->closeFile(fn);

  } // file exists
}

template <class FLOATTYPE, size_t NDIM>
void PsSphere<FLOATTYPE, NDIM>::appendMetaDataset(TxIoBase* txIoPtr,
                                                  TxIoNodeType fn,
                                                  std::string datasetName) {

  TxIoNodeType dn = txIoPtr->openDataSet(fn, datasetName);

  txIoPtr->writeAttribute(dn, "vsLimits", "limits");
  txIoPtr->writeAttribute(dn, "vsTime", "time");
  txIoPtr->writeAttribute(dn, "vsType", "variableWithMesh");
  txIoPtr->writeAttribute(dn, "vsNumSpatialDims", (int)NDIM);

  txIoPtr->closeDataSet(dn);
}

template <class FLOATTYPE, size_t NDIM>
void PsSphere<FLOATTYPE, NDIM>::appendMetaGroup(TxIoBase* txIoPtr,
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

  // appendTime
  size_t timeStep = PsDynObjBase::getCurrDomainStep();
  FLOATTYPE timeVal = (FLOATTYPE)timeStep;
  TxIoNodeType tgn = txIoPtr->createGroup(fn, "time");
  txIoPtr->writeAttribute(tgn, "vsType", "time");
  txIoPtr->writeAttribute(tgn, "vsStep", timeStep);
  txIoPtr->writeAttribute(tgn, "vsTime", timeVal);
}

// Instantiate classes
template class PsSphere<float, 1>;
template class PsSphere<float, 2>;
template class PsSphere<float, 3>;

template class PsSphere<double, 1>;
template class PsSphere<double, 2>;
template class PsSphere<double, 3>;

// From update()
  // ****************************************************
  // Particle sync check
  /*
  this->getCommBase().barrier();
  size_t thisRank = this->getCommBase().getRank();
  this->getCommBase().barrier();

  if ( thisRank == 0 ) {
    size_t numPtcls = this->ptclGroup.size();
      std::cout << " Particle group info from rank 0" << std::endl;
      std::cout << " Number of particles = " << numPtcls << std::endl;
    }
    for (size_t n=0; n<numPtcls; ++n) {
      PsBndryDataBase<FLOATTYPE, NDIM>* ptclPtr = this->ptclGroup[n];
      PsSphereData<FLOATTYPE, NDIM>* spherePtr =
        dynamic_cast< PsSphereData<FLOATTYPE, NDIM>* >(ptclPtr);
      PsTinyVector<int, NDIM> center = spherePtr->getCenter();
        std::cout << " center of particle " << n << " = " << center << std::endl;
      }
    }
  }

  this->getCommBase().barrier();
  if ( thisRank == 1 ) {
    size_t numPtcls = this->ptclGroup.size();
      std::cout << " Particle group info from rank 1" << std::endl;
      std::cout << " Number of particles = " << numPtcls << std::endl;
    }
    for (size_t n=0; n<numPtcls; ++n) {
      PsBndryDataBase<FLOATTYPE, NDIM>* ptclPtr = this->ptclGroup[n];
      PsSphereData<FLOATTYPE, NDIM>* spherePtr =
        dynamic_cast< PsSphereData<FLOATTYPE, NDIM>* >(ptclPtr);
      PsTinyVector<int, NDIM> center = spherePtr->getCenter();
        std::cout << " center of particle " << n << " = "
                  << center << std::endl;
      }
    }
  }
  this->getCommBase().barrier();
  */
  // ****************************************************
