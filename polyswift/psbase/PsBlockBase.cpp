/**
 *
 * @file    PsBlockBase.cpp
 *
 * @brief   Class containing block chain observables.
 *
 * @version $Id: PsBlockBase.cpp 8257 2007-09-12 22:05:20Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsBlockBase.h>
#include <PsPolymer.h>

#include <math.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsBlockBase<FLOATTYPE, NDIM>::PsBlockBase() {

  // Defaults are 1.0 (for monodisperse models)
  weight = 1.0;
  lengthWeight = 1.0;
  densityWeight = 1.0;

  // Default weight is 0 (for monodisperse models)
  ngIndex = 0;

  lengthFrac = 0.0;
  ds = 0.1;
  blockSteps = 0;
  forceBlockSteps = false;

  minBlockSteps = 4;

  qHeadInitialSet = 0;
  qTailInitialSet = 0;
  qHeadFinalSet   = 0;
  qTailFinalSet   = 0;

  polymerObjPtr = NULL;
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsBlockBase<FLOATTYPE, NDIM>::~PsBlockBase() {

  // Clear block lists
  headCntTo.clear();
  tailCntTo.clear();

  // Clear name lists
  headBlockNames.clear();
  tailBlockNames.clear();

  // Clear junction switch lists
  qHeadJntsSet.clear();
  qTailJntsSet.clear();
}

template <class FLOATTYPE, size_t NDIM>
void PsBlockBase<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Debug flag
  if (tas.hasString("printdebug")) {
    std::string dbPrint = tas.getString("printdebug");
    if (dbPrint == "on") this->dbStatus = PSDB_ON;
  }

  // Scoping call to base class
  PsFunctionalQ<FLOATTYPE, NDIM>::setAttrib(tas);
  this->dbprt("PsBlockBase::setAttrib() ");

  // Discretization size of segment length
  if (tas.hasParam("ds")) {
    ds = tas.getParam("ds");
  }
  else {
    TxDebugExcept tde("PsBlockBase::setAttrib: ds not set");
    tde << " in <Block " << this->getName() << " >";
    throw tde;
  }

  // Flag for forcing calculation of blockSteps
  if (tas.hasOption("forceBlockSteps")) {
    forceBlockSteps = tas.getOption("forceBlockSteps");
  }
  else {
    forceBlockSteps = false;
  }

  // Raw quad weight. Used by holder classes to calculate
  // density/length weights (Defaults to 1.0 for monodisperse models)
  if (tas.hasParam("weight")) {
     weight = tas.getParam("weight");
  }

  // Index of block contribution
  // Defaults to 0 for monodisperse
  if (tas.hasParam("ngIndex")) {
    ngIndex = tas.getParam("ngIndex");
  }

  // Fractional block length...must be consistent with other blocks
  if (tas.hasParam("lengthfrac")) {
    lengthFrac = tas.getParam("lengthfrac");
  }

  // List of block names a given block end is connected to
  if (tas.hasStrVec("headjoined")) {
    headBlockNames = tas.getStrVec("headjoined");
  }
  else {
    TxDebugExcept tde("PsBlockBase::setAttrib: headjoined not set");
    tde << " in <Block " << this->getName() << " >";
    throw tde;
  }

  // List of block names a given block end is connected to
  if (tas.hasStrVec("tailjoined")) {
    tailBlockNames = tas.getStrVec("tailjoined");
  }
  else {
    TxDebugExcept tde("PsBlockBase::setAttrib: tailjoined not set");
    tde << " in <Block " << this->getName() << " >";
    throw tde;
  }

  /*
  std::cout << "Block " << this->getName() << " has" << std::endl;
  for (size_t n=0; n<headBlockNames.size(); ++n) {
    std::cout << "  head block name = " << headBlockNames[n] << std::endl;
  }
  for (size_t n=0; n<tailBlockNames.size(); ++n) {
    std::cout << "  tail block name = " << tailBlockNames[n] << std::endl;
  }
  */

}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsBlockBase<FLOATTYPE, NDIM>::buildData() {

  // Scoping call to base class
  PsFunctionalQ<FLOATTYPE, NDIM>::buildData();

  this->dbprt("PsBlockBase::buildData() ");
  this->dbprt("Block ",this->getName(), " is inside polymer ");
  this->dbprt(polymerName);

}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsBlockBase<FLOATTYPE, NDIM>::buildSolvers() {

  // Scoping call to base class
  PsFunctionalQ<FLOATTYPE, NDIM>::buildSolvers();
  this->dbprt("PsBlockBase::buildSolvers() ");

  // Get containing polymer pointer
  polymerObjPtr = PsNamedObject::getObject<PsPolymer<FLOATTYPE, NDIM> >(polymerName);
  if (!polymerObjPtr) {
    TxDebugExcept tde("PsBlockBase::buildSolvers: Polymer ptr not set");
    tde << " in <PsBlockBase " << this->getName() << " >";
    throw tde;
  }

  // register name of block in physField list
  // SWS: need blocks in the chargeField registered?????
  this->monoDensPhysFldPtr->registerBlock(this->getName());

  {

  // Add contribution to density scaling factor for physField
  // and set related field pointers
  // FLOATTYPE phiAvg = lengthFrac*polymerObjPtr->getVolfrac();
  FLOATTYPE phiAvg =
    lengthWeight*lengthFrac*polymerObjPtr->getVolfrac();
  this->monoDensPhysFldPtr->addToDensAverage(phiAvg);

  // Number of steps along block (checking for integer)
  // Sets the length of propagator vectors
  FLOATTYPE lengthRatio = polymerObjPtr->getLengthRatio();
  FLOATTYPE bstmp = (lengthRatio*lengthFrac/ds);

  // Convert candidate num-of-block steps to appropriate
  // integer and do consistency checks
  blockSteps = setBlockSteps(bstmp);

  }

  // Set connectivity
  setCntToLists();
}

