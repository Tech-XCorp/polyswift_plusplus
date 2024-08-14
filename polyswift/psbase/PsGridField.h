/**
 *
 * @file    PsGridField.h
 *
 * @brief   Base class for fields.
 *
 * @version $Id: PsGridField.h 8401 2007-09-26 20:17:05Z cary $
 *
 * Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_GRID_FIELD_H
#define PS_GRID_FIELD_H

// std includes
#include <string>
#include <vector>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// txbase includes
#include <TxNotImplExcept.h>
#include <PrTensor.h>

// psbase includes
//#include <PsDynObj.h>
#include <PsTensor.h>
#include <PsFieldBase.h>

/**
 * A field is a collection of values for each cell.  These values
 * can be accessed by index or index vector
 *
 * The PsGridField overrides the PrTensor class
 * The number of values at each cell is returned by getNumElements.  The
 * values returned by getElements are the values in the cell of that
 * index.
 *
 */
template <class FLOATTYPE, size_t NRANK>
class PsGridField : public PsTensor<FLOATTYPE, NRANK+1>,
                    public PsFieldBase<FLOATTYPE> {

  public:

/**
 * Default constructor
 */
    PsGridField();

/**
 * Copy constructor
 * In TxTensor/PrTensor
 */
    PsGridField(PsFieldBase<FLOATTYPE>& psf);

/**
 * Destructor
 */
    virtual ~PsGridField();

/**
 * Main build/setup method for PsGridField.
 * Dynamic cast to full PsGridBase interface
 * Use grid info to allocate field and then initialize
 *
 * @param gItr grid interface (template-less)
 */
    virtual void setGrid(PsGridBaseItr* gItr);

/**
 * Get reference to grid used to construct field
 *
 * @return grid interface (template-less)
 */
    virtual PsGridBaseItr& getGrid() const;

/**
 * Cast up to base class
 *
 * @return pointer to field interface class
 */
    PsFieldBase<FLOATTYPE>* getBasePtr() {
      PsFieldBase<FLOATTYPE>* psfb =
          dynamic_cast< PsFieldBase<FLOATTYPE>* >(this);
      return psfb;
    }

/**
 * Get a pointer to the data for this field
 *
 * @return non-const float pointer to data
 */
    FLOATTYPE* getDataPtr() const {
      return this->data;
    }

/**
 * Get a const pointer to the data for this field
 *
 * @return const float pointer to data
 */
    const FLOATTYPE* getConstDataPtr() const {
      return this->data;
    }

 // SWS: this only assigns to scalar field and will
 // have to be generalized

/**
 * Take a position in global grid and decide if this position
 * corresponds to an owned position on the PsField object. If
 * owned then transform global postiion to local position and
 * assign val to the field.
 *
 * @param posvec   position vector
 * @param val      scalar value of field
 * @param sendFlag "add" or "set" string flag
 */
    virtual void mapToLocalField(PsTinyVector<int, NRANK>& posvec,
        FLOATTYPE val, std::string sendFlag);

/**
 * k vectors associated with grid
 */
    virtual void calck2();

/**
 * k vectors associated with grid field
 */
    virtual void calckVec();

/**
 * Get number of components (per cell)
 *
 * @return unsigned int number of components
 */
    virtual size_t getNumComponents() const {
      return numComp;
    }

/**
 * Get rank of tensor
 *
 * @return rank of tensor, ie dimen in PrTensor
 */
  virtual size_t getTensorRank() const {
    return this->tensorRank;
  }

/**
 * Get field type string
 *
 * @return field type str
 */
  virtual std::string getFieldType() const {
    return fieldType;
  }

/**
 * Set field type string
 *
 * @param ft field type str
 */
  virtual void setFieldType(std::string ft) {
    fieldType = ft;
  }

/**
 * Gives the number of field elements in a cell.
 *
 * @return the number of field elements per cell
 */
    size_t getNumElements() const {
      return this->getLength(NRANK);
    }

/**
 * get the length of a given dimension
 *
 * @param i the given dimension
 * @return integer size of i-th dimension
 */
    size_t getLength(size_t i) const {
      return PrTensor<FLOATTYPE, NRANK+1>::getLength(i);
    }

/**
 * Get a vector of lengths
 *
 * @return std::vector of sizes for each dimension
 */
    std::vector<size_t> getLengths();

/**
 * Returns the size of the entire tensor
 *
 * @return the size of the entire tensor
 */
    size_t getSize() const {
      return PrTensor<FLOATTYPE, NRANK+1>::getSize();
    }

/**
 * Returns the size for an increment of that dimension
 *
 * @return the size for an increment of that dimension
 */
    size_t getSize(size_t i) const {
      return PrTensor<FLOATTYPE, NRANK+1>::getSize(i);
    }

/**
 * Set the number of field elements in a cell
 *
 * @param numElem the number of field elements per cell
 */
    void setNumElements(size_t numElem) {
      this->setLength(NRANK, numElem);
    }

/**
 * Set the size of PsGridField, last element is number
 * of components/spatial cell and defaults to 1;
 *
 * @param lens size of field for NRANK dimensions
 */
    void setLengths(size_t lens[NRANK]);

/**
 * Set the size of PsGridField, last element is number
 * of components/spatial cell and defaults to 1;
 *
 * @param lensVec size of field for NRANK dimensions
 */
    void setLengths(std::vector<size_t> lensVec);

/**
 * Set the size of PsGridField, last element is number
 * of components/spatial cell and defaults to 1,
 * Easy way to set length of scalar
 *
 * @param lensScalar size of field for NRANK dimensions
 */
    void setLengths(size_t lensScalar);

/**
 * Get an element as an lval (left value)
 * Unspecified indices are interpreted as zero
 *
 * @param indices the index vector
 * @return the value of the element as an lval
 */
    FLOATTYPE& operator()(size_t* indices) {
      return PrTensor<FLOATTYPE, NRANK+1>::operator()(indices);
    }

/**
 * Get an element as an lval (left value)
 * Unspecified indices are interpreted as unity
 *
 * @param i the range of the last index
 * @return the value of the element as an lval
 */
    FLOATTYPE& operator()(size_t i) {
      return PrTensor<FLOATTYPE, NRANK+1>::operator()(i);
    }

/**
 * Get an element as an lval (left value)
 * Unspecified indices are interpreted as unity
 *
 * @param i the range of the second to last index
 * @param j the range of the last index
 * @return the value of the element as an lval
 */
    FLOATTYPE& operator()(size_t i,  size_t j) {
      return PrTensor<FLOATTYPE, NRANK+1>::operator()(i, j);
    }

/**
 * Get an element as an lval (left value)
 *
 * @param i the range of the third to last index
 * @param j the range of the second to last index
 * @param k the range of the last index
 *
 * Unspecified indices are interpreted as unity
 *
 * @return the value of the element as an lval
 */
    FLOATTYPE& operator()(size_t i, size_t j, size_t k) {
      return PrTensor<FLOATTYPE, NRANK+1>::operator()(i, j, k);
    }

/**
 * Get an element as an lval (left value)
 *
 * @param i the range of the fourth to last index
 * @param j the range of the third to last index
 * @param k the range of the second to last index
 * @param l the range of the last index
 *
 * Unspecified indices are interpreted as unity
 *
 * @return the value of the element as an lval
 */
    FLOATTYPE& operator()(size_t i, size_t j, size_t k, size_t l) {
      return PrTensor<FLOATTYPE, NRANK+1>::operator()(i, j, k, l);
    }

/**
 * Get an element as an lval (left value)
 *
 * @param i the range of the fifth to last index
 * @param j the range of the fourth to last index
 * @param k the range of the third to last index
 * @param l the range of the second to last index
 * @param m the range of the last index
 *
 * Unspecified indices are interpreted as unity
 *
 * @return the value of the element as an lval
 */
    FLOATTYPE& operator()(size_t i, size_t j, size_t k, size_t l, size_t m) {
      return PrTensor<FLOATTYPE, NRANK+1>::operator()(i, j, k, l, m);
    }

/**
 * Get an element as an lval (left value)
 *
 * @param i the range of the sixth to last index
 * @param j the range of the fifth to last index
 * @param k the range of the fourth to last index
 * @param l the range of the third to last index
 * @param m the range of the second to last index
 * @param n the range of the last index
 *
 * Unspecified indices are interpreted as unity
 *
 * @return the value of the element as an lval
 */
    FLOATTYPE& operator()(size_t i, size_t j, size_t k,
        size_t l, size_t m, size_t n) {
      return PrTensor<FLOATTYPE, NRANK+1>::operator()(i, j, k, l, m, n);
    }

/**
 * Get an element as an lval (left value)
 *
 * @param i the range of the seventh to last index
 * @param j the range of the sixth to last index
 * @param k the range of the fifth to last index
 * @param l the range of the fourth to last index
 * @param m the range of the third to last index
 * @param n the range of the second to last index
 * @param o the range of the last index
 *
 * Unspecified indices are interpreted as unity
 *
 * @return the value of the element as an lval
 */
    FLOATTYPE& operator()(size_t i, size_t j, size_t k, size_t l,
        size_t m, size_t n, size_t o) {
      return PrTensor<FLOATTYPE, NRANK+1>::operator()(i, j, k, l, m, n, o);
    }

/**
 * Get an element as an rval (right value)
 *
 * @param indices the index vector
 *
 * Unspecified indices are interpreted as zero
 *
 * @return the value of the element as an rval
 */
    FLOATTYPE operator()(size_t* indices) const {
      return PrTensor<FLOATTYPE, NRANK+1>::operator()(indices);
    }

/**
 * Get an element as an rval (right value)
 *
 * @param i the range of the last index
 *
 * Unspecified indices are interpreted as unity
 *
 * @return the value of the element as an rval
 */
    FLOATTYPE operator()(size_t i) const {
      std::vector<size_t> sizeVec;
      return PrTensor<FLOATTYPE, NRANK+1>::operator()(i);
    }

// ******************************************
// Updating field elements
// ******************************************

/**
 * Test method
 *
 * @param psfb dummy field pointer
 */
//    virtual void testAssign(PsFieldBase<FLOATTYPE>* psfb);

/**
 * Multiply data elementwise to this instance of PsGridField
 *
 * @param psf 1st PsGridField operand
 */
    void operator*=(PsFieldBase<FLOATTYPE>& psf);

/**
 * Add data to this instance of PsField
 *
 * @param psf 1st PsField operand
 */
    void operator+=(PsFieldBase<FLOATTYPE>& psf);

/**
 * Subtract data from this instance of PsGridField
 *
 * @param psf 1st PsGridField operand
 */
    void operator-=(PsFieldBase<FLOATTYPE>& psf);

/**
 * Add scalar to this instance of PsField
 *
 * @param c scalar float data
 */
    void operator+=(FLOATTYPE c);

/**
 * Subtract scalar from this instance of PsField
 *
 * @param c scalar float data
 */
    void operator-=(FLOATTYPE c);

/**
 * Multiply scalar to this instance of PsField
 *
 * @param c scalar float data
 */
    void operator*=(FLOATTYPE c);

/**
 * Reset all the elements to a fixed value
 *
 * @param val value to set field
 */
    virtual void reset(double val);

/**
 * Multiply all elements by a given value
 *
 * @param val value to multiply field
 */
    virtual void scale(double val);

/**
 * Check for values above val and clips
 *
 * @param maxVal  max limit to check
 * @param clipVal clip values above this to clipVal
 * @return bool if maxVal exceeded
 */
    virtual bool checkMaxClip(double maxVal, double clipVal);

/**
 * Add all elements together
 *
 * @return sum of all elements
 */
    virtual FLOATTYPE getSumAll();

/**
 * Return maximum value in field (across all components)
 *
 * @return maximum value
 */
    virtual FLOATTYPE maxVal();

/**
 * Return minimum value in field (across all components)
 *
 * @return minimum value
 */
    virtual FLOATTYPE minVal();

/**
 * Apply exponential function elementwise
 */
    virtual void apply_exp();

/**
 * Reset all the elements to a uniform random
 * using PsRandom (can transform by a(rand#)+b )
 *
 * @param scale a
 * @param shift b
 */
    virtual void resetRandom(FLOATTYPE scale, FLOATTYPE shift);

/**
 * Add white noise [-0.5,0.5] to all elements using PsRandom
 *
 * @param scale scale factor for random numbers
 */
    virtual void addRandom(FLOATTYPE scale);

/**
 * Check for inequality
 *
 * @param  x value to check
 * @return result of inequality check
 */
    inline bool operator!=(FLOATTYPE x) {
      for (size_t i=0; i<getSize(); ++i) {
        if (operator()(i) != x) return true;
      }
      return false;
    }

  protected:

    /** Map name field type */
    std::string fieldType;

  private:

    /** Grid interface */
    PsGridBase<FLOATTYPE, NRANK>* gridPtr;

    /** Number of components */
    size_t numComp;

};

#endif // PS_FIELD_H

/**
 * Multiply data elements in two PsFieldBase elementwise
 *
 * @param psf 1st PsFieldBase operand
 * @return ref to PsFieldBase with the product result
 */
//    virtual PsFieldBase<FLOATTYPE>& operator*(PsFieldBase<FLOATTYPE>& psf);

/**
 * Multiply data elements in two PsFields elementwise
 *
 * @param psf 1st PsField operand
 * @return a PsField with the product result
 */
//    PsGridField<FLOATTYPE, NRANK> operator*(PsGridField<FLOATTYPE, NRANK>& psf);
