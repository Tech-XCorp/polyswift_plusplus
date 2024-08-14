/**
 *
 * @file    PsGridField.cpp
 *
 * @brief   Base class for fields.
 *
 * @version $Id: PsGridField.cpp 8401 2007-09-26 20:17:05Z swsides $
 *
 * Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psbase includes
#include <PsGridField.h>

// psstd includes
#include <PsRandom.h>

// txbase includes
#include <TxMaker.h>

//
// Default constructor - sets to one component
//
template <class FLOATTYPE, size_t NRANK>
PsGridField<FLOATTYPE, NRANK>::PsGridField() {
  numComp = 1;
}

//
// destructor
//
template <class FLOATTYPE, size_t NRANK>
PsGridField<FLOATTYPE, NRANK>::~PsGridField() {
}

//
template <class FLOATTYPE, size_t NRANK>
PsGridBaseItr& PsGridField<FLOATTYPE, NRANK>::getGrid() const {

  if (!gridPtr) {
    TxDebugExcept tde("PsGridField:getGrid grid not set");
    std::cout << tde.getMessage() << std::endl;
    throw tde;
  }

  PsGridBaseItr& gridRef = *gridPtr;
  return gridRef;
}

//
template <class FLOATTYPE, size_t NRANK>
void PsGridField<FLOATTYPE, NRANK>::setGrid(PsGridBaseItr* gItr) {

  // Cast to correct grid object
  gridPtr = dynamic_cast< PsGridBase<FLOATTYPE, NRANK>* >(gItr);

  // Get field sizes from grid and build the PsField
  std::vector<size_t> fldLengths = gridPtr->getDecomp().getNumCellsLocal();
  this->setLengths(fldLengths);
  this->reset(0.0);
}

//
template <class FLOATTYPE, size_t NRANK>
void PsGridField<FLOATTYPE, NRANK>::calck2() {

  // Initialize local "vec" lists
  std::vector<size_t> globalSize = gridPtr->getNumCellsGlobal();
  std::vector<FLOATTYPE> n2vec, nkvec, kvec;
  FLOATTYPE n2, k2val;
  for (size_t i = 0; i<globalSize.size(); ++i) {
    n2 = (FLOATTYPE) globalSize[i] / 2.0;
    n2vec.push_back(n2);
    nkvec.push_back(0.0);
    kvec.push_back(0.0);
  }

  // Set k2 vals
  std::vector<FLOATTYPE> drvec = gridPtr->getCellSizes();
  for (size_t i = 0; i < globalSize[0]; ++i) {
    for (size_t j = 0; j< globalSize[1]; ++j) {
      for (size_t k = 0; k < globalSize[2]; ++k) {
        PsTinyVector<int, NRANK> posVec(i,j,k);

        nkvec[2] = n2vec[2] - std::abs(FLOATTYPE(k) - n2vec[2] );
        nkvec[1] = n2vec[1] - std::abs(FLOATTYPE(j) - n2vec[1] );
        nkvec[0] = n2vec[0] - std::abs(FLOATTYPE(i) - n2vec[0] );

        kvec[2] = mksConsts.twopi*nkvec[2]/FLOATTYPE(globalSize[2]);
        kvec[1] = mksConsts.twopi*nkvec[1]/FLOATTYPE(globalSize[1]);
        kvec[0] = mksConsts.twopi*nkvec[0]/FLOATTYPE(globalSize[0]);

        kvec[2] = kvec[2]/drvec[2];
        kvec[1] = kvec[1]/drvec[1];
        kvec[0] = kvec[0]/drvec[0];

// w/segment length ratio scaling
        k2val = kvec[0]*kvec[0] + kvec[1]*kvec[1] + kvec[2]*kvec[2];
        this->mapToLocalField(posVec, k2val, "set");
      }
    }
  } // loop for k2
}

//
template <class FLOATTYPE, size_t NRANK>
void PsGridField<FLOATTYPE, NRANK>::mapToLocalField(
       PsTinyVector<int, NRANK>& posvec,
       FLOATTYPE val,
       std::string sendFlag) {

  // Decomp info
  PsDecompBase<FLOATTYPE, NRANK>& decompObj = gridPtr->getDecomp();
  bool pointOwned = decompObj.hasPosition(posvec);
  std::vector<size_t> l2g = decompObj.getLocalToGlobalShifts();

  if (pointOwned) {

    size_t xloc = (size_t)posvec[0] - l2g[0];
    size_t yloc = (size_t)posvec[1] - l2g[1];
    size_t zloc = (size_t)posvec[2] - l2g[2];

    if (sendFlag == "add") {
      operator()(xloc, yloc, zloc, 0) =
      operator()(xloc, yloc, zloc, 0) + val;
    }
    else if (sendFlag == "set") {
      operator()(xloc, yloc, zloc, 0) = val;
    }
    else {
      TxDebugExcept tde("PsGridField::mapToLocalField, sendFlag not recoginzed");
      std::cout << tde.getMessage() << std::endl;
      throw tde;
    }
  }

}

//
template <class FLOATTYPE, size_t NRANK>
void PsGridField<FLOATTYPE, NRANK>::calckVec() {

  // Initialize local "vec" lists and field
  std::vector<size_t> globalSize = gridPtr->getNumCellsGlobal();
  std::vector<FLOATTYPE> n2vec, nkvec, kvec;
  for (size_t i = 0; i<globalSize.size(); ++i) {
    FLOATTYPE n2 = (FLOATTYPE) globalSize[i] / 2.0;
    n2vec.push_back(n2);
    nkvec.push_back(0.0);
    kvec.push_back(0.0);
  }

  // Global placeholder list for kvector field
  PsGridField<FLOATTYPE, NRANK> psf3d;
  psf3d.setLengths(globalSize);
  psf3d.setNumElements(globalSize.size());
  psf3d.reset(0.0);

  // Initialize ik "Gradient" factors in global list psf3d
  std::vector<FLOATTYPE> drvec = gridPtr->getCellSizes();
  for (size_t i=0; i < globalSize[0]; ++i) {
  for (size_t j=0; j < globalSize[1]; ++j) {
  for (size_t k=0; k < globalSize[2]; ++k) {

    nkvec[2] = n2vec[2] - std::abs(FLOATTYPE(k) - n2vec[2] );
    nkvec[1] = n2vec[1] - std::abs(FLOATTYPE(j) - n2vec[1] );
    nkvec[0] = n2vec[0] - std::abs(FLOATTYPE(i) - n2vec[0] );

    if (k > n2vec[2]) nkvec[2] = -nkvec[2];
    if (j > n2vec[1]) nkvec[1] = -nkvec[1];
    if (i > n2vec[0]) nkvec[0] = -nkvec[0];

    kvec[2] = mksConsts.twopi*nkvec[2]/FLOATTYPE(globalSize[2]);
    kvec[1] = mksConsts.twopi*nkvec[1]/FLOATTYPE(globalSize[1]);
    kvec[0] = mksConsts.twopi*nkvec[0]/FLOATTYPE(globalSize[0]);
    kvec[2] = kvec[2]/drvec[2];
    kvec[1] = kvec[1]/drvec[1];
    kvec[0] = kvec[0]/drvec[0];

    // Removing (-) factor here because transform definition has
    // sign difference from Mathematica
    psf3d(i, j, k, 2) = (FLOATTYPE)kvec[2];
    psf3d(i, j, k, 1) = (FLOATTYPE)kvec[1];
    psf3d(i, j, k, 0) = (FLOATTYPE)kvec[0];

  }}} // loop for ikvec

  // Clean-up
  drvec.clear();
  n2vec.clear();
  nkvec.clear();
  kvec.clear();

  // Local tmp space (single components)
  PsGridBaseItr* gItr = gridPtr;
  PsGridField<FLOATTYPE, NRANK> kxLocalFld;
  PsGridField<FLOATTYPE, NRANK> kyLocalFld;
  PsGridField<FLOATTYPE, NRANK> kzLocalFld;
  kxLocalFld.setGrid(gItr);
  kyLocalFld.setGrid(gItr);
  kzLocalFld.setGrid(gItr);

  // Map to local kvector fields
  for (size_t x=0; x<globalSize[0]; ++x) {
  for (size_t y=0; y<globalSize[1]; ++y) {
  for (size_t z=0; z<globalSize[2]; ++z) {

    PsTinyVector<int, NRANK> posVec(x,y,z);
    FLOATTYPE kxval = psf3d(posVec[0], posVec[1], posVec[2], 0);
    FLOATTYPE kyval = psf3d(posVec[0], posVec[1], posVec[2], 1);
    FLOATTYPE kzval = psf3d(posVec[0], posVec[1], posVec[2], 2);

    kxLocalFld.mapToLocalField(posVec, kxval, "set");
    kyLocalFld.mapToLocalField(posVec, kyval, "set");
    kzLocalFld.mapToLocalField(posVec, kzval, "set");
  }}}

  // Populate multi-component psfield
  std::vector<size_t> localDims = this->getLengths();
  for (size_t x=0; x<localDims[0]; ++x) {
  for (size_t y=0; y<localDims[1]; ++y) {
  for (size_t z=0; z<localDims[2]; ++z) {
    operator()(x, y, z, 0) = kxLocalFld(x, y, z, 0);
    operator()(x, y, z, 1) = kyLocalFld(x, y, z, 0);
    operator()(x, y, z, 2) = kzLocalFld(x, y, z, 0);
  }}}

}

template <class FLOATTYPE, size_t NRANK>
void PsGridField<FLOATTYPE, NRANK>::reset(double val) {
  for (size_t i=0; i<getSize(); ++i)
    operator()(i) = val;
}

/*
template <class FLOATTYPE, size_t NRANK>
void PsGridField<FLOATTYPE, NRANK>::add(FLOATTYPE val) {

  FLOATTYPE tmp;
  for (size_t i=0; i<getSize(); ++i) {
    tmp = operator()(i) + val;
    operator()(i) = tmp;
  }
}
*/

