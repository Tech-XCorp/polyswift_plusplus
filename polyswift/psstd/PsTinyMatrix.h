/**
 *
 * @file    PsTinyMatrix.h
 *
 * @brief   Define interface for the class PsTinyMatrix.
 *
 * @version $Id: PsTinyMatrix.h 11428 2009-01-22 17:53:35Z dws $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

#ifndef PS_TINY_MATRIX_H
#define PS_TINY_MATRIX_H

// std includes
#include <vector>
#include <cmath>
#include <iostream>
#include <iomanip>


// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// txbase includes
#include <TxDebugExcept.h>

// vpstd includes
#include <PsTinyVector.h>
#include <PsCompileTimeAssert.h>

/**
 * @class PsTinyMatrix
 *
 * @brief Class for representing small groups of data, like
 * the dielectric tensor that can then be manipulated as a
 * standard mathematical rank-2 tensor.
 *
 * PsTinyMatrix is a templated class, templated over both the numeric
 * type of the elements and the matrix dimensions.
 *
 * The methods provide an interface like that of a matrix.
 *
 * The determinant and inverse methods are straightforward
 * computations, which will be slow for matrices larger than
 * 3x3, as well as inaccurate for larger matrices.
 *
 * There's a specialization below to NDIM = 0, for the sole
 * purpose of preventing endless recursion during compile.
 *
 * @param TYPE numeric type of the data (best if floatign point).
 * @param MDIM number of rows of matrix.
 * @param NDIM number of columns of matrix.
 */

template <class TYPE, size_t MDIM, size_t NDIM>
class PsTinyMatrix {

