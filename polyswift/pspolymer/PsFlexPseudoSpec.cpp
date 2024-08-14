/**
 * @file    PsFlexPseudoSpec.cpp
 *
 * @brief   Class for solving propagators for flexible Gaussian blocks
 *          with pseudo-spectral algorithm
 *
 * @version $Id: PsFlexPseudoSpec.cpp 8257 2007-09-12 22:05:20Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsFieldBase.h>

// pspolymer includes
#include <PsFlexPseudoSpec.h>

// psstd includes
#include <PsRandom.h>
#include <PsPhysConsts.h>

// Constructor
template <class FLOATTYPE, size_t NDIM, class QTYPE>
PsFlexPseudoSpec<FLOATTYPE, NDIM, QTYPE>::PsFlexPseudoSpec() {

  this->blockTypeStr = "flexibleBlock";

  fftObjPtr = NULL;

  k2     = NULL;
  wfac   = NULL;
  qw     = NULL;
  resPtr = NULL;

  fftSize = 0;
  scaleFFT = 0.0;

  bSegRatio = 1.0;
}

// Destructor
template <class FLOATTYPE, size_t NDIM, class QTYPE>
PsFlexPseudoSpec<FLOATTYPE, NDIM, QTYPE>::~PsFlexPseudoSpec() {
  delete[] k2;
  delete[] wfac;
  delete[] qw;
  delete[] resPtr;
}

//
//
//
template <class FLOATTYPE, size_t NDIM, class QTYPE>
void PsFlexPseudoSpec<FLOATTYPE, NDIM, QTYPE>::setAttrib(
     const TxHierAttribSetIntDbl& tas) {

  // Scoping call to base class
  PsBlock<FLOATTYPE, NDIM, QTYPE>::setAttrib(tas);
  this->dbprt("PsFlexPseudoSpec::setAttrib() ");

  // Ratio of statistical segment lengths
  if (tas.hasParam("bSegRatio")) {
    bSegRatio = tas.getParam("bSegRatio");
  }

  // Object name of FFT object
  if (tas.hasString("fftKind")) {
    fftKind = tas.getString("fftKind");
   }
  else {
    fftKind = "fftWTransposeObj";
    // TxDebugExcept tde("PsFlexPseudoSpec::setAttrib: fftKind not set");
    // tde << " in <PsBlock " << this->getName() << " >";
    // throw tde;
  }

}

//
// build data
//
template <class FLOATTYPE, size_t NDIM, class QTYPE>
void PsFlexPseudoSpec<FLOATTYPE, NDIM, QTYPE>::buildData() {

  // Scoping call to base class, sets the spatial part of qDims
  PsBlock<FLOATTYPE, NDIM, QTYPE>::buildData();
  this->dbprt("PsFlexPseudoSpec::buildData() ");

  // Set physical size of propagator
  PsGridBaseItr* gItr = &this->getGridBase();
  qX.setGrid(gItr);

  this->dbprt("PsFlexPseudoSpec::buildData(): qTotalSize = ",
              (int)this->qTotalSize);

  // Allocate space for transform lists
  // qTotalSize is NOT total_local_size in FFTW
  k2     = new FLOATTYPE[this->qTotalSize];
  wfac   = new FLOATTYPE[this->qTotalSize];
  qw     = new FLOATTYPE[this->qTotalSize];
  resPtr = new FLOATTYPE[this->qTotalSize];
}

//
template <class FLOATTYPE, size_t NDIM, class QTYPE>
void PsFlexPseudoSpec<FLOATTYPE, NDIM, QTYPE>::buildSolvers() {

  // Scoping call to base class
  PsBlock<FLOATTYPE, NDIM, QTYPE>::buildSolvers();
  this->dbprt("PsFlexPseudoSpec::buildSolvers() ");

  // Set/store initial conditions for free-ends, depends on block model
  this->q0.reset(1.0);

  // Get default FFTW method / grid pointers
  fftObjPtr = PsNamedObject::getObject<PsFFTBase<FLOATTYPE, NDIM> >(fftKind);
  fftGridPtr = &fftObjPtr->getGrid();

  // Checking that propagator size is consistent with FFT buffer sizes
  // SWS: This check may be defeating FFTW functionality.... change?
  fftSize = fftObjPtr->getFFTSize();
  if (fftSize != this->qTotalSize) {
    TxDebugExcept tde("PsFlexPseudoSpec::buildSolvers: the FFT data struct size");
    tde << " in <PsFlexPseudoSpec " << this->getName() << " >";
    tde << " is not equal to propagator size";
    tde << "\n fftSize    = " << fftSize;
    tde << "\n qTotalSize = " << this->qTotalSize;
    throw tde;
  }

  //
  // Build k2 list, only depends on ds and system size
  // and for now is only built at beginning of build cycle
  // If MPI is present then parallel is assumed and the
  // scaledFFTPair routine will use the "TRANSPOSED_ORDER"
  // to save communication time. If MPI not present then serial
  // is assumed and the scaledFFTPair routine defaults to
  // "NORMAL" data layout
  //
#ifdef HAVE_MPI
  build_k2_transpose();
#else
  build_k2();
#endif

  // Set FFT scaling
  scaleFFT = 1.0 / ((FLOATTYPE) this->getGridBase().getTotalCellsGlobal() );
}

//
// Reset blocks at beginning of new solve for w-fields
//
template <class FLOATTYPE, size_t NDIM, class QTYPE>
void PsFlexPseudoSpec<FLOATTYPE, NDIM, QTYPE>::reset() {

  // Scoping call to base class
  PsBlock<FLOATTYPE, NDIM, QTYPE>::reset();

  this->dbprt("PsFlexPseudoSpec::reset() ");
  this->dbprt(" ... setting w-fields with physField ",
              this->monoDensPhysFldPtr->getName() );

  // Get physField and the associated conjugate field
  // Obtain data pointer from fieldBase
  FLOATTYPE ds2 = -0.5*this->ds;
  PsFieldBase<FLOATTYPE>& wField = this->monoDensPhysFldPtr->getConjgField();
  const FLOATTYPE* wDataPtr = wField.getConstDataPtr();

  // Set w fields and scale by ds factor
  for (size_t n=0; n<fftSize; ++n) {
    wfac[n] = (FLOATTYPE)std::exp(ds2*wDataPtr[n]);
  }
  this->dbprt("wfac[0] = ", (int)wfac[0]);
  this->dbprt("wfac[1] = ", (int)wfac[1]);
  this->dbprt("wfac[2] = ", (int)wfac[2]);
}

//
// helper method to calck2 list only depends on ds and system size
// and for now is only built at beginning of build cycle
//
template <class FLOATTYPE, size_t NDIM, class QTYPE>
void PsFlexPseudoSpec<FLOATTYPE, NDIM, QTYPE>::build_k2() {

  // Local holder for k2 values
  PsGridField<FLOATTYPE, NDIM> k2Field;
  PsGridBaseItr* gItr = &this->getGridBase();
  k2Field.setGrid(gItr);
  k2Field.calck2();

  // Map k2 field values to normal order layout
  // and form correct exp operator
  size_t n=0;
  for (size_t i = 0; i < this->qDims[0]; ++i) {
  for (size_t j = 0; j< this->qDims[1]; ++j) {
  for (size_t k = 0; k < this->qDims[2]; ++k) {
    k2[n] = std::exp(-1.0*this->ds*k2Field(i, j, k, 0)*bSegRatio*bSegRatio);
    n++;
  }}}

}

//
// helper method to calculate k2 list
// only depends on ds and system size
// and for now is only built at beginning of build cycle
//
template <class FLOATTYPE, size_t NDIM, class QTYPE>
void PsFlexPseudoSpec<FLOATTYPE, NDIM, QTYPE>::build_k2_transpose() {

  // Local holder for k2 values formed from fft grid/decomp
  PsGridField<FLOATTYPE, NDIM> k2Field;
  PsGridBaseItr* gItr = fftGridPtr;
  k2Field.setGrid(gItr);
  k2Field.calck2();

  // Map k2 field values to transpose order layout
  // and form correct exp operator
  std::vector<size_t> kDims = fftGridPtr->getDecomp().getNumCellsLocal();
  size_t n=0;
  for (size_t j = 0; j< kDims[1]; ++j) {
  for (size_t i = 0; i < kDims[0]; ++i) {
  for (size_t k = 0; k < kDims[2]; ++k) {
    k2[n] = std::exp(-1.0*this->ds*k2Field(i, j, k, 0)*bSegRatio*bSegRatio);
    n++;
  }}}

}

/*
 * Solve for q(r,s) starting from solveFromEnd and finish at otherEnd
 *
 * this method manages solution method for the propagator
 * q(r,s+ds) = exp[ ds( \nabla^2 - w(r) )] q(r,s). The q(r,s)
 * structure "knows" about s and the class hierarchy "knows" about
 * the number of solution steps given ds.
 */
