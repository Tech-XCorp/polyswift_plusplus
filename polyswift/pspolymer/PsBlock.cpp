/**
 * @file    PsBlock.cpp
 *
 * @brief   Class for contiguous monomer blocks. Manages block
 *          connectivity that is templated over propagator type
 *
 * @version $Id: PsBlock.cpp 8257 2007-09-12 22:05:20Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// txbase includes
#include <TxTensor.h>

// psbase includes
#include <PsBlock.h>

//
// Constructor
//
template <class FLOATTYPE, size_t NDIM, class QTYPE>
PsBlock<FLOATTYPE, NDIM, QTYPE>::PsBlock() {
}

//
// Destructor
//
template <class FLOATTYPE, size_t NDIM, class QTYPE>
PsBlock<FLOATTYPE, NDIM, QTYPE>::~PsBlock() {

  // Clear junction lists, holds copy of QTYPE
  // so destructor should be called automatically
  qHeadJnts.clear();
  qTailJnts.clear();

  // Clear propagator lists
  qs.clear();
  qts.clear();
}

template <class FLOATTYPE, size_t NDIM, class QTYPE>
void PsBlock<FLOATTYPE, NDIM, QTYPE>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Scoping call to base class
  PsBlockBase<FLOATTYPE, NDIM>::setAttrib(tas);
}

template <class FLOATTYPE, size_t NDIM, class QTYPE>
void PsBlock<FLOATTYPE, NDIM, QTYPE>::buildData() {

  // Scoping call to base class
  PsBlockBase<FLOATTYPE, NDIM>::buildData();
  this->dbprt("PsBlock::buildData() ");

  //
  // Q sizes set here because rest of build cycle needs it.
  // Spatial part of qDims set here...which is assumed to
  // be common for all models.
  //
  PsGridBaseItr* gItr = &this->getGridBase();
  qDims = this->getGridBase().getDecomp().getNumCellsLocal();
  qTotalSize = this->getGridBase().getDecomp().getTotalCellsLocal();

  this->dbprt("qDims[0] = ", (int)qDims[0]);
  this->dbprt("qDims[1] = ", (int)qDims[1]);
  this->dbprt("qDims[2] = ", (int)qDims[2]);

  // Junction value holders
  qprod.setGrid(gItr);
  qInit.setGrid(gItr);

  // Using initial q0 workspace variable to set Jnts maps
  q0.setGrid(gItr);
  q0.reset(1.0);
  q0.setFieldType("q0Field");

  // Build solvers to hold
  qqtIntegral.setGrid(gItr); // result of        int[q(X,s)*qt(X,s) ds]
  qqtDens.setGrid(gItr);     // result of scaled int[q(X,s)*qt(X,s) ds]

  //  std::cout << "leaving PsBlock::buildData" << std::endl;
}

//
//
template <class FLOATTYPE, size_t NDIM, class QTYPE>
void PsBlock<FLOATTYPE, NDIM, QTYPE>::buildSolvers() {

  // Scoping call to base class
  PsBlockBase<FLOATTYPE, NDIM>::buildSolvers();
  this->dbprt("PsBlock::buildSolvers() ");

 /*
  * base class buildSolvers() sets the q___JntsSet flags
  * this list is used to "build" and initialize the actual
  * qHeadJnts data map... actual QTYPE values need to init,
  * Building a local q0 variable in order to set Jnts maps
  */
  size_t cntIndex;
  std::map<size_t, bool>::iterator pos;

  // Set and initialize junction map values: for HEAD
  for (pos = this->qHeadJntsSet.begin();
       pos != this->qHeadJntsSet.end(); ++pos) {
    cntIndex = pos->first;
    std::pair<size_t, QTYPE> mapElem = std::make_pair(cntIndex, q0);
    qHeadJnts.insert(mapElem);
    // This old method calls internal constructor that
    // does not have grid info set
    // qHeadJnts[cntIndex] = q0;
  }

  // Set and initialize junction map values: for TAIL
  for (pos = this->qTailJntsSet.begin();
       pos != this->qTailJntsSet.end(); ++pos) {
    cntIndex = pos->first;
    std::pair<size_t, QTYPE> mapElem = std::make_pair(cntIndex, q0);
    qTailJnts.insert(mapElem);
    // This old method calls internal constructor that
    // does not have grid info set
    // qTailJnts[cntIndex] = q0;
  }

  // Build qs/qts vectors
  //  std::cout << "block steps = " << this->blockSteps << std::endl;
  for (size_t n=0; n<=this->blockSteps; ++n) {
    qs.push_back(q0);
    qts.push_back(q0);
  }
}

