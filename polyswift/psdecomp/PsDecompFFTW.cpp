/**
 *
 * @file    PsDecompFFTW.cpp
 *
 * @brief   Class containing info for slab decomposition from FFTW
 *
 * @version $Id: PsDecompRegular.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2009-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psdecomp includes
#include <PsDecompFFTW.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsDecompFFTW<FLOATTYPE, NDIM>::PsDecompFFTW() {

  // Defaults to NORMAL data layout
  transposeFlag = false;
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsDecompFFTW<FLOATTYPE, NDIM>::~PsDecompFFTW() {

  localToGlobalShifts.clear();
}

template <class FLOATTYPE, size_t NDIM>
void PsDecompFFTW<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Scoping call to base class
  PsDecomp<FLOATTYPE, NDIM>::setAttrib(tas);

  // Debug flag
  if (tas.hasString("transposeFlag")) {
    std::string tStr = tas.getString("transposeFlag");
    if (tStr == "on") transposeFlag = true;
  }

}

//
// Single build method... for special objects
//
template <class FLOATTYPE, size_t NDIM>
void PsDecompFFTW<FLOATTYPE, NDIM>::build() {

  // Scoping call to base class
  PsDecomp<FLOATTYPE, NDIM>::build();
  this->dbprt("PsDecompFFTW::build() ");

  // Get grid method pointer (default for grid as only global needed)
  PsGridBase<FLOATTYPE, NDIM>* gridObjPtr = &this->getGridBase();

  // *************************************************
  // Local dims for FFTW plans
  // Setting defaults (even for serial) so
  // these can be used universally to setup data structures
  // *************************************************

  /** Global size in each dimension for FFTW data */
  int* planDims;

  /** Global sizes for transpose for FFTW data */
  int* planTransposeDims;

  /** Extent of first dimension for FFTW slab decomp */
  int local_nx;

  /** Extent of transposed 1st dimension for FFTW slab decomp */
  int local_ny_after_transpose;

  /** Index of local pos in 1st dimension for FFTW slab decomp */
  int local_x_start = 0;

  /**
   * Index of local position in transposed first dimension
   * for FFTW slab decomp
   */
  int local_y_start_after_transpose = 0;

  /** Total number of local elements in data */
  int total_local_size = 1;

  // *************************************************
  // Set rank/dimensions for plans
  // *************************************************
  int rank;
  std::vector<size_t> dims;
  dims = gridObjPtr->getNumCellsGlobal();
  rank = (int)dims.size();

  // Translate std::vector to int pointer
  // and special transpose plan for backward MPI FFT
  planDims = new int[rank];
  for (int n=0; n<rank; ++n) {
    planDims[n] = dims[n];
  }

  #ifndef HAVE_MPI
  //
  // Setting FFTW size info from global
  // so these can be used to setup data structures
  //
  local_nx = planDims[0];
  local_ny_after_transpose = planDims[1];
  for (int n=0; n<rank; ++n) {
    total_local_size = total_local_size * planDims[n];
  }
  #endif // SERIAL

  #ifdef HAVE_MPI

  /** FFTW plan for forward transforms */
  fftwnd_mpi_plan forwardPlan =
      fftwnd_mpi_create_plan(MPI_COMM_WORLD,
      rank, planDims, FFTW_FORWARD, FFTW_ESTIMATE);

  // Set FFTW decomp parameters
  fftwnd_mpi_local_sizes(forwardPlan,
      &local_nx, &local_x_start, &local_ny_after_transpose,
      &local_y_start_after_transpose, &total_local_size);

  #endif // PARALLEL

  // Get decomp info from FFTW object
  if (transposeFlag) {

    // For plans with TRANSPOSED_ORDER
    localToGlobalShifts.push_back(0);
    localToGlobalShifts.push_back(local_y_start_after_transpose);
    localToGlobalShifts.push_back(0);

    this->numCellsLocal = gridObjPtr->getNumCellsGlobal();
    this->numCellsLocal[1] = local_ny_after_transpose;

  }
  else {

    // For plans with NORMAL_ORDER
    localToGlobalShifts.push_back(local_x_start);
    localToGlobalShifts.push_back(0);
    localToGlobalShifts.push_back(0);

    this->numCellsLocal = gridObjPtr->getNumCellsGlobal();
    this->numCellsLocal[0] = local_nx;

  }

}

//
// overloaded for vector of int's
// Needs to be generalized for transpose decomp
//
template <class FLOATTYPE, size_t NDIM>
bool PsDecompFFTW<FLOATTYPE, NDIM>::hasPosition(
       std::vector<int> globalPos) {

  size_t localxStart = localToGlobalShifts[0];
  size_t localyStart = localToGlobalShifts[1];
  size_t localxEnd = localxStart + this->numCellsLocal[0];
  size_t localyEnd = localyStart + this->numCellsLocal[1];

  bool inX, inY;

  // Inside x slab
  if ( ( (int)localxStart <= globalPos[0]) &&
       ( globalPos[0] < (int)localxEnd) ) {
    inX = true;
  }
  else {
    inX = false;
  }

  // Inside y slab
  if ( ( (int)localyStart <= globalPos[1]) &&
       ( globalPos[1] < (int)localyEnd) ) {
    inY = true;
  }
  else {
    inY = false;
  }

  // Return true if in x slab and y slab
  if (inX && inY) {
    return true;
  }
  else {
    return false;
  }

}

//
// overloaded for tiny vectors
// Needs to be generalized for transpose decomp
//
template <class FLOATTYPE, size_t NDIM>
bool PsDecompFFTW<FLOATTYPE, NDIM>::hasPosition(
       PsTinyVector<int, NDIM> globalPos) {

  size_t localxStart = localToGlobalShifts[0];
  size_t localyStart = localToGlobalShifts[1];
  size_t localxEnd = localxStart + this->numCellsLocal[0];
  size_t localyEnd = localyStart + this->numCellsLocal[1];

  bool inX, inY;

  // Inside x slab
  if ( ( (int)localxStart <= globalPos[0]) &&
       ( globalPos[0] < (int)localxEnd) ) {
    inX = true;
  }
  else {
    inX = false;
  }

  // Inside y slab
  if ( ( (int)localyStart <= globalPos[1]) &&
       ( globalPos[1] < (int)localyEnd) ) {
    inY = true;
  }
  else {
    inY = false;
  }

  // Return true if in x slab and y slab
  if (inX && inY) {
    return true;
  }
  else {
    return false;
  }

}

// Instantiate
template class PsDecompFFTW<float, 1>;
template class PsDecompFFTW<float, 2>;
template class PsDecompFFTW<float, 3>;

template class PsDecompFFTW<double, 1>;
template class PsDecompFFTW<double, 2>;
template class PsDecompFFTW<double, 3>;