  public:

//
// Constructors, destructors, assignment
//

/**
 * Default constructor -- NO INITIALIZATION
 */
    PsTinyMatrix() {}

/**
 * Uniform Initialization Constructor
 *
 * @param initVal initial value for all elements
 */
    PsTinyMatrix(TYPE initVal) {
      for (size_t i=0; i<size(); ++i) (*this)[i] = initVal;
    }

/**
 * Copy constructor
 *
 * @param tv the vector to copy from
 */
    PsTinyMatrix(const PsTinyMatrix& tv) {
      for (size_t i=0; i<size(); ++i) (*this)[i] = tv[i];
    }

/**
 * Create from a regular MxN 2D array
 *
 * @param v the 2D array to copy from
 */
    PsTinyMatrix(const TYPE v[MDIM][NDIM]) {
      for (size_t i=0; i<size(); ++i)
        (*this)[i] = reinterpret_cast<const TYPE*>(v)[i];
    }

/**
 * Assignment from another matrix
 *
 * @param tv the matrix that will overwrite the local data
 *
 * @return local matrix after all data has been overwritten
 */
    PsTinyMatrix& operator=(const PsTinyMatrix& tv) {
      for (size_t i=0; i<size(); ++i) (*this)[i] = tv[i];
      return *this;
    }

/**
 * set all values of this matrix to x
 * @param x value to use
 */
    PsTinyMatrix& operator=(TYPE x) {
      for (size_t i=0; i<size(); ++i) (*this)[i] = x;
      return *this;
    }

//
// Accessors
//

/**
 * Define the[] operator to provide 1D access to the elements.
 *
 * @param i the index of the desired element (0 <= i < MDIM*NDIM)
 *          with no bounds checking
 * @return the desired element by reference
 */
    TYPE& operator[](size_t i) {
      return reinterpret_cast<TYPE*>(data)[i];
    }

/**
 * Define the () operator to provide 2D access to the elements.
 *
 * @param i the row of the desired element (0 <= i < MDIM)
 *          with no bounds checking
 * @param j the column of the desired element (0 <= j < NDIM)
 *          with no bounds checking
 * @return the desired element by reference
 */
    TYPE& operator()(size_t i, size_t j) {
      return data[i][j];
    }

/**
 * Define the[] operator to provide 1D access to the elements.
 *
 * @param i the index of the desired element (0 <= i < MDIM*NDIM)
 *          with no bounds checking
 * @return the desired element by reference
 */
    TYPE operator[](size_t i) const {
      return reinterpret_cast<const TYPE*>(data)[i];
    }

/**
 * Define the () operator to provide 2D access to the elements.
 *
 * @param i the row of the desired element (0 <= i < MDIM)
 *          with no bounds checking
 * @param j the column of the desired element (0 <= j < NDIM)
 *          with no bounds checking
 * @return the desired element by reference
 */
    TYPE operator()(size_t i, size_t j) const {
      return data[i][j];
    }

/**
 * comparison operator
 */
    bool operator<(const PsTinyMatrix& tv) const {
      for (size_t i=0;i<size();++i) {
      if (operator[](i)<tv[i])
        return true;
      if (operator[](i)>tv[i])
        return false;
      }
      return false;
    }

/**
 * comparison operator
 */
    bool operator>(const PsTinyMatrix& tv) const {
      for (size_t i=0;i<size();++i) {
      if (operator[](i)<tv[i])
        return false;
      if (operator[](i)>tv[i])
        return true;
      }
      return false;
    }

/**
 * comparison operator
 */
    bool operator>=(const PsTinyMatrix& tv) const {
      for (size_t i=0;i<size();++i) {
      if (operator[](i)<tv[i])
        return false;
      if (operator[](i)>tv[i])
        return true;
      }
      return true;
    }

/**
 * equivalence operator
 */
    bool operator==(const PsTinyMatrix& tv) const {
      for (size_t i=0;i<size();++i) {
        if (operator[](i)!=tv[i]) return false;
      }
      return true;
    }

/**
 * non equivalence operator
 */
    bool operator!=(const PsTinyMatrix& tv) const {
      return !(operator==(tv));
    }

/**
 * Define the unary - operator for a PsTinyMatrix object.
 *
 * @return the negative of the matrix
 */
    PsTinyMatrix operator-() const {
     PsTinyMatrix res;
     for (size_t i=0; i<size(); ++i) res[i] = - (*this)[i];
     return res;
    }

/**
 * Define the + operator for adding to PsTinyMatrix objects.
 *
 * @param tv the matrix which is being added
 *
 * @return the element-by-element sum of the two matrices
 */
    PsTinyMatrix operator+(const PsTinyMatrix& tv) const {
     PsTinyMatrix res;
     for (size_t i=0; i<size(); ++i) res[i] = (*this)[i] + tv[i];
     return res;
    }

/**
 * Define the - operator for substracting two PsTinyMatrix objects.
 *
 * @param tv the matrix which is being added
 *
 * @return the element-by-element sum of the two matrices
 */
    PsTinyMatrix operator-(const PsTinyMatrix& tv) const {
     PsTinyMatrix res;
     for (size_t i=0; i<size(); ++i) res[i] = (*this)[i] - tv[i];
     return res;
    }

/**
 * Define the += operator for setting the local PsTinyMatrix object
 *   equal to the sum of itself plus another.
 *
 * @param tv the matrix that will be added
 *
 * @return local matrix after all data has been modified by addition
 */
    PsTinyMatrix& operator+=(const PsTinyMatrix& tv) {
      for (size_t i=0; i<size(); ++i) (*this)[i] += tv[i];
      return *this;
    }

/**
 * Define the -= operator for setting the local PsTinyMatrix object
 *   equal to the sum of itself plus another.
 *
 * @param tv the matrix that will be subtracted
 *
 * @return local matrix after all data has been modified by subtraction
 */
    PsTinyMatrix& operator-=(const PsTinyMatrix& tv) {
      for (size_t i=0; i<size(); ++i) (*this)[i] -= tv[i];
      return *this;
    }

/**
 * Define the * operator for multiplication by a scalar.
 *
 * @param fac the scalar that will be multiplied
 *
 * @return local matrix after all data has been multiplied
 */
    PsTinyMatrix& operator*=(TYPE fac) {
      for (size_t i=0; i<size(); ++i) (*this)[i] *= fac;
      return *this;
    }

/**
 * Define the * operator for multiplication by a scalar.
 *
 * @param fac the scalar that will be multiplied
 *
 * @return local matrix after all data has been multiplied
 */
    PsTinyMatrix operator*(TYPE fac) const {
      PsTinyMatrix res;
      for (size_t i=0; i<size(); ++i) res[i] = fac*(*this)[i];
      return res;
    }

/**
 * calculate matrix transpose
 *
 * @return transpose of matrix
 */
    PsTinyMatrix<TYPE, NDIM, MDIM> transpose() {
      PsTinyMatrix<TYPE, NDIM, MDIM> ret;
      for (size_t i=0; i<MDIM; ++i) {
        for (size_t j=0; j<NDIM; ++j) {
          ret(j, i) = (*this)(i, j);
        }
      }
      return ret;
    }

/**
 * calculate determinant
 *
 * @return determinant of matrix
 */
    TYPE det() const;

/**
 * calculate sub-determinant (determinant of matrix with
 * its i^th row and j^th column removed)
 *
 * @param i row to omit
 * @param j column to omit
 * @return determinant of matrix, with row i and col j deleted
 */
    TYPE det(size_t i, size_t j) const;

/**
 * Make inverse matrix
 * (really needs TYPE to be floating point)
 *
 * @return inverse of this matrix
 */
    PsTinyMatrix<TYPE, MDIM, MDIM> inverse() const;

/**
 * Define * as (left)matrix-vector multiplication
 *
 * @param tv TinyVector to multiply by this matrix
 *
 * @return TinyVector, result of matrix*vector
 */
    PsTinyVector<TYPE, MDIM> operator*(
        const PsTinyVector<TYPE, NDIM>& tv) const {

      PsTinyVector<TYPE, MDIM> res;
// mistake if res isn't initialized to zero
      for (size_t i=0; i<MDIM; ++i) {
        for (size_t j=0; j<NDIM; ++j) {
          res[i] += (*this)(i, j) * tv[j];
        }
      }
      return res;
    }

/**
 * Define * as matrix-matrix multiplication
 *
 * @param tm TinyMatrix by which to (right)multiply this matrix
 *
 * @return matrix product
 */
    template <size_t ODIM>
    PsTinyMatrix<TYPE, MDIM, ODIM> operator*(
        const PsTinyMatrix<TYPE, NDIM, ODIM>& tm) const {

      PsTinyMatrix<TYPE, MDIM, ODIM> res;
      for (size_t i=0; i<MDIM; ++i) {
        for (size_t k=0; k<ODIM; ++k) {
          res(i, k) = 0;
          for (size_t j=0; j<NDIM; ++j)
            res(i, k) += operator()(i, j) * tm(j, k);
        }
      }
      return res;
    }

/**
 * create a dyad, given two vectors: M(i, j) = v1(i) * v2(j)
 *
 * E.g., if v is a unit vector, then dyad(v, v) is the
 * projection matrix projecting onto the space spanned by v.
 *
 * @param tv1 first (M dimensional) TinyVector
 * @param tv2 second (N dimensional) TinyVector
 *
 * @return (M x N) TinyMatrix: M(i, j) = v1(i) * v2(j)
 */
    static PsTinyMatrix<TYPE, MDIM, NDIM> dyad(
        const PsTinyVector<TYPE, MDIM>& tv1,
        const PsTinyVector<TYPE, NDIM>& tv2) {

      PsTinyMatrix<TYPE, NDIM, NDIM> res;
      for (size_t i=0; i<MDIM; ++i)
        for (size_t j=0; j<NDIM; ++j)
          res(i, j) = tv1[i] * tv2[j];
      return res;
    }

/**
 * Set matrix to M(i, j) = delta_{i, j}
 */
    void setToIdentity() {
      for (size_t i=0; i<MDIM; ++i) {
        for (size_t j=0; j<NDIM; ++j) {
          (*this)(i, j) = (i==j ? 1 : 0);
        }
      }
    }

/**
 * Get the norm-squared (sum of squares of elements) of the matrix
 *
 * @return the norm
 */
    TYPE normSqr() const {
      TYPE value = 0;
      for (size_t i=0; i<size(); ++i) value += (*this)[i]*(*this)[i];
      return value;
    }

/**
 * Get the total number of elements
 */
    inline size_t size() const {
      return MDIM*NDIM;
    }

/**
 * Send this to a stream
 *
 * @param o the stream to send it to
 */
    void write(std::ostream& o) const {
      const size_t WIDTH = 4;
      if (NDIM ==0 || MDIM ==0) {
        o << "empty" << std::endl;
      } else if (NDIM == 1 && MDIM > 1) {
// it's a column vector, output on 1 row
        for (size_t i=0; i<MDIM-1; ++i) {
          o << std::setw(WIDTH) << operator[](i) << " ";
        }
        o << std::setw(WIDTH) << operator[](MDIM-1)
            << "(column)" << std::endl;
      } else {
        for (size_t i=0; i<MDIM;++i) {
          for (size_t j=0; j<NDIM-1;++j) {
            o << std::setw(WIDTH) << operator()(i, j) << " ";
          }
          o << std::setw(WIDTH) << operator()(i, NDIM-1)
              << std::endl;
        }
      }
    }


