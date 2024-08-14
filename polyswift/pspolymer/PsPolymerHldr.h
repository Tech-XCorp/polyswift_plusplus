/**
 * @file    PsPolymerHldr.h
 *
 * @brief   Base class owning instances of PsPolymer
 *
 * @version $Id: PsPolymerHldr.h 6532 2007-02-01 18:37:00Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_POLYMER_HLDR_H
#define PS_POLYMER_HLDR_H

// std includes
#include <vector>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsDynObj.h>
#include <PsPolymer.h>

/**
 * PsPolymerHldr contains a vector of pointers to PsPolymer objects
 *
 * @param FLOATTYPE the data type
 * @param NDIM the dimensionality
 */
template <class FLOATTYPE, size_t NDIM>
class PsPolymerHldr : public PsDynObj<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 */
    PsPolymerHldr() {
      this->setName("PolymerHdlr");
    }

/**
 * Destructor
 */
    virtual ~PsPolymerHldr() {
      for (size_t i=0; i<polymers.size(); ++i) {
        delete polymers[i];
      }
      polymers.clear();
    }

/**
 * Set from an attribute set.
 *
 * @param tas the attribute set
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Build object 1st step
 */
    virtual void buildData(PsNamedObject* inOwner);

/**
 * Build object 2nd step
 */
    virtual void buildSolvers();

/**
 * Updates all PsPolymer-s
 *
 * @param t simulation time
 */
    virtual void update(double t);

/**
 * Dump all PsPolymer-s
 */
    virtual void dump();

/**
 * Get the sum of volume fraction for all polymers
 *
 * @return total volume fraction
 */
//    virtual FLOATTYPE sumVolFrac() {
//      FLOATTYPE totalVolFrac = 0.0;
//      for (ipoly = polymers.begin(); ipoly != polymers.end(); ++ipoly) {
//      totalVolFrac = totalVolFrac + (*ipoly)->getVolfrac();
//      }
//      return totalVolFrac;
//    }

  protected:

    /** Number of polymers... ie length of the polymers vector */
    size_t numPolymers;

    /** Vector of pointers to the base PsPolymer objects */
    std::vector< PsPolymer<FLOATTYPE, NDIM>* > polymers;

  private:

    /** List of names for the polymers */
    std::vector< std::string > polymerNames;

    /** List of attrib sets for the polymers */
    std::vector< TxHierAttribSetIntDbl > polymerAttribs;

    /** Iterator for polymer list */
    typename std::vector< PsPolymer<FLOATTYPE, NDIM>* >::iterator ipoly;

    /** Make private to prevent use */
    PsPolymerHldr(const PsPolymerHldr<FLOATTYPE, NDIM>& vphh);

    /** Make private to prevent use */
    PsPolymerHldr<FLOATTYPE, NDIM>& operator=(
        const PsPolymerHldr<FLOATTYPE, NDIM>& vphh);
};

#endif // PS_POLYMER_HLDR_H