template <class FLOATTYPE, size_t NRANK>
void PsGridField<FLOATTYPE, NRANK>::scale(double val) {

  double tmp;
  for (size_t i=0; i<getSize(); ++i) {
    tmp = val*operator()(i);
    operator()(i) = tmp;
  }
}

template <class FLOATTYPE, size_t NRANK>
bool PsGridField<FLOATTYPE, NRANK>::checkMaxClip(double maxVal, double clipVal) {

  double tmp;
  bool maxFound=false;

  for (size_t i=0; i<getSize(); ++i) {
    tmp = operator()(i);
    if (tmp > maxVal)  maxFound=true;
    if (tmp > clipVal) operator()(i) = clipVal;
  }

  return maxFound;
}

template <class FLOATTYPE, size_t NRANK>
FLOATTYPE PsGridField<FLOATTYPE, NRANK>::minVal() {

  FLOATTYPE minValFound = std::numeric_limits<FLOATTYPE>::max();

  for (size_t i=0; i<getSize(); ++i) {
    FLOATTYPE val = operator()(i);
    if (val < minValFound) minValFound=val;
  }

  return minValFound;
}

template <class FLOATTYPE, size_t NRANK>
FLOATTYPE PsGridField<FLOATTYPE, NRANK>::maxVal() {

  FLOATTYPE maxValFound = std::numeric_limits<FLOATTYPE>::min();

  for (size_t i=0; i<getSize(); ++i) {
    FLOATTYPE val = operator()(i);
    if (val > maxValFound) maxValFound=val;
  }

  return maxValFound;
}

