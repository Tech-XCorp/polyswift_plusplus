/**
 *
 * @file    PsDomainBase.h
 *
 * @brief   To setup the simulation on individual processors.
 *
 * @version $Id: PsDomainBase.h 7789 2007-07-04 03:38:40Z kpaul $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_DOMAIN_BASE_H
#define PS_DOMAIN_BASE_H

// txbase includes
#include <TxTensor.h>

// psbase includes
#include <PsDynObjBase.h>

// psstd includes
#include <PsNamedObject.h>

/**
 *  -- untemplated class that defines the base class methods
 * of a PsDomain for any type or dimension
 */
class PsDomainBase : public PsNamedObject , public PsDynObjBase {

  public:

/**
 * Default constructor
 */
    PsDomainBase() : PsNamedObject("Domain") {
      //std::cerr << "PsDomainBase constructor entered." << std::endl;
    }

/**
 * Destructor
 */
    virtual ~PsDomainBase() {
    }

/**
 * Set base of output file names
 *
 * @param onm the base for output file names
 */
    virtual void setOutName(const std::string& onm) {
      outName = onm;
    }

/**
 * Set the attributes of this domain and create the bulk
 * of the domain singletons
 *
 * @param tha the hierarchical attribute set describing domain
 * @param restoreNum index of the files to use to restore
 * @param runName string of job name from input file
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tha,
        size_t restoreNum, std::string runName) = 0;

/**
 * Build the solvers for this domain.
 */
    virtual void buildSolvers() = 0;

/**
 * Build the data for this domain.
 */
    virtual void buildData() = 0;

/**
 * Initialize domain on first start-up.
 */
    virtual void initialize() = 0;

/**
 * Restore domain on restart
 */
    virtual void restore() = 0;

// Prevent use of copy constructor and assignment
    PsDomainBase(const PsDomainBase&) = delete;
    PsDomainBase& operator=(const PsDomainBase&) = delete;

  protected:

// The base name for output. File name is base_rank#_seq#.extention
    std::string outName;

// Indicates total number of timesteps for the simulation
    int nsteps;

  private:

};

#endif // PS_DOMAIN_BASE_H

