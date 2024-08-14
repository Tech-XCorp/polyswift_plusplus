/**
 * @file    PsPolyDisperseBCP.cpp
 *
 * @brief   Class for polydisperse block polymer
 *
 * @version $Id: PsPolyDisperseBCP.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// pspolymer includes
#include <PsPolyDisperseBCP.h>

// txbase includes
#include <TxTensor.h>
#include <TxMakerMap.h>

// std includes
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsPolyDisperseBCP<FLOATTYPE, NDIM>::PsPolyDisperseBCP() {
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsPolyDisperseBCP<FLOATTYPE, NDIM>::~PsPolyDisperseBCP() {
  delete quadGL;
}

//
// Re-implementing PsBlockCopolymer::setAttrib.
// Still calling PsPolymer::setAttrib
//
template <class FLOATTYPE, size_t NDIM>
void PsPolyDisperseBCP<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Scoping call to base class
  PsPolymer<FLOATTYPE, NDIM>::setAttrib(tas);
  this->dbprt("PsPolyDisperseBCP::setAttrib() ");

  // Parse <Block>-s
  seedBlockNames = tas.getNamesOfType("Block");
  numSeedBlocks  = seedBlockNames.size();

  // Check model
  if (numSeedBlocks > 2) {
    TxDebugExcept tde("PsPolyDisperseBCP::setAttrib ");
    tde << "polydisperse model with more than two blocks not yet supported";
    throw tde;
  }

  // Store block attribus
  for (size_t n=0; n<numSeedBlocks; ++n) {
    TxHierAttribSetIntDbl attribs = tas.getAttrib(seedBlockNames[n]);
    seedBlockAttribs.push_back(attribs);
    this->pprt("Found <Block ", seedBlockNames[n], " >");
  }

  // alpha exponent in Schulz distribution
  if (tas.hasParam("alpha")) {
    alpha = tas.getParam("alpha");
  }
  else {
    TxDebugExcept tde("PsPolyDisperseBCP::setAttrib alpha not set");
    throw tde;
  }

  // number of quadrature points
  if (tas.hasOption("n_g")) {
    n_g = tas.getOption("n_g");
  }
  else {
    TxDebugExcept tde("PsPolyDisperseBCP::setAttrib n_g not set");
    throw tde;
  }

  // Name of block that is polydisperse
  if (tas.hasString("polyblock")) {
    polyBlockName = tas.getString("polyblock");
  }
  else {
    TxDebugExcept tde("PsPolyDisperseBCP::setAttrib");
    tde << "polyblock not set: Name of polydisperse block must be set";
    throw tde;
  }

  //
  // Build raw abscissas and weights
  // Loop through blocks 'global' polydispersity variables
  // (using one polydis block model) Sets mk_vec and fmk_vec
  //
  quadGL = new PsGaussLagQuad(n_g, alpha-1.0);
  for (size_t n=0; n<numSeedBlocks; ++n) {
    TxHierAttribSetIntDbl blockAttrib = tas.getAttrib(seedBlockNames[n]);
    if (isAttribPoly(blockAttrib)) calcPolyParams(blockAttrib);
  }

  //
  // Create quad block info 'copies' and assign to BlockGrp's
  //
  for (size_t n=0; n<numSeedBlocks; ++n) {

    // Copy original <Block> attribute
    std::string blkName = seedBlockNames[n];
    TxHierAttribSetIntDbl blockAttrib = tas.getAttrib(blkName);

    // For storing blk name/attrib associations
    BlockGrp bG;
    bG.origName   = blkName;
    bG.origAttrib = blockAttrib;

    // Loop on quadrature points
    for (size_t quadIndex=0; quadIndex<n_g; quadIndex++) {

      std::string quadBlockName  = makeQuadName(blkName, quadIndex);
      TxHierAttribSetIntDbl quadAttrib = makeQuadAttrib(blockAttrib, quadIndex);
      FLOATTYPE bigQInit = 1.0;
      PsBlockBase<FLOATTYPE, NDIM>* quadBlk=NULL;

      quadBlockNames.push_back(quadBlockName);
      quadAttrib.setObjectName(quadBlockName);
      quadBlockAttribs.push_back(quadAttrib);

      bG.quadNames.push_back(quadBlockName);
      bG.quadAttribs.push_back(quadAttrib);
      bG.quadBigQs.push_back(bigQInit);
      bG.quadBlkPtrs.push_back(quadBlk);
    } // loop quad pts

    blockGroups.push_back(bG);     // Save blk info
  } // loop on blocks

  // Reset parent data members
  this->numBlocks    = numSeedBlocks*n_g;
  this->blockNames   = quadBlockNames;
  this->blockAttribs = quadBlockAttribs;
}

//
template <class FLOATTYPE, size_t NDIM>
void PsPolyDisperseBCP<FLOATTYPE, NDIM>::buildData() {

  // Scoping call to base class
  PsPolymer<FLOATTYPE, NDIM>::buildData();
  this->dbprt("PsPolyDisperseBCP::buildData() ");

  //
  // Loop on block groups and create PsBlockBase objects
  //
  for (size_t ig=0; ig<blockGroups.size(); ++ig) {

    BlockGrp bG = blockGroups[ig];
    std::vector<std::string> quadNames                       = bG.quadNames;
    std::vector<TxHierAttribSetIntDbl> quadAttribs                 = bG.quadAttribs;
    std::vector< PsBlockBase<FLOATTYPE, NDIM>* > quadBlkPtrs = bG.quadBlkPtrs;

    // Loop on quad points in each group
    for (size_t iq=0; iq<quadBlkPtrs.size(); ++iq) {

      // For each block look for kind
      std::string kind = quadAttribs[iq].getString("kind");

      // Have makermap return a pointer to a PsBlock object
      PsBlockBase<FLOATTYPE, NDIM>* blockPtr =
        TxMakerMap< PsBlockBase<FLOATTYPE, NDIM> >::getNew(kind);
      if (!blockPtr) {
        TxDebugExcept tde("PsPolyDisperseBCP::buildData: Block ptr not set");
        tde << " in <Polymer " << this->getName() << " >";
        throw tde;
      }

      // Set block/name info, register name
      blockPtr->setName(quadNames[iq]);
      blockPtr->setDomainSingletons( this->getDomainSingletons() );
      blockPtr->setAttrib(quadAttribs[iq]);
      blockPtr->setOwner(this->getOwner());
      // SWS: should this be owner set?
      this->getNonConstOwner()->makeAvail(blockPtr, quadNames[iq]);

      // Set name of container polymer in block before buildData
      blockPtr->setPolymerName(this->getName());

      // Build data call for specific block
      // Sets size of q(X) so other parts of blocks can be built
      blockPtr->buildData();

      // Push pointer into blocks structure in base class
      this->blocks.push_back(blockPtr);

      // Check on block group index
      size_t ngIndex = quadAttribs[iq].getOption("ngIndex");
      if (ngIndex != iq) {
        TxDebugExcept tde("PsPolyDisperseBCP::buildData");
        tde << " BlkGroup index doesnt match attrib set";
        throw tde;
      }

      // Store block pointer in correct place in BlkGroup
      bG.quadBlkPtrs[iq] = blockPtr;
    }

    // Re-assign to block group to vector
    blockGroups[ig] = bG;
  }

  // Check blk info
  checkBlkGroups();

  //
  // This container class holds information about the overall
  // polydispersity chain model. Therefore it must be the object
  // that sets the length/density weighting factors (for monomdisperse
  // models these are always 1.0)
  //
  // Second loop after all attributes set above to set
  //  - length weight average factor
  //  - density weight factor
  //
  // The density weight factor is the quadrtaure wt scaled
  // by the gamma[alpha] factor.
  //
  // The length weight factor is different for each block.
  //   - for the fixed block in polydisperse model its 1/ng
  //   - for the poly block its the density weight factor
  //
  for (size_t n=0; n<this->blocks.size(); ++n) {
    PsBlockBase<FLOATTYPE, NDIM>* blockPtr = this->blocks[n];
    std::string blkName = blockPtr->getName();
    FLOATTYPE wt        = blockPtr->getWeight();
    FLOATTYPE lengthWt  = calcLengthWeight(blkName, wt);
    FLOATTYPE densityWt = wt/(FLOATTYPE)quadGL->psGamma(alpha);
    blockPtr->setLengthWeight(lengthWt);
    blockPtr->setDensityWeight(densityWt);
  }

  // Set static block-namemap in base class
  PsBlockBase<FLOATTYPE, NDIM>::buildNameMap(this->blockNames);

  //
  // Length fraction checks
  // Uses quadrature sum to find average length frac
  // for the polydisperse block and adds to other blocks
  //
  this->pprt("Performing length frac sum check");
  FLOATTYPE totalFrac=0.0;

  for (size_t i=0; i<this->numBlocks; ++i) {

    PsBlockBase<FLOATTYPE, NDIM>* blockPtr = this->blocks[i];
    std::string blkName = blockPtr->getName();
    this->pprt("  summing block ", blkName);

    // For polydisperse block using quadrature sum
    FLOATTYPE lenWt = blockPtr->getLengthWeight();
    FLOATTYPE fmk   = blockPtr->getLengthFrac();
    totalFrac = totalFrac + (fmk*lenWt);
  }
  this->pprt("totalFrac sum = ", totalFrac);

  FLOATTYPE tolerance = 0.00001;
  FLOATTYPE fracTarget = 1.0;
  if ( fabs(totalFrac - fracTarget) > tolerance) {
    TxDebugExcept tde("PsPolyDisperseBCP::buildData: |totalFrac - 1.0| ");
    tde << fabs(totalFrac - fracTarget) << " > tolerance";
    tde << " in <Polymer " << this->getName() << " >";
    throw tde;
  }
}

//
// Get the natural-log of the single-chain partition function
// normalization value. For polydispserse BCP this is a weighted
// sum of log(Q) 's
//
template <class FLOATTYPE, size_t NDIM>
FLOATTYPE PsPolyDisperseBCP<FLOATTYPE, NDIM>::getLogBigQ() {

  FLOATTYPE sumLogBigQ = 0.0;
  FLOATTYPE galpha = quadGL->psGamma(alpha);

  this->dbprt("PsPolyDisperseBCP::getLogBigQ, using 1st block grp");

  BlockGrp bG = blockGroups[0];
  std::string origName               = bG.origName;
  std::vector<std::string> quadNames = bG.quadNames;

  for (size_t n=0; n<quadNames.size(); ++n) {
    FLOATTYPE wk = quadGL->getWeight(n);
    FLOATTYPE bigQ = bG.quadBigQs[n];
    //    std::cout << "wt in getLogBigQ [" << n << "] = " << wk
    //        << " bigQ = " << bigQ << std::endl;
    sumLogBigQ =
      sumLogBigQ + ((FLOATTYPE)std::log(bigQ) * wk / galpha);
  }

  return sumLogBigQ;
}

//
// Initializes block data, solves for all propagators
// calls q(X,s)*qt(X,s) over ds method and Q normalization factor
//
template <class FLOATTYPE, size_t NDIM>
void PsPolyDisperseBCP<FLOATTYPE, NDIM>::update(double t) {

  // Base class update
  PsPolymer<FLOATTYPE, NDIM>::update(t);
  this->dbprt("PsPolyDisperseBCP::update() ");

  // Local block pointer
  //  PsBlockBase<FLOATTYPE, NDIM>* blockPtr;

  // ResetsInitialize all blocks -- resets data/switches before update
  for (size_t i=0; i<this->numBlocks; ++i) {
    this->blocks[i]->reset();
  }

  /*
   * Continue looping over blocks until all propagators are
   * calculated. Driver for helper method, updateBlockQ, which
   * performs all checks and publishing Q results
   */
  while (!this->areAllBlocksUpdated() ) {
    for (size_t nblock=0; nblock<this->numBlocks; ++nblock) {
      // Series of "blockPtr->solveQ(end)"
      this->updateBlockQ(nblock,HEAD);
      this->updateBlockQ(nblock,TAIL);
    }
  }

  /*
   * Calculate normalization Q value for an arbitrary block.
   * SWS: CAREFUL... subtlely different with branches etc.?
   */
  for (size_t ig=0; ig<blockGroups.size(); ++ig) {

    BlockGrp bG = blockGroups[ig];
    for (size_t iq=0; iq<bG.quadBigQs.size(); ++iq) {
      FLOATTYPE bigQ = bG.quadBlkPtrs[iq]->calcBigQ();
      bG.quadBigQs[iq] = bigQ;
    }

    blockGroups[ig] = bG; // Re-assign to block grp vec
  }

  /*
   * After updating ALL propagators above calculate int [ q(X,s)*qt(X,s) ds ]
   * Calculated in a separate step, because many observables use this quantity
   */
  for (size_t ig=0; ig<blockGroups.size(); ++ig) {

    BlockGrp bG = blockGroups[ig];
    for (size_t iq=0; iq<bG.quadBigQs.size(); ++iq) {
      FLOATTYPE bigQ = bG.quadBigQs[iq];
      bG.quadBlkPtrs[iq]->setCalcQQTIntegral(bigQ);
    }
  }

  /*
   * One of the methods that uses int [ q(X,s)*qt(X,s) ds ]
   * result. Calculates appropriate densities and
   * sets those values in the held PhysFields
   */
  for (size_t i=0; i<this->numBlocks; ++i)
    this->blocks[i]->setPhysFields();
}

          // *************************** //
          //   Private helper methods    //
          // *************************** //