template <class FLOATTYPE, size_t NDIM, class QTYPE>
void PsBlock<FLOATTYPE, NDIM, QTYPE>::reset() {

  // Scoping call to base class
  PsBlockBase<FLOATTYPE, NDIM>::reset();
  this->dbprt("PsBlock::reset() ");

  // Set free ends...particular to flexible or semi-flexible
  // so q0 is stored in this class but is set by derived classes
  if (this->headCntTo.size() == 0) setInitialQ(HEAD,q0);
  if (this->tailCntTo.size() == 0) setInitialQ(TAIL,q0);
}

//
// This updates only the monomer "densities" in the owned PhysField
//
template <class FLOATTYPE, size_t NDIM, class QTYPE>
void PsBlock<FLOATTYPE, NDIM, QTYPE>::setPhysFields() {

  this->dbprt("calling PsBlock::setPhysFields()");
  this->dbprt("... updating the densities in physField ",
              this->monoDensPhysFldPtr->getName() );

  // Find model correction for a distribution of blocks
  // For non-polydisperse models this will be 1.0.
  // For polydisperse blocks in polydisperse models this
  // will be the quadrature weight/normalization
  FLOATTYPE densWt = this->getDensityWeight();

  // Update monomer densities... this method must "ADD" for multiple chains
  // qqtIntegral includes bigQ normalization
  // Assumes that the density fields have been appropriately
  // initialized to zero, so they may act as counters.
  FLOATTYPE vf = this->polymerObjPtr->getVolfrac();

  // Set density
  FLOATTYPE densFac = densWt*vf/(this->polymerObjPtr->getLengthRatio());
  qqtDens = qqtIntegral;
  qqtDens.scale(densFac);

  // If tracking single block density then set field
  if (this->hasBlockField) {
    PsFieldBase<FLOATTYPE>& blockField = this->blockDensPhysFldPtr->getDensField();
    blockField.reset(0.0);
    blockField += qqtDens;
  }

  // Add to appropriate density field
  PsFieldBase<FLOATTYPE>& densField = this->monoDensPhysFldPtr->getDensField();
  densField += qqtDens;

  PsFieldBase<FLOATTYPE>& wallField  = this->constraintFieldPtr->getDensField();
  PsFieldBase<FLOATTYPE>& maskField  = *(this->tmpFieldPtr);

  // Construct mask field from constraint (1 - wallField)
  maskField.reset(0.0);
  maskField += wallField;
  maskField *= -1.0;
  maskField += 1.0;
}

//
// This integrates q(r,s')*qt(r,s')*dr over volume where s'
// is a specific location on the block. This is needed to
// normalize many of the observables' calculations. s' in
// this case is chosen to be the "HEAD" of the block.
//
template <class FLOATTYPE, size_t NDIM, class QTYPE>
FLOATTYPE PsBlock<FLOATTYPE, NDIM, QTYPE>::calcBigQ() {

  this->dbprt("calling PsBlock::calcBigQ ");

  // Multiple field elements for q(0)*qt(0) and
  // store in local product list
  QTYPE qprod;
  PsGridBaseItr* gItr = &this->getGridBase();
  qprod.setGrid(gItr);
  qprod += qHeadInitial;
  qprod *= qHeadFinal;

  // Get total volume (less constraint volume)
  size_t fieldSize = qHeadInitial.getSize(); // SWS: volume
  FLOATTYPE localVol =
    FLOATTYPE(fieldSize) - this->constraintFieldPtr->calcLocalVolume();

  // Sum for bigQ over all ranks
  size_t nprocs = this->getCommBase().getSize();
  FLOATTYPE localbQ= qprod.getSumAll()/( (FLOATTYPE) (nprocs*localVol) );
  this->dbprt("bigQ(local) = ", localbQ);

  FLOATTYPE globalbQ = this->getCommBase().allReduceSum(localbQ);
  this->dbprt("bigQ = ", globalbQ);
  this->dbprt(" from block ", this->getName() );

  if (globalbQ != globalbQ) {
     TxDebugExcept tde("PsBlock::calcBigQ() found nan/inf?");
     throw tde;
  }

  return globalbQ;
}

