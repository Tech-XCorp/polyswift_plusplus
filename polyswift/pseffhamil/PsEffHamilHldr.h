/**
 *
 * @file    PsEffHamilHldr.h
 *
 * @brief   Base class owning instances of PsEffHamil
 *
 * @version $Id: PsEffHamilHldr.h 6532 2007-02-01 18:37:00Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_EFF_HAMIL_HLDR_H
#define PS_EFF_HAMIL_HLDR_H

// std includes
#include <vector>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsDynObj.h>
#include <PsEffHamilBase.h>

/**
 * PsEffHamilHldr contains a vector of pointers to PsEffHamil objects
 *
 * @param FLOATTYPE the data type
 * @param NDIM the dimensionality
 */
template <class FLOATTYPE, size_t NDIM>
class PsEffHamilHldr : public PsDynObj<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 */
    PsEffHamilHldr() {
      this->setName("EffHamilHdlr");
    }

/**
 * Destructor
 */
    virtual ~PsEffHamilHldr() {

      // Clear effective Hamiltonian holder
      for (size_t i=0; i<effhamils.size(); ++i) {
        delete effhamils[i];
      }
      effhamils.clear();

      // Clear name and attribs
      effhamilNames.clear();
      effhamilAttribs.clear();

    }

/**
 * Set from an attribute set.
 *
 * Stores attribute set and sets name.  By storing the settings
 * in the base class, any derived class has the information it
 * needs to rebuild itself.
 *
 * @param tas the attribute set
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Creates object
 */
    virtual void buildData(PsNamedObject* inOwner);

/**
 * Creates data for diagnostics
 */
    virtual void buildSolvers();

/**
 * Updates all PsEffHamil-s
 *
 * @param t simulation time
 */
    virtual void update(double t);

/**
 * Dump all PsEffHamil-s
 */
    virtual void dump();

  protected:

    /** Number of effhamils... ie length of the effhamils vector */
    size_t numEffHamils;

    /** Vector of pointers to the base PsEffHamil objects */
    std::vector< PsEffHamilBase<FLOATTYPE, NDIM>* > effhamils;

  private:

    /** List of names for the effhamils */
    std::vector< std::string > effhamilNames;

    /** List of attrib sets for the effhamils */
    std::vector< TxHierAttribSetIntDbl > effhamilAttribs;

    /** Make private to prevent use */
    PsEffHamilHldr(const PsEffHamilHldr<FLOATTYPE, NDIM>& vphh);

    /** Make private to prevent use */
    PsEffHamilHldr<FLOATTYPE, NDIM>& operator=(
        const PsEffHamilHldr<FLOATTYPE, NDIM>& vphh);
};

#endif // PS_EFF_HAMIL_HLDR_H
