/**
 *
 * @file    PsTinyMatrix.cpp
 *
 * @brief   Define implementation for the class, PsTinyMatrix.
 *
 * @version $Id: PsTinyMatrix.cpp 11576 2009-02-06 16:16:20Z gwerner $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

// std includes
#include <limits>

// psstd includes
#include <PsTinyMatrix.h>

/**
 * Methods in the .cpp file are determinant and inverse-related;
 * they apply only to square matices (and inverse really for
 * floating types only).
 *
 * Until more methods are needed that are inappropriate for
 * the .h file, it's best to keep these here and explicitly
 * instantiate only square matrices (and perhaps only
 * floating point TYPE).
 *
 * Moreover, these methods are not very good for matrices larger
 * than 3 x 3 (whereas methods in the .h file are just fine),
 * so if one doesn't explicitly instantiate larger matrices,
 * these functions (probably?) won't be available for those
 * sizes, which would be just fine.
 */

//
// calculate determinant
//
template <class TYPE, size_t MDIM, size_t NDIM>
TYPE PsTinyMatrix<TYPE, MDIM, NDIM>::det() const {
// make sure matrix is square
  PsCompileTimeAssert<MDIM == NDIM>();
// May want this if we ever have to compile this func for non-square
  if (MDIM != NDIM) {
    TxDebugExcept tde("PsTinyMatrix::det() error: "
        "can't take determinant of non-square matrix (");
    tde << MDIM << " x " << NDIM << ")";
    throw tde;
  }
  switch (MDIM) {
    case 0 : return 1.;
    case 1 : return (*this)[0];
    case 2 : return (*this)(0, 0)*(*this)(1, 1) -
      (*this)(0, 1)*(*this)(1, 0);
    case 3 : return (*this)(0, 0)*(
      (*this)(1, 1)*(*this)(2, 2) - (*this)(1, 2)*(*this)(2, 1))
      - (*this)(0, 1)*(
      (*this)(1, 0)*(*this)(2, 2) - (*this)(1, 2)*(*this)(2, 0))
      + (*this)(0, 2)*(
      (*this)(1, 0)*(*this)(2, 1) - (*this)(1, 1)*(*this)(2, 0));
      break;
    default : // this gets slow for MDIM > 3 (because of det(i, j))
      TYPE determ = 0.;
// construct submatrix, missing top row and one column
      for (size_t col=0; col<MDIM; ++col) {
        if (col & 1) determ -= (*this)(0, col) * this->det(0, col);
        else determ += (*this)(0, col) * this->det(0, col);
      }
      return determ;
  }
}

//
// calculate sub-determinant
//
template <class TYPE, size_t MDIM, size_t NDIM>
TYPE PsTinyMatrix<TYPE, MDIM, NDIM>::det(size_t i, size_t j) const {
// make sure matrix is square
  PsCompileTimeAssert<MDIM == NDIM>();
// May want this if we ever have to compile this func for non-square
  if (MDIM != NDIM) {
    TxDebugExcept tde("PsTinyMatrix::det(i, j) error: "
        "can't take determinant of non-square matrix (");
    tde << MDIM << " x " << NDIM << ")";
    throw tde;
  }
  if (i>=MDIM || j>=NDIM) {
    std::cerr << "PsTinyMatrix::det(i, j) error: "
        " i = " << i << ",  j = " << j << " out of range" <<
        std::endl;
  }
  switch (MDIM) {
    case 0 : return 1;
    case 1 : return (*this)[0];
    case 2 : return (*this)(1-i, 1-j);
    case 3 : {
      size_t row1 = (i+1)%3;
      size_t col1 = (j+1)%3;
      size_t row2 = (i+2)%3;
      size_t col2 = (j+2)%3;
// The submatrix with elements:
// [(row1, col1), (row1, col2); (row2, col1), (row2, col2)]
// rotated by i rows and j columns from the submatrix
// obtained by deleting row i and row j.
// Since the submatrix is 2x2 and rotating by 1 row or col
// is equivalent to 1 pair exchange of rows or cols,
// we have to multiply by (-1)^(i+j)
      if ((i+j) & 1) {
        return (*this)(row1, col2)*(*this)(row2, col1)
          -(*this)(row1, col1)*(*this)(row2, col2);
      }
      return (*this)(row1, col1)*(*this)(row2, col2)
          - (*this)(row1, col2)*(*this)(row2, col1);
    }
    default : {
// this gets slow: PsTinyMatrix isn't recommended
// subM(m, n) = (*this)((i+1+m) % MDIM), (j+1+n) % NDIM))
// i.e., rotate this matrix until row i is at bottom and col j is
// at right and then delete bottom row and right column
      PsTinyMatrix<TYPE, MDIM-1, NDIM-1> subM;
      for (size_t m=0; m<MDIM-1; ++m) {
        for (size_t n=0; n<NDIM-1; ++n) {
          subM(m, n) = (*this)((i+1+m) % MDIM, (j+1+n) % NDIM);
        }
      }
//subM complete

// if the sum-matrix has even dimensions, then rotation by
// 1 row or 1 col is equivalent to an odd number of pair row
// (or col) exchanges (which multiply the determinant by -1),
// and so we have to multiply by -1 times
// the number of row and col rotations, (-1)^{i+j} to
// get the determinant of the submatrix with row i and col j deleted.
      if (MDIM & 1) {
// MDIM is odd, MDIM-1 is even
        if ((i+j) & 1) return -subM.det();
        else return subM.det();
      }
// MDIM is even, MDIM-1 is odd
      return subM.det();
    }
  }
}