//
// Changes only lengthfrac for the polydisperse block
// Abscissas and weights should already be calculated
//
template <class FLOATTYPE, size_t NDIM>
void
PsPolyDisperseBCP<FLOATTYPE, NDIM>::calcPolyParams(
       const TxHierAttribSetIntDbl& blockAttrib) {

  // Get the <lengthfrac> for the polydispsere block
  FLOATTYPE polyLengthFrac;
  if (blockAttrib.hasParam("lengthfrac")) {
    polyLengthFrac = blockAttrib.getParam("lengthfrac");
  }
  else {
    TxDebugExcept tde("PsPolyDispserseBCP::calcPolyParams ");
    tde << "lengthfrac not found";
    throw tde;
  }

  // overall average length of polymer container object
  FLOATTYPE nLen = (FLOATTYPE)this->getPolymerLength();

  // Length of non-polydisperse blocks
  FLOATTYPE constLengthFrac = 1.0 - polyLengthFrac;
  FLOATTYPE Nc  = constLengthFrac*nLen;
  FLOATTYPE n_b = (nLen - Nc)/alpha;
  FLOATTYPE pdi = (alpha + 1.0) / alpha;

  //
  // Scaled length values for quadrature points
  //   mk = ( y * n_b + N_A)/N_n or  mk = ( xk * n_b + N_A)/N_n
  //   Note: N_A here is the constant length of non-polydisperse blocks Nc
  //   fmk = length fraction of polydisperse block * mk
  //
  for (size_t n=0; n<n_g; n++) {
    FLOATTYPE xk = quadGL->getAbscissa(n);
    FLOATTYPE wk = quadGL->getWeight(n);
    FLOATTYPE mk  = (xk*n_b + Nc)/nLen;
    //    FLOATTYPE fmk = polyLengthFrac*(xk*n_b + Nc)/nLen;
    FLOATTYPE fmk = ((xk*n_b + Nc)/nLen) - constLengthFrac;
    mk_vec.push_back(mk);
    fmk_vec.push_back(fmk);
    // std::cout << "mk[" << n << "]  = " << mk  << std::endl;
    // std::cout << "fmk[" << n << "] = " << fmk << "\n" << std::endl;
  }

  this->pprt("\n");
  this->pprt(" ------------ PolyDisperse Model Parameters ---------- ");
  this->pprt(" Model detail: one constant block, one polydis block   ");
  this->pprt("  \n");
  this->pprt("  Nc of constant block                         = ", Nc);
  this->pprt("  n_b (Schultz length parameter                = ", n_b);
  this->pprt("  I_pdi (polydispersity index: poly block only = ", pdi);

  for (size_t n=0; n<n_g; n++) {

    FLOATTYPE fmk = fmk_vec[n];
    FLOATTYPE fracLenPoly = fmk*nLen;

    /*
    std::string nStr =
      static_cast<std::ostringstream*>(&(std::ostringstream()<< n) )->str();
    std::string fracStr =
      static_cast<std::ostringstream*>(&(std::ostringstream()<< fracLenPoly) )->str();
    */

    std::string nStr    = std::to_string(n);
    std::string fracStr = std::to_string(fracLenPoly);

    this->pprt("  Effective N for polydis blk ", nStr, " ", fracStr);
  }

  this->pprt("\n");
  this->pprt(" ----------------------------------------------------- ");

}