/*
 * This is the old setCntToLists: moved by creating
 * a static name list owned by PsBlockBase,
 * Set the connected block head/tail lists and junction switches
 * This sets the length of the map junction switches. The derived
 * block classes use this in buildSolvers() step to iterate over
 * the qXXXXJnts maps and set keys, and initial values
 */
template <class FLOATTYPE, size_t NDIM>
void PsBlockBase<FLOATTYPE, NDIM>::setCntToLists() {

  std::string cntStr;
  size_t cntIndex;
  BlockEndType blockEnd = HEAD; // Initialization choice

  // Set head block index connections
  for (size_t n=0; n<headBlockNames.size(); ++n) {

    cntStr = headBlockNames[n];

    if (cntStr != "freeEnd") {
      cntIndex = blockNameMap[cntStr];
      headCntTo.push_back(std::make_pair(cntIndex,blockEnd));
      qHeadJntsSet[cntIndex] = 0;
    }
  } // head loop

  // Set tail block index connections
  for (size_t n=0; n<tailBlockNames.size(); ++n) {

    cntStr = tailBlockNames[n];

    if (cntStr != "freeEnd") {
      cntIndex = blockNameMap[cntStr];
      tailCntTo.push_back(std::make_pair(cntIndex,blockEnd));
      qTailJntsSet[cntIndex] = 0;
    }
  } // tail loop

}

//
// Convert candidate num-of-block steps to appropriate
// integer and do consistency checks
//
template <class FLOATTYPE, size_t NDIM>
size_t PsBlockBase<FLOATTYPE, NDIM>::setBlockSteps(FLOATTYPE bs) {

  // appropriate number of blockSteps
  size_t bsInt;

  // Integer part of candidate number-of-block steps
  FLOATTYPE intPart = static_cast<FLOATTYPE>(static_cast<long int>(bs));

  // This is the non-integer part of the candidate number of blocksteps
  FLOATTYPE dx = bs - intPart;

  //
  // If forcing then the nearest integer is always taken
  // Default is forceBlockSteps = false. Logic below
  // implements nint() function
  //
  // "nint(1.03) --> floor(1.03+0.5)  = " << floor(1.03+0.5)
  // "nint(1.51) --> floor(1.51+0.5)  = " << floor(1.51+0.5)
  //
  if (forceBlockSteps) {
    bsInt = floor(bs+0.5);
    this->dbprt("Forcing block steps with nearest integer call");
  }

  else {

    // ************************************
    // Integer checks
    // ************************************
    if ( (dx > 0.0001) && (dx < 0.9999) ) {
      TxDebugExcept tde("PsBlockBase::setBlockSteps: non-integer # of block steps");
      tde << " blockSteps = " << bs;
      tde << " in <PsBlockBase " << this->getName() << " >";
      tde << "\n ceil(bs)  = " << ceil(bs);
      tde << "\n floor(bs) = " << floor(bs);
      throw tde;
    }

    else {
      size_t ceilSteps =  (size_t)ceil(bs);
      size_t floorSteps = (size_t)floor(bs);

      this->dbprt("... bs = ",          (int)bs);
      this->dbprt("... ceil(bs) = ",    (int)ceilSteps);
      this->dbprt("... floor(bs) = ",   (int)floorSteps);
      this->dbprt("... FLceil(bs) = ",  (FLOATTYPE)ceilSteps);
      this->dbprt("... FLfloor(bs) = ", (FLOATTYPE)floorSteps);

      if (ceilSteps == floorSteps) {
        bsInt = ceilSteps;
      }
      else if ( fabs(bs - (FLOATTYPE)ceilSteps) <
        fabs(bs - (FLOATTYPE)floorSteps) ) {
        bsInt = ceilSteps;
      }
      else {
        bsInt = floorSteps;
      }
    } // integer

  } // forceBlockSteps = false

  this->dbprt("... block steps = ",(int)bsInt);
  this->dbprt("for <Block ", this->getName());

  if ( bsInt < minBlockSteps) {
    TxDebugExcept tde("PsBlockBase::buildSolvers: # of block steps too small");
    tde << " blockSteps = " << bsInt;
    tde << " minBlockSteps = " << minBlockSteps;
    tde << " in <PsBlockBase " << this->getName() << " >";
    throw tde;
  }

  return bsInt;
}

