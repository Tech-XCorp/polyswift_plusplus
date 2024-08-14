/**
 * @file    PsPhysFieldHldr.h
 *
 * @brief   Base class owning instances of PsPhysField object(s)
 *
 * @version $Id: PsPhysFieldHldr.h 6532 2007-02-01 18:37:00Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_PHYS_FIELD_HLDR_H
#define PS_PHYS_FIELD_HLDR_H

// std includes
#include <vector>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsDynObj.h>
#include <PsPhysField.h>
#include <PsBlockBase.h>
#include <PsPolymer.h>
#include <PsSolvent.h>

/**
 * PsPhysFieldHldr contains a vector of pointers to PsPhysField objects
 *
 * @param FLOATTYPE the data type
 * @param NDIM the dimensionality
 */
template <class FLOATTYPE, size_t NDIM>
class PsPhysFieldHldr : public PsDynObj<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 */
    PsPhysFieldHldr() {
      this->setName("PhysFieldHdlr");
    }

/**
 * Destructor
 */
    virtual ~PsPhysFieldHldr() {

      // Clear and delete holder
      for (size_t i=0; i<physFields.size(); ++i) {
        delete physFields[i];
      }
      physFields.clear();

      // Clear names/attribs
      physFieldNames.clear();
      physFieldAttribs.clear();

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
 * Creates data for PhysFields
 */
    virtual void buildSolvers();

/**
 * Creates methods for PhysFields
 */
    virtual void buildData(PsNamedObject* inOwner);

/**
 * Initializes methods for PhysFields
 */
    virtual void initialize();

/**
 * Update state of all PhysFields
 */
    virtual void update(double t);

/**
 * Restores data for PhysFields on restarts
 *
 * @param txIoPtr the IO object
 */
    virtual void restore(TxIoBase* txIoPtr);

/**
 * Dump all PsPhysField-s
 *
 * @param txIoPtr the IO object
 */
    virtual void dump(TxIoBase* txIoPtr);

  protected:

    /** Number of physFields... ie length of the physFields vector */
    size_t numPhysFields;

    /** Vector of pointers to the base PsPhysField objects */
    std::vector< PsPhysField<FLOATTYPE, NDIM>* > physFields;

  private:

    /**
     * Checks lists and params
     */
    void check();

    /**
     * Checks member lists and total
     * volume fractions of monomer components
     *
     * @param  prtFlag turns on/off printing check info
     * @return total monomer density fraction (should be 1.0)
     */
    FLOATTYPE checkListsVolFrac(bool prtFlag);

    /**
     * Checks total frac then prints info (should be 1.0)
     *
     * @param tot monomer volume fraction
     */
    void checkSum(FLOATTYPE tot);

    /** List of names for the physFields */
    std::vector< std::string > physFieldNames;

    /** List of attrib sets for the physFields */
    std::vector< TxHierAttribSetIntDbl > physFieldAttribs;

    /** Polymer iterator */
    typename std::vector< PsPhysField<FLOATTYPE, NDIM>* >::iterator iphys;

    /** Make private to prevent use */
    PsPhysFieldHldr(const PsPhysFieldHldr<FLOATTYPE, NDIM>& vphh);

    /** Make private to prevent use */
    PsPhysFieldHldr<FLOATTYPE, NDIM>& operator=(
        const PsPhysFieldHldr<FLOATTYPE, NDIM>& vphh);

};

#endif // PS_PHYS_FIELD_HLDR_H