template <class FLOATTYPE, size_t NRANK>
FLOATTYPE PsGridField<FLOATTYPE, NRANK>::getSumAll() {

  FLOATTYPE tot = 0.0;
  FLOATTYPE val = 0.0;

  for (size_t i=0; i<getSize(); ++i) {
    val = operator()(i);
    tot = tot + val;
  }
  return tot;
}

template <class FLOATTYPE, size_t NRANK>
void PsGridField<FLOATTYPE, NRANK>::apply_exp() {

  double tmp;
  FLOATTYPE tmp1;

  FLOATTYPE* dataPtr = getDataPtr();
  size_t dataPtrSize = getSize();

  for (size_t i=0; i<dataPtrSize; ++i) {
    tmp = (double) dataPtr[i];
    tmp1 = (FLOATTYPE) std::exp(tmp);
    dataPtr[i] = tmp1;
  }

}

template <class FLOATTYPE, size_t NRANK>
void PsGridField<FLOATTYPE, NRANK>::setLengths(size_t lens[NRANK]) {

  size_t sizeLens[NRANK+1];
  for (size_t i=0; i<NRANK; ++i) {
    sizeLens[i] = lens[i];
  }

  sizeLens[NRANK] = 1;
  PrTensor<FLOATTYPE, NRANK+1>::setLengths(sizeLens);
}

