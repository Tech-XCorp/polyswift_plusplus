/**
 *
 * @file    PsMultiSpecFilter.cpp
 *
 * @brief   Class for updating fields with spectral filter algorithm
 *          w/multiple cells for calculating cutoff
 *
 * @version $Id: PsMultiSpecFilter.cpp 8257 2007-09-12 22:05:20Z swsides $
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
#include <PsMultiSpecFilter.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsMultiSpecFilter<FLOATTYPE, NDIM>::PsMultiSpecFilter() {

  totNumCells = 1;
  kcellMap = NULL;

  cutoffFactors.clear();
  cutoffFactorsTmp.clear();
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsMultiSpecFilter<FLOATTYPE, NDIM>::~PsMultiSpecFilter() {

  delete[] kcellMap;
  specCellSizes.clear();
  numSpecCells.clear();

  cutoffFactors.clear();
  cutoffFactorsTmp.clear();
}

template <class FLOATTYPE, size_t NDIM>
void PsMultiSpecFilter<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Scoping call to base class
  PsSpecFilterUpdater<FLOATTYPE, NDIM>::setAttrib(tas);
  this->dbprt("PsMultiSpecFilter::setAttrib() ");

  // Number of spectral cutoff cells in each direction
  if (tas.hasOptVec("specCellSizes")) {
    std::vector<int> cells;
    cells = tas.getOptVec("specCellSizes");
    for (size_t n=0; n<cells.size(); ++n) {
      specCellSizes.push_back(static_cast<size_t>(cells[n]));
    }
  }
  else {
    TxDebugExcept tde("PsMultiSpecFilter::setAttrib: specCellSizes not set");
    tde << " in <Updater " << this->getName() << " >";
    throw tde;
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsMultiSpecFilter<FLOATTYPE, NDIM>::buildData() {

  // Scoping call to base class
  PsSpecFilterUpdater<FLOATTYPE, NDIM>::buildData();
  this->dbprt("PsMultiSpecFilter::buildData() ");

  // Set the number of spectral cells
  std::vector<size_t> globalSize = this->getGridBase().getNumCellsGlobal();
  for (size_t n = 0; n<specCellSizes.size(); ++n) {
    size_t numcells = globalSize[n]/specCellSizes[n];
    numSpecCells.push_back(numcells);
  }

  // Determine total number of spec cells from list
  for (size_t n=0; n<specCellSizes.size(); ++n) {
    this->dbprt("numSpecCells[] = ", (int)numSpecCells[n]);
    totNumCells = totNumCells * numSpecCells[n];
  }

  this->dbprt("total #-spectral filter cells = ", (int)totNumCells);

  // Create cutoff lists with length set by number
  // of spectral cells needed
  cutoffFactors.resize(totNumCells, 0.0);
  cutoffFactorsTmp.resize(totNumCells, 0.0);
}

template <class FLOATTYPE, size_t NDIM>
void PsMultiSpecFilter<FLOATTYPE, NDIM>::buildSolvers() {

  // Scoping call to base class
  PsSpecFilterUpdater<FLOATTYPE, NDIM>::buildSolvers();
  this->dbprt("PsMultiSpecFilter::buildSolvers() ");

  // Transpose fft object (MPI/serial correct???)
  // Moved from applyFilter (kind is still hardwired to 'fftWTransposeObj'
  fftTransObjPtr =
    PsNamedObject::getObject<PsFFTBase<FLOATTYPE, NDIM> >("fftWTransposeObj");
  if (!fftTransObjPtr) {
    TxDebugExcept tde("PsMultiSpecFilter::applyFilter FFT transp ptr not set");
    tde << " in <PsMultiSpecFilter " << this->getName() << " >";
    throw tde;
  }

  // FFT grid object pointer
  PsGridBase<FLOATTYPE, NDIM>* fftTransGridPtr = &fftTransObjPtr->getGrid();

  // List of k space locations mapped to a spectral cell index
  // Need to do this here after fftObj obtained.
  fftSizeMulti = fftTransObjPtr->getFFTSize();
  kcellMap = new size_t[fftSizeMulti];

  // Build the kcellMap values
  build_specCells_transpose();
}

//
// Initialize before update step
// SWS: reorder this so initialize is only called at startup.....
//
template <class FLOATTYPE, size_t NDIM>
void PsMultiSpecFilter<FLOATTYPE, NDIM>::initialize() {

  // Scoping call to base class
  PsSpecFilterUpdater<FLOATTYPE, NDIM>::initialize();
}

template <class FLOATTYPE, size_t NDIM>
void PsMultiSpecFilter<FLOATTYPE, NDIM>::update(double t) {

  // Scoping call to base class
  PsSpecFilterUpdater<FLOATTYPE, NDIM>::update(t);
  this->dbprt("PsMultiSpecFilter::update() ");

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
// helper method to calculate list of spectral filter cells
//
template <class FLOATTYPE, size_t NDIM>
void PsMultiSpecFilter<FLOATTYPE, NDIM>::build_specCells_transpose() {

  // Initialize local "vec" lists
  std::vector<size_t> globalSize = this->getGridBase().getNumCellsGlobal();
  std::vector<size_t> icells;
  for (size_t i = 0; i<globalSize.size(); ++i) {
    icells.push_back(0);
  }

  // This is particular to slab layout (grid)
  PsGridBase<FLOATTYPE, NDIM>* fftGridPtr = &fftTransObjPtr->getGrid();
  std::vector<size_t> kDims  = fftGridPtr->getDecomp().getNumCellsLocal();
  std::vector<size_t> shifts = fftGridPtr->getDecomp().getLocalToGlobalShifts();
  size_t ny_trans      = kDims[1];
  size_t y_start_trans = shifts[1];

// Auxillary variables
  size_t nn = 0;
  size_t iregion = 0;
  size_t j;

// SWS: Is this pattern needed? Because array is kept "flat"
  for (size_t jloc = 0; jloc < ny_trans; ++jloc) {
    for (size_t i = 0; i < globalSize[0]; ++i) {
      for (size_t k = 0; k < globalSize[2]; ++k) {
// shift for parallel data mapping
        j = jloc + y_start_trans;

        icells[0] = floor( (FLOATTYPE)(i/specCellSizes[0]));
        icells[1] = floor( (FLOATTYPE)(j/specCellSizes[1]));
        icells[2] = floor( (FLOATTYPE)(k/specCellSizes[2]));

        iregion = ( icells[2]*(numSpecCells[0]*numSpecCells[1]) ) +
            (icells[1]* numSpecCells[0]) + icells[0];

        nn = (( (jloc*globalSize[0]) + i)*globalSize[2]) + k;
        kcellMap[nn] = iregion;

        this->dbprt("nn        = ", (int)nn);
        this->dbprt("icells[0] = ", (int)icells[0]);
        this->dbprt("icells[1] = ", (int)icells[1]);
        this->dbprt("icells[2] = ", (int)icells[2]);
        this->dbprt("kcellMap[nn] = ", (int)kcellMap[nn]);
      }
    }
  } // loop for k2

// Clean-up
  globalSize.clear();
  icells.clear();

} // transpose k2 build

// Helper method to filter... called from update
template <class FLOATTYPE, size_t NDIM>
void PsMultiSpecFilter<FLOATTYPE, NDIM>::applyFilter(PsFieldBase<FLOATTYPE>& wf) {

  this->dbprt("PsMultiSpecFilter::applyFilter ");

  // Reset counters
  for (size_t n=0; n<totNumCells; ++n) {
    cutoffFactors[n] = 0.0;
    cutoffFactorsTmp[n] = 0.0;
  }

  // Return absolute value of forward transform
  this->fftObjPtr->forwardFFTAbs(wf.getDataPtr(), this->resPtr);

  // Record maximum and apply cutoff for each region
  for (size_t n=0; n<fftSizeMulti; ++n) {
    size_t iregion = kcellMap[n];
    FLOATTYPE kCut = this->cutoffFactor*this->resPtr[n];
    if ( kCut > cutoffFactorsTmp[iregion] )
      cutoffFactorsTmp[iregion] = kCut;
  }

  // Gather maximum cutoffs in each cell
  cutoffFactors = this->getCommBase().allReduceMax(cutoffFactorsTmp);

  // Create mask array
  for (size_t n=0; n<fftSizeMulti; ++n) {

    size_t iregion = kcellMap[n];
    if (this->resPtr[n] < cutoffFactors[iregion])
      this->maskCutPtr[n] = this->filterStrength;
    else
      this->maskCutPtr[n] = 1.0;
  }

  // SWS: this repeats transform work... must refactor
  fftTransObjPtr->scaledFFTPair(wf.getDataPtr(),
      this->maskCutPtr, this->resPtr);

  // This should be through PsField interface SWS: (refactor needed)
  FLOATTYPE* wdata = wf.getDataPtr();
  for (size_t n=0; n<fftSizeMulti; ++n) {
    wdata[n] = this->scaleFFT*this->resPtr[n];
  }

}

// Instantiate classes
template class PsMultiSpecFilter<float, 1>;
template class PsMultiSpecFilter<float, 2>;
template class PsMultiSpecFilter<float, 3>;

template class PsMultiSpecFilter<double, 1>;
template class PsMultiSpecFilter<double, 2>;
template class PsMultiSpecFilter<double, 3>;