template <class FLOATTYPE, size_t NDIM>
FLOATTYPE
PsPolyDisperseBCP<FLOATTYPE, NDIM>::calcLengthWeight(const std::string& blkName,
                                                     FLOATTYPE wt) {
  // local variable
  FLOATTYPE correctionFactor;

  // For polydisperse block, correction is quadrature
  // weight and distribution normalization factor
  // For now tied to distribution functional form
  size_t found = blkName.find(polyBlockName);
  if (found!=std::string::npos) {
    this->dbprt("using polydis correct for block ", blkName);
    correctionFactor = wt/(FLOATTYPE) quadGL->psGamma(alpha);
  }

  // For the constant length block in polydisperse chain
  // this factor can be used to weight average density correctly
  else {
    this->dbprt("using 1/n_g correct for block ", blkName);
    correctionFactor = 1.0/((FLOATTYPE)n_g);
  }

  return correctionFactor;
}

//
// Creating a new attribute set for a quadrature contribution for the
// quadIndex-th block. Changes the following attributes
//
//  - ngIndex
//  - weight
//  - headjoined
//  - tailjoined
//  - lengthfrac (for the polydisperse block)
//
// SWS: TxHier txbase refactor change
template <class FLOATTYPE, size_t NDIM>
TxHierAttribSetIntDbl
PsPolyDisperseBCP<FLOATTYPE, NDIM>::makeQuadAttrib(const TxHierAttribSetIntDbl& blockAttrib,
                                                   size_t quadIndex) {

  // Make copy of block attributes for new quadrature point attribute
  // This is returned at end of method
  TxHierAttribSetIntDbl quadAttrib = blockAttrib;

  // Local lists
  std::vector<std::string> headVec;
  std::vector<std::string> tailVec;

  // Get head/tailjoined lists
  if (quadAttrib.hasStrVec("headjoined")) {
    headVec = quadAttrib.getStrVec("headjoined");
  }
  else {
    TxDebugExcept tde("PsPolyDispserseBCP::makeQuadAttrib headjoined parse error");
    throw tde;
  }
  if (quadAttrib.hasStrVec("tailjoined")) {
    tailVec = quadAttrib.getStrVec("tailjoined");
  }
  else {
    TxDebugExcept tde("PsPolyDispserseBCP::makeQuadAttrib tailjoined parse error");
    throw tde;
  }

  // Local new vecs
  std::vector<std::string> quadHeadVec;
  std::vector<std::string> quadTailVec;

  // Loop over names in headJoined strVec and convert, eg blockA --> blockA_ng_0
  for (size_t n=0; n<headVec.size(); n++) {
    std::string cntBlock = headVec[n];
    if (cntBlock == "freeEnd") {
      quadHeadVec.push_back(cntBlock);
      continue;
    }
    else {
      std::string quadBlockName = makeQuadName(cntBlock, quadIndex);
      quadHeadVec.push_back(quadBlockName);
    }
  }

  // Loop over names in tailJoined strVec and convert
  for (size_t n=0; n<tailVec.size(); n++) {
    std::string cntBlock = tailVec[n];
    if (cntBlock == "freeEnd") {
      quadTailVec.push_back(cntBlock);
      continue;
    }
    else {
      std::string quadBlockName = makeQuadName(cntBlock, quadIndex);
      quadTailVec.push_back(quadBlockName);
    }
  }

  // Block density tracking string
  std::string blockfield;
  if (quadAttrib.hasString("blockfield")) {
    std::string fldStr = quadAttrib.getString("blockfield");
    blockfield = makeQuadName(fldStr, quadIndex);
    quadAttrib.removeString("blockfield");
    bool good = quadAttrib.appendString("blockfield", blockfield);
    if (!good) {
      TxDebugExcept tde("PsPolyDispserseBCP::makeQuadAttrib blockfield parse error");
      throw tde;
    }
  }

  //
  // Edit new attrib set
  //
  bool good;
  quadAttrib.removeOption("ngIndex");
  quadAttrib.removeParam("weight");
  quadAttrib.removeStrVec("headjoined");
  quadAttrib.removeStrVec("tailjoined");

  good = quadAttrib.appendOption("ngIndex", quadIndex);
  if (!good) {
    TxDebugExcept tde("PsPolyDispserseBCP::makeQuadAttrib ngIndex append failed");
    throw tde;
  }

  double wi = quadGL->getWeight(quadIndex);
  good = quadAttrib.appendParam("weight", wi);
  if (!good) {
    TxDebugExcept tde("PsPolyDispserseBCP::makeQuadAttrib weight append failed");
    throw tde;
  }

  good = quadAttrib.appendStrVec("headjoined", quadHeadVec);
  if (!good) {
    TxDebugExcept tde("PsPolyDispserseBCP::makeQuadAttrib headjoined append failed");
    throw tde;
  }

  good = quadAttrib.appendStrVec("tailjoined", quadTailVec);
  if (!good) {
    TxDebugExcept tde("PsPolyDispserseBCP::makeQuadAttrib tailjoined append failed");
    throw tde;
  }

  //
  // For polydisperse block, adjust lengthfrac
  // with scaled lengths from calcPolyParams(..) calls
  //
  if (isAttribPoly(quadAttrib)) {

    FLOATTYPE fmk = fmk_vec[quadIndex];
    quadAttrib.removeParam("lengthfrac");
    bool good = quadAttrib.appendParam("lengthfrac", fmk);
    if (!good) {
      TxDebugExcept tde("PsPolyDispserseBCP::makeQuadAttrib lengthfrac append failed");
      throw tde;
    }
  }

  return quadAttrib;
}

