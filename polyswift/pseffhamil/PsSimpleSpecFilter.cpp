/**
 *
 * @file    PsSimpleSpecFilter.cpp
 *
 * @brief   Class for updating fields with spectral filter algorithm
 *          with single cutoff
 *
 * @version $Id: PsSimpleSpecFilter.cpp 8257 2007-09-12 22:05:20Z swsides $
 *
 * Copyright &copy; 2010-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// txbase includes
#include <TxTensor.h>

// pseffhamil includes
#include <PsSimpleSpecFilter.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsSimpleSpecFilter<FLOATTYPE, NDIM>::PsSimpleSpecFilter() {

}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsSimpleSpecFilter<FLOATTYPE, NDIM>::~PsSimpleSpecFilter() {

}

template <class FLOATTYPE, size_t NDIM>
void PsSimpleSpecFilter<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Scoping call to base class
  PsSpecFilterUpdater<FLOATTYPE, NDIM>::setAttrib(tas);

}

template <class FLOATTYPE, size_t NDIM>
void PsSimpleSpecFilter<FLOATTYPE, NDIM>::buildData() {

  // Scoping call to base class
  PsSpecFilterUpdater<FLOATTYPE, NDIM>::buildData();

}

template <class FLOATTYPE, size_t NDIM>
void PsSimpleSpecFilter<FLOATTYPE, NDIM>::buildSolvers() {

  // Scoping call to base class
  PsSpecFilterUpdater<FLOATTYPE, NDIM>::buildSolvers();

}

//
// Initialize before update step
// SWS: reorder this so initialize is only called at startup.....
template <class FLOATTYPE, size_t NDIM>
void PsSimpleSpecFilter<FLOATTYPE, NDIM>::initialize() {

  // Scoping call to base class
  PsSpecFilterUpdater<FLOATTYPE, NDIM>::initialize();

}

template <class FLOATTYPE, size_t NDIM>
void PsSimpleSpecFilter<FLOATTYPE, NDIM>::update(double t) {

  // Scoping call to base class
  PsSpecFilterUpdater<FLOATTYPE, NDIM>::update(t);

  this->dbprt("PsSimpleSpecFilter::update() ");

  // If not updating this time step...exit
  // If factor is zero then exit because even filtering
  // function with 0 cutoff perturbs fields.
  if (!this->updateFlag)         return;
  if (this->cutoffFactor <= 0.0) return;

  // Loop on update fields
  for (size_t n=0; n<this->numUpdateFields; ++n) {

    // Access conjugate polymer field and filter
    PsFieldBase<FLOATTYPE>& wField = this->updateFields[n]->getConjgField();

    this->dbprt("... filtering ", this->updateFields[n]->getName());
    this->dbprt("... with fft object: ", this->fftObjPtr->getName());

    this->subtractAverage(wField);
    applyFilter(wField);
  }

} // end update

//
// Helper method to filter... called from update
//
template <class FLOATTYPE, size_t NDIM>
void PsSimpleSpecFilter<FLOATTYPE, NDIM>::applyFilter(PsFieldBase<FLOATTYPE>& wf) {

  this->dbprt("PsSimpleSpecFilter::applyFilter ");

  // Return absolute value of forward transform
  this->fftObjPtr->forwardFFTAbs(wf.getDataPtr(),
      this->resPtr);

  // Find maximum value in spectrum to set cutoff
  FLOATTYPE maxVal = this->findMaxVal(this->resPtr, this->fftSize);
  size_t    maxPos = this->findMaxPos(this->resPtr, this->fftSize);
  this->dbprt("maxVal = ", maxVal);
  this->dbprt("maxPos = ", (int)maxPos);

  FLOATTYPE maxCutoff =
    this->cutoffFactor*this->getCommBase().allReduceMax(maxVal);
  /*
    std::cout << "maxCutoff = " << maxCutoff << std::endl;
    for (size_t n=0; n<10; ++n) {
      std::cout << "|F(_phi)| = " << this->resPtr[n] << std::endl;
    }
  }
  */

  // Create mask array
  for (size_t n=0; n<this->fftSize; ++n) {
    if (this->resPtr[n] < maxCutoff)
      this->maskCutPtr[n] = this->filterStrength;
    else
      this->maskCutPtr[n] = 1.0;
  }

  // SWS: this repeats transform work... must refactor
  this->fftObjPtr->scaledFFTPair(wf.getDataPtr(),
      this->maskCutPtr, this->resPtr);

  // This should be through PsField interface SWS: (refactor needed)
  FLOATTYPE* wdata = wf.getDataPtr();
  for (size_t n=0; n<this->fftSize; ++n) {
    wdata[n] = this->scaleFFT*this->resPtr[n];
  }

}

// Instantiate classes
template class PsSimpleSpecFilter<float, 1>;
template class PsSimpleSpecFilter<float, 2>;
template class PsSimpleSpecFilter<float, 3>;

template class PsSimpleSpecFilter<double, 1>;
template class PsSimpleSpecFilter<double, 2>;
template class PsSimpleSpecFilter<double, 3>;
