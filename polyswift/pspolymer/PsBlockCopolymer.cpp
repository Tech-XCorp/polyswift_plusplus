/**
 * @file    PsBlockCopolymer.cpp
 *
 * @brief   Class for block polymer
 *
 * @version $Id: PsBlockCopolymer.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// pspolymer includes
#include <PsBlockCopolymer.h>

// txbase includes
#include <TxTensor.h>
#include <TxMakerMap.h>

// std includes
#include <string>
#include <sstream>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsBlockCopolymer<FLOATTYPE, NDIM>::PsBlockCopolymer() {}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsBlockCopolymer<FLOATTYPE, NDIM>::~PsBlockCopolymer() {

  for (size_t i=0; i<blocks.size(); ++i)
    delete blocks[i];

  numBlocks = 0;

  blocks.clear();
  blockNames.clear();
  blockAttribs.clear();
}

//
// No calls to PsBlocks
//
template <class FLOATTYPE, size_t NDIM>
void
PsBlockCopolymer<FLOATTYPE, NDIM>::
setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Scoping call to base class
  PsPolymer<FLOATTYPE, NDIM>::setAttrib(tas);
  this->dbprt("PsBlockCopolymer::setAttrib() ");

   // Parse input parameters specific for block copolymers
  blockNames = tas.getNamesOfType("Block");
  numBlocks = blockNames.size();

  //
  for (size_t n=0; n<numBlocks; ++n) {
    TxHierAttribSetIntDbl attribs = tas.getAttrib(blockNames[n]);
    blockAttribs.push_back(attribs);
    this->pprt("Found <Block ", blockNames[n], " >");
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsBlockCopolymer<FLOATTYPE, NDIM>::buildData() {

  // Scoping call to base class
  PsPolymer<FLOATTYPE, NDIM>::buildData();
  this->dbprt("PsBlockCopolymer::buildData() ");

  for (size_t i=0; i<numBlocks; ++i) {

    // For each block look for kind
    std::string kind = blockAttribs[i].getString("kind");

    // Have makermap return a pointer to a PsBlock object
    PsBlockBase<FLOATTYPE, NDIM>* blockPtr =
      TxMakerMap< PsBlockBase<FLOATTYPE, NDIM> >::getNew(kind);
    if (!blockPtr) {
      TxDebugExcept tde("PsBlockCopolymer::buildData: Block pointer not set");
      tde << " in <Polymer " << this->getName() << " >";
      throw tde;
    }

    // Set parameters and attributes for particular block
    // and make availible for lookup by name
    blockPtr->setName(blockNames[i]);
    blockPtr->setDomainSingletons( this->getDomainSingletons() );
    blockPtr->setAttrib(blockAttribs[i]);

    // Set name/regis
    blockPtr->setOwner(this->getOwner() );
    // SWS: should this be owner set?
    this->getNonConstOwner()->makeAvail(blockPtr, blockNames[i]);

    // Set name of container polymer in block before buildData
    blockPtr->setPolymerName(this->getName());

    // Build data call for specific block
    // Sets size of q(X) so other parts of blocks can be built
    blockPtr->buildData();

    // Push pointer into blocks structure
    blocks.push_back(blockPtr);
  }

  // Set static block-namemap in base class
  PsBlockBase<FLOATTYPE, NDIM>::buildNameMap(blockNames);

  // Perform checks on length fractions
  FLOATTYPE totalFrac=0.0;
  PsBlockBase<FLOATTYPE, NDIM>* blockPtr;
  for (size_t i=0; i<numBlocks; ++i) {
    blockPtr = blocks[i];
    totalFrac = totalFrac + blockPtr->getLengthFrac();
  }

  FLOATTYPE tolerance = 0.00001;
  FLOATTYPE fracTarget = 1.0;
  if ( fabs(totalFrac - fracTarget) > tolerance) {
    TxDebugExcept tde("PsBlockCopolymer::buildData: |totalFrac - 1.0| ");
    tde << fabs(totalFrac - fracTarget) << " > tolerance";
    tde << " in <Polymer " << this->getName() << " >";
    throw tde;
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsBlockCopolymer<FLOATTYPE, NDIM>::buildSolvers() {

  // Scoping call to base class
  PsPolymer<FLOATTYPE, NDIM>::buildSolvers();
  this->dbprt("PsBlockCopolymer::buildSolvers() ");

   // Build solvers for blocks
  for (size_t i=0; i<numBlocks; ++i) {
    PsBlockBase<FLOATTYPE, NDIM>* blockPtr = blocks[i];
    blockPtr->buildSolvers();
  }

  // Set HEAD/TAIL info in connected-to lists
  // must be done after all blocks processed and built
  for (size_t iblock=0; iblock<numBlocks; ++iblock) {
    buildCntBlockType(iblock,HEAD);
    buildCntBlockType(iblock,TAIL);
  }

}

//
// use block methods to set the
// head/tail connected lists for each block
//
template <class FLOATTYPE, size_t NDIM>
void PsBlockCopolymer<FLOATTYPE, NDIM>::buildCntBlockType(
     size_t iblock, BlockEndType end) {

  // Only checks TAIL because cntTo initialized to HEAD

  // Temp connected variable
  size_t cntToIndx;
  std::vector<std::pair<size_t, BlockEndType> >
    cntToBlocks, cntFromBlocks;

  cntToBlocks = blocks[iblock]->getCntTo(end);

  // loop over cnt-ed blocks
  for (size_t n=0; n<cntToBlocks.size(); ++n) {

    cntToIndx = cntToBlocks[n].first;
    cntFromBlocks = blocks[cntToIndx]->getCntTo(TAIL);

    for (size_t k=0; k<cntFromBlocks.size(); ++k) {
      size_t cntFromIndx = cntFromBlocks[k].first;
      if (cntFromIndx == iblock) blocks[iblock]->buildCntToTail(end,n);
    }
  } // connected end loop

}

//
// Initializes block data, solves for all propagators
// calls q(X,s)*qt(X,s) over ds method and Q normalization factor
//
template <class FLOATTYPE, size_t NDIM>
void PsBlockCopolymer<FLOATTYPE, NDIM>::update(double t) {

  // Base class update
  PsPolymer<FLOATTYPE, NDIM>::update(t);
  this->dbprt("PsBlockCopolymer::update() ");

  // Local block pointer
  PsBlockBase<FLOATTYPE, NDIM>* blockPtr;

  // ResetsInitialize all blocks -- resets data/switches before update
  for (size_t i=0; i<numBlocks; ++i) {
    blocks[i]->reset();
  }

  /*
   * Continue looping over blocks until all propagators are
   * calculated. Driver for helper method, updateBlockQ, which
   * performs all checks and publishing Q results
   */
  while (!areAllBlocksUpdated() ) {
    for (size_t nblock=0; nblock<numBlocks; ++nblock) {
      // Series of "blockPtr->solveQ(end)"
      updateBlockQ(nblock,HEAD);
      updateBlockQ(nblock,TAIL);
    }
  }

  /*
   * Calculate normalization Q value for an arbitrary block.
   * SWS: CAREFUL... subtlely different with branches etc.?
   */
  this->bigQ = blocks[0]->calcBigQ();

  /*
   * After updating ALL propagators above
   * calculate int [ q(X,s)*qt(X,s) ds ]
   * This is calculated in a separate step, because
   * many observables use this quantity
   */
  for (size_t i=0; i<numBlocks; ++i)
    blocks[i]->setCalcQQTIntegral(this->bigQ);

  /*
   * One of the methods that uses int [ q(X,s)*qt(X,s) ds ]
   * result. Calculates appropriate densities and
   * sets those values in the held PhysFields
   */
  for (size_t i=0; i<numBlocks; ++i)
    blocks[i]->setPhysFields();

} // end update