//
// This collapses q(r,s)*qt(r,s) over s. To be used by
// many observables' calculations.
//
// Method: Extended Simpson's rule:
//         For three-point interval x1 --> x3 the integral is
//         approximated by
//         (1/3) * dx * [ f(x1) + 4*f(x2) + f(x3) ]
//
//         For N(odd) points the overlapping ends of all the
//         three-point intervals gives an alternating sum w/coeff
//         of 1,4,2,4,2,4....2,4,1 (inside [])
//
//         For N(even) points use same algorithm for N-1 intervals
//         and then use trapezoidal rule for last interval.
//
template <class FLOATTYPE, size_t NDIM, class QTYPE>
void PsBlock<FLOATTYPE, NDIM, QTYPE>::setCalcQQTIntegral(FLOATTYPE bQ) {

  this->dbprt("calling PsBlock::setCalcQQTIntegral ");

  // Local q data holders
  QTYPE q1;
  QTYPE q2;
  QTYPE q3;
  QTYPE qt1;
  QTYPE qt2;
  QTYPE qt3;

  PsGridBaseItr* gItr = &this->getGridBase();
  q1.setGrid(gItr);
  q2.setGrid(gItr);
  q3.setGrid(gItr);
  qt1.setGrid(gItr);
  qt2.setGrid(gItr);
  qt3.setGrid(gItr);

  // Assuming q(X) are same size for all s
  size_t numSsteps = qs.size();
  size_t npts = numSsteps - 1;
  size_t numStail;

  // Set even/odd flag
  bool even = 0;
  FLOATTYPE modval = std::fmod((FLOATTYPE)numSsteps,(FLOATTYPE)2);
  if (modval == 0.0) {
    even = 1;
    numStail = npts-3;
  }
  else {
    even = 0;
    numStail = npts-2;
  }

  // Should check that qs and qts same size,
  // will be changed when size is explicitly set
  // Flips the order on qt 'by hand'
  size_t step1, step2, step3, stept1, stept2, stept3;

  //  qqtIntegral.reset(0.0);
  PsFieldBase<FLOATTYPE>& qqB = *(qqtIntegral.getBasePtr());
  qqB.reset(0.0);

  // Sum over sets of length 3 intervals
  for (size_t n=0; n<=numStail; n=n+2) {

    step1 = n;
    step2 = n+1;
    step3 = n+2;

    stept1 = numSsteps-n-1;
    stept2 = numSsteps-n-2;
    stept3 = numSsteps-n-3;

    q1  = qs[step1];   q2  = qs[step2];   q3  = qs[step3];
    qt1 = qts[stept1]; qt2 = qts[stept2]; qt3 = qts[stept3];

    // Update over field variables sum on i
    // Old: size_t fieldSize = qs[0].get Size();
    /* for (size_t i=0; i<fieldSize; ++i) {
        qqtIntegral(i) = qqtIntegral(i) +
          (0.33333 * this->ds) * (
            ( 1.0 * q1(i)*qt1(i) ) +
            ( 4.0 * q2(i)*qt2(i) ) +
            ( 1.0 * q3(i)*qt3(i) ) );
    } */

    PsFieldBase<FLOATTYPE>& q1B = *(q1.getBasePtr());
    PsFieldBase<FLOATTYPE>& q2B = *(q2.getBasePtr());
    PsFieldBase<FLOATTYPE>& q3B = *(q3.getBasePtr());
    PsFieldBase<FLOATTYPE>& qt1B = *(qt1.getBasePtr());
    PsFieldBase<FLOATTYPE>& qt2B = *(qt2.getBasePtr());
    PsFieldBase<FLOATTYPE>& qt3B = *(qt3.getBasePtr());

    q1B *= qt1B; q2B *= qt2B; q3B *= qt3B;
    q2B *= 4.0;
    qqB += q1B; qqB += q2B; qqB += q3B;
  }
  qqB *= 0.33333*this->ds;

  // Add one trapezoidal interval at the end
  // of even sized list
  if (even) {

    step1 = numSsteps-2;
    step2 = numSsteps-1;

    stept1 = 1;
    stept2 = 0;

    q1  = qs[step1];   q2  = qs[step2];
    qt1 = qts[stept1]; qt2 = qts[stept2];

    // Update over field variables
    // Old: size_t fieldSize = qs[0].get Size();
    /* for (size_t i=0; i<fieldSize; ++i) {
        qqtIntegral(i) = qqtIntegral(i) +
         0.50 * (
         ( q1(i)*qt1(i)*this->ds ) +
         ( q2(i)*qt2(i)*this->ds ) );
     } */

    PsFieldBase<FLOATTYPE>& q1B = *(q1.getBasePtr());
    PsFieldBase<FLOATTYPE>& q2B = *(q2.getBasePtr());
    PsFieldBase<FLOATTYPE>& qt1B = *(qt1.getBasePtr());
    PsFieldBase<FLOATTYPE>& qt2B = *(qt2.getBasePtr());

    // Reuse tmp q3 variable for counter
    PsFieldBase<FLOATTYPE>& qTB = *(q3.getBasePtr());
    qTB.reset(0.0);

    q1B *= qt1B;
    q2B *= qt2B;
    qTB += q1B;
    qTB += q2B;
    qTB *= this->ds*0.5;
    qqB += qTB;
  } // if even

  // Include normalization bigQ factor
  qqtIntegral.scale(1.0/bQ);
}