//
// eg  string convert blockA ----> blockA_ng_0
//
template <class FLOATTYPE, size_t NDIM>
std::string
PsPolyDisperseBCP<FLOATTYPE, NDIM>::makeQuadName(const std::string& objName,
                                                 size_t index) {

  //  std::string quadIndexStr =
  //    static_cast<std::ostringstream*>(&(std::ostringstream()<< index) )->str();

  std::string quadIndexStr = std::to_string(index);
  std::string quadName = objName + "_ng_" + quadIndexStr;

  return quadName;
}

//
// assumes quad block names constructed in this class
//
template <class FLOATTYPE, size_t NDIM>
bool PsPolyDisperseBCP<FLOATTYPE, NDIM>::isAttribPoly(const TxHierAttribSetIntDbl& attrib) {

  bool isPoly = false;
  std::string blkName = attrib.getObjectName();

  size_t found = blkName.find(polyBlockName);
  if (found!=std::string::npos) {
    isPoly = true;
    this->dbprt("Block ", blkName, " is polydisperse");
  }

  return isPoly;
}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsPolyDisperseBCP<FLOATTYPE,NDIM>::checkBlkGroups() {

  for (size_t n=0; n<quadBlockNames.size(); ++n) {

    //    std::string quadIndexStr =
    //      static_cast<std::ostringstream*>(&(std::ostringstream()<< n) )->str();

    std::string quadIndexStr = std::to_string(n);
    this->pprt("quadBlockName [ ", quadIndexStr, " ] = ", quadBlockNames[n]);
  }

  // File buffer
  std::filebuf fb;

  // Dump attributes to files
  for (size_t ig=0; ig<blockGroups.size(); ++ig) {

    BlockGrp bG = blockGroups[ig];
    std::string origName                     = bG.origName;
    std::vector<std::string> quadNames       = bG.quadNames;
    std::vector<TxHierAttribSetIntDbl> quadAttribs = bG.quadAttribs;

    //
    // Block group info to screen
    //
    this->pprt("Seed block ", origName, " contains:");
    for (size_t iq=0; iq<quadNames.size(); ++iq) {
      this->pprt("... Quad block ", quadNames[iq]);
    }

    // Setup file for orig block
    std::string fileName = origName + ".txt";
    fb.open(fileName.c_str(), std::ios::out);
    std::ostream os(&fb);

    // Loop on quad points
    for (size_t iq=0; iq<quadNames.size(); ++iq) {

      TxHierAttribSetIntDbl quadAttrib = quadAttribs[iq];
      std::string quadName = quadNames[iq];
      PsBlockBase<FLOATTYPE, NDIM>* blockPtr = bG.quadBlkPtrs[iq];
      std::string blockName = blockPtr->getName();
      os << "<Block " << quadName << " > ---> " << blockName << " \n";
      quadAttrib.writeBody(os, 1, 0);
      os << "</Block> \n \n";
    }

    fb.close();
  }

}

// Instantiate
template class PsPolyDisperseBCP<float, 1>;
template class PsPolyDisperseBCP<float, 2>;
template class PsPolyDisperseBCP<float, 3>;

template class PsPolyDisperseBCP<double, 1>;
template class PsPolyDisperseBCP<double, 2>;
template class PsPolyDisperseBCP<double, 3>;
