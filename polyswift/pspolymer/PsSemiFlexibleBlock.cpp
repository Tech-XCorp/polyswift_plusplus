/**
 * @file    PsSemiFlexibleBlock.cpp
 *
 * @brief   Class for solving propagators for semiflexible blocks.
 *
 * @version $Id: PsSemiFlexibleBlock.cpp 8257 2007-09-12 22:05:20Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// pspolymer includes
#include <PsSemiFlexibleBlock.h>

// Constructor
template <class FLOATTYPE, size_t NDIM, class QTYPE>
PsSemiFlexibleBlock<FLOATTYPE, NDIM, QTYPE>::PsSemiFlexibleBlock() {
  this->blockTypeStr = "semiflexibleBlock";
}

// Destructor
template <class FLOATTYPE, size_t NDIM, class QTYPE>
PsSemiFlexibleBlock<FLOATTYPE, NDIM, QTYPE>::~PsSemiFlexibleBlock() {

}

template <class FLOATTYPE, size_t NDIM, class QTYPE>
void PsSemiFlexibleBlock<FLOATTYPE, NDIM, QTYPE>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Scoping call to base class
  PsBlock<FLOATTYPE, NDIM, QTYPE>::setAttrib(tas);
}

template <class FLOATTYPE, size_t NDIM, class QTYPE>
void PsSemiFlexibleBlock<FLOATTYPE, NDIM, QTYPE>::buildData() {

  // Scoping call to base class
  PsBlock<FLOATTYPE, NDIM, QTYPE>::buildData();

  this->dbprt("PsSemiFlexibleBlock::buildData() ");

  // Additional (fake) U size for 3D semi-flex
  this->qDims.push_back(5);
  this->qDims.push_back(3);
  //std::cout << "qDims[fake dims] = added" << std::endl;

  //
  // Get solver objects
  //
}

template <class FLOATTYPE, size_t NDIM, class QTYPE>
void PsSemiFlexibleBlock<FLOATTYPE, NDIM, QTYPE>::buildSolvers() {

  // Scoping call to base class
  PsBlock<FLOATTYPE, NDIM, QTYPE>::buildSolvers();

  this->dbprt("PsSemiFlexibleBlock::buildSolvers() ");

  // Set physical size of propagator
  PsGridBaseItr* gItr = &this->getGridBase();
  qX.setGrid(gItr);

  // SWS: Set/store initial conditions for free-ends
  // SWS: which depends on block model... must CHANGE this!
  this->q0.reset(1.0);
}

template <class FLOATTYPE, size_t NDIM, class QTYPE>
void PsSemiFlexibleBlock<FLOATTYPE, NDIM, QTYPE>::reset() {

  // Scoping call to base class
  PsBlock<FLOATTYPE, NDIM, QTYPE>::reset();

  this->dbprt("PsSemiFlexibleBlock::initialize() ");

  //
  // Set w and other factors
  //
}

/*
 * Solve for q(r,s) starting from solveFromEnd and finish at otherEnd
 *
 * this method manages solution method for the propagator
 * q(r,u,s+ds) = [semi-flex model] q(r,u,s). The q(r,u,s)
 * structure "knows" about s and the class hierarchy "knows" about
 * the number of solution steps given ds.
 */
template <class FLOATTYPE, size_t NDIM, class QTYPE>
void PsSemiFlexibleBlock<FLOATTYPE, NDIM, QTYPE>::solveQ(BlockEndType solveFromEnd) {

  // *********************************************************
  // Diagnostic print stuff
  std::string endStr="HEAD";
  //  if (solveFromEnd == TAIL) endStr = "TAIL";
  //    std::cout << "... solve q(r,s) flexible starting at " <<
  //      endStr << std::endl;
  //  }
  // *********************************************************

  //
  // Set initial condition
  //
  qX = this->getInitialQ(solveFromEnd);

  //
  // Temporary vars
  //
  //  FLOATTYPE* data=0;
  //  FLOATTYPE* fdata=0;
  //  size_t fftSize = fftObjPtr->getFFTSize(); remember friends...

  //
  // Loop over steps in propagator
  //
  for (size_t ss=0; ss<this->blockSteps; ++ss) {

    //    std::cout << "Solving for q: s = " << ss << " to " << ss+1 << std::endl;

    // Must decide to push back to qs or qts

    qX.scale(0.90);

    //    data = qX.get DataPtr();
    //    fdata = fftObjPtr->calcForwardNormalFFT(data);

    // take q(r) (from last step)
    // format for solver
    // get result from solver for q(r,s+ds)
    // push back to q(r,s) <vector>

  }

  //  FLOATTYPE* dataPtr;
  //  dataPtr = qX.get DataPtr();
  //  std::cout << "qX[last] = " << dataPtr[0] << std::endl;

  //  this->setFinalQ(this->getOtherEnd(solveFromEnd),qX);
}

/* Instantiate classes (for semi-flexible block model) */
// NDIM = 1 might be problematic
template class PsSemiFlexibleBlock<float, 1, PsBlockTypes<float, 1>::semiflexQType >;
template class PsSemiFlexibleBlock<float, 2, PsBlockTypes<float, 2>::semiflexQType >;
template class PsSemiFlexibleBlock<float, 3, PsBlockTypes<float, 3>::semiflexQType >;

template class PsSemiFlexibleBlock<double, 1, PsBlockTypes<double, 1>::semiflexQType >;
template class PsSemiFlexibleBlock<double, 2, PsBlockTypes<double, 2>::semiflexQType >;
template class PsSemiFlexibleBlock<double, 3, PsBlockTypes<double, 3>::semiflexQType >;
