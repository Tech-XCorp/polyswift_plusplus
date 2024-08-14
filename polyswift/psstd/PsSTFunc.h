/**
 *
 * @file    PsSTFunc.h
 *
 * @brief   Defines interface for the PsSTFunc class.
 *
 * @version $Id: PsSTFunc.h 6319 2006-11-14 22:39:46Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

#ifndef PS_ST_FUNC_H
#define PS_ST_FUNC_H

// psstd includes
#include <PsTinyVector.h>

// txbase includes
#include <TxHierAttribSetTmpl.h>

template <class FLOATTYPE, size_t NDIM>
class PsSTFunc {

  public:

/**
 * Constructor - sets diagnostic level to zero, rank to zero and communication size to one.
 */
  PsSTFunc();

/**
 * Destructor
 */
  virtual ~PsSTFunc();

/**
 * Set up the functor from data in an attribute set
 */
  virtual void setAttrib(const TxHierAttribSetIntDbl&) = 0;

/**
 * Set the diagnostic level
 */
  virtual void setDiagnose(int d) {
    diagnoseLevel = d;
  }

/**
 * Set the diagnostic level
 */
  virtual void setRank(size_t r) {
    rank = r;
  }

/**
 * Set the diagnostic level
 */
  virtual void setCommSize(size_t s) {
    commSize = s;
  }

/**
 * Return value of the functor at this point in space-time
 *
 * @param x vector of position
 * @param t the time
 */
  virtual FLOATTYPE operator()(const FLOATTYPE* x, FLOATTYPE t) const = 0;

/**
 * Return value of the functor at this point in space-time
 *
 * @param rvec PsTinyVector of position
 * @param t the time
 */
  virtual FLOATTYPE operator()(PsTinyVector<int, NDIM>& rvec, FLOATTYPE t) const = 0;


/**
 * Return value of the functor at this point in space-time
 *
 * @param rvec PsTinyVector of position
 * @param t the time
 */
    virtual FLOATTYPE operator()(PsTinyVector<size_t, NDIM>& rvec, FLOATTYPE t)
      const = 0;


  protected:

  /** The level of diagnostics */
  int diagnoseLevel;

  /** Rank of processor */
  size_t rank;

  /** Size of communicator */
  size_t commSize;
};

#endif // PS_ST_FUNC_H