//
// Make inverse matrix
//
template <class TYPE, size_t MDIM, size_t NDIM>
PsTinyMatrix<TYPE, MDIM, MDIM>
PsTinyMatrix<TYPE, MDIM, NDIM>::inverse() const {
// make sure TYPE can handle division (is not an integer type)
  PsCompileTimeAssert<(!std::numeric_limits<TYPE>::is_integer)>();
// make sure matrix is square
  PsCompileTimeAssert<MDIM == NDIM>();
// May want this if we ever have to compile this func for non-square
  if (MDIM != NDIM) {
    TxDebugExcept tde("PsTinyMatrix::inverse() error: "
        "can't take determinant of non-square matrix (");
    tde << MDIM << " x " << NDIM << ")";
    throw tde;
  }
  PsTinyMatrix<TYPE, NDIM, MDIM> res;
/*
 TxSquareMatrix m(NDIM);
 for (size_t i=0; i<MDIM; ++i)
 for (size_t j=0; j<NDIM; ++j) {
 m(i, j) = (*this)(i, j);
 }
 m.invert();
 for (size_t i=0; i<MDIM; ++i)
 for (size_t j=0; j<NDIM; ++j) {
 res(i, j) = m(i, j);
 }
 */
  TYPE determ = this->det();
  if (determ == 0) {
    std::cerr << "PsTinyMatrix::inverse() error: "
        "trying to invert matrix with zero determinant..."
        "returning identity" << std::endl;
    res.setToIdentity();
    return res;
  }
  switch (MDIM) {
    case 1 : res[0] = 1;
      break;
    case 2 :
      res(0, 0) = (*this)(1, 1);
      res(0, 1) = -(*this)(0, 1);
      res(1, 0) = -(*this)(1, 0);
      res(1, 1) = (*this)(0, 0);
      break;
    default :
      for (size_t i=0; i<MDIM; ++i) {
        for (size_t j=0; j<MDIM; ++j) {
          if ((i+j) & 1) res(i, j) = -this->det(j, i);
          else res(i, j) = this->det(j, i);
        }
      }
  }
  res *= 1./determ;
  return res;
}

//
// Explicitly Instantiate only square, floating point types, of
// size 3x3 and larger (do we really need 1x1?)
//

template class PsTinyMatrix<float, 1, 1>;
template class PsTinyMatrix<float, 2, 2>;
template class PsTinyMatrix<float, 3, 3>;
// instantiate float 4x4 and 5x5 for testing
template class PsTinyMatrix<float, 4, 4>;
template class PsTinyMatrix<float, 5, 5>;

template class PsTinyMatrix<double, 1, 1>;
template class PsTinyMatrix<double, 2, 2>;
template class PsTinyMatrix<double, 3, 3>;