  private:

/**
 * The data array.
 */
    TYPE data[MDIM][NDIM];

};

// specialize to NDIM=0, so that we can recurse
//
// the real recursion ends in switch statements before it
// gets here; however, for some reason, the compiler insists
// on compiling cases like MDIM == 3, even when MDIM = 1,
// even though those cases will not end up in the object code
// (perhaps it compiles before eliminating unreachable code?)
// this simply prevents endless cursion during compile.  It needs
// only the methods that get called during recursion.

template <class TYPE, size_t MDIM>
class PsTinyMatrix<TYPE, MDIM, 0> {
  public:

/** constructor: should never get called */
    PsTinyMatrix() {
      TxDebugExcept tde("PsTinyMatrix<TYPE, MDIM, 0> should"
          " never be instantiated");
      throw tde;
    }

/**
 * calculate determinant
 *
 * @return determinant of matrix
 */
    TYPE det() const { return 1; }

/**
 * calculate sub-determinant (determinant of matrix without
 * it's i^th row and j^th column)
 *
 * the sign is such that:
 * det M = sum_j M_ij * M.det(i, j)
 *
 * @param i row to omit
 * @param j column to omit
 * @return determinant of matrix, with row i and col j deleted
 */
    TYPE det(size_t i, size_t j) { return 1;}

/**
 * Define the () operator to provide 2D access to the elements.
 *
 * @param i the row of the desired element (0 <= i < MDIM)
 *          with no bounds checking
 * @param j the column of the desired element (0 <= j < NDIM)
 *          with no bounds checking
 * @return the desired element by reference
 */
    TYPE& operator()(size_t i, size_t j) { return dummy;}

  private:
    TYPE dummy;
};

// Needed by the Dec/Compaq/HP Alpha compiler CXX:

#endif  // PS_TINY_MATRIX_H

