/**
 * @file    PsSwitchMovTanhSlab.h
 *
 * @brief   Defines interface for moving tanh slab Expression class.
 *
 * @version $Id: PsSwitchMovTanhSlab.h 2137 2020-04-16 20:37:29Z alexanda $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

#ifndef PS_SWITCH_MOV_TANH_SLAB_H
#define PS_SWITCH_MOV_TANH_SLAB_H

// txbase includes
#include <PsParser.h>

// psstd includes
#include <PsMovTanhSlabExpression.h>
#include <string>


/**
 * @class PsSwitchMovTanhSlab
 * @brief Interface of a functor for an arbitrary expression
 */
template <class FLOATTYPE, int NDIM>
class PsSwitchMovTanhSlab : public virtual PsMovTanhSlabExpression<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor - sets default values of parameters
 */
    PsSwitchMovTanhSlab();

/**
 * Destructor
 */
    virtual ~PsSwitchMovTanhSlab() {
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
    virtual FLOATTYPE operator()(PsTinyVector<int, NDIM>& rvec,
                                 FLOATTYPE t) const;


  protected:


  private:

    /* total zone passes through system */
    size_t maxSweeps;

    /* Sweeps of zone through system */
    mutable size_t sweepNumber;

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

    /* Total distance traveled by zones in all directions */
    mutable FLOATTYPE totalPosDist;

    /* Method for switching component perpendicular to zone plane */
    std::string switchType;

    /* List of x,y,z, components */
    mutable std::vector<int> pcompList;

    /* List of x,y,z, buffer distances */
    std::vector<FLOATTYPE> zoneBuffers;

    /** Flag for swtiching off zone-anneal */
    bool turnOffFlag;
};
#endif