//
// Calculate constrained initial q values from all junction values
// at head/tail. Uses setInitialQ to set "junctions"
//
template <class FLOATTYPE, size_t NDIM, class QTYPE>
void PsBlock<FLOATTYPE, NDIM, QTYPE>::combineSetJnt(BlockEndType end) {

  // Iterator type
  typename std::map<size_t, QTYPE>::iterator pos;

  // Local result holder
  qprod.reset(1.0);
  PsFieldBase<FLOATTYPE>& qprodB = qprod;

  // reset for safety
  qInit.reset(1.0);

  if (end == HEAD) {   // Combine HEAD junctions
    for (pos = qHeadJnts.begin(); pos != qHeadJnts.end(); ++pos) {
      qInit = pos->second;
      PsFieldBase<FLOATTYPE>& qInitB = qInit;
      qprodB *= qInitB;
    }
  }
  else {  // Combine TAIL junctions
    for (pos = qTailJnts.begin(); pos != qTailJnts.end(); ++pos) {
      qInit = pos->second;
      PsFieldBase<FLOATTYPE>& qInitB = qInit;
      qprodB *= qInitB;
    }
  }

  // Set combined junction result
  setInitialQ(end, qprod);
}

//
// Get final q(X) field for head/tail
//
template <class FLOATTYPE, size_t NDIM, class QTYPE>
PsFieldBase<FLOATTYPE>* PsBlock<FLOATTYPE, NDIM, QTYPE>::getFinalQ(BlockEndType end) {

  if (end == HEAD)
    return qHeadFinal.getBasePtr();
  else
    return qTailFinal.getBasePtr();
}

//
// Set q for head/tail at appropriate junction holder
// end is endtype, q0 is a pointer to data to set in field
// jntKey is key index of block that junction is connected to
//
template <class FLOATTYPE, size_t NDIM, class QTYPE>
void PsBlock<FLOATTYPE, NDIM, QTYPE>::setJctQ(BlockEndType end,
                                              PsBlockBase<FLOATTYPE, NDIM>* blockB,
                                              BlockEndType blockBend,
                                              size_t jntKey) {

  // Get final Q data from correct end
  PsFieldBase<FLOATTYPE>* fldQ = blockB->getFinalQ(blockBend);

  // Cast to correct field type
  // SWS: If different block models used and conversion
  // SWS: hasnt happened, this should fail
  QTYPE& fromBlkQ = *(dynamic_cast< QTYPE* >(fldQ));

  //    std::pair<size_t, QTYPE> mapElem = std::make_pair(cntIndex, q0);
  //    qTailJnts.insert(mapElem);t6

  if (end == HEAD) {
    this->qHeadJntsSet[jntKey] = 1;
    //    QTYPE& junctQ = qHeadJnts[jntKey];
    //    junctQ = fromBlkQ;
    qHeadJnts.erase(jntKey);
    std::pair<size_t, QTYPE> mapElem = std::make_pair(jntKey, fromBlkQ);
    qHeadJnts.insert(mapElem);
    //    qHeadJnts[jntKey] = fromBlkQ;
  }

  else {
    this->qTailJntsSet[jntKey] = 1;
    //    QTYPE& junctQ = qTailJnts[jntKey];
    //    junctQ = fromBlkQ;
    qTailJnts.erase(jntKey);
    std::pair<size_t, QTYPE> mapElem = std::make_pair(jntKey, fromBlkQ);
    qTailJnts.insert(mapElem);
    //    qTailJnts[jntKey] = fromBlkQ;
  }

} // end setJctQ

// Instantiate classes (for flexible block model)
template class PsBlock<float, 1, PsBlockTypes<float, 1>::flexQType >;
template class PsBlock<float, 2, PsBlockTypes<float, 2>::flexQType >;
template class PsBlock<float, 3, PsBlockTypes<float, 3>::flexQType >;

template class PsBlock<double, 1, PsBlockTypes<double, 1>::flexQType >;
template class PsBlock<double, 2, PsBlockTypes<double, 2>::flexQType >;
template class PsBlock<double, 3, PsBlockTypes<double, 3>::flexQType >;

// Instantiate classes (for semi-flexible block model)
//template class PsBlock<float, 1, PsBlockTypes<float, 1>::semiflexQType >;
template class PsBlock<float, 2, PsBlockTypes<float, 2>::semiflexQType >;
template class PsBlock<float, 3, PsBlockTypes<float, 3>::semiflexQType >;

//template class PsBlock<double, 1, PsBlockTypes<double, 1>::semiflexQType >;
template class PsBlock<double, 2, PsBlockTypes<double, 2>::semiflexQType >;
template class PsBlock<double, 3, PsBlockTypes<double, 3>::semiflexQType >;
