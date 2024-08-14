/**
 * @file    PsPhysFldFuncs.h
 *
 * @brief   Intermeidate class for physical "observable" fields
 *
 * @version $Id: PsPhysFldFuncs.h 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_PHYS_FLD_FUNCS_H
#define PS_PHYS_FLD_FUNCS_H

// standard headers
#include <set>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsPhysField.h>

/**
 * PsPhysFldFuncs class holds common methods for all physfields
 * and error catches un-implmented methods
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 */

template <class FLOATTYPE, size_t NDIM>
class PsPhysFldFuncs : public virtual PsPhysField<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 *
 */
    PsPhysFldFuncs();

/**
 * Destructor
 */
    virtual ~PsPhysFldFuncs();

/**
 * Set the parameters
 *
 * @param tas the parameters
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
   Build the data
 */
    virtual void buildData();

/**
 * Build the solvers structures, fields etc.
 */
    virtual void buildSolvers();

/**
 * First step before updates
 */
//    virtual void initialize();

/**
 * Dump the object: scoping call from derived classes use this method
 *
 * @param txIoPtr IO object pointer
 */
    virtual void dump(TxIoBase* txIoPtr);

/**
 * Restores the field values on restart
 *
 * @param txIoPtr IO object pointer
 */
    virtual void restore(TxIoBase* txIoPtr);

/**
 * Put name of interaction that this physical field is a member
 * into a list
 *
 * @param name interaction name
 */
    virtual void registerInteraction(std::string name) {
      interactionMembers.push_back(name);
    }

/**
 * Get list of interaction names of which this physical field is a member
 *
 * @return vector of interaction names
 */
    virtual std::vector<std::string> getInteractionNames() {
      return interactionMembers;
    }

/**
 * Put name of block that this physical field is a member
 * into a list
 *
 * @param name block name
 */
    virtual void registerBlock(std::string name) {
      blockMembers.push_back(name);
    }

/**
 * Get list of block names of which this physical field is a member
 *
 * @return vector of block names
 */
    virtual std::vector<std::string> getBlockNames() {
      return blockMembers;
    }

/**
 * Put name of solvent that this physical field is a member
 * into a list
 *
 * @param name block name
 */
    virtual void registerSolvent(std::string name) {
      solventMembers.push_back(name);
    }

/**
 * Get list of solvent names of which this physical field is a member
 *
 * @return vector of solvent names
 */
    virtual std::vector<std::string> getSolventNames() {
      return solventMembers;
    }

/**
 * Get flag to check if field has been dumped at least once
 *
 * @return boolean dump check
 */
    virtual bool hasMadeOneDump() {
      return madeOneDump;
    }

/**
 * Get flag to set for only one dump
 *
 * @return boolean for one dump
 */
    virtual bool getOnlyFirstDump() {
      return onlyFirstDump;
    }

/**
 * Return type of underlying fields, delegates from grid
 *
 * @return string name of field type
 */
    virtual std::string getFieldType();

/**
 * Return rank of underlying fields, delegates from grid
 *
 * @return integer rank of field + 1 for data
 */
    virtual size_t getFieldRank();

/**
 * Return size of underlying fields, delegates from grid
 *
 * @return total number of elements in field
 */
    virtual size_t getFieldSize();

// ****************************************************************
//     This section is for methods that are implemented
//     in only some derived classes. Provides a single
//     place to provide error catching implementations.
// ****************************************************************

/**
 * Update specific pressure field members
 * Note: not in PsPhysField interface
 */
    virtual void updatePres() {
      TxDebugExcept tde("PsPhysFldFuncs::updatePres() not implemented");
      throw tde;
    }

/**
 * Return reference to Del[density] field object
 */
    virtual PsFieldBase<FLOATTYPE>& getDelDensField() {
      TxDebugExcept tde("PsPhysFldFuncs::getDelDensField not implemented");
      throw tde;
    }

/**
 * Return reference to density field object shifted by monomer average
 */
    virtual PsFieldBase<FLOATTYPE>& getShiftedDensField() {
      TxDebugExcept tde("PsPhysFldFuncs::getShiftedDensField not implemented");
      throw tde;
    }

