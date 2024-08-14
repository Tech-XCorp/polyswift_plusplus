/**
 *
 * @file    PsTinyVector.h
 *
 * @brief   Define interface for the class PsTinyVector.
 *
 * @version $Id: PsTinyVector.h 10863 2008-11-12 17:40:46Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

#ifndef PS_TINY_VECTOR_H
#define PS_TINY_VECTOR_H

// std includes
#include <vector>
#include <cmath>
#include <iostream>
#include <iomanip>
// #include <math> cmath ok?
#include <limits>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// txbase includes
#include <TxDebugExcept.h>

// psstd includes
#include <PsCompileTimeAssert.h>

/**
 * @class PsTinyVector
 *
 * @brief Class for representing small groups of data, like
 * a particle position or velocity, that can then be manipulated as a
 * standard 1-D mathematical vector.
 *
 * It is convenient to hold all the components of a particle's position
 * or velocity within a single object, which can then be manipulated
 * as a standard 1-D mathematical vector.  For example, this allows
 * dot-products or cross-products to be used when calculating the
 * forces associated with electromagnetic fields.
 *
 * PsTinyVector is a templated class, templated over both the numeric
 * type of the elements and the number of elements in the vector.
 *
 * The intention is that groups of particles or other objects will
 * be represented by, for example, an std::vector<PsTinyVector<float, 3> >,
 * which will allocate a contiguous block of memory for all the data.
 *
 * The methods provide an interface like that of a 1-D mathematical vector.
 *
 * There's no constructor or destructor, because the pointer data is
 * dimensioned through a template parameter, so the compiler takes
 * care of memory allocation and garbage collection.
 *
 *  @param TYPE numeric type of the data.
 *  @param NDIM length of the vector.
 */
template <class TYPE, size_t NDIM>
class PsTinyVector {