template <class FLOATTYPE, size_t NRANK>
void PsGridField<FLOATTYPE, NRANK>::setLengths(std::vector<size_t> lensVec) {

  size_t lensVecSize = lensVec.size();
  size_t* sizeLensPtr;
  sizeLensPtr = new size_t[lensVecSize+1];

  for (size_t i=0; i<lensVecSize; ++i) {
    sizeLensPtr[i] = lensVec[i];
  }

  sizeLensPtr[NRANK] = 1;
  PrTensor<FLOATTYPE, NRANK+1>::setLengths(sizeLensPtr);

  delete[] sizeLensPtr;
}

template <class FLOATTYPE, size_t NRANK>
void PsGridField<FLOATTYPE, NRANK>::setLengths(size_t lensScalar) {

  size_t lensVecSize = 1;
  size_t* sizeLensPtr;
  sizeLensPtr = new size_t[lensVecSize+1];
  sizeLensPtr[0] = lensScalar;

  sizeLensPtr[NRANK] = 1;
  PrTensor<FLOATTYPE, NRANK+1>::setLengths(sizeLensPtr);

  delete[] sizeLensPtr;
}

template <class FLOATTYPE, size_t NRANK>
std::vector<size_t> PsGridField<FLOATTYPE, NRANK>::getLengths() {

  size_t tRank = this->tensorRank;
  size_t dimLength;
  std::vector<size_t> dimLengths;

  for (size_t n=0; n<tRank; ++n) {
    dimLength = PrTensor<FLOATTYPE, NRANK+1>::getLength(n);
    dimLengths.push_back(dimLength);
  }

  return dimLengths;
}

template <class FLOATTYPE, size_t NRANK>
void PsGridField<FLOATTYPE, NRANK>::resetRandom(FLOATTYPE scale, FLOATTYPE shift) {

  // Note: these separate calls to psRandom are
  // probably not optimized... and not checking
  // if PsRandom has been set!!!

  FLOATTYPE randNum = 0.0;

  for (size_t i=0; i<getSize(); ++i) {
    randNum = psRandom<FLOATTYPE>();
    operator()(i) = scale*(randNum + shift);
  }
}

template <class FLOATTYPE, size_t NRANK>
void PsGridField<FLOATTYPE, NRANK>::addRandom(FLOATTYPE scale) {

  // Note: these separate calls to psRandom are
  // probably not optimized... and not checking
  // if PsRandom has been set!!!

  FLOATTYPE tmpRan = 0.0;
  FLOATTYPE tmpFld = 0.0;

  for (size_t i=0; i<getSize(); ++i) {
    tmpFld = operator()(i);
    tmpRan = scale*psRandomGaussian<FLOATTYPE>();
    operator()(i) = tmpFld + tmpRan;
  }
}

/*
template <class FLOATTYPE, size_t NRANK>
void PsGridField<FLOATTYPE, NRANK>::testAssign(PsFieldBase<FLOATTYPE>* psfb) {
  size_t psSize = psfb->getSize();
  //std::cout << "From PsGridField::testAssign psSize = " << psSize << std::endl;
}
*/

//
// Add the scalar elementwise to this object's data
//
template <class FLOATTYPE, size_t NRANK>
void PsGridField<FLOATTYPE, NRANK>::operator+=(FLOATTYPE c) {

  FLOATTYPE* dataPtr = getDataPtr();
  size_t dataPtrSize = getSize();

  for (size_t i=0; i<dataPtrSize; ++i) {
    dataPtr[i] = dataPtr[i] + c;
  }
}

