/**
 *
 * @file    PsCutExpression.h
 *
 * @brief   Defines interface for PsCutExpression class.
 *
 * @version $Id: PsCutExpression.h 6319 2006-11-14 22:39:46Z sizemore $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

#ifndef PS_CUT_EXPRESSION_H
#define PS_CUT_EXPRESSION_H

// txbase includes
#include <PsParser.h>

// psstd includes
#include <PsExpression.h>


/**
 * @class PsCutExpression
 * @brief Interface of a functor for an arbitrary expression
 */
template <class FLOATTYPE, int NDIM>
class PsCutExpression : public virtual PsExpression<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor - sets default values of parameters
 */
    PsCutExpression();

/**
 * Destructor
 */
    virtual ~PsCutExpression() {}

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


  protected:

    /* Name string in input file */
    std::string lowerValInputName;

    /* Name string in input file */
    std::string upperValInputName;

 private:

    FLOATTYPE lowerVal;
    FLOATTYPE upperVal;
};

#endif // PS_CUT_EXPRESSION_H

