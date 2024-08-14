/**
 *
 * @file    PsChiCutExpression.h
 *
 * @brief   Defines interface for PsChiCutExpression class.
 *
 * @version $Id: PsChiCutExpression.h 2137 2020-04-16 20:37:29Z alexanda $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

#ifndef PS_CHI_CUT_EXPRESSION_H
#define PS_CHI_CUT_EXPRESSION_H

// txbase includes
#include <PsParser.h>

// psstd includes
#include <PsCutExpression.h>


/**
 * @class PsChiCutExpression
 * @brief Interface of a functor for an arbitrary expression
 */
template <class FLOATTYPE, int NDIM>
class PsChiCutExpression : public virtual PsCutExpression<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor - sets default values of parameters
 */
    PsChiCutExpression();

/**
 * Destructor
 */
    virtual ~PsChiCutExpression() {}

/**
 * Set up the functor from data in an attribute set
 *
 * @param tas An attribute set
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);


  protected:

  private:

};

#endif // PS_CHICUT_EXPRESSION_H

