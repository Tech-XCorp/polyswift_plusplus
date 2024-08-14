/**
 *
 * @file    PsSolventHldr.h
 *
 * @brief   Base class owning instances of PsSolvent
 *
 * @version $Id: PsSolventHldr.h 6532 2007-02-01 18:37:00Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_SOLVENT_HLDR_H
#define PS_SOLVENT_HLDR_H

// std includes
#include <vector>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsDynObj.h>
#include <PsSolvent.h>

/**
 * PsSolventHldr contains a vector of pointers to PsSolvent objects
 *
 * @param FLOATTYPE the data type
 * @param NDIM the dimensionality
 */
template <class FLOATTYPE, size_t NDIM>
class PsSolventHldr : public PsDynObj<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 */
    PsSolventHldr() {
      this->setName("SolventHdlr");
    }

/**
 * Destructor
 */
    virtual ~PsSolventHldr() {
      for (size_t i=0; i<solvents.size(); ++i) {
        delete solvents[i];
      }
      solvents.clear();
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
 * Finish object
 */
    virtual void buildSolvers();

/**
 * Updates all PsSolvent-s
 *
 * @param t simulation time
 */
    virtual void update(double t);

/**
 * Dump all PsSolvent-s
 */
    virtual void dump();

  protected:

/** Number of solvents... ie length of the solvents vector */
    size_t numSolvents;

/** Vector of pointers to the base PsSolvent objects */
    std::vector< PsSolvent<FLOATTYPE, NDIM>* > solvents;

  private:

/** List of names for the solvents */
    std::vector< std::string > solventNames;

/** List of attrib sets for the solvents */
    std::vector< TxHierAttribSetIntDbl > solventAttribs;

/** Iterator for solvent list */
    typename std::vector< PsSolvent<FLOATTYPE, NDIM>* >::iterator isolvent;

/** Make private to prevent use */
    PsSolventHldr(const PsSolventHldr<FLOATTYPE, NDIM>& vphh);

/** Make private to prevent use */
    PsSolventHldr<FLOATTYPE, NDIM>& operator=(
        const PsSolventHldr<FLOATTYPE, NDIM>& vphh);
};

#endif // PS_SOLVENT_HLDR_H