template <class FLOATTYPE, size_t NDIM, class QTYPE>
void PsFlexPseudoSpec<FLOATTYPE, NDIM, QTYPE>::solveQ(
         BlockEndType solveFromEnd) {

  // *********************************************************
  // Diagnostics
  this->dbprt("PsFlexPseudoSpec::solveQ() ");
  std::string endStr="HEAD";
  if (solveFromEnd == TAIL) endStr = "TAIL";
  this->dbprt("...solve q(r,s) flex starting at ", endStr);
  // *********************************************************

  // Set initial condition... need this copy?
  //  std::cout << "proc # = " << this->getCommBase().getRank()
  //        << "  solveFromEnd = " << solveFromEnd
  //        << std::endl;

  qX = this->getInitialQ(solveFromEnd);

  // Store initial condition in full propagator
  if (solveFromEnd == HEAD) this->qs[0]  = qX;
  if (solveFromEnd == TAIL) this->qts[0] = qX;

  // Loop over steps in propagator
  for (size_t ss=1; ss<=this->blockSteps; ++ss) {

    // Get qX data and apply half-field factor to q(r,s)
    // SWS: data access is dangerous...and should be refactored
    FLOATTYPE* qdata = qX.getDataPtr();
    for (size_t n=0; n<fftSize; ++n)
      qw[n] = qdata[n]*wfac[n];

    // Perform pseudo-spectral transform pair:  F^-1[k2*F[q*w]]
    fftObjPtr->scaledFFTPair(qw, k2, resPtr);

    // Apply other half-field factor to q(r,s) and
    // put into qdata this effectively assigns data to qX
    for (size_t n=0; n<fftSize; ++n)
      qdata[n] = resPtr[n]*wfac[n];

    // Transform scale factor, global simulation size
    // and put into full propagator
    qX.scale(scaleFFT);
    if (solveFromEnd == HEAD) this->qs[ss]  = qX;
    if (solveFromEnd == TAIL) this->qts[ss] = qX;

  } // loop on blockSteps

  // Set holder for final q(r,s) field
  this->setFinalQ(this->getOtherEnd(solveFromEnd),qX);
}

// Instantiate classes (for flexible block model)
template class PsFlexPseudoSpec<float, 1, PsBlockTypes<float, 1>::flexQType >;
template class PsFlexPseudoSpec<float, 2, PsBlockTypes<float, 2>::flexQType >;
template class PsFlexPseudoSpec<float, 3, PsBlockTypes<float, 3>::flexQType >;

template class PsFlexPseudoSpec<double, 1, PsBlockTypes<double, 1>::flexQType >;
template class PsFlexPseudoSpec<double, 2, PsBlockTypes<double, 2>::flexQType >;
template class PsFlexPseudoSpec<double, 3, PsBlockTypes<double, 3>::flexQType >;
