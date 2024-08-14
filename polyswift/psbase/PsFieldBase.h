/**
 *
 * @file    PsFieldBase.h
 *
 * @brief   Base class for PsField that provides interface to any field.
 *
 * @version $Id: PsField.h 8329 2007-09-21 16:12:04Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_FIELD_BASE_H
#define PS_FIELD_BASE_H

// std includes
#include <string>
#include <vector>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase
#include <PsGridBase.h>
#include <PsGridBaseItr.h>

/**
 * A field interface (PsFieldBase) class
 */
template <class FLOATTYPE>
class PsFieldBase {

 public:

  PsFieldBase() { };

  // Copy constructor
  //  PsFieldBase(PsFieldBase<FLOATTYPE>& psf);

  // Destructor
  virtual ~PsFieldBase() { };

/**
 * Get rank of tensor
 *
 * @return rank of tensor, ie dimen in PrTensor
 */
    virtual size_t getTensorRank() const = 0;

/**
 * Get number of components (per cell)
 *
 * @return field type str
 */
   virtual size_t getNumComponents() const = 0;

/**
 * Get field type string
 *
 * @return field type str
 */
    virtual std::string getFieldType() const = 0;

/**
 * Set field type string
 *
 * @param ft field type str
 */
    virtual void setFieldType(std::string ft) = 0;

/**
 * Returns the size of the entire tensor
 *
 * @return the size of the entire tensor
 */
    virtual size_t getSize() const = 0;

/**
 * Main build/setup method for PsGridField.
 * Dynamic cast to full PsGridBase interface
 * Use grid info to allocate field and then initialize
 *
 * @param gridItrPtr grid interface (template-less)
 */
    virtual void setGrid(PsGridBaseItr* gridItrPtr) = 0;

/**
 * Get reference to grid used to construct field
 *
 * @return grid interface (template-less)
 */
    virtual PsGridBaseItr& getGrid() const = 0;

 /** ******************************************** */
 /**       Overloaded field data operators        */
 /** ******************************************** */

/**
 * Add scalar to this instance of PsField
 *
 * @param c scalar float data
 */
    virtual void operator+=(FLOATTYPE c) = 0;

/**
 * Add data to this instance of PsField
 *
 * @param psf 1st PsField operand
 */
    virtual void operator+=(PsFieldBase<FLOATTYPE>& psf) = 0;

/**
 * Subtract scalar from this instance of PsField
 *
 * @param c scalar float data
 */
    virtual void operator-=(FLOATTYPE c) = 0;

/**
 * Subtract data from this instance of PsField
 *
 * @param psf 1st PsField operand
 */
    virtual void operator-=(PsFieldBase<FLOATTYPE>& psf) = 0;

/**
 * Multiply scalar to this instance of PsField
 *
 * @param c scalar float data
 */
    virtual void operator*=(FLOATTYPE c) = 0;

/**
 * Multiply data elementwise to this instance of PsField
 *
 * @param psf 1st PsField operand
 */
    virtual void operator*=(PsFieldBase<FLOATTYPE>& psf) = 0;

/**
 * Reset all the elements to a fixed value
 *
 * @param val value to set field
 */
    virtual void reset(double val) = 0;

/**
 * Multiply all elements by a given value
 *
 * @param val value to multiply field
 */
    virtual void scale(double val) = 0;

/**
 * Check for values above val and clips
 *
 * @param maxVal  max limit to check
 * @param clipVal clip values above this to clipVal
 * @return bool if maxVal exceeded
 */
    virtual bool checkMaxClip(double maxVal, double clipVal) = 0;

/**
 * Return maximum value in field (across all components)
 *
 * @return maximum value
 */
    virtual FLOATTYPE maxVal() = 0;

/**
 * Return minimum value in field (across all components)
 *
 * @return minimum value
 */
    virtual FLOATTYPE minVal() = 0;

/**
 * Apply exponential function elementwise
 */
    virtual void apply_exp() = 0;

/**
 * Reset all the elements to a uniform random
 * using PsRandom (can transform by a(rand#)+b )
 *
 * @param scale a
 * @param shift b
 */
    virtual void resetRandom(FLOATTYPE scale, FLOATTYPE shift) = 0;

/**
 * Add all elements together
 *
 * @return sum of all elements
 */
    virtual FLOATTYPE getSumAll() = 0;

/**
 * Add white noise [-0.5,0.5] to all elements using PsRandom
 *
 * @param scale scale factor for random numbers
 */
    virtual void addRandom(FLOATTYPE scale) = 0;

/**
 * Get a pointer to the data for this field
 *
 * @return non-const float pointer to data
 */
    virtual FLOATTYPE* getDataPtr() const = 0;

/**
 * Get a const pointer to the data for this field
 *
 * @return const float pointer to data
 */
    virtual const FLOATTYPE* getConstDataPtr() const = 0;

 protected:

 private:

};
#endif // PS_FIELD_BASE_H