/**
 * Get average observable density
 *
 * @return average observable density
 */
    virtual FLOATTYPE getDensAverage() {
      TxDebugExcept tde("PsPhysFldFuncs::getDensAverage not implemented");
      throw tde;
    }

/**
 * Add to average observable density
 *
 * @param avg average observable density contribution
 */
    virtual void addToDensAverage(FLOATTYPE avg) {
      TxDebugExcept tde("PsPhysFldFuncs::addToDensAverage not implemented");
      throw tde;
    }

/**
 * Set average monomer density to zero
 */
    virtual void resetDensAverage() {
      TxDebugExcept tde("PsPhysFldFuncs::resetDensAverage not implemented");
      throw tde;
    }

/**
 * Calculate product of held physical fields
 *
 * @param dField reference for result field
 */
    virtual void calcFieldProd(PsFieldBase<FLOATTYPE>& dField) {
      TxDebugExcept tde("PsPhysFldFuncs::calcFieldProd not implemented");
      throw tde;
    }

/**
 * Get the volume of the field on local domain
 *
 * @return volume on a processor
 */
    virtual FLOATTYPE calcLocalVolume() {
      TxDebugExcept tde("PsPhysFldFuncs::calcLocalVolume not implemented ");
      tde << "for field " << this->getName();
      throw tde;
    }

// ****************************************************************
// ****************************************************************

  protected:

    // SWS: separation for solvent and polymer/block object is
    // SWS: a problem that needs fixing

    /** Names of interactions that include this physical field */
    std::vector<std::string> interactionMembers;

    /** Names of copolymer blocks that include this physical field */
    std::vector<std::string> blockMembers;

    /** Names of copolymer blocks that include this physical field */
    std::vector<std::string> solventMembers;

    //
    // Data
    //

    /** Name of density field */
    std::string densFieldName;

    /** Name of conjugate field */
    std::string conjFieldName;

    /** Type of field */
    std::string fieldType;

/**
 * Helper method to format field data for dumping
 *
 * @param txIoPtr IO object pointer
 * @param fn file node
 * @param name string name of dataset
 * @param fld  reference to field data
 */
    void dumpField(TxIoBase* txIoPtr,
                   TxIoNodeType fn,
                   const std::string name,
                   PsFieldBase<FLOATTYPE>& fld);

/**
 * Helper method to format field data for inputing
 *
 * @param txIoPtr IO object pointer
 * @param fn file node
 * @param name string name of dataset
 * @param fld  reference to field data
 */
    void readField(TxIoBase* txIoPtr,
                   TxIoNodeType fn,
                   const std::string name,
                   PsFieldBase<FLOATTYPE>& fld);

/**
 * Append metadata to a dataset common to all PhysFldFuncss
 *
 * @param fn          file node
 * @param datasetName name string for dataset to append metadata
 */
    virtual void appendMetaDataset(TxIoBase* txIoPtr,
                                   TxIoNodeType fn,
                                   std::string datasetName);

/**
 * Append vizschema GROUP attributes
 *
 * @param fn file node
 */
    virtual void appendMetaGroup(TxIoBase* txIoPtr, TxIoNodeType fn);

 private:

/// IO data members global size
    std::vector<size_t> dataSize;

/// IO data members local data size
    std::vector<size_t> dataLen;

/// IO data members local data begin
    std::vector<size_t> dataBeg;

/// Variable to mimic NDIM (since all data is NDIM=3)
    size_t idim;

/// Flag to only dump at beginning of sim: default is off
    bool onlyFirstDump;

/// Flag to track if field has been dumped this sim
    bool madeOneDump;

/// Make private to prevent use
    PsPhysFldFuncs(const PsPhysFldFuncs<FLOATTYPE, NDIM>& vphh);

/// Make private to prevent use
    PsPhysFldFuncs<FLOATTYPE, NDIM>& operator=(
        const PsPhysFldFuncs<FLOATTYPE, NDIM>& vphh);
};
#endif  // PS_PHYS_FLD_FUNCS_H
