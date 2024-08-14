/**
 * @file    PsMovTanhSlabExpression.h
 *
 * @brief   Defines interface for moving tanh slab Expression class.
 *
 * @version $Id: PsMovTanhSlabExpression.h 2137 2020-04-16 20:37:29Z alexanda $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

#ifndef PS_MOV_TANH_SLAB_EXPRESSION_H
#define PS_MOV_TANH_SLAB_EXPRESSION_H

// txbase includes
#include <PsParser.h>

// psstd includes
#include <PsExpression.h>


/**
 * @class PsMovTanhSlabExpression
 * @brief Interface of a functor for an arbitrary expression
 */
template <class FLOATTYPE, int NDIM>
class PsMovTanhSlabExpression : public virtual PsExpression<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor - sets default values of parameters
 */
    PsMovTanhSlabExpression();

/**
 * Destructor
 */
    virtual ~PsMovTanhSlabExpression() {
      delete[] xvec;
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
 */
    virtual FLOATTYPE operator()(const FLOATTYPE* x, FLOATTYPE t) const;

/**
 * Return value of the functor at this point in space-time
 *
 * @param rvec PsTinyVector of position
 * @param t the time
 * @return the value of the operator
 */
    virtual FLOATTYPE operator()(PsTinyVector<int, NDIM>& rvec, FLOATTYPE t) const;


  protected:


  private:

    /* Workspace for converting spatial arg types */
    FLOATTYPE* xvec;

   /* Width of the tanh region */
    FLOATTYPE zoneSize;

    /* Width of tanh edges */
    FLOATTYPE widthParam;

    /* Value outside of slab */
    FLOATTYPE chiNmax;

    /* Value inside of slab */
    FLOATTYPE chiNmin;

    /* Component parallel to zone plane */
    size_t pcomp;

};

#endif // PS_MOV_TANH_SLAB_EXPRESSION_H
