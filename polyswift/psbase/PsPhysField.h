/**
 *
 * @file    PsPhysField.h
 *
 * @brief   Interface for physical "observable" fields
 *
 * @version $Id: PsPhysField.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_PHYS_FIELD_H
#define PS_PHYS_FIELD_H

// standard headers
#include <set>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#undef HAVE_CONFIG_H
#endif

// txbase headers
#include <TxDebugExcept.h>

// psbase includes
#include <PsDynObj.h>
#include <PsFieldBase.h>
#include <PsGridBase.h>
#include <PsCommBase.h>
#include <PsDecompBase.h>

#include <PsGridField.h>

/**
 * PsPhysField class is an interface to all of the "observable fields"
 * (e.g. monomer densites phi(r), charge density rho(r) etc.
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 */

template <class FLOATTYPE, size_t NDIM>
class PsPhysField : public virtual PsDynObj<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 */
    PsPhysField();

/**
 * Destructor
 */
    virtual ~PsPhysField();

/**
 * Parse attribute set
 *
 * @param tas the parameters
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas) = 0;

/**
 * Build the data, such as updaters, initial condition setters, ...
 */
    virtual void buildData() = 0;

/**
 * Build the solvers structures, fields etc.
 */
    virtual void buildSolvers() = 0;

/**
 * Initialize object
 */
    virtual void initialize() = 0;

/**
 * Reset object
 */
    virtual void resetDensField() = 0;

/**
 * Dump the object: scoping call from derived classes
 * will use this method
 *
 * @param txIoPtr IO object pointer
 */
    virtual void dump(TxIoBase* txIoPtr) = 0;

/**
 * Restores the field values on restart
 *

 * @param txIoPtr IO object pointer
 */
    virtual void restore(TxIoBase* txIoPtr) = 0;

/**
 * Update object
 */
    virtual void update(double t) = 0;

/**
 * Return reference to any density field object
 *
 * @return reference to field object
 */
    virtual PsFieldBase<FLOATTYPE>& getDensField() = 0;

/**
 * Return reference to any density field object
 *
 * @return reference to field object
 */
    virtual PsFieldBase<FLOATTYPE>& getConjgField() = 0;

/**
 * Return map string type of underlying fields
 *
 * @return string name of field type
 */
    virtual std::string getFieldType() = 0;

/**
 * Return rank of underlying fields
 *
 * @return integer rank of field
 */
    virtual size_t getFieldRank() = 0;

/**
 * Return size of underlying fields
 *
 * @return total number of elements in field
 */
    virtual size_t getFieldSize() = 0;

/**
 * Put name of interaction that this physical
 * field is a member into a list
 *
 * @param name interaction name
 */
    virtual void registerInteraction(std::string name) = 0;

/**
 * Get list of interaction names of which this physical field is a member
 *
 * @return vector of interaction names
 */
    virtual std::vector<std::string> getInteractionNames() = 0;

/**
 * Put name of block that this physical field is a member
 * into a list
 *
 * @param name block name
 */
    virtual void registerBlock(std::string name) = 0;

/**
 * Get list of block names of which this physical field is a member
 *
 * @return vector of block names
 */
    virtual std::vector<std::string> getBlockNames() = 0;

/**
 * Put name of solvent that this physical field is a member
 * into a list
 *
 * @param name block name
 */
    virtual void registerSolvent(std::string name) = 0;

/**
 * Get list of solvent names of which this physical field is a member
 *
 * @return vector of solvent names
 */
    virtual std::vector<std::string> getSolventNames() = 0;

/**
 * Get flag to check if field has been dumped at least once
 *
 * @return boolean dump check
 */
    virtual bool hasMadeOneDump() = 0;

/**
 * Get flag to set for only one dump
 *
 * @return boolean for one dump
 */
    virtual bool getOnlyFirstDump() = 0;

// ***********************************************************
//     This section is for methods that are implemented
//     in only some derived classes. Provides a single
//     place to provide error catching implementations.
// ***********************************************************

/**
 * Update specific pressure field members
 */
    virtual void updatePres() = 0;

/**
 * Return reference to Del[density] field object
 */
    virtual PsFieldBase<FLOATTYPE>& getDelDensField() = 0;

/**
 * Return reference to density field object shifted by monomer average
 */
    virtual PsFieldBase<FLOATTYPE>& getShiftedDensField() = 0;

/**
 * Get average observable density
 *
 * @return average observable density
 */
    virtual FLOATTYPE getDensAverage() = 0;

/**
 * Add to average observable density
 *
 * @param avg average observable density contribution
 */
    virtual void addToDensAverage(FLOATTYPE avg) = 0;

/**
 * Set average monomer density to zero
 */
    virtual void resetDensAverage() = 0;

/**
 * Calculate product of held physical fields
 *
 * @param dField reference for result field
 */
    virtual void calcFieldProd(PsFieldBase<FLOATTYPE>& dField) = 0;

/**
 * Get the volume of the field on local domain
 *
 * @return volume on a processor
 */
    virtual FLOATTYPE calcLocalVolume() = 0;

  protected:

  private:

    /** Make private to prevent use */
    PsPhysField(const PsPhysField<FLOATTYPE, NDIM>& vphh);

    /** Make private to prevent use */
    PsPhysField<FLOATTYPE, NDIM>& operator=(
        const PsPhysField<FLOATTYPE, NDIM>& vphh);
};
#endif  // PS_PHYS_FIELD_H