//
// Add the scalar elementwise to this object's data
//
template <class FLOATTYPE, size_t NRANK>
void PsGridField<FLOATTYPE, NRANK>::operator-=(FLOATTYPE c) {

  FLOATTYPE* dataPtr = getDataPtr();
  size_t dataPtrSize = getSize();

  for (size_t i=0; i<dataPtrSize; ++i) {
    dataPtr[i] = dataPtr[i] - c;
  }
}

//
// Multiply the scalar elementwise to this object's data
//
template <class FLOATTYPE, size_t NRANK>
void PsGridField<FLOATTYPE, NRANK>::operator*=(FLOATTYPE c) {

  FLOATTYPE* dataPtr = getDataPtr();
  size_t dataPtrSize = getSize();

  for (size_t i=0; i<dataPtrSize; ++i) {
    dataPtr[i] = c*dataPtr[i];
  }
}

//
// Add the main data in psf to this object
//
template <class FLOATTYPE, size_t NRANK>
void PsGridField<FLOATTYPE, NRANK>::operator+=(PsFieldBase<FLOATTYPE>& psf) {

  const FLOATTYPE* psfPtr = psf.getConstDataPtr();
  FLOATTYPE* dataPtr = getDataPtr();

  size_t psfPtrSize = psf.getSize();
  size_t dataPtrSize = getSize();

  if ( psfPtrSize != dataPtrSize) {
    TxDebugExcept tde("PsGridField::operator += not defined for");
    tde <<  " different rank/sizes";
    tde << " in PsFieldBase" << " >";
    tde << "psfPtrSize  = " << psfPtrSize  << "\n";
    tde << "dataPtrSize = " << dataPtrSize << "\n";
    std::cout << tde.getMessage() << std::endl;
    throw tde;
  }

  for (size_t i=0; i<dataPtrSize; ++i) {
    dataPtr[i] = dataPtr[i] + psfPtr[i];
  }
}

//
// Add the main data in psf to this object
//
template <class FLOATTYPE, size_t NRANK>
void PsGridField<FLOATTYPE, NRANK>::operator-=(PsFieldBase<FLOATTYPE>& psf) {

  const FLOATTYPE* psfPtr = psf.getConstDataPtr();
  FLOATTYPE* dataPtr = getDataPtr();

  size_t psfPtrSize = psf.getSize();
  size_t dataPtrSize = getSize();

  if ( psfPtrSize != dataPtrSize) {
    TxDebugExcept tde("PsGridField::operator -= not defined for");
    tde <<  " different rank/sizes";
    tde << " in PsFieldBase" << " >";
    std::cout << tde.getMessage() << std::endl;
    throw tde;
  }

  for (size_t i=0; i<dataPtrSize; ++i) {
    dataPtr[i] = dataPtr[i] - psfPtr[i];
  }
}

//
// Multiply elementwise the main data in psf to this object
//
template <class FLOATTYPE, size_t NRANK>
void PsGridField<FLOATTYPE, NRANK>::operator*=(PsFieldBase<FLOATTYPE>& psf) {

  const FLOATTYPE* psfPtr = psf.getConstDataPtr();
  FLOATTYPE* dataPtr      = getDataPtr();
  size_t psfPtrSize  = psf.getSize();
  size_t dataPtrSize = getSize();

  if ( psfPtrSize != dataPtrSize) {
    TxDebugExcept tde("PsGridField::operator *= not defined for");
    tde <<  " different rank/sizes in PsFieldBase";
    std::cout << tde.getMessage() << std::endl;
    throw tde;
  }

  for (size_t i=0; i<dataPtrSize; ++i) {
    dataPtr[i] = dataPtr[i] * psfPtr[i];
  }
}

// Instanitate (remember to change in map)
template class PsGridField<float, 1>;
template class PsGridField<float, 2>;
template class PsGridField<float, 3>;
template class PsGridField<float, 4>;
template class PsGridField<float, 5>;
template class PsGridField<float, 6>;

template class PsGridField<double, 1>;
template class PsGridField<double, 2>;
template class PsGridField<double, 3>;
template class PsGridField<double, 4>;
template class PsGridField<double, 5>;
template class PsGridField<double, 6>;