//
// helper method for calculating block propagators
// calls appropriate methods for calculating the
// propagator function for the block with index "nblock"
// starting from the end "end" (head/tail)
//
template <class FLOATTYPE, size_t NDIM>
void PsBlockCopolymer<FLOATTYPE, NDIM>::updateBlockQ(
       size_t nblock, BlockEndType end) {

  PsBlockBase<FLOATTYPE, NDIM>* blockPtr = blocks[nblock];
  BlockEndType otherend = blockPtr->getOtherEnd(end);

  // Check and set junction values.
  if (!blockPtr->isQSet(INITIAL,end) ) {
    if (blockPtr->areJntsSet(end) ) {
      blockPtr->combineSetJnt(end);
    }
  }

  // If q at end of block set and
  // not already solved, then solve/publish
  if ( blockPtr->isQSet(INITIAL,end) &&
       !blockPtr->isQSet(FINAL,otherend) ) {

    // Solve PDE for this block then make
    // connected blocks aware of result from solveQ
    blockPtr->solveQ(end);
    publishQFrom(otherend,nblock);
  }

}

/*
 * This is the method that communicates Q values from
 * one block to another
 *
 * IMPORTANT: this is where type conversions for Q values
 *   CAN/SHOULD occur... the qType for iblock and fromIndex will have
 *   to be checked and the appropriate type conversion made.
 *   For uniform block types (NOW) this is not needed.
 */
