/**
 *
 * @file    PsNoArgFunc.h
 *
 * @brief   Defines interface for PsNoArgFunc class.
 *
 * @version $Id: PsNoArgFunc.h 6319 2006-11-14 22:39:46Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

#ifndef PS_NO_ARG_FUNC_H
#define PS_NO_ARG_FUNC_H

// txbase includes
#include <TxHierAttribSetTmpl.h>

/**
 * @class PsNoArgFunc
 * @brief Functors over space time
 */
template <class FLOATTYPE>
class PsNoArgFunc {

  public:

/**
 * Constructor - does nothing.
 */
    PsNoArgFunc() {}

/**
 * Destructor
 */
    virtual ~PsNoArgFunc() {}

/**
 * Set up the functor from data in an attribute set
 *
 * @param tas the attribute set to use
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas) {}

/**
 * Set a seed for this functor (useful for pseudo-random number generators)
 *
 * @param s the seed
 */
    virtual void setSeed(size_t s) {}

/**
 * Return value of the functor at this point in space-time
 *
 */
    virtual FLOATTYPE operator()() const = 0;

  protected:

};

#endif // PS_NO_ARG_FUNC_H

