/**
 * @file    PsExpression.h
 *
 * @brief   Defines interface for PsExpression class.
 *
 * @version $Id: PsExpression.h 6319 2006-11-14 22:39:46Z sizemore $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

#ifndef PS_EXPRESSION_H
#define PS_EXPRESSION_H

// txbase includes
#include <PsParser.h>

// vpfunc includes
#include <PsSTFunc.h>

/**
 * @class PsExpression
 * @brief Interface of a functor for an arbitrary expression
 */
template <class FLOATTYPE, int NDIM>
class PsExpression : public virtual PsSTFunc<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor - sets default values of parameters
 */
    PsExpression();

/**
 * Destructor
 */
    virtual ~PsExpression() {
      delete parser;
    }

/**
 * Set up the functor from data in an attribute set
 *
 * @param tas An attribute set containing the parameters of the
 * plane wave.
 *
 * The attribute relevant to this class within tas is:
 *
 * - @c expression (real vector): the expression to be evaluated
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Return value of the functor at this point in space-time
 *
 * @param x vector of position
 * @param t the time
 * @return the value of the operator
 *
 */
    virtual FLOATTYPE operator()(const FLOATTYPE* x, FLOATTYPE t) const;


/**
 * Return value of the functor at this point in space-time
 *
 * @param rvec PsTinyVector of position
 * @param t the time
 * @return the value of the operator
 */
    virtual FLOATTYPE operator()(PsTinyVector<size_t, NDIM>& rvec, FLOATTYPE t) const {
      // SWS: not implemented at this time
      return 0.0;
    }

/**
 * Return value of the functor at this point in space-time
 *
 * @param rvec PsTinyVector of position
 * @param t the time
 * @return the value of the operator
 */
    virtual FLOATTYPE operator()(PsTinyVector<int, NDIM>& rvec, FLOATTYPE t) const {
      // SWS: not implemented at this time
      return 0.0;
    }


  protected:

    std::string exprNameStr;

    std::string expression;
    PsParser<FLOATTYPE>* parser;
    FLOATTYPE* xPtr[NDIM];
    FLOATTYPE* tPtr;
    FLOATTYPE dummyVar; // stores the arguments not used in expression

};

#endif // PS_EXPRESSION_H
