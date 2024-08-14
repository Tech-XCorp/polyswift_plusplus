/**
 *
 * @file    PsConstraintUpdater.h
 *
 * @brief   Class for holding constraint field in simulation model
 *
 * @version $Id: PsConstraintUpdater.h 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_CONSTRAINT_UPDATER_H
#define PS_CONSTRAINT_UPDATER_H

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#undef HAVE_CONFIG_H
#endif

// psbase includes
#include <PsUpdater.h>

/**
 * A PsConstraintUpdater contains the constraint field pointer
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 */

template <class FLOATTYPE, size_t NDIM>
class PsConstraintUpdater : public virtual PsUpdater<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 */
    PsConstraintUpdater();

/**
 * Destructor must destroy blocks
 */
    virtual ~PsConstraintUpdater();

/**
 * Set the parameters
 *
 * @param tas the parameters
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Build the data
 */
    virtual void buildData();

/**
 * Build the solvers structures
 */
    virtual void buildSolvers();

/**
 * Initialize
 */
    virtual void initialize();

/**
 * Update method
 *
 * @param t the time
 */
    virtual void update(double t);

  protected:

    /** Field workspace */
    //    PsFieldBase<FLOATTYPE>* dFld;

    /** Pointer to physical field for constraint */
    PsPhysField<FLOATTYPE, NDIM>* constraintFieldPtr;

  private:

    /** Constructor private to prevent use */
    PsConstraintUpdater(const PsConstraintUpdater<FLOATTYPE, NDIM>& psb);

   /** Assignment private to prevent use */
    PsConstraintUpdater<FLOATTYPE, NDIM>& operator=(
       const PsConstraintUpdater<FLOATTYPE, NDIM>& psb);
};

#endif  // PS_CONSTRAINT_UPDATER_H