  public:

// Need a type to distinguish no-initialization constructor
// (until we can get rid of initialization in default constructor)
    enum PsVectorInitialization { NO_INIT };
//
// Constructors, destructors, assignment
//

/**
 * Default constructor
 */
    PsTinyVector() {}

/**
 * Constructor with no initialization
 */
    explicit PsTinyVector(PsVectorInitialization initType) { }

/**
 * Copy constructor -- templated for any source vector at least as long
 * as this vector; if it's too long, copy only as many elements as needed.
 *
 * 2008 Nov 7, GRW: implicit conversion can lead to bugs (e.g., if someone
 *   thinks that a function takes a 4-vector argument, but it actually
 *   takes a 3-vector argument), although it is sometimes very handy.
 *   Since one purpose of TinyVector is to enforce the lengths of vectors
 *   in function arguments, on the whole I think it better to make this
 *   constructor explicit.
 *
 * @param tv the vector to copy from
 */
    template <size_t MDIM>
    explicit PsTinyVector(const PsTinyVector<TYPE, MDIM>& tv) {
      PsCompileTimeAssert< MDIM >= NDIM >();
      for (size_t i=0; i<NDIM; ++i) data[i] = tv[i];
    }
/**
 * Construct from shorter vector, padding with desired value
 *
 * 2008 Nov 7, GRW: implicit conversion can lead to bugs (e.g., if someone
 *   thinks that a function takes a 4-vector argument, but it actually
 *   takes a 3-vector argument), although it is sometimes very handy.
 *   Since one purpose of TinyVector is to enforce the lengths of vectors
 *   in function arguments, on the whole I think it better to make this
 *   constructor explicit.
 *
 * @param tv the vector to copy from
 * @param val a value with which to set components of this vector
 *        that are not in tv
 */
    template <size_t MDIM>
    explicit PsTinyVector(const PsTinyVector<TYPE, MDIM>& tv, TYPE val) {
      PsCompileTimeAssert< MDIM <= NDIM >();
      for (size_t i=0; i<MDIM; ++i) data[i] = tv[i];
      for (size_t i=MDIM; i<NDIM; ++i) data[i] = val;
    }

/**
 * Create from an std::vector.  Allow implicit conversion (because
 * we check for size).
 * ! Will throw an exception if std::vector is not long enough !
 * @param v a vector with length at least NDIM
 * @throws TxDebugExcept if std::vector isn't long enough
 */
    PsTinyVector(std::vector<TYPE> const & v) {
      if (v.size() < NDIM) {
        TxDebugExcept tde("PsTinyVector::PsTinyVector(std::vector) error: "
          "tried to create a PsTinyVector with ");
        tde << NDIM << " elements from a std::vector with " <<
          v.size() << " elements";
        throw tde;
      }
      for (size_t i=0; i<NDIM; ++i) data[i] = v[i];
    }

/**
 * Create with initialization to uniform value
 * allow implicit conversion
 *
 * @param val the value to which all components should be set
 */
    PsTinyVector(TYPE val) {
      for (size_t i=0; i<NDIM; ++i) data[i] = val;
    }


/**
 * Create with initialization, set all values of this vector
 * to x,y,z (need NDIM=3)
 *
 * @param x TYPE x value
 * @param y TYPE y value
 * @param z TYPE z value
 */
    PsTinyVector(TYPE x, TYPE y, TYPE z) {
      if ( NDIM < 3 ) {
        TxDebugExcept tde("PsTinyVector::(x,y,z) constructor must be used NDIM=3");
        std::cout << tde.getMessage() << std::endl;
        throw tde;
      }
      data[0] = x; data[1] = y; data[2] = z;
    }

/**
 * Create with initialization, set all values of this vector
 * to x,y,z (need NDIM=3)
 * do explicity conversion from int
 *
 * @param val the value to which all components should be set

    PsTinyVector(size_t x, size_t y, size_t z) {
      if ( NDIM < 3 ) {
        TxDebugExcept tde("Error PsTinyVector::(x,y,z)");
        tde << "constructor must be used NDIM=3";
        std::cout << tde.getMessage() << std::endl;
        throw tde;
      }
      data[0] = (TYPE)x; data[1] = (TYPE)y; data[2] = (TYPE)z;
    }
 */

/**
 * set all values of this vector to x
 * @param x value to which all components of this vector will be set
 * @return this vector
 */
    PsTinyVector& operator=(TYPE x) {
      for (size_t i=0; i<NDIM; ++i) data[i] = x;
      return *this;
    }

/**
 * Assignment from another (possibly longer) vector.
 * If other vector is longer, copy only as many elements as needed.
 *
 * GRW: should assignment from other vector sizes be disabled, for the
 * same reason that construction from other sizes is explicit?
 *
 * @param tv the vector that will overwrite the local data
 *
 * @return local vector after all data has been overwritten
 */
    template <size_t MDIM>
    PsTinyVector& operator=(const PsTinyVector<TYPE, MDIM>& tv) {
      PsCompileTimeAssert< MDIM >= NDIM >();
      for (size_t i=0; i<NDIM; ++i) data[i] = tv[i];
      return *this;
    }

/**
 * Assignment from a shorter vector --
 * extra elements are padded with desired value.
 *
 * @param tv the vector that will overwrite the local data
 * @param val a value with which to set components of this vector
 *        that are not in tv
 *
 * @return local vector after all data has been overwritten
 */
    template <size_t MDIM>
    PsTinyVector& assign(PsTinyVector<TYPE, MDIM> const & tv, TYPE val) {
      PsCompileTimeAssert< MDIM <= NDIM >();
      for (size_t i=0; i<MDIM; ++i) data[i] = tv[i];
      for (size_t i=MDIM; i<NDIM; ++i) data[i] = val;
      return *this;
    }

/**
 * Assignment from a shorter vector -- leaving extra elements unassigned.
 *
 * @param tv the vector that will overwrite the local data
 * @return local vector after all data has been overwritten
 */
    template <size_t MDIM>
    PsTinyVector& partialAssign(const PsTinyVector<TYPE, MDIM>& tv) {
      PsCompileTimeAssert< MDIM <= NDIM >();
      for (size_t i=0; i<MDIM; ++i) data[i] = tv[i];
      return *this;
    }


/**
 * "conversion constructor" from an array: constructs a TinyVector
 * in place of a plain array, allowing one to use TinyVector capabilities
 * with a plain array.
 *
 * @param ra an NDIM-dimensional array
 * @return a reference to a TinyVector whose data member equals ra
 */
    static PsTinyVector const & inPlaceOfArray(const TYPE ra[NDIM]) {
// casting away const-ness is okay because the default constructor could
// be const, if const constructors were allowed
      return *(new(const_cast<TYPE *>(ra)) PsTinyVector(NO_INIT));
    }

/**
 * "conversion constructor" from an array: constructs a TinyVector
 * in place of a plain array, allowing one to use TinyVector capabilities
 * with a plain array.
 *
 * @param ra an NDIM-dimensional array
 * @return a reference to a TinyVector whose data member equals ra
 */
    static PsTinyVector & inPlaceOfArray(TYPE ra[NDIM]) {
      return *(new(ra) PsTinyVector(NO_INIT));
    }

/**
 * convert to std::vector
 * @return std::vector of length NDIM, with same elements as this vector
 */
    std::vector<TYPE> makeStdVec() const {
      return std::vector<TYPE>(this->getArrayPtr(),
        this->getArrayPtr() + NDIM);
    }


//
// Accessors
//

/**
 * get pointer to const raw data (use of this is discouraged)
 * @return pointer to raw data
 */
    const TYPE * getArrayPtr() const {
      return data;
    }

/**
 * get pointer to raw data (use of this is discouraged)
 * @return pointer to raw data
 */
    TYPE * getArrayPtr() {
      return data;
    }

/**
 * Define the [] operator to provide access to the elements.
 *
 * @param i the index of the desired element (0 <= i < NDIM)
 *          with no bounds checking
 * @return the desired element by reference
 */
    TYPE& operator[](size_t i) {
      return data[i];
    }

/**
 * Define the [] operator to provide access to the elements.
 *
 * @param i the index of the desired element (0 <= i < NDIM)
 *          with no bounds checking
 * @return the desired element by reference
 */
    TYPE operator[](size_t i) const {
      return data[i];
    }

/**
 * at(...) provides range-checked access to the elements.
 *
 * @param i the index of the desired element
 * @return the desired element by reference
 * @throws TxDebugExcept unless 0 <= i < NDIM
 */
    TYPE& at(size_t i) {
      if ( i >= NDIM ) {
        TxDebugExcept tde("PsTinyVector::at() (non-const) out-of-range"
            " error: index ");
        tde << i << " >= " << NDIM;
        throw tde;
    }
      return data[i];
    }

/**
 * at(...) provides range-checked access to the elements.
 *
 * @param i the index of the desired element
 * @return the desired element by reference
 * @throws TxDebugExcept unless 0 <= i < NDIM
 */
    TYPE at(size_t i) const {
      if ( i >= NDIM ) {
        TxDebugExcept tde("PsTinyVector::at() (const) out-of-range"
            " error: index ");
        tde << i << " >= " << NDIM;
        throw tde;
    }
      return data[i];
    }

/**
 * comparison operator
 */
    bool operator<(const PsTinyVector& tv) const {
      for (size_t i=0;i<NDIM;++i) {
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
    bool operator>(const PsTinyVector& tv) const {
      for (size_t i=0;i<NDIM;++i) {
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
    bool operator>=(const PsTinyVector& tv) const {
      for (size_t i=0;i<NDIM;++i) {
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
    bool operator==(const PsTinyVector& tv) const {
      for (size_t i=0;i<NDIM;++i) {
        if (operator[](i)!=tv[i]) return false;
      }
      return true;
    }

/**
 * non equivalence operator
 */
    bool operator!=(const PsTinyVector& tv) const {
      return !(operator==(tv));
    }

/**
 * Define the negation operator.
 *
 * @return the element-by-element negative of the vector
 */
    PsTinyVector operator-() const {
      PsTinyVector res;
      for (size_t i=0; i<NDIM; ++i) res.data[i] = -data[i];
      return res;
    }

/**
 * Define the + operator for adding to PsTinyVector objects.
 *
 * @param tv the vector which is being added
 *
 * @return the element-by-element sum of the two vectors
 */
    PsTinyVector operator+(const PsTinyVector& tv) const {
      PsTinyVector res;
      for (size_t i=0; i<NDIM; ++i) res.data[i] = data[i] + tv.data[i];
      return res;
    }

/**
 * Define the - operator for substracting two PsTinyVector objects.
 *
 * @param tv the vector which is being added
 *
 * @return the element-by-element sum of the two vectors
 */
    PsTinyVector operator-(const PsTinyVector& tv) const {
      PsTinyVector res;
      for (size_t i=0; i<NDIM; ++i) res.data[i] = data[i] - tv.data[i];
      return res;
    }

/**
 * Define the += operator for setting the local PsTinyVector object
 *   equal to the sum of itself plus another.
 *
 * Should be no harm in adding a vector of different length to this vector?
 *
 * @param tv the vector that will be added
 *
 * @return local vector after all data has been modified by addition
 */
    template <size_t MDIM>
    PsTinyVector& operator+=(const PsTinyVector<TYPE, MDIM>& tv) {
      if (MDIM < NDIM) {
        for (size_t i=0; i<MDIM; ++i) data[i] += tv[i];
      }
      else {
        for (size_t i=0; i<NDIM; ++i) data[i] += tv[i];
      }
      return *this;
    }

/**
 * Define the * operator for multiplication by a scalar.
 *
 * @param fac the scalar that will be multiplied
 *
 * @return local vector after all data has been multiplied
 */
    PsTinyVector& operator*=(TYPE fac) {
      for (size_t i=0; i<NDIM; ++i) data[i] *= fac;
      return *this;
    }

/**
 * Define the * operator for multiplication by a scalar.
 *
 * @param fac the scalar that will be multiplied
 *
 * @return local vector after all data has been multiplied
 */
    PsTinyVector operator*(TYPE fac) const {
      PsTinyVector res;
      for (size_t i=0; i<NDIM; ++i) res.data[i] = fac*data[i];
      return res;
    }

/**
 * Define the / operator for division by a scalar.
 *
 * @param fac the denominator of the division
 *
 * @return local vector after all data has been divided
 */
    PsTinyVector operator/(TYPE fac) const {
      PsTinyVector res;
      for (size_t i=0; i<NDIM; ++i) res.data[i] = data[i]/fac;
      return res;
    }

/**
 * Define the * operator for the dot product of two PsTinyVector objects.
 *
 * @param tv the vector which is being dotted
 *
 * @return the dot product of this vector with tv
 */
    TYPE operator*(const PsTinyVector& tv) const {
      TYPE res=0;
      for (size_t i=0; i<NDIM; ++i) res += data[i] * tv.data[i];
      return res;
    }

/**
 * Get the length squared (norm squared) of the vector
 *
 * @return the length squared
 */
    TYPE lengthSqr() const {
      TYPE value = 0;
      for (size_t i=0; i<NDIM; ++i) value += data[i]*data[i];
      return value;
    }

/**
 * Get the length (norm) of the vector
 *
 * @return the length
 */
    TYPE length() const {
      PsCompileTimeAssert<
          (std::numeric_limits<TYPE>::is_exact == false) >();
      return sqrt(lengthSqr());
    }

#define WID 4
/**
 * Send this to a stream
 *
 * @param o the stream to send it to
 */
    void write(std::ostream& o) const {
      for (size_t i=0; i<NDIM-1;++i) o << std::setw(WID) <<
        operator[](i) << " ";
      o << std::setw(WID) << operator[](NDIM-1);
    }

  private:

/**
 * The data array.
 */
    TYPE data[NDIM];

};

//
// Output operator
//
template <class TYPE, size_t NDIM>
std::ostream& operator<<(std::ostream& ostr,
        const PsTinyVector<TYPE, NDIM>& tv) {
  tv.write(ostr);
  return ostr;
}


/**
 * Define rounding for all elements in a TinyVector
 *
 * @param rvec TYPE input vector
 * @return vector with round applied to all components
 */
 template <class TYPE, size_t NDIM>
  PsTinyVector<int, NDIM> round(const PsTinyVector<TYPE, NDIM>& rvec) {

   PsTinyVector<int, NDIM> ivec;
   for (size_t ic=0; ic<NDIM; ++ic) {
     // ivec[ic] = (int)(round(rvec[ic]) );
     ivec[ic] = (int) (floor( (rvec[ic] + 0.5)));
   }
   return ivec;
 }


/**
 * Define the cross-product for 2 3D TinyVectors.
 * Don't use an operator because cross products aren't associative
 *
 * Note: in NDIM, one takes the cross-product of NDIM-1 vectors
 *
 * @param v the first 3D vector in the 3D cross-product
 * @param w the second 3D vector in the 3D cross-product
 * @return v cross w
 */
 template <class TYPE>
 PsTinyVector<TYPE, 3> cross(const PsTinyVector<TYPE, 3>& v,
     const PsTinyVector<TYPE, 3>& w) {
   PsTinyVector<TYPE, 3> res;
   res[0] = v[1] * w[2] - v[2] * w[1];
   res[1] = v[2] * w[0] - v[0] * w[2];
   res[2] = v[0] * w[1] - v[1] * w[0];
   return res;
 }


/**
 * Define the dot-product for 2 3D TinyVectors.
 * This duplicates the capability of the overloaded *-operator
 * defined above, but is more readable, if the use of the
 * overloaded operator is unfamiliar.
 *
 * @param v the first 3D vector in the 3D dot-product
 * @param w the second 3D vector in the 3D dot-product
 * @return v dot w
 */
 template <class TYPE>
 TYPE dot(const PsTinyVector<TYPE, 3>& v,
     const PsTinyVector<TYPE, 3>& w) {
  TYPE res = v[0]*w[0] + v[1]*w[1] + v[2]*w[2];
  return res;
 }

/**
 * Define pre-multiplication of TinyVector with a scalar.
 *
 * Note: A member function already defines
 *       post-multiplication of a TinyVEctor by a scalar.
 *
 * @param fac the scalar
 * @param v the vector to be multiplied
 * @return the product of fac and v
 */
 template <class TYPE, size_t NDIM>
 PsTinyVector<TYPE, NDIM> operator*(const TYPE fac,
     const PsTinyVector<TYPE, NDIM>& v) {
  PsTinyVector<TYPE, NDIM> res = v*fac;
  return res;
 }
#endif  // PS_TINY_VECTOR_H