template <class FLOATTYPE, size_t NDIM>
void PsBlockBase<FLOATTYPE, NDIM>::reset() {

  this->dbprt("PsBlockBase::reset() ");

  // Reset head/tail q values
  qHeadInitialSet = 0;
  qTailInitialSet = 0;
  qHeadFinalSet   = 0;
  qTailFinalSet   = 0;

  // Reset switches for head/tail junctions
  std::map<size_t, bool>::iterator pos;
  for (pos = qHeadJntsSet.begin(); pos != qHeadJntsSet.end(); ++pos)
    qHeadJntsSet[pos->first] = 0;
  for (pos = qTailJntsSet.begin(); pos != qTailJntsSet.end(); ++pos)
    qTailJntsSet[pos->first] = 0;
}

//
// get connected head/tail list
//
template <class FLOATTYPE, size_t NDIM>
std::vector<std::pair<size_t, BlockEndType> >
PsBlockBase<FLOATTYPE, NDIM>::getCntTo(BlockEndType end) {
  if (end == HEAD) {
    return headCntTo;
  }
  else {
    return tailCntTo;
  }
}

//
// check if initial/final Q set
//
template <class FLOATTYPE, size_t NDIM>
bool PsBlockBase<FLOATTYPE, NDIM>::isQSet(BcType bc, BlockEndType end) {

  if (bc == INITIAL) {
    if (end == HEAD) return qHeadInitialSet;
    else             return qTailInitialSet;
  }

  else {
    if (end == HEAD) return qHeadFinalSet;
    else             return qTailFinalSet;
  }

}

//
template <class FLOATTYPE, size_t NDIM>
void PsBlockBase<FLOATTYPE, NDIM>::buildCntToTail(BlockEndType end, size_t nblock) {

  size_t iblock;
  if (end == HEAD) {
    iblock = headCntTo[nblock].first;
    headCntTo[nblock] = std::make_pair(iblock,TAIL);
  }
  else {
    iblock = tailCntTo[nblock].first;
    tailCntTo[nblock] = std::make_pair(iblock,TAIL);
  }
}

template <class FLOATTYPE, size_t NDIM>
bool PsBlockBase<FLOATTYPE, NDIM>::areJntsSet(BlockEndType end) {

  std::map<size_t, bool>::iterator pos;
  bool allQSet = 1; // initial set to yes

  if (end == HEAD) {
    for (pos = qHeadJntsSet.begin(); pos != qHeadJntsSet.end(); ++pos) {
      if (!pos->second) allQSet = 0;
    }
    return allQSet;
  }

  else {
    for (pos = qTailJntsSet.begin(); pos != qTailJntsSet.end(); ++pos) {
      if (!pos->second) allQSet = 0;
    }
    return allQSet;
  }
}

template <class FLOATTYPE, size_t NDIM>
bool PsBlockBase<FLOATTYPE, NDIM>::isBlockUpdated() {
  bool updated = 0;
  if (qHeadFinalSet && qTailFinalSet) updated = 1;
  return updated;
}

template <class FLOATTYPE, size_t NDIM>
void PsBlockBase<FLOATTYPE, NDIM>::buildNameMap(std::vector<std::string> blockNames) {
  size_t numNames = blockNames.size();
  for (size_t n=0; n<numNames; ++n) {
    blockNameMap[blockNames[n]] = n;
  }
}

// block name map
template <class FLOATTYPE, size_t NDIM>
std::map<std::string, size_t> PsBlockBase<FLOATTYPE, NDIM>::blockNameMap;

// Instantiate classes
template class PsBlockBase<float, 1>;
template class PsBlockBase<float, 2>;
template class PsBlockBase<float, 3>;

template class PsBlockBase<double, 1>;
template class PsBlockBase<double, 2>;
template class PsBlockBase<double, 3>;