template <class FLOATTYPE, size_t NDIM>
void PsBlockCopolymer<FLOATTYPE, NDIM>::publishQFrom(
        BlockEndType end, size_t fromIndex) {

  // local connected-to list
  std::vector<std::pair<size_t, BlockEndType> > blockCntTo;
  blockCntTo = blocks[fromIndex]->getCntTo(end);

  // Set the from block base pointer
  PsBlockBase<FLOATTYPE, NDIM>* fromBlockPtr = blocks[fromIndex];

  // Check that block solution Q exists
  if ( fromBlockPtr->isQSet(FINAL,end) ) {

    // Loop over connected blocks and set Q values
    for (size_t n=0; n<blockCntTo.size(); ++n) {

      size_t toIndex = blockCntTo[n].first;
      BlockEndType endType = blockCntTo[n].second;
      PsBlockBase<FLOATTYPE, NDIM>* toBlockPtr = blocks[toIndex];
      /* ********************************************************* */
      // SWS: Conversion for q(X) values depending on block type
      // SWS: convertQ(fromBlockPtr, toBlockPtr);
      /* ********************************************************* */
      // Set junction values
      toBlockPtr->setJctQ(endType,fromBlockPtr,end,fromIndex);
    }

  } // check if final Q value on fromIndex block is set

}

//
// Convert q(X) values depending on block block types
// Code must be added to this method depending on the kinds
// of derived block types.
//
template <class FLOATTYPE, size_t NDIM>
void PsBlockCopolymer<FLOATTYPE, NDIM>::convertQ(
          PsBlockBase<FLOATTYPE, NDIM>* fromBlockPtr,
          PsBlockBase<FLOATTYPE, NDIM>* toBlockPtr) {

  std::string fromTypeStr, toTypeStr;
  size_t convertType = 10000;

  //
  // Select block conversion type
  //
  fromTypeStr = fromBlockPtr->getBlockType();
  toTypeStr   = toBlockPtr->getBlockType();
  if (fromTypeStr == "flexibleBlock"     && toTypeStr == "flexibleBlock")
    convertType = 0;
  if (fromTypeStr == "semiflexibleBlock" && toTypeStr == "semiflexibleBlock")
    convertType = 1;
  if (fromTypeStr == "semiflexibleBlock" && toTypeStr == "flexibleBlock")
    convertType = 2;
  if (fromTypeStr == "flexibleBlock"     && toTypeStr == "semiflexibleBlock")
    convertType = 3;

  //
  // Convert Q values appropriately
  //
  switch (convertType) {
    case 0:
      std::cout << "convertQ flex --> flex" << std::endl;
      break;
    case 1:
      break;
      std::cout << "convertQ semiflex --> semiflex" << std::endl;
    case 2:
      std::cout << "convertQ semiflex --> flex" << std::endl;
      break;
    case 3:
      std::cout << "convertQ flex --> semiflex" << std::endl;
      break;
    default:
      std::cout << "Error: convertQ types not known" << std::endl;
      break;
  }
}

// Instantiate
template class PsBlockCopolymer<float, 1>;
template class PsBlockCopolymer<float, 2>;
template class PsBlockCopolymer<float, 3>;

template class PsBlockCopolymer<double, 1>;
template class PsBlockCopolymer<double, 2>;
template class PsBlockCopolymer<double, 3>;
